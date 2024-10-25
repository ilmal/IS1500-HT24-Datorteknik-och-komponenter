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

int mytime = 0x5957; // start time (00:59:57)
char textstring[] = "text, more text, and even more text!";
volatile int* led_start = (volatile int*) 0x04000000; // pointer to memory address for leds

// Assignment 2 -- Timers
volatile int* timer_status = (volatile int*)0x04000020; // Address for the timer
volatile int* timer_control = (volatile int*)0x04000024; // Control register
volatile int* timer_periodl = (volatile int*)0x04000028; // Event flag register
volatile int* timer_periodh = (volatile int*)0x0400002C; // periodh address

// Global counter for timeouts
volatile int timeoutcount = 0;

// Assignment 1g -- Checks if the button is pressed on the board
int get_btn(void){
  volatile int* btn = (volatile int*) 0x040000d0;  // set the address of the button on the board
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

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Assignment 2b -- Initialize timer */
void labinit(void)
{
  // Load the timer with the count for 100 ms (30 MHz / 10 = 3,000,000) => (30 000 000 / 10 = 3 000 000)
  *timer_periodl = 0xC6C0; // Lower 16 bits of 3 000 000 = 0x2DC6C0 in registry periodl
  *timer_periodh = 0x002D; // Upper 16 bits of 3 000 000 = 0x2DC6C0 in registry periodh

  // bit 3 in timer control starts timer and bit 2 enables CONT which makes the counter repeat after reaching 0
  *timer_control = 0x6; // Enable the timer -- 0x6 = 110 in binary
}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();

  int counter = 0; // variable to count seconds passed since start of loop in order to break at 15 secs
  volatile int hours = 0;  // variable to count ammount of hours passed 
  volatile int minutes = 0;
  volatile int seconds = 0;

  // set all displays to 0 so it doesnt display 8's in the beginning
  set_displays(0, 0);
  set_displays(1, 0);
  set_displays(2, 0);
  set_displays(3, 0);
  set_displays(4, 0);
  set_displays(5, 0);
  print_dec(get_sw());

  // Enter a forever loop
  while (1) {
    // variables that retrieve time values from mytime
    volatile int one_second = mytime & 0x000F;  // use 1111 hex to mask out 4 lsb which is the second
    volatile int ten_second = (mytime & 0x00F0) >> 4; // mask out ten sec digit and shift to the 4lsb to retrieve value
    volatile int one_minute = (mytime & 0x0F00) >> 8; // mask out one minute digit and shift to the 4lsb to retrieve value
    volatile int ten_minute = (mytime & 0xF000) >> 12;  // mask out ten minute digit and shift to the 4lsb to retrieve value
    volatile int one_hour = (mytime & 0xF0000) >> 16;  // mask out one hour digit and shift to the 4lsb to retrieve value
    volatile int ten_hour = (mytime & 0xF00000) >> 20;  // mask out ten hour digit and shift to the 4lsb to retrieve value

    // adding the values to variables that count hours, mins, and secs
    hours = ten_hour * 10 + one_hour;
    minutes = ten_minute * 10 + one_minute;
    seconds = ten_second * 10 + one_second;

    // variables storing state of switches and button
    volatile int sw_status = get_sw();
    volatile int btn_status = get_btn();

    // if button-click is detected
    if (btn_status){
      volatile int mod_switches = sw_status >> 8 & 0x3;  // shift 8 bits to the right and mask 0x3 = 11 in binary to get the two switches
      volatile int sw_values = sw_status & 0x3F; // mask 0x3F = 111111 to get the first 6 switches

      volatile int exit_switch = sw_status >> 7 & 0x1; // shift 7 bits to the right and mask 0x1 = 1 in binary to get the SW8 switch

      if (exit_switch) {
        break; // break the loop if SW8 is pressed
      }

      // adjust hours, mins or secs depending on mod_switches
      switch (mod_switches) {
        case 1: seconds = sw_values; break;
        case 2: minutes = sw_values; break;
        case 3: hours = sw_values; break;
        default: break;
      }

      // set each display to the corresponding digit variable 
      set_displays(0, seconds % 10);
      set_displays(1, seconds / 10);
      set_displays(2, minutes % 10);
      set_displays(3, minutes / 10);
      set_displays(4, hours % 10);
      set_displays(5, hours / 10);

      mytime = (hours / 10 << 20) | (hours % 10 << 16) | (minutes / 10 << 12) | (minutes % 10 << 8) | (seconds / 10 << 4) | seconds % 10;
    }

    // Check if the timer has timed out
    // first bit in timer status indicates timeout (TO)
    if (*timer_status & 0x1) { // Check if the event flag indicates a timeout (is set to 1)
      *timer_status = 0x0; // write 0 to reset TO bit
      timeoutcount++; // Increment timeout count to track amount of timeouts

      // Update time and display only once every 10 timeouts (10 timeouts = 1 sec)
      if (timeoutcount >= 10) {
        timeoutcount = 0; // Reset the timeout count

        // time2string( textstring, mytime ); // Converts mytime to string
        // display_string( textstring ); //Print out the string 'textstring'
        //delay( 1000 );          // Delays 1 sec (adjust this value)
        tick( &mytime );     // Ticks the clock once

        // variables that retrieve time values from mytime
        one_second = mytime & 0x000F;  // use 1111 hex to mask out 4 lsb which is the second
        ten_second = (mytime & 0x00F0) >> 4; // mask out ten sec digit and shift to the 4lsb to retrieve value
        one_minute = (mytime & 0x0F00) >> 8; // mask out one minute digit and shift to the 4lsb to retrieve value
        ten_minute = (mytime & 0xF000) >> 12;  // mask out ten minute digit and shift to the 4lsb to retrieve value
        one_hour = (mytime & 0xF0000) >> 16;  // mask out one hour digit and shift to the 4lsb to retrieve value
        ten_hour = (mytime & 0xF00000) >> 20;  // mask out ten hour digit and shift to the 4lsb to retrieve value

        // adding the values to variables that count hours, mins, and secs
        hours = ten_hour * 10 + one_hour;
        minutes = ten_minute * 10 + one_minute;
        seconds = ten_second * 10 + one_second;

        // check if seconds are over 60 and increment minutes
        if (seconds >= 60) {
          seconds = seconds % 60;
          minutes++;
        }

        // check if minutes are over 60 and increment hours
        if (minutes >= 60) {
          minutes = minutes % 60;
          hours++;
        }

        // testing to see the time with set variables for each digit
        print("Time: ");
        print_dec(hours);
        print(":");
        print_dec(minutes);
        print(":");
        print_dec(seconds);
        print("\n");

        // set each display to the corresponding digit variable 
        set_displays(0, seconds % 10);
        set_displays(1, seconds / 10);
        set_displays(2, minutes % 10);
        set_displays(3, minutes / 10);
        set_displays(4, hours % 10);
        set_displays(5, hours / 10);

        counter++; // increment the counter each iteration of while-loop to count seconds
        set_leds(counter); // set the leds to the counter value in order to count binary

        mytime = (hours / 10 << 20) | (hours % 10 << 16) | (minutes / 10 << 12) | (minutes % 10 << 8) | (seconds / 10 << 4) | seconds % 10;

        // check if the board has counted leds for 15 secs
        if (counter == 15) {
          continue; // set to 'break' instead if it should stop at 15 secs
        }
      }
    }
  }
  return 0;
}


