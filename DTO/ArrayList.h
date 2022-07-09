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
#include "ClassClass.h"
#include "FunctionClass.h"
#include "Array.h"

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

		class indexOf : public Command {
		public:
			indexOf() {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				for (size_t i{ 0 }; i < arr->m_value.size(); i++) {
					IObject** args{ new IObject * [2]{arr->m_value[i],a} };
					CommandReturn* q{ arr->m_value[i]->exec("equals",args,2) };
					if (q->isThrow())
						return q;
					bool r{ ((BooleanO*)q->getObject())->m_value };
					delete[] args;
					delete q;
					if (r)
						return new CommandReturn(new LongO(GLOBAL::getClasses()->getClass(Paths::Long), i), true, false);
				}
				return new CommandReturn(new LongO(GLOBAL::getClasses()->getClass(Paths::Long), -1), true, false);
			}
			Command* clone()override { return new indexOf(); }
		};

		class lastIndexOf : public Command {
		public:
			lastIndexOf() {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				for (size_t i{ arr->m_value.size() };; i--) {
					IObject** args{ new IObject * [2]{arr->m_value[i],a} };
					CommandReturn* q{ arr->m_value[i]->exec("equals",args,2) };
					if (q->isThrow())
						return q;
					bool r{ ((BooleanO*)q->getObject())->m_value };
					delete[] args;
					delete q;
					if (r)
						return new CommandReturn(new LongO(GLOBAL::getClasses()->getClass(Paths::Long), i), true, false);
					if (i == 0)
						break;
				}
				return new CommandReturn(new LongO(GLOBAL::getClasses()->getClass(Paths::Long), -1), true, false);
			}
			Command* clone()override { return new lastIndexOf(); }
		};

		class clear : public Command {
		public:
			clear() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				arr->m_value.clear();
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new clear(); }
		};

		class contains : public Command {
		public:
			ArrayListO* m_arr{ nullptr };
			contains() {}
			contains(ArrayListO* arr) :m_arr{ arr } {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				ArrayListO* arr;
				if (m_arr == nullptr)
					arr = (ArrayListO*)mem.get("this");
				else
					arr = m_arr;

				for (size_t i{ 0 }; i < arr->m_value.size(); i++) {
					IObject** args{ new IObject * [2]{arr->m_value[i],a} };
					CommandReturn* q{ arr->m_value[i]->exec("equals",args,2) };
					if (q->isThrow())
						return q;
					bool r{ ((BooleanO*)q->getObject())->m_value };
					delete[] args;
					delete q;
					if (r)
						return new CommandReturn(new BooleanO(GLOBAL::getClasses()->getClass(Paths::Boolean), true), true, false);
				}
				return new CommandReturn(new BooleanO(GLOBAL::getClasses()->getClass(Paths::Boolean), false), true, false);
			}
			Command* clone()override { return new contains(); }
		};

		class containsAll : public Command {
		public:
			containsAll() {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };

				FunctionO* func{ new FunctionO(GLOBAL::getClasses()->getClass(Paths::Function),
					new Function(new Signature("",GLOBAL::getClasses()->getClass(Paths::Boolean),
						new Arg[1]{GLOBAL::getClasses()->getClass(Paths::Object),"c"},
							2),new Command * [1]{new contains(arr)},1)) };

				IObject** args{ new IObject * [2]{a,func} };


				CommandReturn* q{ a->exec("forEach", args, 2) };
				if (q->isThrow())
					return q;
				bool r{ ((BooleanO*)q->getObject())->m_value };
				delete[] args;
				delete q;
				return new CommandReturn(new BooleanO(GLOBAL::getClasses()->getClass(Paths::Boolean), r), true, false);
			}
			Command* clone()override { return new containsAll(); }
		};

		class isEmpty : public Command {
		public:
			isEmpty() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };

				return new CommandReturn(new BooleanO(GLOBAL::getClasses()->getClass(Paths::Boolean), arr->m_value.empty()), true, false);
			}
			Command* clone()override { return new isEmpty(); }
		};

		class remove : public Command {
		public:
			ArrayListO* m_arr{ nullptr };
			remove() {}
			remove(ArrayListO* arr) :m_arr(arr) {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				ArrayListO* arr;

				if (m_arr == nullptr)
					arr = (ArrayListO*)mem.get("this");
				else
					arr = m_arr;

				for (size_t i{ 0 }; i < arr->m_value.size(); i++) {
					IObject** args{ new IObject * [2]{arr->m_value[i],a} };
					CommandReturn* q{ arr->m_value[i]->exec("equals",args,2) };
					if (q->isThrow())
						return q;
					bool r{ ((BooleanO*)q->getObject())->m_value };
					delete[] args;
					delete q;
					if (r) {
						arr->m_value.erase(arr->m_value.begin() + i);
						break;
					}
				}
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new remove(); }
		};

		class removeI : public Command {
		public:
			removeI() {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("i") };
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };

				CommandReturn* q{ a->exec("toLong", a) };
				if (q->isThrow())
					return q;
				arr->m_value.erase(arr->m_value.begin() + ((LongO*)q->getObject())->m_value);
				delete q;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new removeI(); }
		};

		class removeAll : public Command {
		public:
			removeAll() {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };

				FunctionO* func{ new FunctionO(GLOBAL::getClasses()->getClass(Paths::Function),
					new Function(new Signature("",GLOBAL::getClasses()->getClass(Paths::Boolean),
						new Arg[1]{GLOBAL::getClasses()->getClass(Paths::Object),"c"},
							2),new Command * [1]{new remove(arr)},1)) };

				IObject** args{ new IObject * [2]{a,func} };


				CommandReturn* q{ a->exec("forEach", args, 2) };
				if (q->isThrow())
					return q;
				delete[] args;
				delete q;

				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new removeAll(); }
		};

		class size : public Command {
		public:
			size() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };

				return new CommandReturn(new LongO(GLOBAL::getClasses()->getClass(Paths::Long), arr->m_value.size()), true, false);
			}
			Command* clone()override { return new size(); }
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

		class sort : public Command {
		private:
			class doSort {
			public:
				FunctionO* m_func;
				doSort(IObject* func) :m_func((FunctionO*)func) {}
				bool operator() (IObject* a, IObject* b)
				{
					//a < b
					MemoryObject mem{};
					IObject** args{ new IObject * [2]{a,b} };
					CommandReturn* q{ m_func->m_value->exec(mem,args,2) };
					bool r{ ((BooleanO*)q->getObject())->m_value };
					delete[] args;
					delete q;
					return r;
				}
			};
		public:
			sort() {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("func") };
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };

				std::sort(arr->m_value.begin(), arr->m_value.end(), doSort{ a });

				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new sort(); }
		};

		class subList : public Command {
		public:
			ArrayListC* m_s;
			subList(ArrayListC* s) :m_s{ s } {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("a") };
				IObject* b{ mem.get("b") };

				int64_t i1{ ((LongO*)a->exec("toLong",a)->getObject())->m_value };
				int64_t i2{ ((LongO*)b->exec("toLong",b)->getObject())->m_value };

				ArrayListO* arr{ (ArrayListO*)mem.get("this") };

				ArrayListO* newArr{ new ArrayListO(m_s, i2 - i1) };

				for (int64_t i{ 0 }; i < i2 - i1; i++) {
					newArr->m_value[i] = arr->m_value[i1 + i];
					GarbageCollector::Add(newArr->m_value[i]);
				}

				return new CommandReturn(newArr, true, false);
			}
			Command* clone()override { return new subList(m_s); }
		};

		class toArray : public Command {
		public:
			Interface* m_default;
			toArray(Interface* _default) :m_default(_default) {}
			CommandReturn* exec(MemoryObject& mem) override {
				ClassO* a{ (ClassO*)mem.get("c") };

				Interface* i;
				if (a == nullptr)
					i = m_default;
				else
					i = (Interface*)a->m_value;

				ArrayListO* arr{ (ArrayListO*)mem.get("this") };

				ArrayO* newArr{ new ArrayO(GLOBAL::getClasses()->getClass(Paths::Array + '<' + i->getName() + '>'),arr->m_value.size()) };

				for (size_t i{ 0 }; i < arr->m_value.size(); i++) {
					newArr->m_value[i] = arr->m_value[i];
					GarbageCollector::Add(newArr->m_value[i]);
				}

				return new CommandReturn(newArr, true, false);
			}
			Command* clone()override { return new toArray(m_default); }
		};

		class ensureCapacity : public Command {
		public:
			ensureCapacity() {}
			CommandReturn* exec(MemoryObject& mem) override {
				IObject* a{ mem.get("c") };
				CommandReturn* q{ a->exec("toLong", a) };
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				size_t i{ arr->m_value.size() };
				arr->m_value.reserve(((LongO*)q->getObject())->m_value);
				for (; i < arr->m_value.size(); i++) {
					arr->m_value[i] = nullptr;
				}
				delete q;
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new ensureCapacity(); }
		};

		class trimToSize : public Command {
		public:
			trimToSize() {}
			CommandReturn* exec(MemoryObject& mem) override {
				ArrayListO* arr{ (ArrayListO*)mem.get("this") };
				arr->m_value.resize(arr->m_value.size());
				return new CommandReturn(new NullObject(), true, false);
			}
			Command* clone()override { return new trimToSize(); }
		};
	};
}