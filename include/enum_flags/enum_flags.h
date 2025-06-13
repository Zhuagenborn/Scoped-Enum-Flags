/**
 * @file enum_flags.h
 * @brief The type-safe bit flag manager for C++11 scoped enumerations.
 *
 * @details
 * This type-safe bit flag manager supports:
 *
 * - Setting and clearing individual or multiple flags.
 * - Checking if any or multiple flags are set.
 * - Combining multiple flags into a single flag.
 *
 * @par GitHub
 * https://github.com/Zhuagenborn
 */

#pragma once

#include <algorithm>
#include <initializer_list>
#include <ranges>
#include <type_traits>
#include <utility>

//! The type-safe bit flag manager for C++11 scoped enumerations.
template <typename Enum>
    requires std::is_scoped_enum_v<std::decay_t<Enum>>
             && std::unsigned_integral<std::underlying_type_t<std::decay_t<Enum>>>
class EnumFlags {
    //! The underlying type of the enumeration.
    using RawType = std::underlying_type_t<std::decay_t<Enum>>;

public:
    //! Create an enumeration flag value.
    static consteval RawType CreateFlag(const std::size_t shift) noexcept {
        return static_cast<RawType>(1) << shift;
    }

    auto operator<=>(const EnumFlags&) = delete;

    //! Construct flags from an initializer list of enumeration values.
    constexpr EnumFlags(const std::initializer_list<Enum> flags) noexcept {
        AddFlags(flags);
    }

    //! Construct flags from a range of enumeration values.
    template <std::ranges::range Flags>
        requires std::is_same_v<Enum, std::ranges::range_value_t<Flags>>
    constexpr EnumFlags(Flags&& flags) noexcept {
        AddFlags(std::forward<Flags>(flags));
    }

    //! Construct flags from a underlying-type value.
    constexpr EnumFlags(const RawType flags = 0) noexcept : flags_ {flags} {}

    //! Construct flags from an enumeration value.
    constexpr EnumFlags(const Enum flag) noexcept : flags_ {std::to_underlying(flag)} {}

    //! Clear all flags.
    constexpr EnumFlags& Clear() noexcept {
        flags_ = 0;
        return *this;
    }

    //! Remove specific flags.
    constexpr EnumFlags& Remove(const EnumFlags flags) noexcept {
        flags_ &= ~static_cast<RawType>(flags);
        return *this;
    }

    //! Add specific flags.
    constexpr EnumFlags& Add(const EnumFlags flags) noexcept {
        flags_ |= static_cast<RawType>(flags);
        return *this;
    }

    //! Check whether a flag is set.
    constexpr bool Has(const Enum flag) const noexcept {
        return (flags_ & std::to_underlying(flag)) != 0;
    }

    //! Check whether all specific flags are set.
    constexpr bool HasAll(const EnumFlags flags) const noexcept {
        return (flags_ & static_cast<RawType>(flags)) == static_cast<RawType>(flags);
    }

    //! Check whether at least one of the specific flags is set.
    constexpr bool HasAny(const EnumFlags flags) const noexcept {
        return (flags_ & static_cast<RawType>(flags)) != 0;
    }

    //! Check whether any flags are set.
    constexpr bool HasAny() const noexcept {
        return flags_ != 0;
    }

    //! Same as @ref Has.
    constexpr bool operator&(const Enum flag) const noexcept {
        return Has(flag);
    }

    //! Same as @ref HasAll.
    constexpr bool operator&(const EnumFlags flags) const noexcept {
        return HasAll(flags);
    }

    //! Reset the current flags to specific flags.
    constexpr EnumFlags& operator&=(const EnumFlags flags) noexcept {
        flags_ = flags;
        return *this;
    }

    //! Create a new @p EnumFlags combining the current flags with specific flags.
    constexpr EnumFlags operator|(const EnumFlags flags) const noexcept {
        return EnumFlags {*this}.Add(flags);
    }

    //! Same as @ref Add.
    constexpr EnumFlags& operator|=(const EnumFlags flags) noexcept {
        return Add(flags);
    }

    //! Get the underlying value.
    constexpr operator RawType() const noexcept {
        return flags_;
    }

    constexpr void swap(EnumFlags& flags) noexcept {
        std::ranges::swap(flags_, flags.flags_);
    }

    constexpr bool operator==(const EnumFlags&) const noexcept = default;

private:
    template <std::ranges::range Flags>
        requires std::same_as<Enum, std::ranges::range_value_t<Flags>>
    constexpr void AddFlags(const Flags& flags) noexcept {
        std::ranges::for_each(flags, [this](const auto flag) noexcept { Add(flag); });
    }

    RawType flags_ {0};
};

template <typename Enum>
constexpr void swap(EnumFlags<Enum>& lhs, EnumFlags<Enum>& rhs) noexcept {
    lhs.swap(rhs);
}