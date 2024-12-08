#include "board_io.h"
volatile int *led_start = (volatile int *)0x04000000; // pointer to memory address for leds

// Assignment 2 -- Timers
volatile int *timer_status = (volatile int *)0x04000020;  // Address for the timer
volatile int *timer_control = (volatile int *)0x04000024; // Control register
volatile int *timer_periodl = (volatile int *)0x04000028; // Event flag register
volatile int *timer_periodh = (volatile int *)0x0400002C; // periodh address

// Global counter for timeouts
volatile int timeoutcount = 0;

// actual global stuff
volatile int switch_event = 0;
volatile int timer_event = 0;
volatile int button_event = 0;

extern void print(const char *);

int get_btn(void)
{
    volatile int *btn = (volatile int *)0x040000d0; // set the address of the button on the board
    int btn_value = *btn;                           // set btn_value to the value of the button at address btn
    return btn_value & 0x1;                         // perform and operation to check one lsb to see button status
}

int get_sw(void)
{
    int *sw = (int *)0x04000010; // set the address of all the toggle switches
    int sw_value = *sw;          // retrieve value of switches in adress sw and set sw_value to that value
    return sw_value & 0x3FF;     // 3ff = 001 111 111 111 - > finds value (on/off) for each swtich (there are 10 toggles)
}

void set_displays(int display_number, int value)
{
    // check if the display number is valid
    if (display_number > 5 || display_number < 0)
    {
        print("Invalid display number\n");
        return;
    }

    int display_value = 0;                       // value to set on each display to represent a specific number
    volatile int offset = 0x10 * display_number; // offset to set which one of the displays to use

    // switch statement to set chosen number on display using binary (0 is on, 1 is off)
    switch (value)
    {
    case 0:
        display_value = 64; // 1 000 000 in binary
        break;
    case 1:
        display_value = 121; // 1 111 001 in binary
        break;
    case 2:
        display_value = 36; // 0 100 100 in binary
        break;
    case 3:
        display_value = 48; // 0 110 000 in binary
        break;
    case 4:
        display_value = 25; // 0 011 001 in binry
        break;
    case 5:
        display_value = 18; // 0 010 010 in binary
        break;
    case 6:
        display_value = 2; // 0 000 010 in binry
        break;
    case 7:
        display_value = 120; // 1 111 000 in binary
        break;
    case 8:
        display_value = 0; // 0 000 000 in binary
        break;
    case 9:
        display_value = 24; // 0 011 000 in binary
        break;
    case 10:
        // F
        display_value = 0b0001110;
        break;
    case 11:
        // o
        display_value = 0b0100011;
        break;
    case 12:
        // d
        display_value = 0b0100001;
        break;
    case 13:
        // H
        display_value = 0b0001001;
        break;
    case 100:
        // empty
        display_value = 0b1111111;
        break;
    default:
        display_value = 64; // set 0 as default number
        break;
    }

    volatile int *display_address = (volatile int *)(0x04000050 + offset); // set the address of the chosen display
    *display_address = display_value;                                      // set value of display_address to display_value which is set in swtich cases
}

void set_leds(int led_mask)
{
    *led_start = led_mask; // set the value of led_start to led_mask (sequence of 1s and 0s)
}

int get_leds(int led_mask)
{
    return *led_start & 0b1111111111;
}

void labinit(void)
{
    // Load the timer with the count for 100 ms (30 MHz / 10 = 3,000,000)
    *timer_periodl = 0x06C0; // Lower 16 bits of 3000000 = 0x2DC6C0
    *timer_periodh = 0x002D; // Upper 16 bits of 3000000

    *timer_control = 0b111; // Enable the timer

    volatile int *interupt_mask_btn = (volatile int *)0x040000d8; // 2 offsets * 4 = 8
    *interupt_mask_btn = 0b1;                                     // enable interrupt for button by setting interrupt mask to 1

    volatile int *interupt_mask_switches = (volatile int *)0x04000018; // 2 offsets * 4 = 8
    *interupt_mask_switches = 0b1111111111;                            // enable interrupt for button by setting interrupt mask to 1

    enable_interrupt(); // Enable global interrupts and timer interrupts (+ button)
}

int button_toggle = 0;

void handle_interrupt(unsigned cause)
{
    if (cause == 16)
    {                          // Timer interrupt
        *timer_status &= ~0x1; // Clear the interrupt flag
        timeoutcount++;

        if (timeoutcount >= 10)
        {
            timeoutcount = 0;
            timer_event = 1; // Signal that 1 second has passed
        }
    }

    if (cause == 17)
    { // Switch interrupt
        volatile int *capture = (volatile int *)0x0400001c;
        *capture = 0b0; // Clear the interrupt

        switch_event = 1; // Signal that a switch event occurred
    }

    if (cause == 18)
    { // Button interrupt
        volatile int *edge_capture = (volatile int *)0x040000dc;
        *edge_capture = 0b0; // Clear the interrupt

        if (button_toggle)
        {
            button_event = 1; // Signal that a button was pressed
            button_toggle = 0;
        }
        else
        {
            button_toggle = 1;
        }
    }
}
