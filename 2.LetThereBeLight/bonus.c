// Bonus. Done - Boom...!!

int main(void) {

        // Notice : int, uint and char* are 16-bit whereas char is 8-bit

        // I need to better understand the memory mapping. The offset isn't needed in assembler, so maybe our program is sitting in different space?
        unsigned int OFFSET         = 0x0020;
        // --

        unsigned int DDRB_addr      = 0x0004 + OFFSET;
        unsigned int PORTB_addr     = 0x0005 + OFFSET;

        unsigned char* DDRB_ptr  = (unsigned char*)DDRB_addr;
        unsigned char* PORTB_ptr = (unsigned char*)PORTB_addr;

        *DDRB_ptr  |= (1 << 4);
        *PORTB_ptr |= (1 << 4);
        // *PORTB_ptr &=~(1 << 4);

        for (;;) {
        // loop();
        }

        return 0;
}
