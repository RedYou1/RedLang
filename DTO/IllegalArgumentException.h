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
	class IllegalArgumentExceptionO : public ExceptionO {
	public:
		IllegalArgumentExceptionO(Class* type, std::string message)
			: ExceptionO(type, message) {
		}
	};

	class IllegalArgumentExceptionC : public Class {
	public:
		IllegalArgumentExceptionC() : Class("IllegalArgumentException", Paths::IllegalArgumentException, GLOBAL::getClasses()->getClass(Paths::Exception)) {
		}

		class ExceptionConstruct :public Command {
		public:
			IllegalArgumentExceptionC* m_s;
			ExceptionConstruct(IllegalArgumentExceptionC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				StringO* a{ (StringO*)mem.get("c") };
				Object* c{ new ExceptionO(m_s,a->m_value) };
				mem.set("this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ExceptionConstruct(m_s); }
		};
	};
}