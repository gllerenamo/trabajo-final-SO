void delay_ms(int ms) {
    for (int i = 0; i < ms * 1000; i++) {
        asm volatile("nop");
    }
}