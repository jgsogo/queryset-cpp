
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../print_helper.hpp"
#include "../../queryset/backends/sqlite3.h"
#include "../../queryset/queryset.h"

namespace fs = boost::filesystem;

typedef qs::backends::Sqlite3DataSource<int, std::string, float> myDataSource;
typedef myDataSource::qs_type::value_type mytuple;


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

BOOST_FIXTURE_TEST_SUITE(backends_sqlite3_groupby, Fixture)

BOOST_AUTO_TEST_CASE(group_by_single)
{
    // TODO: La ejecuci�n del group_by se puede delegar hasta la base de datos (en teor�a ser� m�s r�pida que la que se hace en C++)
    qs::backends::Sqlite3DataSource<int, std::string, float> sqlite3(connection, "people");
    QuerySet<int, std::string, float> qs(sqlite3);
    auto f4 = qs.groupBy<std::string>();
    BOOST_CHECK_EQUAL(f4.count(), 3);

    BOOST_CHECK_EQUAL(f4.at("jgsogo").count(), 2);
    BOOST_CHECK_EQUAL(f4.at("conan").count(), 1);
    BOOST_CHECK_EQUAL(f4.at("lasote").count(), 1);
}

BOOST_AUTO_TEST_SUITE_END()