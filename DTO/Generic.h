#pragma once
#include <string>
#include <forward_list>
#include <vector>
#include "Interface.h"

namespace DTO {
	class GenPossibility {
	private:
		union {
			Interface* m_type;
			Interface** m_possibilities;
		};
		size_t m_possibilitiesLen;
	public:
		GenPossibility() :m_type(nullptr), m_possibilitiesLen(0) {}
		GenPossibility(Interface* type) :m_type(type), m_possibilitiesLen{ 0 } {}
		GenPossibility(Interface** possibilities, size_t possibilitiesLen) :m_possibilities(possibilities), m_possibilitiesLen(possibilitiesLen) {}
		~GenPossibility() {
			if (m_possibilitiesLen != 0)
				delete[] m_possibilities;
		}
		bool isOk(Interface* type) {
			if (m_possibilitiesLen == 0) {
				return type == m_type;
			}
			else {
				for (size_t i{ 0 }; i < m_possibilitiesLen; i++) {
					if (!type->instanceOf(m_possibilities[i]))
						return false;
				}
				return true;
			}
		}
	};

	class Generic : public SourceFile {
	public:
		Generic(std::string name, std::string path) :SourceFile(name, path) {}
		virtual ~Generic() override = default;
		virtual SourceFile* create(std::string newName, Interface** gens, size_t genSize) = 0;
		virtual void add(GenPossibility* gens, size_t genSize, SourceFile* _class) = 0;
		virtual bool contains(Interface** gens, size_t genSize) = 0;
		virtual SourceFile* get(Interface** gens, size_t genSize) = 0;
	};

	class GenericDynamic :public Generic {
	private:
		std::forward_list<std::pair<std::vector<GenPossibility>, SourceFile*>> m_classes;
	public:
		GenericDynamic(std::string name, std::string path) :Generic(name, path) {}
		virtual ~GenericDynamic() override;
		void add(GenPossibility* gens, size_t genSize, SourceFile* _class) override;
		bool contains(Interface** gens, size_t genSize)override;
		SourceFile* get(Interface** gens, size_t genSize)override;
	};

	class GenericStatic :public Generic {
	private:
		std::forward_list<std::pair<GenPossibility*, SourceFile*>> m_classes;
		size_t m_genSize;
	public:
		GenericStatic(std::string name, std::string path, size_t genSize) :Generic(name, path), m_genSize(genSize) {}
		virtual ~GenericStatic() override;
		size_t genSize() { return m_genSize; }
		void add(GenPossibility* gens, size_t genSize, SourceFile* _class) override;
		bool contains(Interface** gens, size_t genSize)override;
		SourceFile* get(Interface** gens, size_t genSize)override;
	};

	class GenericI : public SourceFile {
	private:
		GenPossibility* m_possibilities;
		size_t m_possSize;
	public:
		GenericI(std::string name, std::string path, GenPossibility* possibilities, size_t possSize) :SourceFile(name, path), m_possibilities{ possibilities }, m_possSize{ possSize } {}
		bool isOk(Interface* _class);
	};
}