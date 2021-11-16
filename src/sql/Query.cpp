#include "Query.h"

std::string sql::str_query(QueryType type) {

	switch (type) {
	case INSERT:
		return "INSERT INTO ";
	case DELETE:
		return "DELETE FROM";
	case SELECT:
		return "SELECT";
	case UPDATE:
		return "UPDATE";
	case JOIN:
		return "JOIN";
	case WHERE:
		return "WHERE";
	default:
		throw new QueryError("Wrong qury type was provided to str_query");

	}

}
