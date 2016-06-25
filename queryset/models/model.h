
#pragma once

#include <type_traits>

#include "spdlog/spdlog.h"
#include "manager.h"
#include <iostream>


namespace qs {

    namespace _detail {

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

            protected:
                mutable tuple _data;
                mutable bool _evaluated;
        };

        template <typename T1, typename T2>
        std::ostream& type_name(std::ostream& os) {
            T1 t1; T2 t2;
            os << typeid(t1).name() << ", " << typeid(t2).name();
            return os;
        }

        template <typename T1, typename T2, typename... Ts, typename = typename std::enable_if<(sizeof...(Ts) > 0), bool>::type>
        std::ostream& type_name(std::ostream& os) {
            T1 t;
            os << typeid(t).name() << ", ";
            return type_name<T2, Ts...>(os);
        }

        template <typename TModel, typename tpk, typename... Args>
        struct helper {
            static std::string name() {
                TModel m; return typeid(m).name();
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
        class BaseModel : public _detail::BaseModelImpl<tpk, Args...> {
            using BaseImpl = _detail::BaseModelImpl<tpk, Args...>;
            public:
                BaseModel() {};
                BaseModel(const tuple& data) : BaseImpl(data) {};
                virtual ~BaseModel() {};

                //static std::string name() { return _detail::name<TModel, tpk, Args...>; }

                static MemoryManager<TModel, tpk, Args...>& objects() {
                    //static_assert(std::is_base_of<BaseManager<Args...>, TManager>::value, "First template argument to qs::Model must be the model itself.");
                    static typename MemoryManager<TModel, tpk, Args...> manager;
                    return manager;
                }

                template <typename T, typename... Ts>
                static Manager<T, TModel, tpk, Args...>& objects(const T& t, Ts... ts) {
                    //static_assert(std::is_base_of<BaseManager<Args...>, TManager>::value, "First template argument to qs::Model must be the model itself.");
                    static typename Manager<T, TModel, tpk, Args...> manager(t, ts...);
                    return manager;
                }

                virtual void print(std::ostream& os) const {
                    os << _detail::helper<TModel, tpk, Args...>::name() << "[" << pk() << "]";
                }
            protected:
                friend bool operator==(const BaseModel<TModel, tpk, Args...>& lhs, const BaseModel<TModel, tpk, Args...>& rhs) {
                    return lhs.pk() == rhs.pk();
                }

                friend bool operator<(const BaseModel<TModel, tpk, Args...>& lhs, const BaseModel<TModel, tpk, Args...>& rhs) {
                    return lhs.pk() < rhs.pk();
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
    class BaseModel : public _detail::BaseModel<TModel, tpk, Args...> {
        using BaseImpl = _detail::BaseModel<TModel, tpk, Args...>;
        public:
            BaseModel() {};
            BaseModel(const tuple& data) : BaseImpl(data) {};
            virtual ~BaseModel() {};

        protected:
            void eval() const {
                if (!_evaluated) {
                    tuple data = TModel::objects().all().get(pk());
                    assert(std::get<0>(data) == pk());
                    _data = data;
                    _evaluated = true;
                }
            }
    };


    template <typename tpk, typename... Args>
    class BaseModel<void, tpk, Args...> : public _detail::BaseModel<void, tpk, Args...> {
        using BaseImpl = _detail::BaseModel<void, tpk, Args...>;
        public:
            BaseModel() {};
            BaseModel(const tuple& data) : BaseImpl(data) {};
            virtual ~BaseModel() {};

        protected:
            void eval() const {
                if (!_evaluated) {
                    tuple data = _detail::BaseModel<void, tpk, Args...>::objects().all().get(pk());
                    assert(std::get<0>(data) == pk());
                    _data = data;
                    _evaluated = true;
                }
            }
    };

    template <typename tpk, typename... Args>
    using Model = BaseModel<void, tpk, Args...>;

}

template<class Ch, class Tr, typename TModel, typename tpk, typename... Args>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, const qs::BaseModel<TModel, tpk, Args...>& rhs) {
    rhs.print(os);
    return os;
}
