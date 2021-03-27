# Monad CXX

## build

### CMake

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```

Build additional tests with the cmake parameters `-DDBUILD_TESTS=ON`.

### Nix

In order to just compile the tests and build the package, run:

```sh
$ nix-build
$ # see result/
```

For the incremental workflow, run:

```sh
$ nix-shell
$ mkdir build
$ cd build
$ cmake -DBUILD_TESTS=ON -G Ninja ..
$ ninja # or make
$ ctest
```

nix installation: https://nixos.org/download.html
