#pragma once

#include "base_exception.hpp"

#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace orlov {
    class illegal_value_exception final : public base_exception {
    public:
        explicit illegal_value_exception(const std::string &what_arg) :
                base_exception(what_arg) {}
    };

    template<class underlying_type,
            class ranges>
    class ranged_value final {
    public:
        using underlying_t = underlying_type;

        ranged_value(underlying_type value) :
                value_{value} {
            verify_value(value);
        }

        ranged_value &operator=(underlying_type value) {
            set(value);
            return *this;
        }

        ranged_value &operator=(underlying_type &&value) {
            set(std::move(value));
            return *this;
        }

        underlying_type get() const {
            return value_;
        }

        void set(underlying_type value) {
            verify_value(value);
            value_ = value;
        }

        static ranged_value from_string(const std::string &str) {
            underlying_type value;
            std::istringstream(str) >> value;
            return value;
        }

    private:
        underlying_type value_;

        void verify_value(underlying_type value) {
            if ((value < ranges::min) || (value > ranges::max)) {
                using namespace std::string_literals;
                throw illegal_value_exception("illegal value: "s + std::to_string(value));
            }
        }
    };

    template<class underlying_type, class ranges>
    std::ostream &operator<<(std::ostream &os, ranged_value<underlying_type, ranges> value) {
        return os << std::to_string(value.get());
    }

    template<class underlying_type, class ranges>
    bool
    operator==(const ranged_value<underlying_type, ranges> &lhs, const ranged_value<underlying_type, ranges> &rhs) {
        return lhs.get() == rhs.get(); // comparison of possible floating point values with == intended
    }

    template<class underlying_type, class ranges>
    bool
    operator!=(const ranged_value<underlying_type, ranges> &lhs, const ranged_value<underlying_type, ranges> &rhs) {
        return !(lhs == rhs);
    }

    template<class underlying_type, class ranges>
    ranged_value<underlying_type, ranges>
    operator+(const ranged_value<underlying_type, ranges> &lhs, const ranged_value<underlying_type, ranges> &rhs) {
        return lhs.get() + rhs.get();
    }

    template<class underlying_type, class ranges>
    ranged_value<underlying_type, ranges>
    operator-(const ranged_value<underlying_type, ranges> &lhs, const ranged_value<underlying_type, ranges> &rhs) {
        return lhs.get() - rhs.get();
    }

    template<class underlying_lhs, class underlying_rhs, class ranges_lhs, class ranges_rhs>
    typename std::common_type<underlying_lhs, underlying_rhs>::type
    operator*(const ranged_value<underlying_lhs, ranges_lhs> &lhs,
              const ranged_value<underlying_rhs, ranges_rhs> &rhs) {
        return lhs.get() * rhs.get();
    }

    template<class underlying_type, class ranges, class T>
    typename std::common_type<underlying_type, T>::type
    operator*(const ranged_value<underlying_type, ranges> &lhs, const T &rhs) {
        return lhs.get() * rhs;
    }

    template<class underlying_type, class ranges, class T>
    typename std::common_type<underlying_type, T>::type
    operator*(const T &lhs, const ranged_value<underlying_type, ranges> &rhs) {
        return lhs * rhs.get();
    }

    template<class underlying_lhs, class underlying_rhs, class ranges_lhs, class ranges_rhs>
    typename std::common_type<underlying_lhs, underlying_rhs>::type
    operator/(const ranged_value<underlying_lhs, ranges_lhs> &lhs,
              const ranged_value<underlying_rhs, ranges_rhs> &rhs) {
        return lhs.get() / rhs.get();
    }

    template<class underlying_type, class ranges, class T>
    typename std::common_type<underlying_type, T>::type
    operator/(const ranged_value<underlying_type, ranges> &lhs, const T &rhs) {
        return lhs.get() / rhs;
    }

    template<class underlying_type, class ranges, class T>
    typename std::common_type<underlying_type, T>::type
    operator/(const T &lhs, const ranged_value<underlying_type, ranges> &rhs) {
        return lhs / rhs.get();
    }
}
