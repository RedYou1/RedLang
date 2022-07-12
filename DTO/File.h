#pragma once
#include <string>
#include <filesystem>
#include "Object.h"
#include "String.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Global.h"
#include "Boolean.h"

namespace DTO {
	class FileO : public Object {
	public:
		std::filesystem::path m_path;

		FileO(Class* type, std::filesystem::path value) :Object(type) {
			m_path = value;
		}

		Object* clone()override { return new FileO(m_type, m_path); }
	};

	class FileC : public Class {
	public:
		FileC() : Class(L"File", Paths::File, GLOBAL::getClasses()->getClass(Paths::Object)) {
		}

		class Equals :public Command {
		public:
			BooleanC* m_s;
			Equals(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FileO* o{ (FileO*)mem.get(L"this") };
				FileO* c{ (FileO*)mem.get(L"c") };
				return new CommandReturn(new BooleanO(m_s, o->m_path == c->m_path), true, false);
			}
			Command* clone()override { return new Equals(m_s); }
		};
		class FileCopy :public Command {
		public:
			FileC* m_s;
			FileCopy(FileC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				Object* c{ new FileO(m_s, ((FileO*)mem.get(L"c"))->m_path) };
				mem.set(L"this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new FileCopy(m_s); }
		};
		class FileConstruct :public Command {
		public:
			FileC* m_s;
			FileConstruct(FileC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				std::filesystem::path _path{ ((StringO*)mem.get(L">workspace"))->m_value + std::filesystem::path::preferred_separator +
											 ((StringO*)mem.get(L"c"))->m_value };

				Object* c{ new FileO(m_s, _path) };
				mem.set(L"this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new FileConstruct(m_s); }
		};

		class GetPath :public Command {
		public:
			StringC* m_s;
			GetPath(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FileO* a{ (FileO*)mem.get(L"this") };
				return new CommandReturn(new StringO(m_s, a->m_path.wstring()), true, false);
			}
			Command* clone()override { return new GetPath(m_s); }
		};

		class GetAbsolutePath :public Command {
		public:
			StringC* m_s;
			GetAbsolutePath(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FileO* a{ (FileO*)mem.get(L"this") };
				return new CommandReturn(new StringO(m_s, std::filesystem::absolute(a->m_path).wstring()), true, false);
			}
			Command* clone()override { return new GetAbsolutePath(m_s); }
		};

		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FileO* a{ (FileO*)mem.get(L"this") };
				return new CommandReturn(new StringO(m_s, L"File:" + a->m_path.wstring()), true, false);
			}
			Command* clone()override { return new ToString(m_s); }
		};
	};
}