#include <gtest/gtest.h>
#include "User.h"
#include <libpq-fe.h>
#include "db_test/Statement_test.h"
#include "sql_test/Query_test.h"
#include "sql_test/QueryBuilder_test.h"


static void
exit_nicely(PGconn* conn)
{
    PQfinish(conn);
    exit(1);
}


const int DEBUG = 0;


int main(int argc, char* argv[]) {


	if (DEBUG == 1) {

		//std::cout << "First";
		//User u1;
		//User u2;
		//UserVisit uv1;

		//SET(u1, name, "Alex");
		//SET(u1, id, 55);
		//int id = GET(u1, id, int);
		//std::string name = GETS(u1, name, std::string);


	}
	else {

		testing::InitGoogleTest(&argc, argv);
		//testing::InitGoogleTest();
		RUN_ALL_TESTS();

	}

}