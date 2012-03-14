#ifndef KERNEL_H
#define KERNEL_H

// videobuffer consts
#define videobuffer_segment_RM 0xb800
#define video_segm_selector 0b00011000
#define videobuffer_size 2000

// interrupts
#define CMOS_index_port 0x70
#define NMI_bit 0x80
#define master_interrupt_mask_register 0x21
#define master_interrupt_service_register 0x20
#define slave_interrupt_mask_register 0xa1
#define slave_interrupt_service_register 0xa0

// switching to Protected Mode
#define PM_bit 0x01

// addresses
#define seg_read_kernel	0x1000
#define addr_read_kernel 0x10000
#define ABS(addr) (addr_read_kernel+(addr))

#define code_segm_selector 0b00001000
#define data_segm_selector 0b00010000


#define scan_codes_buffer_size 10
#define space_char 32

#endif //KERNEL_H
