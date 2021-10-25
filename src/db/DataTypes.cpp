#include "DataTypes.h"


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

		Column* create_column(ColumnDescription desc) {

			if (not is_columntype_value_correct(desc.get_type())) {

				throw ColumnTypeError("Not existing columns type was provided while trying create column");

			}
			switch (desc.get_type())
			{
			case INTEGER:

			case STRING:
				break;
			case FLOAT:
				break;
			default:
				break;
			}

		}


	} // end datatypes
} // end db