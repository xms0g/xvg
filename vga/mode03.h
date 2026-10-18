#ifndef MODE03_H
#define MODE03_H

#include "register.h"

static const external_general extRegs03 = {
    .misc_output = 0x67
};

static const sequencer seqRegs03 = {
    .reset = 0x03,
    .clocking_mode = 0x00,
    .map_mask = 0x03,
    .char_map_select = 0x00,
    .seq_memory_mode = 0x02
};

static crt_controller crtcRegs03 = {
    .horizontal_total = 0x5F,
    .end_horizontal_display = 0x4F,
    .start_horizontal_blanking = 0x50,
    .end_horizontal_blanking = 0x82,
    .start_horizontal_retrace = 0x55,
    .end_horizontal_retrace = 0x81,
    .vertical_total = 0xBF,
    .overflow = 0x1F,
    .preset_row_scan = 0x00,
    .maximum_scanline = 0x4F,
    .cursor_start = 0x0D,
    .cursor_end = 0x0E,
    .start_address_high = 0x00,
    .start_address_low = 0x00,
    .cursor_location_high = 0x00,
    .cursor_location_low = 0x50,
    .vertical_retrace_start = 0x9C,
    .vertical_retrace_end = 0x0E,
    .vertical_display_end = 0x8F,
    .offset = 0x28,
    .underline_location = 0x1F,
    .start_vertical_blanking = 0x96,
    .end_vertical_blanking = 0xB9,
    .crtc_mode_control = 0xA3,
    .line_compare = 0xFF
};

static const gfx_controller gfxcRegs03 = {
    .set_reset = 0x00,
    .enable_set_reset = 0x00,
    .color_compare = 0x00,
    .data_rotate = 0x00,
    .read_map_select = 0x00,
    .gfx_mode = 0x10,
    .misc_graphics = 0x0E,
    .color_dont_care = 0x00,
    .bit_mask = 0xFF
};

static const attribute_controller attrcRegs03 = {
    .palette = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F},
    .attr_mode_control = 0x0C,
    .overscan_color = 0x00,
    .color_plane_enable = 0x0F,
    .horizontal_pixel_panning = 0x08,
    .color_select = 0x00
};

#endif