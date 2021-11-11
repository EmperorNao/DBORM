#pragma once
#include "../TestUser.h"
#include "../../src/db/Table.h"
#include "../../src/sql/Session.h"
#include <gtest/gtest.h>


TEST(TableTest, TableChanging) {

	test::TestU u1;
	SET(u1, name, "Alex");
	SET(u1, id, 1);

	std::string name = GETS(u1, name);
	int id = GET(u1, id, int);

	ASSERT_EQ(name, "Alex");
	ASSERT_EQ(id, 1);

}


TEST(TableTest, TableWrong) {

	test::TestU u1;
	SET(u1, name, 25);
	SET(u1, id, "azaz");
	ASSERT_ANY_THROW(int myid = GET(u1, id, int));

}