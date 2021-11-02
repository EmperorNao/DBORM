#pragma once
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
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

	#define SET_STR(obj, name, value) ((db::String*)obj[#name])->set(value);
#define GET_STR(obj, name) ((db::String*)obj[#name])->get();
	#define STR(var) db::String*(var)
	#define INT db::Integer*
	#define FL db::Float*

	#define BY_TYPE(type) CLASS_##type();

	//#define SET(object, col_name, value) db::ct = object.meta[#col_name]->name; \
		//					(BY_TYPE(ct)(object[#col_name]) = value;


	class Column;

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


	class Table {
	

	public:

		std::map<std::string, Column*> container;
		Table() {};
		virtual Column* operator[](std::string s) final;
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

	protected:
		Table* table;
		std::string name;
		bool pk;
		bool fk;
		std::string fk_table;
		std::string fk_atribute;

	public:
		Column(Table* _table, 
			std::string _name, 
			bool _pk = false, 
			bool _fk = false, 
			std::string _fk_table = "",
			std::string _fk_atribute = "") 
			: table(_table), name(_name), pk(_pk), fk(_fk), fk_table(_fk_table), fk_atribute(_fk_atribute) {};

	};


	class String: public Column {

		std::string value;
	public:
		String(Table* _table,
			std::string _name,
			bool _pk = false,
			bool _fk = false,
			std::string _fk_table = "",
			std::string _fk_atribute = "") 
			: Column(_table, _name, _pk, _fk, _fk_table, _fk_atribute) {};

		String(Table* _table, ColumnDescription desc)
			: Column(_table, desc.name, desc.pk, desc.fk, desc.fk_table, desc.fk_column) {};

		void operator=(std::string r) {

			String* real_column = (String*)(this->table->get_real_column(this->name));
			if (real_column->value != r) {

				std::cout << "Update!\n";
				real_column->value = r;

			}

		}

		void set(std::string s) {

			std::cout << "Update!\n";
			this->value = s;

		}

		std::string get() { return value; };

	};


	class Integer : public Column {

		int_fast32_t value;
	public:
		Integer(Table* _table,
			std::string _name,
			bool _pk = false,
			bool _fk = false,
			std::string _fk_table = "",
			std::string _fk_atribute = "")
			: Column(_table, _name, _pk, _fk, _fk_table, _fk_atribute) {};

		Integer(Table* _table, ColumnDescription desc)
			: Column(_table, desc.name, desc.pk, desc.fk, desc.fk_table, desc.fk_column) {};

		void operator=(int_fast32_t r) {

			Integer* real_column = (Integer*)(this->table->get_real_column(this->name));
			if (real_column->value != r) {

				std::cout << "Update!\n";
				real_column->value = r;

			}

		}

		void set(int v) {

			std::cout << "Update!\n";
			this->value = v;

		}

		int get() { return value; };



	};


	class Float : public Column {

		 float_t value;
	public:
		Float(Table* _table,
			std::string _name,
			bool _pk = false,
			bool _fk = false,
			std::string _fk_table = "",
			std::string _fk_atribute = "")
			: Column(_table, _name, _pk, _fk, _fk_table, _fk_atribute) {};

		Float(Table* _table, ColumnDescription desc)
			: Column(_table, desc.name, desc.pk, desc.fk, desc.fk_table, desc.fk_column) {};

		void operator=(float_t r) {

			Float* real_column = (Float*)(this->table->get_real_column(this->name));
			if (real_column->value != r) {

				std::cout << "Update!\n";
				real_column->value = r;

			}

		}

		void set(float v) {

			std::cout << "Update!\n";
			this->value = v;

		}

		float get() { return value; };

	};



	namespace datatypes {

		Column* create_column(Table* t, ColumnDescription desc);

	}

}


