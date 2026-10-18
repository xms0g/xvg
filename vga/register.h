#ifndef REGISTER_H
#define REGISTER_H

typedef unsigned char u8;

/**
 * VGA Registers
 *
 * https://wiki.osdev.org/VGA_Hardware   
 * http://www.osdever.net/FreeVGA/vga/vga.htm
 * https://fd.lod.bz/rbil/ports/video/index.html
 */

typedef struct external_general {
    u8 misc_output;
    u8 feat_control;
    u8 input_status0;
    u8 input_status1;
} external_general;

typedef union sequencer {
    u8 m[5];
    struct {
        u8 reset;
        u8 clocking_mode;
        u8 map_mask;
        u8 char_map_select;
        u8 seq_memory_mode;
    };
} sequencer;

typedef union crt_controller {
    u8 m[25];
    struct {
        u8 horizontal_total;
        u8 end_horizontal_display;
        u8 start_horizontal_blanking;
        u8 end_horizontal_blanking;
        u8 start_horizontal_retrace;
        u8 end_horizontal_retrace;
        u8 vertical_total;
        u8 overflow;
        u8 preset_row_scan;
        u8 maximum_scanline;
        u8 cursor_start;
        u8 cursor_end;
        u8 start_address_high;
        u8 start_address_low;
        u8 cursor_location_high;
        u8 cursor_location_low;
        u8 vertical_retrace_start;
        u8 vertical_retrace_end;
        u8 vertical_display_end;
        u8 offset;
        u8 underline_location;
        u8 start_vertical_blanking;
        u8 end_vertical_blanking;
        u8 crtc_mode_control;
        u8 line_compare;
    };
} crt_controller;

typedef union gfx_controller {
    u8 m[9];
    struct {
        u8 set_reset;
        u8 enable_set_reset;
        u8 color_compare;
        u8 data_rotate;
        u8 read_map_select;
        u8 gfx_mode;
        u8 misc_graphics;
        u8 color_dont_care;
        u8 bit_mask;
    }; 
} gfx_controller;

typedef union attribute_controller {
    u8 m[21];
    struct {
        u8 palette[16];
        u8 attr_mode_control;
        u8 overscan_color;
        u8 color_plane_enable;
        u8 horizontal_pixel_panning;
        u8 color_select;
    };
} attribute_controller;

#endif 