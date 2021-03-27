// 2021 by Stephan Roslen

#ifndef __MONADCXX_MAYBE_MONAD_H__
#define __MONADCXX_MAYBE_MONAD_H__

#include <optional>
#include <type_traits>

#include "MonadHelper.h"

namespace MonadCXX::detail {

struct MaybeMonadBase {

  template<typename T>
  struct TypeConstructor {
    using BaseType = T;

    template<DecaysTo<T> T_>
    explicit constexpr TypeConstructor(T_&& t_) noexcept(
        std::is_nothrow_constructible_v<std::optional<T>, T_>)
        : data(std::forward<T_>(t_)) {}
    TypeConstructor(std::nullopt_t) noexcept {};

    std::optional<T> data{std::nullopt};

    template<typename Morphism>
    auto mbind(Morphism&& morphism) const& {
      return MaybeMonadBase::mbind(*this, std::forward<Morphism>(morphism));
    }

    template<typename Morphism>
    auto mbind(Morphism&& morphism) && {
      return MaybeMonadBase::mbind(std::move(*this), std::forward<Morphism>(morphism));
    }
  };

  template<typename T>
  using InverseTypeConstructor = typename T::BaseType;

  template<typename T>
  static constexpr TypeConstructor<T> nothing() noexcept {
    return TypeConstructor<T>{std::nullopt};
  }

  template<typename MA, typename Morphism>
  static constexpr MBindResultType<MaybeMonadBase, MA, Morphism> mbind(MA&& ma, Morphism&& morphism) noexcept(
      noexcept(morphism(*ma.data))) {
    if (!ma.data) {
      return nothing<InverseTypeConstructor<MBindResultType<MaybeMonadBase, MA, Morphism>>>();
    }
    return morphism(*ma.data);
  }

  template<typename A>
  static constexpr MReturnResultType<MaybeMonadBase, A> mreturn(A&& a) noexcept(
      std::is_nothrow_constructible_v<std::decay_t<A>, A>) {
    return MReturnResultType<MaybeMonadBase, A>{std::forward<A>(a)};
  }
};

} // namespace MonadCXX::detail

namespace MonadCXX {

using MaybeMonad = detail::Monad<detail::MaybeMonadBase>;

}

#endif