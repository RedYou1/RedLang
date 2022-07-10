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
	class SizedArrayO : public Object {
	public:
		IObject** m_value;
		size_t m_size;

		SizedArrayO(Class* type, size_t size) :Object(type), m_size(size) {
			m_value = new IObject * [m_size];
			Class* obj{ GLOBAL::getClasses()->getClass(Paths::Object) };
			for (size_t i{ 0 }; i < m_size; i++) {
				m_value[i] = new NullObject(obj);
			}
		}
	private:
		SizedArrayO(Class* type, IObject** value, size_t size) :Object(type), m_value(value), m_size(size) {
		}
	public:

		Object* clone()override {
			return new SizedArrayO(m_type, m_value, m_size);
		}
	};

	class SizedArray : public GenericStatic {
	private:
		class SizedArrayC : public Class {
		public:
			Instanciable* m_type;
			SizedArrayC(std::string name, Instanciable* type);
		};
	public:
		SizedArray() : GenericStatic("SizedArray", Paths::SizedArray, 1) {
		}

		SourceFile* create(std::string newName, Instanciable** gens, size_t genSize)override;

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				SizedArrayO* o{ (SizedArrayO*)mem.get("this") };
				SizedArrayO* c{ (SizedArrayO*)mem.get("c") };
				return new CommandReturn(new BooleanO(m_s, o->m_value == c->m_value), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class ArrayConstruct :public Command {
		public:
			SizedArrayC* m_s;
			ArrayConstruct(SizedArrayC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				Instanciable** i{ new Instanciable * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), m_s->getName() + ".Array", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get("toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;
				Object* c{ new SizedArrayO(m_s, (size_t)(((LongO*)q->getObject())->m_value)) };
				mem.set("this", c);
				delete q;
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ArrayConstruct(m_s); }
		};
		class Get : public Command {
		public:
			SizedArrayC* m_s;
			Get(SizedArrayC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				Instanciable** i{ new Instanciable * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), m_s->getName() + ".Get", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get("toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;
				SizedArrayO* arr{ (SizedArrayO*)mem.get("this") };
				IObject* arro{ arr->m_value[((NumberO*)q->getObject())->toLong()] };
				delete q;
				return new CommandReturn(arro, true, false);
			}
			Command* clone()override { return new Get(m_s); }
		};
		class Set : public Command {
		public:
			SizedArrayC* m_s;
			Set(SizedArrayC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				Instanciable** i{ new Instanciable * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), m_s->getName() + ".Set", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get("toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;
				SizedArrayO* arr{ (SizedArrayO*)mem.get("this") };
				GarbageCollector::Remove(arr->m_value[((NumberO*)q->getObject())->toLong()]);
				arr->m_value[((NumberO*)q->getObject())->toLong()] = mem.get("o");
				GarbageCollector::Add(arr->m_value[((NumberO*)q->getObject())->toLong()]);
				delete q;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new Set(m_s); }
		};
		class Size :public Command {
		public:
			LongC* m_s;
			Size(LongC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				SizedArrayO* arr{ (SizedArrayO*)mem.get("this") };
				return new CommandReturn(new LongO(m_s, (int64_t)arr->m_size), true, false);
			}
			Command* clone()override { return new Size(m_s); }
		};
		class Resize :public Command {
		public:
			Resize() {}
			CommandReturn* exec(MemoryObject& mem) override {
				SizedArrayO* arr{ (SizedArrayO*)mem.get("this") };
				IObject* a{ mem.get("c") };

				Instanciable** i{ new Instanciable * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), arr->getClass()->getName() + ".Resize", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get("toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;

				uint64_t newSize{ (uint64_t)((LongO*)q->getObject())->m_value };

				for (uint64_t i{ newSize }; i < arr->m_size; i++) {
					GarbageCollector::Remove(arr->m_value[i]);
				}
				IObject** newArr{ new IObject * [newSize] };
				memcpy(newArr, arr->m_value, std::min(arr->m_size, newSize) * sizeof(IObject*));
				Class* obj{ GLOBAL::getClasses()->getClass(Paths::Object) };
				for (uint64_t i{ arr->m_size }; i < newSize; i++) {
					newArr[i] = new NullObject(obj);
				}
				delete[] arr->m_value;
				arr->m_value = newArr;
				arr->m_size = newSize;
				delete q;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new Resize(); }
		};
	};
}