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
*/



class TestU : public db::Table {

public:

	OBJ_TABLE(TestU)
	TestU() {

		START_DECL(TestU)
			COLUMN(id, db::datatypes::INTEGER)
			COLUMN(name, db::datatypes::STRING)
			PRIMARY_KEY(id)
		END_DECL(TestU)
	}

};

OBJ_DECL(TestU);


class TestUVisit : public db::Table {

public:

	OBJ_TABLE(TestUVisit)
	TestUVisit() {

		START_DECL(TestUVisit)
			COLUMN(id, db::datatypes::INTEGER)
			COLUMN(user_id, db::datatypes::INTEGER)
			COLUMN(day, db::datatypes::INTEGER)
			COLUMN(money_paid, db::datatypes::FLOAT)
			PRIMARY_KEY(id)
			FOREIGN_KEY(user_id, TestU, id)
		END_DECL(TestUVisit)

	}

};

OBJ_DECL(TestUVisit)