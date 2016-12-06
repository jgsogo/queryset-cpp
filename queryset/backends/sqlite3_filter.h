
#pragma once

#include "../filters.h"

namespace qs {
	namespace _impl {
		template <typename Type, typename... Args>
		class SQLite3FilterContainer : public FilterContainer<Type, Args...> {
			using FilterContainer<Type, Args...>::queryset_type;
			public:
				SQLite3FilterContainer() : FilterContainer() {}
				virtual ~SQLite3FilterContainer() {}

				virtual queryset_type apply(const queryset_type& qs) const {
					throw std::runtime_error("Call not expected");
				}

		};
	}

	namespace backends {

		template <typename... Args>
		using SQLite3FilterContainer = _impl::SQLite3FilterContainer<void, Args...>;

		template <typename Type, typename... Args>
		using TypedSQLite3FilterContainer = _impl::SQLite3FilterContainer<Type, Args...>;

	}
}
