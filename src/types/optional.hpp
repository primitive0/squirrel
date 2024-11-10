#pragma once

#include <optional>

template<typename T>
using Optional = std::optional<T>;

inline constexpr std::nullopt_t None = std::nullopt;

using NoneType = std::nullopt_t;
