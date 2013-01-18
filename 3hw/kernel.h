#ifndef KERNEL_H
#define KERNEL_H

/************************
* 	Protected Mode	*
*************************/
#define PM_bit 0x01 
#define scan_codes_buffer_size 10
#define space_char 32

#define NT_flag_bit (1<<14) //in new version
#define tss_size 0x68

/************************
* 	Segments	*
*************************/
#define code_segment_selector 0b00001000 
#define data_segment_selector 0b00010000 

#define videobuffer_segment_selector 0b00011000 

#define videobuffer_segment_real 0xb800
#define videobuffer_size 0x4000

#define task0_selector 0b00100000
#define task1_selector 0b00101000
#define task2_selector 0b00110000
#define task1_gate_selector 0b00111000
#define task2_gate_selector 0b01000000

/************************
*	Memory		*
*************************/
#define seg_read_kernel	0x1000
#define addr_read_kernel 0x10000
#define ABS(addr) (addr_read_kernel+(addr))

/************************
*	Interrupts	*
*************************/
#define CMOS_index_port 0x70
#define NMI_bit 0x80

#define master_interrupt_mask_register 0x21
#define master_interrupt_service_register 0x20
#define slave_interrupt_mask_register 0xa1
#define slave_interrupt_service_register 0xa0

#endif //KERNEL_H
