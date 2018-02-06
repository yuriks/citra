// Copyright 2018 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <boost/variant.hpp>
#include "common/common_funcs.h"

namespace Common {

struct OkType {};

template <typename T = OkType, typename E = boost::blank>
class Result : public boost::variant<T, E> {
public:
    // boost::variant guarantees that if any type has a nothrow constructor, then assignments will
    // never heap allocate and instead the variant will be set to that type. We want to enforce
    // this, so assert that the Error type fits that criteria.
    static_assert(boost::has_nothrow_constructor<E>::value, "Error type in Result must have "
                                                            "nothrow default constructor in order "
                                                            "to avoid heap allocations");

    using boost::variant<T, E>::variant;

    bool Succeeded() const {
        return this->which() == 0;
    }

    bool Failed() const {
        return this->which() == 1;
    }

    /// Asserts that the result succeeded and returns a reference to it.
    T& Unwrap() & {
        // TODO(yuriks): Try to format the E type here if possible
        ASSERT_MSG(Succeeded(), "Tried to Unwrap failed ResultVal");
        return boost::get<T>(*this);
    }

    const T& Unwrap() const& {
        ASSERT_MSG(Succeeded(), "Tried to Unwrap failed ResultVal");
        return boost::get<T>(*this);
    }

    T&& Unwrap() && {
        ASSERT_MSG(Succeeded(), "Tried to Unwrap failed ResultVal");
        return std::move(boost::get<T>(*this));
    }

    /// Asserts that the result failed and returns a reference to the error
    E& UnwrapErr() & {
        // TODO(yuriks): Try to format the T type here if possible
        ASSERT_MSG(Failed(), "Tried to UnwrapErr successful ResultVal");
        return boost::get<E>(*this);
    }

    E& UnwrapErr() const& {
        ASSERT_MSG(Failed(), "Tried to UnwrapErr successful ResultVal");
        return boost::get<E>(*this);
    }

    E&& UnwrapErr() && {
        ASSERT_MSG(Failed(), "Tried to Unwrap successful ResultVal");
        return boost::get<E>(std::move(*this));
    }
};

#define CASCADE_COMMON_RESULT(target, source)                                                      \
    auto CONCAT2(check_result_L, __LINE__) = source;                                               \
    if (!CONCAT2(check_result_L, __LINE__).Succeeded())                                            \
        return CONCAT2(check_result_L, __LINE__).UnwrapErr();                                      \
    target = std::move(CONCAT2(check_result_L, __LINE__).Unwrap())

} // namespace Common
