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
		CastExceptionO(Class* type, std::string message, CommandReturn* from, Interface* to)
			: ExceptionO(type, message + "{from:" + from->getObject()->getClass()->getName() + ",to:" + to->getName() + "}") {
			delete from;
		}
	};

	class CastExceptionC : public Class {
	public:
		CastExceptionC() : Class("CastException", "", GLOBAL::getClasses()->getClass(Paths::Exception)) {
		}

		class ExceptionConstruct :public Command {
		public:
			CastExceptionC* m_s;
			ExceptionConstruct(CastExceptionC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				StringO* a{ (StringO*)mem.get("c") };
				Object* c{ new ExceptionO(m_s, a->m_value) };
				mem.set("this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ExceptionConstruct(m_s); }
		};
	};
}