
#pragma once

#include "easylogging++.h"
#include "../datasource.h"
#include "../queryset.h"

namespace qs {

    template <typename... Args>
    class BaseManager {
        public:
            BaseManager() {};
            virtual ~BaseManager() {};

            virtual ::QuerySet<Args...> all() const = 0;
    };


    template <typename TDataSource, typename... Args>
    class Manager;

    template <typename... Args>
    class Manager<std::string, Args...> : public BaseManager<Args...> {
        public:
            Manager(const std::string& filename) : _datasource(filename) {};
            virtual ~Manager() {};

            virtual ::QuerySet<Args...> all() const {
                LOG(DEBUG) << "Manager<std::string, Args...>::all()";
                return ::QuerySet<Args...>(_datasource);
            }
        protected:
            utils::FileQueryset<Args...> _datasource;
    };

    template <typename... Args>
    class Manager<void, Args...> : public BaseManager<Args...> {
        public:
            Manager() {};
            virtual ~Manager() {};

            virtual ::QuerySet<Args...> all() const {
                LOG(DEBUG) << "Manager<std::string, Args...>::all()";
                return ::QuerySet<Args...>(_datasource);
            }

        protected:
            utils::MemoryQueryset<Args...> _datasource;
    };

}
