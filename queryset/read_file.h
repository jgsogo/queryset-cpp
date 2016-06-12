
#pragma once

#include <fstream>
#include <tuple>
#include <vector>
#include <string>
#include <sstream>
#include "utils/queryset.h"

namespace utils {
        
    // Reference: http://stackoverflow.com/questions/14033828/translating-a-stdtuple-into-a-template-parameter-pack
    //  - [BUG]: Initializator list order: http://stackoverflow.com/questions/14060264/order-of-evaluation-of-elements-in-list-initialization
    //  - Maybe improve with: http://stackoverflow.com/questions/10014713/build-tuple-using-variadic-templates
    namespace {
        template <typename T>
        T read(std::istringstream& is) {
            T t; is >> t; return t;
        }
    }
        
    template<typename... Args>
    std::tuple<Args...> parse(std::istringstream& stream) {            
        return std::tuple<Args...> { read<Args>(stream)... };
    }
        
    template<typename... Args>
    void read_file(const std::string& filename, queryset<Args...>& data) {
        std::ifstream infile(filename);
        if (!infile.good()) {
            std::stringstream os; os << "Cannot open file: '" << filename << "'";
            throw std::runtime_error(os.str().c_str());
        }
            
        std::string line;
        while (std::getline(infile, line)) {
            if (line.compare(0, 1, "#") != 0) {
                std::istringstream iss(line);
                data.push_back(parse<Args...>(iss));
            }
        }
    }
        
}
