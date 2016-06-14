
#pragma once

#include "../datasource.h"

namespace qs {

    template <typename... Args>
    class BaseManager {
        public:
            typedef QuerySet<Args...> QuerySet;
        public:
            virtual ~BaseManager() {};

            virtual QuerySet all() const = 0;
    };


    template <typename... Args, typename TDataSource = void>
    class Manager;

    template <typename... Args>
    class Manager<Args..., std::string> : public BaseManager<Args...> {
        public:
            Manager(const std::string& filename) : _datasource(filename) {};
            virtual ~Manager() {};

            virtual QuerySet all() const {
                return QuerySet(_datasource);
            }
        protected:
            utils::FileQueryset<Args...> _datasource;
    };

    template <typename... Args>
    class Manager<Args..., void> : public BaseManager<Args...> {
        public:
            Manager() {};
            virtual ~Manager() {};

            virtual QuerySet all() const {
                return QuerySet(_datasource);
            }

        protected:
            utils::MemoryQueryset<Args...> _datasource;
    };

}