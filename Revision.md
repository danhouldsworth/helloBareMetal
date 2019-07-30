# Revision

Coming back to this cold after 18 months away, this is a useful resource in many ways, but still lacking in others.
As ever, the learning is in the doing. And coming into a nicely formatted folder structure using git version control scares me away from editing much, but that leaves the revision to just reading rather than doing.

## Type out in long hand each task.

Write out the task. Think about why that was even an interesting question. If we have more, then add to the main repo.
What the library doesn't cover is :
* a crash course on PORTS (eg which change between chips), the principle of setting Input / Output.
* internal / external oscillators.
* compiler variables
* libraries / reference sheets (C and assember)
* A big part of embedded programming is referencing the datasheets. If we assume that could have another period of rustiness, then need to include a quick bootstrap tasks on this.

## Probably the best way is to start from scratch!!
Then can cherry pick the tasks / solutions from the original. And use them as reference while I build the second.
But the act of thinking about the challenges, writing them down, then creating a variety of solutions for each platform is not be missed.

## Hardware set up

Chips of interest :
* ATmega328p (I have loads, and Arduino is based on this),
* ATmega32u4 the NanoWii board is based on this
* ATmega8 (most of the SimonK ESCs are based on this)

Setups of interest :
* Internal oscillator (to minimise setup time when using bare chip)
* External oscillator (Many of the breakout boards, ESCs and FCs will have this)
* 6pin 		ISP programming (PCBs)
* Breakout 	ISP programming (Breadboards)
* Bootloader programming (USB)
* Bootloader programming (USART)
* Bootloader programming (PWM)
* Drivers needed for USB-->Serial FTDI (MacAir, MacPro MacRetina)


