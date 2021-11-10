#pragma once
#include "../User.h"
#include "../../src/db/Table.h"
#include "../../src/sql/Session.h"
#include "../../src/sql/QueryBuilder.h"
#include <gtest/gtest.h>


TEST(QueryBuilderTest, Select) {

	sql::QueryBuilder qb;
	sql::Engine e;
	sql::Session* s = sql::create_session(&e);
	std::string res = qb.build(s->select(TBL(TestU), COLUMNS(id, name))->get_cur_query());
	std::cout << res << std::endl;

}


TEST(QueryBuilderTest, Join) {

	sql::QueryBuilder qb;
	sql::Engine e;
	sql::Session* s = sql::create_session(&e);
	std::string res = qb.build(
		s->
		select(TBL(TestU), COLUMNS(name))->
		join(TBL(TestU), TBL(TestUVisit), COLUMNS(user_id))->
		get_cur_query());
	std::cout << res << std::endl;

}


TEST(QueryBuilderTest, WhereSelect) {

	sql::QueryBuilder qb;
	sql::Engine e;
	sql::Session* s = sql::create_session(&e);
	TestU u;
	std::string res = qb.build(
		s->
		select(TBL(TestU), COLUMNS(name))->
		where(STATEMENT(TestU, id, <, 35))->
		get_cur_query()
	);
	std::cout << res << std::endl;

}


TEST(QueryBuilderTest, WhereJoin) {

	sql::QueryBuilder qb;
	sql::Engine e;
	sql::Session* s = sql::create_session(&e);
	TestU u;
	std::string res = qb.build(
		s->
		select(TBL(TestU), COLUMNS(name))->
		join(TBL(TestU), TBL(TestUVisit), COLUMNS(user_id))->
		where(STATEMENT(TestU, id, < , 35))->
		get_cur_query());
	std::cout << res << std::endl;

}