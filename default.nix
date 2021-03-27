with import <nixpkgs> {};

gcc10Stdenv.mkDerivation rec {
  name = "monadcxx";
  src = ./.;

  nativeBuildInputs = with pkgs; [ cmake ninja ];

  checkInputs = with pkgs; [ gtest ];
  doCheck = true;

  cmakeFlags = lib.optional doCheck [ "-DBUILD_TESTS=ON" ];
}
