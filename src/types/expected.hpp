#pragma once

#include <expected>

template<typename R, typename E>
using Expected = std::expected<R, E>;

template<typename E>
using Unexpected = std::unexpected<E>;
