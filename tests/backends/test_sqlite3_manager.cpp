
#include <boost/test/unit_test.hpp>

#include "../print_helper.hpp"
#include "../../queryset/models/model.h"
#include "../../queryset/backends/sqlite3.h"
#include "../config_tests.h"

const std::string table_name_ = "table_name";
const std::array<std::string, 2> column_names_2_ = { "int", "string" };
const std::array<std::string, 3> column_names_3_ = { "int", "string1", "string2" };

/* Test with querysets model */
typedef qs::Model<qs::sqlite3::Manager, int, std::string> myModel;
typedef qs::sqlite3::Manager<myModel> myManager;
const std::string myManager::_table_name = table_name_;
const std::array<std::string, 2> myManager::_column_names = column_names_2_;

BOOST_AUTO_TEST_SUITE(sqlite3_manager)

BOOST_AUTO_TEST_CASE(static_members)
{
	BOOST_CHECK_EQUAL(myManager::table_name(), table_name_);
	BOOST_CHECK_EQUAL_COLLECTIONS(myManager::column_names().begin(), myManager::column_names().end(), column_names_2_.begin(), column_names_2_.end());
}

BOOST_AUTO_TEST_CASE(create_table)
{
	std::ostringstream os;
	os << "CREATE TABLE " << table_name_ << " (";
	os << column_names_2_[0] << " INT PRIMARY KEY, ";
	os << column_names_2_[1] << " TEXT";
	os << ")";
	auto sql = myManager::create_table_sql();
	BOOST_CHECK_EQUAL(sql, os.str());
}

BOOST_AUTO_TEST_SUITE_END()


/* Custom model */
class CustomModel : public qs::BaseModel<CustomModel, qs::sqlite3::Manager, int, std::string, std::string> {
public:
	CustomModel() {};
	CustomModel(const BaseModel::tuple& data) : qs::BaseModel<CustomModel, qs::sqlite3::Manager, int, std::string, std::string>(data) {};
	virtual ~CustomModel() {};
};
typedef qs::sqlite3::Manager<CustomModel> CustomModelManager;
const std::string CustomModelManager::_table_name = table_name_;
const std::array<std::string, 3> CustomModelManager::_column_names = column_names_3_;

BOOST_AUTO_TEST_SUITE(sqlite3_manager_custom_model)

BOOST_AUTO_TEST_CASE(static_members)
{
	BOOST_CHECK_EQUAL(CustomModelManager::table_name(), table_name_);
	BOOST_CHECK_EQUAL_COLLECTIONS(CustomModelManager::column_names().begin(), CustomModelManager::column_names().end(), column_names_3_.begin(), column_names_3_.end());
}

BOOST_AUTO_TEST_CASE(create_table)
{
	std::ostringstream os;
	os << "CREATE TABLE " << table_name_ << " (";
	os << column_names_3_[0] << " INT PRIMARY KEY, ";
	os << column_names_3_[1] << " TEXT, ";
	os << column_names_3_[2] << " TEXT";
	os << ")";
	auto sql = CustomModelManager::create_table_sql();
	BOOST_CHECK_EQUAL(sql, os.str());
}

BOOST_AUTO_TEST_SUITE_END()

