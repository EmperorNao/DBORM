#pragma once
#include "../db/Table.h"
#include "Query.h"


namespace sql {


	class Engine {


	};


	class Session {

	private:


	public:
		Session(Engine* e) {};
		void begin() {};
		void end() {};
		void rollback() {};
		void commit() {};

		/*void insert();
		void delete_out();
*/

		void select();
		void join();
		void where();
		void execute();


	};





}

