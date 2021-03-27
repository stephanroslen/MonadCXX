# Monad CXX

## build

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
