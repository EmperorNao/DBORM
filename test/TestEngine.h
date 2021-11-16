#pragma once
#include "../src/sql/Session.h"
#include "../src/sql/Engine.h"

namespace test {

	class TestResult : public sql::Result {

	private:
		typedef long long ll;
		bool was_free = false;

	public:
		TestResult() {

			std::cout << "Calling test constructor\n\n";
			was_free = false;

		}

		std::string get_value(ll row, ll col) {

			return "";

		};

		std::string get_value(ll row, std::string col) {

			return "";

		}

		void out() { };

		void free() {

			if (was_free) {

				throw new sql::ResultError("Calling free to already freed object");

			}
			was_free = true;
			std::cout << "Calling free to avoid memory leak\n\n";

		}

		~TestResult() {

			std::cout << "Calling destructor to avoid memory leak\n\n";

		};

	};


	class TestEngine: public sql::Engine {

	public:
		TestEngine(): Engine("") {};
		sql::Result* execute(sql::Query* q) {

			sql::QueryBuilder builder;
			std::string query = builder.build(q);
			return new TestResult();

		}
		sql::Result* execute(std::string q) {

			return nullptr;

		}
		void begin() {};
		void end() {};
		void commit() {};
		void rollback() {};
		void migrate(std::string, sql::MigrationFormat) {};

	};

}