#include <gtest/gtest.h>
#include "db_test/User.h"
#include <libpq-fe.h>


static void
exit_nicely(PGconn* conn)
{
    PQfinish(conn);
    exit(1);
}


const int DEBUG = 1;


int main(int argc, char* argv[]) {


	if (DEBUG == 1) {

		std::cout << "First";
		User u1;
		User u2;
		UserVisit uv1;
		u1["name"] = "Alex";
		u1["id"] = 55;
		//SET_STR(u1, name, "32")
		//std::cout << GET_STR(u1, name);

	}
	else {

		testing::InitGoogleTest(&argc, argv);
		//testing::InitGoogleTest();
		RUN_ALL_TESTS();

	}

}