#pragma once
#include "Column.h"


namespace db {


	namespace datatypes {

		enum ColumnType {

			INTEGER = 0,
			STRING = 1,
			FLOAT = 2

		};
		bool is_columntype_value_correct(ColumnType v);

		class ColumnTypeError {

			std::string message;
		public:
			ColumnTypeError(std::string m) : message(m) {};

			virtual std::string what() const throw()
			{
				return message;
			}

		};

	} // end datatypes

	class ColumnDescription {


	private:

	public:
		std::string name;
		datatypes::ColumnType type;
		bool pk;
		bool fk;
		std::string fk_table;
		ColumnDescription* fk_atribute;

		ColumnDescription() {};

		ColumnDescription(
			std::string _name,
			datatypes::ColumnType _type,
			bool _pk = false,
			bool _fk = false,
			std::string _fk_table = "",
			ColumnDescription* c = nullptr
		) : name(_name), type(_type), pk(_pk), fk(_fk), fk_table(_fk_table), fk_atribute(c)
		{};

		datatypes::ColumnType get_type() { return type; }
		void set_pk() { pk = true; }
		void set_fk(std::string _fk_table, ColumnDescription* _fk_atribute) {
			fk = true;
			fk_table = _fk_table;
			fk_atribute = _fk_atribute;
		}

	};

	namespace datatypes {

		Column* create_column(ColumnDescription desc);

	}

} // end db