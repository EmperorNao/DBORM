#include "QueryBuilder.h"


namespace sql {	
	
	std::string QueryBuilder::build(Query* q) {

		if (q == nullptr) {

			throw new QueryError("Empty query was provided in builder");

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
		Insert* insert;
		Delete* del;
		Update* update;

		QueryType start;
		QueryType cur = q->get_protocol();
		int nextind = 0;
		if (cur != SELECT and cur != INSERT and cur != DELETE) {

			throw new QueryError("Wrong type of query was first");

		}
		else {

			start = cur;
			query += str_query(cur);
			if (cur == SELECT) {

				select = (Select*)q;
				std::string start_table = select->get_table();
				do {

					select = (Select*)q;
					auto columns = select->get_columns();
					std::string table_name = select->get_table();
					if (table_name != start_table) {

						query += ", ";

					}
					for (int i = 0; i < columns.size() - 1; ++i) {

						if (columns[i] != "*") {
							query += " " + table_name + "." + columns[i] + ",";
						}
						else {
							query += " " + columns[i] + ",";
						}

					}
					if (columns.size()) {

						if (columns[columns.size() - 1] != "*") {
							query += " " + table_name + "." + columns[columns.size() - 1];
						}
						else {
							query += " " + columns[columns.size() - 1];
						}

					}

					q = q->get_parent();
					nextind += 1;

				} while ((q != nullptr) and (q->get_protocol() == SELECT));
				query += " FROM " + start_table;

			}
			else if (cur == INSERT) {

				insert = (Insert*)q;
				query += insert->get_table() + " ";
				std::vector<std::string> columns;
				for (auto el : insert->get_meta()) {

					columns.push_back(el.first);

				}

				query += "(";
				for (int i = 0; i < columns.size() - 1; ++i) {

					query += columns[i] + ", ";

				}
				if (columns.size()) {

					query += columns[columns.size() - 1];

				}
				query += ") ";
				query += "VALUES ";

				auto values = insert->get_values();

				for (int val = 0; val < values.size() - 1; ++val) {

					query += "(";
					for (int col = 0; col < columns.size() - 1; ++col) {

						query += values[val]->get(columns[col], false) + ", ";

					}
					if (columns.size()) {

						query += values[val]->get(columns[columns.size() - 1], false);

					}
					query += "), ";

				}
				if (values.size()) {

					query += "(";
					for (int col = 0; col < columns.size() - 1; ++col) {

						query += values[values.size() - 1]->get(columns[col], false) + ", ";

					}
					if (columns.size()) {

						query += values[values.size() - 1]->get(columns[columns.size() - 1], false);

					}
					query += ")";

				}
				return query;

			}
			else if (cur == DELETE) {

				del = (Delete*)q;
				query += del->get_table();
				auto values = del->get_values();

				if (values.size()) {

					std::string pk = values[0]->get_pk_key(del->get_table(), del->get_meta());

					query += " WHERE " + pk + " IN (";
					for (int i = 0; i < values.size() - 1; ++i) {

						query += values[i]->get(pk) + ", ";

					}
					query += values[values.size() - 1]->get(pk);
					query += ")";
					return query;


				}
				return query;

			}
			else if (cur == UPDATE) {

				update = (Update*)q;
				db::Table* value = update->get_value();
				std::string pk = value->get_pk_key(update->get_table(), del->get_meta());

				query += update->get_table() + " ";

				std::vector<std::string> columns;
				for (auto el : update->get_meta()) {

					if (el.first != pk) {
						columns.push_back(el.first);
					}

				}

				query += " SET (";
				for (int i = 0; i < columns.size() - 1; ++i) {

					query += columns[i] + ", ";

				}
				if (columns.size()) {

					query += columns[columns.size() - 1];

				}
				query += ") ";
				query += " = ";


				query += "(";
				for (int col = 0; col < columns.size() - 1; ++col) {

					query += value->get(columns[col], false) + ", ";

				}
				if (columns.size()) {

					query += value->get(columns[columns.size() - 1], false);

				}
				query += ")";
				
				return query;

			}
			else {

				// TODO insert and delete queries
				throw new QueryError("Still in develepoment");

			}


		}

		QueryType last = start;

		for (int i = nextind; i < list.size(); ++i) {

			q = list[i];
			cur = q->get_protocol();
			switch (cur) {
			case INSERT:
				throw new QueryError("Misplaced insert in query");
			case DELETE:
				throw new QueryError("Misplaced delete in query");
			case SELECT:
				throw new QueryError("Misplaced select in query");
			case UPDATE:
				throw new QueryError("Misplaces update in query");
			case JOIN:
				if (start != SELECT and last != JOIN) {

					throw new QueryError("Can't join after something other then select and join");

				}
				join = (Join*)q;
				query += " " + str_query(cur) + " " + join->get_additional_table() + " on " \
					+ join->get_main_table() + "." + join->get_main_column() + "=" \
					+ join->get_additional_table() + "." + join->get_additional_column();

				break;
			case WHERE:
				if (start == INSERT) {

					throw new QueryError("Can't do where in insert query");

				}
				where = (Where*)q;
				query += " " + str_query(cur) + " " + where->get_state();

				break;
			default:
				throw new QueryError("Unknown query type was proveded while converting query");

			}
			last = cur;

		}

		return query;

	};


}