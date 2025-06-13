#include "enum_flags/enum_flags.h"

#include <gtest/gtest.h>

#include <list>
#include <unordered_set>
#include <vector>

namespace {

enum class Opt : unsigned int {
    A = EnumFlags<Opt>::CreateFlag(0),
    B = EnumFlags<Opt>::CreateFlag(1),
    C = EnumFlags<Opt>::CreateFlag(2),
    D = EnumFlags<Opt>::CreateFlag(3),
    E = EnumFlags<Opt>::CreateFlag(4)
};

template <typename T>
class TypedEnumFlags : public testing::Test {};

using FlagContainers = testing::Types<std::unordered_set<Opt>, std::initializer_list<Opt>,
                                      std::vector<Opt>, std::list<Opt>>;
}  // namespace

TYPED_TEST_SUITE(TypedEnumFlags, FlagContainers);

TYPED_TEST(TypedEnumFlags, Construction) {
    const TypeParam opts {Opt::A, Opt::B};
    const EnumFlags<Opt> flags {opts};
    EXPECT_TRUE(flags & Opt::A);
    EXPECT_TRUE(flags & Opt::B);
}

TEST(EnumFlags, CheckIfFlagsExist) {
    const EnumFlags<Opt> flags {Opt::A, Opt::B, Opt::C};
    EXPECT_TRUE(flags & Opt::A);
    EXPECT_TRUE(flags & Opt::B);
    EXPECT_FALSE(flags & Opt::D);

    EXPECT_TRUE(flags.HasAny({Opt::A, Opt::D}));
    EXPECT_FALSE(flags.HasAny({Opt::D, Opt::E}));

    EXPECT_TRUE(flags.HasAll({Opt::A, Opt::B}));
    EXPECT_TRUE((flags & EnumFlags<Opt> {Opt::A, Opt::B}));
    EXPECT_FALSE(flags.HasAll({Opt::C, Opt::D}));
    EXPECT_FALSE((flags & EnumFlags<Opt> {Opt::C, Opt::D}));
}

TEST(EnumFlags, Assignment) {
    EnumFlags<Opt> flags {Opt::A, Opt::B};
    EXPECT_TRUE(flags & Opt::A);
    EXPECT_TRUE(flags & Opt::B);
    EXPECT_FALSE(flags & Opt::C);
    EXPECT_FALSE(flags & Opt::D);

    flags &= {Opt::C, Opt::D};
    EXPECT_FALSE(flags & Opt::A);
    EXPECT_FALSE(flags & Opt::B);
    EXPECT_TRUE(flags & Opt::C);
    EXPECT_TRUE(flags & Opt::D);
}

TEST(EnumFlags, AddFlags) {
    EnumFlags<Opt> flags;

    EXPECT_FALSE(flags & Opt::A);
    flags |= Opt::A;
    EXPECT_TRUE(flags & Opt::A);

    EXPECT_FALSE(flags & Opt::B);
    EXPECT_FALSE(flags & Opt::C);
    flags |= {Opt::B, Opt::C};
    EXPECT_TRUE(flags & Opt::B);
    EXPECT_TRUE(flags & Opt::C);

    const auto new_flags {flags | Opt::D};
    EXPECT_TRUE(new_flags & Opt::D);
    EXPECT_FALSE(flags & Opt::D);
}

TEST(EnumFlags, ClearFlags) {
    EnumFlags<Opt> flags {Opt::A, Opt::B};
    EXPECT_TRUE(flags & Opt::A);
    EXPECT_TRUE(flags & Opt::B);
    EXPECT_TRUE(flags.HasAny());

    flags.Clear();
    EXPECT_FALSE(flags & Opt::A);
    EXPECT_FALSE(flags & Opt::B);
    EXPECT_FALSE(flags.HasAny());
}

TEST(EnumFlags, RemoveFlags) {
    EnumFlags<Opt> flags {Opt::A, Opt::B, Opt::C};

    EXPECT_TRUE(flags & Opt::C);
    flags.Remove(Opt::C);
    EXPECT_FALSE(flags & Opt::C);

    EXPECT_TRUE(flags & Opt::A);
    EXPECT_TRUE(flags & Opt::B);
    flags.Remove({Opt::A, Opt::B});
    EXPECT_FALSE(flags & Opt::A);
    EXPECT_FALSE(flags & Opt::B);
}

TEST(EnumFlags, SwapFlags) {
    EnumFlags<Opt> flags1 {Opt::A, Opt::B};
    EXPECT_TRUE(flags1 & Opt::A);
    EXPECT_TRUE(flags1 & Opt::B);
    EXPECT_FALSE(flags1 & Opt::C);
    EXPECT_FALSE(flags1 & Opt::D);

    EnumFlags<Opt> flags2 {Opt::C, Opt::D};
    EXPECT_FALSE(flags2 & Opt::A);
    EXPECT_FALSE(flags2 & Opt::B);
    EXPECT_TRUE(flags2 & Opt::C);
    EXPECT_TRUE(flags2 & Opt::D);

    swap(flags1, flags2);
    EXPECT_FALSE(flags1 & Opt::A);
    EXPECT_FALSE(flags1 & Opt::B);
    EXPECT_TRUE(flags1 & Opt::C);
    EXPECT_TRUE(flags1 & Opt::D);

    EXPECT_TRUE(flags2 & Opt::A);
    EXPECT_TRUE(flags2 & Opt::B);
    EXPECT_FALSE(flags2 & Opt::C);
    EXPECT_FALSE(flags2 & Opt::D);
}

TEST(EnumFlags, Comparison) {
    EXPECT_EQ((EnumFlags<Opt> {Opt::A, Opt::B}), (EnumFlags<Opt> {Opt::A, Opt::B}));
    EXPECT_NE((EnumFlags<Opt> {Opt::A, Opt::B}), (EnumFlags<Opt> {Opt::A, Opt::C}));
}