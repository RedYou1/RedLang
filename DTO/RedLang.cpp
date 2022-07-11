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
#include "SizedArray.h"
#include "List.h"
#include "ArrayList.h"

void DTO::RedLang::importRedLang(SourceFile* (*parser)(std::string)) {
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
	GLOBAL::getClasses()->add(Paths::SizedArray, new SizedArray());
	GLOBAL::getClasses()->add(Paths::List, new List());
	GLOBAL::getClasses()->add(Paths::ArrayList, new ArrayList());

	object->addFunc("equals", Bool, new Arg[2]{ object,"this", object,"c" }, 2, new ObjectClass::Equals(Bool));
	object->addFunc("toString", String, new Arg[1]{ object,"this" }, 1, new ObjectClass::ToString(String));
	object->addFunc("getClass", classClass, new Arg[1]{ object,"this" }, 1, new ObjectClass::GetClass(classClass));
	object->addFunc("hashCode", Long, new Arg[1]{ object,"this" }, 1, new ObjectClass::HashCode(Long));

	Bool->addFunc("not", Bool, new Arg[1]{ Bool,"this" }, 1, new BooleanC::Not(Bool));
	Bool->addFunc("equals", Bool, new Arg[2]{ Bool,"this", Bool,"c" }, 2, new BooleanC::Equals(Bool));
	Bool->addFunc("Boolean", Bool, new Arg[2]{ Bool,"this", Bool,"c" }, 2, new BooleanC::BooleanConstruct(Bool));
	Bool->addFunc("toString", String, new Arg[1]{ Bool,"this" }, 1, new BooleanC::ToString(String));

	Char->addFunc("equals", Bool, new Arg[2]{ Char,"this", Char,"c" }, 2, new CharC::Equals(Bool));
	Char->addFunc("Char", Char, new Arg[2]{ Char,"this", Char,"c" }, 2, new CharC::CharConstruct(Char));
	Char->addFunc("toString", String, new Arg[1]{ Char,"this" }, 1, new CharC::ToString(String));

	String->addFunc("equals", Bool, new Arg[2]{ String,"this", String,"c" }, 2, new StringC::Equals(Bool));
	String->addFunc("String", String, new Arg[2]{ String,"this", Char,"c" }, 2, new StringC::CharConstruct(String));
	String->addFunc("String", String, new Arg[2]{ String,"this", String,"c" }, 2, new StringC::StringConstruct(String));
	String->addFunc("concat", String, new Arg[2]{ String,"this", object,"other" }, 2, new StringC::Concat(String));
	String->addFunc("toString", String, new Arg[1]{ String,"this" }, 1, new StringC::ToString(String));

	system->addFunc("getWord", String, new Arg[0]{ }, 0, new System::GetWord(String));
	system->addFunc("print", nullptr, new Arg[1]{ object,"s" }, 1, new System::print());
	system->addFunc("println", nullptr, new Arg[1]{ object,"s" }, 1, new System::println());
	system->getStatVars()->add("is32x", new StatVar(Bool, new BooleanO(Bool, sizeof(intptr_t) == 4)));
	system->getStatVars()->add("is64x", new StatVar(Bool, new BooleanO(Bool, sizeof(intptr_t) == 8)));

	number->add("toByte", new Signature("", Byte, new Arg[1]{ number,"this" }, 1));
	number->add("toShort", new Signature("", Short, new Arg[1]{ number,"this" }, 1));
	number->add("toInteger", new Signature("", Integer, new Arg[1]{ number,"this" }, 1));
	number->add("toFloat", new Signature("", Float, new Arg[1]{ number,"this" }, 1));
	number->add("toLong", new Signature("", Long, new Arg[1]{ number,"this" }, 1));
	number->add("toDouble", new Signature("", Double, new Arg[1]{ number,"this" }, 1));
	number->add("lt", new Signature("", Bool, new Arg[2]{ number,"this", number,"other" }, 2));
	number->add("lte", new Signature("", Bool, new Arg[2]{ number,"this", number,"other" }, 2));
	number->add("gt", new Signature("", Bool, new Arg[2]{ number,"this", number,"other" }, 2));
	number->add("gte", new Signature("", Bool, new Arg[2]{ number,"this", number,"other" }, 2));
	number->add("add", new Signature("", number, new Arg[2]{ number,"this", number,"other" }, 2));
	number->add("sub", new Signature("", number, new Arg[2]{ number,"this", number,"other" }, 2));
	number->add("mult", new Signature("", number, new Arg[2]{ number,"this", number,"other" }, 2));
	number->add("div", new Signature("", number, new Arg[2]{ number,"this", number,"other" }, 2));
	number->add("mod", new Signature("", number, new Arg[2]{ number,"this", number,"other" }, 2));

	Byte->addFunc("equals", Bool, new Arg[2]{ Byte,"this", Byte,"c" }, 2, new ByteC::Equals(Bool));
	Byte->addFunc("Byte", Byte, new Arg[2]{ Byte,"this", Byte,"c" }, 2, new ByteC::ByteConstruct(Byte));
	Byte->addFunc("toString", String, new Arg[1]{ Byte,"this" }, 1, new ByteC::ToString(String));
	Byte->addFunc("toByte", Byte, new Arg[1]{ Byte,"this" }, 1, new ByteC::toByte(Byte));
	Byte->addFunc("toShort", Short, new Arg[1]{ Byte,"this" }, 1, new ByteC::toShort(Short));
	Byte->addFunc("toInteger", Byte, new Arg[1]{ Byte,"this" }, 1, new ByteC::toByte(Byte));
	Byte->addFunc("toFloat", Float, new Arg[1]{ Byte,"this" }, 1, new ByteC::toFloat(Float));
	Byte->addFunc("toLong", Long, new Arg[1]{ Byte,"this" }, 1, new ByteC::toLong(Long));
	Byte->addFunc("toDouble", Double, new Arg[1]{ Byte,"this" }, 1, new ByteC::toDouble(Double));
	Byte->addFunc("lt", Bool, new Arg[2]{ Byte,"this", number,"other" }, 2, new ByteC::LessThan(Bool, argExcept));
	Byte->addFunc("lte", Bool, new Arg[2]{ Byte,"this", number,"other" }, 2, new ByteC::LessThanEquals(Bool, argExcept));
	Byte->addFunc("gt", Bool, new Arg[2]{ Byte,"this", number,"other" }, 2, new ByteC::GreaterThan(Bool, argExcept));
	Byte->addFunc("gte", Bool, new Arg[2]{ Byte,"this", number,"other" }, 2, new ByteC::GreaterThanEquals(Bool, argExcept));
	Byte->addFunc("add", number, new Arg[2]{ Byte,"this", number,"other" }, 2, new ByteC::Add(Byte, argExcept));
	Byte->addFunc("sub", number, new Arg[2]{ Byte,"this", number,"other" }, 2, new ByteC::Sub(Byte, argExcept));
	Byte->addFunc("mult", number, new Arg[2]{ Byte,"this", number,"other" }, 2, new ByteC::Mult(Byte, argExcept));
	Byte->addFunc("div", number, new Arg[2]{ Byte,"this", number,"other" }, 2, new ByteC::Div(Byte, argExcept));
	Byte->addFunc("mod", number, new Arg[2]{ Byte,"this", number,"other" }, 2, new ByteC::Mod(Byte, argExcept));

	Short->addFunc("equals", Bool, new Arg[2]{ Short,"this", Short,"c" }, 2, new ShortC::Equals(Bool));
	Short->addFunc("Short", Short, new Arg[2]{ Short,"this", Short,"c" }, 2, new ShortC::ShortConstruct(Short));
	Short->addFunc("toString", String, new Arg[1]{ Short,"this" }, 1, new ShortC::ToString(String));
	Short->addFunc("toByte", Byte, new Arg[1]{ Short,"this" }, 1, new ShortC::toByte(Byte));
	Short->addFunc("toShort", Short, new Arg[1]{ Short,"this" }, 1, new ShortC::toShort(Short));
	Short->addFunc("toInteger", Short, new Arg[1]{ Short,"this" }, 1, new ShortC::toShort(Short));
	Short->addFunc("toFloat", Float, new Arg[1]{ Short,"this" }, 1, new ShortC::toFloat(Float));
	Short->addFunc("toLong", Long, new Arg[1]{ Short,"this" }, 1, new ShortC::toLong(Long));
	Short->addFunc("toDouble", Double, new Arg[1]{ Short,"this" }, 1, new ShortC::toDouble(Double));
	Short->addFunc("lt", Bool, new Arg[2]{ Short,"this", number,"other" }, 2, new ShortC::LessThan(Bool, argExcept));
	Short->addFunc("lte", Bool, new Arg[2]{ Short,"this", number,"other" }, 2, new ShortC::LessThanEquals(Bool, argExcept));
	Short->addFunc("gt", Bool, new Arg[2]{ Short,"this", number,"other" }, 2, new ShortC::GreaterThan(Bool, argExcept));
	Short->addFunc("gte", Bool, new Arg[2]{ Short,"this", number,"other" }, 2, new ShortC::GreaterThanEquals(Bool, argExcept));
	Short->addFunc("add", number, new Arg[2]{ Short,"this", number,"other" }, 2, new ShortC::Add(Short, argExcept));
	Short->addFunc("sub", number, new Arg[2]{ Short,"this", number,"other" }, 2, new ShortC::Sub(Short, argExcept));
	Short->addFunc("mult", number, new Arg[2]{ Short,"this", number,"other" }, 2, new ShortC::Mult(Short, argExcept));
	Short->addFunc("div", number, new Arg[2]{ Short,"this", number,"other" }, 2, new ShortC::Div(Short, argExcept));
	Short->addFunc("mod", number, new Arg[2]{ Short,"this", number,"other" }, 2, new ShortC::Mod(Short, argExcept));

	Integer->addFunc("equals", Bool, new Arg[2]{ Integer,"this", Integer,"c" }, 2, new IntegerC::Equals(Bool));
	Integer->addFunc("Integer", Integer, new Arg[2]{ Integer,"this", Integer,"c" }, 2, new IntegerC::IntegerConstruct(Integer));
	Integer->addFunc("toString", String, new Arg[1]{ Integer,"this" }, 1, new IntegerC::ToString(String));
	Integer->addFunc("toByte", Byte, new Arg[1]{ Integer,"this" }, 1, new IntegerC::toByte(Byte));
	Integer->addFunc("toShort", Short, new Arg[1]{ Integer,"this" }, 1, new IntegerC::toShort(Short));
	Integer->addFunc("toInteger", Integer, new Arg[1]{ Integer,"this" }, 1, new IntegerC::toInteger(Integer));
	Integer->addFunc("toFloat", Float, new Arg[1]{ Integer,"this" }, 1, new IntegerC::toFloat(Float));
	Integer->addFunc("toLong", Long, new Arg[1]{ Integer,"this" }, 1, new IntegerC::toLong(Long));
	Integer->addFunc("toDouble", Double, new Arg[1]{ Integer,"this" }, 1, new IntegerC::toDouble(Double));
	Integer->addFunc("lt", Bool, new Arg[2]{ Integer,"this", number,"other" }, 2, new IntegerC::LessThan(Bool, argExcept));
	Integer->addFunc("lte", Bool, new Arg[2]{ Integer,"this", number,"other" }, 2, new IntegerC::LessThanEquals(Bool, argExcept));
	Integer->addFunc("gt", Bool, new Arg[2]{ Integer,"this", number,"other" }, 2, new IntegerC::GreaterThan(Bool, argExcept));
	Integer->addFunc("gte", Bool, new Arg[2]{ Integer,"this", number,"other" }, 2, new IntegerC::GreaterThanEquals(Bool, argExcept));
	Integer->addFunc("add", number, new Arg[2]{ Integer,"this", number,"other" }, 2, new IntegerC::Add(Integer, argExcept));
	Integer->addFunc("sub", number, new Arg[2]{ Integer,"this", number,"other" }, 2, new IntegerC::Sub(Integer, argExcept));
	Integer->addFunc("mult", number, new Arg[2]{ Integer,"this", number,"other" }, 2, new IntegerC::Mult(Integer, argExcept));
	Integer->addFunc("div", number, new Arg[2]{ Integer,"this", number,"other" }, 2, new IntegerC::Div(Integer, argExcept));
	Integer->addFunc("mod", number, new Arg[2]{ Integer,"this", number,"other" }, 2, new IntegerC::Mod(Integer, argExcept));

	Float->addFunc("equals", Bool, new Arg[2]{ Float,"this", Float,"c" }, 2, new FloatC::Equals(Bool));
	Float->addFunc("Float", Float, new Arg[2]{ Float,"this", Float,"c" }, 2, new FloatC::FloatConstruct(Float));
	Float->addFunc("toString", String, new Arg[1]{ Float,"this" }, 1, new FloatC::ToString(String));
	Float->addFunc("toByte", Byte, new Arg[1]{ Float,"this" }, 1, new FloatC::toByte(Byte));
	Float->addFunc("toShort", Short, new Arg[1]{ Float,"this" }, 1, new FloatC::toShort(Short));
	Float->addFunc("toInteger", Float, new Arg[1]{ Float,"this" }, 1, new FloatC::toFloat(Float));
	Float->addFunc("toFloat", Float, new Arg[1]{ Float,"this" }, 1, new FloatC::toFloat(Float));
	Float->addFunc("toLong", Long, new Arg[1]{ Float,"this" }, 1, new FloatC::toLong(Long));
	Float->addFunc("toDouble", Double, new Arg[1]{ Float,"this" }, 1, new FloatC::toDouble(Double));
	Float->addFunc("lt", Bool, new Arg[2]{ Float,"this", number,"other" }, 2, new FloatC::LessThan(Bool, argExcept));
	Float->addFunc("lte", Bool, new Arg[2]{ Float,"this", number,"other" }, 2, new FloatC::LessThanEquals(Bool, argExcept));
	Float->addFunc("gt", Bool, new Arg[2]{ Float,"this", number,"other" }, 2, new FloatC::GreaterThan(Bool, argExcept));
	Float->addFunc("gte", Bool, new Arg[2]{ Float,"this", number,"other" }, 2, new FloatC::GreaterThanEquals(Bool, argExcept));
	Float->addFunc("add", number, new Arg[2]{ Float,"this", number,"other" }, 2, new FloatC::Add(Float, argExcept));
	Float->addFunc("sub", number, new Arg[2]{ Float,"this", number,"other" }, 2, new FloatC::Sub(Float, argExcept));
	Float->addFunc("mult", number, new Arg[2]{ Float,"this", number,"other" }, 2, new FloatC::Mult(Float, argExcept));
	Float->addFunc("div", number, new Arg[2]{ Float,"this", number,"other" }, 2, new FloatC::Div(Float, argExcept));
	Float->addFunc("mod", number, new Arg[2]{ Float,"this", number,"other" }, 2, new FloatC::Mod(Float, argExcept));

	Long->addFunc("equals", Bool, new Arg[2]{ Long,"this", Long,"c" }, 2, new LongC::Equals(Bool));
	Long->addFunc("Long", Long, new Arg[2]{ Long,"this", Long,"c" }, 2, new LongC::LongConstruct(Long));
	Long->addFunc("toString", String, new Arg[1]{ Long,"this" }, 1, new LongC::ToString(String));
	Long->addFunc("toByte", Byte, new Arg[1]{ Long,"this" }, 1, new LongC::toByte(Byte));
	Long->addFunc("toShort", Short, new Arg[1]{ Long,"this" }, 1, new LongC::toShort(Short));
	Long->addFunc("toInteger", Long, new Arg[1]{ Long,"this" }, 1, new LongC::toLong(Long));
	Long->addFunc("toFloat", Float, new Arg[1]{ Long,"this" }, 1, new LongC::toFloat(Float));
	Long->addFunc("toLong", Long, new Arg[1]{ Long,"this" }, 1, new LongC::toLong(Long));
	Long->addFunc("toDouble", Double, new Arg[1]{ Long,"this" }, 1, new LongC::toDouble(Double));
	Long->addFunc("lt", Bool, new Arg[2]{ Long,"this", number,"other" }, 2, new LongC::LessThan(Bool, argExcept));
	Long->addFunc("lte", Bool, new Arg[2]{ Long,"this", number,"other" }, 2, new LongC::LessThanEquals(Bool, argExcept));
	Long->addFunc("gt", Bool, new Arg[2]{ Long,"this", number,"other" }, 2, new LongC::GreaterThan(Bool, argExcept));
	Long->addFunc("gte", Bool, new Arg[2]{ Long,"this", number,"other" }, 2, new LongC::GreaterThanEquals(Bool, argExcept));
	Long->addFunc("add", number, new Arg[2]{ Long,"this", number,"other" }, 2, new LongC::Add(Long, argExcept));
	Long->addFunc("sub", number, new Arg[2]{ Long,"this", number,"other" }, 2, new LongC::Sub(Long, argExcept));
	Long->addFunc("mult", number, new Arg[2]{ Long,"this", number,"other" }, 2, new LongC::Mult(Long, argExcept));
	Long->addFunc("div", number, new Arg[2]{ Long,"this", number,"other" }, 2, new LongC::Div(Long, argExcept));
	Long->addFunc("mod", number, new Arg[2]{ Long,"this", number,"other" }, 2, new LongC::Mod(Long, argExcept));

	Double->addFunc("equals", Bool, new Arg[2]{ Double,"this", Double,"c" }, 2, new DoubleC::Equals(Bool));
	Double->addFunc("Double", Double, new Arg[2]{ Double,"this", Double,"c" }, 2, new DoubleC::DoubleConstruct(Double));
	Double->addFunc("toString", String, new Arg[1]{ Double,"this" }, 1, new DoubleC::ToString(String));
	Double->addFunc("toByte", Byte, new Arg[1]{ Double,"this" }, 1, new DoubleC::toByte(Byte));
	Double->addFunc("toShort", Short, new Arg[1]{ Double,"this" }, 1, new DoubleC::toShort(Short));
	Double->addFunc("toInteger", Double, new Arg[1]{ Double,"this" }, 1, new DoubleC::toDouble(Double));
	Double->addFunc("toFloat", Float, new Arg[1]{ Double,"this" }, 1, new DoubleC::toFloat(Float));
	Double->addFunc("toLong", Long, new Arg[1]{ Double,"this" }, 1, new DoubleC::toLong(Long));
	Double->addFunc("toDouble", Double, new Arg[1]{ Double,"this" }, 1, new DoubleC::toDouble(Double));
	Double->addFunc("lt", Bool, new Arg[2]{ Double,"this", number,"other" }, 2, new DoubleC::LessThan(Bool, argExcept));
	Double->addFunc("lte", Bool, new Arg[2]{ Double,"this", number,"other" }, 2, new DoubleC::LessThanEquals(Bool, argExcept));
	Double->addFunc("gt", Bool, new Arg[2]{ Double,"this", number,"other" }, 2, new DoubleC::GreaterThan(Bool, argExcept));
	Double->addFunc("gte", Bool, new Arg[2]{ Double,"this", number,"other" }, 2, new DoubleC::GreaterThanEquals(Bool, argExcept));
	Double->addFunc("add", number, new Arg[2]{ Double,"this", number,"other" }, 2, new DoubleC::Add(Double, argExcept));
	Double->addFunc("sub", number, new Arg[2]{ Double,"this", number,"other" }, 2, new DoubleC::Sub(Double, argExcept));
	Double->addFunc("mult", number, new Arg[2]{ Double,"this", number,"other" }, 2, new DoubleC::Mult(Double, argExcept));
	Double->addFunc("div", number, new Arg[2]{ Double,"this", number,"other" }, 2, new DoubleC::Div(Double, argExcept));
	Double->addFunc("mod", number, new Arg[2]{ Double,"this", number,"other" }, 2, new DoubleC::Mod(Double, argExcept));

	classClass->addFunc("Class", classClass, new Arg[2]{ classClass,"this", String,"c" }, 2, new ClassClass::ClassConstruct(classClass, parser));
	classClass->addFunc("name", String, new Arg[1]{ classClass,"this" }, 1, new ClassClass::GetName(String));
	classClass->addFunc("statVar", nullptr, new Arg[2]{ classClass,"this", String,"c" }, 2, new ClassClass::GetStatVar(String));
	classClass->addFunc("invoke", nullptr, new Arg[2]{ classClass,"this", String,"func" }, 2, new ClassClass::Invoke(String), true);
	classClass->addFunc("instanceOf", Bool, new Arg[2]{ classClass,"this", classClass,"c" }, 2, new ClassClass::InstanceOf(Bool));
	classClass->addFunc("equals", Bool, new Arg[2]{ classClass,"this", classClass,"c" }, 2, new ClassClass::Equals(Bool));
	classClass->addFunc("toString", String, new Arg[1]{ classClass,"this" }, 1, new ClassClass::ToString(String));

	func->addFunc("Function", func, new Arg[2]{ func,"this", func,"c" }, 2, new FunctionClass::FunctionConstruct(func));
	func->addFunc("execute", String, new Arg[1]{ func,"this" }, 1, new FunctionClass::Execute(func), true);
	func->addFunc("equals", Bool, new Arg[2]{ func,"this", func,"c" }, 2, new FunctionClass::Equals(Bool));
	func->addFunc("toString", String, new Arg[1]{ func,"this" }, 1, new FunctionClass::ToString(String));

	thread->addFunc("Thread", thread, new Arg[2]{ thread,"this", func,"func" }, 2, new ThreadC::ThreadConstruct(thread), true);
	thread->addFunc("hardware_concurrency", nullptr, new Arg[0]{ }, 0, new ThreadC::HardwareConcurrency(Integer));
	thread->addFunc("equals", Bool, new Arg[2]{ thread,"this", thread,"c" }, 2, new ThreadC::Equals(Bool));
	thread->addFunc("toString", String, new Arg[1]{ thread,"this" }, 1, new ThreadC::ToString(String));
	thread->addFunc("start", nullptr, new Arg[1]{ thread,"this" }, 1, new ThreadC::Start(String));
	thread->addFunc("join", nullptr, new Arg[1]{ thread,"this" }, 1, new ThreadC::Join(String));
	//thread->addFunc("detach",nullptr, new Arg[1]{ thread,"this" }, 1, new ThreadC::Detach(String));

	lock->addFunc("Lock", lock, new Arg[1]{ lock,"this" }, 1, new LockC::LockConstruct(lock));
	lock->addFunc("equals", Bool, new Arg[2]{ lock,"this", lock,"c" }, 2, new LockC::Equals(Bool));
	lock->addFunc("toString", String, new Arg[1]{ lock,"this" }, 1, new LockC::ToString(String));
	lock->addFunc("lock", nullptr, new Arg[1]{ lock,"this" }, 1, new LockC::Lock(String));
	lock->addFunc("unLock", nullptr, new Arg[1]{ lock,"this" }, 1, new LockC::Unlock(String));

	except->addFunc("equals", Bool, new Arg[2]{ except,"this", except,"c" }, 2, new ExceptionC::Equals(Bool));
	except->addFunc("Exception", except, new Arg[2]{ except,"this", String,"c" }, 2, new ExceptionC::ExceptionConstruct(except));
	except->addFunc("toString", String, new Arg[1]{ except,"this" }, 1, new ExceptionC::ToString(String));

	castExcept->addFunc("CastException", castExcept, new Arg[2]{ castExcept,"this", String,"c" }, 2, new CastExceptionC::ExceptionConstruct(castExcept));
	nullExcept->addFunc("NullException", nullExcept, new Arg[2]{ nullExcept,"this", String,"c" }, 2, new NullException::ExceptionConstruct(nullExcept));
	argExcept->addFunc("IllegalArgumentException", argExcept, new Arg[2]{ argExcept,"this", String,"c" }, 2, new IllegalArgumentExceptionC::ExceptionConstruct(argExcept));
}