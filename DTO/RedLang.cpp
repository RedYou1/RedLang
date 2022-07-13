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
#include "Iterable.h"
#include "Collection.h"
#include "Array.h"
#include "List.h"
#include "ArrayList.h"
#include "File.h"

void DTO::RedLang::importRedLang(SourceFile* (*parser)(std::filesystem::path)) {
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
	GLOBAL::getClasses()->add(Paths::Integer, Integer);
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

	FileC* file{ new FileC() };
	GLOBAL::getClasses()->add(Paths::File, file);

	ExceptionC* except{ new ExceptionC() };
	GLOBAL::getClasses()->add(Paths::Exception, except);
	CastExceptionC* castExcept{ new CastExceptionC() };
	GLOBAL::getClasses()->add(Paths::CastException, castExcept);
	NullException* nullExcept{ new NullException() };
	GLOBAL::getClasses()->add(Paths::NullException, nullExcept);
	IllegalArgumentExceptionC* argExcept{ new IllegalArgumentExceptionC() };
	GLOBAL::getClasses()->add(Paths::IllegalArgumentException, argExcept);

	GLOBAL::getClasses()->add(Paths::Iterable, new Iterable());
	GLOBAL::getClasses()->add(Paths::Collection, new Collection());
	GLOBAL::getClasses()->add(Paths::Array, new Array());
	GLOBAL::getClasses()->add(Paths::List, new List());
	GLOBAL::getClasses()->add(Paths::ArrayList, new ArrayList());

	object->addFunc(L"equals", Bool, new Arg[2]{ object,L"this", object,L"c" }, 2, new ObjectClass::Equals(Bool));
	object->addFunc(L"toString", String, new Arg[1]{ object,L"this" }, 1, new ObjectClass::ToString(String));
	object->addFunc(L"getClass", classClass, new Arg[1]{ object,L"this" }, 1, new ObjectClass::GetClass(classClass));
	object->addFunc(L"hashCode", Long, new Arg[1]{ object,L"this" }, 1, new ObjectClass::HashCode(Long));

	Bool->addFunc(L"not", Bool, new Arg[1]{ Bool,L"this" }, 1, new BooleanC::Not(Bool));
	Bool->addFunc(L"equals", Bool, new Arg[2]{ Bool,L"this", Bool,L"c" }, 2, new BooleanC::Equals(Bool));
	Bool->addFunc(L"Boolean", Bool, new Arg[2]{ Bool,L"this", Bool,L"c" }, 2, new BooleanC::BooleanConstruct(Bool));
	Bool->addFunc(L"toString", String, new Arg[1]{ Bool,L"this" }, 1, new BooleanC::ToString(String));

	Char->addFunc(L"equals", Bool, new Arg[2]{ Char,L"this", Char,L"c" }, 2, new CharC::Equals(Bool));
	Char->addFunc(L"Char", Char, new Arg[2]{ Char,L"this", Char,L"c" }, 2, new CharC::CharConstruct(Char));
	Char->addFunc(L"toString", String, new Arg[1]{ Char,L"this" }, 1, new CharC::ToString(String));

	String->addFunc(L"equals", Bool, new Arg[2]{ String,L"this", String,L"c" }, 2, new StringC::Equals(Bool));
	String->addFunc(L"String", String, new Arg[2]{ String,L"this", Char,L"c" }, 2, new StringC::CharConstruct(String));
	String->addFunc(L"String", String, new Arg[2]{ String,L"this", String,L"c" }, 2, new StringC::StringConstruct(String));
	String->addFunc(L"concat", String, new Arg[2]{ String,L"this", object,L"other" }, 2, new StringC::Concat(String));
	String->addFunc(L"toString", String, new Arg[1]{ String,L"this" }, 1, new StringC::ToString(String));

	system->addFunc(L"getWord", String, new Arg[0]{ }, 0, new System::GetWord(String));
	system->addFunc(L"print", nullptr, new Arg[1]{ object,L"s" }, 1, new System::print());
	system->addFunc(L"println", nullptr, new Arg[1]{ object,L"s" }, 1, new System::println());
	system->getStatVars()->add(L"is32x", new StatVar(Bool, new BooleanO(Bool, sizeof(intptr_t) == 4)));
	system->getStatVars()->add(L"is64x", new StatVar(Bool, new BooleanO(Bool, sizeof(intptr_t) == 8)));

	number->add(L"toByte", new Signature(L"", Byte, new Arg[1]{ number,L"this" }, 1));
	number->add(L"toShort", new Signature(L"", Short, new Arg[1]{ number,L"this" }, 1));
	number->add(L"toInteger", new Signature(L"", Integer, new Arg[1]{ number,L"this" }, 1));
	number->add(L"toFloat", new Signature(L"", Float, new Arg[1]{ number,L"this" }, 1));
	number->add(L"toLong", new Signature(L"", Long, new Arg[1]{ number,L"this" }, 1));
	number->add(L"toDouble", new Signature(L"", Double, new Arg[1]{ number,L"this" }, 1));
	number->add(L"lt", new Signature(L"", Bool, new Arg[2]{ number,L"this", number,L"other" }, 2));
	number->add(L"lte", new Signature(L"", Bool, new Arg[2]{ number,L"this", number,L"other" }, 2));
	number->add(L"gt", new Signature(L"", Bool, new Arg[2]{ number,L"this", number,L"other" }, 2));
	number->add(L"gte", new Signature(L"", Bool, new Arg[2]{ number,L"this", number,L"other" }, 2));
	number->add(L"add", new Signature(L"", number, new Arg[2]{ number,L"this", number,L"other" }, 2));
	number->add(L"sub", new Signature(L"", number, new Arg[2]{ number,L"this", number,L"other" }, 2));
	number->add(L"mult", new Signature(L"", number, new Arg[2]{ number,L"this", number,L"other" }, 2));
	number->add(L"div", new Signature(L"", number, new Arg[2]{ number,L"this", number,L"other" }, 2));
	number->add(L"mod", new Signature(L"", number, new Arg[2]{ number,L"this", number,L"other" }, 2));

	Byte->addFunc(L"equals", Bool, new Arg[2]{ Byte,L"this", Byte,L"c" }, 2, new ByteC::Equals(Bool));
	Byte->addFunc(L"Byte", Byte, new Arg[2]{ Byte,L"this", Byte,L"c" }, 2, new ByteC::ByteConstruct(Byte));
	Byte->addFunc(L"toString", String, new Arg[1]{ Byte,L"this" }, 1, new ByteC::ToString(String));
	Byte->addFunc(L"toByte", Byte, new Arg[1]{ Byte,L"this" }, 1, new ByteC::toByte(Byte));
	Byte->addFunc(L"toShort", Short, new Arg[1]{ Byte,L"this" }, 1, new ByteC::toShort(Short));
	Byte->addFunc(L"toInteger", Byte, new Arg[1]{ Byte,L"this" }, 1, new ByteC::toByte(Byte));
	Byte->addFunc(L"toFloat", Float, new Arg[1]{ Byte,L"this" }, 1, new ByteC::toFloat(Float));
	Byte->addFunc(L"toLong", Long, new Arg[1]{ Byte,L"this" }, 1, new ByteC::toLong(Long));
	Byte->addFunc(L"toDouble", Double, new Arg[1]{ Byte,L"this" }, 1, new ByteC::toDouble(Double));
	Byte->addFunc(L"lt", Bool, new Arg[2]{ Byte,L"this", number,L"other" }, 2, new ByteC::LessThan(Bool, argExcept));
	Byte->addFunc(L"lte", Bool, new Arg[2]{ Byte,L"this", number,L"other" }, 2, new ByteC::LessThanEquals(Bool, argExcept));
	Byte->addFunc(L"gt", Bool, new Arg[2]{ Byte,L"this", number,L"other" }, 2, new ByteC::GreaterThan(Bool, argExcept));
	Byte->addFunc(L"gte", Bool, new Arg[2]{ Byte,L"this", number,L"other" }, 2, new ByteC::GreaterThanEquals(Bool, argExcept));
	Byte->addFunc(L"add", number, new Arg[2]{ Byte,L"this", number,L"other" }, 2, new ByteC::Add(Byte, argExcept));
	Byte->addFunc(L"sub", number, new Arg[2]{ Byte,L"this", number,L"other" }, 2, new ByteC::Sub(Byte, argExcept));
	Byte->addFunc(L"mult", number, new Arg[2]{ Byte,L"this", number,L"other" }, 2, new ByteC::Mult(Byte, argExcept));
	Byte->addFunc(L"div", number, new Arg[2]{ Byte,L"this", number,L"other" }, 2, new ByteC::Div(Byte, argExcept));
	Byte->addFunc(L"mod", number, new Arg[2]{ Byte,L"this", number,L"other" }, 2, new ByteC::Mod(Byte, argExcept));

	Short->addFunc(L"equals", Bool, new Arg[2]{ Short,L"this", Short,L"c" }, 2, new ShortC::Equals(Bool));
	Short->addFunc(L"Short", Short, new Arg[2]{ Short,L"this", Short,L"c" }, 2, new ShortC::ShortConstruct(Short));
	Short->addFunc(L"toString", String, new Arg[1]{ Short,L"this" }, 1, new ShortC::ToString(String));
	Short->addFunc(L"toByte", Byte, new Arg[1]{ Short,L"this" }, 1, new ShortC::toByte(Byte));
	Short->addFunc(L"toShort", Short, new Arg[1]{ Short,L"this" }, 1, new ShortC::toShort(Short));
	Short->addFunc(L"toInteger", Short, new Arg[1]{ Short,L"this" }, 1, new ShortC::toShort(Short));
	Short->addFunc(L"toFloat", Float, new Arg[1]{ Short,L"this" }, 1, new ShortC::toFloat(Float));
	Short->addFunc(L"toLong", Long, new Arg[1]{ Short,L"this" }, 1, new ShortC::toLong(Long));
	Short->addFunc(L"toDouble", Double, new Arg[1]{ Short,L"this" }, 1, new ShortC::toDouble(Double));
	Short->addFunc(L"lt", Bool, new Arg[2]{ Short,L"this", number,L"other" }, 2, new ShortC::LessThan(Bool, argExcept));
	Short->addFunc(L"lte", Bool, new Arg[2]{ Short,L"this", number,L"other" }, 2, new ShortC::LessThanEquals(Bool, argExcept));
	Short->addFunc(L"gt", Bool, new Arg[2]{ Short,L"this", number,L"other" }, 2, new ShortC::GreaterThan(Bool, argExcept));
	Short->addFunc(L"gte", Bool, new Arg[2]{ Short,L"this", number,L"other" }, 2, new ShortC::GreaterThanEquals(Bool, argExcept));
	Short->addFunc(L"add", number, new Arg[2]{ Short,L"this", number,L"other" }, 2, new ShortC::Add(Short, argExcept));
	Short->addFunc(L"sub", number, new Arg[2]{ Short,L"this", number,L"other" }, 2, new ShortC::Sub(Short, argExcept));
	Short->addFunc(L"mult", number, new Arg[2]{ Short,L"this", number,L"other" }, 2, new ShortC::Mult(Short, argExcept));
	Short->addFunc(L"div", number, new Arg[2]{ Short,L"this", number,L"other" }, 2, new ShortC::Div(Short, argExcept));
	Short->addFunc(L"mod", number, new Arg[2]{ Short,L"this", number,L"other" }, 2, new ShortC::Mod(Short, argExcept));

	Integer->addFunc(L"equals", Bool, new Arg[2]{ Integer,L"this", Integer,L"c" }, 2, new IntegerC::Equals(Bool));
	Integer->addFunc(L"Integer", Integer, new Arg[2]{ Integer,L"this", Integer,L"c" }, 2, new IntegerC::IntegerConstruct(Integer));
	Integer->addFunc(L"toString", String, new Arg[1]{ Integer,L"this" }, 1, new IntegerC::ToString(String));
	Integer->addFunc(L"toByte", Byte, new Arg[1]{ Integer,L"this" }, 1, new IntegerC::toByte(Byte));
	Integer->addFunc(L"toShort", Short, new Arg[1]{ Integer,L"this" }, 1, new IntegerC::toShort(Short));
	Integer->addFunc(L"toInteger", Integer, new Arg[1]{ Integer,L"this" }, 1, new IntegerC::toInteger(Integer));
	Integer->addFunc(L"toFloat", Float, new Arg[1]{ Integer,L"this" }, 1, new IntegerC::toFloat(Float));
	Integer->addFunc(L"toLong", Long, new Arg[1]{ Integer,L"this" }, 1, new IntegerC::toLong(Long));
	Integer->addFunc(L"toDouble", Double, new Arg[1]{ Integer,L"this" }, 1, new IntegerC::toDouble(Double));
	Integer->addFunc(L"lt", Bool, new Arg[2]{ Integer,L"this", number,L"other" }, 2, new IntegerC::LessThan(Bool, argExcept));
	Integer->addFunc(L"lte", Bool, new Arg[2]{ Integer,L"this", number,L"other" }, 2, new IntegerC::LessThanEquals(Bool, argExcept));
	Integer->addFunc(L"gt", Bool, new Arg[2]{ Integer,L"this", number,L"other" }, 2, new IntegerC::GreaterThan(Bool, argExcept));
	Integer->addFunc(L"gte", Bool, new Arg[2]{ Integer,L"this", number,L"other" }, 2, new IntegerC::GreaterThanEquals(Bool, argExcept));
	Integer->addFunc(L"add", number, new Arg[2]{ Integer,L"this", number,L"other" }, 2, new IntegerC::Add(Integer, argExcept));
	Integer->addFunc(L"sub", number, new Arg[2]{ Integer,L"this", number,L"other" }, 2, new IntegerC::Sub(Integer, argExcept));
	Integer->addFunc(L"mult", number, new Arg[2]{ Integer,L"this", number,L"other" }, 2, new IntegerC::Mult(Integer, argExcept));
	Integer->addFunc(L"div", number, new Arg[2]{ Integer,L"this", number,L"other" }, 2, new IntegerC::Div(Integer, argExcept));
	Integer->addFunc(L"mod", number, new Arg[2]{ Integer,L"this", number,L"other" }, 2, new IntegerC::Mod(Integer, argExcept));

	Float->addFunc(L"equals", Bool, new Arg[2]{ Float,L"this", Float,L"c" }, 2, new FloatC::Equals(Bool));
	Float->addFunc(L"Float", Float, new Arg[2]{ Float,L"this", Float,L"c" }, 2, new FloatC::FloatConstruct(Float));
	Float->addFunc(L"toString", String, new Arg[1]{ Float,L"this" }, 1, new FloatC::ToString(String));
	Float->addFunc(L"toByte", Byte, new Arg[1]{ Float,L"this" }, 1, new FloatC::toByte(Byte));
	Float->addFunc(L"toShort", Short, new Arg[1]{ Float,L"this" }, 1, new FloatC::toShort(Short));
	Float->addFunc(L"toInteger", Float, new Arg[1]{ Float,L"this" }, 1, new FloatC::toFloat(Float));
	Float->addFunc(L"toFloat", Float, new Arg[1]{ Float,L"this" }, 1, new FloatC::toFloat(Float));
	Float->addFunc(L"toLong", Long, new Arg[1]{ Float,L"this" }, 1, new FloatC::toLong(Long));
	Float->addFunc(L"toDouble", Double, new Arg[1]{ Float,L"this" }, 1, new FloatC::toDouble(Double));
	Float->addFunc(L"lt", Bool, new Arg[2]{ Float,L"this", number,L"other" }, 2, new FloatC::LessThan(Bool, argExcept));
	Float->addFunc(L"lte", Bool, new Arg[2]{ Float,L"this", number,L"other" }, 2, new FloatC::LessThanEquals(Bool, argExcept));
	Float->addFunc(L"gt", Bool, new Arg[2]{ Float,L"this", number,L"other" }, 2, new FloatC::GreaterThan(Bool, argExcept));
	Float->addFunc(L"gte", Bool, new Arg[2]{ Float,L"this", number,L"other" }, 2, new FloatC::GreaterThanEquals(Bool, argExcept));
	Float->addFunc(L"add", number, new Arg[2]{ Float,L"this", number,L"other" }, 2, new FloatC::Add(Float, argExcept));
	Float->addFunc(L"sub", number, new Arg[2]{ Float,L"this", number,L"other" }, 2, new FloatC::Sub(Float, argExcept));
	Float->addFunc(L"mult", number, new Arg[2]{ Float,L"this", number,L"other" }, 2, new FloatC::Mult(Float, argExcept));
	Float->addFunc(L"div", number, new Arg[2]{ Float,L"this", number,L"other" }, 2, new FloatC::Div(Float, argExcept));
	Float->addFunc(L"mod", number, new Arg[2]{ Float,L"this", number,L"other" }, 2, new FloatC::Mod(Float, argExcept));

	Long->addFunc(L"equals", Bool, new Arg[2]{ Long,L"this", Long,L"c" }, 2, new LongC::Equals(Bool));
	Long->addFunc(L"Long", Long, new Arg[2]{ Long,L"this", Long,L"c" }, 2, new LongC::LongConstruct(Long));
	Long->addFunc(L"toString", String, new Arg[1]{ Long,L"this" }, 1, new LongC::ToString(String));
	Long->addFunc(L"toByte", Byte, new Arg[1]{ Long,L"this" }, 1, new LongC::toByte(Byte));
	Long->addFunc(L"toShort", Short, new Arg[1]{ Long,L"this" }, 1, new LongC::toShort(Short));
	Long->addFunc(L"toInteger", Long, new Arg[1]{ Long,L"this" }, 1, new LongC::toLong(Long));
	Long->addFunc(L"toFloat", Float, new Arg[1]{ Long,L"this" }, 1, new LongC::toFloat(Float));
	Long->addFunc(L"toLong", Long, new Arg[1]{ Long,L"this" }, 1, new LongC::toLong(Long));
	Long->addFunc(L"toDouble", Double, new Arg[1]{ Long,L"this" }, 1, new LongC::toDouble(Double));
	Long->addFunc(L"lt", Bool, new Arg[2]{ Long,L"this", number,L"other" }, 2, new LongC::LessThan(Bool, argExcept));
	Long->addFunc(L"lte", Bool, new Arg[2]{ Long,L"this", number,L"other" }, 2, new LongC::LessThanEquals(Bool, argExcept));
	Long->addFunc(L"gt", Bool, new Arg[2]{ Long,L"this", number,L"other" }, 2, new LongC::GreaterThan(Bool, argExcept));
	Long->addFunc(L"gte", Bool, new Arg[2]{ Long,L"this", number,L"other" }, 2, new LongC::GreaterThanEquals(Bool, argExcept));
	Long->addFunc(L"add", number, new Arg[2]{ Long,L"this", number,L"other" }, 2, new LongC::Add(Long, argExcept));
	Long->addFunc(L"sub", number, new Arg[2]{ Long,L"this", number,L"other" }, 2, new LongC::Sub(Long, argExcept));
	Long->addFunc(L"mult", number, new Arg[2]{ Long,L"this", number,L"other" }, 2, new LongC::Mult(Long, argExcept));
	Long->addFunc(L"div", number, new Arg[2]{ Long,L"this", number,L"other" }, 2, new LongC::Div(Long, argExcept));
	Long->addFunc(L"mod", number, new Arg[2]{ Long,L"this", number,L"other" }, 2, new LongC::Mod(Long, argExcept));

	Double->addFunc(L"equals", Bool, new Arg[2]{ Double,L"this", Double,L"c" }, 2, new DoubleC::Equals(Bool));
	Double->addFunc(L"Double", Double, new Arg[2]{ Double,L"this", Double,L"c" }, 2, new DoubleC::DoubleConstruct(Double));
	Double->addFunc(L"toString", String, new Arg[1]{ Double,L"this" }, 1, new DoubleC::ToString(String));
	Double->addFunc(L"toByte", Byte, new Arg[1]{ Double,L"this" }, 1, new DoubleC::toByte(Byte));
	Double->addFunc(L"toShort", Short, new Arg[1]{ Double,L"this" }, 1, new DoubleC::toShort(Short));
	Double->addFunc(L"toInteger", Double, new Arg[1]{ Double,L"this" }, 1, new DoubleC::toDouble(Double));
	Double->addFunc(L"toFloat", Float, new Arg[1]{ Double,L"this" }, 1, new DoubleC::toFloat(Float));
	Double->addFunc(L"toLong", Long, new Arg[1]{ Double,L"this" }, 1, new DoubleC::toLong(Long));
	Double->addFunc(L"toDouble", Double, new Arg[1]{ Double,L"this" }, 1, new DoubleC::toDouble(Double));
	Double->addFunc(L"lt", Bool, new Arg[2]{ Double,L"this", number,L"other" }, 2, new DoubleC::LessThan(Bool, argExcept));
	Double->addFunc(L"lte", Bool, new Arg[2]{ Double,L"this", number,L"other" }, 2, new DoubleC::LessThanEquals(Bool, argExcept));
	Double->addFunc(L"gt", Bool, new Arg[2]{ Double,L"this", number,L"other" }, 2, new DoubleC::GreaterThan(Bool, argExcept));
	Double->addFunc(L"gte", Bool, new Arg[2]{ Double,L"this", number,L"other" }, 2, new DoubleC::GreaterThanEquals(Bool, argExcept));
	Double->addFunc(L"add", number, new Arg[2]{ Double,L"this", number,L"other" }, 2, new DoubleC::Add(Double, argExcept));
	Double->addFunc(L"sub", number, new Arg[2]{ Double,L"this", number,L"other" }, 2, new DoubleC::Sub(Double, argExcept));
	Double->addFunc(L"mult", number, new Arg[2]{ Double,L"this", number,L"other" }, 2, new DoubleC::Mult(Double, argExcept));
	Double->addFunc(L"div", number, new Arg[2]{ Double,L"this", number,L"other" }, 2, new DoubleC::Div(Double, argExcept));
	Double->addFunc(L"mod", number, new Arg[2]{ Double,L"this", number,L"other" }, 2, new DoubleC::Mod(Double, argExcept));

	classClass->addFunc(L"Class", classClass, new Arg[2]{ classClass,L"this", file,L"c" }, 2, new ClassClass::ClassConstruct(classClass, parser));
	classClass->addFunc(L"name", String, new Arg[1]{ classClass,L"this" }, 1, new ClassClass::GetName(String));
	classClass->addFunc(L"statVar", nullptr, new Arg[2]{ classClass,L"this", String,L"c" }, 2, new ClassClass::GetStatVar(String));
	classClass->addFunc(L"invoke", nullptr, new Arg[2]{ classClass,L"this", String,L"func" }, 2, new ClassClass::Invoke(String), true);
	classClass->addFunc(L"instanceOf", Bool, new Arg[2]{ classClass,L"this", classClass,L"c" }, 2, new ClassClass::InstanceOf(Bool));
	classClass->addFunc(L"equals", Bool, new Arg[2]{ classClass,L"this", classClass,L"c" }, 2, new ClassClass::Equals(Bool));
	classClass->addFunc(L"toString", String, new Arg[1]{ classClass,L"this" }, 1, new ClassClass::ToString(String));

	func->addFunc(L"Function", func, new Arg[2]{ func,L"this", func,L"c" }, 2, new FunctionClass::FunctionConstruct(func));
	func->addFunc(L"execute", String, new Arg[1]{ func,L"this" }, 1, new FunctionClass::Execute(func), true);
	func->addFunc(L"equals", Bool, new Arg[2]{ func,L"this", func,L"c" }, 2, new FunctionClass::Equals(Bool));
	func->addFunc(L"toString", String, new Arg[1]{ func,L"this" }, 1, new FunctionClass::ToString(String));

	thread->addFunc(L"Thread", thread, new Arg[2]{ thread,L"this", func,L"func" }, 2, new ThreadC::ThreadConstruct(thread), true);
	thread->addFunc(L"hardware_concurrency", nullptr, new Arg[0]{ }, 0, new ThreadC::HardwareConcurrency(Integer));
	thread->addFunc(L"equals", Bool, new Arg[2]{ thread,L"this", thread,L"c" }, 2, new ThreadC::Equals(Bool));
	thread->addFunc(L"toString", String, new Arg[1]{ thread,L"this" }, 1, new ThreadC::ToString(String));
	thread->addFunc(L"start", nullptr, new Arg[1]{ thread,L"this" }, 1, new ThreadC::Start(String));
	thread->addFunc(L"join", nullptr, new Arg[1]{ thread,L"this" }, 1, new ThreadC::Join(String));
	//thread->addFunc(L"detach",nullptr, new Arg[1]{ thread,L"this" }, 1, new ThreadC::Detach(String));

	lock->addFunc(L"Lock", lock, new Arg[1]{ lock,L"this" }, 1, new LockC::LockConstruct(lock));
	lock->addFunc(L"equals", Bool, new Arg[2]{ lock,L"this", lock,L"c" }, 2, new LockC::Equals(Bool));
	lock->addFunc(L"toString", String, new Arg[1]{ lock,L"this" }, 1, new LockC::ToString(String));
	lock->addFunc(L"lock", nullptr, new Arg[1]{ lock,L"this" }, 1, new LockC::Lock(String));
	lock->addFunc(L"unLock", nullptr, new Arg[1]{ lock,L"this" }, 1, new LockC::Unlock(String));

	file->addFunc(L"File", file, new Arg[2]{ file,L"this", file,L"c" }, 2, new FileC::FileCopy(file));
	file->addFunc(L"File", file, new Arg[2]{ file,L"this", String,L"c" }, 2, new FileC::FileConstruct(file));
	file->addFunc(L"equals", Bool, new Arg[2]{ file,L"this", file,L"c" }, 2, new FileC::Equals(Bool));
	file->addFunc(L"toString", String, new Arg[1]{ file,L"this" }, 1, new FileC::ToString(String));
	file->addFunc(L"getRelativePath", String, new Arg[1]{ file,L"this" }, 1, new FileC::GetRelativePath(String));
	file->addFunc(L"getAbsolutePath", String, new Arg[1]{ file,L"this" }, 1, new FileC::GetAbsolutePath(String));
	Array::ArrayC* fileArray{ (Array::ArrayC*)GLOBAL::getClasses()->checkGetClass(std::wstring(Paths::Array) + L"<" + std::wstring(Paths::File) + L">") };
	file->addFunc(L"getFiles", fileArray, new Arg[1]{ file,L"this" }, 1, new FileC::GetFiles(file, fileArray), true);
	file->addFunc(L"getName", String, new Arg[1]{ file,L"this" }, 1, new FileC::GetName(String));
	file->addFunc(L"getExtension", String, new Arg[1]{ file,L"this" }, 1, new FileC::GetExtension(String));
	file->addFunc(L"isDirectory", Bool, new Arg[1]{ file,L"this" }, 1, new FileC::IsDirectory(Bool));

	except->addFunc(L"equals", Bool, new Arg[2]{ except,L"this", except,L"c" }, 2, new ExceptionC::Equals(Bool));
	except->addFunc(L"Exception", except, new Arg[2]{ except,L"this", String,L"c" }, 2, new ExceptionC::ExceptionConstruct(except));
	except->addFunc(L"toString", String, new Arg[1]{ except,L"this" }, 1, new ExceptionC::ToString(String));

	castExcept->addFunc(L"CastException", castExcept, new Arg[2]{ castExcept,L"this", String,L"c" }, 2, new CastExceptionC::ExceptionConstruct(castExcept));
	nullExcept->addFunc(L"NullException", nullExcept, new Arg[2]{ nullExcept,L"this", String,L"c" }, 2, new NullException::ExceptionConstruct(nullExcept));
	argExcept->addFunc(L"IllegalArgumentException", argExcept, new Arg[2]{ argExcept,L"this", String,L"c" }, 2, new IllegalArgumentExceptionC::ExceptionConstruct(argExcept));
}