#include "Session.h"
#include <stdarg.h>
#include <ranges>
#include <set>
#include <filesystem>
#include <fstream>
#include <iostream>


namespace sql {

	/*sql::Session* sql::Session::update() { return this; };
	sql::Session* sql::Session::del() { return this; };*/

	sql::Session* sql::Session::select(std::string table_name, db::meta_info meta, std::vector<std::string> cols = {}) {

		if (not cols.size()) {

			cols = { "*" };

		}

		this->current = new Select(table_name, cols, this->current);

		return this;

	};

	sql::Session* sql::Session::join(std::string main_table, db::meta_info main_meta, std::string additional_table, db::meta_info add_meta, std::vector<std::string> col) {

		QueryType cur = this->current->get_protocol();
		if (cur != SELECT and cur != JOIN) {

			throw new QueryError("Can't join if last query was not select or join");

		}

		if (col.size() > 1) {

			throw new QueryError("Can't join on more then one column in one join");

		}

		std::string column = col[0];
		if (add_meta.find(column) == add_meta.end()) {

			throw new QueryError("Don't find column " + column + " to join");

		}
		if (not add_meta[column].fk) {

			throw new QueryError("Column " + column + " needed to be foreign key for join, but it's not");

		}

		std::string main_column = "";
		for (auto& el : main_meta) {

			if (el.second.pk) {

				main_column = el.first;
				break;

			}

		}

		if (main_column == "") {

			throw new QueryError("Don't find primary key to join in table");

		}

		this->current = new Join(main_table, main_column, additional_table, col[0], this->current);
		return this;

	}

	sql::Session* sql::Session::where(db::Statement st) { 
			
		QueryType cur = this->current->get_protocol();
		if (cur != SELECT and cur != JOIN) {

			throw new QueryError("Can't use where if last query was not select or join");

		}
		this->current = new Where(st, this->current);
		return this;  
	
	};

	Session* sql::Session::insert(std::string table_name, db::meta_info meta, std::vector<db::Table*> values) {

		if (this->current != nullptr) {

			throw new QueryError("Insert in wrong position in query");

		}
		this->current = new Insert(table_name, meta, values);
		return this;

	}

	Session* sql::Session::del(std::string table_name, db::meta_info meta, std::vector<db::Table*> values) {

		if (this->current != nullptr) {

			throw new QueryError("Delete in wrong position in query");

		}
		this->current = new Delete(table_name, meta, values);
		return this;

	}

	Session* sql::Session::update(std::string table_name, db::meta_info meta, db::Table* value) {

		if (this->current != nullptr) {

			throw new QueryError("Update in wrong position in query");

		}
		this->current = new Update(table_name, meta, value);
		return this;


	}

	Session* sql::Session::group_by(std::string table_name, db::meta_info meta, std::vector<std::string> columns) {

		this->current = new GroupBy(table_name, columns, this->current);

		return this;

	}

	void sql::Session::migrate(std::string path_to_save, std::string migration_name, MigrationFormat format) {

		Result* r = engine->execute("SELECT * from information_schema.tables where table_schema not in ('information_schema', 'pg_catalog')");
		std::vector<std::string> tables(r->get_nrows());
		for (int row = 0; row < r->get_nrows(); ++row) {

			tables[row] = r->get_value(row, "table_name");

		}
		r->free();

		std::vector<Result*> inside_data;

		try {

			for (auto table : tables) {

				Result* res = engine->execute("SELECT * from " + table);
				inside_data.push_back(res);

			}

		}
		catch (std::exception* e) {

			for (int i = 0; i < inside_data.size(); ++i) {

				inside_data[i]->free();

			}
			throw e;

		}

		try {

			if (format == DBORM) {

				std::filesystem::path dir(path_to_save);
				std::filesystem::path filename(migration_name);
				std::filesystem::path full_path = dir / filename;
				std::ofstream file;
				file.open(full_path.c_str(), std::ios::out);
				int size = tables.size();

				file << migration_name << std::endl << std::to_string(size) << std::endl;
				for (int i = 0; i < size; ++i) {

					file << tables[i] << std::endl;

				}
				file.close();

				for (int k = 0; k < size; ++k) {

					file.open(dir / std::filesystem::path(migration_name + "_" + tables[k]));
					int rows = inside_data[k]->get_nrows();
					int cols = inside_data[k]->get_ncols();
					std::vector<std::string> columns = inside_data[k]->get_columns();

					for (int j = 0; j < cols; ++j) {

						file << "\"" << columns[j] << "\"" << " ";

					}
					file << std::endl;
					for (int i = 0; i < rows; ++i) {

						for (int j = 0; j < cols; ++j) {

							std::string data = inside_data[k]->get_value(i, j);
							file << "\"" << data << "\"" << " ";

						}
						file << std::endl;

					}
					file.close();

				}

			}
			else if (format == CSV) {



			}
			else if (format == JSON) {

				// TODO

			}

			for (int i = 0; i < inside_data.size(); ++i) {

				inside_data[i]->free();

			}
			return;

		} // try end
		catch (std::exception* e) {

			for (int i = 0; i < inside_data.size(); ++i) {

				inside_data[i]->free();

			}

		}
		throw new MigrationError("Don't find right example");
		return;

	}

	void sql::Session::load_migration(std::string path, std::string migration_name, MigrationFormat format) {

		if (format == DBORM) {

			std::filesystem::path dir(path);
			std::filesystem::path filename(migration_name);
			std::filesystem::path full_path = dir / filename;
			std::ifstream file;
			file.open(full_path.c_str(), std::iostream::in);
			std::vector<std::string> tables;

			std::string s;
			file >> s;
			file >> s;
			int num_tables = std::stoi(s);
			for (int i = 0; i < num_tables; ++i) {

				file >> s;
				tables.push_back(s);

			}

			sql::Result* res;
			for (auto table : tables) {

				std::vector<db::Table*> table_values;

				full_path = dir / std::filesystem::path(migration_name + "_" + table);
				std::ifstream file;
				file.open(full_path.c_str(), std::iostream::in);
				engine->execute("DELETE FROM " + table);

				std::getline(file, s);
 				std::vector values = db::split(s, std::set({ ' ' }));

				std::vector<std::string> columns;
				for (auto value : values) {

					columns.push_back(value.substr(1, value.size() - 2));

				}

				int i = 0;
				while (std::getline(file, s)) {

					std::vector values = db::split(s, std::set({ ' ' }));

					db::Table* t = new db::Table(table, columns);
					table_values.push_back(t);
					int j = 0;
					for (auto value : values) {

						table_values[i]->set(columns[j], value.substr(1, value.size() - 2));
						++j;

					}
					++i;

				}
				db::meta_info meta;
				for (auto col : columns) {

					meta[col] = db::ColumnDescription(table, col, db::datatypes::STRING);

				}
				this->insert(table, meta, table_values)->execute();
				for (int i = 0; i < table_values.size(); ++i) {

					delete table_values[i];

				}

			}

		}


	}

	Result* sql::Session::execute() { 
	
		if (autotransaction) {

			begin();

		}
		try {
			Result* res = engine->execute(this->current);
			this->results.push_back(res);
			this->current = nullptr;
			if (autotransaction) {

				end();

			}
			return res;
		}
		catch (std::exception* e) {
			throw e;
		}
	
	};

}

