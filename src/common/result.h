// Copyright 2018 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <boost/optional.hpp>
#include "common/common_funcs.h"

namespace Common {

struct OkType {};
struct ErrType {};

template <typename T = OkType, typename E = ErrType>
class Result {
public:
    Result(const T& value) : value(boost::in_place_init, value) {}
    Result(const E& error) : error(boost::in_place_init, error) {}
    Result(T&& value) : value(boost::in_place_init, std::move(value)) {}
    Result(E&& error) : error(boost::in_place_init, std::move(error)) {}

    bool Succeeded() const {
        return static_cast<bool>(value);
    }

    bool Failed() const {
        return static_cast<bool>(error);
    }

    /// Asserts that the result succeeded and returns a reference to it.
    T& Unwrap() & {
        // TODO(yuriks): Try to format the E type here if possible
        ASSERT_MSG(Succeeded(), "Tried to Unwrap failed ResultVal");
        return *value;
    }

    const T& Unwrap() const& {
        ASSERT_MSG(Succeeded(), "Tried to Unwrap failed ResultVal");
        return *value;
    }

    T&& Unwrap() && {
        ASSERT_MSG(Succeeded(), "Tried to Unwrap failed ResultVal");
        return std::move(*value);
    }

    /// Asserts that the result failed and returns a reference to the error
    E& UnwrapErr() & {
        // TODO(yuriks): Try to format the T type here if possible
        ASSERT_MSG(Failed(), "Tried to UnwrapErr successful ResultVal");
        return *error;
    }

    const E& UnwrapErr() const& {
        ASSERT_MSG(Failed(), "Tried to UnwrapErr successful ResultVal");
        return *error;
    }

    E&& UnwrapErr() && {
        ASSERT_MSG(Failed(), "Tried to Unwrap successful ResultVal");
        return std::move(*error);
    }

private:
    // Invariant: Exactly one of value or error contains a value
    boost::optional<T> value;
    boost::optional<E> error;
};

#define CASCADE_COMMON_RESULT(target, source)                                                      \
    auto CONCAT2(check_result_L, __LINE__) = source;                                               \
    if (CONCAT2(check_result_L, __LINE__).Failed())                                                \
        return CONCAT2(check_result_L, __LINE__).UnwrapErr();                                      \
    target = std::move(CONCAT2(check_result_L, __LINE__).Unwrap())

} // namespace Common
