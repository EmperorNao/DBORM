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
		UPDATE,
		JOIN,
		WHERE
	};

	std::string str_query(QueryType type);

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
		std::string get_table() const { return table_name; }

	};

	class Join : public Query {

	private:
		std::string main_table;
		std::string main_column;
		std::string additional_table;
		std::string additional_column;
	public:
		Join(std::string m_table, std::string _main_column, std::string add_table, std::string _column_on, Query* q = nullptr) :
			main_table(m_table),
			main_column(_main_column),
			additional_table(add_table),
			additional_column(_column_on),
			Query(JOIN, q) {};

		std::string get_main_table() const { return main_table; }
		std::string get_additional_table() const { return additional_table; }
		std::string get_main_column() const { return main_column; }
		std::string get_additional_column() const { return additional_column; }

	};

	class Where : public Query {

	private:
		db::Statement statement;
	public:
		Where(db::Statement& s, Query* q = nullptr) :
			statement(s),
			Query(WHERE, q) {};

		std::string get_state()  const { return statement.get_state();  }

	};

}
