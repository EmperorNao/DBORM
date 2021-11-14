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

	#define END_DECL(classname) for (auto& item : classname::meta) { \
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
	#define	GET(object, column_name, type) object.get<type>(#column_name);
	#define	GETS(object, column_name, type) object.get(#column_name);

	#define TBL(classname) classname::table_name, classname::meta
	#define COLUMNS(...) db::split(#__VA_ARGS__, {' ', ','}) 
	#define STATEMENT(classname, colname, op, value) (constant_##classname[#colname] op value)
	#define LIKE(classname, colname, value) (constant_##classname[#colname].like(value))



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

		std::string get(std::string name) {

			return this->container[name]->get_string();

		}

	};

	namespace datatypes {

		Column* create_column(ColumnDescription desc);

	}

}


