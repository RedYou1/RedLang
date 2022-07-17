#pragma once
#include <string>
#include <sstream>

#include "Object.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "String.h"
#include "Boolean.h"


namespace DTO {
	class ClassClass;
	class LongC;

	class ObjectClass : public Class {
	public:
		ObjectClass() : Class(L"Object", Paths::Object, nullptr) {
		}

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* o{ mem.get(L"this") };
				IObject* c{ mem.get(L"c") };
				return new CommandReturn(new BooleanO(m_s, o == c), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* o{ mem.get(L"this") };
				std::wstringstream ss;


				ss << o->getClass()->getName() << "@";
				if (dynamic_cast<NullObject*>(o) != nullptr) {
					ss << "NULL";
				}
				else {
					ss << std::hex << (void*)o;
				}

				return new CommandReturn(new StringO(m_s, ss.str()), true, false);
			}
			Command* clone()override { return new ToString(m_s); }
		};
		class GetClass :public Command {
		public:
			ClassClass* m_s;
			GetClass(ClassClass* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new GetClass(m_s); }
		};
		class HashCode :public Command {
		public:
			LongC* m_s;
			HashCode(LongC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new HashCode(m_s); }
		};

		class GetPtr :public Command {
		public:
			LongC* m_s;
			GetPtr(LongC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new GetPtr(m_s); }
		};
	};
}