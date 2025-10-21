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
    homepage = "https://github.com/hyprwm/hyprwire";
    description = "A fast and consistent wire protocol for IPC";
    license = lib.licenses.bsd3;
    platforms = lib.platforms.linux;
  };
}
