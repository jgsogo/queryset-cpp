
#pragma once

#include <boost/filesystem.hpp>

#include "../../queryset/backends/filesystem.h"
#include "../../queryset/backends/sqlite3.h"
#include "../../queryset/datasource.h"
#include "../config_tests.h"

typedef std::tuple<int, std::string, float> MyModelTuple;

typedef qs::MemoryDataSource<MyModelTuple, int, std::string, float> MemoryDataSource;
typedef qs::filesystem::DataSource<MyModelTuple, int, std::string, float> FileDataSource;
typedef qs::sqlite3::DataSource<MyModelTuple, int, std::string, float> SQLite3DataSource;

template <typename T>
struct FixtureDataSource;


template <>
struct FixtureDataSource<MemoryDataSource> {
    FixtureDataSource() {
        this->populate();
    };
    void populate() {
        typedef MemoryDataSource::qs_type::value_type mytuple;
        datasource >> MyModelTuple{1, "jgsogo", 1.f};
        datasource >> MyModelTuple{2, "conan", 0.9f};
        datasource >> MyModelTuple{3, "lasote", 1.f};
        datasource >> MyModelTuple{4, "jgsogo", 0.9f};
    }

    MemoryDataSource datasource;
};


template<>
struct FixtureDataSource<FileDataSource> {
    FixtureDataSource() : datasource(file_data.string()) {
    }

	const fs::path file_data = test_data_dir / fs::path("fixture_datasource.tsv");
	FileDataSource datasource;
};


template <>
struct FixtureDataSource<SQLite3DataSource> {
    static sqlite::connection& init_connection(sqlite::connection& connection) {
        connection.busy_timeout(1000);
		// Create table
		sqlite::command c1(connection, "CREATE TABLE people ( id INT PRIMARY KEY, name TEXT, height REAL )");
		c1.step();
        return connection;
    }

    FixtureDataSource() : connection(database.string()), datasource(init_connection(connection), "people") {
		this->populate();
    }
    ~FixtureDataSource() {
        connection.close();
		boost::filesystem::remove(database);
    }
    void populate() {
        // or, for brevity
		*connection.make_command("INSERT INTO people VALUES( ?, ?, ? )") << int(1) << "jgsogo" << 1.0 << sqlite::exec;
		*connection.make_command("INSERT INTO people VALUES( ?, ?, ? )") << int(2) << "conan" << 0.9 << sqlite::exec;
		*connection.make_command("INSERT INTO people VALUES( ?, ?, ? )") << int(3) << "lasote" << 1.0 << sqlite::exec;
		*connection.make_command("INSERT INTO people VALUES( ?, ?, ? )") << int(4) << "jgsogo" << 0.9 << sqlite::exec;
    }

	const fs::path database = fs::temp_directory_path() / fs::unique_path();
	sqlite::connection connection;
	SQLite3DataSource datasource;
};
