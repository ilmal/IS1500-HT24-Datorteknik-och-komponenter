#ifndef BOARD_IO_H
#define BOARD_IO_H

// Function prototypes
int get_btn(void);
int get_sw(void);
void set_displays(int display_number, int value);
void set_leds(int value);
int get_leds(int led_mask);
void labinit(void);
void handle_interrupt(unsigned cause);

extern void enable_interrupt(void); // access to enable_interrupt func from boot.S

extern volatile int switch_event;
extern volatile int timer_event;
extern volatile int button_event;

#endif // BOARD_IO_H
