{
  lib,
  inputs,
  self,
}:
let
  mkDate =
    longDate:
    (lib.concatStringsSep "-" [
      (builtins.substring 0 4 longDate)
      (builtins.substring 4 2 longDate)
      (builtins.substring 6 2 longDate)
    ]);

  version = lib.removeSuffix "\n" (builtins.readFile ../VERSION);
in
{
  default = inputs.self.overlays.hyprpwcenter;

  hyprpwcenter = lib.composeManyExtensions [
    inputs.aquamarine.overlays.default
    inputs.hyprgraphics.overlays.default
    inputs.hyprtoolkit.overlays.default
    inputs.hyprutils.overlays.default
    (final: prev: {
      hyprpwcenter = prev.callPackage ./default.nix {
        stdenv = prev.gcc15Stdenv;
        version =
          version
          + "+date="
          + (mkDate (inputs.self.lastModifiedDate or "19700101"))
          + "_"
          + (inputs.self.shortRev or "dirty");
      };
    })
  ];
}
