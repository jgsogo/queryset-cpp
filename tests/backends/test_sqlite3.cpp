
#include <boost/test/unit_test.hpp>

#include "../print_helper.hpp"
#include "../../queryset/backends/sqlite3.h"
#include "../config_tests.h"

typedef qs::backends::Sqlite3DataSource<int, std::string, float> myDataSource;
typedef myDataSource::qs_type::value_type mytuple;


struct Fixture {

    Fixture() : connection(database.string()) {
        connection.busy_timeout(1000);
		// Create table
		sqlite::command c1(connection, "CREATE TABLE people ( id INT PRIMARY KEY, name TEXT, height REAL )");
		c1.step();
    }
    ~Fixture() {
        connection.close();
		boost::filesystem::remove(database);
    }
	const fs::path database = fs::temp_directory_path() / fs::unique_path();
	sqlite::connection connection;
};


BOOST_FIXTURE_TEST_SUITE(backend_sqlite3, Fixture)

BOOST_AUTO_TEST_CASE(invalid_table)
{
    BOOST_CHECK_THROW(myDataSource backend(connection, "invalid-table"), sqlite::sqlite_error);
}

BOOST_AUTO_TEST_SUITE_END()
