{
  description = "";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    { flake-utils, nixpkgs, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            libgcc
            cmake

            (pkgs.writeShellScriptBin "build" ''
              if [ ! -d ./build ]; then
                  mkdir build
                  cd build
                  cmake ..
                  cd ..
              fi

              cmake --build build --parallel $(nproc)
            '')

            (pkgs.writeShellScriptBin "clean" ''
              if [ -d ./build ]; then
                  rm -fr build
              fi
            '')

            # For checking the encoded files
            xxd
          ];
        };
      }
    );
}
