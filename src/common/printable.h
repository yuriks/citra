// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <iosfwd>
#include <string>
#include <type_traits>
#include <fmt/format.h>

namespace Common {

/**
 * Tag indicating that an object can create a debugging string representing itself. Type must
 * publicly inherit from this tag and then implement a method with this signature:
 *
 *     void DebugFmt(fmt::Writer& w) const;
 *
 * If the class is already already polymorphic (has virtual functions), then prefer using
 * VirtualPrintable instead.
 */
class Printable {};

/**
 * Extension of Printable for classes using virtual dispatch. This is compatible with Printable, but
 * makes DebugFmt() a virtual function, allowing dynamically printing sub-types from a base pointer.
 * If the class uses no virtuals then prefer plain Printable instead.
 */
class VirtualPrintable : public Printable {
public:
    /**
     * Formats a string describing the object, for debugging purposes.
     *
     * @note To output a std::string, simply pipe it into the writer using `<<` (ex.: `w <<
     * my_string;`). To write a fmt format string, use the `format(...)` method (ex.:
     * `w.format("[Foo: {}]", my_num);`).
     */
    virtual void DebugFmt(fmt::Writer& w) const = 0;
};

/// Blanket implementation of formatting, which uses DebugFmt() on any type tagged with Printable.
template <typename T, typename = std::enable_if_t<std::is_base_of_v<Printable, T>>>
void format_arg(fmt::BasicFormatter<char>& f, const char*&, const T& object) {
    // Enforce correct usage of the tag types.
    static_assert(std::is_base_of_v<VirtualPrintable, T> || !std::is_polymorphic_v<T>,
                  "Use VirtualPrintable for polymorphic types instead of Printable.");
    static_assert(!std::is_base_of_v<VirtualPrintable, T> || std::has_virtual_destructor_v<T>,
                  "Use Printable for non-polymorphic types instead of VirtualPrintable.");

    object.DebugFmt(f.writer());
}

/// Blanket implementation of operator<<, which uses DebugFmt() on any type tagged with Printable.
template <typename T, typename = std::enable_if_t<std::is_base_of_v<Printable, T>>>
std::ostream& operator<<(std::ostream& s, const T& object) {
    // Enforce correct usage of the tag types.
    static_assert(std::is_base_of_v<VirtualPrintable, T> || !std::is_polymorphic_v<T>,
                  "Use VirtualPrintable for polymorphic types instead of Printable.");
    static_assert(!std::is_base_of_v<VirtualPrintable, T> || std::has_virtual_destructor_v<T>,
                  "Use Printable for non-polymorphic types instead of VirtualPrintable.");

    fmt::MemoryWriter w;
    object.DebugFmt(w);
    return w.str();
}

} // namespace Common
