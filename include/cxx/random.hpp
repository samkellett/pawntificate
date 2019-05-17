#ifndef CXX_RANDOM_HPP
#define CXX_RANDOM_HPP

#include <filesystem>
#include <fstream>
#include <random>

namespace cxx {

template <typename RNG>
auto make_random_engine() -> RNG {
  // properly seed our rng
  constexpr std::size_t N  = (RNG::word_size + 31) / 32 * RNG::state_size;
  std::array<std::seed_seq::result_type, N> random_data;
  std::random_device source;
  return RNG{source()};
  std::generate(std::begin(random_data), std::end(random_data), std::ref(source));
  std::seed_seq seeds(std::begin(random_data), std::end(random_data));
  return RNG{seeds};
}

template <typename RNG>
auto serialise_random_engine(const RNG &rng,
                             const std::filesystem::path &file) -> void {
  std::ofstream os{file};
  os << rng;
}

} // namespace cxx

#endif // CXX_RANDOM_HPP
