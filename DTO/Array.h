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
#include "FunctionClass.h"

namespace DTO {
	class ArrayO : public Object {
	public:
		IObject** m_value;
		size_t m_size;

		ArrayO(Class* type, size_t size) :Object(type), m_size(size) {
			m_value = new IObject * [m_size];
		}

		~ArrayO() override {
			for (size_t i{ 0 }; i < m_size; i++) {
				GarbageCollector::Remove(m_value[i]);
			}
			delete[] m_value;
		}
	private:
		ArrayO(Class* type, IObject** value, size_t size) :Object(type), m_value(value), m_size(size) {
		}
	public:

		Object* clone()override {
			IObject** o{ new IObject * [m_size] };
			for (size_t i{ 0 }; i < m_size; i++) {
				o[i] = m_value[i];
				GarbageCollector::Add(o[i]);
			}
			return new ArrayO(m_type, o, m_size);
		}
	};

	class Array : public GenericStatic {
	public:
		Array() : GenericStatic(L"Array", Paths::Array, 1) {
		}

		SourceFile* create(std::wstring newName, Instanciable** gens, size_t genSize)override;

		class ArrayC : public Class {
		public:
			Instanciable* m_type;
			ArrayC(std::wstring name, Instanciable* type);
		};

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayO* o{ (ArrayO*)mem.get(L"this") };
				ArrayO* c{ (ArrayO*)mem.get(L"c") };
				return new CommandReturn(new BooleanO(m_s, o->m_value == c->m_value), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class ArrayConstruct :public Command {
		public:
			ArrayC* m_s;
			ArrayConstruct(ArrayC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get(L"c") };
				Instanciable** i{ new Instanciable * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), m_s->getName() + L".Array", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get(L"toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;
				ArrayO* c{ new ArrayO(m_s, (size_t)(((LongO*)q->getObject())->m_value)) };
				Class* obj{ GLOBAL::getClasses()->getClass(Paths::Object) };
				for (size_t i{ 0 }; i < c->m_size; i++) {
					c->m_value[i] = new NullObject(obj);
					GarbageCollector::Add(c->m_value[i]);
				}
				mem.set(L"this", c);
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
				IObject* a{ mem.get(L"c") };
				Instanciable** i{ new Instanciable * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), m_s->getName() + L".Get", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get(L"toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;
				ArrayO* arr{ (ArrayO*)mem.get(L"this") };
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
				IObject* a{ mem.get(L"c") };
				Instanciable** i{ new Instanciable * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), m_s->getName() + L".Set", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get(L"toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;
				ArrayO* arr{ (ArrayO*)mem.get(L"this") };
				int64_t index{ ((LongO*)q->getObject())->m_value };
				GarbageCollector::Remove(arr->m_value[index]);
				arr->m_value[index] = mem.get(L"o");
				GarbageCollector::Add(arr->m_value[index]);
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
				ArrayO* arr{ (ArrayO*)mem.get(L"this") };
				return new CommandReturn(new LongO(m_s, (int64_t)arr->m_size), true, false);
			}
			Command* clone()override { return new Size(m_s); }
		};
		class Resize :public Command {
		public:
			Resize() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayO* arr{ (ArrayO*)mem.get(L"this") };
				IObject* a{ mem.get(L"c") };

				Instanciable** i{ new Instanciable * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
				if (!a->getClass()->instanceOf(i[0])) {
					delete[] i;
					return new CommandReturn(new CastExceptionO(GLOBAL::getClasses()->getClass(Paths::CastException), arr->getClass()->getName() + L".Resize", new CommandReturn(a, false, false), GLOBAL::getClasses()->getInterface(Paths::Number)), false, true);
				}
				IObject** o{ new IObject * [1]{a} };
				MemoryObject mem2{};
				CommandReturn* q{ a->getClass()->getFuncs()->get(L"toLong", i, 1)->exec(mem2, o, 1) };
				delete[] i;
				delete[] o;

				uint64_t newSize{ (uint64_t)((LongO*)q->getObject())->m_value };

				for (uint64_t i{ newSize }; i < arr->m_size; i++) {
					GarbageCollector::Remove(arr->m_value[i]);
				}
				IObject** newArr{ new IObject * [newSize] };
				memcpy(newArr, arr->m_value, std::min(arr->m_size, newSize) * sizeof(IObject*));
				Class* obj{ GLOBAL::getClasses()->getClass(Paths::Object) };
				for (uint64_t i{ newSize }; i < arr->m_size; i++) {
					GarbageCollector::Remove(arr->m_value[i]);
				}
				for (uint64_t i{ arr->m_size }; i < newSize; i++) {
					newArr[i] = new NullObject(obj);
					GarbageCollector::Add(newArr[i]);
				}
				delete[] arr->m_value;
				arr->m_value = newArr;
				arr->m_size = newSize;
				delete q;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new Resize(); }
		};

		class forEach : public Command {
		public:
			forEach() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayO* array{ (ArrayO*)mem.get(L"this") };
				FunctionO* func{ (FunctionO*)mem.get(L"func") };
				size_t size{ array->m_size };
				IObject** i{ new IObject * [1] };
				for (size_t c{ 0 }; c < size; c++) {
					MemoryObject mem{};
					i[0] = array->m_value[c];
					delete func->m_value->exec(mem, i, 1);
				}
				delete[] i;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new forEach(); }
		};
	};
}