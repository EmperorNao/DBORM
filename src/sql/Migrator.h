#pragma once
#include "../db/Table.h"
#include "Query.h"
#include <initializer_list>
#include "QueryBuilder.h"
#include "Engine.h"


namespace sql {

	enum MigrationFormat {
		CSV, JSON
	};


	class MigrationError : public std::exception {

		std::string message;
	public:
		MigrationError(std::string m) : message(m) {};

		virtual const char* what() const throw()
		{
			return message.c_str();
		}

	};


	class MigrationRunner {

	private:
		Engine* engine;
	public:
		MigrationRunner(Engine* e): engine(e) {};
		void migrate(std::string filename, std::vector<std::string> tables);

	};

}