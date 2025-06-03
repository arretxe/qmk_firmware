/*
Copyright 2019 Stephen Peery

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H
#include "process_midi.h"
#include "raw_hid.h"
#include "qmk_rc.h"
#include "rgb_matrix.h"

#ifdef RAW_ENABLE
#define QMK_RC_BUFFER_MAX 64//XAM 32
uint8_t qmk_rc_buffer[QMK_RC_BUFFER_MAX] = {};

void raw_hid_receive(uint8_t *data, uint8_t length) {
  qmk_rc_receive(qmk_rc_buffer, QMK_RC_BUFFER_MAX, data, length);
}
#endif

extern matrix_row_t raw_matrix[MATRIX_ROWS]; //raw values
extern matrix_row_t last_matrix[MATRIX_ROWS];  // raw values
extern matrix_row_t matrix[MATRIX_ROWS]; //debounced values


// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_names {
    _MIDI,
    _BELOW,
	_MIDIALT,
	_BELOWALT,
	_BASE,
    _FN,
};

/*enum my_keycodes {
  FOO = SAFE_RANGE,
  BAR
};*/

//XAM TODO: beste bi kapa, base letrak, ta beste bi kapatan midik -> FN+zeoze izatea beste kapata saltatzeko, ta berriz berdine bueltatzeko
//MIDI modun daonen LED-a piztu?
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {//TODO: XAM: repe daren botoiai belozidade pixkat diferentea eman proatzeko?
    /*  Row:        0           1           2           3        4           5        6           7           8           9           10          11          12          13          14          15      */
    [_MIDI] = { {   KC_ESC,     KC_NO,      MI_A_4,     MI_Fs_4, MI_E_4,     MI_C_4,  MI_A_3,     MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_Ds_3,    MI_Gs_2,    MO(_FN),    MI_OCTU},//MI_OCTD},//KC_DEL  },
                {   KC_GRV,     MI_D_5,     MI_B_4,     MIr_A_4, MI_F_4,     MI_D_4,  MI_B_3,     MIr_A_3,    MI_F_3,     MI_D_3,     MI_B_2,     MI_Gs_3,    MI_Ds_4,    KC_BSPC,    KC_NO,      MI2_OCTU},//MI_OCTU},//KC_HOME },
                {   KC_TAB,     KC_Q,       KC_W,       KC_E,    KC_R,       KC_T,    KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,    KC_NO,      MI_TRNSU},//MI_TRNSD},//KC_PGUP },
                {   KC_CAPS,    KC_A,       KC_S,       KC_D,    MI4_F,      MI2_A_1, MI4_D,      MI2_Fs_1,   MI4_A,      MI3_C_2,    KC_SCLN,    KC_QUOT,    KC_NO,      KC_ENT,     KC_NO,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
                {   KC_LSFT,    KC_NO,      KC_Z,       KC_X,    MI4_C,      MI2_E_1, MI4_G,      MI2_B_1,    MI4_E,      MI2_Gs_1,   KC_DOT,     KC_SLSH,    KC_RSFT,    KC_UP,      KC_NO,      MI_TRNS_3},//MI2_OCTD  },
                {   MO(_FN),    KC_LGUI,    KC_LALT,    KC_NO,   KC_NO,      KC_NO,   MO(_BELOW), MO(_FN),    MO(_FN),    MO(_FN),    MO(_BELOW), MO(_FN),    MO(_FN),    KC_DOWN,    KC_NO,      TRIKL_REG_TGL},//MI_TRNS_N2},//MI2_OCTU }
              },
    [_BELOW]= { {   _______,    _______,     MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_Cs_3,    MI_As_2,    _______,    MI_OCTD  },
                {   _______,    _______,     MI_E_5,    MI_C_5,  MIr_G_4,    MI_E_4,  MI_C_4,     MIr_G_3,   MI_E_3,     MI_C_3,     MIr_G_2,    MI_As_3,    MI_Cs_4,    _______,    _______,    MI2_OCTD},//MI_OCTU  },
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    MI_TRNSD},//MI_TRNSD },
                {   _______,    _______,    _______,    _______, MI4_F,      MI2_A_1, MI4_D,     MI2_Fs_1,   MI4_A,      MI3_C_2,    _______,    _______,    _______,    _______,    _______,    MI_TRNS_2},//MI_TRNSU },
                {   _______,    _______,    _______,    _______, MI4_C,      MI2_E_1, MI4_G,     MI2_B_1,    MI4_E,      MI2_Gs_1,   _______,    _______,    _______,    _______,    _______,    MI_TRNS_5},//MI2_OCTD },
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TG(_MIDIALT)},//MI2_OCTU }
              },
	[_MIDIALT] = { {KC_ESC,     KC_NO,      MI_A_4,     MI_Fs_4, MI_E_4,     MI_C_4,  MI_A_3,     MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_Ds_3,    MI_Gs_2,    MO(_FN),    MI_OCTU},//MI_OCTD},//KC_DEL  },
                {   KC_GRV,     MI_D_5,     MI_B_4,     MI_G_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_G_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_Gs_3,    MI_Ds_4,    KC_BSPC,    KC_NO,      MI2_OCTU},//MI_OCTU},//KC_HOME },
                {   KC_TAB,     KC_Q,       KC_W,       KC_E,    KC_R,       KC_T,    KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,    KC_NO,      MI_TRNSU},//MI_TRNSD},//KC_PGUP },
                {   KC_CAPS,    KC_A,       KC_S,       KC_D,    MI4_F,      MI2_A_1, MI4_D,      MI2_Fs_1,   MI4_A,      MI3_C_2,    KC_SCLN,    KC_QUOT,    KC_NO,      KC_ENT,     KC_NO,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
                {   KC_LSFT,    KC_NO,      KC_Z,       KC_X,    MI4_C,      MI2_E_1, MI4_G,      MI2_B_1,    MI4_E,      MI2_Gs_1,   KC_DOT,     KC_SLSH,    KC_RSFT,    KC_UP,      KC_NO,      MI_TRNS_3},//MI2_OCTD  },
                {   MO(_FN),    KC_LGUI,    KC_LALT,    KC_NO,   KC_NO,      KC_NO,   MO(_BELOWALT), MO(_FN),    MO(_FN),    MO(_FN),    MO(_BELOW), MO(_FN),    MO(_FN),    KC_DOWN,    KC_NO,      TRIKL_REG_TGL},//MI_TRNS_N2},//MI2_OCTU }
              },
    [_BELOWALT]= { {_______,    _______,     MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_Cs_3,    MI_As_2,    _______,    MI_OCTD  },
                {   _______,    _______,     MI_E_5,    MI_C_5,  MI_A_4,     MI_E_4,  MI_C_4,     MI_A_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_As_3,    MI_Cs_4,    _______,    _______,    MI2_OCTD},//MI_OCTU  },
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    MI_TRNSD},//MI_TRNSD },
                {   _______,    _______,    _______,    _______, MI4_F,      MI2_A_1, MI4_D,     MI2_Fs_1,   MI4_A,      MI3_C_2,    _______,    _______,    _______,    _______,    _______,    MI_TRNS_2},//MI_TRNSU },
                {   _______,    _______,    _______,    _______, MI4_C,      MI2_E_1, MI4_G,     MI2_B_1,    MI4_E,      MI2_Gs_1,   _______,    _______,    _______,    _______,    _______,    MI_TRNS_5},//MI2_OCTD },
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TG(_MIDIALT)},//MI2_OCTU }
              },

    [_BASE] = { {   KC_ESC,     KC_NO,      KC_F1,      KC_F2,   KC_F3,      KC_F4,   KC_F5,     KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     MO(_FN),    KC_DEL  },
                {   KC_GRV,     KC_1,       KC_2,       KC_3,    KC_4,       KC_5,    KC_6,      KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,     KC_BSPC,    KC_NO,      KC_HOME },
                {   KC_TAB,     KC_Q,       KC_W,       KC_E,    KC_R,       KC_T,    KC_Y,      KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,    KC_NO,      KC_PGUP },
                {   KC_CAPS,    KC_A,       KC_S,       KC_D,    KC_F,       KC_G,    KC_H,      KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,    KC_NO,      KC_ENT,     KC_NO,      KC_PGDN },
                {   KC_LSFT,    KC_NO,      KC_Z,       KC_X,    KC_C,       KC_V,    KC_B,      KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_RSFT,    KC_UP,      KC_NO,      KC_END  },
                {   _______,    KC_LGUI,    KC_LALT,    KC_NO,   KC_NO,      KC_NO,   KC_SPC,    KC_NO,      KC_NO,      KC_NO,      KC_SPC,    MO(_FN),    MO(_FN),    KC_DOWN,    KC_NO,      KC_RGHT }
              },
    [_FN]   = { {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RESET  },
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TG(_BASE)},
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_TOG },//on/off RGB
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_MOD },//RGB effect change
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_SAI },//SATURATION
                {   _______,    _______,    _______,    _______, _______,    _______, KC_LSFT,   _______,    _______,    _______,    KC_LSFT,    _______,    _______,    _______,    _______,    RGB_HUI }//HUE
             }
};


uint8_t below_press_num = 0;
uint8_t rgb_matrix_current_mode = 0;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case MO(_BELOW):
	
		if (record->event.pressed) {
			below_press_num++;
			if (below_press_num == 2) return false;
		} else {
			below_press_num--;
			if (below_press_num == 1) return false;
		}
	//both, when pressing and when releasing
		//process_midi_all_notes_off();//TODO, try removing this, just set matrix as all released and it should handle it (i could do it only for first two rows!)
		for (uint8_t i = 0; i <= 1; i++) {//release first two rows
			raw_matrix[i] = 0;
			last_matrix[i] = 0;//TODO test without this//set to previously 0, so durin next check when it sees it at '1' will send noteon
			matrix[i] = 0;
		}
		/*raw_matrix[5] |= (MATRIX_ROW_SHIFTER << 6);
		last_matrix[5] |= (MATRIX_ROW_SHIFTER << 6);
		matrix[5] |= (MATRIX_ROW_SHIFTER << 6);*/
		return true;
		
	case TG(_BASE) :
	    //if (record->event->key.row=2 && record->event->key.column=15) {
			if (record->event.pressed) {
				if (layer_state_is(_BASE)) {//if base is enabled, it is about to be disabled, so turn LED off
					writePin(LED_CAPS_LOCK_PIN, 1);
					rgb_matrix_mode_noeeprom(rgb_matrix_current_mode);
				} else {
					rgb_matrix_current_mode = rgb_matrix_get_mode();
					rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_IX10_SOLID);//IX10_SOLID
					#ifdef MIDI_ENABLE
					process_midi_all_notes_off();
					#endif
					writePin(LED_CAPS_LOCK_PIN, 0);
					//rgb_matrix_disable_noeeprom()	)//TODO: Turn off LED effects to allow raw messages, or maybe set some custom lighting with only non-playing leds on
				}
			}
		//}
		return true;
  }
  return true;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}

extern rgb_config_t rgb_matrix_config;
void rgb_matrix_indicators_user() {
	if (IS_LAYER_ON(_BASE)) {
     //TODO: for x check the table and turn leds accordingly
		for (uint8_t i = 0;  i < DRIVER_LED_TOTAL; i++) {
			//if (LED_OVERRIDES[i] == 1){
				//r1        r2                   r3                  r4                  r5=space
		    if ((i<=12) || (i>=17 && i<=27) || (i>=49 && i<=54) || (i>=62 && i<=67) || (i==76)) {
				rgb_matrix_set_color(i, LED_OVERRIDES[i].R, LED_OVERRIDES[i].G, LED_OVERRIDES[i].B);
            } else {
				RGB rgb = hsv_to_rgb(rgb_matrix_config.hsv);
			    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
			//rgb_matrix_set_color(i, 0, 0, 0);
			}
		}
	}
}