#pragma once
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <cstddef>
#include <concepts>
//#include "DataTypes.h"



namespace db {

	/*
	* ACTIVE MACROS
	*/
	#define OBJ_DECL(classname) static classname constant_##classname;
	#define OBJ_TABLE(classname) public: \
										static inline std::map<std::string, db::ColumnDescription> meta;\
										static inline bool exist = false; \
										static inline std::string table_name; \
										//static Session* session;

	#define SET_NAME(classname) classname::table_name = #classname;
	#define START_DECL(classname) SET_NAME(classname)	\
									if (! classname::exist) {	\
										classname::exist = true;

	#define END_DECL(classname) } else { \
								for (auto& item : classname::meta) { \
								this->container[item.first] = db::datatypes::create_column(this, item.second); }}


	#define COLUMN(name, type, ...) this->meta[#name] = db::ColumnDescription(#name, type);
	#define PRIMARY_KEY(name) this->meta[#name].set_pk();
	#define FOREIGN_KEY(name, tablename_on, colname) this->meta[#name].set_fk(tablename_on::table_name, tablename_on::table_name);



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
		concept convertable = requires(T a) {
			{ std::to_string(a)};
		};

		template <convertable T>//*/
		//template <typename T>
		std::string serialize(T value) {

			try {
				return std::to_string(value);
			}
			catch (...) {

				throw ValueError("Wrong value was provided to serialization");

			}

		}


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
		datatypes::ColumnType type;
		bool pk;
		bool fk;
		std::string fk_table;
		std::string fk_column = "";

		ColumnDescription() {};

		ColumnDescription(
			std::string _name,
			datatypes::ColumnType _type,
			bool _pk = false,
			bool _fk = false,
			std::string _fk_table = "",
			std::string _fk_column = ""
		) : name(_name), type(_type), pk(_pk), fk(_fk), fk_table(_fk_table), fk_column(_fk_column)
		{};

		datatypes::ColumnType get_type() { return type; }
		void set_pk() { pk = true; }
		void set_fk(std::string _fk_table, std::string _fk_atribute) {
			fk = true;
			fk_table = _fk_table;
			fk_column = _fk_atribute;
		}

	};


	class Column;

	class Table {
	

	public:

		std::map<std::string, Column*> container;
		Table() {};
		//Column operator[](std::string name) { return *(this->container[name]); }

		virtual Column* get_real_column(std::string name) { 
			
			if (container.find(name) != container.end()) {

				return this->container[name];

			}
			throw ColumnNameError(name);

		
		};

		void update(std::string column_name) {

			// action in case of update column

		}


	};

	class Column {

		std::string value;
		datatypes::ColumnType type;
		Table* table;
		std::string name;
		bool pk;
		bool fk;
		std::string fk_table;
		std::string fk_atribute;

	public:
		Column(Table* _table, 
			std::string _name, 
			datatypes::ColumnType _type,
			bool _pk = false, 
			bool _fk = false, 
			std::string _fk_table = "",
			std::string _fk_atribute = "") 
			: table(_table), name(_name), type(_type), pk(_pk), fk(_fk), fk_table(_fk_table), fk_atribute(_fk_atribute) {};

		Column(Table* _table, const ColumnDescription& desc)
			: table(_table), name(desc.name), type(desc.type), pk(desc.pk), fk(desc.fk), fk_table(desc.fk_table), fk_atribute(desc.fk_column) {}

		template <datatypes::convertable T>
		void operator=(T r_value) { 
			
			std::cout << "FOCKING UPDATE ON " << r_value << std::endl;
			try {
				this->table->get_real_column(this->name)->value = db::datatypes::serialize(r_value);
			}
			catch (datatypes::ValueError& e) {

				std::cerr << e.what() << std::endl;
				return;

			}

		}

		void operator=(std::string r_value) {

			this->table->get_real_column(this->name)->value = r_value;

		}

		/*template <const char*>
		void operator=(const char* r_value) {

			std::cout << "FOCKING UPDATE ON " << r_value << std::endl;
			this->table->get_real_column(this->name)->value = r_value;

		}*/


	};


	namespace datatypes {


		Column* create_column(Table* t, ColumnDescription desc);

	}

}


