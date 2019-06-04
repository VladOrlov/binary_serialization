#pragma once

#include <cstddef>
#include <cstring>
#include <memory>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace orlov {
    template<class... Ts>
    struct visitor_options : Ts ... {
        using Ts::operator()...;
    };
    template<class... Ts> visitor_options(Ts...) -> visitor_options<Ts...>;

    struct data_description {
        std::size_t size;
        std::unique_ptr<char[]> data;

        auto getData() {
            return data.get();
        }
    };

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

    template<class T>
    T from_bytes(void *bytes) {
        return *static_cast<T *>(bytes);
    }

    template<class T>
    std::vector<T> vector_from_bytes(void *bytes) {
        auto size = from_bytes<std::size_t>(bytes);
        auto elements_memory = (static_cast<char *>(bytes) + sizeof(std::size_t));
        auto elements_begin = reinterpret_cast<T *>(elements_memory);
        auto elements_end = elements_begin + size;
        std::vector<T> elements(0);
        elements.reserve(size);
        std::copy(elements_begin, elements_end, std::back_inserter(elements));
        assert(elements.size() == size);
        return elements;
    }

    template<class T>
    std::unique_ptr<char[]> to_bytes(const T &obj) {
        auto memory = std::make_unique<char[]>(sizeof(T));
        std::memcpy(memory.get(), &obj, sizeof(T));
        return memory;
    }

    template<class T>
    std::unique_ptr<char[]> vector_to_bytes(const std::vector<T> &container) {
        auto container_size = container.size();
        auto memory_ptr = std::make_unique<char[]>(
                sizeof(container_size) + (sizeof(T) * container_size)); // size followed by elements
        std::memcpy(memory_ptr.get(), &container_size, sizeof(container_size));
        auto elements_begin = reinterpret_cast<T *>(memory_ptr.get() + sizeof(container_size));
        std::copy(container.cbegin(), container.cend(), elements_begin);
        return memory_ptr;
    }
}
