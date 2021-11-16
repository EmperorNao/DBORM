#pragma once
#include "../TestUser.h"
#include "../../src/db/Table.h"
#include "../../src/sql/Session.h"
#include "../TestEngine.h"
#include <gtest/gtest.h>


TEST(ResultTest, DesctructorCallTest) {

	using namespace test;
	TestEngine engine;
	sql::Session s(&engine);
	test::TestResult* res = (test::TestResult*)s.select(TBL(TestU), COLUMNS(id, name))->execute();

}


TEST(ResultTest, FewResultsTest) {

	using namespace test;
	TestEngine engine;
	sql::Session s(&engine);
	test::TestResult* res = (test::TestResult*)s.select(TBL(TestU), COLUMNS(id, name))->execute();
	res = (test::TestResult*)s.select(TBL(TestU), COLUMNS(id))->execute();
	res = (test::TestResult*)s.select(TBL(TestU), COLUMNS(name))->execute();

}
