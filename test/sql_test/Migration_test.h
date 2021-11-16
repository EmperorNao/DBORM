#pragma once
#include "../TestUser.h"
#include "../../src/db/Table.h"
#include "../../src/sql/Session.h"
#include "../../src/sql/psql/PsqlEngine.h"
#include <gtest/gtest.h>


TEST(MigrationTest, StartTest) {

	using namespace sql::psql;
	PsqlEngine e("postgres", "postgres", "password");
	sql::Session s(&e);
	s.migrate();

}