
#pragma once

#include "easylogging++.h"
#include "../datasource.h"
#include "../queryset.h"

namespace qs {

    template <typename... Args>
    class BaseManager {
        public:
            BaseManager() {
                LOG(DEBUG) << "BaseManager<Args...>::BaseManager()";
            };
            virtual ~BaseManager() {
                LOG(DEBUG) << "BaseManager<Args...>::~BaseManager()";
            };

            virtual ::QuerySet<Args...> all() const = 0;
    };


    template <typename TDataSource, typename... Args>
    class Manager;

    template <typename... Args>
    class Manager<std::string, Args...> : public BaseManager<Args...> {
        public:
            Manager(const std::string& filename) : _datasource(filename) {
                LOG(DEBUG) << "Manager<std::string, Args...>[" << this << "]::Manager(filename='" << filename << "')";
            };
            virtual ~Manager() {
                LOG(DEBUG) << "Manager<std::string, Args...>[" << this << "]::~Manager()";
            };

            virtual ::QuerySet<Args...> all() const {
                LOG(DEBUG) << "Manager<std::string, Args...>[" << this << "]::all()";
                return ::QuerySet<Args...>(_datasource);
            }
        protected:
            utils::FileQueryset<Args...> _datasource;
    };

    template <typename... Args>
    class Manager<void, Args...> : public BaseManager<Args...> {
        public:
            Manager() {
                LOG(DEBUG) << "Manager<Args...>[" << this << "]::Manager()";
            };
            virtual ~Manager() {
                LOG(DEBUG) << "Manager<Args...>[" << this << "]::~Manager()";
            };

            virtual ::QuerySet<Args...> all() const {
                LOG(DEBUG) << "Manager<std::string, Args...>[" << this << "]::all()";
                return ::QuerySet<Args...>(_datasource);
            }

        protected:
            utils::MemoryQueryset<Args...> _datasource;
    };

}
