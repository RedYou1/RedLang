#pragma once
#include <string>
#include "Object.h"
#include "Command.h"
#include "Generic.h"
#include "Return.h"
#include "Boolean.h"
#include "Long.h"
#include "Global.h"
#include "MemoryFunction.h"
#include "Class.h"
#include "CastException.h"
#include "GarbageCollector.h"
#include "CastException.h"

namespace DTO {
	class ArrayO : public Object {
	public:
		IObject** m_value;

		ArrayO(Class* type, size_t size) :Object(type) {
			m_value = new IObject * [size];
			Class* obj{ GLOBAL::getClasses()->getClass(Paths::Object) };
			for (size_t i{ 0 }; i < size; i++) {
				m_value[i] = new NullObject(obj);
			}
		}
	private:
		ArrayO(Class* type, IObject** value) :Object(type) {
			m_value = value;
		}
	public:

		Object* clone()override {

			return new ArrayO(m_type, m_value);
		}
	};

	class Array : public GenericStatic {
	private:
		class ArrayC : public Class {
		public:
			Interface* m_type;
			ArrayC(std::string name, Interface* type);
		};
	public:
		Array() : GenericStatic("Array", Paths::Array, 1) {
		}

		SourceFile* create(std::string newName, Interface** gens, size_t genSize)override;

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayO* o{ (ArrayO*)mem.get("this") };
				ArrayO* c{ (ArrayO*)mem.get("c") };
				return new CommandReturn(new BooleanO(m_s, o->m_value == c->m_value), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class ArrayConstruct :public Command {
		public:
			ArrayC* m_s;
			ArrayConstruct(ArrayC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				Interface** i{ new Interface * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), m_s->getName() + ".Array", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get("toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;
				Object* c{ new ArrayO(m_s, (size_t)(((LongO*)q->getObject())->m_value)) };
				mem.set("this", c);
				delete q;
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ArrayConstruct(m_s); }
		};
		class Get : public Command {
		public:
			ArrayC* m_s;
			Get(ArrayC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				Interface** i{ new Interface * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), m_s->getName() + ".Get", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get("toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;
				ArrayO* arr{ (ArrayO*)mem.get("this") };
				IObject* arro{ arr->m_value[((NumberO*)q->getObject())->toLong()] };
				delete q;
				return new CommandReturn(arro, true, false);
			}
			Command* clone()override { return new Get(m_s); }
		};
		class Set : public Command {
		public:
			ArrayC* m_s;
			Set(ArrayC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				Interface** i{ new Interface * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), m_s->getName() + ".Set", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get("toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;
				ArrayO* arr{ (ArrayO*)mem.get("this") };
				GarbageCollector::Remove(arr->m_value[((NumberO*)q->getObject())->toLong()]);
				arr->m_value[((NumberO*)q->getObject())->toLong()] = mem.get("o");
				GarbageCollector::Add(arr->m_value[((NumberO*)q->getObject())->toLong()]);
				delete q;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new Set(m_s); }
		};
	};
}