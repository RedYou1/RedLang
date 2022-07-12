#pragma once
#include <string>

#include "Object.h"
#include "String.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Boolean.h"
#include "Exception.h"
#include "Global.h"

namespace DTO {
	class CastExceptionO : public ExceptionO {
	public:
		CastExceptionO(Class* type, std::wstring message, CommandReturn* from, Instanciable* to)
			: ExceptionO(type, message + L"{from:" + from->getObject()->getClass()->getName() + L",to:" + to->getName() + L"}") {
			delete from;
		}
	};

	class CastExceptionC : public Class {
	public:
		CastExceptionC() : Class(L"CastException", Paths::CastException, GLOBAL::getClasses()->getClass(Paths::Exception)) {
		}

		class ExceptionConstruct :public Command {
		public:
			CastExceptionC* m_s;
			ExceptionConstruct(CastExceptionC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				StringO* a{ (StringO*)mem.get(L"c") };
				Object* c{ new ExceptionO(m_s, a->m_value) };
				mem.set(L"this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ExceptionConstruct(m_s); }
		};
	};
}