
#pragma once

#include "spdlog/spdlog.h"
#include "../datasource.h"
#include "../queryset.h"

namespace qs {

    template <typename... Args>
    class BaseManager {
        public:
            BaseManager() {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseManager<Args...>::BaseManager()");
            };
            virtual ~BaseManager() {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseManager<Args...>::~BaseManager()");
            };

            virtual ::QuerySet<Args...> all() const = 0;
    };


    template <typename TDataSource, typename... Args>
    class Manager;

    template <typename... Args>
    class Manager<std::string, Args...> : public BaseManager<Args...> {
        public:
            Manager(const std::string& filename) : _datasource(filename) {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, Args...>[{}]::Manager(filename='{}')", (void*)this, filename);
            };
            virtual ~Manager() {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, Args...>[{}]::~Manager()", (void*)this);
            };

            virtual ::QuerySet<Args...> all() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, Args...>[{}]::all()", (void*)this);
                return ::QuerySet<Args...>(_datasource);
            }
        protected:
            utils::FileQueryset<Args...> _datasource;
    };

    template <typename... Args>
    class Manager<void, Args...> : public BaseManager<Args...> {
        public:
            Manager() {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<Args...>[{}]::Manager()", (void*)this);
            };
            virtual ~Manager() {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<Args...>[{}]::~Manager()", (void*)this);
            };

            virtual ::QuerySet<Args...> all() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "Manager<std::string, Args...>[{}]::all()", (void*)this);
                return ::QuerySet<Args...>(_datasource);
            }

        protected:
            utils::MemoryQueryset<Args...> _datasource;
    };

}
