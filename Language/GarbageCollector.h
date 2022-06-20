#pragma once
#include <map>
#include <mutex>
#include "Object.h"

class GarbageCollector {
private:
	static std::mutex _mutex;
	static size_t _isRunning;
	static std::thread::id _current_id;
	static std::lock_guard<std::mutex>* _lock;
	static std::map<Object*, size_t> _commands;

	static void lock();
	static void unLock();
public:
	static void Add(Object* cmd);
	static void Remove(Object* cmd);
	static void Clear();
	static size_t GetAmount();
	static size_t GetAmountRef(Object* cmd);
};