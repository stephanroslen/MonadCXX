// 2021 by Stephan Roslen

#include <gtest/gtest.h>

#include <iostream>

#include <MonadCXX.h>

namespace {

template<typename T>
std::ostream& operator<<(std::ostream& os, const MonadCXX::MaybeMonad::TypeConstructor<T>& maybe) {
  if (!maybe.data) {
    return os << "Nothing";
  } else {
    return os << "Just " << *maybe.data;
  }
}

} // namespace

namespace MonadCXX::tests {

TEST(MaybeMonad, TypeConstructor) {
  auto m3 = MaybeMonad::mreturn(3);
  ASSERT_TRUE(m3.data);
  EXPECT_EQ(*m3.data, 3);
  auto mnothing = MaybeMonad::nothing<int>();
  EXPECT_FALSE(mnothing.data);
}

TEST(MaybeMonad, Bind) {
  auto mDouble = MaybeMonad::mreturn(3).mbind([](int i) { return MaybeMonad::mreturn(2 * i); });
  ASSERT_TRUE(mDouble.data);
  EXPECT_EQ(*mDouble.data, 6);
  auto mNothingOut = MaybeMonad::mreturn(3).mbind([](int) { return MaybeMonad::nothing<int>(); });
  ASSERT_FALSE(mNothingOut.data);
  auto mNothingIn = MaybeMonad::nothing<int>().mbind([](int i) { return MaybeMonad::mreturn(2 * i); });
  ASSERT_FALSE(mNothingIn.data);
  auto mDifferentTypes = MaybeMonad::mreturn(3).mbind([](int i) { return MaybeMonad::mreturn(std::to_string(i)); });
  ASSERT_TRUE(mDifferentTypes.data);
  EXPECT_EQ(*mDifferentTypes.data, "3");

  const auto complexLambda{[](int i) {
    if (i < 10) {
      return MaybeMonad::mreturn(std::to_string(i));
    }
    return MaybeMonad::nothing<std::string>();
  }};
  auto mComplexJust = MaybeMonad::mreturn(3).mbind(complexLambda);
  ASSERT_TRUE(mComplexJust.data);
  EXPECT_EQ(*mComplexJust.data, "3");
  auto mComplexNothing = MaybeMonad::mreturn(23).mbind(complexLambda);
  EXPECT_FALSE(mComplexNothing.data);
}

TEST(MaybeMonad, Kleisli) {
  auto combined = MaybeMonad::mkleisli([](int i) { return MaybeMonad::mreturn(i * 2); },
                                       [](int i) { return MaybeMonad::mreturn(std::to_string(i)); });
  auto mJust = MaybeMonad::mreturn(3).mbind(combined);
  ASSERT_TRUE(mJust.data);
  EXPECT_EQ(*mJust.data, "6");
  auto mNothing = MaybeMonad::nothing<int>().mbind(combined);
  EXPECT_FALSE(mNothing.data);
}

TEST(MaybeMonad, FMap) {
  auto mapped = MaybeMonad::mfmap([](int i){return std::to_string(i);});
  auto mJust = mapped(MaybeMonad::mreturn(3));
  ASSERT_TRUE(mJust.data);
  EXPECT_EQ(*mJust.data, "3");
  auto mNothing = mapped(MaybeMonad::nothing<int>());
  EXPECT_FALSE(mNothing.data);
}

TEST(MaybeMonad, Join) {
  auto mJustRejoined = MaybeMonad::mjoin(MaybeMonad::mreturn(MaybeMonad::mreturn(3)));
  ASSERT_TRUE(mJustRejoined.data);
  EXPECT_EQ(mJustRejoined.data, 3);
  auto mJustNothingRejoined = MaybeMonad::mjoin(MaybeMonad::mreturn(MaybeMonad::nothing<int>()));
  ASSERT_FALSE(mJustNothingRejoined.data);
  auto mNothingRejoined = MaybeMonad::mjoin(MaybeMonad::nothing<MaybeMonad::TypeConstructor<int>>());
  ASSERT_FALSE(mNothingRejoined.data);
}

} // namespace MonadCXX::tests