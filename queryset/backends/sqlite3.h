
#pragma once

#include <numeric>
#include <sqlite3cc.h>
#include "../datasource.h"

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

		template <typename Type, typename... Args>
		class Sqlite3Queryset : public _impl::ImplDataSource<Type, Args...> {
			public:
				Sqlite3Queryset(sqlite::connection& conn, const std::string& table_name) :
					_connection(conn),
					_table_name(table_name) {
					// Get column names
					sqlite::query q1(_connection, "SELECT * FROM " + _table_name + " LIMIT 0");
					for (auto i = 0; i < _column_names.size(); ++i) {
						_column_names[i] = q1.column_name(i);
					}
				}
				Sqlite3Queryset(const Sqlite3Queryset& other) : 
					_connection(other._connection), 
					_table_name(other._table_name), 
					_column_names(other._column_names),
					_impl::ImplDataSource<Type, Args...>(other) {}
				virtual ~Sqlite3Queryset() {}

				virtual qs_type apply(const _impl::FilterContainer<Type, Args...>& filters) const {
					qs_type ret;
					// Build query to database
					std::ostringstream sql; sql << "SELECT * FROM " << _table_name;
					auto filters_apply = filters.get_value_filters_apply();
					if (filters_apply.any()) {
						sql << " WHERE";
						const auto& value_filters = filters.get_value_filters();
						::utils::tuple::enumerate(value_filters, [this, &sql](const auto& i, const auto& values) {
							if (values.size() == 1) {
								sql << " " << _column_names[i] << " LIKE \"" << *values.begin() << "\"";
							}
							else if (values.size() > 1) {
								sql << " " << _column_names[i] << " IN (";
								sql << *values.begin();
								std::for_each(std::next(values.begin()), values.end(), [&sql](const auto& item) { sql << ", " << item; });
								sql << ")";
							}
						});
					}

					sqlite::query q1(_connection, sql.str());
					for (sqlite::query::iterator it = q1.begin(); it != q1.end(); it++) {
						auto item = unpack<Args...>(*it);
						ret.push_back(item);
					}
					return ret;
				}

			protected:
				sqlite::connection& _connection;
				const std::string _table_name;
				std::array<std::string, sizeof...(Args)> _column_names;
		};

	}

	namespace backends {
		template <typename... Args>
		using Sqlite3Queryset = _impl::Sqlite3Queryset<void, Args...>;

		template <typename Type, typename... Args>
		using TypedSqlite3Queryset = _impl::Sqlite3Queryset<Type, Args...>;
	}
}