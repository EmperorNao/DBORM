#pragma once
#include "../TestUser.h"
#include "../../src/db/Table.h"
#include <gtest/gtest.h>


TEST(StatementTest, Equality) {

	using namespace test;
	EXPECT_EQ(STATEMENT(TestU, id, == , 1).get_state(), "TestU.id = 1");

}


TEST(StatementTest, Not) {

	using namespace test;
	EXPECT_EQ(STATEMENT(TestU, id, !=, 1).get_state(), "TestU.id != 1");

}



TEST(StatementTest, Greater) {

	using namespace test;
	EXPECT_EQ(STATEMENT(TestU, id, > , 1).get_state(), "TestU.id > 1");

}


TEST(StatementTest, Lower) {

	using namespace test;
	EXPECT_EQ(STATEMENT(TestU, id, < , 1).get_state(), "TestU.id < 1");

}


TEST(StatementTest, GreaterEqual) {

	using namespace test;
	EXPECT_EQ(STATEMENT(TestU, id, >= , 1).get_state(), "TestU.id >= 1");

}


TEST(StatementTest, LowerEqual) {

	using namespace test;
	EXPECT_EQ(STATEMENT(TestU, id, <= , 1).get_state(), "TestU.id <= 1");

}


TEST(StatementTest, Like) {

	using namespace test;
	EXPECT_EQ(LIKE(TestU, name, "Ararat").get_state(), "TestU.name like 'Ararat'");

}


TEST(StatementTest, Complex) {

	using namespace test;
	EXPECT_EQ((LIKE(TestU, name, "Vlad") and STATEMENT(TestU, id, ==, 1)).get_state(), "(TestU.name like 'Vlad') and (TestU.id = 1)");

}
