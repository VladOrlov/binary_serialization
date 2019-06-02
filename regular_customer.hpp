#pragma once

#include "ranged_value.hpp"
#include "util.hpp"

#include <cstdint>
#include <limits>
#include <string>
#include <fstream>
#include <vector>

#include <iostream>

#include <cstdint>
#include <limits>
#include <string>
#include <utility>
#include <variant>

using namespace std;
using namespace orlov;

namespace orlov {
    struct taxpayer_id_int_ranges {
        static constexpr std::uint64_t min = 1;
        static constexpr std::uint64_t max = 10e10;
    };
    using taxpayer_id_int_t = ranged_value<std::uint64_t, taxpayer_id_int_ranges>;
    using taxpayer_id_t = std::variant<std::monostate, taxpayer_id_int_t, std::string>;

    struct amount_ranges {
        static constexpr double min = 0;
        static constexpr double max = std::numeric_limits<double>::max();
    };
    using amount_t = ranged_value<double, amount_ranges>;

    struct discount_ranges {
        static constexpr double min = 0;
        static constexpr double max = 1 - std::numeric_limits<double>::epsilon();
    };
    using discount_t = ranged_value<double, discount_ranges>;

    class regular_customer final {
    private:
        taxpayer_id_t taxpayer_id_;
        amount_t amount_spent_;
        discount_t discount_rate_;

    public:
        explicit
        regular_customer(taxpayer_id_t taxpayer_id, amount_t amount_spent = 0, discount_t discount_rate = 0) :
                taxpayer_id_{taxpayer_id},
                amount_spent_{amount_spent},
                discount_rate_{discount_rate} {

        }

        auto get_amount_spent() const {
            return amount_spent_.get();
        }

        void set_amount_spent(amount_t::underlying_t value) {
            amount_spent_.set(value);
        }

        auto get_discount_rate() const {
            return discount_rate_.get();
        }

        auto get_id() const {
            std::string str;
            std::visit(visitor_options{
                    [&str](taxpayer_id_int_t arg) {
                        str = std::to_string(arg.get());
                    },
                    [&str](const std::string &arg) {
                        str = arg;
                    },
                    [](std::monostate) {
                        throw std::bad_variant_access(); // this should never happen
                    }
            }, taxpayer_id_);
            return str;
        }

        void set_discount_rate(discount_t::underlying_t value) {
            discount_rate_.set(value);
        }

        void print() {
            cout << "CUSTOMER ID: " << get_id() << endl
                 << "CUSTOMER AMOUNT: " << get_amount_spent() << endl
                 << "CUSTOMER DISCOUNT_RATE: " << get_discount_rate() << endl;
        }

        std::string str() const {
            std::ostringstream os;
            os << to_string(taxpayer_id_);
            return os.str();
        }

        std::string to_string(const taxpayer_id_t &id) const {
            std::string str;
            std::visit(visitor_options{
                    [&str](taxpayer_id_int_t arg) {
                        str = std::to_string(arg.get());
                    },
                    [&str](const std::string &arg) {
                        str = arg;
                    },
                    [](std::monostate) {
                        throw std::bad_variant_access(); // this should never happen
                    }
            }, id);
            return str;
        }

        std::string serialize() const {
            std::string taxpayer_id_marker{};
            std::visit(visitor_options{
                    [&taxpayer_id_marker](taxpayer_id_int_t) {
                        taxpayer_id_marker = "taxpayer_id_int";
                    },
                    [&taxpayer_id_marker](std::string) {
                        taxpayer_id_marker = "taxpayer_id_str";
                    },
                    [](std::monostate) {
                        throw std::bad_variant_access(); // this should never happen
                    }
            }, taxpayer_id_);
            std::ostringstream os;
            os << "{" << taxpayer_id_marker << ":" << to_string(taxpayer_id_) << "&spent:" << amount_spent_
               << "&discount:" << discount_rate_ << "}";
            return os.str();
        }

        static regular_customer deserialize(const std::string &str) {
            using namespace std::string_literals;
            auto members = split(str, "&");
            if (members.size() != 3) {
                throw exception();
            }

            const auto &id_str = members[0];
            auto id_tokens = split(id_str, ":");
            if (id_tokens.size() != 2) {
                throw exception();
            }
            taxpayer_id_t id;
            const std::string id_marker = id_tokens[0].substr(1); // trim starting brace
            if (id_marker == "taxpayer_id_int") {
                taxpayer_id_int_t::underlying_t id_int;
                std::istringstream(id_tokens[1]) >> id_int;
                id = id_int;
            } else if (id_marker == "taxpayer_id_str") {
                id = id_tokens[1];
            } else {
                throw exception();
            }

            const auto &spent_str = members[1];
            auto spent_tokens = split(spent_str, ":");
            if (spent_tokens.size() != 2) {
                throw exception();
            }
            amount_t spent = amount_t::from_string(spent_tokens[1]);

            const auto &discount_str = members[2];
            auto discount_tokens = split(discount_str, ":");
            if (discount_tokens.size() != 2) {
                throw exception();
            }
            discount_t::underlying_t discount;
            std::istringstream(discount_tokens[1]) >> discount;

            return regular_customer{id, spent, discount};
        }
    };
}