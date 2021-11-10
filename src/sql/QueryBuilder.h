#pragma once
#include "../db/Table.h"
#include "Query.h"
#include <initializer_list>


namespace sql {


	class QueryBuilder {

	public:
		QueryBuilder() {};
		std::string build(Query* q);
		std::string convert(std::vector<Query*> list);

	};

};
