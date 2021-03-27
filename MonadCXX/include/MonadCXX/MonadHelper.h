// 2021 by Stephan Roslen

#ifndef __MONADCXX_MONAD_HELPER_H__
#define __MONADCXX_MONAD_HELPER_H__

namespace MonadCXX::detail {

template<typename T, typename To>
concept DecaysTo = std::is_same_v<std::decay_t<T>, To>;

template<typename T>
concept IsMonadBase = requires() {
  typename T::template TypeConstructor<int>;
  {
    T::mreturn(std::declval<int>())
  }
  ->std::same_as<typename T::template TypeConstructor<int>>;
  {
    T::mbind(T::mreturn(std::declval<int>()), [](int a) { return T::mreturn(a); })
  }
  ->std::same_as<typename T::template TypeConstructor<int>>;
  requires std::same_as<int, typename T::template InverseTypeConstructor<typename T::template TypeConstructor<int>>>;
};

template<typename MonadBase, typename TPA, typename Morphism>
using MBindResultType =
    std::invoke_result_t<Morphism, typename MonadBase::template InverseTypeConstructor<std::decay_t<TPA>>>;

template<typename MonadBase, typename A>
using MReturnResultType = typename MonadBase::template TypeConstructor<std::decay_t<A>>;

template<IsMonadBase MonadBase>
struct Monad : MonadBase {
  template<typename MMA>
  static typename MonadBase::template InverseTypeConstructor<std::decay_t<MMA>> mjoin(MMA&& mma) {
    return MonadBase::mbind(std::forward<MMA>(mma), []<typename A>(A&& a) { return std::forward<A>(a); });
  }

  template<typename MorphismA, typename MorphismB>
  static auto mkleisli(MorphismA&& morphisma_, MorphismB&& morphismb_) {
    return [morphisma = std::forward<MorphismA>(morphisma_),
            morphismb = std::forward<MorphismB>(morphismb_)]<typename A>(A&& a) {
      return MonadBase::mbind(std::move(morphisma)(std::forward<A>(a)), std::move(morphismb));
    };
  }

  template<typename MorphismF>
  static auto mfmap(MorphismF&& morphismf_0) {
    return [morphismf_1 = std::forward<MorphismF>(morphismf_0)]<typename MA>(MA&& ma) {
      return MonadBase::mbind(std::forward<MA>(ma), [morphismf = std::move(morphismf_1)]<typename T>(T&& a) {
        return MonadBase::mreturn(std::move(morphismf)(std::forward<T>(a)));
      });
    };
  }
};

} // namespace MonadCXX::detail

#endif