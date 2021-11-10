#pragma once
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <cstddef>
#include <concepts>
#include <vector>
#include <set>
//#include "DataTypes.h"



namespace db {

	std::vector<std::string> split(std::string s, std::set<char> delimeters);
	/*
	* ACTIVE MACROS
	*/
	#define OBJ_DECL(classname) static classname constant_##classname;
	#define OBJ_TABLE(classname) public: \
										static inline db::meta_info meta;\
										static inline bool exist = false; \
										static inline std::string table_name; \
										//static Session* session;

	#define SET_NAME(classname) classname::table_name = #classname;
	#define START_DECL(classname) SET_NAME(classname)	\
									if (! classname::exist) {	\
										classname::exist = true;

	#define END_DECL(classname) } else { \
								for (auto& item : classname::meta) { \
								this->container[item.first] = db::datatypes::create_column(item.second); }}


	#define COL(name, type, ...) this->meta[#name] = db::ColumnDescription(table_name, #name, type);
	#define PRIMARY_KEY(name) this->meta[#name].set_pk();
	#define FOREIGN_KEY(name, tablename_on, colname) this->meta[#name].set_fk(tablename_on::table_name, tablename_on::table_name);
	#define SET(object, column_name, value) object.set(#column_name, value);
	#define	GET(object, column_name, type) object.get<type>(#column_name);
	#define	GETS(object, column_name, type) object.get(#column_name);

	#define TBL(classname) classname::table_name, classname::meta
	#define COLUMNS(...) db::split(#__VA_ARGS__, {' ', ','}) 
	#define STATEMENT(classname, colname, op, value) constant_##classname[#colname] op value



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

		class ValueError {

			std::string message;
		public:
			ValueError(std::string m) : message(m) {};

			virtual std::string what() const throw()
			{
				return message;
			}

		};



		template <typename T>
		concept convertable_to_str = requires(T a) {
			{ std::to_string(a)};
		};

		/*template <typename T>
		concept is_str = requires(T a) {
			{ a } -> std::convertible_to<std::string>;
		};


		template <typename T> 
		concept convertable = requires(T) {
			convertable_to_str<T>;
		};
		*/

		template <convertable_to_str T>
		std::string serialize(T value) {

			try {
				return std::to_string(value);
			}
			catch (...) {

				throw ValueError("Wrong value was provided to serialization");

			}

		}

		double deserialize_d(std::string v);

		int deserialize_i(std::string v);


		/*class DataType {

			ColumnType type;
		public:
			DataType(ColumnType t) : type(t) {};

		};

		class String: public DataType {

			String(): DataType(STRING) {}
			

		};*/

	} // end datatypes


	class ColumnNameError {

		std::string message;
	public:
		ColumnNameError(std::string colname)
			: message("Do not find column " + colname + " in table") {};

		virtual std::string what() const throw()
		{
			return message;
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

			return Statement("(" + this->state + " )" + " and " + "(" + r.state + ")");

		}

		Statement operator!() {

			return Statement("not (" + this->state + " )");

		}

		Statement operator||(const Statement& r) {

			return Statement("(" + this->state + " )" + " or " + "(" + r.state + ")");

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

			std::cout << "FOCKING UPDATE ON " << r_value << " IN operator=" << std::endl;
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
				throw datatypes::ColumnTypeError("Wrong type was in column while trying get attibute");
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
		Statement operator>=(T r) { return Statement(this->table_name + "." + this->name + " >= " + this->serialize(r)); };

		template <datatypes::convertable_to_str T>
		Statement operator<=(T r) { return Statement(this->table_name + "." + this->name + " <= " + this->serialize(r)); };

		Statement like(std::string r) { return Statement(this->table_name + "." + this->name + " like " + "'" + r + "'"); }

	};


	class Table {
	

	public:

		std::map<std::string, Column*> container;
		Table() {};
		Column operator[](std::string name) { return *(this->container[name]); }

		virtual Column* get_real_column(std::string name) { 
			
			if (container.find(name) != container.end()) {

				return this->container[name];

			}
			throw ColumnNameError(name);

		
		};

		template <datatypes::convertable_to_str T>
		void set(std::string name, T r_value) {

			std::cout << "FOCKING UPDATE ON " << r_value << " IN SET" << std::endl;
			try {

				this->container[name]->set(r_value);

			}
			catch (datatypes::ValueError& e) {

				std::cerr << e.what() << std::endl;
				return;

			}

		}

		void set(std::string name, std::string r_value) {

			this->container["name"]->set_string(r_value);

		}

		template <datatypes::convertable_to_str T>
		T get(std::string name) {

			return this->container[name]->get<T>();
			
		}

		std::string get(std::string name) {

			return this->container[name]->get_string();

		}

	};

	namespace datatypes {


		Column* create_column(ColumnDescription desc);

	}

}


