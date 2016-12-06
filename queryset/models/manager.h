
#pragma once

#include "spdlog/spdlog.h"
#include "../datasource.h"
#include "../_impl/queryset.h"
#include "../backends/filesystem.h"

namespace qs {

    namespace _impl {
        // Get templated parameters from typed class
        // Credit: http://stackoverflow.com/questions/4691657/is-it-possible-to-store-a-template-parameter-pack-without-expanding-it
        template <typename T, typename... Args>
        struct expand {
            typedef FileQueryset<T, Args...> FileQuerysetType;
            typedef MemoryQueryset<T, Args...> MemoryQuerysetType;
        };

        template <typename T, typename... Args>
        struct expand<QuerySet<T, Args...>>
        {
            typedef typename expand<T, Args...>::FileQuerysetType FileQuerysetType;
            typedef typename expand<T, Args...>::MemoryQuerysetType MemoryQuerysetType;
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
    };

    template <typename TDataSource, typename TModel>
    class Manager;

    template <typename TModel>
    class Manager<std::string, TModel> : public BaseManager<TModel> {
        using Base = BaseManager<TModel>;
        public:
            Manager(const std::string& filename) : _datasource(filename) {};
            virtual ~Manager() {};

            virtual typename Base::QuerySet all() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, TModel>[{}]::all()", (void*)this);
                return typename Base::QuerySet(_datasource);
            }
        protected:
            typename _impl::expand<typename Base::QuerySet>::FileQuerysetType _datasource;
    };

    template <typename TModel>
    class Manager<void, TModel> : public BaseManager<TModel> {
        using Base = BaseManager<TModel>;
        public:
            Manager() {};
            virtual ~Manager() {};

            virtual typename Base::QuerySet all() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<void, TModel>[{}]::all()", (void*)this);
                return typename Base::QuerySet(_datasource);
            }

        protected:
            typename _impl::expand<typename Base::QuerySet>::MemoryQuerysetType _datasource;
    };

    template <typename TModel>
    using FileManager = Manager<std::string, TModel>;

    template <typename TModel>
    using MemoryManager = Manager<void, TModel>;

}
