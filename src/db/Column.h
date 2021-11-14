#pragma once
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <cstddef>
#include <concepts>
#include <vector>
#include <set>
#include <exception>


namespace db {

	namespace datatypes {

		enum ColumnType {

			INTEGER = 0,
			STRING = 1,
			FLOAT = 2

		};
		bool is_columntype_value_correct(ColumnType v);

		class ColumnTypeError : public std::exception {

			std::string message;
		public:
			ColumnTypeError(std::string m) : message(m) {};

			virtual const char* what() const throw()
			{
				return message.c_str();
			}

		};

		class ValueError : public std::exception {

			std::string message;
		public:
			ValueError(std::string m) : message(m) {};

			virtual const char* what() const throw()
			{
				return message.c_str();
			}


		};

		template <typename T>
		concept convertable_to_str = requires(T a) {
			{ std::to_string(a)};
		};

		template <convertable_to_str T>
		std::string serialize(T value) {

			try {
				return std::to_string(value);
			}
			catch (...) {

				throw new ValueError("Wrong value was provided to serialization");

			}

		}
		double deserialize_d(std::string v);
		int deserialize_i(std::string v);

	} // end datatypes


	class ColumnNameError : public std::exception {

		std::string message;
	public:
		ColumnNameError(std::string colname)
			: message("Do not find column " + colname + " in table") {};

		virtual const char* what() const throw()
		{
			return message.c_str();
		}

	};


	class ColumnDescription {

	public:
		std::string name;
		std::string table_name;
		datatypes::ColumnType type;
		bool pk;
		bool fk;
		std::string fk_table;
		std::string fk_column = "";

		ColumnDescription() {};

		ColumnDescription(
			std::string _table_name,
			std::string _name,
			datatypes::ColumnType _type,
			bool _pk = false,
			bool _fk = false,
			std::string _fk_table = "",
			std::string _fk_column = ""
		) : name(_name), table_name(_table_name), type(_type), pk(_pk), fk(_fk), fk_table(_fk_table), fk_column(_fk_column)
		{};

		datatypes::ColumnType get_type() { return type; }
		void set_pk() { pk = true; }
		void set_fk(std::string _fk_table, std::string _fk_atribute) {
			fk = true;
			fk_table = _fk_table;
			fk_column = _fk_atribute;
		}

	};
	typedef std::map<std::string, db::ColumnDescription> meta_info;


	class Statement {

	private:
		std::string state;
	public:
		Statement(std::string _state) :
			state(_state) {};

		std::string get_state() const {

			return this->state;

		}


		Statement operator&&(const Statement& r) {

			return Statement("(" + this->state + ")" + " and " + "(" + r.state + ")");

		}

		Statement operator!() {

			return Statement("not (" + this->state + ")");

		}

		Statement operator||(const Statement& r) {

			return Statement("(" + this->state + ")" + " or " + "(" + r.state + ")");

		}

	};


	class Column {

		std::string value;
		datatypes::ColumnType type;
		std::string table_name;
		std::string name;
		bool pk;
		bool fk;
		std::string fk_table;
		std::string fk_atribute;

	public:
		Column(
			std::string _table_name,
			std::string _name,
			datatypes::ColumnType _type,
			bool _pk = false,
			bool _fk = false,
			std::string _fk_table = "",
			std::string _fk_atribute = "")
			: name(_name), table_name(_table_name), type(_type), pk(_pk),
			fk(_fk), fk_table(_fk_table), fk_atribute(_fk_atribute) {};

		Column(const ColumnDescription& desc)
			: name(desc.name), table_name(desc.table_name), type(desc.type), pk(desc.pk),
			fk(desc.fk), fk_table(desc.fk_table), fk_atribute(desc.fk_column) {}

		// Main parts
		// mutators and accessors

		std::string get_table() const {

			return this->table_name;

		}

		template <datatypes::convertable_to_str T>
		std::string serialize(T v) const {

			return db::datatypes::serialize(v);

		}

		template <datatypes::convertable_to_str T>
		void set(T r_value) {

			try {
				this->value = this->serialize(r_value);
			}
			catch (datatypes::ValueError& e) {

				std::cerr << e.what() << std::endl;
				return;

			}

		}

		template <datatypes::convertable_to_str T>
		T get() const {

			std::string value = this->value;
			switch (this->type)
			{
			case datatypes::ColumnType::INTEGER:
				return datatypes::deserialize_i(value);
			case datatypes::ColumnType::FLOAT:
				return datatypes::deserialize_d(value);
			default:
				throw new datatypes::ColumnTypeError("Wrong type was in column while trying get attibute");
			}

		}

		std::string get_string() const {

			return this->value;

		}

		void set_string(std::string r_value) {

			this->value = r_value;

		}


		// Generating statements
		template <datatypes::convertable_to_str T>
		Statement operator>(T r) { return Statement(this->table_name + "." + this->name + " > " + this->serialize(r)); };

		template <datatypes::convertable_to_str T>
		Statement operator<(T r) { return Statement(this->table_name + "." + this->name + " < " + this->serialize(r)); };

		template <datatypes::convertable_to_str T>
		Statement operator==(T r) { return Statement(this->table_name + "." + this->name + " = " + this->serialize(r)); };

		template <datatypes::convertable_to_str T>
		Statement operator!=(T r) { return Statement(this->table_name + "." + this->name + " != " + this->serialize(r)); };

		template <datatypes::convertable_to_str T>
		Statement operator>=(T r) { return Statement(this->table_name + "." + this->name + " >= " + this->serialize(r)); };

		template <datatypes::convertable_to_str T>
		Statement operator<=(T r) { return Statement(this->table_name + "." + this->name + " <= " + this->serialize(r)); };

		Statement like(std::string r) { return Statement(this->table_name + "." + this->name + " like " + "'" + r + "'"); }

	};

}