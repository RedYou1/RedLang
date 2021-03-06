#pragma once
#include <string>
#include "Object.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Boolean.h"
#include "IllegalArgumentException.h"
#include "Global.h"

#include "Number.h"

namespace DTO {
	class ByteC;
	class ShortC;
	class IntegerC;
	class LongC;
	class DoubleC;

	class FloatO : public NumberO {
	public:
		float_t m_value;

		FloatO(Class* type);

		FloatO(Class* type, float_t value);

		Object* clone()override;

		int64_t toLong() override { return (int64_t)m_value; }
		double_t toDouble() override { return (double_t)m_value; }
		bool isInteger() override { return false; }
	};

	class FloatC : public Class {
	public:
		FloatC(Number* number) : Class(L"Float", Paths::Float, GLOBAL::getClasses()->getClass(Paths::Object), new Interface* [1]{ number }, 1) {
		}

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Equals(m_s); }
		};
		class FloatConstruct :public Command {
		public:
			FloatC* m_s;
			FloatConstruct(FloatC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new FloatConstruct(m_s); }
		};

		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new ToString(m_s); }
		};

		class toByte :public Command {
		public:
			ByteC* m_s;
			toByte(ByteC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new toByte(m_s); }
		};
		class toShort :public Command {
		public:
			ShortC* m_s;
			toShort(ShortC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new toShort(m_s); }
		};
		class toInteger :public Command {
		public:
			IntegerC* m_s;
			toInteger(IntegerC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new toInteger(m_s); }
		};
		class toFloat :public Command {
		public:
			FloatC* m_s;
			toFloat(FloatC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new toFloat(m_s); }
		};
		class toLong :public Command {
		public:
			LongC* m_s;
			toLong(LongC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new toLong(m_s); }
		};
		class toDouble :public Command {
		public:
			DoubleC* m_s;
			toDouble(DoubleC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new toDouble(m_s); }
		};

		class LessThan :public Command {
		public:
			BooleanC* m_s;
			IllegalArgumentExceptionC* m_except;
			LessThan(BooleanC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new LessThan(m_s, m_except); }
		};
		class LessThanEquals :public Command {
		public:
			BooleanC* m_s;
			IllegalArgumentExceptionC* m_except;
			LessThanEquals(BooleanC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new LessThanEquals(m_s, m_except); }
		};
		class GreaterThan :public Command {
		public:
			BooleanC* m_s;
			IllegalArgumentExceptionC* m_except;
			GreaterThan(BooleanC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new GreaterThan(m_s, m_except); }
		};
		class GreaterThanEquals :public Command {
		public:
			BooleanC* m_s;
			IllegalArgumentExceptionC* m_except;
			GreaterThanEquals(BooleanC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new GreaterThanEquals(m_s, m_except); }
		};
		class Add :public Command {
		public:
			FloatC* m_s;
			IllegalArgumentExceptionC* m_except;
			Add(FloatC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Add(m_s, m_except); }
		};
		class Sub :public Command {
		public:
			FloatC* m_s;
			IllegalArgumentExceptionC* m_except;
			Sub(FloatC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Sub(m_s, m_except); }
		};
		class Mult :public Command {
		public:
			FloatC* m_s;
			IllegalArgumentExceptionC* m_except;
			Mult(FloatC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Mult(m_s, m_except); }
		};
		class Div :public Command {
		public:
			FloatC* m_s;
			IllegalArgumentExceptionC* m_except;
			Div(FloatC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Div(m_s, m_except); }
		};
		class Mod :public Command {
		public:
			FloatC* m_s;
			IllegalArgumentExceptionC* m_except;
			Mod(FloatC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Mod(m_s, m_except); }
		};
	};
}