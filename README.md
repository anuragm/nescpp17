# nescpp17

NES Emulator written in C++.

### Requires

On MacOS install [Homebrew](https://brew.sh/) and then

```sh
brew install cmake
brew install clang-format@11
```

### Compilation

Change to `build` directory and use `cmake` to compile

```sh
cd build
cmake ..
make
```

### Run

The main entry point is `nesemu` binary. After compilation, the binary is found
in the `build` directory.

```
cd build
./nesemu
```

### Linting

Run

```sh
make lint
```
