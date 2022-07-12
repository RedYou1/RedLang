#pragma once
#include <string>
#include "MemoryObject.h"
#include "Object.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"

#include "Global.h"

namespace DTO {
	class BooleanC;

	class StringO : public Object {
	public:
		std::wstring m_value;

		StringO(Class* type) :Object(type) {
			m_value = L"";
		}

		StringO(Class* type, std::wstring value) :Object(type) {
			m_value = value;
		}

		Object* clone()override { return new StringO(m_type, m_value); }
	};

	class StringC : public Class {
	public:
		StringC() : Class(L"String", Paths::String, GLOBAL::getClasses()->getClass(Paths::Object)) {
		}

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Equals(m_s); }
		};
		class CharConstruct :public Command {
		public:
			StringC* m_s;
			CharConstruct(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new CharConstruct(m_s); }
		};
		class StringConstruct :public Command {
		public:
			StringC* m_s;
			StringConstruct(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new StringConstruct(m_s); }
		};

		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				return new CommandReturn(mem.get(L"this"), true, false);
			}
			Command* clone()override { return new ToString(m_s); }
		};
		class Concat :public Command {
		public:
			StringC* m_s;
			Concat(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				StringO* a{ (StringO*)mem.get(L"this") };
				IObject* b{ mem.get(L"other") };

				CommandReturn* q{ b->exec(L"toString",b) };

				a->m_value += ((StringO*)q->getObject())->m_value;

				delete q;
				return new CommandReturn(a, true, false);
			}
			Command* clone()override { return new Concat(m_s); }
		};
	};
}