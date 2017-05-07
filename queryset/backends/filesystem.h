
#pragma once

#include "../datasource.h"
#include "../models/manager.h"
#include "read_file.h"

namespace qs {
	namespace _impl {

		template <typename Type, typename... Args>
		class FileDataSource : public _impl::ImplDataSource<Type, Args...> {
			public:
				using qs_type = typename _impl::ImplDataSource<Type, Args...>::qs_type;
			public:
                FileDataSource(const std::string& filename, const bool cache = true) : _filename(filename), _cache(cache), _evaluated(false) {}
                FileDataSource(const FileDataSource& other) : _filename(other._filename), _impl::ImplDataSource<Type, Args...>(other), _cache(false), _evaluated(false) {}
				virtual ~FileDataSource() {}

				virtual qs_type apply(const _impl::FilterContainer<Type, Args...>& filters) const {
					if (!_cache || !_evaluated) {
						SPDLOG_DEBUG(spdlog::get("qs"), "DB HIT! -- FileQueryset::apply -- {}", _filename);
						_filedata.clear();
						utils::read_file<Type, Args...>(_filename, _filedata);
						_evaluated = true;
					}
					return filters.apply(_filedata);
				}

			protected:
				const std::string _filename;
				bool _cache;
				mutable bool _evaluated;
				mutable qs_type _filedata;
		};
	}

	namespace filesystem {

		//template <typename... Args>
		//using DataSource = _impl::FileDataSource<void, Args...>;

		template <typename Type, typename... Args>
		using DataSource = _impl::FileDataSource<Type, Args...>;


		template <typename TModel>
		class Manager : public qs::Manager<TModel, _impl::FileDataSource, std::string> {
			public:
				Manager() : qs::Manager<TModel, _impl::FileDataSource, std::string>(_filename) {};

				static const std::string& filename() { return _filename; };
			protected:
				static const std::string _filename;
		};

	}
}
