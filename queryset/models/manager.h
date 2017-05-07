
#pragma once

#include "spdlog/spdlog.h"
#include "../datasource.h"
#include "../_impl/queryset.h"

namespace qs {

    namespace _impl {
        // Get templated parameters from typed class
        // Credit: http://stackoverflow.com/questions/4691657/is-it-possible-to-store-a-template-parameter-pack-without-expanding-it
		template <typename T, 
				  template <typename T, typename... Args> class TDataSource,
				  typename... Args>
		struct expand {
			typedef TDataSource<T, Args...> DataSourceType;
		};

        template <typename T, 
				  template <typename T, typename... Args> class TDataSource,
				  typename... Args>
        struct expand<QuerySet<T, Args...>, TDataSource>
        {
			typedef typename expand<T, TDataSource, Args...>::DataSourceType DataSourceType;
        };
    }

    template <typename TModel>
    class BaseManager {
        public:
            using QuerySet = typename TModel::QuerySet;
        public:
            BaseManager() {};
            virtual ~BaseManager() {};

            virtual QuerySet all() const = 0;
			virtual void insert(const TModel& item) = 0;
    };

	template <typename TModel,
		template <typename T, typename... ArgsDataSource> class TDataSource,
		typename... Args>
	class Manager : public BaseManager<TModel> {
		using Base = BaseManager<TModel>;
		public:
			using DataSourceType = typename _impl::expand<typename Base::QuerySet, TDataSource>::DataSourceType;
		public:
			Manager(Args... args) : _datasource(args...) {};
			virtual ~Manager() {};

			virtual typename Base::QuerySet all() const {
				//SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, TModel>[{}]::all()", (void*)this);
				return typename Base::QuerySet(_datasource);
			}
			
			virtual void insert(const TModel& item) {
				_datasource.insert(item);
			}
		protected:
			DataSourceType _datasource;
	};

	template <typename TModel,
			  template <typename T, typename... ArgsDataSource> class TDataSource>
	class Manager<TModel, TDataSource, void> : public BaseManager<TModel> {
		using Base = BaseManager<TModel>;
		public:
			using DataSourceType = typename _impl::expand<typename Base::QuerySet, TDataSource>::DataSourceType;
		public:
			Manager() {};
			virtual ~Manager() {};

			virtual typename Base::QuerySet all() const {
				//SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, TModel>[{}]::all()", (void*)this);
				return typename Base::QuerySet(_datasource);
			}

			virtual void insert(const TModel& item) {
				_datasource.insert(item);
			}

		protected:
			DataSourceType _datasource;
	};


    namespace memory {
        template <typename TModel>
        using Manager = qs::Manager<TModel, _impl::MemoryDataSource, void>;
    }
}
