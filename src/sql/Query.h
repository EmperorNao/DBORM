#pragma once
#include <string>
#include <vector>
#include "../db/Table.h"


namespace sql {

	class Query {

	private:
		std::string base_protocol;

	public:
		Query(std::string base) : base_protocol(base) {};
		std::string get_protocol() { return base_protocol; };

	};

	class Select : public Query {

	private:
		std::string table_name;
		std::vector<std::string> columns;
	public:
		Select(std::string _table, std::vector<std::string> _columns) 
			: table_name(_table), columns(_columns), Query("select") {};
		std::vector<std::string> get_columns() const { return columns; };

	};

	class Join : public Query {

	private:
		std::string table_join;
		std::string column_on;
		Join(std::string table, std::string _column_on) :
			table_join(table),
			column_on(_column_on),
			Query("join") {};

	};

	class Where : public Query {

	private:
		db::Statement statement;
		Where(db::Statement& s) :
			statement(s),
			Query("where") {};
			
	};




}
