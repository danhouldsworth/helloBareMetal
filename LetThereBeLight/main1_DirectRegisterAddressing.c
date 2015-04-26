// Done! Boom...!!

typedef unsigned char uint8;

#define setREG(ADDR) (*(volatile uint8 *)(ADDR))
#define DDRB  0x24
#define PORTB 0x25

int main(void) {

    setREG(DDRB)  |= (1 << 4);
    setREG(PORTB) |= (1 << 4);
    setREG(PORTB) &=~(1 << 4);

    for (;;) {
     // loop();
    }

    return 0;
}
