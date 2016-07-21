
#pragma once

#include <type_traits>
#include <iostream>

#include "spdlog/spdlog.h"

#include "manager.h"


namespace qs {

    namespace _impl {

        template <typename tpk, typename... Args>
        class BaseModelImpl {
            public:
                typedef typename std::tuple<tpk, Args...> tuple;
            
            public:
                BaseModelImpl() : _evaluated(false) {};
                BaseModelImpl(const tuple& data) : _data(data), _evaluated(true) {};
                virtual ~BaseModelImpl() {};

                tpk pk() const {
                    return std::get<0>(_data);
                }

                operator tuple() const {
                    return _data;
                }

                template <std::size_t I>
                auto get() const {
                    return std::get<I>(_data);
                }

            protected:
                mutable tuple _data;
                mutable bool _evaluated;
        };

        template <typename T1, typename T2>
        std::ostream& type_name(std::ostream& os) {
            os << typeid(T1).name() << ", " << typeid(T2).name();
            return os;
        }

        template <typename T1, typename T2, typename... Ts, typename = typename std::enable_if<(sizeof...(Ts) > 0), bool>::type>
        std::ostream& type_name(std::ostream& os) {
            os << typeid(T1).name() << ", ";
            return type_name<T2, Ts...>(os);
        }

        template <typename TModel, typename tpk, typename... Args>
        struct helper {
            static std::string name() {
                return typeid(TModel).name();
            }
        };

        template <typename tpk, typename... Args>
        struct helper<void, tpk, Args...> {
            static std::string name() {
                std::ostringstream os;
                os << "BaseModel<";
                type_name<tpk, Args...>(os);
                os << ">";
                return os.str();
            }
        };

        template <typename TModel, typename tpk, typename... Args>
        class BaseModel : public _impl::BaseModelImpl<tpk, Args...> {
            using BaseImpl = _impl::BaseModelImpl<tpk, Args...>;
            public:
                BaseModel() {};
                BaseModel(const typename BaseImpl::tuple& data) : BaseImpl(data) {};
                virtual ~BaseModel() {};

                //static std::string name() { return _impl::name<TModel, tpk, Args...>; }

                static MemoryManager<TModel>& objects() {
                    //static_assert(std::is_base_of<BaseManager<Args...>, TManager>::value, "First template argument to qs::Model must be the model itself.");
                    static MemoryManager<TModel> manager;
                    return manager;
                }

                template <typename T, typename... Ts>
                static Manager<T, TModel>& objects(const T& t, Ts... ts) {
                    //static_assert(std::is_base_of<BaseManager<Args...>, TManager>::value, "First template argument to qs::Model must be the model itself.");
                    static Manager<T, TModel> manager(t, ts...);
                    return manager;
                }

                virtual void print(std::ostream& os) const {
                    os << _impl::helper<TModel, tpk, Args...>::name() << "[" << BaseImpl::pk() << "]";
                }
            protected:
                // Equality
                friend bool operator==(const BaseModel<TModel, tpk, Args...>& lhs, const BaseModel<TModel, tpk, Args...>& rhs) {
                    return lhs.pk() == rhs.pk();
                }
                friend bool operator==(const BaseModel<TModel, tpk, Args...>& lhs, const tpk& rhs) {
                    return lhs.pk() == rhs;
                }

                // Comparaison
                friend bool operator<(const BaseModel<TModel, tpk, Args...>& lhs, const BaseModel<TModel, tpk, Args...>& rhs) {
                    return lhs.pk() < rhs.pk();
                }
                friend bool operator<(const BaseModel<TModel, tpk, Args...>& lhs, const tpk& rhs) {
                    return lhs.pk() < rhs;
                }

                // assign value
                friend std::istream& operator >> (std::istream & lhs, BaseModel<TModel, tpk, Args...>& rhs)
                {
                    // Read just one value: primary key
                    lhs >> std::get<0>(rhs._data);
                    return lhs;
                }
        };

    }


    template <typename TModel, typename tpk, typename... Args>
    class BaseModel : public _impl::BaseModel<TModel, tpk, Args...> {
        using BaseImpl = _impl::BaseModel<TModel, tpk, Args...>;
        public:
            using QuerySet = _impl::QuerySet<TModel, tpk, Args...>;
        public:
            BaseModel() {};
            BaseModel(const typename BaseImpl::tuple& data) : BaseImpl(data) {};
            virtual ~BaseModel() = 0;

        protected:
            void eval() const {
                if (!BaseImpl::_evaluated) {
                    BaseImpl::_data = TModel::objects().all().get(BaseImpl::pk());
                    BaseImpl::_evaluated = true;
                }
            }
    };
    template <typename TModel, typename tpk, typename... Args>
    BaseModel<TModel, tpk, Args...>::~BaseModel() {};


    template <typename tpk, typename... Args>
    class BaseModel<void, tpk, Args...> : public _impl::BaseModel<BaseModel<void, tpk, Args...>, tpk, Args...> {
        using BaseImpl = _impl::BaseModel<void, tpk, Args...>;
        public:
            using QuerySet = _impl::QuerySet<void, tpk, Args...>;;
        public:
            BaseModel() {};
            BaseModel(const typename BaseImpl::tuple& data) : BaseImpl(data) {};
            virtual ~BaseModel() = 0;

        protected:
            void eval() const {
                if (!BaseImpl::_evaluated) {
                    BaseImpl::_data = _impl::BaseModel<BaseModel, tpk, Args...>::objects().all().get(BaseImpl::pk());
                    BaseImpl::_evaluated = true;
                }
            }
    };
    template <typename tpk, typename... Args>
    BaseModel<void, tpk, Args...>::~BaseModel() {};


    template <typename tpk, typename... Args>
    class Model final : public BaseModel<void, tpk, Args...> {
        using BaseM = BaseModel<void, tpk, Args...>;
        public:
            Model() {};
            Model(const typename BaseM::tuple& data) : BaseM(data) {};
            virtual ~Model() {};
    };

    // std::get delegate, we not always have a std::tuple here
    template <std::size_t I, typename TModel, typename tpk, typename... Args>
    auto getter(const qs::BaseModel<TModel, tpk, Args...>& item) {
        return item.template get<I>();
    }

}


template<class Ch, class Tr, typename TModel, typename tpk, typename... Args>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, const qs::BaseModel<TModel, tpk, Args...>& rhs) {
    rhs.print(os);
    return os;
}
