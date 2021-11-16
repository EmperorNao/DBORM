#pragma once
#include "../Session.h"
#include <libpq-fe.h>
#include <algorithm>


namespace sql {

	namespace psql {

		class PsqlResult : public Result {

		private:
			PGresult* result;

			bool is_null() {

				return result == nullptr;

			}

		public:
			PsqlResult(PGresult* res): result(res) { 
			
				if (is_null()) {

					throw new ResultError("Result was nullptr when trying to create it");

				}
				this->set_nrows(PQntuples(result));
				this->set_ncols(PQnfields(result));

				std::vector<std::string> columns(this->get_ncols());
				for (ll col = 0; col < columns.size(); ++col) {

					columns[col] = PQfname(result, col);

				}
				this->set_columns(columns);
				
			};

			inline std::string get_value(ll row, ll col) {

				if (is_null()) {

					throw new ResultError("Trying to use already free result");

				}

				if (row > get_nrows()) {

					throw new ResultError("Wrong row number " + std::to_string(row) + " was provided in get value");

				}
				if (col > get_ncols()) {

					throw new ResultError("Wrong column number " + std::to_string(col) + " was provided in get value");

				}
				return PQgetvalue(result, row, col);

			};

			std::string get_value(ll row, std::string col) {

				if (is_null()) {

					throw new ResultError("Trying to use already free result");

				}

				auto columns = get_columns();
				auto it = std::find(columns.begin(), columns.end(), col);

				if (it == columns.end()) {

					throw new ResultError("Wrong column " + col + " was provided in get value");

				}
				return get_value(row, it - columns.begin());

			}

			void free() {

				if (is_null()) {

					throw new ResultError("Trying to use already free result");

				}
				PQclear(result);
				result = nullptr;

			};

			void out() {

				auto columns = get_columns();
				for (int j = 0; j < get_ncols(); j++) {
					printf("%-15s", columns[j].c_str());

				}
				printf("\n");
				for (int i = 0; i < get_nrows(); i++) {

					for (int j = 0; j < get_ncols(); j++) {
						printf("%-15s", get_value(i, j).c_str());

					}
					printf("\n");
					
				}

			}


			~PsqlResult() {
			
				PQclear(result);

			};

		};

		class PsqlEngine : public Engine {

		private:

			PGconn* connection;
			QueryBuilder qb;

			void exit_nicely() {

				PQfinish(connection);

			}

			std::string connection_err() {

				return (std::string)PQerrorMessage(connection);

			}

		public:
			PsqlEngine(
				std::string _dbname,
				std::string _user = "",
				std::string _password = "",
				std::string _host = "",
				std::string _port = "") : Engine(_dbname, _user, _password, _host, _port)
			{
			
				std::string conninfo = "";
				conninfo += "dbname = " + _dbname;
				if (_user != "") {

					conninfo += " user = " + _user;

				}
				if (_password != "") {

					conninfo += " password = " + _password;

				}
				if (_host != "") {

					conninfo += " host = " + _host;

				}
				if (_port != "") {

					conninfo += " port = " + _port;

				}


				connection = PQconnectdb(conninfo.c_str());
				if (PQstatus(connection) != CONNECTION_OK) {
					std::string err = connection_err();
					exit_nicely();
					throw new EngineError("Error during trying to create connection: " + err);
				}
			
			}

			void begin() {

				PGresult* res = PQexec(connection, "BEGIN");
				if (PQresultStatus(res) != PGRES_COMMAND_OK) {
					std::string err = connection_err();
					PQclear(res);
					exit_nicely();
					throw new EngineError("Error during trying to begin: " + err);
				}

			}

			void end() {

				PGresult* res = PQexec(connection, "END");
				if (PQresultStatus(res) != PGRES_COMMAND_OK) {
					std::string err = connection_err();
					PQclear(res);
					exit_nicely();
					throw new EngineError("Error during trying to end: " + err);
				}

			}

			void commit() {

				PGresult* res = PQexec(connection, "COMMIT");
				if (PQresultStatus(res) != PGRES_COMMAND_OK) {
					std::string err = connection_err();
					PQclear(res);
					exit_nicely();
					throw new EngineError("Error during trying to commit: " + err);
				}

			}

			void rollback() {

				PGresult* res = PQexec(connection, "ROLLBACK");
				if (PQresultStatus(res) != PGRES_COMMAND_OK) {
					std::string err = connection_err();
					PQclear(res);
					exit_nicely();
					throw new EngineError("Error during trying to rollback: " + err);
				}

			}

			Result* execute(Query* q) {

				std::string translated_query = qb.build(q);
				PGresult* res = PQexec(connection, translated_query.c_str());
				if (PQresultStatus(res) != PGRES_TUPLES_OK) {
					std::string err = connection_err();
					PQclear(res);
					exit_nicely();
					throw new EngineError("Error during trying to execute query: " + err);
				}
				return new PsqlResult(res);

			};


			Result* execute(std::string q) {

				PGresult* res = PQexec(connection, q.c_str());
				if (PQresultStatus(res) != PGRES_TUPLES_OK) {
					std::string err = connection_err();
					PQclear(res);
					exit_nicely();
					throw new EngineError("Error during trying to execute query: " + err);
				}
				return new PsqlResult(res);

			};



		};



	} // psql end

} // sql end