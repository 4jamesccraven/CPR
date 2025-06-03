{
  description = "";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      flake-utils,
      nixpkgs,
      ...
    }:
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

            just

            # For checking the encoded files
            xxd
          ];
        };

        packages.default = self.packages.${system}.cpr;
        packages.cpr =
          with pkgs;
          stdenv.mkDerivation {
            name = "cpr-0.1.0";
            src = ./.;

            nativeBuildInputs = [
              cli11
              cmake
            ];

            installPhase = ''
              mkdir -p $out/bin
              cp ./cpr $out/bin

              chmod +x $out/bin/cpr
            '';
            enableParallelBuilding = true;
          };
      }
    );
}
