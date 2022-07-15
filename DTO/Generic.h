#pragma once
#include <string>
#include <forward_list>
#include <vector>
#include <filesystem>
#include "Instanciable.h"

namespace DTO {
	class GenPossibility {
	private:
		union {
			Instanciable* m_type;
			Instanciable** m_possibilities;
		};
		size_t m_possibilitiesLen;
	public:
		GenPossibility();
		GenPossibility(Instanciable* type);
		GenPossibility(Instanciable** possibilities, size_t possibilitiesLen);
		~GenPossibility();
		bool isOk(Instanciable* type);
		bool isOk(GenPossibility& type);
	};

	class Generic : public SourceFile {
	public:
		Generic(std::wstring name, std::filesystem::path path) :SourceFile(name, path) {}
		virtual ~Generic() override = default;
		virtual SourceFile* create(std::wstring newName, Instanciable** gens, size_t genSize) = 0;
		virtual void add(GenPossibility* gens, size_t genSize, SourceFile* _class) = 0;
		virtual bool contains(Instanciable** gens, size_t genSize) = 0;
		virtual SourceFile* get(Instanciable** gens, size_t genSize) = 0;
	};

	class GenericDynamic :public Generic {
	private:
		std::forward_list<std::pair<std::vector<GenPossibility>, SourceFile*>> m_classes;
	public:
		GenericDynamic(std::wstring name, std::filesystem::path path) :Generic(name, path) {}
		virtual ~GenericDynamic() override;
		void add(GenPossibility* gens, size_t genSize, SourceFile* _class) override;
		bool contains(Instanciable** gens, size_t genSize)override;
		SourceFile* get(Instanciable** gens, size_t genSize)override;
	};

	class GenericStatic :public Generic {
	private:
		std::forward_list<std::pair<GenPossibility*, SourceFile*>> m_classes;
		size_t m_genSize;
	public:
		GenericStatic(std::wstring name, std::filesystem::path path, size_t genSize) :Generic(name, path), m_genSize(genSize) {}
		virtual ~GenericStatic() override;
		size_t genSize() { return m_genSize; }
		void add(GenPossibility* gens, size_t genSize, SourceFile* _class) override;
		bool contains(Instanciable** gens, size_t genSize)override;
		SourceFile* get(Instanciable** gens, size_t genSize)override;
	};

	class GenericI : public Instanciable {
	private:
		std::wstring* m_names;
		GenPossibility* m_possibilities;
		size_t m_possSize;
	public:
		GenericI(std::wstring name, std::filesystem::path path, GenPossibility* possibilities, std::wstring* names, size_t possSize) :Instanciable(name, path), m_names(names), m_possibilities{ possibilities }, m_possSize{ possSize } {}
		bool instanceOf(Instanciable* _class) override;
	};
}