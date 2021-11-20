#pragma once
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <cstddef>
#include <concepts>
#include <vector>
#include <set>
#include "Column.h"
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

	#define END_DECL(classname) this->get_pk_key(classname::table_name, classname::meta); \
								for (auto& item : classname::meta) { \
								this->container[item.first] = db::datatypes::create_column(item.second); \
									} \
								} \
								else { \
								for (auto& item : classname::meta) { \
								this->container[item.first] = db::datatypes::create_column(item.second); }}


	#define COL(name, type, ...) this->meta[#name] = db::ColumnDescription(table_name, #name, type);
	#define PRIMARY_KEY(name) this->meta[#name].set_pk();
	#define FOREIGN_KEY(name, tablename_on, colname) this->meta[#name].set_fk(tablename_on::table_name, tablename_on::table_name);

	#define SET(object, column_name, value) object.set(#column_name, value);
	#define SET_TO_POINTER(object, column_name, value) object->set(#column_name, value);
	#define	GET(object, column_name, type) object.get<type>(#column_name);
	#define	GET_FROM_POINTER(object, column_name, type) object->get<type>(#column_name);
	#define	GETS(object, column_name, type) object.get(#column_name);

	#define TBL(classname) classname::table_name, classname::meta
	#define COLUMNS(...) db::split(#__VA_ARGS__, {' ', ','}) 
	#define STATEMENT(classname, colname, op, value) (constant_##classname[#colname] op value)
	#define LIKE(classname, colname, value) (constant_##classname[#colname].like(value))


	#define AS_STR(str) #str
	#define SPLAY(name, column) AS_STR(name(##column##))

	#define MAX(column) SPLAY(MAX, column)
	#define MIN(column) SPLAY(MIN, column)
	#define AVG(column) SPLAY(AVG, column)
	#define SUM(column) SPLAY(SUM, column)


	class TableError : public std::exception {

		std::string message;
	public:
		TableError(std::string m) : message(m) {};
		virtual const char* what() const throw()
		{
			return message.c_str();
		}


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
			throw new ColumnNameError(name);

		
		};

		virtual std::string get_pk_key(std::string table_name, meta_info meta) {

			std::vector<std::string> keys = {};
			for (auto el : meta) {

				if (el.second.pk) {

					keys.push_back(el.first);

				}

			}
			if (keys.size() != 1) {

				throw new TableError("Number of primary keys in table " + table_name + 
					" is wrong : " + std::to_string(keys.size()));

			}
			return keys[0];

		}

		template <datatypes::convertable_to_str T>
		void set(std::string name, T r_value) {

			try {

				this->container[name]->set(r_value);

			}
			catch (datatypes::ValueError& e) {

				std::cerr << e.what() << std::endl;
				return;

			}

		}

		void set(std::string name, std::string r_value) {

			this->container[name]->set_string(r_value);

		}

		template <datatypes::convertable_to_str T>
		T get(std::string name) {

			return this->container[name]->get<T>();
			
		}

		std::string get(std::string name, bool str = true) {

			std::string subs = this->container[name]->get_string();
			if (not subs.size()) {

				return "\'\'";

			}
			if (str and (subs.size() > 2) and (subs[0] == '\'') and (subs[subs.size() - 1] == '\'')) {

				return subs.substr(1, subs.size() - 2);

			}
			return subs;

		}

	};

	namespace datatypes {

		Column* create_column(ColumnDescription desc);

	}

}


