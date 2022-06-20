#include "GarbageCollector.h"

std::mutex GarbageCollector::_mutex{};
size_t GarbageCollector::_isRunning{ 0 };
std::thread::id GarbageCollector::_current_id{ std::this_thread::get_id() };
std::lock_guard<std::mutex>* GarbageCollector::_lock{ nullptr };
std::map<Object*, size_t> GarbageCollector::_commands{};

void GarbageCollector::lock() {
	if (_isRunning == 0 || _current_id != std::this_thread::get_id()) {
		_lock = new std::lock_guard<std::mutex>(_mutex);
		_current_id = std::this_thread::get_id();
	}
	_isRunning++;
}

void GarbageCollector::unLock() {
	_isRunning--;
	if (_isRunning == 0)
		delete _lock;
}

void GarbageCollector::Add(Object* cmd)
{
	if (cmd == nullptr)
		return;
	lock();

	std::map<Object*, size_t>::iterator it{ _commands.find(cmd) };
	if (it == _commands.end())
		_commands.insert(std::pair<Object*, size_t>(cmd, 1));
	else
		it->second++;

	unLock();
}

void GarbageCollector::Remove(Object* cmd)
{
	if (cmd == nullptr)
		return;
	lock();

	std::map<Object*, size_t>::iterator it{ _commands.find(cmd) };
	if (it == _commands.end())
		throw "not found";
	if (it->second <= 1) {
		_commands.erase(cmd);
		delete cmd;
	}
	else
		it->second--;

	unLock();
}

void GarbageCollector::Clear()
{
	lock();
	for (std::map<Object*, size_t>::iterator it{ _commands.begin() }; it != _commands.end(); it++) {
		delete it->first;
	}
	unLock();
}

size_t GarbageCollector::GetAmount()
{
	lock();
	size_t size{ _commands.size() };
	unLock();
	return size;
}

size_t GarbageCollector::GetAmountRef(Object* cmd)
{
	if (cmd == nullptr)
		return 0;
	lock();
	std::map<Object*, size_t>::iterator it{ _commands.find(cmd) };
	bool _throw{ it == _commands.end() };
	size_t amount{ _throw ? 0 : it->second };
	unLock();

	if (_throw)
		throw "not found";
	return amount;
}
