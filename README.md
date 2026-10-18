# XVG

A custom VGA driver compatible with x86 kernels, along with a simple graphics library (SGL) designed to provide basic drawing capabilities on VGA-compatible displays. The driver interfaces directly with VGA hardware, providing low-level access to manipulate the display buffer and control the screen output. The graphics library built on top of the driver offers functions for drawing basic shapes and handling colors.

<img src="img/qemu.png" alt="image" width="700" height="auto">

## Features
+ Low-level VGA driver for interfacing with VGA-compatible displays.
+ Double buffering for reducing flickering and screen tearing.
+ Support for setting display modes, manipulating display buffer, and controlling screen output.
+ Simple graphics library built on top of the VGA driver.
+ Drawing functions for basic shapes (lines, rectangles, circles and triangles).

## Prerequisites
- **Development Environment:** Set up a development environment with a compatible toolchain, such as NASM for assembly and GCC for C code.

Example Usage:
```c
#include "vga.h"
#include "sgl.h"

void main() {
    xv_init_gfx_mode(MODE13H);

    sgl_clear(BLACK);

    sgl_draw_rect(150, 10, 100, 50, GREEN);

    sgl_draw_line(120, 14, 24, 30, RED);

    sgl_draw_circle(100,100,50, CYAN);

    sgl_draw_filled_tri(100, 100, 50, 150, 120, 120, MAGENTA);

    sgl_draw_tri(200, 100, 150, 170, 250, 190, YELLOW);
    
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 100; j++) {
            sgl_put_pixel(i, 50 + j, i);
        }
    }

    sgl_swap_buffers();
}
```
## License

This project is licensed under the MIT License. See the LICENSE file for details.

