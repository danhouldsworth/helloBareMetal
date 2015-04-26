// Bonus. Done - Boom...!!

int main(void) {

    // Use casting (of int address) to pointer to avoid mismatch typewarning.
    unsigned char* DDRB  = (unsigned char*)(0x24);
    unsigned char* PORTB = (unsigned char*)(0x25);

    // DDRB/PORTB are pointers to octet registers.
    *DDRB  |= (1 << 4);
    *PORTB |= (1 << 4);
    // *PORTB &=~(1 << 4);

    for (;;) {
     // loop();
    }

    return 0;
}
