#include "Object.h"
#include "Command.h"
#include "MemoryObject.h"
#include "MemoryFunction.h"
#include "MemoryVar.h"
#include "Class.h"
#include "Var.h"
#include "NullException.h"

std::mutex DTO::IObject::s_mutex{};
size_t DTO::IObject::s_isRunning{ 0 };
std::thread::id DTO::IObject::s_current_id{ std::this_thread::get_id() };
std::lock_guard<std::mutex>* DTO::IObject::s_lock{ nullptr };

void DTO::IObject::lock() {
	if (s_isRunning == 0 || s_current_id != std::this_thread::get_id()) {
		s_lock = new std::lock_guard<std::mutex>(s_mutex);
		s_current_id = std::this_thread::get_id();
	}
	s_isRunning++;
}

void DTO::IObject::unLock() {
	s_isRunning--;
	if (s_isRunning == 0)
		delete s_lock;
}

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
			m_vars[c]->addRef();
			delete r;
		}
	}
}

DTO::Object::~Object() {
	for (size_t c{ 0 }; c < m_size; c++)
		m_vars[c]->removeRef();
	delete[] m_vars;
}

DTO::IObject* DTO::Object::get(std::wstring name) {
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
	m_vars[i]->removeRef();
	obj->addRef();
	m_vars[i] = obj;
}

DTO::IObject* DTO::Object::clone()
{
	IObject** vars{ new IObject * [m_size] };
	for (size_t c(0); c < m_size; c++) {
		vars[c] = m_vars[c]->clone();
		vars[c]->addRef();
	}
	return new Object(m_type, vars, m_size);
}

DTO::CommandReturn* DTO::Object::exec(std::wstring name, IObject** args, size_t argsSize)
{
	Instanciable** i{ new Instanciable * [argsSize] };
	for (size_t c{ 0 }; c < argsSize; c++) {
		i[c] = args[c]->getClass();
	}
	MemoryObject mem2{};
	CommandReturn* q{ getClass()->getFuncs()->get(name, i, argsSize)->exec(mem2, args, argsSize) };
	delete[] i;
	return q;
}

DTO::CommandReturn* DTO::NullObject::exec(std::wstring name, IObject** args, size_t argsSize)
{
	return new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Can't execute a function on a null object."), true, true);
}

DTO::CommandReturn* DTO::Object::exec(std::wstring name, IObject* arg)
{
	IObject** o{ new IObject * [1]{arg} };
	Instanciable** i{ new Instanciable * [1]{arg->getClass()} };
	MemoryObject mem2{};
	CommandReturn* q{ getClass()->getFuncs()->get(name, i, 1)->exec(mem2, o, 1) };
	delete[] i;
	delete[] o;
	return q;
}

DTO::CommandReturn* DTO::NullObject::exec(std::wstring name, IObject* arg)
{
	return new CommandReturn(new NullExceptionO(GLOBAL::getClasses()->getClass(Paths::NullException), L"Can't execute a function on a null object."), true, true);
}

void DTO::IObject::addRef()
{
	if (this == nullptr)
		return;

	lock();

	m_refs++;

	unLock();
}

void DTO::IObject::removeRef()
{
	if (this == nullptr)
		return;

	lock();

	if (m_refs <= 1) {
		delete this;
	}
	else
		m_refs--;

	unLock();
}

size_t DTO::IObject::GetAmountRef()
{
	return m_refs;
}
