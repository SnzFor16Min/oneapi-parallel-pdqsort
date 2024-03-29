﻿#ifndef _PDQ_UTIL
#define _PDQ_UTIL

#include <iostream>
#include <format>
#include <ranges>
#include <random>
#include <numeric>

namespace util
{
namespace rng = std::ranges;
namespace sv = rng::views;

template <rng::range R>
void print(R const& r) {
  // display only first n and last n
  size_t constexpr n = 10;
  size_t const len = rng::size(r);
  std::cout << "[ ";
  if (len > 2 * n) {
    for (auto const& e : r | sv::take(n))
      std::cout << e << ' ';
    std::cout << std::format("..<{} items>.. ", len - 2 * n);
    for (auto const& e : r | sv::reverse | sv::take(n) | sv::reverse)
      std::cout << e << ' ';
  } else {
    for (auto const& e : r)
      std::cout << e << ' ';
  }
  std::cout << "]" << std::endl;
}

template <class T>
struct stat_result {
  T min, max;
  double avg;
};

template <rng::range R, class T = rng::range_value_t<R>>
constexpr stat_result<T> stat(R const& r) {
  auto const minmax = rng::minmax_element(r);
  double const avg = std::accumulate(rng::cbegin(r), rng::cend(r), 0.0) / rng::size(r);
  return { .min = *minmax.min, .max = *minmax.max, .avg = avg };
}

enum class GenMode { Random, Sorted, RevSorted };

template <class T = size_t>
auto generate_vec(size_t const len, GenMode const mode = GenMode::Random) {
  std::vector<T> v(len);
  switch (mode) {
    case GenMode::Random: {
      auto dist = std::uniform_int_distribution<T>(1, len);
      auto gen = std::mt19937{ std::random_device{}() };
      rng::generate(v, [&] { return dist(gen); });
      break;
    }
    case GenMode::Sorted: {
      rng::iota(v, 1);
      break;
    }
    case GenMode::RevSorted: {
      rng::iota(sv::reverse(v), 1);
      break;
    }
  }
  return v;
}

}  // namespace util

#endif