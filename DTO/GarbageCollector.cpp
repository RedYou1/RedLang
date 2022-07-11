#include "GarbageCollector.h"

std::mutex DTO::GarbageCollector::_mutex{};
size_t DTO::GarbageCollector::_isRunning{ 0 };
std::thread::id DTO::GarbageCollector::_current_id{ std::this_thread::get_id() };
std::lock_guard<std::mutex>* DTO::GarbageCollector::_lock{ nullptr };
std::map<DTO::IObject*, size_t> DTO::GarbageCollector::_commands{};

void DTO::GarbageCollector::lock() {
	if (_isRunning == 0 || _current_id != std::this_thread::get_id()) {
		_lock = new std::lock_guard<std::mutex>(_mutex);
		_current_id = std::this_thread::get_id();
	}
	_isRunning++;
}

void DTO::GarbageCollector::unLock() {
	_isRunning--;
	if (_isRunning == 0)
		delete _lock;
}

void DTO::GarbageCollector::Add(IObject* cmd)
{
	if (cmd == nullptr)
		return;
	lock();

	std::map<IObject*, size_t>::iterator it{ _commands.find(cmd) };
	if (it == _commands.end())
		_commands.insert(std::pair<IObject*, size_t>(cmd, 1));
	else
		it->second++;

	unLock();
}

void DTO::GarbageCollector::Remove(IObject* cmd)
{
	if (cmd == nullptr)
		return;
	lock();

	std::map<IObject*, size_t>::iterator it{ _commands.find(cmd) };
	if (it == _commands.end())
		return;
	if (it->second <= 1) {
		_commands.erase(cmd);
		delete cmd;
	}
	else
		it->second--;

	unLock();
}

void DTO::GarbageCollector::Clear()
{
	lock();
	for (std::map<IObject*, size_t>::iterator it{ _commands.begin() }; it != _commands.end(); it++) {
		delete it->first;
	}
	unLock();
}

size_t DTO::GarbageCollector::GetAmount()
{
	lock();
	size_t size{ _commands.size() };
	unLock();
	return size;
}

size_t DTO::GarbageCollector::GetAmountRef(IObject* cmd)
{
	if (cmd == nullptr)
		return 0;
	lock();
	std::map<IObject*, size_t>::iterator it{ _commands.find(cmd) };
	bool _throw{ it == _commands.end() };
	size_t amount{ _throw ? 0 : it->second };
	unLock();

	if (_throw)
		throw "not found";
	return amount;
}
