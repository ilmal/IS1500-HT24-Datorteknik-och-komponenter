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

volatile int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";
volatile int* led_start = (volatile int*) 0x04000000;


int get_btn(void){
  volatile int* btn = (volatile int*) 0x040000d0; 
  int btn_value = *btn;  
  return btn_value & 0x3FF;  
}

int get_sw( void ){
  volatile int* sw = (volatile int*) 0x04000010; 
  int sw_value = *sw;  
  return sw_value & 0x3FF;  
}

void set_displays(int display_number, int value){

  if (display_number > 5 || display_number < 0) {
    print("Invalid display number\n");
    return;
  }

  int display_value = 0;
  int offset = 0x10 * display_number;

  switch (value)
  {
  case 0:
    display_value = 64;
    break;
  case 1:
    display_value = 121;
    break;
  case 2:
    display_value = 36;
    break;
  case 3: 
    display_value = 48;
    break;
  case 4:
    display_value = 25;
    break;
  case 5:
    display_value = 18;
    break;
  case 6:
    display_value = 2;
    break;
  case 7:
    display_value = 120;
    break;
  case 8:
    display_value = 0;
    break;
  case 9:
    display_value = 24;
    break;
  default:
    display_value = 64;
    break;
  }

  volatile int* display_address = (volatile int*)(0x04000050 + offset);
  *display_address = display_value; 
}

// Assignment 1c/1d -- Function to set the LEDs on the board 
void set_leds(int led_mask) {
  *led_start = led_mask;  
}

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{
  print("Interrupt triggered\n");
  print("Cause: ");
  print_dec(cause);
  print("\n");
  return;
}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}

/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();

  int counter = 0;
  int hours = 0;

  // Set all displays to 0
  set_displays(0, 0);
  set_displays(1, 0);
  set_displays(2, 0);
  set_displays(3, 0);
  set_displays(4, 0);
  set_displays(5, 0);

  print_dec(get_sw());
  print("\n");

  print_dec(get_btn());
  print("\n");
  
  print("Starting loop...\n");

  // Enter a forever loop
  while (1) {
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    delay( 10 );          // Delays 1 sec (adjust this value)
    tick( &mytime );     // Ticks the clock once

    print("HOURS: ");
    print_dec(hours);
    print("\n");

    print("MYTIME: ");
    print_dec((mytime & 0xFFFF));  // This should match print_dec(mytime)
    print("\n");

    // Now compare the parsed BCD value to 5959
    if ((mytime & 0xFFFF) == 0x0) {  // Same as 0x5959 in BCD
        hours++;
        print("Hours incremented: ");
        print_dec(hours);
        print("\n");
    }

    print_dec(hours & 0xF);
    print("\n");
    print_dec((hours & 0xF0) >> 4);
    print("\n");
    
    set_displays(0, (mytime & 0xF));
    set_displays(1, (mytime & 0xF0) >> 4);
    set_displays(2, (mytime & 0xF00) >> 8);
    set_displays(3, (mytime & 0xF000) >> 12);
    set_displays(4, hours & 0xF);
    set_displays(5, (hours & 0xF0) >> 4);


    counter++;
    set_leds(counter);
    if (counter == 15) {
      continue;
    }
  }

  print("Loop ended\n");
  return 0;
}


