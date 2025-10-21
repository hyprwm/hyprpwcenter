{
  lib,
  stdenv,
  cmake,
  pkg-config,
  aquamarine,
  cairo,
  hyprgraphics,
  hyprtoolkit,
  hyprutils,
  pipewire,
  libdrm,
  pixman,
  version ? "git",
  doCheck ? false,
}:
stdenv.mkDerivation {
  pname = "hyprpwcenter";
  inherit version doCheck;

  src = ../.;

  nativeBuildInputs = [
    cmake
    pkg-config
  ];

  buildInputs = [
    aquamarine
    cairo
    libdrm
    hyprgraphics
    hyprtoolkit
    hyprutils
    pipewire
    pixman
  ];

  meta = {
    homepage = "https://github.com/hyprwm/hyprpwcenter";
    description = "A GUI Pipewire control center";
    license = lib.licenses.bsd3;
    platforms = lib.platforms.linux;
    mainProgram = "hyprpwcenter";
  };
}
