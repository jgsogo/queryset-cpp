
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../print_helper.hpp"
#include "../../queryset/backends/sqlite3.h"
#include "../config_tests.h"

namespace fs = boost::filesystem;

typedef qs::backends::Sqlite3Queryset<int, std::string, float> myQuerySet;
typedef myQuerySet::qs_type::value_type mytuple;


struct Fixture {
	const fs::path database = fs::temp_directory_path() / fs::unique_path();
	sqlite::connection connection;

	Fixture() : connection(database.string()) {
		connection.busy_timeout(1000);

		// Create table
		sqlite::command c1(connection, "CREATE TABLE people ( id INT PRIMARY KEY, name TEXT, height REAL )");
		c1.step();

		// or, for brevity
		*connection.make_command("INSERT INTO people VALUES( ?, ?, ? )") << 1 << "jgsogo" << 1.0 << sqlite::exec;
		*connection.make_command("INSERT INTO people VALUES( ?, ?, ? )") << 2 << "conan" << 0.9 << sqlite::exec;
		*connection.make_command("INSERT INTO people VALUES( ?, ?, ? )") << 3 << "lasote" << 1.0 << sqlite::exec;
		*connection.make_command("INSERT INTO people VALUES( ?, ?, ? )") << 4 << "jgsogo" << 0.9 << sqlite::exec;
	}

	~Fixture() {
		connection.close();
		boost::filesystem::remove(database);
	}

};

BOOST_FIXTURE_TEST_SUITE(backends_sqlite3, Fixture)

BOOST_AUTO_TEST_CASE(basic)
{
	myQuerySet backend(connection, "people");

	qs::FilterContainer<int, std::string, float> filters;
	auto qs = backend.apply(filters);

	BOOST_CHECK_EQUAL(qs.size(), 4);
}

BOOST_AUTO_TEST_SUITE_END()
