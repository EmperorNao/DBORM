#pragma once
#include "../src/sql/Session.h"

namespace test {

	class TestEngine: public sql::Engine {

	public:
		TestEngine() {};
		void execute(sql::Query* q) {

			// just pass

		}



	};

}