#pragma once
#include <string>
#include "MemoryObject.h"
#include "Object.h"
#include "Command.h"
#include "Class.h"

class ReplaceStatVar : public Command {
private:
	Class* m_class;
	std::string m_name;
	Command* m_value;
public:
	ReplaceStatVar(Class* _class, std::string name, Command* value);
	virtual ~ReplaceStatVar() override { delete m_value; }

	CommandReturn* exec(MemoryObject& mem) override;
	Command* clone() override;
};