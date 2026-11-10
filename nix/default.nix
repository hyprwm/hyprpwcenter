{
  lib,
  stdenv,
  cmake,
  pkg-config,
  hyprtoolkit,
  pipewire,
  version ? "git",
}:
stdenv.mkDerivation {
  pname = "hyprpwcenter";
  inherit version;

  src = ../.;

  nativeBuildInputs = [
    cmake
    pkg-config
  ];

  buildInputs = [
    hyprtoolkit
    pipewire
  ];

  meta = {
    homepage = "https://github.com/hyprwm/hyprpwcenter";
    description = "A GUI Pipewire control center";
    license = lib.licenses.bsd3;
    platforms = lib.platforms.linux;
    mainProgram = "hyprpwcenter";
  };
}
