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
        prep-build = (
          pkgs.writeShellScriptBin "prep-build" ''
            if [ ! -d ./build ]; then
                mkdir build
                cd build
                cmake ..
                cd ..
            fi
          ''
        );
        build = (
          pkgs.writeShellScriptBin "build" ''
            ${prep-build}/bin/prep-build

            cmake --build build --parallel $(nproc)
          ''
        );
        clean = (
          pkgs.writeShellScriptBin "clean" ''
            if [ -d ./build ]; then
                rm -fr build
            fi
          ''
        );
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            libgcc
            cmake

            prep-build
            build
            clean

            # For checking the encoded files
            xxd
          ];
        };
      }
    );
}
