#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace orlov {
    template<class... Ts>
    struct visitor_options : Ts ... {
        using Ts::operator()...;
    };
    template<class... Ts> visitor_options(Ts...) -> visitor_options<Ts...>;

    std::vector<std::string> split(std::string input, const std::string &delim) {
        std::vector<std::string> tokens;
        std::string next_token;
        std::size_t pos = 0;
        const std::size_t count = delim.length();
        while ((pos = input.find(delim)) != std::string::npos) {
            next_token = input.substr(0, pos);
            tokens.push_back(std::move(next_token));
            input.erase(0, pos + count);
        }
        tokens.push_back(std::move(input));
        return tokens;
    }
}
