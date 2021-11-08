#pragma once
#include <string>
#include <vector>
#include "../db/Table.h"


namespace sql {

	class QueryError {

		std::string message;
	public:
		QueryError(std::string m) : message(m) {};

		virtual std::string what() const throw()
		{
			return message;
		}

	};

	enum QueryType {
		INSERT,
		DELETE,
		SELECT,
		JOIN,
		WHERE
	};

	class Query {

	private:
		QueryType base_protocol;
		Query* parent;

	public:
		Query(QueryType base, Query* q = nullptr) : base_protocol(base), parent(q) {};
		QueryType get_protocol() { return base_protocol; };
		Query* get_parent() { return parent; }

	};

	class Select : public Query {

	private:
		std::string table_name;
		std::vector<std::string> columns;
	public:
		Select(std::string _table, std::vector<std::string> _columns, Query* q = nullptr) 
			: table_name(_table), columns(_columns), Query(SELECT, q) {};
		std::vector<std::string> get_columns() const { return columns; };

	};

	class Join : public Query {

	private:
		std::string main_table;
		std::string additional_table;
		std::string column_on;
	public:
		Join(std::string m_table, std::string add_table, std::string _column_on, Query* q = nullptr) :
			main_table(m_table),
			additional_table(add_table),
			column_on(_column_on),
			Query(JOIN, q) {};

	};

	class Where : public Query {

	private:
		db::Statement statement;
	public:
		Where(db::Statement& s, Query* q = nullptr) :
			statement(s),
			Query(WHERE, q) {};
			
	};

}
