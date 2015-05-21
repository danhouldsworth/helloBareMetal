TASK

1. Using existing libraries, get a simple display running from an AVR over SPI.
2. Using the datasheets, and refering to the examples above, write a light weight AVR / OLED driver with basic graphic functions.
3. Add text support with a range of font size.
4. Add bitmap support.

Bonus : Extend the above to drive the 1.8" TFT display.

Suggested Approach
Hide the details of the particular display from the applications by writing to bitmapped canvas in memory which is then copied wholesale to the display.
- Allows the update and refresh of the display to be a scheduled task at say 20 Hz which is sufficent for dynamic displays (eg scrolling text in 1 pixel increments; animated icons etc)
- requires the minimum of low level driver functions for any given display ( init, clear, fill from memory)
- functions for writing to canvas at specified coords for both graphics and text can use logical "or"ing of pixel values to allow adding to existing content to build up final image, overlapping icons, etc.
- Font tables are easy to generate using photoshop and convert to source files
- an example using the Newhaven OLED 130 x 32 display (SSD 1305 driver) can be provided

