#pragma once
#include <string>


namespace DTO {
	class IObject;
	class MemoryObject;

	class Command;

	class CommandReturn {
	private:
		IObject* m_object;
		bool m_return;
		bool m_throw;
	public:
		CommandReturn(IObject* object, bool _return, bool _throw);
		CommandReturn(CommandReturn* value);
		~CommandReturn();
		IObject* getObject();
		bool exitFunction();
		bool isReturn();
		void setReturn(bool _return);
		bool isThrow();
		void setThrow(bool _throw);
	};

	class Command {
	public:
		virtual ~Command() {}

		virtual CommandReturn* exec(MemoryObject& mem) = 0;

		virtual Command* clone() = 0;
	};
}