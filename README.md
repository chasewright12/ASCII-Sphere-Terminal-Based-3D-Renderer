## Introduction

A terminal-based 3D renderer written in C that projects a rotating sphere using ASCII characters. Inspired by the classic "donut.c" shading technique, this project maps 3D spherical coordinates onto a 2D character grid, using a z-buffer for depth sorting and a luminance gradient (`.,-~:;=!*#$@`) to simulate shading based on a simple lighting model.

### Project Core

- Real-time rotation on two axes (X and Y)
- Perspective projection with z-buffering for correct depth occlusion
- ASCII luminance shading to simulate light and shadow
- Single-frame buffering (built as one string per frame) to avoid flickering

### Requirements

- GCC (or any C compiler with `math.h` support)
- A Unix-like terminal (Linux, macOS, or WSL) — uses ANSI escape codes for screen clearing

### Build & Run
```bash
gcc main.c -o sphere -lm
./sphere
```
<div align="center">
Press 'Ctrl+C' to stop the animation.
</div>

### How it Works?

1. Points are generated on the sphere's surface using spherical coordinates (`theta`, `phi`).
2. Two rotation matrices (X-axis and Y-axis) animate the sphere over time.
3. Points are projected from 3D to 2D using a simple perspective transform (`1/z`).
4. A z-buffer ensures only the closest point per screen position is drawn, hiding the far side of the sphere.
5. A luminance value (derived from surface angle relative to a fixed light direction) selects the ASCII character used to render each point.

### Images
<div align="center">
<img src="./photos/Captura de tela 2026-09-02 025654.png">
<legend>Figure.1 - ASCII Setup Image</legend>
</div>

### Langs

<div align="center">
    <table border="1">
    <td width="350px">
    <strong>
    C
    </strong>
    <br>
    ASCII 3D Projection written in C.
    <td>
    <img src="https://img.icons8.com/?size=100&id=111021&format=png&color=000000">
    </td>
    </td>
</div>
