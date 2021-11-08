#include "Session.h"
#include <stdarg.h>
#include <ranges>
#include <set>



namespace sql {

	Session* create_session(Engine* e) { return new Session(e); }

	sql::Session* sql::Session::update() { return this; };
	sql::Session* sql::Session::del() { return this; };

	sql::Session* sql::Session::select(std::string table_name, std::vector<std::string> cols = {}) {

		if (not cols.size()) {

			cols = { "*" };

		}

		for (auto& el : cols) {

			std::cout << el << "\n";

		}

		this->current = new Select(table_name, cols, this->current);

		return this;

	};


	sql::Session* sql::Session::join(std::string main_table, std::string additional_table, std::vector<std::string> col) {

		QueryType cur = this->current->get_protocol();
		if (cur != SELECT and cur != JOIN) {

			throw QueryError("Can't join if last query was not select or join");

		}

		if (col.size() > 1) {

			throw QueryError("Can't join on more then one column in one join");

		}
		this->current = new Join(main_table, additional_table, col[0], this->current);
		return this;

	}

	sql::Session* sql::Session::where(db::Statement st) { 
			
		QueryType cur = this->current->get_protocol();
		if (cur != SELECT and cur != JOIN) {

			throw QueryError("Can't use where if last query was not select or join");

		}
		this->current = new Where(st, this->current);
		return this;  
	
	
	};

	sql::Session* sql::Session::execute() { 
	
		try {

			this->engine->execute(this->current);

		}
		catch (QueryError& qe) {

			throw qe;

		}

		this->current = nullptr;
		return this;  
	
	};

}

