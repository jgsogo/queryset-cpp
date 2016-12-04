
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../print_helper.hpp"
#include "../../queryset/backends/sqlite3.h"
#include "../config_tests.h"

namespace fs = boost::filesystem;

typedef qs::backends::Sqlite3Queryset<int, std::string, float> myQuerySet;
typedef myQuerySet::qs_type::value_type mytuple;


struct Fixture {
	Fixture() {
		database = fs::temp_directory_path() / fs::unique_path();
		/*
		int rc = sqlite3_open(database.string().c_str(), &db);
		std::string sql = "CREATE TABLE " + table_name + "("  \
			"ID		INT		PRIMARY KEY NOT NULL," \
			"NAME	TEXT	NOT NULL," \
			"SALARY	REAL);";

		char *zErrMsg = 0;
		rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stderr, "Opened database successfully\n");
		}

		// Create SQL statement
		sql = "INSERT INTO " + table_name + " (ID,NAME,SALARY) "  \
			"VALUES (1, 'Paul', 20000.00 ); " \
			"INSERT INTO " + table_name + " (ID,NAME,SALARY) "  \
			"VALUES (2, 'Allen', 15000.00 ); "     \
			"INSERT INTO " + table_name + " (ID,NAME,SALARY)" \
			"VALUES (3, 'Teddy', 20000.00 );" \
			"INSERT INTO " + table_name + " (ID,NAME,SALARY)" \
			"VALUES (4, 'Mark', 65000.00 );";

		// Execute SQL statement
		rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stdout, "Records created successfully\n");
		}
		*/
	}

	~Fixture() {
		//sqlite3_close(db);
		//boost::filesystem::remove(database);
	}

	//sqlite3 *db;
	const std::string table_name = "COMPANY";
	fs::path database;
};

BOOST_FIXTURE_TEST_SUITE(backends_sqlite3, Fixture)

BOOST_AUTO_TEST_CASE(basic)
{
	/*
	myQuerySet backend(db, table_name);

	qs::FilterContainer<int, std::string, float> filters;
	auto qs = backend.apply(filters);

	BOOST_CHECK_EQUAL(qs.size(), 3);
	*/
}

BOOST_AUTO_TEST_SUITE_END()
