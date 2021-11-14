#pragma once
#include "../TestUser.h"
#include "../../src/db/Table.h"
#include "../../src/sql/Session.h"
#include "../../src/sql/psql/PsqlEngine.h"
#include <gtest/gtest.h>


TEST(PsqlEngineTest, SelectTest) {

	using namespace sql::psql;
	
	PsqlEngine e("postgres", "postgres", "password");
	sql::Session s(&e);
	try {
		PsqlResult* res = (PsqlResult*)s.select("posgtres", {}, COLUMNS(*))->execute();
		res->out();
	}
	catch (std::exception* e) {
		
		std::cout << "Exception goes\n" << e->what();

	}

}