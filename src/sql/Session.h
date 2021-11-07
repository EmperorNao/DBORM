#pragma once
#include "../db/Table.h"
#include "Query.h"
#include <initializer_list>



namespace sql {


	class Engine {

	};


	class Session {

	private:
		
		Query* current;
		Engine* engine;

	public:
		Session(Engine* e): engine(e) { current = nullptr; };
		void begin() {};
		void end() {};
		void rollback() {};
		void commit() {};

		//void add_info(std::string table, std::map<std::string, db::ColumnDescription> meta);
		Session* update();
		Session* del();
		Session* select(std::string, std::vector<std::string>);

		Session* join(std::string main_table, std::string additional_table, std::vector<std::string>);
		Session* where(db::Statement st);
		Session* execute(); 

	};

	class QueryError {

		std::string message;
	public:
		QueryError(std::string m) : message(m) {};

		virtual std::string what() const throw()
		{
			return message;
		}

	};

	Session* create_session(Engine* e);

}

