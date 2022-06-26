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
	class ShortC;
	class IntegerC;
	class FloatC;
	class LongC;
	class DoubleC;

	class ByteO : public NumberO {
	public:
		int8_t m_value;

		ByteO(Class* type);

		ByteO(Class* type, int8_t value);

		Object* clone()override;

		int64_t toLong() override { return (int64_t)m_value; }
		double_t toDouble() override { return (double_t)m_value; }
		bool isInteger() override { return true; }
	};

	class ByteC : public Class {
	public:
		ByteC(Number* number) : Class("Byte", Paths::Byte, GLOBAL::getClasses()->getClass(Paths::Object), new Interface* [1]{ number }, 1) {
		}

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Equals(m_s); }
		};
		class ByteConstruct :public Command {
		public:
			ByteC* m_s;
			ByteConstruct(ByteC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new ByteConstruct(m_s); }
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
			ByteC* m_s;
			IllegalArgumentExceptionC* m_except;
			Add(ByteC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Add(m_s, m_except); }
		};
		class Sub :public Command {
		public:
			ByteC* m_s;
			IllegalArgumentExceptionC* m_except;
			Sub(ByteC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Sub(m_s, m_except); }
		};
		class Mult :public Command {
		public:
			ByteC* m_s;
			IllegalArgumentExceptionC* m_except;
			Mult(ByteC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Mult(m_s, m_except); }
		};
		class Div :public Command {
		public:
			ByteC* m_s;
			IllegalArgumentExceptionC* m_except;
			Div(ByteC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Div(m_s, m_except); }
		};
		class Mod :public Command {
		public:
			ByteC* m_s;
			IllegalArgumentExceptionC* m_except;
			Mod(ByteC* s, IllegalArgumentExceptionC* except) :m_s(s), m_except(except) {}
			CommandReturn* exec(MemoryObject& mem) override;
			Command* clone()override { return new Mod(m_s, m_except); }
		};
	};
}