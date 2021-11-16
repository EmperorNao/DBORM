#pragma once
#include "../db/Table.h"
#include "Query.h"
#include <initializer_list>
#include "QueryBuilder.h"
#include "Engine.h"


namespace sql {

	class Session {

	private:
		
		Query* current;
		Engine* engine;
		std::vector<Result*> results;
		bool autotransaction = false;


	public:
		Session(Engine* e, bool autotrans = false): engine(e), autotransaction(autotrans) { current = nullptr; };
		~Session() { this->free_all_result(); }
		void begin() { this->engine->begin(); };
		void end() { this->engine->end(); };
		void commit() { this->engine->commit(); };
		void rollback() { this->engine->rollback(); };

		Query* get_cur_query() { 

			Query* t = current;
			this->current = nullptr;
			return t;
		
		}

		void migrate(std::string path_to_save, std::string file_name, MigrationFormat format);


		////void add_info(std::string table, std::map<std::string, db::ColumnDescription> meta);
		//Session* update(std::string table_name, db::meta_info meta, db::Table* t);
		//Session* del(std::string table_name, db::meta_info meta, db::Table* t);

		//Session* update(std::string table_name, db::meta_info meta);
		//Session* del(std::string table_name, db::meta_info meta);

		//Session* insert(std::string table_name, db::meta_info meta, db::Table* t);

		//void migrate(std::string table_name, db::meta_info meta, std::string )
		Session* select(std::string, db::meta_info meta, std::vector<std::string>);

		Session* join(std::string main_table, db::meta_info main_meta, std::string additional_table, db::meta_info add_meta, std::vector<std::string>);
		Session* where(db::Statement st);
		Result* execute();

		void free_all_result() {

			for (auto& el : results) {

				try {
					el->free();
				}
				catch (ResultError& e) {

					// pass
					// situation when user already freed res

				}

			}
			results = {};

		}

	};

}