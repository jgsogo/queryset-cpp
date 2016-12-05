
#pragma once

#include <sqlite3cc.h>
#include "../datasource.h"

namespace qs {
	namespace _impl {

		template <typename Type, typename... Args>
		class Sqlite3Queryset : public _impl::ImplDataSource<Type, Args...> {
			public:
				Sqlite3Queryset(sqlite::connection& conn, const std::string& table_name) : _connection(conn), _table_name(table_name){}
				Sqlite3Queryset(const Sqlite3Queryset& other) : _connection(other._connection), _table_name(other._table_name), _impl::ImplDataSource<Type, Args...>(other) {}
				virtual ~Sqlite3Queryset() {}

				virtual qs_type apply(const _impl::FilterContainer<Type, Args...>& filters) const {
					qs_type ret;

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