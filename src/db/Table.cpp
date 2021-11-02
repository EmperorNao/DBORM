#include "Table.h"
#include <iostream>


namespace db {

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

			switch (desc.get_type())
			{
			case INTEGER:
				return new Integer(t, desc);
			case STRING:
				return new String(t, desc);
			case FLOAT:
				return new Float(t, desc);
			default:
				throw ColumnTypeError("Not existing columns type was provided while trying create column");
			}


		}


	} // end datatypes

	Column* Table::operator[](std::string name) {

		// TODO call 
		return this->container[name];

	}
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