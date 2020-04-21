#include QMK_KEYBOARD_H
#include "ryancdotorg.h"

typedef struct {
  uint16_t timer_last;
  led_t native;
  uint8_t num_lock;
  uint8_t caps_lock;
  uint8_t scroll_lock;
  uint8_t compose;
  uint8_t pad:5;
  uint8_t cycle:3;
} blink_t;

static blink_t leds;

bool led_update_user(led_t led_state) {
  leds.native = led_state;
  return false;
}

static void blink_leds(void) {
  if (timer_elapsed(leds.timer_last) > 124) {
    leds.timer_last = timer_read();
    writePin(F0, leds.compose ? !(leds.compose >> leds.cycle & 1) : !leds.native.compose);
    writePin(F1, leds.scroll_lock ? !(leds.scroll_lock >> leds.cycle & 1) : !leds.native.scroll_lock);
    writePin(F2, leds.num_lock ? !(leds.num_lock >> leds.cycle & 1) : !leds.native.num_lock);
    writePin(F3, leds.caps_lock ? !(leds.caps_lock >> leds.cycle & 1) : !leds.native.caps_lock);
    leds.cycle = (leds.cycle + 1) & 7;
  }
}

void matrix_scan_user(void) { blink_leds(); }

enum my_layers {
  _QWERTY,
  _CMKMDH,
  _KEYPAD,
  _SYMBOL,
  _DUMMY
};

enum my_keycodes {
    TEST_KEYCODE_PLEASE_IGNORE = SAFE_RANGE,
};

//Tap dance enums
enum _tap_dance {
  X_CTL,
  X_LOL,
  HOME_PGUP,
  END_PGDN,
  SOME_OTHER_DANCE
};

/****************************************************************************************************
*
* Keymap: Default Layer in Qwerty
*
* ,-------------------------------------------------------------------------------------------------------------------.
* | Esc    |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F8  |  F9  |  F10 |  F12 | PSCR | SLCK | PAUS |  FN0 |  BOOT  |
* |--------+------+------+------+------+------+---------------------------+------+------+------+------+------+--------|
* | =+     |  1!  |  2@  |  3#  |  4$  |  5%  |                           |  6^  |  7&  |  8*  |  9(  |  0)  | -_     |
* |--------+------+------+------+------+------|                           +------+------+------+------+------+--------|
* | Tab    |   Q  |   W  |   E  |   R  |   T  |                           |   Y  |   U  |   I  |   O  |   P  | \|     |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | Caps   |   A  |   S  |   D  |   F  |   G  |                           |   H  |   J  |   K  |   L  |  ;:  | '"     |
* |--------+------+------+------+------+------|                           |------+------+------+------+------+--------|
* | Shift  |   Z  |   X  |   C  |   V  |   B  |                           |   N  |   M  |  ,<  |  .>  |  /?  | Shift  |
* `--------+------+------+------+------+-------                           `------+------+------+------+------+--------'
*          | `~   | INS  | Left | Right|                                         | Up   | Down |  [{  |  ]}  |
*          `---------------------------'                                         `---------------------------'
*                                        ,-------------.         ,-------------.
*                                        | Ctrl | Alt  |         | Gui  | Ctrl |
*                                 ,------|------|------|         |------+------+------.
*                                 |      |      | Home |         | PgUp |      |      |
*                                 | BkSp | Del  |------|         |------|Return| Space|
*                                 |      |      | End  |         | PgDn |      |      |
*                                 `--------------------'         `--------------------'
*/

#define LCTLESC LCTL_T(KC_ESC)
#define SYMBSLS LT(_SYMBOL, KC_BSLS)
#define SYMTAB  LT(_SYMBOL, KC_TAB)
#define HOMPGUP _______
#define ENDPGDN _______
#define TD_CTL  TD(X_CTL)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERTY] = LAYOUT(

TD_CTL ,KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8  ,
        KC_EQL ,KC_1   ,KC_2   ,KC_3   ,KC_4   ,KC_5   ,
        SYMTAB ,KC_Q   ,KC_W   ,KC_E   ,KC_R   ,KC_T   ,
        LCTLESC,KC_A   ,KC_S   ,KC_D   ,KC_F   ,KC_G   ,
        KC_LSFT,KC_Z   ,KC_X   ,KC_C   ,KC_V   ,KC_B   ,
                KC_GRV ,KC_NUBS,KC_LEFT,KC_RGHT,
                                                KC_LCTL,KC_LALT,
                                                        KC_HOME,
                                        KC_BSPC,KC_DEL ,KC_END ,

KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,KC_PSCR,KC_SLCK,KC_PAUS,NK_TOGG,KC_NO  ,
        KC_6   ,KC_7   ,KC_8   ,KC_9   ,KC_0   ,KC_MINS,
        KC_Y   ,KC_U   ,KC_I   ,KC_O   ,KC_P   ,SYMBSLS,
        KC_H   ,KC_J   ,KC_K   ,KC_L   ,KC_SCLN,KC_QUOT,
        KC_N   ,KC_M   ,KC_COMM,KC_DOT ,KC_SLSH,KC_RSFT,
                KC_UP  ,KC_DOWN,KC_LBRC,KC_RBRC,
KC_RGUI,KC_RCTL,
KC_PGUP,
KC_PGDN,KC_ENTER ,KC_SPC
),

[_CMKMDH] = LAYOUT(
_______,_______,_______,_______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        _______,KC_Q   ,KC_W   ,KC_F   ,KC_P   ,KC_B   ,
        _______,KC_A   ,KC_R   ,KC_S   ,KC_T   ,KC_G   ,
        _______,KC_Z   ,KC_X   ,KC_C   ,KC_D   ,KC_V   ,
                _______,_______,_______,_______,
                                                _______,_______,
                                                        _______,
                                        _______,_______,_______,

_______,_______,_______,_______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        KC_J   ,KC_L   ,KC_U   ,KC_Y   ,KC_SCLN,_______,
        KC_M   ,KC_N   ,KC_E   ,KC_I   ,KC_O   ,_______,
        KC_K   ,KC_H   ,KC_COMM,KC_DOT ,KC_SLSH,_______,
                _______,_______,_______,_______,
_______,_______,
_______,
_______,_______,_______
),
[_KEYPAD] = LAYOUT(
_______,_______,_______,_______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
                _______,KC_INS ,_______,_______,
                                                _______,_______,
                                                        _______,
                                        _______,_______,_______,

_______,_______,_______,_______,_______,_______,_______,_______,_______,
        _______,KC_NLCK,KC_PEQL,KC_PSLS,KC_PAST,_______,
        _______,KC_P7  ,KC_P8  ,KC_P9  ,KC_PMNS,_______,
        _______,KC_P4  ,KC_P5  ,KC_P6  ,KC_PPLS,_______,
        _______,KC_P1  ,KC_P2  ,KC_P3  ,KC_PENT,_______,
                _______,_______,KC_PDOT,KC_PENT,
_______,_______,
_______,
_______,_______,KC_P0
),
[_SYMBOL] = LAYOUT(
_______,_______,_______,_______,_______,_______,_______,_______,_______,
        KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,
        _______,_______,KC_LT  ,KC_LCBR,KC_BSLS,KC_TILD,
        _______,KC_QUOT,KC_EQL ,KC_MINS,KC_LPRN,KC_PLUS,
        _______,KC_GRV ,KC_QUES,KC_HASH,KC_LBRC,KC_PIPE,
                _______,_______,_______,_______,
                                                _______,_______,
                                                        _______,
                                        _______,_______,_______,

_______,_______,_______,_______,_______,_______,_______,_______,_______,
        KC_F7  ,KC_F8  ,KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,
        KC_PERC,KC_AT  ,KC_RCBR,KC_GT  ,_______,_______,
        KC_ASTR,KC_RPRN,KC_UNDS,KC_SLSH,KC_DQUO,_______,
        KC_AMPR,KC_RBRC,KC_DLR ,KC_EXLM,KC_CIRC,_______,
                _______,_______,_______,_______,
_______,_______,
_______,
_______,_______,_______
),
[_DUMMY] = LAYOUT(
_______,_______,_______,_______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
                _______,_______,_______,_______,
                                                _______,_______,
                                                        _______,
                                        _______,_______,_______,

_______,_______,_______,_______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,
                _______,_______,_______,_______,
_______,_______,
_______,
_______,_______,_______
),
};

void matrix_init_user(void) {

}

//void matrix_scan_user(void) {
//
//}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}

void led_set_user(uint8_t usb_led) {

}

qk_tap_dance_action_t tap_dance_actions[] = {
  [X_CTL] = ACTION_TAP_DANCE_MULTI(KC_X, KC_LCTRL, KC_ESC, KC_LALT, KC_3, KC_Q, KC_Z),
// ACTION_TAP_DANCE_QUAD(KC_X, KC_LCTRL, KC_ESC, KC_LALT),
//*/
};
