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
	class NullExceptionO : public ExceptionO {
	public:
		NullExceptionO(Class* type, std::string message)
			: ExceptionO(type, message) {
		}
	};

	class NullException : public Class {
	public:
		NullException() : Class("NullException", Paths::NullException, GLOBAL::getClasses()->getClass(Paths::Exception)) {
		}

		class ExceptionConstruct :public Command {
		public:
			NullException* m_s;
			ExceptionConstruct(NullException* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				StringO* a{ (StringO*)mem.get("c") };
				Object* c{ new ExceptionO(m_s, a->m_value) };
				mem.set("this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone() override { return new ExceptionConstruct(m_s); }
		};
	};
}