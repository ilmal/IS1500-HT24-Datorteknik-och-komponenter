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
volatile int* led_start = (volatile int*) 0x04000000;     // pointer to memory address for leds

// Assignment 1g -- Checks if the button is pressed on the board
int get_btn(void){
  volatile int* btn = (volatile int*) 0x040000d0;  // set the address of the button on the board
  int btn_value = *btn;  // set btn_value to the value of the button at address btn
  return btn_value & 0x1;  // perform and operation to check 1 lsb to see button status
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
  volatile int offset = 0x10 * display_number; // offset to choose which one of the displays on the board to use

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
  *led_start = 0b1111111111 & led_mask;  // set the value of led_start to led_mask (sequence of 1s and 0s)
}

// Helper function to convert a decimal number to the same digits in hex (ex: dec 13 -> 0x13)
int decimal_to_hexdigit(int decimal_value) {
    int tens = (decimal_value / 10) << 4;  // Tens place (shifted to upper nibble)
    int ones = decimal_value % 10;         // Ones place (lower nibble)
    return tens | ones;                    // OR operation to combine
}

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}


/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();

  int counter = 0;
  while (counter < 15) {
    delay(1000); // delay 1 sec
    counter++; // increment the counter each iteration of while-loop to count seconds
    set_leds(counter); // set the leds to the counter value in order to count binary
  }

  volatile int hours = 0;  // variable to count ammount of hours passed 
  volatile int minutes = 0; // variable to count amount of mins passed
  volatile int seconds = 0; // variable to count amount of secs passed

  // set all displays to 0 so it doesnt display 8's in the beginning
  set_displays(0, 0);
  set_displays(1, 0);
  set_displays(2, 0);
  set_displays(3, 0);
  set_displays(4, 0);
  set_displays(5, 0);

  // Enter a forever loop
  while (1) {
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    delay( 1000 );          // Delays 1 sec (adjust this value)
    tick( &mytime );     // Ticks the clock once'

    // store each digit of the time in a variable
    volatile int one_second = mytime & 0x000F;  // use 1111 hex to mask out 4 lsb which is the second
    volatile int ten_second = (mytime & 0x00F0) >> 4; // mask out ten sec digit and shift to the 4lsb to retrieve value
    volatile int one_minute = (mytime & 0x0F00) >> 8; // mask out one minute digit and shift to the 4lsb to retrieve value
    volatile int ten_minute = (mytime & 0xF000) >> 12;  // mask out ten minute digit and shift to the 4lsb to retrieve value
    volatile int one_hour = (mytime & 0xF0000) >> 16;  // mask out one hour digit and shift to the 4lsb to retrieve value
    volatile int ten_hour = (mytime & 0xF00000) >> 20;  // mask out ten hour digit and shift to the 4lsb to retrieve value

    hours = ten_hour * 10 + one_hour;   // calculate amount of hours from data retrieved from mytime
    minutes = ten_minute * 10 + one_minute; // calculate amount of mmins
    seconds = ten_second * 10 + one_second; // calculate amount of secs

    volatile int sw_status = get_sw();  // variable storing status of switches using get_sw()
    volatile int btn_status = get_btn();  // variable storing button status using get_btn()
    
    // if button-click detected
    if (btn_status){
      // retrieve status of the two leftmost switches in order to choose unit to change
      volatile int mod_switches = sw_status >> 8 & 0x3;  // shift 8 bits to the right and mask 0x3 = 11 in binary to get the two switches
      
      volatile int exit_switch = sw_status >> 7 & 0x1; // shift 7 bits to the right and mask 0x1 = 1 in binary to get the SW8 switch

      if (exit_switch) {
        break; // break the loop if SW8 is pressed
      }

      // retrieve status of 6 rightmost switches in order to read what value to set unit to
      volatile int sw_values = sw_status & 0x3F; // mask 0x3F = 111111 to get the first 6 switches

      // depending on values of mod_switches
      switch (mod_switches) {
        case 1: seconds = sw_values; break; // when switches are 01, change seconds to value at 6 rightmost switches
        case 2: minutes = sw_values; break; // when switches are 10, change minutes
        case 3: hours = sw_values; break; // when switches are 11, change hours
        default: break;
      }      
    }

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


    // setting mytime to edited time 
    mytime = (hours / 10 << 20) | (hours % 10 << 16) | (minutes / 10 << 12) | (minutes % 10 << 8) | (seconds / 10 << 4) | seconds % 10;

  }
  return 0;
}


