#pragma once
#include "../TestUser.h"
#include "../../src/db/Table.h"
#include "../../src/sql/Session.h"
#include "../../src/sql/psql/PsqlEngine.h"
#include <gtest/gtest.h>



TEST(PsqlEngineTest, ConnectionTest) {

	using namespace sql::psql;
	
	EXPECT_THROW(PsqlEngine e("postgres", "randomname", "randompassword"), sql::EngineError*);

}


TEST(PsqlEngineTest, SelectTest) {

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

}


TEST(PsqlEngineTest, BeginTest) {

	using namespace sql::psql;

	PsqlEngine e("postgres", "postgres", "password");
	e.begin();

}


TEST(PsqlEngineTest, EndTest) {

	using namespace sql::psql;

	PsqlEngine e("postgres", "postgres", "password");
	e.begin();
	e.end();

}


TEST(PsqlEngineTest, CommitTest) {

	using namespace sql::psql;

	PsqlEngine e("postgres", "postgres", "password");
	e.begin();
	e.commit();

}



TEST(PsqlEngineTest, MigrationTest) {

	using namespace sql::psql;
	PsqlEngine e("ormtest", "postgres", "password");
	sql::Session s(&e);
	s.migrate();

}