#pragma once
#include "../db/Table.h"
#include "Query.h"
#include <initializer_list>


namespace sql {

	class QueryBuilder {


	private:
		const std::vector<std::string> aggregation = { "MAX", "MIN", "AVG", "SUM" };
		std::string convert(std::vector<Query*> list);
		std::string fullname(std::string table_name, std::string column_name);
		std::string join_string(std::vector<std::string> values, std::string symb);
	public:
		QueryBuilder() {};
		std::string build(Query* q);

	};

};
