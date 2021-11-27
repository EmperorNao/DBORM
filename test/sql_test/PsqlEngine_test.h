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

	PsqlEngine e("ormtest", "postgres", "password");
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
	using namespace test;
	PsqlEngine e("ormtest", "postgres", "password");
	sql::Session s(&e);

	try {

		s.del(TBL(TestU))->execute();
		s.del(TBL(TestUVisit))->execute();

		test::TestU u1;
		SET(u1, name, "FirstTestUser");
		SET(u1, id, 0);
		s.insert(TBL(TestU), &u1)->execute();

		SET(u1, name, "SecondTestUser");
		SET(u1, id, 1);
		s.insert(TBL(TestU), &u1)->execute();

		test::TestUVisit v1;

		SET(v1, id, 0);
		SET(v1, user_id, 0);
		SET(v1, day, 3);
		SET(v1, money_paid, 25.5);
		s.insert(TBL(TestUVisit), &v1)->execute();

		SET(v1, id, 1);
		SET(v1, user_id, 0);
		SET(v1, day, 5);
		SET(v1, money_paid, 72.6);
		s.insert(TBL(TestUVisit), &v1)->execute();

		s.migrate("P:\\D\\Programming\\DBORM\\migration_test", "test", sql::MigrationFormat::DBORM);

	}
	catch(std::exception* e){

		std::cout << e->what() << std::endl;

	}

}


TEST(PsqlEngineTest, LoadMigrationTest) {

	using namespace sql::psql;
	using namespace test;
	using namespace sql;
	PsqlEngine e("ormtest", "postgres", "password");
	sql::Session s(&e);

	Result* res;

	s.del(TBL(TestU))->execute();
	s.del(TBL(TestUVisit))->execute();

	std::cout << "Before migration\n";
	std::cout << "TestU" << std::endl;
	res = s.select(TBL(TestU), COLUMNS(*))->execute();
	res->out();
	std::cout << "TestUVisit" << std::endl;
	res = s.select(TBL(TestUVisit), COLUMNS(*))->execute();
	res->out();

	s.load_migration("P:\\D\\Programming\\DBORM\\migration_test", "test", sql::MigrationFormat::DBORM);

	std::cout << "After migration\n";
	std::cout << "TestU" << std::endl;
	res = s.select(TBL(TestU), COLUMNS(*))->execute();
	res->out();
	std::cout << "TestUVisit" << std::endl;
	res = s.select(TBL(TestUVisit), COLUMNS(*))->execute();
	res->out();

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


TEST(PsqlEngineTest, GroupByTest) {

	using namespace sql::psql;
	using namespace test;
	PsqlEngine e("ormtest", "postgres", "password");
	sql::Session s(&e);

	try {

		s.del(TBL(TestU))->execute();
		test::TestU u1;
		SET(u1, name, "First");
		SET(u1, id, 0);
		sql::Result* res;

		s.insert(TBL(TestU), &u1)->execute();

		SET(u1, name, "Second");
		SET(u1, id, 1);
		s.insert(TBL(TestU), &u1)->execute();

		SET(u1, name, "First");
		SET(u1, id, 2);
		s.insert(TBL(TestU), &u1)->execute();

		SET(u1, name, "Second");
		SET(u1, id, 100);
		s.insert(TBL(TestU), &u1)->execute();

		res = s.select(TBL(TestU), COLUMNS(name, SUM(id), MAX(id), MIN(id), AVG(id)))->group_by(TBL(TestU), COLUMNS(name))->execute();
		res->out();

		s.del(TBL(TestU))->execute();

	}
	catch (std::exception* e) {

		std::cout << e->what();

	}

}



TEST(PsqlEngineTest, IsolationLevelTest) {

	using namespace sql::psql;
	using namespace test;
	PsqlEngine e("ormtest", "postgres", "password");
	sql::Session s(&e);

	try {

		s.begin();
		s.set_iso_level(sql::IsolationLevel::SERIALIZABLE);
		s.end();

	}
	catch (std::exception* e) {

		std::cout << e->what();

	}

}