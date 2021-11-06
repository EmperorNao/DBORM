#pragma once
#include "User.h"
#include "../../src/db/Table.h"
#include <gtest/gtest.h>


TEST(StatementTest, Equality) {

	TestU u1;
	EXPECT_EQ((u1["id"] == 1).get_state(), "TestU.id = 1");

}

TEST(StatementTest, Greater) {

	TestU u1;
	EXPECT_EQ((u1["id"] > 1).get_state(), "TestU.id > 1");

}

TEST(StatementTest, Lower) {

	TestU u1;
	EXPECT_EQ((u1["id"] < 1).get_state(), "TestU.id < 1");

}

TEST(StatementTest, GreaterEqual) {

	TestU u1;
	EXPECT_EQ((u1["id"] >= 1).get_state(), "TestU.id >= 1");

}


TEST(StatementTest, LowerEqual) {

	TestU u1;
	EXPECT_EQ((u1["id"] <= 1).get_state(), "TestU.id <= 1");

}


TEST(StatementTest, Like) {

	TestU u1;
	EXPECT_EQ((u1["name"].like("Ararat")).get_state(), "TestU.name like 'Ararat'");

}



