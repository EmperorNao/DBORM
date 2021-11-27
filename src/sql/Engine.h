#pragma once
#include "Query.h"


namespace sql {


	enum MigrationFormat {
		DBORM, CSV, JSON
	};

	enum IsolationLevel {
		ReadUncommitted, ReadCommitted, RepeatableRead, SERIALIZABLE
	};


	class MigrationError : public std::exception {

		std::string message;
	public:
		MigrationError(std::string m) : message(m) {};

		virtual const char* what() const throw()
		{
			return message.c_str();
		}

	};


	typedef long long ll;

	class ResultError : public std::exception {

		std::string message;
	public:
		ResultError(std::string m) : message(m) {};
		virtual const char* what() const throw()
		{
			return message.c_str();
		}


	};


	class EngineError : public std::exception {

		std::string message;
	public:
		EngineError(std::string m) : message(m) {};

		virtual const char* what() const throw()
		{
			return message.c_str();
		}

	};


	class Result {

	private:
		std::vector<std::string> columns;
		ll rows;
		ll cols;
	protected:
		void set_nrows(ll nrows) { rows = nrows; }
		void set_ncols(ll ncols) { cols = ncols; }
		void set_columns(std::vector<std::string> colnames) { columns = colnames; }


	public:

		Result() {};
		std::vector<std::string> get_columns() { return columns; }
		ll get_ncols() { return cols; }
		ll get_nrows() { return rows; }
		
		virtual std::string get_value(ll row, ll col) = 0;
		virtual std::string get_value(ll row, std::string col) = 0;
		virtual void free() = 0;
		virtual void out() = 0;
		~Result() {};

	};


	class Engine {

	private:
		std::string host;
		std::string port;
		std::string user;
		std::string password;
		std::string dbname;
	public:
		Engine(
			std::string _dbname, 
			std::string _user = "",
			std::string _password = "",
			std::string _host = "",
			std::string _port = "") : host(_host), user(_user), password(_password), port(_port), dbname(_dbname)
		{};
		virtual Result* execute(Query* q) = 0;
		virtual Result* execute(std::string) = 0;
		virtual void begin() = 0;
		virtual void end() = 0;
		virtual void rollback() = 0;
		virtual void commit() = 0;
		virtual void set_iso_level(IsolationLevel l) = 0;

	};

}