#pragma once
#include "../db/Table.h"
#include "Query.h"
#include <initializer_list>


namespace sql {

	class QueryBuilder {

	private:
		std::string convert(std::vector<Query*> list);
	public:
		QueryBuilder() {};
		std::string build(Query* q);

	};

};
