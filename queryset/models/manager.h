
#pragma once

#include "spdlog/spdlog.h"
#include "../datasource.h"
#include "../queryset.h"


namespace qs {

    template <typename pk, typename... Args>
    class BaseManager {
        public:
            typedef typename pk pk;
            typedef typename std::tuple<pk, Args...> tuple;
            typedef typename QuerySet<pk, Args...> QuerySet;
        public:
            BaseManager() {};
            virtual ~BaseManager() {};

            virtual QuerySet all() const = 0;
    };


    template <typename TDataSource, typename pk, typename... Args>
    class Manager;

    template <typename pk, typename... Args>
    class Manager<std::string, pk, Args...> : public BaseManager<pk, Args...> {
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

    template <typename pk, typename... Args>
    class Manager<void, pk, Args...> : public BaseManager<pk, Args...> {
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

    template <typename pk, typename... Args>
    using FileManager = Manager<std::string, pk, Args...>;

    template <typename pk, typename... Args>
    using MemoryManager = Manager<void, pk, Args...>;

}
