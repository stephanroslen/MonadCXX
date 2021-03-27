// 2021 by Stephan Roslen

#ifndef __MONADCXX_STATE_MONAD_H__
#define __MONADCXX_STATE_MONAD_H__

#include <functional>
#include <type_traits>

#include "MonadHelper.h"

namespace MonadCXX::detail {

template<typename Result, typename StateDesc>
struct ResultStateDesc {
  Result result;
  StateDesc stateDesc;
};

template<typename StateDesc>
struct StateMonadBase {
  template<typename Result>
  using ResultStateDescType = ResultStateDesc<Result, StateDesc>;

  template<typename Result>
  struct TypeConstructor {
    using BaseType = Result;

    template<std::convertible_to<std::function<ResultStateDescType<Result>(StateDesc)>> T_>
    explicit constexpr TypeConstructor(T_&& t_) noexcept(
        std::is_nothrow_constructible_v<std::function<ResultStateDescType<Result>(StateDesc)>, T_&&>)
        : data{std::forward<T_>(t_)} {}

    std::function<ResultStateDescType<Result>(StateDesc)> data;

    template<typename Morphism>
    auto mbind(Morphism&& morphism) const& {
      return StateMonadBase::mbind(*this, std::forward<Morphism>(morphism));
    }

    template<typename Morphism>
    auto mbind(Morphism&& morphism) && {
      return StateMonadBase::mbind(std::move(*this), std::forward<Morphism>(morphism));
    }
  };

  template<typename T>
  using InverseTypeConstructor = typename T::BaseType;

  template<typename MA, typename Morphism>
  static MBindResultType<StateMonadBase, MA, Morphism> mbind(MA&& ma_, Morphism&& morphism_) {
    return MBindResultType<StateMonadBase, MA, Morphism>{[ma = std::forward<MA>(ma_), morphism = std::forward<Morphism>(morphism_)](StateDesc s) {
      auto rsd  = std::move(ma.data)(std::move(s));
      auto tmps = std::move(morphism)(std::move(rsd.result));
      return std::move(tmps.data)(std::move(rsd.stateDesc));
    }};
  }

  template<typename A>
  static MReturnResultType<StateMonadBase, A> mreturn(A&& a_) {
    return MReturnResultType<StateMonadBase, A>{[a = std::forward<A>(a_)](StateDesc s) {
      return ResultStateDesc{std::move(a), std::move(s)};
    }};
  }
};

} // namespace MonadCXX::detail

namespace MonadCXX {

template<typename StateDesc>
using StateMonad = detail::Monad<detail::StateMonadBase<StateDesc>>;

}

#endif