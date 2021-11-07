#include "Table.h"
#include <iostream>
#include <vector>
#include <set>


namespace db {

	//Column Table::operator[](std::string name) { return *this->container[name];  };

	namespace datatypes {


		bool is_columntype_value_correct(ColumnType v) {

			switch (v)
			{
			case ColumnType::INTEGER:
				return true;
			case ColumnType::STRING:
				return true;
			case ColumnType::FLOAT:
				return true;
			default:
				return false;
			}

		}

		Column* create_column(ColumnDescription desc) {

			if (is_columntype_value_correct(desc.type)) {
				return new Column(desc);
			}
			throw ColumnTypeError("Not existing columns type was provided while trying create column");

		}

		double deserialize_d(std::string v) {

			return std::stod(v);

		}

		int deserialize_i(std::string v) {

			return std::stoi(v);

		}




	} // end datatypes

	std::vector<std::string> split(std::string s, std::set<char> delimeters) {

		std::vector<std::string> container;
		std::string cur = "";
		int i = 0;
		while (i < s.size()) {

			if (delimeters.find(s[i]) != delimeters.end()) {

				if (cur.size()) {

					container.push_back(cur);

				}
				cur = "";

			}
			else {

				cur += s[i];

			}
			++i;

		}
		if (cur.size()) {

			container.push_back(cur);

		}
		return container;

	}

	/*
	Column* Table::operator[](std::string name) {

		// TODO call 
		return this->container[name];

	}
	*/
	/*
	void Table::create_info_from_meta() {

		std::cout << Table::table_name << std::endl;
		for (auto& item : Table::meta)
		{
			std::cout << item.first << " : " << item.second.name << std::endl; //Вывод ключей и значений
		}


	}
	*/

} // end db