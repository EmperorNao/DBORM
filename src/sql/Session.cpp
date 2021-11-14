#include "Session.h"
#include <stdarg.h>
#include <ranges>
#include <set>



namespace sql {

	sql::Session* sql::Session::update() { return this; };
	sql::Session* sql::Session::del() { return this; };

	sql::Session* sql::Session::select(std::string table_name, db::meta_info meta, std::vector<std::string> cols = {}) {

		if (not cols.size()) {

			cols = { "*" };

		}

		this->current = new Select(table_name, cols, this->current);

		return this;

	};


	sql::Session* sql::Session::join(std::string main_table, db::meta_info main_meta, std::string additional_table, db::meta_info add_meta, std::vector<std::string> col) {

		QueryType cur = this->current->get_protocol();
		if (cur != SELECT and cur != JOIN) {

			throw new QueryError("Can't join if last query was not select or join");

		}

		if (col.size() > 1) {

			throw new QueryError("Can't join on more then one column in one join");

		}

		std::string column = col[0];
		if (add_meta.find(column) == add_meta.end()) {

			throw new QueryError("Don't find column " + column + " to join");

		}
		if (not add_meta[column].fk) {

			throw new QueryError("Column " + column + " needed to be foreign key for join, but it's not");

		}

		std::string main_column = "";
		for (auto& el : main_meta) {

			if (el.second.pk) {

				main_column = el.first;
				break;

			}

		}

		if (main_column == "") {

			throw new QueryError("Don't find primary key to join in table");

		}

		this->current = new Join(main_table, main_column, additional_table, col[0], this->current);
		return this;

	}

	sql::Session* sql::Session::where(db::Statement st) { 
			
		QueryType cur = this->current->get_protocol();
		if (cur != SELECT and cur != JOIN) {

			throw new QueryError("Can't use where if last query was not select or join");

		}
		this->current = new Where(st, this->current);
		return this;  
	
	
	};

	Result* sql::Session::execute() { 
	
		if (autotransaction) {

			begin();

		}
		try {
			Result* res = engine->execute(this->current);
			this->results.push_back(res);
			this->current = nullptr;
			if (autotransaction) {

				end();

			}
			return res;
		}
		catch (std::exception* e) {
			throw e;
		}
	
	};

}

