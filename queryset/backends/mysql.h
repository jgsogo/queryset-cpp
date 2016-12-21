
#pragma once

#include <numeric>
#include <mysql_devapi.h>
#include "../datasource.h"
#include "../utils/join.h"

namespace qs {
	namespace _impl {

        /*
		// Unpack row to tuple
		template <typename T>
		std::tuple<T> unpack(sqlite::row& row) {
			T item; row >> item;
			return std::tuple<T>{item};
		}

		template<typename T, typename... Ts, typename = typename std::enable_if<(sizeof...(Ts) > 0), bool>::type>
		std::tuple<T, Ts...> unpack(sqlite::row& row) {
			auto t1 = unpack<T>(row);
			auto t2 = unpack<Ts...>(row);
			return std::tuple_cat(t1, t2);
		}
        */
        struct joiner {
            joiner(std::ostringstream& os, const std::string& sep) : _os(os), _sep(sep) {};
            template <class T>
            inline void operator()(const std::size_t& i, const T& it);
            std::ostringstream& _os;
            const std::string _sep;
        };
        template <>
        inline void joiner::operator()<std::string>(const std::size_t& i, const std::string& it) { _os << (i != 0 ? _sep : "") << "\"" + it + "\""; }
        template <class T>
        inline void joiner::operator()(const std::size_t& i, const T& it) { _os << (i != 0 ? _sep : "") << it; }

        template <typename T> inline std::string equal_clause(const T& value) {
            return " = " + std::to_string(value);
        }
        template <typename T> inline std::string equal_clause(const std::set<T>& values) {
            std::string sql = " IN (" + utils::join(values, ", ") + ")";
            return sql;
        }
        template <> inline std::string equal_clause<std::string>(const std::string& value) {
            return " LIKE \"" + value + "\"";
        }
        template <> inline std::string equal_clause<std::string>(const std::set<std::string>& values) {
            return " REGEXP \"(" + utils::join(values, "|") + ")\"";
        }

        template <typename Type, typename... Args>
        class MySQLDataSource : public _impl::ImplDataSource<Type, Args...> {
            public:
                using qs_type = typename _impl::ImplDataSource<Type, Args...>::qs_type;
            public:
                MySQLDataSource(sql::Connection& conn, const std::string& table_name) :
					_connection(conn),
					_table_name(table_name) {
					// Get column names
					std::shared_ptr<sql::ResultSet> res = this->execute_query("SELECT * FROM " + _table_name + " LIMIT 0");
					for (auto i = 0; i < _column_names.size(); ++i) {
						_column_names[i] = q1.column_name(i);
					}
				}
                MySQLDataSource(const Sqlite3DataSource& other) :
					_connection(other._connection),
					_table_name(other._table_name),
					_column_names(other._column_names),
					_impl::ImplDataSource<Type, Args...>(other) {}
				virtual ~MySQLDataSource() {}

				virtual qs_type apply(const _impl::FilterContainer<Type, Args...>& filters) const {
					qs_type ret;
					// Build query to database
					std::ostringstream sql; sql << "SELECT * FROM " << _table_name;
                    where_clause(sql, filters);

                    std::shared_ptr<sql::ResultSet> res = this->execute_query(sql);
					for (sqlite::query::iterator it = q1.begin(); it != q1.end(); it++) {
                                                sqlite::row r = *it; // TODO: Remove this copy
						auto item = unpack<Args...>(r);
						ret.push_back(item);
					}
					return ret;
				}

                virtual void remove(const FilterContainer<Type, Args...>& filters) const {
                    std::ostringstream sql; sql << "DELETE FROM " << _table_name;
                    where_clause(sql, filters);
                    std::shared_ptr<sql::ResultSet> res = this->execute_query(sql);
                    _connection.make_command(sql.str())->exec();
                };

                void insert(const std::tuple<Args...>& item) {
                    std::ostringstream sql; sql << "INSERT INTO " + _table_name + " VALUES(";
                    joiner strfy(sql, ", ");
                    ::utils::tuple::enumerate(item, strfy);
                    sql << ")";
                    std::shared_ptr<sql::ResultSet> res = this->execute_query(sql);

                    _connection.make_command(sql.str())->exec();
                }

            protected:
                std::shared_ptr<sql::ResultSet> execute_query(const std::string& sql) {
                    sql::Statement *stmt = _connection.createStatement();
                    std::shared_ptr<sql::ResultSet> ret(stmt->executeQuery(sql));
                    delete stmt;
                    return ret;
                }

                std::ostringstream& where_clause(std::ostringstream& sql, const FilterContainer<Type, Args...>& filters) const {
                    auto filters_apply = filters.get_value_filters_apply();
                    if (filters_apply.any()) {
                        sql << " WHERE ";
                        std::vector<std::string> clauses;
                        const auto& value_filters = filters.get_value_filters();
                        ::utils::tuple::enumerate(value_filters, [this, &clauses](const auto& i, const auto& values) {
                            if (values.size() == 1) {
                                clauses.push_back(_column_names[i] + equal_clause(*values.begin()));
                            }
                            else if (values.size() > 1) {
                                clauses.push_back(_column_names[i] + equal_clause(values));
                            }
                        });
                        sql << utils::join(clauses, " AND ");
                    }
                    return sql;
                }

			protected:
				sql::Connection& _connection;
				const std::string _table_name;
				std::array<std::string, sizeof...(Args)> _column_names;
		};

	}

	namespace backends {
		template <typename... Args>
		using MySQLDataSource = _impl::MySQLDataSource<void, Args...>;

		template <typename Type, typename... Args>
		using TypedMySQLDataSource = _impl::MySQLDataSource<Type, Args...>;
	}
}
