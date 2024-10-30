/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );
extern void enable_interrupt(void); // access to enable_interrupt func from boot.S

int mytime = 0x5957; // start time (00:59:57)
char textstring[] = "text, more text, and even more text!";
volatile int* led_start = (volatile int*) 0x04000000;
int prime = 1234567;    // assignment 3b -- adding int prime

// Assignment 2 Timers
volatile int* timer_status = (volatile int*)0x04000020; // Address for the timer
volatile int* timer_control = (volatile int*)0x04000024; // Control register
volatile int* timer_periodl = (volatile int*)0x04000028; // Event flag register
volatile int* timer_periodh = (volatile int*)0x0400002C; // periodh address

// Global counter for timeouts
volatile int timeoutcount = 0;

// Global pointer for btn
volatile int* btn = (volatile int*) 0x040000d0;  // set the address of the button on the board

// Assignment 1g -- Checks if the button is pressed on the board
int get_btn(void){
  btn = (volatile int*) 0x040000d0;  // set the address of the button on the board
  int btn_value = *btn;  // set btn_value to the value of the button at address btn
  return btn_value & 0x1;  // perform and operation to check one lsb to see button status
}

// Assignment 1f -- checks the status of toggle switches on the board 
int get_sw(void){
  int* sw = (int*) 0x04000010; // set the address of all the toggle switches
  int sw_value = *sw;  // retrieve value of switches in adress sw and set sw_value to that value
  return sw_value & 0x3FF;  // 3ff = 001 111 111 111 - > finds value (on/off) for each swtich (there are 10 toggles)
}

// Assignment 1e -- Function to set digit lights on the board
void set_displays(int display_number, int value){
  // check if the display number is valid
  if (display_number > 5 || display_number < 0) {
    print("Invalid display number\n");
    return;
  }

  int display_value = 0;  // value to set on each display to represent a specific number
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
    display_value = 2;  // 0 000 010 in binry
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
  default:
    display_value = 64; // set 0 as default number 
    break;
  }

  volatile int* display_address = (volatile int*)(0x04000050 + offset); // set the address of the chosen display
  *display_address = display_value; // set value of display_address to display_value which is set in swtich cases
}

// Assignment 1c/1d -- Function to set the LEDs on the board 
void set_leds(int led_mask) {
  *led_start = led_mask;  // set the value of led_start to led_mask (sequence of 1s and 0s)
}

// Helper function to convert a decimal number to the same digits in hex (ex: 13 -> 0x13)
int decimal_to_hexdigit(int decimal_value) {
    int tens = (decimal_value / 10) << 4;  // Tens place (shifted to upper nibble)
    int ones = decimal_value % 10;         // Ones place (lower nibble)
    return tens | ones;                    // OR operation to combine
}

// func to display the time every time there's a timer interrupt
void display_time(void) {
  volatile int one_second = mytime & 0x000F;  // use 1111 hex to mask out 4 lsb which is the second
  volatile int ten_second = (mytime & 0x00F0) >> 4; // mask out ten sec digit and shift to the 4lsb to retrieve value
  volatile int one_minute = (mytime & 0x0F00) >> 8; // mask out one minute digit and shift to the 4lsb to retrieve value
  volatile int ten_minute = (mytime & 0xF000) >> 12;  // mask out ten minute digit and shift to the 4lsb to retrieve value
  volatile int one_hour = (mytime & 0xF0000) >> 16;  // mask out one hour digit and shift to the 4lsb to retrieve value
  volatile int ten_hour = (mytime & 0xF00000) >> 20;  // mask out ten hour digit and shift to the 4lsb to retrieve value

  // set each display to the corresponding digit variable 
  set_displays(0, one_second);
  set_displays(1, ten_second);
  set_displays(2, one_minute);
  set_displays(3, ten_minute);
  set_displays(4, one_hour);
  set_displays(5, ten_hour);

}

/* assignment 3d/e -- adjusting handle_interrupt */
void handle_interrupt ( unsigned cause ) {

  if (cause == 16) { // Check if interrupt is from the timer
      *timer_status = *timer_status & ~0x1; // Clear the interrupt flag (~0x1 is inverted and clears only 1lsb)
      timeoutcount++; // Increment the timeout counter

      // Assignment 3f -- clear timeoutcount every 10 timouts since that's a second
      if (timeoutcount == 10) {
          timeoutcount = 0; // Reset counter after 10 interrupts

        tick( &mytime );     // Ticks the clock once
        display_time();      // calls for func to display time
      }
  }

  // surprise assignment -- increment 2 secs when button is pressed
  if (cause == 18) {
    print("Button pressed\n");
    volatile int* edge_capture = (volatile int*) 0x040000dc;  //edge capture registry adre
    *edge_capture = 0b0; // clear the interrupt
    tick(&mytime);
    display_time();
  }
}

/* Add your code here for initializing interrupts. */
void labinit(void)
{
  // Load the timer with the count for 100 ms (30 MHz / 10 = 3,000,000)
  *timer_periodl = 0x06C0; // Lower 16 bits of 3000000 = 0x2DC6C0
  *timer_periodh = 0x002D; // Upper 16 bits of 3000000

  *timer_control = 0b111; // Enable the timer

  volatile int* interupt_mask_btn = (volatile int*) 0x040000d8; // 2 offsets * 4 = 8 

  *interupt_mask_btn = 0b1; // enable interrupt for button by setting interrupt mask to 1

  // Assignment 3h -- enable interrupts  
  print("Enabling interrupts\n");
  enable_interrupt();      // Enable global interrupts and timer interrupts (+ button)
  print("Interrupts enabled\n");
}


/* assignment 3c -- adjusting main-func */
int main ( void ) {
  labinit();
  while (1) {
    prime = nextprime( prime );
    print ("Prime: ");
    print_dec( prime );
    print("\n");
  }
}