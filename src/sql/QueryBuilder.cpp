#include "QueryBuilder.h"

namespace sql {
	
	
	std::string QueryBuilder::build(Query* q) {

		if (q == nullptr) {

			throw QueryError("Empty query was provided in builder");

		}
		std::vector<Query*> list;
		while (q != nullptr) {

			list.push_back(q);
			q = q->get_parent();

		}
		std::reverse(list.begin(), list.end());

		return this->convert(list);

	};

	std::string QueryBuilder::convert(std::vector<Query*> list) {

		Query* q = list[0];
		std::string query;

		Join* join;
		Where* where;
		Select* select;

		QueryType start;
		QueryType cur = q->get_protocol();
		if (cur != SELECT and cur != INSERT and cur != DELETE) {

			throw QueryError("Wrong type of query was first");

		}
		else {

			start = cur;
			query += str_query(cur);
			if (cur == SELECT) {

				select = (Select*)q;
				auto columns = select->get_columns();
				std::string table_name = select->get_table();
				for (int i = 0; i < columns.size() - 1; ++i) {

					query += " " + table_name + "." + columns[i] + ",";

				}
				if (columns.size()) {

					query += " " + table_name + "." + columns[columns.size() - 1];

				}
				query += " FROM " + table_name;

			}
			else {

				// TODO insert and delete queries
				throw QueryError("Still in develepoment");

			}


		}

		QueryType last = start;

		for (int i = 1; i < list.size(); ++i) {

			q = list[i];
			cur = q->get_protocol();
			switch (cur) {
			case INSERT:
				throw QueryError("Misplaced insert in query");
			case DELETE:
				throw QueryError("Misplaced delete in query");
			case SELECT:
				if (last != SELECT) {

					throw QueryError("Misplaced select in query");

				}
			case JOIN:
				if (start != SELECT and last != JOIN) {

					throw QueryError("Can't join after something other then select and join");

				}
				join = (Join*)q;
				query += " " + str_query(cur) + " " + join->get_additional_table() + " on " \
					+ join->get_main_table() + "." + join->get_main_column() + "=" \
					+ join->get_additional_table() + "." + join->get_additional_column();

				break;
			case WHERE:
				if (start == INSERT) {

					throw QueryError("Can't do where in insert query");

				}
				where = (Where*)q;
				query += " " + str_query(cur) + " " + where->get_state();

				break;
			default:
				throw QueryError("Unknown query type was proveded while converting query");

			}

			last = cur;


		}

		return query;

	};


}