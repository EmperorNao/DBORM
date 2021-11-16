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


TEST(PsqlEngineTest, SelectJoinTest) {

	using namespace sql::psql;
	using namespace test;

	PsqlEngine e("postgres", "postgres", "password");
	sql::Session s(&e);
	try {
		sql::Result* res = s.select(TBL(TestU), COLUMNS(name))->select(TBL(TestUVisit), COLUMNS(money_paid))->join(TBL(TestU), TBL(TestUVisit), COLUMNS(user_id))->execute();
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
	s.migrate("P:\\D\\Programming\\DBORM\\migration_test", "test", sql::MigrationFormat::DBORM);

}


TEST(PsqlEngineTest, InsertDeleteTest) {

	using namespace sql::psql;
	using namespace test;
	PsqlEngine e("ormtest", "postgres", "password");
	sql::Session s(&e);

	test::TestU u1;
	SET(u1, name, "FirstTestUser");
	SET(u1, id, 0);
	sql::Result* res;

	try {

		std::cout << "Table before delete" << std::endl;
		res = s.select(TBL(TestU), COLUMNS(*))->execute();
		res->out();

		s.del(TBL(TestU), &u1)->execute();

		std::cout << "Table after delete" << std::endl;
		res = s.select(TBL(TestU), COLUMNS(*))->execute();
		res->out();

		int start_nrows = res->get_nrows();

		s.insert(TBL(TestU), &u1)->execute();

		std::cout << "Table after insert" << std::endl;
		res = s.select(TBL(TestU), COLUMNS(*))->execute();
		res->out();
		int after_nrows = res->get_nrows();

		s.del(TBL(TestU), &u1)->execute();

		EXPECT_EQ(start_nrows, after_nrows - 1);

	}
	catch (std::exception* e) {

		std::cout << e->what();

	}

}


TEST(PsqlEngineTest, UpdateTest) {

	using namespace sql::psql;
	using namespace test;
	PsqlEngine e("ormtest", "postgres", "password");
	sql::Session s(&e);

	test::TestU u1;
	SET(u1, name, "FirstTestUser");
	SET(u1, id, 0);
	sql::Result* res;

	try {

		std::cout << "Table before insert" << std::endl;
		res = s.select(TBL(TestU), COLUMNS(*))->execute();
		res->out();

		s.insert(TBL(TestU), &u1)->execute();

		std::cout << "Table after insert" << std::endl;
		res = s.select(TBL(TestU), COLUMNS(*))->execute();
		res->out();

		SET(u1, name, "NEWSPECIALTESTNAME");
		s.update(TBL(TestU), &u1)->execute();

		std::cout << "Table after update" << std::endl;
		res = s.select(TBL(TestU), COLUMNS(*))->execute();
		res->out();

		s.del(TBL(TestU), &u1)->execute();

	}
	catch (std::exception* e) {

		std::cout << e->what();

	}

}

