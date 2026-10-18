#ifndef VGA_H
#define VGA_H

enum GfxMode {
    MODE13H,
    MODE12H,
    MODE03H
};

enum COLORS {
    BLACK,          /* dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,       /* light colors */
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

void xv_init_gfx_mode(int mode);

void xv_plot_pixel(int x, int y, unsigned short color);

void xv_plot_pixelf(int x, int y, unsigned short color);

void xv_clr_screen(unsigned short color);

void xv_clr_offscreen(unsigned short color);

void xv_wait_vretrace();

void xv_swap_buffers();

#endif