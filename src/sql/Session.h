#pragma once
#include "../db/Table.h"
#include "Query.h"
#include <initializer_list>
#include "QueryBuilder.h"


namespace sql {

	class Engine {

	public:
		Engine() {};
		virtual void execute(Query* q) = 0;

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

		Query* get_cur_query() { 

			Query* t = current;
			this->current = nullptr;
			return t;
		
		}

		//void add_info(std::string table, std::map<std::string, db::ColumnDescription> meta);
		Session* update();
		Session* del();
		Session* select(std::string, db::meta_info meta, std::vector<std::string>);

		Session* join(std::string main_table, db::meta_info main_meta, std::string additional_table, db::meta_info add_meta, std::vector<std::string>);
		Session* where(db::Statement st);
		Session* execute(); 

	};

	Session* create_session(Engine* e);

}

