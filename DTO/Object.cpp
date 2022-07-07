#include "Object.h"
#include "MemoryObject.h"
#include "MemoryVar.h"
#include "Class.h"
#include "Var.h"
#include "GarbageCollector.h"
#include "NullException.h"

DTO::Object::Object(Class* type, IObject** vars, size_t size)
	:IObject(type), m_vars(vars), m_size(size)
{
}

DTO::Object::Object(Class* type) : IObject(type), m_size(m_type->getVars()->size()) {
	m_vars = new IObject * [m_size];
	for (size_t c{ 0 }; c < m_size; c++) {
		if (m_type->getVars()->get(c)->m_default == nullptr) {
			m_vars[c] = nullptr;
		}
		else {
			MemoryObject m{ };
			CommandReturn* r{ m_type->getVars()->get(c)->m_default->exec(m) };
			m_vars[c] = r->getObject();
			GarbageCollector::Add(m_vars[c]);
			delete r;
		}
	}
}

DTO::Object::~Object() {
	for (size_t c{ 0 }; c < m_size; c++)
		GarbageCollector::Remove(m_vars[c]);
	delete[] m_vars;
}

DTO::IObject* DTO::Object::get(std::string name) {
	size_t i{ m_type->getVars()->get(name)->m_index };
	if (i >= m_size)
		throw "OutOfBound";
	return m_vars[i];
}

DTO::IObject* DTO::Object::get(size_t i) {
	if (i >= m_size)
		throw "OutOfBound";
	return m_vars[i];
}

void DTO::Object::set(size_t i, IObject* obj)
{
	if (i >= m_size)
		throw "OutOfBound";
	GarbageCollector::Remove(m_vars[i]);
	GarbageCollector::Add(obj);
	m_vars[i] = obj;
}

DTO::IObject* DTO::Object::clone()
{
	IObject** vars{ new IObject * [m_size] };
	for (size_t c(0); c < m_size; c++) {
		vars[c] = m_vars[c]->clone();
		GarbageCollector::Add(vars[c]);
	}
	return new Object(m_type, vars, m_size);
}

CommandReturn* DTO::Object::exec(std::string name, IObject** args, size_t argsSize)
{
	Interface** i{ new Interface * [argsSize] };
	for (size_t c{ 0 }; c < argsSize; c++) {
		i[c] = args[c]->getClass();
	}
	MemoryObject mem2{};
	CommandReturn* q{ getClass()->getFuncs()->get(name, i, argsSize)->exec(mem2, args, argsSize) };
	delete[] i;
	return q;
}

CommandReturn* DTO::NullObject::exec(std::string name, IObject** args, size_t argsSize)
{
	return new CommandReturn(new NullExceptionO("Can't execute a function on a null object."), true, true);
}

CommandReturn* DTO::Object::exec(std::string name, IObject* arg)
{
	IObject** o{ new IObject * [1]{arg} };
	Interface** i{ new Interface * [1]{arg->getClass()} };
	MemoryObject mem2{};
	CommandReturn* q{ getClass()->getFuncs()->get(name, i, 1)->exec(mem2, o, 1) };
	delete[] i;
	delete[] o;
	return q;
}

CommandReturn* DTO::NullObject::exec(std::string name, IObject* arg)
{
	return new CommandReturn(new NullExceptionO("Can't execute a function on a null object."), true, true);
}
