#pragma once
#include "../../src/db/Table.h"
#include "../../src/sql/Session.h"
#include "../../src/sql/psql/PsqlEngine.h"
#include <gtest/gtest.h>
#include <chrono>
#include "../test/TestUser.h"
#include <random>
#include <fstream>


std::chrono::system_clock::time_point now() {

	return std::chrono::system_clock::now();

}
double microseconds(std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point end) {

	return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

}
std::vector<int> generate(size_t nrows) {

	std::vector<int> v(nrows);
	for (size_t i = 0; i < nrows; ++i) {

		v[i] = rand();

	}
	return v;

}

void fill_table_orm(size_t nrows) {

	using namespace sql::psql;
	using namespace sql;
	using namespace test;
	PsqlEngine e("ormtest", "postgres", "password");
	Session s(&e);
	TestU u1;
	Result* res;

	std::vector<db::Table*> users(nrows);
	for (size_t i = 0; i < nrows; ++i) {

		users[i] = new TestU();
		SET_TO_POINTER(users[i], id, i);

	}
	s.insert(TBL(TestU), users)->execute();

	for (size_t i = 0; i < nrows; ++i) {

		free(users[i]);

	}

	res = s.select(TBL(TestU), COLUMNS(*))->execute();
	if (res->get_nrows() != nrows) {

		throw std::exception("Can not fill database");

	}

}
void clear_table_orm() {

	using namespace sql::psql;
	using namespace sql;
	using namespace test;
	PsqlEngine e("ormtest", "postgres", "password");
	Session s(&e);
	TestU u1;
	Result* res;

	res = s.del(TBL(TestU))->execute();
	if (res->get_nrows() != 0) {

		throw std::exception("Can not clear database");

	}

}
void copy_from_orm(sql::Session* s) {

	using namespace sql::psql;
	using namespace sql;
	using namespace test;
	TestU u1;
	Result* res;

	res = s->select(TBL(TestU), COLUMNS(*))->execute();
	std::vector<int> res_values(res->get_nrows());
	for (ll i = 0; i < res->get_nrows(); ++i) {

		res_values[i] = std::stoi(res->get_value(i, "id"));

	}

};


static void exit_nicely(PGconn* conn)
{
	PQfinish(conn);
	exit(1);
}
void fill_table_libpq(size_t nrows) {

	PGconn* conn;
	PGresult* res;
	conn = PQconnectdb("dbname = ormtest user = postgres password = password");

	/* Check to see that the backend connection was successfully made */
	if (PQstatus(conn) != CONNECTION_OK)
	{
		fprintf(stderr, "Connection to database failed: %s",
			PQerrorMessage(conn));
		exit_nicely(conn);
	}

	std::vector<std::string> vs(nrows);
	for (size_t i = 0; i < nrows; ++i) {

		vs[i] = std::to_string(i);

	}
	
	std::string symb = ",\'\'), (";
	std::string values = "INSERT INTO TestU(id, name) VALUES (";
	for (size_t i = 0; i + 1 < vs.size(); ++i) {

		values += vs[i] + symb;

	}
	if (values.size()) {

		values += vs[vs.size() - 1] + ",\'\'";

	}
	values += ")";

	res = PQexec(conn,
		(values).c_str());
	PQclear(res);

	res = PQexec(conn, "SELECT * from TestU");
	if (PQntuples(res) != nrows) {

		PQclear(res);
		throw std::exception("Can not fill database");

	}
	PQclear(res);

}
void clear_table_libpq() {

	PGconn* conn;
	PGresult* res;
	conn = PQconnectdb("dbname = ormtest user = postgres password = password");

	/* Check to see that the backend connection was successfully made */
	if (PQstatus(conn) != CONNECTION_OK)
	{
		fprintf(stderr, "Connection to database failed: %s",
			PQerrorMessage(conn));
		exit_nicely(conn);
	}

	res = PQexec(conn, "DELETE FROM TestU");
	PQclear(res);

	res = PQexec(conn, "SELECT * from TestU");
	if (PQntuples(res) != 0) {

		PQclear(res);
		throw std::exception("Can not fill database");

	}
	PQclear(res);

}
void copy_from_libpq(PGconn* conn) {

	PGresult* res;

	res = PQexec(conn, "SELECT * from TestU");

	std::vector<int> res_values(PQntuples(res));
	for (int i = 0; i < PQntuples(res); ++i) {

		res_values[i] = std::stoi(PQgetvalue(res, i, 0));

	}
	PQclear(res);

}


void do_benchmark(std::string filename, size_t nrows) {

	
	try {

		std::fstream file(filename, std::fstream::in | std::fstream::out | std::fstream::app);
		auto start = now();
		auto end = now();
		auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

		const int n_experiments = 10;

		sql::psql::PsqlEngine e("ormtest", "postgres", "password");
		sql::Session s(&e);

		file << nrows << std::endl;
		start = now();
		fill_table_orm(nrows);
		end = now();
		double ormtime_insert = microseconds(start, end) / n_experiments / 1000000;
		std::cout << "Average time for ORM inserting " << nrows << " nrows  is "
			<< ormtime_insert << " s." << std::endl;
		file << ormtime_insert << std::endl;


		start = now();
		for (int i = 0; i < n_experiments; ++i) {

			copy_from_orm(&s);

		}
		end = now();
		double ormtime_main = microseconds(start, end) / n_experiments / 1000000;
		std::cout << "Average time for ORM extracting " << nrows << " nrows  is " 
			<< ormtime_main << " s." << std::endl;
		file << ormtime_main << std::endl;

		clear_table_orm();


		PGconn* conn;
		conn = PQconnectdb("dbname = ormtest user = postgres password = password");

		start = now();
		fill_table_orm(nrows);
		end = now();
		double libpqtime_insert = microseconds(start, end) / n_experiments / 1000000;
		std::cout << "Average time for LIBPQ inserting " << nrows << " nrows  is "
			<< libpqtime_insert << " s." << std::endl;
		file << libpqtime_insert << std::endl;


		start = now();
		for (int i = 0; i < n_experiments; ++i) {

			copy_from_libpq(conn);

		}
		end = now();
		double libpqtime_main = microseconds(start, end) / n_experiments / 1000000;
		std::cout << "Average time for LIBPQ extracting " << nrows << " nrows  is " 
			<< libpqtime_main << " s." << std::endl;

		file << libpqtime_main << std::endl;
		clear_table_libpq();

		std::cout << "ORM is slower then LIBPQ on inserting "
			<< ((ormtime_insert - libpqtime_insert) / ormtime_insert) * 100 << "%" << std::endl;
		std::cout << "ORM is slower then LIBPQ on extracting " 
			<< ((ormtime_main - libpqtime_main) / ormtime_main) * 100 << "%" << std::endl;

	}
	catch (std::exception* e) {

		std::cout << e->what() << std::endl;

	}

}