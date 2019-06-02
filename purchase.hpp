#pragma once

#include "goods.hpp"
#include "regular_customer.hpp"

#include <algorithm>
#include <cstddef>
#include <experimental/iterator>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace orlov {
    price_t price_for_customer(const goods &goods, const regular_customer &customer) {
        if (goods.has_discount()) {
            return goods.get_price() * (1 - customer.get_discount_rate());
        } else {
            return goods.get_price();
        }
    }

    class purchase {
    public:
        using goods_list = std::vector<goods>;

        explicit purchase(regular_customer customer, goods_list goods = {}) :
                customer_{std::move(customer)},
                goods_{std::move(goods)},
                total_paid_{std::accumulate(goods_.cbegin(), goods_.cend(), price_t{0},
                                            [&customer](const price_t &acc, const class goods &inc) {
                                                return acc + price_for_customer(inc, customer);
                                            })} {
            goods_.shrink_to_fit(); // implements requirement of consuming minimum required memory
        }

        const regular_customer &get_customer() const {
            return customer_;
        }

        const goods_list &get_goods() const {
            return goods_;
        }

        price_t total_price() const {
            return std::accumulate(goods_.cbegin(), goods_.cend(), price_t{0},
                                   [](const price_t &acc, const goods &inc) {
                                       return acc + inc.get_price();
                                   });
        }

        price_t total_paid() const {
            return total_paid_;
        }

        void append(goods elem) {
            // FIXME implement strong exception guarantee
            price_t inc = price_for_customer(elem, get_customer());
            goods_.push_back(std::move(elem));
            total_paid_ = total_paid_ + inc;
        }

        const goods &operator[](std::size_t idx) const {
            return goods_[idx]; // no bounds checking intended; similar to stl containers
        }

        goods &operator[](std::size_t idx) {
            return goods_[idx]; // no bounds checking intended; similar to stl containers
        }

        std::string str() const {
            // FIXME prettify
            return serialize_goods();
        }

        std::string serialize() const {
            std::ostringstream os;
            os << "{customer-" << customer_.serialize() << "^goods-" << serialize_goods() << "^paid:" << total_paid_
               << "}";
            return os.str();
        }

        void print() {
            cout << serialize() << endl;
        }

        static purchase deserialize(const std::string &str) {
            using namespace std::string_literals;
            auto members = split(str, "^");
            if (members.size() != 3) {
//                throw illegal_customer_exception("cannot parse purchase: "s + str);
            }

            const auto &customer_str = members[0];
            auto customer_tokens = split(customer_str, "-");
            if (customer_tokens.size() != 2) {
//                throw illegal_customer_exception("cannot parse customer: "s + customer_str);
            }
            regular_customer customer = regular_customer::deserialize(customer_tokens[1]);

            const auto &goods_str = members[1];
            auto goods_tokens = split(goods_str, "-");
            if (goods_tokens.size() != 2) {
//                throw illegal_customer_exception("cannot parse goods: "s + goods_str);
            }
            goods_list goods = deserialize_goods(goods_tokens[1]);

            const auto &paid_str = members[2];
            auto paid_tokens = split(paid_str, ":");
            if (paid_tokens.size() != 2) {
//                throw illegal_customer_exception("cannot parse price paid: "s + paid_str);
            }
            price_t::underlying_t paid;
            std::istringstream(paid_tokens[1]) >> paid;

            // FIXME paid is not used

            return purchase{customer, goods};
        }

    private:
        std::string serialize_goods() const {
            std::ostringstream os;
            os << "[";
            std::transform(goods_.cbegin(), goods_.cend(), std::experimental::ostream_joiner<std::string>(os, "|"),
                           [](const goods &goods) {
                               return goods.serialize();
                           });
            os << "]";
            return os.str();
        }

        static goods_list deserialize_goods(const std::string &str) {
            const auto goods_as_strings = split(str.substr(1, str.size() - 1), "|");
            goods_list goods;
            std::transform(goods_as_strings.cbegin(), goods_as_strings.cend(), std::back_inserter(goods),
                           [](const std::string &goods_str) {
                               return goods::deserialize(goods_str);
                           });
            return goods;
        }

        regular_customer customer_;
        goods_list goods_;
        price_t total_paid_;
    };

    std::string serialize_purchases(const std::vector<purchase> &purchases) {
        std::ostringstream os;
        std::transform(purchases.cbegin(), purchases.cend(), std::experimental::ostream_joiner<std::string>(os, "\n"),
                       [](const purchase &purchase) {
                           return purchase.serialize();
                       });
        return os.str();
    }

    std::vector<purchase> deserialize_purchases(const std::string &str) {
        const auto purchases_as_strings = split(str, "\n");
        std::vector<purchase> purchases;
        std::transform(purchases_as_strings.cbegin(), purchases_as_strings.cend(), std::back_inserter(purchases),
                       [](const std::string &purchase_str) {
                           return purchase::deserialize(purchase_str);
                       });
        return purchases;
    }

    purchase::goods_list linearize_goods(const std::vector<purchase> &purchases) {
        purchase::goods_list goods;
        for (const purchase &p : purchases) {
            std::copy(p.get_goods().cbegin(), p.get_goods().cend(), std::back_inserter(goods));
        }
        return goods;
    }

    std::unordered_map<std::string, purchase::goods_list>
    group_purchases_by_customer(const std::vector<purchase> &purchases) {
        std::unordered_map<std::string, purchase::goods_list> purchases_by_customer;
        for (const purchase &p : purchases) {
            const auto &cid = p.get_customer().str();
            purchases_by_customer.try_emplace(cid);
            std::copy(p.get_goods().cbegin(), p.get_goods().cend(), std::back_inserter(purchases_by_customer[cid]));
        }
        return purchases_by_customer;
    }

    void print_purchases_as_table(const std::vector<purchase> &purchases, std::ostream &os) {
        auto purchases_by_customer = group_purchases_by_customer(purchases);

        static constexpr unsigned column_width_cid = 14;
        static constexpr unsigned column_width_price = 14;
        static constexpr unsigned column_width_goods = 80 - 4 - column_width_cid - column_width_price;
        static const std::string header{
                "| Customer ID |                      Goods                      |      Price     |"};
        static const std::string separator(header.size(), '-');
        os << header << '\n' << separator << '\n';
        os << std::fixed << std::setprecision(2);
        using std::setw;
        for (const auto &pc : purchases_by_customer) {
            os << setw(column_width_cid) << pc.first << '\n';
            for (const goods &g : pc.second) {
                os << setw(column_width_cid) << '*'
                   << setw(column_width_goods) << g.get_description()
                   << setw(column_width_price) << g.get_price() << '\n';
            }
        }
    }
}
