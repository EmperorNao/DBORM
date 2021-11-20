#include <gtest/gtest.h>
#include <libpq-fe.h>

#include "db_test/Table_test.h"
#include "db_test/Statement_test.h"
#include "sql_test/Query_test.h"
#include "sql_test/QueryBuilder_test.h"
#include "sql_test/Result_test.h"
#include "sql_test/PsqlEngine_test.h"

#include "../benchmark/benchmarking.h"


const int DEBUG = 1;


int main(int argc, char* argv[]) {


	if (DEBUG == 1) {

		for (int i = 100000; i < 250001; i+= 25000) {

			std::cout << "ELEMENTS: " << i << std::endl;
			do_benchmark("plot.txt", i);
			std::cout << std::endl;

		}

	}
	else {

		testing::InitGoogleTest(&argc, argv);
		//testing::InitGoogleTest();
		RUN_ALL_TESTS();

	}

}