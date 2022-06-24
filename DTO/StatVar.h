#pragma once
#include "Class.h"
#include "Command.h"
#include "GarbageCollector.h"

namespace DTO {
	class StatVar {
	private:
		Interface* m_type;
		IObject* m_value;
	public:
		StatVar(Interface* type, IObject* value) :m_type(type), m_value(value) { GarbageCollector::Add(m_value); }
		~StatVar() { GarbageCollector::Remove(m_value); }

		Interface* GetType() { return m_type; }

		IObject* GetValue() { return m_value; }

		void SetValue(IObject* value) {
			GarbageCollector::Remove(m_value);
			m_value = value;
			GarbageCollector::Add(m_value);
		}
	};
}