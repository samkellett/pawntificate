#ifndef CXX_MAKE_ARRAY_HPP
#define CXX_MAKE_ARRAY_HPP

#include <array>
#include <type_traits>

namespace cxx {
namespace detail {

template <typename>
struct is_ref_wrapper : std::false_type {};

template <typename T>
struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

template<typename T>
using not_ref_wrapper = std::negation<is_ref_wrapper<std::decay_t<T>>>;

template <typename D, typename...>
struct return_type_helper { using type = D; };

template <typename... Types>
struct return_type_helper<void, Types...> : std::common_type<Types...> {
  static_assert(std::conjunction_v<not_ref_wrapper<Types>...>,
                "Types cannot contain reference_wrappers when D is void");
};

template <typename D, typename... Types>
using return_type_helper_t = typename return_type_helper<D, Types...>::type;

template <typename D, typename... Types>
using return_type = std::array<return_type_helper_t<D, Types...>, sizeof...(Types)>;

} // namespace detail

template <typename D = void, typename... Types>
constexpr detail::return_type<D, Types...> make_array(Types&&... t) {
  return {std::forward<Types>(t)...};
}

} // namespace cxx

#endif // CXX_MAKE_ARRAY_HPP
