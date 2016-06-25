
#pragma once

#include "spdlog/spdlog.h"
#include "../datasource.h"
#include "queryset_typed.h"

namespace qs {

    namespace _detail {
        // Get templated parameters from typed class
        // Credit: http://stackoverflow.com/questions/4691657/is-it-possible-to-store-a-template-parameter-pack-without-expanding-it
        template <typename T, typename... Args>
        struct expand {
            typedef utils::FileQueryset<T, Args...> FileQueryset;
            typedef utils::MemoryQueryset<T, Args...> MemoryQueryset;
        };

        template <typename T, typename... Args>
        struct expand<QuerySet<T, Args...>>
        {
            typedef typename expand<T, Args...>::FileQueryset FileQueryset;
            typedef typename expand<T, Args...>::MemoryQueryset MemoryQueryset;
        };

        template <typename Type, typename... Args>
        struct expand<TypedQuerySet<Type, Args...>>
        {
            typedef typename expand<Args...>::FileQueryset FileQueryset;
            typedef typename expand<Args...>::MemoryQueryset MemoryQueryset;
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
            typename _detail::expand<typename Base::QuerySet>::FileQueryset _datasource;
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
            typename _detail::expand<typename Base::QuerySet>::MemoryQueryset _datasource;
    };

    template <typename TModel>
    using FileManager = Manager<std::string, TModel>;

    template <typename TModel>
    using MemoryManager = Manager<void, TModel>;

}
