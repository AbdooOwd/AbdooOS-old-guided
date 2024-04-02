unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx , %% al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
    __asm__(" out %%al , %% dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx , %% ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data) {
    __asm__(" out %%ax , %% dx" : : "a" (data), "d" (port));
}

static inline void outb(u16 port, u8 val) {
    __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile ("inb %w1, %b0"
        : "=a"(ret)
        : "Nd"(port)
        : "memory");
    return ret;
}