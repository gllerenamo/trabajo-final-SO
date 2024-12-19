#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"

#define IDT_ENTRIES 256
#define MAX_INTERRUPTS 128

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

typedef struct {
    uint16_t base_low;     // Bits bajos de la dirección de la rutina
    uint16_t sel;          // Selector de segmento (generalmente 0x08 para el kernel)
    uint8_t  always0;      // Siempre 0
    uint8_t  flags;        // Flags (tipo y nivel de privilegio)
    uint16_t base_high;    // Bits altos de la dirección de la rutina
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;        // Tamaño de la tabla (número de bytes - 1)
    uint32_t base;         // Dirección base de la tabla
} __attribute__((packed)) idt_ptr_t;

typedef void (*isr_t)(uint8_t);

extern idt_entry_t idt[IDT_ENTRIES];
extern idt_ptr_t idt_ptr;
void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install();

extern isr_t interrupt_handlers[MAX_INTERRUPTS];
void register_interrupt_handler(uint8_t n, isr_t handler);
void isr_handler(uint8_t number);

void pic_remap();

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

void init_interrupts();

#endif