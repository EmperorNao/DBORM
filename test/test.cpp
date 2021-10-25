#include <gtest/gtest.h>
#include "db_test/User.h"


const int DEBUG = 1;

int main(int argc, char* argv[]) {

	if (DEBUG == 1) {

		std::cout << "HelloWorld";
		User u1();
		User u2();
		UserVisit uv1();

	}
	else {

		testing::InitGoogleTest(&argc, argv);
		//testing::InitGoogleTest();
		RUN_ALL_TESTS();

	}

}