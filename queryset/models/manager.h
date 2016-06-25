
#pragma once

#include "spdlog/spdlog.h"
#include "../datasource.h"
#include "queryset_typed.h"


namespace qs {

    template <typename TModel, typename pk, typename...Args>
    class BaseManager {
        public:
            typedef typename qs::TypedQuerySet<TModel, pk, Args...> QuerySet;
        public:
            BaseManager() {};
            virtual ~BaseManager() {};

            virtual QuerySet all() const = 0;
    };

    template <typename pk, typename...Args>
    class BaseManager<void, pk, Args...> {
        public:
            typedef typename QuerySet<pk, Args...> QuerySet;
        public:
            BaseManager() {};
            virtual ~BaseManager() {};

            virtual QuerySet all() const = 0;
    };


    template <typename TDataSource, typename TModel, typename pk, typename... Args>
    class Manager;

    template <typename TModel, typename pk, typename... Args>
    class Manager<std::string, TModel, pk, Args...> : public BaseManager<TModel, pk, Args...> {
        public:
            Manager(const std::string& filename) : _datasource(filename) {};
            virtual ~Manager() {};

            virtual QuerySet all() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, pk, Args...>[{}]::all()", (void*)this);
                return QuerySet(_datasource);
            }
        protected:
            utils::FileQueryset<pk, Args...> _datasource;
    };

    template <typename TModel, typename pk, typename... Args>
    class Manager<void, TModel, pk, Args...> : public BaseManager<TModel, pk, Args...> {
        public:
            Manager() {};
            virtual ~Manager() {};

            virtual QuerySet all() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, Args...>[{}]::all()", (void*)this);
                return QuerySet(_datasource);
            }

        protected:
            utils::MemoryQueryset<pk, Args...> _datasource;
    };

    template <typename TModel, typename pk, typename... Args>
    using FileManager = Manager<std::string, TModel, pk, Args...>;

    template <typename TModel, typename pk, typename... Args>
    using MemoryManager = Manager<void, TModel, pk, Args...>;

}
