#include "vga.h"
#include "io.h"
#include "mode13.h"
#include "mode12.h"
#include "mode03.h"

#define	VGA_AC_INDEX        0x3C0
#define	VGA_AC_WRITE        0x3C0
#define	VGA_AC_READ         0x3C1
#define	VGA_STATUS_READ     0x3DA
#define	VGA_MISC_WRITE      0x3C2
#define	VGA_MISC_READ       0x3CC

/*          COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX      0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA       0x3D5		/* 0x3B5 */
#define VGA_GC_INDEX        0x3CE
#define VGA_GC_DATA         0x3CF
#define VGA_SEQ_INDEX       0x3C4
#define VGA_SEQ_DATA        0x3C5

#define	VGA_NUM_AC_REGS     21
#define	VGA_NUM_CRTC_REGS   25
#define	VGA_NUM_GC_REGS     9
#define	VGA_NUM_SEQ_REGS    5

/********************************
 *      vram Address            *
 ********************************/

static u8* vram = (u8 *) 0xA0000;

/**
 * Backbuffer
 *
 * Extended memory area 0x00100000 - 0x00EFFFFF
 * https://wiki.osdev.org/Memory_Map_(x86)
 */
static u8* offscreen = (u8 *) 0x00100;

/********************************
 *      Some globals            *
 ********************************/
static unsigned int width;
static unsigned int height;
static unsigned int bytes;

/********************************
 *      Private Functions       *
 ********************************/

static void init_regs(const external_general* ext, const sequencer* seq, crt_controller* crtc, 
                    const gfx_controller* gfxc, const attribute_controller* attrc);

/********************************
 *      Public Driver API       *
 ********************************/
 
void xv_init_gfx_mode(int mode) {
    switch (mode) {
        case MODE03H:
            width = 80;
            height = 25;
            bytes = 2000;

            init_regs(&extRegs03, 
                    &seqRegs03, 
                    &crtcRegs03, 
                    &gfxcRegs03, 
                    &attrcRegs03);
            break;
        case MODE13H:
            width = 320;
            height = 200;
            bytes = 64000;
            
            init_regs(&extRegs13, 
                    &seqRegs13, 
                    &crtcRegs13, 
                    &gfxcRegs13, 
                    &attrcRegs13);
            break;
        case MODE12H:
            width = 640;
            height = 480;
            bytes = 38400;
            
            init_regs(&extRegs12, 
                    &seqRegs12, 
                    &crtcRegs12, 
                    &gfxcRegs12, 
                    &attrcRegs12);
            break;
        default:
            break;
    }

    xv_clr_offscreen(0x00);
}

void xv_plot_pixel(int x, int y, unsigned short color) {
    unsigned short offset = width * y + x;
    vram[offset] = color;
}

void xv_plot_pixelf(int x, int y, unsigned short color) {
    unsigned short offset = width * y + x;
    offscreen[offset] = color;
}

void xv_clr_screen(unsigned short color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            xv_plot_pixel(i, j, color);
        }
    }
}

void xv_clr_offscreen(unsigned short color) {
    for (int i = 0; i < bytes; ++i) {
        offscreen[i] = color;
    }
}

void xv_wait_vretrace() {
    while (insb(VGA_STATUS_READ) & 0x08);
    while (!(insb(VGA_STATUS_READ) & 0x08)); 
}

void xv_swap_buffers() {
    xv_wait_vretrace();

    for (int i = 0; i < bytes; ++i) {
        vram[i] = offscreen[i];
    }
}

void init_regs(const external_general* ext, const sequencer* seq, crt_controller* crtc, 
            const gfx_controller* gfxc, const attribute_controller* attrc) {
	// write MISCELLANEOUS reg
    outb(VGA_MISC_WRITE, ext->misc_output);
    // write sequencer regs
    for (int i = 0; i < VGA_NUM_SEQ_REGS; ++i) {
        outb(VGA_SEQ_INDEX, i);
        outb(VGA_SEQ_DATA, seq->m[i]);
    }
		
    // unlock CRTC registers
    outb(VGA_CRTC_INDEX, 0x03);
    outb(VGA_CRTC_DATA, insb(VGA_CRTC_DATA) | 0x80);
    outb(VGA_CRTC_INDEX, 0x11);
    outb(VGA_CRTC_DATA, insb(VGA_CRTC_DATA) & ~0x80);
    // make sure they remain unlocked
    crtc->end_horizontal_blanking |= 0x80;
    crtc->vertical_retrace_end &= ~0x80;
    // write CRTC regs
    for (int i = 0; i < VGA_NUM_CRTC_REGS; ++i) {
        outb(VGA_CRTC_INDEX, i);
        outb(VGA_CRTC_DATA, crtc->m[i]);
    }

    // write GRAPHICS CONTROLLER regs
    for (int i = 0; i < VGA_NUM_GC_REGS; ++i) {
        outb(VGA_GC_INDEX, i);
        outb(VGA_GC_DATA, gfxc->m[i]);
    }

    // write ATTRIBUTE CONTROLLER regs
    for (int i = 0; i < VGA_NUM_AC_REGS; ++i) {
        (void)insb(VGA_STATUS_READ);
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, attrc->m[i]);
    }
		
    // lock 16-color palette and unblank display
    (void)insb(VGA_STATUS_READ);
    outb(VGA_AC_INDEX, 0x20);
}
