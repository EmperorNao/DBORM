#include "Column.h"

namespace db {

	//Column Table::operator[](std::string name) { return *this->container[name];  };

	namespace datatypes {


		bool is_columntype_value_correct(ColumnType v) {

			switch (v)
			{
			case ColumnType::INTEGER:
				return true;
			case ColumnType::STRING:
				return true;
			case ColumnType::FLOAT:
				return true;
			default:
				return false;
			}

		}

		Column* create_column(ColumnDescription desc) {

			if (is_columntype_value_correct(desc.type)) {
				return new Column(desc);
			}
			throw new ColumnTypeError("Not existing columns type was provided while trying create column");

		}

		double deserialize_d(std::string v) {

			return std::stod(v);

		}

		int deserialize_i(std::string v) {

			return std::stoi(v);

		}

	} // end datatypes

	std::vector<std::string> split(std::string s, std::set<char> delimeters) {

		std::vector<std::string> container;
		std::string cur = "";
		int i = 0;
		while (i < s.size()) {

			if (delimeters.find(s[i]) != delimeters.end()) {

				if (cur.size()) {

					container.push_back(cur);

				}
				cur = "";

			}
			else {

				cur += s[i];

			}
			++i;

		}
		if (cur.size()) {

			container.push_back(cur);

		}
		return container;

	}

	//std::string aggregation_str(AggregationType a) {
	//	switch (a)
	//	{
	//	case db::MAX:
	//		return "MAX";
	//	case db::MIN:
	//		return "MIN";
	//	case db::SUM:
	//		return "SUM";
	//	case db::AVG:
	//		return "AVG";
	//	default:
	//		throw new AggregationError("Wrong aggregation type");
	//	}
	//}

} // end db
