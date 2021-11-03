#include "Table.h"
#include <iostream>


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

		Column* create_column(Table* t, ColumnDescription desc) {

			if (is_columntype_value_correct(desc.type)) {
				return new Column(t, desc);
			}
			throw ColumnTypeError("Not existing columns type was provided while trying create column");

		}


	} // end datatypes

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