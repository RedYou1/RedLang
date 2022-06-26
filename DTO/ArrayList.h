#pragma once
#include <string>
#include <vector>
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
	class ArrayListO : public Object {
	public:
		std::vector<IObject*> m_value;

		ArrayListO(Class* type) :Object(type), m_value() {}
		ArrayListO(Class* type, size_t size) :Object(type), m_value(size) {}
		ArrayListO(Class* type, std::vector<IObject*> value) :Object(type), m_value(value) {
			for (IObject* var : m_value)
			{
				GarbageCollector::Add(var);
			}
		}

		Object* clone()override {
			return new ArrayListO(m_type, m_value);
		}
	};

	class ArrayList : public Generic {
	private:
		class ArrayListC : public Class {
		public:
			Interface* m_type;
			ArrayListC(std::string name, Interface* type);
		};
	public:
		ArrayList() : Generic("ArrayList", Paths::ArrayList) {
		}

		SourceFile* create(std::string newName, SourceFile** gens, size_t genSize)override;

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* o{ (ArrayListO*)mem.get("this") };
				ArrayListO* c{ (ArrayListO*)mem.get("c") };
				return new CommandReturn(new BooleanO(m_s, o->m_value == c->m_value), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class ArrayConstructEmpty :public Command {
		public:
			ArrayListC* m_s;
			ArrayConstructEmpty(ArrayListC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				Object* c{ new ArrayListO(m_s) };
				mem.set("this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ArrayConstructEmpty(m_s); }
		};
		class ArrayConstructSize :public Command {
		public:
			ArrayListC* m_s;
			ArrayConstructSize(ArrayListC* s) :m_s(s) {}
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
				Object* c{ new ArrayListO(m_s, (size_t)(((LongO*)q->getObject())->m_value)) };
				mem.set("this", c);
				delete q;
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ArrayConstructSize(m_s); }
		};
		class ArrayConstructCopy :public Command {
		public:
			ArrayListC* m_s;
			ArrayConstructCopy(ArrayListC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* a{ (ArrayListO*)mem.get("c") };
				Object* c{ new ArrayListO(m_s,a->m_value) };
				mem.set("this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ArrayConstructCopy(m_s); }
		};
		class Get : public Command {
		public:
			ArrayListC* m_s;
			Get(ArrayListC* s) :m_s(s) {}
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
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				IObject* arro{ arr->m_value[((NumberO*)q->getObject())->toLong()] };
				delete q;
				return new CommandReturn(arro, true, false);
			}
			Command* clone()override { return new Get(m_s); }
		};
		class Set : public Command {
		public:
			ArrayListC* m_s;
			Set(ArrayListC* s) :m_s(s) {}
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
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				GarbageCollector::Remove(arr->m_value[((NumberO*)q->getObject())->toLong()]);
				arr->m_value[((NumberO*)q->getObject())->toLong()] = mem.get("o");
				GarbageCollector::Add(arr->m_value[((NumberO*)q->getObject())->toLong()]);
				delete q;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new Set(m_s); }
		};
		class PushBack :public Command {
		public:
			PushBack() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* a{ (ArrayListO*)mem.get("this") };
				IObject* b{ mem.get("c") };
				a->m_value.push_back(b);
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new PushBack(); }
		};
		class PopBack :public Command {
		public:
			PopBack() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* a{ (ArrayListO*)mem.get("this") };
				a->m_value.pop_back();
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new PopBack(); }
		};
		class Size :public Command {
		public:
			LongC* m_s;
			Size(LongC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				return new CommandReturn(new LongO(m_s, (int64_t)arr->m_value.size()), true, false);
			}
			Command* clone()override { return new Size(m_s); }
		};
		class Resize :public Command {
		public:
			Resize() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				IObject* a{ mem.get("c") };

				Interface** i{ new Interface * [1]{GLOBAL::getClasses()->getInterface(Paths::Number)} };
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

				for (uint64_t i{ newSize }; i < arr->m_value.size(); i++) {
					GarbageCollector::Remove(arr->m_value[i]);
				}
				uint64_t lastSize{ arr->m_value.size() };
				arr->m_value.resize(newSize);
				Class* obj{ GLOBAL::getClasses()->getClass(Paths::Object) };
				for (uint64_t i{ lastSize }; i < newSize; i++) {
					arr->m_value[i] = new NullObject(obj);
				}
				delete q;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new Resize(); }
		};
	};
}