#include <gtest/gtest.h>
#include <libpq-fe.h>

#include "db_test/Table_test.h"
#include "db_test/Statement_test.h"
#include "sql_test/Query_test.h"
#include "sql_test/QueryBuilder_test.h"
#include "sql_test/Result_test.h"
#include "sql_test/PsqlEngine_test.h"

static void
exit_nicely(PGconn* conn)
{
    PQfinish(conn);
    exit(1);
}


const int DEBUG = 1;


int main(int argc, char* argv[]) {


	if (DEBUG == 1) {

        using namespace sql::psql;

        PsqlEngine e("postgres", "postgres", "password");
        sql::Session s(&e);
        try {
            PsqlResult* res = (PsqlResult*)s.select("pg_database", {}, COLUMNS(*))->execute();
            res->out();
        }
        catch (std::exception* e) {

            std::cout << "Exception goes\n" << e->what();

        }

        return 0;


	}
	else {

		testing::InitGoogleTest(&argc, argv);
		//testing::InitGoogleTest();
		RUN_ALL_TESTS();

	}

}