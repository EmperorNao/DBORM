#pragma once
#include "../../src/db/Table.h"
#include <iostream>



/*MACROS TO DEFINE YOUR OBJECT
* MUST HAD EMPTY CONSTRUCTOR
* YOU DON'T NEED TO DESCRIBE YOUR CLASS FIELDS INSIDE CLASS BODY
*
* class User {
*
* ... <your code here > ...
*
* public:
*
*		User() {
*
*			START_DECL(User)
*				COLUMN(id, INTEGER)
*				COLUMN(name, STRING)
*				PRIMARY_KEY(id)
*			END_DECL(User)
*
*
*		}
*
* }
* OBJ_DECL(User)
* 
* if you are using namespaces, in few situations you'll need to use 
* using namespace namespace_name;
* because of parts realization
* 
* 
*/


namespace test {

	class TestU : public db::Table {

	public:

		OBJ_TABLE(TestU)
			TestU() {

			START_DECL(TestU)
				COL(id, db::datatypes::INTEGER)
				COL(name, db::datatypes::STRING)
				PRIMARY_KEY(id)
			END_DECL(TestU);

		}

	};
	OBJ_DECL(TestU)


	class TestUVisit : public db::Table {

	public:

		OBJ_TABLE(TestUVisit)
			TestUVisit() {

			START_DECL(TestUVisit)
				COL(id, db::datatypes::INTEGER)
				COL(user_id, db::datatypes::INTEGER)
				COL(day, db::datatypes::INTEGER)
				COL(money_paid, db::datatypes::FLOAT)
				PRIMARY_KEY(id)
				FOREIGN_KEY(user_id, TestU, id)
			END_DECL(TestUVisit)

		}

	};

	OBJ_DECL(TestUVisit)

}