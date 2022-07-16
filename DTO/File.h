#pragma once
#include <string>
#include <filesystem>
#include <queue>
#include "Object.h"
#include "String.h"
#include "Command.h"
#include "Class.h"
#include "Return.h"
#include "Global.h"
#include "Boolean.h"
#include "Array.h"

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
				std::filesystem::path _path{ ((FileO*)mem.get(L">workspace"))->m_path.parent_path().wstring()
					+ std::filesystem::path::preferred_separator +
					((StringO*)mem.get(L"c"))->m_value };

				Object* c{ new FileO(m_s, _path) };
				mem.set(L"this", c);
				return new CommandReturn(c, true, false);
			}
			Command* clone()override { return new FileConstruct(m_s); }
		};

		class GetRelativePath :public Command {
		public:
			StringC* m_s;
			GetRelativePath(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FileO* a{ (FileO*)mem.get(L"this") };
				return new CommandReturn(new StringO(m_s, std::filesystem::relative(a->m_path, ((FileO*)mem.get(L">workspace"))->m_path.parent_path()).wstring()), true, false);
			}
			Command* clone()override { return new GetRelativePath(m_s); }
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

		class GetFiles :public Command {
		public:
			FileC* m_s;
			Array::ArrayC* m_arr;
			GetFiles(FileC* s, Array::ArrayC* arr) :m_s(s), m_arr(arr) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FileO* a{ (FileO*)mem.get(L"this") };
				bool file{ ((BooleanO*)mem.get(L"file"))->m_value };
				bool dir{ ((BooleanO*)mem.get(L"dir"))->m_value };
				bool recursive{ ((BooleanO*)mem.get(L"recursive"))->m_value };

				std::queue<std::filesystem::path> q{};
				if (recursive)
					for (std::filesystem::directory_entry it : std::filesystem::recursive_directory_iterator(a->m_path)) {
						if ((file && !it.is_directory()) ||
							(dir && it.is_directory()))
							q.push(it.path());
					}
				else
					for (std::filesystem::directory_entry it : std::filesystem::directory_iterator(a->m_path)) {
						if ((file && !it.is_directory()) ||
							(dir && it.is_directory()))
							q.push(it.path());
					}

				ArrayO* arr{ new ArrayO(m_arr,q.size()) };

				for (size_t i{ 0 }; i < arr->m_size; i++) {
					arr->m_value[i] = new FileO(m_s, q.front());
					q.pop();
					arr->m_value[i]->addRef();
				}

				return new CommandReturn(arr, true, false);
			}
			Command* clone()override { return new GetFiles(m_s, m_arr); }
		};

		class IsDirectory :public Command {
		public:
			BooleanC* m_s;
			IsDirectory(BooleanC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FileO* a{ (FileO*)mem.get(L"this") };
				return new CommandReturn(new BooleanO(m_s, std::filesystem::is_directory(a->m_path)), true, false);
			}
			Command* clone()override { return new IsDirectory(m_s); }
		};

		class GetName :public Command {
		public:
			StringC* m_s;
			GetName(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FileO* a{ (FileO*)mem.get(L"this") };
				return new CommandReturn(new StringO(m_s, a->m_path.filename()), true, false);
			}
			Command* clone()override { return new GetName(m_s); }
		};

		class GetExtension :public Command {
		public:
			StringC* m_s;
			GetExtension(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FileO* a{ (FileO*)mem.get(L"this") };
				return new CommandReturn(new StringO(m_s, a->m_path.extension()), true, false);
			}
			Command* clone()override { return new GetExtension(m_s); }
		};

		class ToString :public Command {
		public:
			StringC* m_s;
			ToString(StringC* s) :m_s(s) {}
			CommandReturn* exec(MemoryObject& mem) override {
				FileO* a{ (FileO*)mem.get(L"this") };
				return new CommandReturn(new StringO(m_s, L"File:" + std::filesystem::absolute(a->m_path).wstring()), true, false);
			}
			Command* clone()override { return new ToString(m_s); }
		};
	};
}