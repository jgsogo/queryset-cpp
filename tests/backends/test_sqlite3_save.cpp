
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

BOOST_FIXTURE_TEST_SUITE(backends_sqlite3_save, Fixture)

BOOST_AUTO_TEST_CASE(add_one)
{
    qs::backends::Sqlite3DataSource<int, std::string, float> sqlite3(connection, "people");
    QuerySet<int, std::string, float> qs(sqlite3);
    BOOST_CHECK_EQUAL(qs.count(), 4);
    
    // Add one item to db
    std::tuple<int, std::string, float> item{ 5, "sqlite3", 1.0 };
    sqlite3.insert(item);
    BOOST_CHECK_EQUAL(qs.count(), 4); // Query cache needs to be reseted.
    qs.reset();
    BOOST_CHECK_EQUAL(qs.count(), 5);
}

BOOST_AUTO_TEST_CASE(delete_one)
{
    qs::backends::Sqlite3DataSource<int, std::string, float> sqlite3(connection, "people");
    {
        QuerySet<int, std::string, float> qs(sqlite3);
        BOOST_CHECK_EQUAL(qs.count(), 4);
    }
    {
        QuerySet<int, std::string, float> qs(sqlite3);
        qs.filter(1);
        BOOST_CHECK_EQUAL(qs.count(), 1);
        qs.remove();
    }
    {
        QuerySet<int, std::string, float> qs(sqlite3);
        BOOST_CHECK_EQUAL(qs.count(), 3);
    }
}

BOOST_AUTO_TEST_SUITE_END()