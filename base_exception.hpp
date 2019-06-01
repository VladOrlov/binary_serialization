#pragma once

#include <stdexcept>

namespace orlov {
    class base_exception : public std::runtime_error {
    protected:
        explicit base_exception(const std::string &what_arg) :
                runtime_error(what_arg) {}
    };
}
