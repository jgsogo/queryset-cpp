
#pragma once

namespace utils {
    
    // Credit: http://stackoverflow.com/questions/23726038/how-can-i-create-a-new-primitive-type-using-c11-style-strong-typedefs

    template <typename T, int N> // N is used for tagging
    struct strong_typedef {
        using strong_type = strong_typedef<T, N>; // typedef for the strong type
        using type = T; // the wrapped type
        T value; // the wrapped value

        explicit strong_typedef(const T& val): value(val){}; // constructor
        strong_typedef(const strong_typedef& other) : value(other.value) {}; // constructor
        strong_typedef(){value={};}; // default, zero-initialization

        // operator overloading, basic example: 
        bool operator==(const strong_type& rhs) const {
            return value == rhs.value;
        }

        // comparator
        bool operator<(const strong_type& rhs) const {
            return value < rhs.value;
        }

        // display it
        friend std::ostream& operator<<(std::ostream & lhs, const strong_typedef& rhs)
        {
            lhs << rhs.value;
            return lhs;
        }

        // assign value
        friend std::istream& operator>>(std::istream & lhs, strong_typedef& rhs)
        {
            lhs >> rhs.value;
            return lhs;
        }
    };

}

