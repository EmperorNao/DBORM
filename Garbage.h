#pragma once


// LEGACY MACROS
/*
#define VARGS_(_10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N
#define VARGS(...) VARGS_(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)

#define MACRO_2(a, b) std::cout << a << ' ' << b;

#define MACRO_1(a) MACRO_2(a, "test") // Supply default argument

#define MACRO(...) CONCAT(MACRO_, VARGS(__VA_ARGS__))(__VA_ARGS__)

#define STRING_2(name,  pk) new String(pk)
#define STRING_1(name) STRING_2(name, false)

#define STRING(name, ...) this->meta[#name] = CONCAT(STRING_, VARGS(__VA_ARGS__))(__VA_ARGS__);
#define INT(name, ...) this->meta[#name] = CONCAT(INT_, VARGS(__VA_ARGS__))(__VA_ARGS__);
*/


/*
#define SET_TEMPLATE(session, T) session.add_info(T::table_name);

class Session {

public:
    void add_info(std::string tname) {

        std::cout << tname;

    };

};


template <typename T>
void function(Session s) {

    SET_TEMPLATE(s, T)

}


Session s;
function<User>(s);

*/
