
#include <boost/test/unit_test.hpp>

#include "../print_helper.hpp"
#include "../../queryset/backends/mysql.h"

typedef qs::backends::MySQLDataSource<int, std::string, float> myDataSource;
typedef myDataSource::qs_type::value_type mytuple;


struct Fixture {

    Fixture() {
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        connection = driver->connect("tcp://127.0.0.1:3306", "user", "password");

        sql::Statement *stmt = con->createStatement();
        stmt->execute("CREATE TABLE people ( id INT PRIMARY KEY, name TEXT, height REAL )");
        delete stmt;
    }
    ~Fixture() {
        delete connection;
    }

	sql::Connection *connection;
};


BOOST_FIXTURE_TEST_SUITE(backend_mysql, Fixture)

BOOST_AUTO_TEST_CASE(invalid_table)
{
    //BOOST_CHECK_THROW(myDataSource backend(*connection, "invalid-table"), sqlite::sqlite_error);
}

BOOST_AUTO_TEST_SUITE_END()
