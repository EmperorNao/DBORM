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

			std::cout << "Calling test constructor" << std::endl;
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
			std::cout << "Calling free to avoid memory leak" << std::endl;

		}

		~TestResult() {

			std::cout << "Calling destructor to avoid memory leak" << std::endl;

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
		void begin() {};
		void end() {};
		void commit() {};
		void rollback() {};

	};

}