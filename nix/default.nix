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

  installPhase = ''
    runHook preInstall

    mkdir -p $out/bin $out/share/applications

    cp hyprpwcenter $out/bin

    cat <<INI > $out/share/applications/Hyprpwcenter.desktop
    [Desktop Entry]
    Name=Hyprpwcenter
    Comment=A GUI Pipewire control center 
    Exec=hyprpwcenter
    Icon=audio-speakers
    Terminal=false
    Type=Application
    Categories=AudioVideo;Audio;Mixer;Settings;
    Keywords=hyprpwcenter;Pipewire;Microphone;Volume;Fade;Balance;Headset;Speakers;Headphones;Audio;Mixer;Output;Input;Devices;Playback;Recording;System Sounds;Sound Card;Settings;Preferences;
    INI
    runHook postInstall    
  '';

  meta = {
    homepage = "https://github.com/hyprwm/hyprpwcenter";
    description = "A GUI Pipewire control center";
    license = lib.licenses.bsd3;
    platforms = lib.platforms.linux;
    mainProgram = "hyprpwcenter";
  };
}
