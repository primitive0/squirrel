#pragma once

#include <iterator>

template<typename It>
concept InputIterator = std::input_iterator<It>;

template<typename It>
concept ForwardIterator = std::forward_iterator<It>;

template<typename Sentinel, typename It>
concept SentinelFor = std::sentinel_for<Sentinel, It>;
