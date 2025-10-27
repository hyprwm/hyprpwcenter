{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    systems.url = "github:nix-systems/default-linux";

    aquamarine = {
      url = "github:hyprwm/aquamarine";
      inputs = {
        nixpkgs.follows = "nixpkgs";
        systems.follows = "systems";
        hyprutils.follows = "hyprutils";
      };
    };

    hyprgraphics = {
      url = "github:hyprwm/hyprgraphics";
      inputs = {
        hyprutils.follows = "hyprutils";
        nixpkgs.follows = "nixpkgs";
        systems.follows = "systems";
      };
    };

    hyprtoolkit = {
      url = "github:hyprwm/hyprtoolkit";
      inputs = {
        nixpkgs.follows = "nixpkgs";
        systems.follows = "systems";
        aquamarine.follows = "aquamarine";
        hyprgraphics.follows = "hyprgraphics";
        hyprutils.follows = "hyprutils";
      };
    };

    hyprutils = {
      url = "github:hyprwm/hyprutils";
      inputs = {
        nixpkgs.follows = "nixpkgs";
        systems.follows = "systems";
      };
    };

  };

  outputs =
    inputs@{
      self,
      nixpkgs,
      systems,
      ...
    }:
    let
      inherit (nixpkgs) lib;
      eachSystem = lib.genAttrs (import systems);
      pkgsFor = eachSystem (
        system:
        import nixpkgs {
          localSystem.system = system;
          overlays = with self.overlays; [
            hyprpwcenter
          ];
        }
      );
    in
    {
      overlays = import ./nix/overlays.nix { inherit self lib inputs; };

      packages = eachSystem (system: {
        default = self.packages.${system}.hyprpwcenter;
        inherit (pkgsFor.${system}) hyprpwcenter;
      });

      checks = eachSystem (system: self.packages.${system});

      formatter = eachSystem (system: pkgsFor.${system}.alejandra);
    };
}
