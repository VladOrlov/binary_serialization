#pragma once

#include "ranged_value.hpp"
#include "util.hpp"

#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

namespace orlov {
    struct price_ranges {
        static constexpr double min = 0;
        static constexpr double max = std::numeric_limits<double>::max();
    };
    using price_t = ranged_value<double, price_ranges>;

    class illegal_goods_exception final : public base_exception {
    public:
        explicit illegal_goods_exception(const std::string &what_arg) :
                base_exception(what_arg) {}
    };

    class goods final {
    public:
        goods(const std::string &description, price_t price, bool has_discount = false) :
                description_{description},
                price_{price},
                has_discount_{has_discount} {}

        auto get_description() const {
            return description_;
        }

        void set_description(const std::string &value) {
            description_ = value;
        }

        auto get_price() const {
            return price_;
        }

        void set_price(price_t::underlying_t value) {
            price_.set(value);
        }

        auto has_discount() const {
            return has_discount_;
        }

        void set_discount(bool value) {
            has_discount_ = value;
        }

        std::string str() const {
            std::ostringstream os;
            os << description_ << '@' << price_;
            return os.str();
        }

        std::string serialize() const {
            std::ostringstream os;
            os << "{description:" << description_ << "&price:" << price_ << "&discount:" << std::boolalpha
               << has_discount_ << "}";
            return os.str();
        }

        static goods deserialize(const std::string &str) {
            using namespace std::string_literals;
            auto members = split(str, "&");
            if (members.size() != 3) {
                throw illegal_goods_exception("cannot parse goods: "s + str);
            }

            const auto &desc_str = members[0];
            auto desc_tokens = split(desc_str, ":");
            if (desc_tokens.size() != 2) {
                throw illegal_goods_exception("cannot parse goods description: "s + desc_str);
            }
            const std::string &desc = desc_tokens[1];

            const auto &price_str = members[1];
            auto price_tokens = split(price_str, ":");
            if (price_tokens.size() != 2) {
                throw illegal_goods_exception("cannot parse price: "s + price_str);
            }
            price_t price = price_t::from_string(price_tokens[1]);

            const auto &discount_str = members[2];
            auto discount_tokens = split(discount_str, ":");
            if (discount_tokens.size() != 2) {
                throw illegal_goods_exception("cannot parse discount: "s + discount_str);
            }
            bool discount;
            std::istringstream(discount_tokens[1]) >> std::boolalpha >> discount;

            return {desc, price, discount};
        }

    private:
        std::string description_;
        price_t price_;
        bool has_discount_;
    };

    bool operator==(const goods &lhs, const goods &rhs) {
        return (lhs.get_description() == rhs.get_description()) && (lhs.get_price() == rhs.get_price()) &&
               (lhs.has_discount() == rhs.has_discount());
    }

    bool operator!=(const goods &lhs, const goods &rhs) {
        return !(lhs == rhs);
    }

    std::ostream &operator<<(std::ostream &os, const goods &goods) {
        os << goods.str();
        return os;
    }
}
