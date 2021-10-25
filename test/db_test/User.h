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



class User : public db::Table {

public:

	OBJ_TABLE(User)
	User() {

		START_DECL(User)
			COLUMN(id, db::datatypes::INTEGER)
			COLUMN(name, db::datatypes::STRING)
			PRIMARY_KEY(id)
		END_DECL(User)

	}

};

OBJ_DECL(User);


class UserVisit : public db::Table {

public:

	OBJ_TABLE()
	UserVisit() {

		START_DECL(UserVisit)
			COLUMN(id, db::datatypes::INTEGER)
			COLUMN(user_id, db::datatypes::INTEGER)
			COLUMN(day, db::datatypes::INTEGER)
			PRIMARY_KEY(id)
			FOREIGN_KEY(user_id, User, id)
		END_DECL(UserVisit)

	}

};

OBJ_DECL(UserVisit)
