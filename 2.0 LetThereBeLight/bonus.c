// Bonus. Done - Boom...!!

int main(void) {

        // Notice : int, uint and char* are 16-bit whereas char is 8-bit

        unsigned int OFFSET         = 0x0020; // The 32 general purpose registers come first in Data memory, then IO regs..
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
