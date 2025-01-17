{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc
    gdb
    lemon-graph
    pkg-config
    gnumake
  ];
}

