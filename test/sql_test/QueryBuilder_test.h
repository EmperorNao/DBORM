#pragma once
#include "../TestUser.h"
#include "../../src/db/Table.h"
#include "../../src/sql/Session.h"
#include "../../src/sql/QueryBuilder.h"
#include <gtest/gtest.h>


TEST(QueryBuilderTest, Select) {

	sql::QueryBuilder qb;
	using test::TestU;
	sql::Engine* e = new test::TestEngine();
	sql::Session s(e);

	std::string res = qb.build(s.select(TBL(TestU), COLUMNS(id, name))->get_cur_query());
	ASSERT_EQ(res, "SELECT TestU.id, TestU.name FROM TestU");

}


TEST(QueryBuilderTest, Join) {

	sql::QueryBuilder qb;
	using test::TestU;
	using test::TestUVisit;
	sql::Engine* e = new test::TestEngine();
	sql::Session s(e);

	std::string res = qb.build(
		s.
		select(TBL(TestU), COLUMNS(name))->
		join(TBL(TestU), TBL(TestUVisit), COLUMNS(user_id))->
		get_cur_query());
	ASSERT_EQ(res, "SELECT TestU.name FROM TestU JOIN TestUVisit on TestU.id=TestUVisit.user_id");

}


TEST(QueryBuilderTest, WhereSelect) {

	sql::QueryBuilder qb;
	using namespace test;
	sql::Engine* e = new test::TestEngine();
	sql::Session s(e);

	std::string res = qb.build(
		s.
		select(TBL(TestU), COLUMNS(name))->
		where(STATEMENT(TestU, id, <, 35))->
		get_cur_query()
	);
	ASSERT_EQ(res, "SELECT TestU.name FROM TestU WHERE TestU.id < 35");

}


TEST(QueryBuilderTest, WhereJoin) {

	sql::QueryBuilder qb;
	using namespace test;
	sql::Engine* e = new test::TestEngine();
	sql::Session s(e);

	std::string res = qb.build(
		s.
		select(TBL(TestU), COLUMNS(name))->
		join(TBL(TestU), TBL(TestUVisit), COLUMNS(user_id))->
		where(STATEMENT(TestU, id, < , 35))->
		get_cur_query());
	ASSERT_EQ(res, "SELECT TestU.name FROM TestU JOIN TestUVisit on TestU.id=TestUVisit.user_id WHERE TestU.id < 35");

}