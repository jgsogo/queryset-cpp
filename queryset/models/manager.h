
#pragma once

#include "spdlog/spdlog.h"
#include "../datasource.h"
#include "../queryset.h"


namespace qs {

    template <typename... Args>
    class BaseManager {
        public:
            typedef typename std::tuple<Args...> tuple;
            typedef typename QuerySet<Args...> QuerySet;
        public:
            BaseManager() {};
            virtual ~BaseManager() {};

            virtual QuerySet all() const = 0;
    };


    template <typename TDataSource, typename... Args>
    class Manager;

    template <typename... Args>
    class Manager<std::string, Args...> : public BaseManager<Args...> {
        public:
            Manager(const std::string& filename) : _datasource(filename) {};
            virtual ~Manager() {};

            virtual QuerySet all() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, Args...>[{}]::all()", (void*)this);
                return QuerySet(_datasource);
            }
        protected:
            utils::FileQueryset<Args...> _datasource;
    };

    template <typename... Args>
    class Manager<void, Args...> : public BaseManager<Args...> {
        public:
            Manager() {};
            virtual ~Manager() {};

            virtual QuerySet all() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, Args...>[{}]::all()", (void*)this);
                return QuerySet(_datasource);
            }

        protected:
            utils::MemoryQueryset<Args...> _datasource;
    };

    template <typename... Args>
    using FileManager = Manager<std::string, Args...>;

    template <typename... Args>
    using MemoryManager = Manager<void, Args...>;

}
