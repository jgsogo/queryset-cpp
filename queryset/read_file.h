
#pragma once

#include <fstream>
#include <tuple>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "utils/queryset.h"
#include "_impl/utils.h"

namespace utils {
        
    // Reference: http://stackoverflow.com/questions/14033828/translating-a-stdtuple-into-a-template-parameter-pack
    //  - [BUG]: Initializator list order: http://stackoverflow.com/questions/14060264/order-of-evaluation-of-elements-in-list-initialization
    //  - Maybe improve with: http://stackoverflow.com/questions/10014713/build-tuple-using-variadic-templates
    namespace {
        template <typename T>
        T read(std::istringstream& is) {
            std::string item;
            std::getline(is, item, '\t');  // TODO: I need this workaround because there can be empty fields (is >> t consumes all space/tab elements that are adjacent).
            std::stringstream iss(item);
            T t; iss >> t;
            SPDLOG_DEBUG(spdlog::get("qs"), "Read 'item={}' to type '{}' equals to '{}'", item, typeid(t).name(), t);
            return t;
        }

        template <>
        std::string read<std::string>(std::istringstream& is) {
            std::string item;
            std::getline(is, item, '\t');
            return item;
        }

    }
        
    template <typename T>
    std::tuple<T> parse(std::istringstream& stream) {
        return std::tuple<T>{read<T>(stream)};
    }

    template<typename T, typename... Ts, typename = typename std::enable_if<(sizeof...(Ts) > 0), bool>::type>
    std::tuple<T, Ts...> parse(std::istringstream& stream) {
        // TODO: Function below preferred        
        auto t1 = parse<T>(stream);
        auto t2 = parse<Ts...>(stream);
        return std::tuple_cat(t1, t2);
    }
    /*
    TODO: Initialization order fails! :/
    template <typename... Args>
    std::tuple<Args...> parse(std::istringstream& stream) {
        return std::tuple<Args...>{read<Args>(stream)...}
    }
    */

    template<typename Type, typename... Args>
    void read_file(const std::string& filename, typename qs::_impl::utils_queryset<Type, Args...>::type& data) {
        std::ifstream infile(filename);
        if (!infile.good()) {
            std::stringstream os; os << "Cannot open file: '" << filename << "'";
            throw std::runtime_error(os.str().c_str());
        }

        std::string line;
        while (std::getline(infile, line)) {
            if (line.compare(0, 1, "#") != 0) {  // TODO: Only the first line will be a header ;D
                std::istringstream iss(line);
                data.push_back(parse<Args...>(iss));
            }
        }
    }
        
}
