
#include <boost/test/unit_test.hpp>

#include "../print_helper.hpp"
#include "../../queryset/models/model.h"
#include "../../queryset/backends/sqlite3.h"
#include "../config_tests.h"

const std::string table_name_ = "table_name";
const std::array<std::string, 2> column_names_ = { "int", "string" };

typedef qs::Model<qs::manager::Sqlite3Manager, int, std::string> myModel;
typedef qs::manager::Sqlite3Manager<myModel> myManager;
const std::string myManager::_table_name = table_name_;
const std::array<std::string, 2> myManager::_column_names = column_names_;

BOOST_AUTO_TEST_SUITE(sqlite3_manager)

BOOST_AUTO_TEST_CASE(static_members)
{
	BOOST_CHECK_EQUAL(myManager::table_name(), table_name_);
	BOOST_CHECK_EQUAL_COLLECTIONS(myManager::column_names().begin(), myManager::column_names().end(), column_names_.begin(), column_names_.end());
}

BOOST_AUTO_TEST_CASE(create_table)
{
	std::ostringstream os;
	os << "CREATE TABLE " << table_name_ << " (";
	os << column_names_[0] << " INT PRIMARY KEY, ";
	os << column_names_[1] << " TEXT";
	os << ")";
	auto sql = myManager::create_table_sql();
	BOOST_CHECK_EQUAL(sql, os.str());
}

BOOST_AUTO_TEST_SUITE_END()
