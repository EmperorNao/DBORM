#pragma once
#include "../TestUser.h"
#include "../../src/db/Table.h"
#include "../../src/sql/Session.h"
#include "../TestEngine.h"
#include <gtest/gtest.h>


TEST(QueryTest, QuerySelect) {

	using test::TestU;
	sql::Engine* e = new test::TestEngine();
	sql::Session s(e);
	s.select(TBL(TestU), COLUMNS(first, second, third));

}


TEST(QueryTest, QueryJoin) {

	using test::TestU;
	using test::TestUVisit;

	sql::Engine* e = new test::TestEngine();
	sql::Session s = sql::Session(e);
	s.select(TBL(TestU), COLUMNS(name))->join(TBL(TestU), TBL(TestUVisit), COLUMNS(user_id));

}


TEST(QueryTest, QueryWhere) {

	using test::TestU;
	sql::Engine* e = new test::TestEngine();
	sql::Session s(e);
	test::TestU u;
	s.select(TBL(TestU), COLUMNS(name))->where(u["id"] < 35);

}

