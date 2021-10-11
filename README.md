# WirelessIRProbe
ATtiny85 code for IR Wireless CNC touch probe. Based on 8MHz internal clock for ATtiny.
ATtiny sleeps and monitors NC switch input of touch probe. When probe touches, switch opens, CPU wakes up, and generates approx. 
38KHz output signal that is used to drive IR LEDs. When switch closes, CPU goes back to sleep. Includes provisions for 
switch debounce.
