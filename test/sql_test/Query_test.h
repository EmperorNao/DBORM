#pragma once
#include "../User.h"
#include "../../src/db/Table.h"
#include "../../src/sql/Session.h"
#include <gtest/gtest.h>


TEST(QueryTest, QuerySelect) {

	sql::Engine e;
	sql::Session* s = sql::create_session(&e);
	s->select(TBL(TestU), COLUMNS(first, second, third));

}


TEST(QueryTest, QueryJoin) {

	sql::Engine e;
	sql::Session* s = sql::create_session(&e);
	s->select(TBL(TestU), COLUMNS(name))->join(TBL(TestU), TBL(TestUVisit), COLUMNS(user_id));

}


