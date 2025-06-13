# Scoped Enumeration Bit Flags

![C++](docs/badges/C++.svg)
[![CMake](docs/badges/Made-with-CMake.svg)](https://cmake.org)
![GitHub Actions](docs/badges/Made-with-GitHub-Actions.svg)
![License](docs/badges/License-MIT.svg)

## Introduction

A header-only type-safe bit flag manager for *C++* scoped enumerations written in *C++23*, supporting:

- Setting and clearing individual or multiple flags.
- Checking if any or multiple flags are set.
- Combining multiple flags into a single flag.

## Unit Tests

### Prerequisites

- Install *GoogleTest*.
- Install *CMake*.

### Building

Go to the project folder and run:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

### Running

Go to the `build` folder and run:

```bash
ctest -VV
```

## Examples

See more examples in `tests/enum_flags_tests.cpp`.

```c++
enum class Opt : unsigned int {
    A = EnumFlags<Opt>::CreateFlag(0),
    B = EnumFlags<Opt>::CreateFlag(1),
    C = EnumFlags<Opt>::CreateFlag(2),
    D = EnumFlags<Opt>::CreateFlag(3),
    E = EnumFlags<Opt>::CreateFlag(4)
};
```

```c++
const EnumFlags<Opt> flags {Opt::A, Opt::B, Opt::C};
EXPECT_TRUE(flags & Opt::A);
EXPECT_FALSE(flags & Opt::D);

EXPECT_TRUE(flags.HasAny({Opt::A, Opt::D}));
EXPECT_FALSE(flags.HasAny({Opt::D, Opt::E}));

EXPECT_TRUE(flags.HasAll({Opt::A, Opt::B}));
EXPECT_TRUE((flags & EnumFlags<Opt> {Opt::A, Opt::B}));
EXPECT_FALSE(flags.HasAll({Opt::C, Opt::D}));
EXPECT_FALSE((flags & EnumFlags<Opt> {Opt::C, Opt::D}));
```

```c++
EnumFlags<Opt> flags;
EXPECT_FALSE(flags & Opt::A);
flags |= Opt::A;
EXPECT_TRUE(flags & Opt::A);

const auto new_flags {flags | Opt::D};
EXPECT_TRUE(new_flags & Opt::D);
```

```c++
EnumFlags<Opt> flags {Opt::A, Opt::B, Opt::C};
EXPECT_TRUE(flags & Opt::C);
flags.Remove(Opt::C);
EXPECT_FALSE(flags & Opt::C);
```

## License

Distributed under the *MIT License*. See `LICENSE` for more information.