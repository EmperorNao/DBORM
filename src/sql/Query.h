#pragma once
#include <string>
#include <vector>


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
		std::vector<std::string> columns;
	public:
		Select(std::vector<std::string> _columns) : columns(_columns), Query("select") {};
		std::vector<std::string> get_columns() { return columns; };

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
		std::string table_join;
		std::string column_on;	
		Where(std::string table, std::string _column_on) :
			table_join(table),
			column_on(_column_on),
			Query("join") {};

	};

	/*
	class Condition {

	public:
		Condition* inner;
		std::char combine;


	};
	User["name"] > 12

	User.select("*").where(User["name"] > 12);
	*/




}
