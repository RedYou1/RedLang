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
#include "Collection.h"
#include "FunctionClass.h"

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
				CommandReturn* q{ a->exec("toLong", a) };
				Object* c{ new ArrayListO(m_s, (size_t)(((LongO*)q->getObject())->m_value)) };
				mem.set("this", c);
				delete q;
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ArrayConstructSize(m_s); }
		};
		class ArrayConstructCopy :public Command {
		private:
			class get :public Command {
			public:
				ArrayListO* m_array;
				size_t m_index;
				get(ArrayListO* array) :m_array(array), m_index(0) {}
				get(ArrayListO* array, size_t index) :m_array(array), m_index(index) {}
				CommandReturn* exec(MemoryObject& mem) override {
					IObject* a{ mem.get("c") };
					m_array->m_value[m_index] = a;
					m_index++;
					return new CommandReturn(new NullObject(), true, false);
				}
				Command* clone()override { return new get(m_array, m_index); }
			};
		public:
			ArrayListC* m_s;
			ArrayConstructCopy(ArrayListC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				CommandReturn* q{ a->exec("size", a) };
				size_t size{ (size_t)(((LongO*)q->getObject())->m_value) };
				ArrayListO* c{ new ArrayListO(m_s, size) };
				FunctionO* func{ new FunctionO((FunctionClass*)GLOBAL::getClasses()->getInterface(Paths::Function),
					new Function(new Signature("", nullptr, new Arg[1]{ m_s->m_type, "c" }, 1),
						new Command * [1]{ new get(c) }, 1)) };
				IObject** i{ new IObject * [2]{a,func} };
				CommandReturn* e{ a->exec("forEach", i, 2) };
				delete[] i;
				delete e;
				delete q;
				delete func->m_value;
				delete func;
				mem.set("this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new ArrayConstructCopy(m_s); }
		};

		class forEach : public Command {
		public:
			forEach() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* array{ (ArrayListO*)mem.get("this") };
				FunctionO* func{ (FunctionO*)mem.get("func") };
				size_t size{ array->m_value.size() };
				IObject** i{ new IObject * [1] };
				for (size_t c{ 0 }; c < size; c++) {
					MemoryObject mem{};
					i[0] = array->m_value[c];
					func->m_value->exec(mem, i, 1);
				}
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new forEach(); }
		};

		class add : public Command {
		public:
			add() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* a{ (ArrayListO*)mem.get("this") };
				IObject* b{ mem.get("c") };
				a->m_value.push_back(b);
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new add(); }
		};

		class addI : public Command {
		public:
			addI() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* a{ (ArrayListO*)mem.get("this") };
				IObject* i{ mem.get("i") };
				IObject* b{ mem.get("c") };
				CommandReturn* q{ i->exec("toLong",i) };
				size_t index{ (size_t)(((LongO*)q->getObject())->m_value) };
				delete q;
				size_t size{ a->m_value.size() };
				if (index > size) {
					return new CommandReturn(new IllegalArgumentExceptionO(GLOBAL::getClasses()->getClass(Paths::IllegalArgumentException), "index too big"), true, true);
				}
				a->m_value.insert(a->m_value.begin() + index, b);
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new addI(); }
		};

		class addAll : public Command {
		public:
			ArrayListC* m_s;
			FunctionO* m_func;
			addAll(ArrayListC* s) :m_s{ s },
				m_func{ new FunctionO((FunctionClass*)GLOBAL::getClasses()->getInterface(Paths::Function), new Function(
					new Signature("", nullptr,new Arg[2]{ m_s,"this", m_s->m_type,"c" }, 2), new Command * [1]{ new add() }, 1)) }{}
			~addAll() override { delete m_func->m_value; delete m_func; }
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* a{ (ArrayListO*)mem.get("this") };
				IObject* b{ mem.get("c") };
				IObject** ob{ new IObject * [2]{a,m_func} };
				CommandReturn* q{ b->exec("forEach",ob,2) };
				delete[] ob;
				delete q;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new addAll(m_s); }
		};

		class addAllI :public Command {
		private:
			class addInside :public Command {
			public:
				ArrayListO* m_array;
				size_t m_index;
				addInside(ArrayListO* array, size_t index) :m_array(array), m_index(index) {}
				CommandReturn* exec(MemoryObject& mem) override {
					IObject* a{ mem.get("c") };
					m_array->m_value[m_index] = a;
					m_index++;
					return new CommandReturn(new NullObject(), true, false);
				}
				Command* clone()override { return new addInside(m_array, m_index); }
			};
		public:
			ArrayListC* m_s;
			addAllI(ArrayListC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* array{ (ArrayListO*)mem.get("this") };
				IObject* a{ mem.get("c") };
				IObject* i{ mem.get("i") };
				CommandReturn* q{ a->exec("size", a) };
				size_t size{ (size_t)(((LongO*)q->getObject())->m_value) };
				delete q;
				q = i->exec("toLong", i);
				size_t index{ (size_t)(((LongO*)q->getObject())->m_value) };
				delete q;

				array->m_value.reserve(array->m_value.size() + size);
				memmove(&array->m_value[index + size], &array->m_value[index], size);

				FunctionO* func{
				new FunctionO((FunctionClass*)GLOBAL::getClasses()->getInterface(Paths::Function),
					new Function(new Signature("", nullptr, new Arg[1]{ m_s->m_type, "c" }, 1),
						new Command * [1]{ new addInside(array,index) }, 1))
				};
				IObject** ob{ new IObject * [2]{a,func} };
				CommandReturn* e{ a->exec("forEach", ob, 2) };
				delete[] ob;
				delete e;
				delete func->m_value;
				delete func;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new addAllI(m_s); }
		};

		class get : public Command {
		public:
			get() {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				CommandReturn* q{ a->exec("toLong", a) };
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				IObject* arro{ arr->m_value[((NumberO*)q->getObject())->toLong()] };
				delete q;
				return new CommandReturn(arro, true, false);
			}
			Command* clone()override { return new get(); }
		};
		class set : public Command {
		public:
			set() {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				CommandReturn* q{ a->exec("toLong", a) };
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				GarbageCollector::Remove(arr->m_value[((NumberO*)q->getObject())->toLong()]);
				arr->m_value[((NumberO*)q->getObject())->toLong()] = mem.get("o");
				GarbageCollector::Add(arr->m_value[((NumberO*)q->getObject())->toLong()]);
				delete q;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new set(); }
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