#include "RedLang.h"

#include "Class.h"
#include "Function.h"
#include "Command.h"
#include "ObjectClass.h"
#include "Boolean.h"
#include "Char.h"
#include "String.h"
#include "Byte.h"
#include "Short.h"
#include "Integer.h"
#include "Float.h"
#include "Long.h"
#include "Double.h"
#include "System.h"
#include "Exception.h"
#include "CastException.h"
#include "NullException.h"
#include "Global.h"
#include "ClassClass.h"
#include "Thread.h"
#include "Lock.h"
#include "FunctionClass.h"
#include "Number.h"
#include "IllegalArgumentException.h"
#include "Array.h"

void DTO::RedLang::importRedLang() {
	ObjectClass* object{ new ObjectClass() };
	GLOBAL::getClasses()->add(Paths::Object, object);

	BooleanC* Bool{ new BooleanC() };
	GLOBAL::getClasses()->add(Paths::Boolean, Bool);
	CharC* Char{ new CharC() };
	GLOBAL::getClasses()->add(Paths::Char, Char);
	StringC* String{ new StringC() };
	GLOBAL::getClasses()->add(Paths::String, String);
	System* system{ new System() };
	GLOBAL::getClasses()->add(Paths::System, system);

	Number* number{ new Number() };
	GLOBAL::getClasses()->add(Paths::Number, number);
	ByteC* Byte{ new ByteC(number) };
	GLOBAL::getClasses()->add(Paths::Byte, Byte);
	ShortC* Short{ new ShortC(number) };
	GLOBAL::getClasses()->add(Paths::Short, Short);
	IntegerC* Integer{ new IntegerC(number) };
	GLOBAL::getClasses()->add(Paths::Interger, Integer);
	FloatC* Float{ new FloatC(number) };
	GLOBAL::getClasses()->add(Paths::Float, Float);
	LongC* Long{ new LongC(number) };
	GLOBAL::getClasses()->add(Paths::Long, Long);
	DoubleC* Double{ new DoubleC(number) };
	GLOBAL::getClasses()->add(Paths::Double, Double);

	ClassClass* classClass{ new ClassClass() };
	GLOBAL::getClasses()->add(Paths::Class, classClass);

	FunctionClass* func{ new FunctionClass() };
	GLOBAL::getClasses()->add(Paths::Function, func);

	ThreadC* thread{ new ThreadC() };
	GLOBAL::getClasses()->add(Paths::Thread, thread);
	LockC* lock{ new LockC() };
	GLOBAL::getClasses()->add(Paths::Lock, lock);

	ExceptionC* except{ new ExceptionC() };
	GLOBAL::getClasses()->add(Paths::Exception, except);
	CastExceptionC* castExcept{ new CastExceptionC() };
	GLOBAL::getClasses()->add(Paths::CastException, castExcept);
	NullException* nullExcept{ new NullException() };
	GLOBAL::getClasses()->add(Paths::NullException, nullExcept);
	IllegalArgumentExceptionC* argExcept{ new IllegalArgumentExceptionC() };
	GLOBAL::getClasses()->add(Paths::IllegalArgumentException, argExcept);

	Array* array{ new Array() };
	GLOBAL::getClasses()->add(Paths::Array, array);

	object->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ object, object }, new std::string[]{ "this","c" }, 2), new Command * [1]{ new ObjectClass::Equals(Bool) }, 1));
	object->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ object }, new std::string[]{ "this" }, 1), new Command * [1]{ new ObjectClass::ToString(String) }, 1));
	object->getFuncs()->add("getClass", new Function(new Signature("", classClass, new Interface * [1]{ object }, new std::string[]{ "this" }, 1), new Command * [1]{ new ObjectClass::GetClass(classClass) }, 1));
	object->getFuncs()->add("hashCode", new Function(new Signature("", Long, new Interface * [1]{ object }, new std::string[]{ "this" }, 1), new Command * [1]{ new ObjectClass::HashCode(Long) }, 1));

	Bool->getFuncs()->add("not", new Function(new Signature("", Bool, new Interface * [1]{ Bool }, new std::string[1]{ "this" }, 1), new Command * [1]{ new BooleanC::Not(Bool) }, 1));
	Bool->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ Bool, Bool }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new BooleanC::Equals(Bool) }, 1));
	Bool->getStatFuncs()->add("Boolean", new Function(new Signature("", Bool, new Interface * [2]{ Bool, Bool }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new BooleanC::BooleanConstruct(Bool) }, 1));
	Bool->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ Bool }, new std::string[1]{ "this" }, 1), new Command * [1]{ new BooleanC::ToString(String) }, 1));

	Char->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ Char, Char }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new CharC::Equals(Bool) }, 1));
	Char->getStatFuncs()->add("Char", new Function(new Signature("", Char, new Interface * [2]{ Char, Char }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new CharC::CharConstruct(Char) }, 1));
	Char->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ Char }, new std::string[1]{ "this" }, 1), new Command * [1]{ new CharC::ToString(String) }, 1));

	String->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ String, String }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new StringC::Equals(Bool) }, 1));
	String->getStatFuncs()->add("String", new Function(new Signature("", String, new Interface * [2]{ String, Char }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new StringC::CharConstruct(String) }, 1));
	String->getStatFuncs()->add("String", new Function(new Signature("", String, new Interface * [2]{ String, String }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new StringC::StringConstruct(String) }, 1));
	String->getFuncs()->add("concat", new Function(new Signature("", String, new Interface * [2]{ String,String }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new StringC::Concat(String) }, 1));
	String->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ String }, new std::string[]{ "this" }, 1), new Command * [1]{ new StringC::ToString(String) }, 1));

	system->getStatFuncs()->add("getWord", new Function(new Signature("", String, new Interface * [0]{ }, new std::string[0]{ }, 0), new Command * [1]{ new System::GetWord(String) }, 1));
	system->getStatFuncs()->add("print", new Function(new Signature("", nullptr, new Interface * [1]{ object }, new std::string[1]{ "s" }, 1), new Command * [1]{ new System::print() }, 1));
	system->getStatFuncs()->add("println", new Function(new Signature("", nullptr, new Interface * [1]{ object }, new std::string[1]{ "s" }, 1), new Command * [1]{ new System::println() }, 1));
	system->getStatVars()->add("is32x", new StatVar(Bool, new BooleanO(Bool, sizeof(intptr_t) == 4)));
	system->getStatVars()->add("is64x", new StatVar(Bool, new BooleanO(Bool, sizeof(intptr_t) == 8)));

	number->add("toByte", new Signature("", Byte, new Interface * [1]{ number }, new std::string[1]{ "this" }, 1));
	number->add("toShort", new Signature("", Short, new Interface * [1]{ number }, new std::string[1]{ "this" }, 1));
	number->add("toInteger", new Signature("", Integer, new Interface * [1]{ number }, new std::string[1]{ "this" }, 1));
	number->add("toFloat", new Signature("", Float, new Interface * [1]{ number }, new std::string[1]{ "this" }, 1));
	number->add("toLong", new Signature("", Long, new Interface * [1]{ number }, new std::string[1]{ "this" }, 1));
	number->add("toDouble", new Signature("", Double, new Interface * [1]{ number }, new std::string[1]{ "this" }, 1));
	number->add("lt", new Signature("", Bool, new Interface * [2]{ number,number }, new std::string[2]{ "this","other" }, 2));
	number->add("lte", new Signature("", Bool, new Interface * [2]{ number,number }, new std::string[2]{ "this","other" }, 2));
	number->add("gt", new Signature("", Bool, new Interface * [2]{ number,number }, new std::string[2]{ "this","other" }, 2));
	number->add("gte", new Signature("", Bool, new Interface * [2]{ number,number }, new std::string[2]{ "this","other" }, 2));
	number->add("add", new Signature("", number, new Interface * [2]{ number,number }, new std::string[2]{ "this","other" }, 2));
	number->add("sub", new Signature("", number, new Interface * [2]{ number,number }, new std::string[2]{ "this","other" }, 2));
	number->add("mult", new Signature("", number, new Interface * [2]{ number,number }, new std::string[2]{ "this","other" }, 2));
	number->add("div", new Signature("", number, new Interface * [2]{ number,number }, new std::string[2]{ "this","other" }, 2));
	number->add("mod", new Signature("", number, new Interface * [2]{ number,number }, new std::string[2]{ "this","other" }, 2));

	Byte->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ Byte, Byte }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ByteC::Equals(Bool) }, 1));
	Byte->getStatFuncs()->add("Byte", new Function(new Signature("", Byte, new Interface * [2]{ Byte, Byte }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ByteC::ByteConstruct(Byte) }, 1));
	Byte->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ Byte }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ByteC::ToString(String) }, 1));
	Byte->getFuncs()->add("toByte", new Function(new Signature("", Byte, new Interface * [1]{ Byte }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ByteC::toByte(Byte) }, 1));
	Byte->getFuncs()->add("toShort", new Function(new Signature("", Short, new Interface * [1]{ Byte }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ByteC::toShort(Short) }, 1));
	Byte->getFuncs()->add("toInteger", new Function(new Signature("", Byte, new Interface * [1]{ Byte }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ByteC::toByte(Byte) }, 1));
	Byte->getFuncs()->add("toFloat", new Function(new Signature("", Float, new Interface * [1]{ Byte }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ByteC::toFloat(Float) }, 1));
	Byte->getFuncs()->add("toLong", new Function(new Signature("", Long, new Interface * [1]{ Byte }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ByteC::toLong(Long) }, 1));
	Byte->getFuncs()->add("toDouble", new Function(new Signature("", Double, new Interface * [1]{ Byte }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ByteC::toDouble(Double) }, 1));
	Byte->getFuncs()->add("lt", new Function(new Signature("", Bool, new Interface * [2]{ Byte,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ByteC::LessThan(Bool,argExcept) }, 1));
	Byte->getFuncs()->add("lte", new Function(new Signature("", Bool, new Interface * [2]{ Byte,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ByteC::LessThanEquals(Bool,argExcept) }, 1));
	Byte->getFuncs()->add("gt", new Function(new Signature("", Bool, new Interface * [2]{ Byte,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ByteC::GreaterThan(Bool,argExcept) }, 1));
	Byte->getFuncs()->add("gte", new Function(new Signature("", Bool, new Interface * [2]{ Byte,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ByteC::GreaterThanEquals(Bool,argExcept) }, 1));
	Byte->getFuncs()->add("add", new Function(new Signature("", number, new Interface * [2]{ Byte,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ByteC::Add(Byte,argExcept) }, 1));
	Byte->getFuncs()->add("sub", new Function(new Signature("", number, new Interface * [2]{ Byte,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ByteC::Sub(Byte,argExcept) }, 1));
	Byte->getFuncs()->add("mult", new Function(new Signature("", number, new Interface * [2]{ Byte,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ByteC::Mult(Byte,argExcept) }, 1));
	Byte->getFuncs()->add("div", new Function(new Signature("", number, new Interface * [2]{ Byte,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ByteC::Div(Byte,argExcept) }, 1));
	Byte->getFuncs()->add("mod", new Function(new Signature("", number, new Interface * [2]{ Byte,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ByteC::Mod(Byte,argExcept) }, 1));

	Short->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ Short, Short }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ShortC::Equals(Bool) }, 1));
	Short->getStatFuncs()->add("Short", new Function(new Signature("", Short, new Interface * [2]{ Short, Short }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ShortC::ShortConstruct(Short) }, 1));
	Short->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ Short }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ShortC::ToString(String) }, 1));
	Short->getFuncs()->add("toByte", new Function(new Signature("", Byte, new Interface * [1]{ Short }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ShortC::toByte(Byte) }, 1));
	Short->getFuncs()->add("toShort", new Function(new Signature("", Short, new Interface * [1]{ Short }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ShortC::toShort(Short) }, 1));
	Short->getFuncs()->add("toInteger", new Function(new Signature("", Short, new Interface * [1]{ Short }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ShortC::toShort(Short) }, 1));
	Short->getFuncs()->add("toFloat", new Function(new Signature("", Float, new Interface * [1]{ Short }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ShortC::toFloat(Float) }, 1));
	Short->getFuncs()->add("toLong", new Function(new Signature("", Long, new Interface * [1]{ Short }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ShortC::toLong(Long) }, 1));
	Short->getFuncs()->add("toDouble", new Function(new Signature("", Double, new Interface * [1]{ Short }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ShortC::toDouble(Double) }, 1));
	Short->getFuncs()->add("lt", new Function(new Signature("", Bool, new Interface * [2]{ Short,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ShortC::LessThan(Bool,argExcept) }, 1));
	Short->getFuncs()->add("lte", new Function(new Signature("", Bool, new Interface * [2]{ Short,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ShortC::LessThanEquals(Bool,argExcept) }, 1));
	Short->getFuncs()->add("gt", new Function(new Signature("", Bool, new Interface * [2]{ Short,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ShortC::GreaterThan(Bool,argExcept) }, 1));
	Short->getFuncs()->add("gte", new Function(new Signature("", Bool, new Interface * [2]{ Short,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ShortC::GreaterThanEquals(Bool,argExcept) }, 1));
	Short->getFuncs()->add("add", new Function(new Signature("", number, new Interface * [2]{ Short,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ShortC::Add(Short,argExcept) }, 1));
	Short->getFuncs()->add("sub", new Function(new Signature("", number, new Interface * [2]{ Short,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ShortC::Sub(Short,argExcept) }, 1));
	Short->getFuncs()->add("mult", new Function(new Signature("", number, new Interface * [2]{ Short,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ShortC::Mult(Short,argExcept) }, 1));
	Short->getFuncs()->add("div", new Function(new Signature("", number, new Interface * [2]{ Short,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ShortC::Div(Short,argExcept) }, 1));
	Short->getFuncs()->add("mod", new Function(new Signature("", number, new Interface * [2]{ Short,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new ShortC::Mod(Short,argExcept) }, 1));

	Integer->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ Integer, Integer }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new IntegerC::Equals(Bool) }, 1));
	Integer->getStatFuncs()->add("Integer", new Function(new Signature("", Integer, new Interface * [2]{ Integer, Integer }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new IntegerC::IntegerConstruct(Integer) }, 1));
	Integer->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ Integer }, new std::string[1]{ "this" }, 1), new Command * [1]{ new IntegerC::ToString(String) }, 1));
	Integer->getFuncs()->add("toByte", new Function(new Signature("", Byte, new Interface * [1]{ Integer }, new std::string[1]{ "this" }, 1), new Command * [1]{ new IntegerC::toByte(Byte) }, 1));
	Integer->getFuncs()->add("toShort", new Function(new Signature("", Short, new Interface * [1]{ Integer }, new std::string[1]{ "this" }, 1), new Command * [1]{ new IntegerC::toShort(Short) }, 1));
	Integer->getFuncs()->add("toInteger", new Function(new Signature("", Integer, new Interface * [1]{ Integer }, new std::string[1]{ "this" }, 1), new Command * [1]{ new IntegerC::toInteger(Integer) }, 1));
	Integer->getFuncs()->add("toFloat", new Function(new Signature("", Float, new Interface * [1]{ Integer }, new std::string[1]{ "this" }, 1), new Command * [1]{ new IntegerC::toFloat(Float) }, 1));
	Integer->getFuncs()->add("toLong", new Function(new Signature("", Long, new Interface * [1]{ Integer }, new std::string[1]{ "this" }, 1), new Command * [1]{ new IntegerC::toLong(Long) }, 1));
	Integer->getFuncs()->add("toDouble", new Function(new Signature("", Double, new Interface * [1]{ Integer }, new std::string[1]{ "this" }, 1), new Command * [1]{ new IntegerC::toDouble(Double) }, 1));
	Integer->getFuncs()->add("lt", new Function(new Signature("", Bool, new Interface * [2]{ Integer,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new IntegerC::LessThan(Bool,argExcept) }, 1));
	Integer->getFuncs()->add("lte", new Function(new Signature("", Bool, new Interface * [2]{ Integer,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new IntegerC::LessThanEquals(Bool,argExcept) }, 1));
	Integer->getFuncs()->add("gt", new Function(new Signature("", Bool, new Interface * [2]{ Integer,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new IntegerC::GreaterThan(Bool,argExcept) }, 1));
	Integer->getFuncs()->add("gte", new Function(new Signature("", Bool, new Interface * [2]{ Integer,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new IntegerC::GreaterThanEquals(Bool,argExcept) }, 1));
	Integer->getFuncs()->add("add", new Function(new Signature("", number, new Interface * [2]{ Integer,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new IntegerC::Add(Integer,argExcept) }, 1));
	Integer->getFuncs()->add("sub", new Function(new Signature("", number, new Interface * [2]{ Integer,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new IntegerC::Sub(Integer,argExcept) }, 1));
	Integer->getFuncs()->add("mult", new Function(new Signature("", number, new Interface * [2]{ Integer,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new IntegerC::Mult(Integer,argExcept) }, 1));
	Integer->getFuncs()->add("div", new Function(new Signature("", number, new Interface * [2]{ Integer,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new IntegerC::Div(Integer,argExcept) }, 1));
	Integer->getFuncs()->add("mod", new Function(new Signature("", number, new Interface * [2]{ Integer,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new IntegerC::Mod(Integer,argExcept) }, 1));

	Float->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ Float, Float }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new FloatC::Equals(Bool) }, 1));
	Float->getStatFuncs()->add("Float", new Function(new Signature("", Float, new Interface * [2]{ Float, Float }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new FloatC::FloatConstruct(Float) }, 1));
	Float->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ Float }, new std::string[1]{ "this" }, 1), new Command * [1]{ new FloatC::ToString(String) }, 1));
	Float->getFuncs()->add("toByte", new Function(new Signature("", Byte, new Interface * [1]{ Float }, new std::string[1]{ "this" }, 1), new Command * [1]{ new FloatC::toByte(Byte) }, 1));
	Float->getFuncs()->add("toShort", new Function(new Signature("", Short, new Interface * [1]{ Float }, new std::string[1]{ "this" }, 1), new Command * [1]{ new FloatC::toShort(Short) }, 1));
	Float->getFuncs()->add("toInteger", new Function(new Signature("", Float, new Interface * [1]{ Float }, new std::string[1]{ "this" }, 1), new Command * [1]{ new FloatC::toFloat(Float) }, 1));
	Float->getFuncs()->add("toFloat", new Function(new Signature("", Float, new Interface * [1]{ Float }, new std::string[1]{ "this" }, 1), new Command * [1]{ new FloatC::toFloat(Float) }, 1));
	Float->getFuncs()->add("toLong", new Function(new Signature("", Long, new Interface * [1]{ Float }, new std::string[1]{ "this" }, 1), new Command * [1]{ new FloatC::toLong(Long) }, 1));
	Float->getFuncs()->add("toDouble", new Function(new Signature("", Double, new Interface * [1]{ Float }, new std::string[1]{ "this" }, 1), new Command * [1]{ new FloatC::toDouble(Double) }, 1));
	Float->getFuncs()->add("lt", new Function(new Signature("", Bool, new Interface * [2]{ Float,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new FloatC::LessThan(Bool,argExcept) }, 1));
	Float->getFuncs()->add("lte", new Function(new Signature("", Bool, new Interface * [2]{ Float,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new FloatC::LessThanEquals(Bool,argExcept) }, 1));
	Float->getFuncs()->add("gt", new Function(new Signature("", Bool, new Interface * [2]{ Float,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new FloatC::GreaterThan(Bool,argExcept) }, 1));
	Float->getFuncs()->add("gte", new Function(new Signature("", Bool, new Interface * [2]{ Float,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new FloatC::GreaterThanEquals(Bool,argExcept) }, 1));
	Float->getFuncs()->add("add", new Function(new Signature("", number, new Interface * [2]{ Float,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new FloatC::Add(Float,argExcept) }, 1));
	Float->getFuncs()->add("sub", new Function(new Signature("", number, new Interface * [2]{ Float,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new FloatC::Sub(Float,argExcept) }, 1));
	Float->getFuncs()->add("mult", new Function(new Signature("", number, new Interface * [2]{ Float,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new FloatC::Mult(Float,argExcept) }, 1));
	Float->getFuncs()->add("div", new Function(new Signature("", number, new Interface * [2]{ Float,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new FloatC::Div(Float,argExcept) }, 1));
	Float->getFuncs()->add("mod", new Function(new Signature("", number, new Interface * [2]{ Float,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new FloatC::Mod(Float,argExcept) }, 1));

	Long->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ Long, Long }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new LongC::Equals(Bool) }, 1));
	Long->getStatFuncs()->add("Long", new Function(new Signature("", Long, new Interface * [2]{ Long, Long }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new LongC::LongConstruct(Long) }, 1));
	Long->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ Long }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LongC::ToString(String) }, 1));
	Long->getFuncs()->add("toByte", new Function(new Signature("", Byte, new Interface * [1]{ Long }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LongC::toByte(Byte) }, 1));
	Long->getFuncs()->add("toShort", new Function(new Signature("", Short, new Interface * [1]{ Long }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LongC::toShort(Short) }, 1));
	Long->getFuncs()->add("toInteger", new Function(new Signature("", Long, new Interface * [1]{ Long }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LongC::toLong(Long) }, 1));
	Long->getFuncs()->add("toFloat", new Function(new Signature("", Float, new Interface * [1]{ Long }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LongC::toFloat(Float) }, 1));
	Long->getFuncs()->add("toLong", new Function(new Signature("", Long, new Interface * [1]{ Long }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LongC::toLong(Long) }, 1));
	Long->getFuncs()->add("toDouble", new Function(new Signature("", Double, new Interface * [1]{ Long }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LongC::toDouble(Double) }, 1));
	Long->getFuncs()->add("lt", new Function(new Signature("", Bool, new Interface * [2]{ Long,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new LongC::LessThan(Bool,argExcept) }, 1));
	Long->getFuncs()->add("lte", new Function(new Signature("", Bool, new Interface * [2]{ Long,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new LongC::LessThanEquals(Bool,argExcept) }, 1));
	Long->getFuncs()->add("gt", new Function(new Signature("", Bool, new Interface * [2]{ Long,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new LongC::GreaterThan(Bool,argExcept) }, 1));
	Long->getFuncs()->add("gte", new Function(new Signature("", Bool, new Interface * [2]{ Long,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new LongC::GreaterThanEquals(Bool,argExcept) }, 1));
	Long->getFuncs()->add("add", new Function(new Signature("", number, new Interface * [2]{ Long,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new LongC::Add(Long,argExcept) }, 1));
	Long->getFuncs()->add("sub", new Function(new Signature("", number, new Interface * [2]{ Long,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new LongC::Sub(Long,argExcept) }, 1));
	Long->getFuncs()->add("mult", new Function(new Signature("", number, new Interface * [2]{ Long,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new LongC::Mult(Long,argExcept) }, 1));
	Long->getFuncs()->add("div", new Function(new Signature("", number, new Interface * [2]{ Long,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new LongC::Div(Long,argExcept) }, 1));
	Long->getFuncs()->add("mod", new Function(new Signature("", number, new Interface * [2]{ Long,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new LongC::Mod(Long,argExcept) }, 1));

	Double->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ Double, Double }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new DoubleC::Equals(Bool) }, 1));
	Double->getStatFuncs()->add("Double", new Function(new Signature("", Double, new Interface * [2]{ Double, Double }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new DoubleC::DoubleConstruct(Double) }, 1));
	Double->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ Double }, new std::string[1]{ "this" }, 1), new Command * [1]{ new DoubleC::ToString(String) }, 1));
	Double->getFuncs()->add("toByte", new Function(new Signature("", Byte, new Interface * [1]{ Double }, new std::string[1]{ "this" }, 1), new Command * [1]{ new DoubleC::toByte(Byte) }, 1));
	Double->getFuncs()->add("toShort", new Function(new Signature("", Short, new Interface * [1]{ Double }, new std::string[1]{ "this" }, 1), new Command * [1]{ new DoubleC::toShort(Short) }, 1));
	Double->getFuncs()->add("toInteger", new Function(new Signature("", Double, new Interface * [1]{ Double }, new std::string[1]{ "this" }, 1), new Command * [1]{ new DoubleC::toDouble(Double) }, 1));
	Double->getFuncs()->add("toFloat", new Function(new Signature("", Float, new Interface * [1]{ Double }, new std::string[1]{ "this" }, 1), new Command * [1]{ new DoubleC::toFloat(Float) }, 1));
	Double->getFuncs()->add("toLong", new Function(new Signature("", Long, new Interface * [1]{ Double }, new std::string[1]{ "this" }, 1), new Command * [1]{ new DoubleC::toLong(Long) }, 1));
	Double->getFuncs()->add("toDouble", new Function(new Signature("", Double, new Interface * [1]{ Double }, new std::string[1]{ "this" }, 1), new Command * [1]{ new DoubleC::toDouble(Double) }, 1));
	Double->getFuncs()->add("lt", new Function(new Signature("", Bool, new Interface * [2]{ Double,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new DoubleC::LessThan(Bool,argExcept) }, 1));
	Double->getFuncs()->add("lte", new Function(new Signature("", Bool, new Interface * [2]{ Double,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new DoubleC::LessThanEquals(Bool,argExcept) }, 1));
	Double->getFuncs()->add("gt", new Function(new Signature("", Bool, new Interface * [2]{ Double,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new DoubleC::GreaterThan(Bool,argExcept) }, 1));
	Double->getFuncs()->add("gte", new Function(new Signature("", Bool, new Interface * [2]{ Double,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new DoubleC::GreaterThanEquals(Bool,argExcept) }, 1));
	Double->getFuncs()->add("add", new Function(new Signature("", number, new Interface * [2]{ Double,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new DoubleC::Add(Double,argExcept) }, 1));
	Double->getFuncs()->add("sub", new Function(new Signature("", number, new Interface * [2]{ Double,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new DoubleC::Sub(Double,argExcept) }, 1));
	Double->getFuncs()->add("mult", new Function(new Signature("", number, new Interface * [2]{ Double,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new DoubleC::Mult(Double,argExcept) }, 1));
	Double->getFuncs()->add("div", new Function(new Signature("", number, new Interface * [2]{ Double,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new DoubleC::Div(Double,argExcept) }, 1));
	Double->getFuncs()->add("mod", new Function(new Signature("", number, new Interface * [2]{ Double,number }, new std::string[2]{ "this","other" }, 2), new Command * [1]{ new DoubleC::Mod(Double,argExcept) }, 1));

	classClass->getStatFuncs()->add("Class", new Function(new Signature("", classClass, new Interface * [2]{ classClass,String }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ClassClass::ClassConstruct(classClass) }, 1));
	classClass->getFuncs()->add("name", new Function(new Signature("", String, new Interface * [1]{ classClass }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ClassClass::GetName(String) }, 1));
	classClass->getFuncs()->add("statVar", new Function(new Signature("", nullptr, new Interface * [2]{ classClass,String }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ClassClass::GetStatVar(String) }, 1));
	classClass->getFuncs()->add("invoke", new Function(new Signature("", nullptr, new Interface * [2]{ classClass,String }, new std::string[2]{ "this","func" }, 2, true), new Command * [1]{ new ClassClass::Invoke(String) }, 1));
	classClass->getFuncs()->add("instanceOf", new Function(new Signature("", Bool, new Interface * [2]{ classClass,classClass }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ClassClass::InstanceOf(Bool) }, 1));
	classClass->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ classClass, classClass }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ClassClass::Equals(Bool) }, 1));
	classClass->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ classClass }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ClassClass::ToString(String) }, 1));

	func->getStatFuncs()->add("Function", new Function(new Signature("", func, new Interface * [2]{ func,func }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new FunctionClass::FunctionConstruct(func) }, 1));
	func->getFuncs()->add("execute", new Function(new Signature("", String, new Interface * [1]{ func }, new std::string[1]{ "this" }, 1, true), new Command * [1]{ new FunctionClass::Execute(func) }, 1));
	func->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ func, func }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new FunctionClass::Equals(Bool) }, 1));
	func->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ func }, new std::string[1]{ "this" }, 1), new Command * [1]{ new FunctionClass::ToString(String) }, 1));

	thread->getStatFuncs()->add("Thread", new Function(new Signature("", thread, new Interface * [2]{ thread,func }, new std::string[2]{ "this","func" }, 2, true), new Command * [1]{ new ThreadC::ThreadConstruct(thread) }, 1));
	thread->getStatFuncs()->add("hardware_concurrency", new Function(new Signature("", nullptr, new Interface * [0]{ }, new std::string[0]{  }, 0), new Command * [1]{ new ThreadC::HardwareConcurrency(Integer) }, 1));
	thread->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ thread, thread }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ThreadC::Equals(Bool) }, 1));
	thread->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ thread }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ThreadC::ToString(String) }, 1));
	thread->getFuncs()->add("start", new Function(new Signature("", nullptr, new Interface * [1]{ thread }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ThreadC::Start(String) }, 1));
	thread->getFuncs()->add("join", new Function(new Signature("", nullptr, new Interface * [1]{ thread }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ThreadC::Join(String) }, 1));
	//thread->getFuncs()->add("detach", new Function(new Signature("",nullptr, new Interface * [1]{ thread }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ThreadC::Detach(String) }, 1));

	lock->getStatFuncs()->add("Lock", new Function(new Signature("", lock, new Interface * [1]{ lock }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LockC::LockConstruct(lock) }, 1));
	lock->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ lock, lock }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new LockC::Equals(Bool) }, 1));
	lock->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ lock }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LockC::ToString(String) }, 1));
	lock->getFuncs()->add("lock", new Function(new Signature("", nullptr, new Interface * [1]{ lock }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LockC::Lock(String) }, 1));
	lock->getFuncs()->add("unLock", new Function(new Signature("", nullptr, new Interface * [1]{ lock }, new std::string[1]{ "this" }, 1), new Command * [1]{ new LockC::Unlock(String) }, 1));

	except->getFuncs()->add("Equals", new Function(new Signature("", Bool, new Interface * [2]{ except, except }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ExceptionC::Equals(Bool) }, 1));
	except->getStatFuncs()->add("Exception", new Function(new Signature("", except, new Interface * [2]{ except, String }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new ExceptionC::ExceptionConstruct(except) }, 1));
	except->getFuncs()->add("toString", new Function(new Signature("", String, new Interface * [1]{ except }, new std::string[1]{ "this" }, 1), new Command * [1]{ new ExceptionC::ToString(String) }, 1));

	castExcept->getStatFuncs()->add("CastException", new Function(new Signature("", castExcept, new Interface * [2]{ castExcept, String }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new CastExceptionC::ExceptionConstruct(castExcept) }, 1));
	nullExcept->getStatFuncs()->add("NullException", new Function(new Signature("", nullExcept, new Interface * [2]{ nullExcept, String }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new NullException::ExceptionConstruct(nullExcept) }, 1));
	argExcept->getStatFuncs()->add("IllegalArgumentException", new Function(new Signature("", argExcept, new Interface * [2]{ argExcept, String }, new std::string[2]{ "this","c" }, 2), new Command * [1]{ new IllegalArgumentExceptionC::ExceptionConstruct(argExcept) }, 1));
}