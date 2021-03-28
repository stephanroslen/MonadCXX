// 2021 by Stephan Roslen

#include <gtest/gtest.h>

#include <string>

#include <MonadCXX.h>

namespace {

struct SomeStateDesc {
  std::string name;
  int age;

  bool operator==(const SomeStateDesc& rhs) const { return std::tie(name, age) == std::tie(rhs.name, rhs.age); }
  bool operator!=(const SomeStateDesc& rhs) const { return !(rhs == *this); }
};

struct Empty {};

using SomeState = MonadCXX::StateMonad<SomeStateDesc>;

} // namespace

namespace MonadCXX::tests {

TEST(StateMonad, Basic) {
  const SomeStateDesc in{"Whatever", 23};
  EXPECT_EQ(SomeState::mreturn(Empty{}).data(in).stateDesc, in);
}

TEST(StateMonad, Bind) {
  const SomeStateDesc in{"Whatever", 23};
  const SomeStateDesc expected{"Whatever Whatever", 46};
  const auto m{SomeState::mreturn(Empty{}).mbind([](Empty) {
    return SomeState::TypeConstructor<Empty>{[](SomeStateDesc val) {
      val.name = val.name + " " + val.name;
      val.age *= 2;
      return SomeState::ResultStateDescType<Empty>{{}, std::move(val)};
    }};
  })};
  EXPECT_EQ(m.data(in).stateDesc, expected);
}

TEST(StateMonad, Join) {
  const SomeStateDesc in{"Whatever", 23};
  const auto mma{SomeState::mreturn(SomeState::mreturn(Empty{}))};
  const auto mrejoined = SomeState::mjoin(mma);
  EXPECT_EQ(mrejoined.data(in).stateDesc, in);
}

TEST(StateMonad, FMap) {
  const SomeStateDesc in{};
  const auto mout{SomeState::mfmap([](int i) { return std::to_string(i); })(SomeState::mreturn(3)).data(in)};
  EXPECT_EQ(mout.result, "3");
  EXPECT_EQ(mout.stateDesc, in);
}

TEST(StateMonad, Kleisli) {
  const SomeStateDesc in{"Whatever", 23};
  const auto combined{SomeState::mkleisli(
      [](Empty) {
        return SomeState::TypeConstructor<int>{[](SomeStateDesc val) {
          int age = val.age;
          return SomeState::ResultStateDescType<int>{age, std::move(val)};
        }};
      },
      [](int a) {
        return SomeState::TypeConstructor<Empty>{[a](SomeStateDesc val) {
          val.name = std::to_string(a);
          return SomeState::ResultStateDescType<Empty>{Empty{}, std::move(val)};
        }};
      })};
  const SomeStateDesc expected{"23", 23};
  EXPECT_EQ(SomeState::mreturn(Empty{}).mbind(combined).data(in).stateDesc, expected);
}

} // namespace MonadCXX::tests