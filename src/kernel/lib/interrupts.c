#include "interrupts.h"

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_ptr;
isr_t interrupt_handlers[MAX_INTERRUPTS] = {0};

void init_interrupts() {
    idt_install();
    for (int i = 0; i < MAX_INTERRUPTS; i++) {
        register_interrupt_handler(i, 0);
    }
    /*
    pic_remap();
    isr_install();

    __asm__ __volatile__ ("sti"); // Habilita las interrupciones
    */
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void pic_remap() {
    // Reprograma el PIC para que las interrupciones IRQ no interfieran con los vectores de CPU
    outb(PIC1_COMMAND, 0x11); // Inicia el proceso de inicialización
    outb(PIC2_COMMAND, 0x11);

    outb(PIC1_DATA, 0x20); // Asigna la base de interrupciones para el PIC1
    outb(PIC2_DATA, 0x28); // Asigna la base de interrupciones para el PIC2

    outb(PIC1_DATA, 0x04); // Configura PIC1 para notificar a PIC2
    outb(PIC2_DATA, 0x02); // Configura PIC2 para recibir señales de PIC1

    outb(PIC1_DATA, 0x01); // Modo 8086/88
    outb(PIC2_DATA, 0x01);

    // Enmascara todas las interrupciones al inicio
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install() {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint32_t)&idt;

    // Cargar la IDT con el ensamblador
    __asm__ __volatile__("lidt (%0)" : : "r"(&idt_ptr));
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

/*
void isr0() {
    print("Excepción: División por cero.\n");
}

void isr14() {
    print("Excepción: Page fault.\n");
}

void isr_install() {
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
}
*/

void isr_handler(uint8_t number) {
    isr_t handler = interrupt_handlers[number];
    if (handler) handler(number);
}


