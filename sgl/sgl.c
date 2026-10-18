#include "sgl.h"
#include "vga.h"

#define abs(x) (x < 0 ? 0 - x : x)

#define ceil(x) ((int)x + 1)

#define floor(x) (int)x

#define signbit(x) (x < 0.0 ? 1 : 0)

#define round(x) signbit(x) ? ceil(x - 0.5) : floor(x + 0.5)

#define swap(x, y) { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); }

/********************************
 *      Private Functions       *
 ********************************/

static void circle(int xc, int yc, int x, int y, unsigned short color);

static void sgl_fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned short color);

static void sgl_fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned short color);

/********************************
 *      Public SGL API          *
 ********************************/

void sgl_swap_buffers() {
    xv_swap_buffers();
}

void sgl_clear(unsigned short color) {
    xv_clr_offscreen(color);
}

void sgl_put_pixel(int x, int y, unsigned short color) {
    xv_plot_pixelf(x, y, color);
}

void sgl_draw_line(int x0, int y0, int x1, int y1, unsigned short color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int step = (abs(dx) >= abs(dy)) ? abs(dx) : abs(dy);

    float xinc = dx / (float)(step);
    float yinc = dy / (float)(step);

    float x = x0;
    float y = y0;

    for (int i = 0; i < step; ++i) {
        sgl_put_pixel(round(x), round(y), color);
        
        x += xinc;
        y += yinc;
    }
}

void sgl_draw_rect(int x, int y, int width, int height, unsigned short color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            sgl_put_pixel(x+i, y+j, color);
        }
    }
}

void sgl_draw_circle(int xc, int yc, int r, unsigned short color) {
    int x = 0, y = r; 
    int d = 3 - 2 * r;

    circle(xc, yc, x, y, color); 
    
    while (y >= x) { 
        // for each pixel we will 
        // draw all eight pixels 
          
        x++; 
  
        // check for decision parameter 
        // and correspondingly  
        // update d, x, y 
        if (d > 0) { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } else {
            d = d + 4 * x + 6;
        }
             
        circle(xc, yc, x, y, color);
    }
}

void sgl_draw_tri(int x0, int y0, int x1, int y1, int x2, int y2, unsigned short color) {
    sgl_draw_line(x0, y0, x1, y1, color);
    sgl_draw_line(x1, y1, x2, y2, color);
    sgl_draw_line(x2, y2, x0, y0, color);
}

/**
* Draw a filled triangle with the flat-top/flat-bottom method
* We split the original triangle in two, half flat-bottom and half flat-top
*
*
*          (x0,y0)
*            / \
*           /   \
*          /     \
*         /       \
*        /         \
*   (x1,y1)------(Mx,My)
*       \_           \
*          \_         \
*             \_       \
*                \_     \
*                   \    \
*                     \_  \
*                        \_\
*                           \
*                         (x2,y2)
*
*/
void sgl_draw_filled_tri(int x0, int y0, int x1, int y1, int x2, int y2, unsigned short color) {
    if (y0 > y1) {
        swap(y0, y1);
        swap(x0, x1);
    }

    if (y1 > y2) {
        swap(y1, y2);
        swap(x1, x2);
    }

    if (y0 > y1) {
        swap(y0, y1);
        swap(x0, x1);
    }

    if (y1 == y2) {
        // Draw flat-bottom triangle
        sgl_fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1) {
        // Draw flat-top triangle
        sgl_fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    } else {
        // Calculate the new vertex (Mx,My) using triangle similarity
        int My = y1;
        int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

        // Draw flat-bottom triangle
        sgl_fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);

        // Draw flat-top triangle
        sgl_fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
    }
}

/**
* Draw a filled a triangle with a flat bottom
*
*
*        (x0,y0)
*          / \
*         /   \
*        /     \
*       /       \
*      /         \
*  (x1,y1)------(x2,y2)
*
*/
void sgl_fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned short color) {
    // Inverse slope because we're calculating the x changes with respect to y
    float inv_slope1 = (float)(x1 - x0) / (y1 - y0);
    float inv_slope2 = (float)(x2 - x0) / (y2 - y0);

    // Start x_start and x_end from the top vertex (x0,y0)
    float xstart = x0;
    float xend = x0;

    // Loop all the scanlines from top to bottom
    for (int y = y0; y <= y2; ++y) {
        sgl_draw_line(xstart, y, xend, y, color);
        xstart += inv_slope1;
        xend += inv_slope2;
    }
}


/**
* Draw a filled a triangle with a flat top
*
*
*  (x0,y0)------(x1,y1)
*      \         /
*       \       /
*        \     /
*         \   /
*          \ /
*        (x2,y2)
*
*/
void sgl_fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, unsigned short color) {
    float inv_slope1 = (float)(x2 - x0) / (y2 - y0);
    float inv_slope2 = (float)(x2 - x1) / (y2 - y1);

    // Start x_start and x_end from the top vertex (x0,y0)
    float xstart = x2;
    float xend = x2;

    // Loop all the scanlines from top to bottom
    for (int y = y2; y >= y0; --y) {
        sgl_draw_line(xstart, y, xend, y, color);
        xstart -= inv_slope1;
        xend -= inv_slope2;
    }
}

void circle(int xc, int yc, int x, int y, unsigned short color) {
    sgl_put_pixel(xc+x, yc+y, color); 
    sgl_put_pixel(xc-x, yc+y, color); 
    sgl_put_pixel(xc+x, yc-y, color); 
    sgl_put_pixel(xc-x, yc-y, color); 
    sgl_put_pixel(xc+y, yc+x, color); 
    sgl_put_pixel(xc-y, yc+x, color); 
    sgl_put_pixel(xc+y, yc-x, color); 
    sgl_put_pixel(xc-y, yc-x, color); 
}