
#pragma once

#include <type_traits>
#include <iostream>

#include "spdlog/spdlog.h"
#include "../utils/to_string.h"
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

                std::string to_string() const {
                    return utils::to_string(pk());
                }

                operator tuple&() const {
                    return _data;
                }

				template <std::size_t I>
				auto get() const {
					if (I != 0 && !_evaluated) {
						this->eval();
					}
					return std::get<I>(_data);
				}

			protected:
				virtual void eval() const = 0;

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
        class BaseModel : public BaseModelImpl<tpk, Args...> {
            using BaseImpl = BaseModelImpl<tpk, Args...>;
            public:
                BaseModel() {};
                BaseModel(const typename BaseImpl::tuple& data) : BaseImpl(data) {};
                virtual ~BaseModel() {};

                //static std::string name() { return _impl::name<TModel, tpk, Args...>; }

				/*
                template <typename T, typename... Ts>
                static Manager<T, TModel>& objects(const T& t, Ts... ts) {
                    //static_assert(std::is_base_of<BaseManager<Args...>, TManager>::value, "First template argument to qs::Model must be the model itself.");
                    static Manager<T, TModel> manager(t, ts...);
                    return manager;
                }
				*/

                virtual void print(std::ostream& os) const {
                    os << helper<TModel, tpk, Args...>::name() << "[" << BaseImpl::pk() << "]";
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


    template <typename TModel,
              template <typename TModel> class TManager,
              typename tpk, typename... Args>
    class BaseModel : public _impl::BaseModel<TModel, tpk, Args...> {
        using BaseImpl = _impl::BaseModel<TModel, tpk, Args...>;
        using Manager = TManager<TModel>;
        public:
            using QuerySet = _impl::QuerySet<TModel, tpk, Args...>;
        public:
            BaseModel() {};
            BaseModel(const typename BaseImpl::tuple& data) : BaseImpl(data) {};
            virtual ~BaseModel() = 0;

            static Manager& objects() {
                static Manager manager;
                return manager;
            }
			
            //template <typename... Args2>
            //static Manager& objects(Args2... args) {
            //    static Manager manager(args...);
            //    return manager;
            //}

        protected:
            void eval() const {
                if (!this->_evaluated) {
					this->_data = TModel::objects().all().get(this->pk());
					this->_evaluated = true;
                }
            }
    };
    template <typename TModel, template <typename TModel> class TManager, typename tpk, typename... Args>
    BaseModel<TModel, TManager, tpk, Args...>::~BaseModel() {};


    template <template <typename TModel> class TManager, typename tpk, typename... Args>
    class Model final : public BaseModel<Model<TManager, tpk, Args...>, TManager, tpk, Args...> {
        using BaseM = BaseModel<Model<TManager, tpk, Args...>, TManager, tpk, Args...>;
        public:
            Model() {};
            Model(const typename BaseM::tuple& data) : BaseM(data) {};
            virtual ~Model() {};

    };

    // std::get delegate, we not always have a std::tuple here
    template <std::size_t I, typename TModel, template <typename TModel> class TManager, typename tpk, typename... Args>
    auto getter(const qs::BaseModel<TModel, TManager, tpk, Args...>& item) {
        return item.template get<I>();
    }

}


template<class Ch, class Tr, typename TModel, template <typename TModel> class TManager, typename tpk, typename... Args>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, const qs::BaseModel<TModel, TManager, tpk, Args...>& rhs) {
    rhs.print(os);
    return os;
}
