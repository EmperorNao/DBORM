#pragma once
#include "../db/Table.h"
#include "Query.h"
#include <initializer_list>



namespace sql {

	class QueryBuilder {

	public:
		QueryBuilder();
		std::string build(Query* q) {

			if (q == nullptr) {

				throw QueryError("Empty query was provided in builder");

			}
			std::vector<Query*> list;
			while (q->get_parent() != nullptr) {

				list.push_back(q);
				q = q->get_parent();

			}
			std::reverse(list.begin(), list.end());

			return this->convert(list);

		}

		std::string convert(std::vector<Query*> list) {

			Query* q = list[0];
			QueryType p = q->get_protocol();
			if (p != SELECT and p != INSERT and p != DELETE) {

				// TODO

			}
			

		}


	};


	class Engine {

	public:
		Engine() {};
		void execute(Query* q) {



		}

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

	Session* create_session(Engine* e);

}

