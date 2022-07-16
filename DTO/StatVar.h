#pragma once
#include "Class.h"
#include "Command.h"
#include "Object.h"

namespace DTO {
	class StatVar {
	private:
		Instanciable* m_type;
		IObject* m_value;
	public:
		StatVar(Instanciable* type, IObject* value) :m_type(type), m_value(value) { m_value->addRef(); }
		~StatVar() { m_value->removeRef(); }

		Instanciable* GetType() { return m_type; }

		IObject* GetValue() { return m_value; }

		void SetValue(IObject* value) {
			m_value->removeRef();
			m_value = value;
			m_value->addRef();
		}
	};
}