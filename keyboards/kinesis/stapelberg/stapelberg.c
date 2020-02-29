#include "stapelberg.h"

void matrix_init_kb(void) {
	// put your keyboard start-up code here
	// runs once when the firmware starts up

	matrix_init_user();
}

void matrix_scan_kb(void) {
	// put your looping keyboard code here
	// runs every cycle (a lot)

	matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
	// put your per-action keyboard code here
	// runs for every action, just before processing by the firmware

	return process_record_user(keycode, record);
}

void keyboard_pre_init_kb() {
  /* Setting status LEDs pins to output and +5V (off) */
  setPinOutput(F0); // Keypad LED
  setPinOutput(F1); // Scroll Lock LED
  setPinOutput(F2); // Num Lock LED
  setPinOutput(F3); // Caps Lock LED
  writePinHigh(F0);
  writePinHigh(F1);
  writePinHigh(F2);
  writePinHigh(F3);

  keyboard_pre_init_user();
}

bool led_update_kb(led_t led_state) {
  bool res = led_update_user(led_state);
  if (res) {
    writePin(F0, !led_state.compose);
    writePin(F1, !led_state.scroll_lock);
    writePin(F2, !led_state.num_lock);
    writePin(F3, !led_state.caps_lock);
  }
  return res;
}
