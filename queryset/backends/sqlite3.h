
#pragma once

#include <numeric>
#include <sqlite3cc.h>
#include "../datasource.h"
#include "../utils/join.h"
#include "../utils/to_string.h"
#include "../models/manager.h"

namespace qs {
	namespace _impl {

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
		inline void joiner::operator()(const std::size_t& i, const T& it) {
			// TODO: May use SFINAE instead of this if
			if (std::integral_constant<bool, utils::HasToStringMethod<T>::Has>()) {
				this->operator()(i, utils::to_string(it)); // Delegate so bundles are placed around strings.
			}
			else {
				_os << (i != 0 ? _sep : "") << it;
			}
		}

        template <typename T> inline std::string equal_clause(const T& value) {
			// TODO: May use SFINAE instead of this if
			if (std::integral_constant<bool, utils::HasToStringMethod<T>::Has>()) {
				return equal_clause(utils::to_string(value)); // Delegate so bundles are placed around strings.
			}
			else {
				return " = " + utils::to_string(value);
			}
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
        class Sqlite3DataSource : public _impl::ImplDataSource<Type, Args...> {
            public:
                using qs_type = typename _impl::ImplDataSource<Type, Args...>::qs_type;
            public:
                Sqlite3DataSource(sqlite::connection& conn, const std::string& table_name) : 
					_connection(conn),
					_table_name(table_name) {
					// Get column names                                                        
					sqlite::query q1(_connection, "SELECT * FROM " + _table_name + " LIMIT 0");
					for (auto i = 0; i < _column_names.size(); ++i) {
						_column_names[i] = q1.column_name(i);
					}
				}
                Sqlite3DataSource(const Sqlite3DataSource& other) :
					_connection(other._connection), 
					_table_name(other._table_name),
					_column_names(other._column_names),
					_impl::ImplDataSource<Type, Args...>(other) {}
				virtual ~Sqlite3DataSource() {}

				virtual qs_type apply(const _impl::FilterContainer<Type, Args...>& filters) const {
					qs_type ret;
					// Build query to database
					std::ostringstream sql; sql << "SELECT * FROM " << _table_name;
                    where_clause(sql, filters);

					sqlite::query q1(_connection, sql.str());
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
                    _connection.make_command(sql.str())->exec();
                };

                void insert(const std::tuple<Args...>& item) {
                    std::ostringstream sql; sql << "INSERT INTO " + _table_name + " VALUES(";
                    joiner strfy(sql, ", ");
                    ::utils::tuple::enumerate(item, strfy);
                    sql << ")";
                    _connection.make_command(sql.str())->exec();
                }

            protected:
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
				sqlite::connection& _connection;
				const std::string _table_name;
				std::array<std::string, sizeof...(Args)> _column_names;
		};

	}

	namespace sqlite3 {
		//template <typename... Args>
		//using Sqlite3DataSource = _impl::Sqlite3DataSource<void, Args...>;

		template <typename Type, typename... Args>
		using DataSource = _impl::Sqlite3DataSource<Type, Args...>;

		template <std::size_t N>
		struct joiner_pk {
			joiner_pk(std::ostringstream& os, const std::string& sep, const std::array<std::string, N>& column_names) : _os(os), _sep(sep), _column_names(column_names) {};
			std::ostringstream& _os;
			const std::string _sep;
			const std::array<std::string, N>& _column_names;

			void _on_type(const std::size_t& i, const std::string& column_type) {
				if (i != 0) { _os << _sep; }
				_os << _column_names[i] << " " << column_type;
				if (i == 0) { _os << " PRIMARY KEY"; }
			}

			template <class T> inline void operator()(const std::size_t& i, const T&) { _on_type(i, "TEXT"); } // All is text except some fixed types.
			template <> inline void operator()<int>(const std::size_t& i, const int&) { _on_type(i, "INT"); } // TODO: SFINAE for integral type
			template <> inline void operator()<std::size_t>(const std::size_t& i, const std::size_t&) { _on_type(i, "INT"); }
			template <> inline void operator()<float>(const std::size_t& i, const float&) { _on_type(i, "REAL"); } // TODO: SFINAE for real number typ
			template <> inline void operator()<double>(const std::size_t& i, const double&) { _on_type(i, "REAL"); }
		};

		template <typename TModel>
		class Manager : public qs::Manager<TModel, _impl::Sqlite3DataSource, sqlite::connection&, const std::string&> {
			using BaseManager = typename qs::Manager<TModel, _impl::Sqlite3DataSource, sqlite::connection&, const std::string&>;
			using DataSourceType = typename BaseManager::DataSourceType;
			using TupleType = typename DataSourceType::tuple_type;
			using ArrayType = typename std::array<std::string, std::tuple_size<TupleType>::value>;
			public:
				Manager() : BaseManager(get_connection(), _table_name) {}

				static std::string create_table_sql() {
					std::ostringstream sql; sql << "CREATE TABLE " + _table_name + " (";
					joiner_pk<std::tuple_size<TupleType>::value> strfy(sql, ", ", _column_names);
					::utils::tuple::enumerate(TupleType(), strfy);
					sql << ")";
					return sql.str();
				}

				static void create_table(sqlite::connection& conn) {
					auto sql = create_table_sql();
					conn.make_command(sql)->exec();
				}

				static sqlite::connection& get_connection();

				static const std::string& table_name() { return _table_name; };
				static const ArrayType& column_names() { return _column_names; };
			protected:
				static const std::string _table_name;
				static const ArrayType _column_names;
		};

	}

}
