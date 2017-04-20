
#pragma once

#include <sqlite3cc.h>
#include "../datasource.h"
#include "sqlite3_filter.h"

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
		        using qs_type = typename _impl::ImplDataSource<Type, Args...>::qs_type;
			public:
				Sqlite3Queryset(sqlite::connection& conn, const std::string& table_name) : _connection(conn), _table_name(table_name){}
				Sqlite3Queryset(const Sqlite3Queryset& other) : _connection(other._connection), _table_name(other._table_name), _impl::ImplDataSource<Type, Args...>(other) {}
				virtual ~Sqlite3Queryset() {}

				virtual _impl::FilterContainer<Type, Args...> create_filter() const {
					return _impl::SQLite3FilterContainer<Type, Args...>();
				}

				virtual qs_type apply(const _impl::FilterContainer<Type, Args...>& filters) const {
					qs_type ret;

					const _impl::SQLite3FilterContainer<Type, Args...>& filter = static_cast<const _impl::SQLite3FilterContainer<Type, Args...>&>(filters);
					sqlite::query q1(_connection, "SELECT * FROM " + _table_name);
					for (sqlite::query::iterator it = q1.begin(); it != q1.end(); it++) {
					    sqlite::row row = *it;
						auto item = unpack<Args...>(row);
						ret.push_back(item);
					}
					return ret;
				}

			protected:
				sqlite::connection& _connection;
				const std::string _table_name;
		};

	}

	namespace backends {
		template <typename... Args>
		using Sqlite3Queryset = _impl::Sqlite3Queryset<void, Args...>;

		template <typename Type, typename... Args>
		using TypedSqlite3Queryset = _impl::Sqlite3Queryset<Type, Args...>;
	}
}