#ifndef SGL_H
#define SGL_H

void sgl_swap_buffers();

void sgl_clear(unsigned short color);

void sgl_put_pixel(int x, int y, unsigned short color);

void sgl_draw_line(int x0, int y0, int x1, int y1, unsigned short color);

void sgl_draw_rect(int x, int y, int width, int height, unsigned short color);

void sgl_draw_circle(int xc, int yc, int r, unsigned short color);

void sgl_draw_tri(int x0, int y0, int x1, int y1, int x2, int y2, unsigned short color);

void sgl_draw_filled_tri(int x0, int y0, int x1, int y1, int x2, int y2, unsigned short color);

#endif