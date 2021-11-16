#pragma once
#include <string>
#include <vector>
#include "../db/Table.h"


namespace sql {

	class QueryError : public std::exception {

		std::string message;
	public:
		QueryError(std::string m) : message(m) {};

		virtual const char* what() const throw()
		{
			return message.c_str();
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

	class Insert : public Query {

	private:
		std::string table_name;
		db::meta_info meta;
		std::vector<db::Table*> values;
	public:
		Insert(std::string _table, db::meta_info _meta, std::vector<db::Table*> _values = {}, Query* q = nullptr)
			: table_name(_table), meta(_meta), values(_values), Query(INSERT, q) {};
		std::vector<db::Table*> get_values() const { return values; };
		db::meta_info get_meta() const { return meta; };
		std::string get_table() const { return table_name; }

	};

	class Delete : public Query {

	private:
		std::string table_name;
		db::meta_info meta;
		std::vector<db::Table*> values;

	public:
		Delete(std::string _table, db::meta_info _meta, std::vector<db::Table*> _values = {}, Query* q = nullptr)
			: table_name(_table), meta(_meta), values(_values), Query(DELETE, q) {};
		std::vector<db::Table*> get_values() const { return values; };
		db::meta_info get_meta() const { return meta; };
		std::string get_table() const { return table_name; }

	};

	class Update : public Query {

	private:
		std::string table_name;
		db::meta_info meta;
		db::Table* value;

	public:
		Update(std::string _table, db::meta_info _meta, db::Table* _value, Query* q = nullptr)
			: table_name(_table), meta(_meta), value(_value), Query(UPDATE, q) {};
		db::Table* get_value() const { return value; };
		db::meta_info get_meta() const { return meta; };
		std::string get_table() const { return table_name; }

	};

}
