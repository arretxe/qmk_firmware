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
#include "qmk_midi.h"
//#include "raw_hid.h"
//#include "qmk_rc.h"
#include "rgb_matrix.h"

#include "color.h"
#include "quantum.h" //for reset eeprom



extern matrix_row_t raw_matrix[MATRIX_ROWS]; //raw values
extern matrix_row_t last_matrix[MATRIX_ROWS];  // raw values

bool is_key_led(uint8_t i) {
    bool led = false;
#if defined(TRIKL_IX10_T)
    led = ((i<=13) || (i>=17 && i<=27) || (i>=49 && i<=54) || (i>=62 && i<=67));// || (i==76));
#elif defined(TRIKL_IX11_T)
    // led = ((i<=12) || (i>=16 && i<=27) || (i>=31 && i<=42) || (i>=49 && i<=54) || (i>=62 && i<=67) || (i==76));
    led = ((i>=1 && i<=13) || (i>=17 && i<=27) || (i>=32 && i<=41) || (i>=47 && i<=56) || (i>=60 && i<=69));// || (i==76));
#elif defined(TRIKL_IX11CR_T)
    led = ((i<=12) || (i>=16 && i<=27) || (i>=31 && i<=42) || (i>=46 && i<=56) || (i>=59 && i<=69));// || (i==76));
#elif defined(TRIKL_IXLR_T)
    led = ((i<=13) || (i>=16 && i<=28) || (i>=31 && i<=42) || (i>=46 && i<=56) || (i>=59 && i<=69));// || (i==76));
#elif defined(TRIKL_IXLL_T)
    led = ((i<=13) || (i>=16 && i<=28) || (i>=31 && i<=42) || (i>=46 && i<=56) || (i>=59 && i<=69));// || (i==76));
#endif
    return led;
}
extern matrix_row_t matrix[MATRIX_ROWS]; //debounced values
void enter_drum_mode(bool IN_OUT);

// void bootmagic_lite(void) {
    // We need multiple scans because debouncing can't be turned off.
    //matrix_scan();
	// matrix_scan_keys(raw_matrix,0);
	// matrix_scan();
// #if defined(DEBOUNCE) && DEBOUNCE > 0
    // wait_ms(DEBOUNCE * 2);
// #else
    // wait_ms(10);
// #endif
    //matrix_scan();
	// matrix_scan_keys(raw_matrix,0);
    // matrix_scan();
    // If the configured key (commonly Esc) is held down on power up,
    // reset the EEPROM valid state and jump to bootloader.
    // This isn't very generalized, but we need something that doesn't
    // rely on user's keymaps in firmware or EEPROM.
    
	//uint8_t row = BOOTMAGIC_LITE_ROW;
    //uint8_t col = BOOTMAGIC_LITE_COLUMN;



    //if (matrix_get_row(row) & (1 << col)) {
		// if(raw_matrix[0] != 0){//only first button is pressed
        // //bootmagic_lite_reset_eeprom();// custom itteko aukera emtaue berezs eztu eze itten
        // eeconfig_init(); //XAM proatzeko gehittuet
		// eeconfig_disable();//XAM kenduta neuken baino garbi itteko hola igual hobe?
        // // Jump to bootloader.
        // bootloader_jump();
    // }
// }

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_names {
	/*_BASE,*/
#if defined(MIDI_TRIKL)
  _R1,
  _L1,
  _RB1,
  _LB1,
	_R2,
	_L2,
	_RB2,
	_LB2,
	_R3,
	_L3,
	_RB3,
	_LB3,
	_R4,
	_L4,
	_RB4,
	_LB4,
	_MIDIDRUM,
	_BELOWDRUM,
#endif
	// _BASE,
  _FN,
	_FN2,
};
void matrix_init_user(void) {
    // _R1 is already ON automatically because it is the first layer in the enum, but still explicitly turn it on so that we can then query if it is on
    // Turn on keys layers:
    layer_on(_R1);
    layer_on(_L1);
}


enum my_keycodes {
  FUNC = SAFE_RANGE,
  FUNC2,
  MOB,
  NEXTR,
  NEXTL,
  MOBELOWDRUM,
  PANIKO,
  BELLOW_RGB,
  BASS_MUTE
};

//XAM TODO: beste bi kapa, base letrak, ta beste bi kapatan midik -> FN+zeoze izatea beste kapata saltatzeko, ta berriz berdine bueltatzeko
//MIDI modun daonen LED-a piztu?
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { //TODO: XAM: repe daren botoiai belozidade pixkat diferentea eman proatzeko?
    /*  Row:        0           1           2           3        4           5        6           7           8           9           10          11          12          13          14            15      */

#if defined(MIDI_TRIKL)    
  #if defined(TRIKL_IX10_T)  
  // //TRIKI
  //  [_R1] =      { {   FUNC, _______,      MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_Ds_3,    MI_Gs_2,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
  //                  {   _______,  _______,      MI_B_4,   MI_A_4,  MI_F_4,     MI_D_4,    MI_B_3,    MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_Gs_3,    MI_Ds_4, _______,    _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
  //                  {   _______, _______,   _______,    _______,   _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,    _______, _______, _______,       MI2_OCTU},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
  //                  {   _______, _______,      _______,  _______,   MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,     _______,    _______,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
  //                  {   _______, _______,      _______,  _______,   MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,    _______,    _______,     _______,    _______,      MI_TRNS_3},//MI2_OCTD  },
  //                  {   _______, _______,      _______,  _______,   _______,    _______, MOB,    _______,    _______,     _______,   MOB,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
  //                },
  
  //      [_RB1]= { {   FUNC,     _______,  MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_Cs_3,    MI_As_2,    _______,    MI_TRNSD},//MI_OCTD  },
  //                  {   _______,     _______,      MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_As_3,    MI_Cs_4,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
  //                  {   _______,               _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______, _______, _______, MI2_OCTD},//MI_VELD MI_TRNSD},//MI_TRNSD },
  //                  {   _______,     _______,      _______,     _______, MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,    _______,    _______,    PC_UP_1},//TO(_MIDIDRUM)},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
  //                  {   _______,     _______,      _______,     _______, MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
  //                  {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
  //                },//------
  //  [_L1] =      { {   FUNC, _______,      MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_Ds_3,    MI_Gs_2,     _______,         _______},//MI_OCTU},//MI_OCTD},//KC_DEL  },
  //                  {   _______,  _______,      MI_B_4,   MI_A_4,  MI_F_4,     MI_D_4,    MI_B_3,    MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_Gs_3,    MI_Ds_4, _______,    _______,      _______},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
  //                  {   _______, _______,   _______,    _______,   _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,    _______, _______, _______,       _______},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
  //                  {   _______, _______,      _______,  _______,   MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,     _______,    _______,      _______},//MI_TRNSU},//KC_PGDN },
  //                  {   _______, _______,      _______,  _______,   MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,    _______,    _______,     _______,    _______,      _______},//MI2_OCTD  },
  //                  {   _______, _______,      _______,  _______,   _______,    _______, _______,    _______,    _______,     _______,   _______,    _______,    _______,     _______,    _______,      _______},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
  //                },
  
  //      [_LB1]= { {   FUNC,     _______,  MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_Cs_3,    MI_As_2,    _______,    _______},//MI_OCTD  },
  //                  {   _______,     _______,      MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_As_3,    MI_Cs_4,    _______,    _______,    _______},//MI2_OCTD},//MI_OCTU  },
  //                  {   _______,               _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______, _______, _______, _______},//MI_VELD MI_TRNSD},//MI_TRNSD },
  //                  {   _______,     _______,      _______,     _______, MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,    _______,    _______,    _______},//TO(_MIDIDRUM)},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
  //                  {   _______,     _______,      _______,     _______, MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,    _______,    _______,    _______,    _______,    _______},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
  //                  {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
  //                },//------
//TAPIA
/*[_MIDI] = { {   FUNC, _______,      MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_Ds_3,    MI_Gs_2,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
                {   _______,     _______,      MI_B_4,     MI_G_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_G_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_Gs_3,    MI_Ds_4, _______,    _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
                {   _______, _______,   _______,    _______,   _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,    _______, _______, _______,       MI_VELU},//MI_VELUMI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
//                {   _______, _______,      _______,  _______,   MI2_F_1,    MI2_Fs_1, MI2_D_1,    MI2_Ds_1,   MI2_A_1,    MI2_As_1,   _______,    _______,    _______,     _______,    _______,      MI2_OCTD},//MI_TRNSU},//KC_PGDN },
//                {   _______, _______,      _______,  _______,   MI2_C_1,    MI2_Cs_1, MI2_G_1,    MI2_Gs_1,    MI2_E_1,    MI2_B_1,   _______,    _______,    _______,     _______,    _______,      MI_TRNS_3},//MI2_OCTD  },
                {   _______, _______,      _______,  _______,   MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,     _______,    _______,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
                {   _______, _______,      _______,  _______,   MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,    _______,    _______,     _______,    _______,      MI_TRNS_3},//MI2_OCTD  },
                {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
              },

    [_BELOW]= { {   FUNC,     _______,  MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_Cs_3,    MI_As_2,    _______,    MI_TRNSD},//MI_OCTD  },
                {   _______,     _______,      MI_E_5,    MI_C_5,  MI_A_4,     MI_E_4,  MI_C_4,     MI_A_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_As_3,    MI_Cs_4,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
                {   _______,               _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______, _______, _______, MI_VELD},//MI_VELD MI_TRNSD},//MI_TRNSD },
//                {   _______,     _______,      _______,     _______, MI2_F_1,    MI2_Fs_1, MI2_D_1,    MI2_Ds_1,   MI2_A_1,    MI2_As_1,    _______,    _______,    _______,    _______,    _______,    MI_TRNS_2},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
//                {   _______,     _______,      _______,     _______, MI2_C_1,    MI2_Cs_1, MI2_G_1,    MI2_Gs_1,    MI2_E_1,    MI2_B_1,   _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
                {   _______,     _______,      _______,     _______, MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,    _______,    _______,    MI_TRNS_5},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
                {   _______,     _______,      _______,     _______, MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
                {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
              },*///------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// DAN AUSTRALIA TODO
   [_R1] =      { {   FUNC, _______,      MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_Ds_3,    MI_Gs_2,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
                   {   _______,  _______,      MI_B_4,   MI_A_4,  MI_F_4,     MI_D_4,    MI_B_3,    MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_Gs_3,    MI_Ds_4, _______,    _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
                   {   _______, _______,   _______,    _______,   _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,    _______, _______, _______,       MI2_OCTU},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
                   {   _______, _______,      _______,  _______,   _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,     _______,    _______,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
                   {   _______, _______,      _______,  _______,   _______,    _______, _______,    _______,    _______,    _______,   _______,    _______,    _______,     _______,    _______,      MI_TRNS_3},//MI2_OCTD  },
                   {   _______, _______,      _______,  _______,   _______,    _______, MOB,    _______,    _______,     _______,   MOB,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
                 },
  
       [_RB1]= { {   FUNC,     _______,  MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_Cs_3,    MI_As_2,    _______,    MI_TRNSD},//MI_OCTD  },
                   {   _______,     _______,      MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_As_3,    MI_Cs_4,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
                   {   _______,               _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______, _______, _______, MI2_OCTD},//MI_VELD MI_TRNSD},//MI_TRNSD },
                   {   _______,     _______,      _______,     _______, _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    PC_UP_1},//TO(_MIDIDRUM)},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
                   {   _______,     _______,      _______,     _______, _______,    _______, _______,    _______,    _______,    _______,   _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
                   {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
                 },//------
   [_L1] =      { {   FUNC, _______,      _______,   _______,   _______,    _______,    _______,    _______,    _______,     _______,     _______,     _______,    _______,    _______,     _______,         _______},//MI_OCTU},//MI_OCTD},//KC_DEL  },
                   {   _______,  _______,      _______,   _______,  _______,     _______,    _______,    _______,     _______,     _______,     _______,     _______,    _______, _______,    _______,      _______},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
                   {   _______, _______,   _______,    _______,   _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,    _______, _______, _______,       _______},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
                   {   _______, _______,      _______,  _______,   MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,     _______,    _______,      _______},//MI_TRNSU},//KC_PGDN },
                   {   _______, _______,      _______,  _______,   MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,    _______,    _______,     _______,    _______,      _______},//MI2_OCTD  },
                   {   _______, _______,      _______,  _______,   _______,    _______, _______,    _______,    _______,     _______,   _______,    _______,    _______,     _______,    _______,      _______},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
                 },
  
       [_LB1]= { {   FUNC,     _______,  _______,    _______,  _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,     _______,    _______,    _______,    _______},//MI_OCTD  },
                   {   _______,     _______,      _______,    _______,  _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,    _______,    _______,    _______,    _______},//MI2_OCTD},//MI_OCTU  },
                   {   _______,               _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______, _______, _______, _______},//MI_VELD MI_TRNSD},//MI_TRNSD },
                   {   _______,     _______,      _______,     _______, MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,    _______,    _______,    _______},//TO(_MIDIDRUM)},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
                   {   _______,     _______,      _______,     _______, MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,    _______,    _______,    _______,    _______,    _______},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
                   {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }        
                 },//------
                   ////// PICHARDO
/*  [_MIDI] = { {   FUNC, _______,      MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_D_2,    MI_Ds_3,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
                {   _______,     _______,      MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_G_2,    MI_Gs_3, _______,    _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
                {   _______, _______,   _______,    _______,   _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,    _______, _______, _______,       MI2_OCTU},//MI_VELUMI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
//                {   _______, _______,      _______,  _______,   MI2_F_1,    MI2_Fs_1, MI2_D_1,    MI2_Ds_1,   MI2_A_1,    MI2_As_1,   _______,    _______,    _______,     _______,    _______,      MI2_OCTD},//MI_TRNSU},//KC_PGDN },
//                {   _______, _______,      _______,  _______,   MI2_C_1,    MI2_Cs_1, MI2_G_1,    MI2_Gs_1,    MI2_E_1,    MI2_B_1,   _______,    _______,    _______,     _______,    _______,      MI_TRNS_3},//MI2_OCTD  },
                {   _______, _______,      _______,  _______,   MI2_F_1,    MI3_A_1, MI2_C_1,    MI4_Ds_1,   MI2_E_1,    MI4_G_1,     _______,    _______,    _______,     _______,    _______,      MI_TRNS_3},//MI_TRNSU},//KC_PGDN },
                {   _______, _______,      _______,  _______,   MI2_G_1,    MI3_B_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI3_Cs_2,   _______,    _______,    _______,     _______,    _______,      MI_TRNS_0},//MI2_OCTD  },
                {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
              },

    [_BELOW]= { {   FUNC,     _______,  MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_B_1,    MI_Cs_3,    _______,    MI_TRNSD},//MI_OCTD  },
                {   _______,     _______,      MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_E_2,    MI_As_3,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
                {   _______,               _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______, _______, _______, MI2_OCTD},//MI_VELD MI_TRNSD},//MI_TRNSD },
//                {   _______,     _______,      _______,     _______, MI2_F_1,    MI2_Fs_1, MI2_D_1,    MI2_Ds_1,   MI2_A_1,    MI2_As_1,    _______,    _______,    _______,    _______,    _______,    MI_TRNS_2},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
//                {   _______,     _______,      _______,     _______, MI2_C_1,    MI2_Cs_1, MI2_G_1,    MI2_Gs_1,    MI2_E_1,    MI2_B_1,   _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
                {   _______,     _______,      _______,     _______, MI2_A_1,    MI3_Cs_2, MI2_E_1,    MI3_Gs_1,   MI2_B_1,    MI3_Ds_1,    _______,    _______,    _______,    _______,    _______,    MI_TRNS_3},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
                {   _______,     _______,      _______,     _______, MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_D_1,    MI3_Fs_1,   _______,    _______,    _______,    _______,    _______,    MI_TRNS_0},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
                {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
              },*///------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//irish                                                                                                                                                                     //MI_TRNSU  MI_OCTU MI_TRNS_0 MI_TRNS_N2 TRIKL_ALT_TGL TRIKL_REG_TGL TO(_MIDIDRUM) MI_BENDD BASS_MUTE
    // [_MIDI] = { {   FUNC, _______,      MI_Cs_5,   MI_As_4,   MI_Gs_4,    MI_E_4,    MI_Cs_4,    MI_As_3,    MI_Gs_3,     MI_E_3,     MI_Cs_3,     MI_As_2,    MI_Gs_2,    MI_E_2,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
    //             {   _______,     _______,      MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,    MI_B_3,     MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_A_2,    MI_F_2, _______,    _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
    //             {   _______, _______,   _______,    _______,   _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,    _______, _______, _______,       MI2_OCTU},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
    //             {   _______, _______,      _______,  _______,   _______,    MI2_F_1, MI3_A_1,    MI2_A_1,    MI3_Cs_2,    _______,    _______,    _______,    _______,     _______,    _______,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
    //             {   _______, _______,      _______,  _______,   _______,    MI2_G_1, MI3_B_1,    MI2_D_1,    MI3_Fs_1,    _______,   _______,    _______,    _______,     _______,    _______,      MI_TRNS_2},//MI2_OCTD  },
    //             {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
    //           },

    // [_BELOW]= { {   FUNC,     _______,  MI_Fs_5,    MI_Ds_5,  MI_B_4,     MI_Fs_4,  MI_Ds_4,     MI_B_3,    MI_Fs_3,     MI_Ds_3,     MI_B_2,     MI_Fs_2,     MI_Ds_2,    MI_B_1,    _______,    MI_TRNSD},//MI_OCTD  },
    //             {   _______,     _______,      MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,    MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_E_2,    MI_C_2,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
    //             {   _______,               _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
    //             {   _______,     _______,      _______,     _______, _______,    MI2_D_1, MI3_Fs_1,    MI2_E_1,   MI3_Gs_1,    _______,    _______,    _______,    _______,    _______,    _______,    MI_TRNS_0},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
    //             {   _______,     _______,      _______,     _______, _______,    MI2_C_1, MI3_E_1,    MI2_G_1,    MI3_B_1,    _______,   _______,    _______,    _______,    _______,    _______,    MI_TRNS_2},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
    //             {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//irish John                                                                                                                                                                     //MI_TRNSU  MI_OCTU MI_TRNS_0 MI_TRNS_N2 TRIKL_ALT_TGL TRIKL_REG_TGL TO(_MIDIDRUM) MI_BENDD BASS_MUTE
    // [_MIDI] = { {   FUNC, _______,      MI_Cs_5,   MI_As_4,   MI_Gs_4,    MI_E_4,    MI_Cs_4,    MI_As_3,    MI_Gs_3,     MI_E_3,     MI_Cs_3,     MI_As_2,    MI_Gs_2,    MI_Fs_2,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
    //             {   _______,     _______,      MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,    MI_B_3,     MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_A_2,    MI_Cs_2, _______,    _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
    //             {   _______, _______,   _______,    _______,   _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,    _______, _______, _______,       MI2_OCTU},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
    //             {   _______, _______,      _______,  _______,   MI2_A_1, MI4_C_2,    MI2_F_1, MI3_A_1,    MI2_A_1,    MI3_Cs_2,       _______,    _______,    _______,     _______,    _______,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
    //             {   _______, _______,      _______,  _______,   MI2_D_1, MI4_F_1,    MI2_G_1, MI3_B_1,    MI2_D_1,    MI3_Fs_1,       _______,    _______,    _______,     _______,    _______,      MI_TRNS_1},//MI2_OCTD  },
    //             {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
    //           },

    // [_BELOW]= { {   FUNC,     _______,  MI_Fs_5,    MI_Ds_5,  MI_B_4,     MI_Fs_4,  MI_Ds_4,     MI_B_3,    MI_Fs_3,     MI_Ds_3,     MI_B_2,     MI_Fs_2,     MI_Ds_2,    MI_D_2,    _______,    MI_TRNSD},//MI_OCTD  },
    //             {   _______,     _______,      MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,    MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_E_2,    MI_Cs_2,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
    //             {   _______,               _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
    //             {   _______,     _______,      _______,     _______, MI2_E_1,  MI4_G_1,   MI2_D_1, MI3_Fs_1,    MI2_E_1,   MI3_Gs_1,       _______,    _______,    _______,    _______,    _______,    MI_TRNS_2},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
    //             {   _______,     _______,      _______,     _______, MI2_G_1,  MI4_As_1,   MI2_C_1, MI3_E_1,    MI2_G_1,    MI3_B_1,       _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
    //             {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
    //           },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            /*LOGAN G/G#*//*[_MIDI] = { {FUNC, _______,      MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_E_2,    MI_C_2,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
                {   _______,     _______,      MI_G_4,     MI_F_4,  MI_Cs_4,     MI_As_3,  MI_G_3,     MI_F_3,     MI_Cs_3,     MI_As_2,     MI_G_2,     MI_F_2,    MI_Cs_2, _______,    _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
                {   _______, _______,   _______,    _______,   _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,    _______, _______, _______,       MI_VELU},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
//                {   _______, _______,      _______,  _______,   MI2_F_1,    MI2_Fs_1, MI2_D_1,    MI2_Ds_1,   MI2_A_1,    MI2_As_1,   _______,    _______,    _______,     _______,    _______,      MI2_OCTD},//MI_TRNSU},//KC_PGDN },
//                {   _______, _______,      _______,  _______,   MI2_C_1,    MI2_Cs_1, MI2_G_1,    MI2_Gs_1,    MI2_E_1,    MI2_B_1,   _______,    _______,    _______,     _______,    _______,      MI_TRNS_3},//MI2_OCTD  },
                {   _______, _______,      _______,  _______,   MI2_Fs_1,    MI3_As_2, MI2_As_1,    MI4_Cs_2,   MI2_G_1,    MI3_B_2,    _______,    _______,    _______,     _______,    _______,      MI_TRNS_6},//MI_TRNSU},//KC_PGDN },
                {   _______, _______,      _______,  _______,   MI2_Gs_1,    MI3_C_1, MI2_Ds_1,    MI3_G_1,    MI2_Cs_1,    MI3_F_1,   _______,    _______,    _______,     _______,    _______,      MI_TRNS_7},//MI2_OCTD  },
                {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
              },

    [_BELOW]= { {   FUNC,     _______,  MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_Cs_2,    MI_G_1,    _______,    MI_TRNSD},//MI_OCTD  },
                {   _______,     _______,      MI_C_5,    MI_Gs_4,  MI_Ds_4,     MI_C_4,  MI_Gs_3,     MI_Ds_3,    MI_C_3,     MI_Gs_2,     MI_Ds_2,     MI_C_2,    MI_Gs_1,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
                {   _______,               _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______, _______, _______, MI_VELD},//MI_TRNSD},//MI_TRNSD },
//                {   _______,     _______,      _______,     _______, MI2_F_1,    MI2_Fs_1, MI2_D_1,    MI2_Ds_1,   MI2_A_1,    MI2_As_1,    _______,    _______,    _______,    _______,    _______,    MI_TRNS_2},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
//                {   _______,     _______,      _______,     _______, MI2_C_1,    MI2_Cs_1, MI2_G_1,    MI2_Gs_1,    MI2_E_1,    MI2_B_1,   _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
                {   _______,     _______,      _______,     _______, MI2_B_1,    MI3_Ds_1, MI2_F_1,    MI3_A_1,   MI2_C_1,    MI3_E_2,    _______,    _______,    _______,    _______,    _______,    MI_TRNS_4},//TO(_MIDIALT)},//TO(_MIDIDRUM)},//MI_TRNSU },
                {   _______,     _______,      _______,     _______, MI2_Cs_1,    MI3_F_1, MI2_Gs_1,    MI3_C_1,    MI2_Ds_1,    MI3_G_1,   _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
                {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
              },*///------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // [_MIDIALT] = { {  FUNC,     _______,      MI_A_4,     MI_Fs_4, MI_E_4,     MI_C_4,  MI_A_3,     MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_Ds_3,    MI_Gs_2,    _______,    MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
  //               {   _______,     MI_D_5,     MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_G_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_Gs_3,    MI_Ds_4,    _______,    _______,      MI_OCTU},//MI2_OCTU},//MI_OCTU},//KC_HOME },
  //               {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,        MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
  //               {   _______,    _______,       _______,    _______,    MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,     _______,     _______,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
  //               {   _______,    _______,      _______,     _______,    MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,    _______,    _______,    _______,      _______,      MI_TRNS_3},//MI2_OCTD  },
  //               {   _______,       _______,    _______,    _______,   _______,      _______,   MOBELOWALT, _______, _______,    _______,    MOBELOWALT, _______,    _______,    _______,    _______,      TRIKL_REG_TGL},//MI_TRNS_N2},//MI2_OCTU }
  //             },
  //  [_BELOWALT]= { {FUNC,    _______,     MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_Cs_3,    MI_As_2,    _______,    MI_TRNSD},//MI_OCTD  },
  //               {   _______,    _______,     MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_A_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_As_3,    MI_Cs_4,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
  //               {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
  //               {   _______,    _______,    _______,    _______, MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,    _______,    _______,    TO(_MIDI)},//MI_TRNS_6},//MI_TRNSU },
  //               {   _______,    _______,    _______,    _______, MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//MI2_OCTD },
  //               {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//MI2_OCTU }
  //             },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
#elif defined(TRIKL_IX11_T)    
//UK
//    [_MIDI] = { {   FUNC, _______,      MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_D_2,    MI_Ds_3,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                {   _______,     _______,      MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_F_2,    MI_Gs_3, _______,    _______,      MI_OCTD},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                {   _______,               _______,  MI_As_4,   MI_G_4,   MI_Cs_4,   MI_As_3,   MI_G_3,    MI_Cs_3,    MI_As_2,    MI_G_2,  MI_Cs_2,    MI_As_1,    _______, _______, _______, TO(_MIDIALT)},//MI2_OCTU},//
//                /*{   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_Ds_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_Ds_3,    MI_Cs_3,    MI_As_2,    _______, _______, _______, MI2_OCTU},//*/
//                /*{   _______,               MI_E_5,   MI_D_5, MI_As_4,   MI_G_4,   MI_E_4,   MI_D_4,    MI_As_3,    MI_G_3,    MI_E_3,  MI_D_3,  MI_Ds_3,    MI_Gs_2, _______, _______, MI_BENDD},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },*/
//                {   _______,    _______,       _______,       _______,  MI2_F_0,    MI3_A_1, MI2_A_1,    MI4_C_2,     MI2_As_0,    MI3_D_2,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_7},//MI_TRNSU},//KC_PGDN },
//                {   _______,    _______,      _______,     _______,     MI2_G_0,    MI3_B_1, MI2_D_1,    MI3_Fs_1,    MI2_B_0,    MI4_D_2,   _______,     _______,    _______,    _______,      _______,      MI_TRNS_5},//MI2_OCTD  },
//                {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//              },
//
//    [_BELOW]= { {   FUNC,     _______,  MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_C_2,    MI_Cs_3,    _______,    MI_TRNSD},//MI_OCTD  },
//                {   _______,     _______,      MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_E_2,    MI_As_3,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                {   _______,               _______,  MI_Gs_4,   MI_A_4,   MI_Ds_4,   MI_Gs_3,   MI_A_3,    MI_Ds_3,    MI_Gs_2,    MI_A_2,    MI_Ds_2,    MI_Gs_1,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
//                /*{   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_Ds_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_Ds_3,    MI_Cs_3,    MI_As_2,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },*/
//                {   _______,    _______,       _______,       _______,  MI2_F_0,    MI3_A_1, MI2_E_1,    MI3_Gs_1,   MI2_Ds_1,    MI3_G_1,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_2},//MI_TRNSU},//KC_PGDN },
//                {   _______,    _______,      _______,     _______,     MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_Gs_0,    MI3_C_2,   _______,     _______,    _______,    _______,      _______,      TRIKL_DRUM_TGL},//MI2_OCTD  },
//                {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_REG_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//              },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//    [_MIDIALT] = { {   FUNC, _______,      MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_D_2,    MI_Ds_3,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                {   _______,     _______,      MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_As_2,    MI_Gs_3, _______,    _______,      MI_OCTD},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                {   _______,               _______,  MI_As_4,   MI_G_4,   MI_Ds_4,   MI_G_3,   MI_As_3,    MI_Ds_3,    MI_G_2,    MI_G_2,  MI_Cs_2,    MI_As_1,    _______, _______, _______, TO(_MIDI)},//MI2_OCTU},//
//                /*{   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_Ds_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_Ds_3,    MI_Cs_3,    MI_As_2,    _______, _______, _______, MI2_OCTU},//*/
//                /*{   _______,               MI_E_5,   MI_D_5, MI_As_4,   MI_G_4,   MI_E_4,   MI_D_4,    MI_As_3,    MI_G_3,    MI_E_3,  MI_D_3,  MI_Ds_3,    MI_Gs_2, _______, _______, MI_BENDD},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },*/
//                {   _______,    _______,       _______,       _______,  MI2_F_0,    MI3_A_1, MI2_A_1,    MI4_C_2,     MI2_As_0,    MI3_D_2,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_7},//MI_TRNSU},//KC_PGDN },
//                {   _______,    _______,      _______,     _______,     MI2_G_0,    MI3_B_1, MI2_D_1,    MI3_Fs_1,    MI2_D_1,    MI4_F_1,   _______,     _______,    _______,    _______,      _______,      MI_TRNS_5},//MI2_OCTD  },
//                {   _______, _______,      _______,  _______,   _______,    _______, MOBELOWALT,    _______,    _______,     _______,   MOBELOWALT,    _______,    _______,     _______,    _______,      TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//              },
//
//    [_BELOWALT]= { {   FUNC,     _______,  MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_C_2,    MI_Cs_3,    _______,    MI_TRNSD},//MI_OCTD  },
//                {   _______,     _______,      MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_E_2,    MI_As_3,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                {   _______,               _______,  MI_Gs_4,   MI_A_4,   MI_Cs_4,   MI_F_3,   MI_Gs_3,    MI_Cs_3,    MI_A_2,    MI_A_2,    MI_Ds_2,    MI_Gs_1,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
//                /*{   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_Ds_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_Ds_3,    MI_Cs_3,    MI_As_2,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },*/
//                {   _______,    _______,       _______,       _______,  MI2_F_0,    MI3_A_1, MI2_E_1,    MI3_Gs_1,   MI2_B_0,    MI4_D_2,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_2},//MI_TRNSU},//KC_PGDN },
//                {   _______,    _______,      _______,     _______,     MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_A_0,    MI4_C_2,   _______,     _______,    _______,    _______,      _______,      TRIKL_DRUM_TGL},//MI2_OCTD  },
//                {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_REG_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//              },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*MEX34*/     [_R1] = { {   FUNC, _______,  MI_C_5, MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,    MI_Ds_3,  _______ ,    _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
                 {   _______,           MI_F_5,   MI_D_5,    MI_B_4,   MI_A_4,    MI_F_4,     MI_D_4,   MI_B_3,    MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,  MI_Gs_3,  _______, _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
                 {   _______,                MI_G_5, MI_E_5,  MI_D_5,   MI_As_4,     MI_G_4,   MI_E_4,   MI_D_4,     MI_As_3,    MI_G_3,    MI_E_3,    MI_Cs_4,    _______, _______, _______, MI2_OCTU},//
                 {   _______,    _______,       _______,       _______,    _______,   _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_3},//MI_TRNSU},//KC_PGDN },
                 {   _______,    _______,      _______,     _______,      _______,    _______, _______,    _______,    _______,    _______,   _______,     _______,    _______,    _______,      _______,      MI_TRNS_0},//MI2_OCTD  },
                 {   _______, _______,      _______,  _______,   _______,    _______, MOB,    _______,    _______,     _______,   MOB,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
               },
           [_L1] = { {   _______, _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,     _______,     _______,    _______, _______, _______ ,    _______,         _______},//MI_OCTU},//MI_OCTD},//KC_DEL  },
                 {   _______,             _______,    _______,   _______,    _______,     _______,   _______,    _______,     _______,     _______,     _______,  _______, _______,    _______, _______,      _______},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
                 {   _______,                 _______, _______,  _______,   _______,     _______,  _______,   _______,    _______,    _______,    _______,    _______,    _______, _______, _______, _______},//
                 {   _______,    _______,       _______,                  MI2_As_1,   MI3_D_2, MI2_D_1,    MI4_F_2,   MI2_A_1,    MI4_C_2,   _______, _______,    _______,    _______,      _______,     _______,      _______},//MI_TRNSU},//KC_PGDN },
                 {   _______,    _______,      _______,     _______,      MI2_C_1,   MI3_E_1, MI2_G_1,    MI3_B_1,   MI2_D_1,   MI3_Fs_1,   _______,     _______,    _______,    _______,      _______,      _______},//MI2_OCTD  },
                 {   _______, _______,      _______,  _______,   _______,    _______, _______,    _______,    _______,     _______,   _______,    _______,    _______,     _______,    _______,      _______},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
               },

     [_RB1]= { {   _______,     _______,   MI_G_5, MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_Cs_3,     _______, _______,    MI_TRNSD},//MI_OCTD  },
                 {   _______,        MI_C_6, MI_G_5,   MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_Fs_3,  _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
                 {   _______,               MI_C_6, MI_A_5,  MI_F_5,   MI_C_5,   MI_A_4,   MI_F_4,   MI_C_4,    MI_A_3,    MI_F_3,    MI_C_3,  MI_Eb_4,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
                 {   _______,    _______,       _______,    _______,    _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_5},//MI_TRNSU},//KC_PGDN },
                 {   _______,    _______,      _______,     _______,    _______,     _______, _______,    _______,    _______,    _______,   _______,     _______,    _______,    _______,      _______,      TRIKL_ALT_TGL},//MI2_OCTD  },
                 {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
              },
     [_LB1]= { {   _______,     _______,   _______,    _______,  _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,  _______,   _______, _______,    _______},//MI_OCTD  },
                 {   _______,        _______,   _______,    _______,  _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,  _______,  _______,    _______,    _______},//MI2_OCTD},//MI_OCTU  },
                 {   _______,               _______, _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,  _______,    _______, _______, _______, _______},//MI_TRNSD},//MI_TRNSD },
                 {   _______,    _______,       _______,                MI2_As_1,   MI3_D_2,   MI2_A_1,    MI3_Cs_2,   MI2_E_1,    MI3_Gs_1,  _______,  _______,    _______,    _______,      _______,     _______,      _______},//MI_TRNSU},//KC_PGDN },
                 {   _______,    _______,      _______,     _______,    MI2_F_1,     MI3_A_1,  MI2_C_1,    MI3_E_1,    MI2_G_1,    MI3_B_1,   _______,     _______,    _______,    _______,      _______,      _______},//MI2_OCTD  },
                 {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
              },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// /*FR34*/     [_R1] = { {   FUNC, _______,   MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,    MI_Fs_2, MI_E_2, _______ ,    _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                  {   _______,             MI_D_5,    MI_B_4,   MI_A_4,    MI_F_4,     MI_D_4,   MI_B_3,    MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,  MI_A_2, MI_F_2,    _______, _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                  {   _______,                 MI_Cs_5, MI_Bb_4,  MI_Gs_4,   MI_G_4,     MI_Cs_4,  MI_Bb_3,   MI_Gs_3,    MI_G_3,    MI_Cs_3,    MI_Bb_2,    MI_Gs_2,    _______, _______, _______, MI2_OCTU},//
//                  {   _______,    _______,       _______,       _______,    _______,   _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_3},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,      _______,    _______, _______,    _______,    _______,    _______,   _______,     _______,    _______,    _______,      _______,      MI_TRNS_0},//MI2_OCTD  },
//                  {   _______, _______,      _______,  _______,   _______,    _______, MOB,    _______,    _______,     _______,   MOB,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//                },
//            [_L1] = { {   _______, _______,   _______,   _______,   _______,    _______,    _______,    _______,    _______,     _______,     _______,    _______, _______, _______ ,    _______,         _______},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                  {   _______,             _______,    _______,   _______,    _______,     _______,   _______,    _______,     _______,     _______,     _______,  _______, _______,    _______, _______,      _______},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                  {   _______,                 _______, _______,  _______,   _______,     _______,  _______,   _______,    _______,    _______,    _______,    _______,    _______, _______, _______, _______},//
//                  {   _______,    _______,       _______,       _______,    MI2_F_1,   MI3_A_1, MI2_A_1,    MI3_Cs_2,   MI2_Bb_1,    MI3_D_2,    _______,    _______,    _______,      _______,     _______,      _______},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,      MI2_G_1,    MI3_B_1, MI2_D_1,    MI3_Fs_1,    MI2_B_1,    MI3_Ds_2,   _______,     _______,    _______,    _______,      _______,      _______},//MI2_OCTD  },
//                  {   _______, _______,      _______,  _______,   _______,    _______, _______,    _______,    _______,     _______,   _______,    _______,    _______,     _______,    _______,      _______},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//                },

//      [_RB1]= { {   _______,     _______,   MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,  MI_B_2,   _______, _______,    MI_TRNSD},//MI_OCTD  },
//                  {   _______,        MI_G_5,   MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_E_2,  MI_C_2,  _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                  {   _______,               MI_Gs_5, MI_Eb_5,  MI_Bb_4,   MI_Gs_4,   MI_Eb_4,   MI_Bb_3,   MI_Gs_3,    MI_Eb_3,    MI_Bb_2,    MI_Gs_2,  MI_Eb_2,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
//                  {   _______,    _______,       _______,    _______,    _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_5},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,    _______,     _______, _______,    _______,    _______,    _______,   _______,     _______,    _______,    _______,      _______,      TRIKL_ALT_TGL},//MI2_OCTD  },
//                  {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//               },
//      [_LB1]= { {   _______,     _______,   _______,    _______,  _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,  _______,   _______, _______,    _______},//MI_OCTD  },
//                  {   _______,        _______,   _______,    _______,  _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,  _______,  _______,    _______,    _______},//MI2_OCTD},//MI_OCTU  },
//                  {   _______,               _______, _______,  _______,   _______,   _______,   _______,   _______,    _______,    _______,    _______,  _______,    _______, _______, _______, _______},//MI_TRNSD},//MI_TRNSD },
//                  {   _______,    _______,       _______,    _______,    MI2_F_1,    MI3_A_1, MI2_E_1,    MI3_Gs_1,   MI2_Eb_1,    MI3_G_1,    _______,    _______,    _______,      _______,     _______,      _______},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,    MI2_C_1,     MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_Ab_1,    MI3_C_2,   _______,     _______,    _______,    _______,      _______,      _______},//MI2_OCTD  },
//                  {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//               },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// /*FR33*/     [_MIDI] = { {   FUNC, _______,   MI_C_5, MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,    MI_Fs_2, MI_D_2,    _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                  {   _______,        _______,  MI_D_5,  MI_B_4,   MI_A_4,    MI_F_4,     MI_D_4,   MI_B_3,    MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_F_2,   _______, _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                  {   _______,           _______, MI_Cs_5,  MI_Bb_4,  MI_Gs_4,   MI_G_4,     MI_Cs_4,  MI_Bb_3,   MI_Gs_3,    MI_G_3,    MI_Cs_3,    MI_As_2,     _______, _______, _______, MI2_OCTU},//
//                  /*{   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_Ds_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_Ds_3,    MI_Cs_3,    MI_As_2,    _______, _______, _______, MI2_OCTU},//*/
//                  /*{   _______,               MI_E_5,   MI_D_5, MI_As_4,   MI_G_4,   MI_E_4,   MI_D_4,    MI_As_3,    MI_G_3,    MI_E_3,  MI_D_3,  MI_Ds_3,    MI_Gs_2, _______, _______, MI_BENDD},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },*/
//                  {   _______,    _______,       _______,       _______,    MI2_F_1,   MI3_A_1, MI2_A_1,    MI4_C_2,   MI2_Bb_1,    MI3_D_2,    _______,    _______,    _______,      _______,     _______,      MI_VELU},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,      MI2_G_1,    MI3_B_1, MI2_D_1,    MI3_Fs_1,    MI2_B_1,    MI4_D_2,   _______,     _______,    _______,    _______,      _______,      MI_TRNS_0},//MI2_OCTD  },
//                  {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//                },

//      [_BELOW]= { {   FUNC,     _______,   MI_G_5,    MI_D_5,    MI_B_4,    MI_G_4,   MI_D_4,    MI_B_3,   MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,   MI_D_2,  MI_B_1,    _______,    MI_TRNSD},//MI_OCTD  },
//                  {   _______,        _______,  MI_G_5,   MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,  MI_E_2,  _______,      _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                  {   _______,        _______,     MI_Gs_5, MI_Eb_5,  MI_Bb_4,   MI_Gs_4,   MI_Eb_4,   MI_Bb_3,   MI_Gs_3,    MI_Eb_3,    MI_Bb_2,    MI_Gs_2,  _______,     _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
//                  /*{   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_Ds_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_Ds_3,    MI_Cs_3,    MI_As_2,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },*/
//                  {   _______,    _______,       _______,    _______,    MI2_F_1,    MI3_A_1, MI2_E_1,    MI3_Gs_1,   MI2_Eb_1,    MI3_G_1,    _______,    _______,    _______,      _______,     _______,      MI_VELD},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,    MI2_C_1,     MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_Ab_1,    MI3_C_2,   _______,     _______,    _______,    _______,      _______,      TRIKL_ALT_TGL},//MI2_OCTD  },
//                  {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//                },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// /*FR33*/     [_MIDI] = { {   FUNC, _______,   MI_C_5, MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,    MI_Fs_2, MI_Cs_2,    _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                  {   _______,        _______,  MI_D_5,  MI_B_4,   MI_A_4,    MI_F_4,     MI_D_4,   MI_B_3,    MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_Bb_2,   _______, _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                  {   _______,           _______, MI_Cs_5,  MI_Bb_4,  MI_Gs_4,   MI_G_4,     MI_Cs_4,  MI_Bb_3,   MI_Gs_3,    MI_G_3,    MI_Cs_3,    MI_Bb_2,     _______, _______, _______, MI2_OCTU},//
//                  /*{   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_Ds_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_Ds_3,    MI_Cs_3,    MI_As_2,    _______, _______, _______, MI2_OCTU},//*/
//                  /*{   _______,               MI_E_5,   MI_D_5, MI_As_4,   MI_G_4,   MI_E_4,   MI_D_4,    MI_As_3,    MI_G_3,    MI_E_3,  MI_D_3,  MI_Ds_3,    MI_Gs_2, _______, _______, MI_BENDD},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },*/
//                  {   _______,    _______,       _______,       _______,    MI2_F_1,   MI3_A_1, MI2_A_1,    MI4_C_2,   MI2_Bb_1,    MI3_D_2,    _______,    _______,    _______,      _______,     _______,      MI_VELU},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,      MI2_G_1,    MI3_B_1, MI2_D_1,    MI3_Fs_1,    MI2_B_1,    MI4_D_2,   _______,     _______,    _______,    _______,      _______,      MI_TRNS_0},//MI2_OCTD  },
//                  {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//                },

//      [_BELOW]= { {   FUNC,     _______,   MI_G_5,    MI_D_5,    MI_B_4,    MI_G_4,   MI_D_4,    MI_B_3,   MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,   MI_D_2,  MI_Eb_2,    _______,    MI_TRNSD},//MI_OCTD  },
//                  {   _______,        _______,  MI_G_5,   MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,  MI_Gs_2,  _______,      _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                  {   _______,        _______,     MI_Gs_5, MI_Eb_5,  MI_Bb_4,   MI_Gs_4,   MI_Eb_4,   MI_Bb_3,   MI_Gs_3,    MI_Eb_3,    MI_Bb_2,    MI_Gs_2,  _______,     _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
//                  /*{   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_Ds_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_Ds_3,    MI_Cs_3,    MI_As_2,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },*/
//                  {   _______,    _______,       _______,    _______,    MI2_F_1,    MI3_A_1, MI2_E_1,    MI3_Gs_1,   MI2_Eb_1,    MI3_G_1,    _______,    _______,    _______,      _______,     _______,      MI_VELD},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,    MI2_C_1,     MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_Ab_1,    MI3_C_2,   _______,     _______,    _______,    _______,      _______,      TRIKL_ALT_TGL},//MI2_OCTD  },
//                  {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//                },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// /*HELI*/     [_MIDI] = { {   FUNC,_______, MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,    MI_Fs_2, MI_D_2, MI_Ds_3 ,    _______,   MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                  {   _______,  _______,  MI_B_4,   MI_A_4,    MI_F_4,     MI_D_4,   MI_B_3,    MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,  MI_G_2, MI_Gs_3,        _______, _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                  {   _______,      _______,   MI_D_5,      MI_As_4,    MI_G_4,   MI_E_4,     MI_C_4,    MI_As_3,    MI_G_3,    MI_E_3,    MI_C_3,    MI_Cs_4,       _______, _______, _______, MI_VELU},//
//                  {   _______,    _______,       _______,       _______,    MI2_As_1,   MI3_D_2, MI2_D_1,    MI4_F_2,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,      MI2_C_1,   MI3_E_1, MI2_G_1,    MI3_B_1,   MI2_D_1,   MI3_Fs_1,   MI2_Fs_1,     _______,    _______,    _______,      _______,      TRIKL_ALT_TGL},//MI2_OCTD  },
//                  {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//                },

//      [_BELOW]= { {   FUNC,_______,   MI_D_5,   MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,  MI_B_1,   MI_Cs_3,    _______,    MI_TRNSD},//MI_OCTD  },
//                  {   _______,   _______,  MI_E_5,   MI_C_5,  MI_G_4,    MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,    MI_E_2,  MI_Fs_3,            _______, _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                  {   _______,   _______,       MI_F_5,    MI_C_5,    MI_A_4,   MI_F_4,   MI_C_4,   MI_A_3,    MI_F_3,     MI_C_3,     MI_A_2,    MI_Ds_4,     _______, _______, _______, MI_VELD},//MI_TRNSD},//MI_TRNSD },
//                  {   _______,    _______,       _______,    _______,         MI2_D_1,    MI3_Fs_1, MI2_A_1,    MI3_Cs_2,   MI2_E_1,    MI3_Gs_1,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_5},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,    MI2_F_1,     MI3_A_1,  MI2_C_1,    MI3_E_1,    MI2_G_1,    MI3_B_1,   MI2_D_2,     _______,    _______,    _______,      _______,      TO(_MIDIALT)},//MI2_OCTD  },
//                  {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//               },
// /*HELIALT*/     [_MIDIALT] = { {   FUNC, _______,MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,    MI_Fs_2, MI_D_2, MI_Ds_3 ,       _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                  {   _______,  _______,  MI_B_4,   MI_A_4,    MI_F_4,     MI_D_4,   MI_B_3,    MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,  MI_G_2, MI_Gs_3,        _______, _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                  {   _______,      _______,   MI_D_5,      MI_As_4,    MI_G_4,   MI_E_4,     MI_D_4,    MI_As_3,    MI_G_3,    MI_E_3,    MI_C_3,    MI_Cs_4,       _______, _______, _______, MI_VELU},//
//                  {   _______,    _______,       _______,       _______,    MI2_As_1,   MI3_D_2, MI2_D_1,    MI4_F_2,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,      MI2_C_1,   MI3_E_1, MI2_G_1,    MI3_B_1,   MI2_D_1,   MI3_Fs_1,   MI2_Fs_1,     _______,    _______,    _______,      _______,      TRIKL_ALT_TGL},//MI2_OCTD  },
//                  {   _______, _______,      _______,  _______,   _______,    _______, MOBELOWALT,    _______,    _______,     _______,   MOBELOWALT,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//                },

//      [_BELOWALT]= { {   FUNC,_______,  MI_D_5,   MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,  MI_B_1,   MI_Cs_3,    _______,    MI_TRNSD},//MI_OCTD  },
//                  {   _______,   _______,  MI_E_5,   MI_C_5,  MI_G_4,    MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,    MI_E_2,  MI_Fs_3,            _______, _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                  {   _______,   _______,       MI_F_5,    MI_C_5,    MI_A_4,   MI_F_4,   MI_C_4,   MI_A_3,    MI_F_3,     MI_C_3,     MI_A_2,    MI_Ds_4,     _______, _______, _______, MI_VELD},//MI_TRNSD},//MI_TRNSD },
//                  {   _______,    _______,       _______,    _______,         MI2_As_1,   MI3_D_2, MI2_A_1,    MI3_Cs_2,   MI2_E_1,    MI3_Gs_1,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_5},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,    MI2_F_1,     MI3_A_1,  MI2_C_1,    MI3_E_1,    MI2_G_1,    MI3_B_1,   MI2_D_2,     _______,    _______,    _______,      _______,      TO(_MIDI)},//MI2_OCTD  },
//                  {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//                },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// /*PANTHER34*/     [_MIDI] = { {   FUNC, _______,  MI_C_5,   MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,    MI_Ds_3,  _______ ,    _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                  {   _______,            MI_F_5,  MI_D_5,    MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,    MI_Gs_3, _______,    _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                  {   _______,                  MI_G_5, MI_E_5, MI_D_5,   MI_As_4,   MI_G_4,   MI_E_4,   MI_D_4,    MI_As_3,    MI_G_3,    MI_E_3,    MI_Cs_4,    _______, _______, _______, MI2_OCTU},//
//                   {   _______,    _______,       _______,       _______,   MI2_As_1,   MI3_D_2, MI2_D_1,    MI4_F_2,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
//                   {   _______,    _______,      _______,     _______,      MI2_C_1,   MI3_E_1, MI2_G_1,    MI3_B_1,   MI2_D_1,   MI3_Fs_1,   _______,     _______,    _______,    _______,      _______,      TRIKL_ALT_TGL},//MI2_OCTD  },
//                  {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//                },

//      [_BELOW]= { {   FUNC,     _______,  MI_G_5, MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_Cs_3,     _______, _______,    MI_TRNSD},//MI_OCTD  },
//                  {   _______,       MI_C_6, MI_G_5,   MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_Fs_3,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                  {   _______,               MI_C_6, MI_A_5,  MI_F_5,   MI_C_5,   MI_A_4,   MI_F_4,   MI_C_4,    MI_A_3,    MI_F_3,    MI_C_3,      MI_Ds_4,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
//                   {   _______,    _______,       _______,    _______,    MI2_As_1,   MI3_D_2,   MI2_A_1,    MI3_Cs_2,   MI2_E_1,    MI3_Gs_1,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_0},//MI_TRNSU},//KC_PGDN },
//                   {   _______,    _______,      _______,     _______,    MI2_F_1,     MI3_A_1,  MI2_C_1,    MI3_E_1,    MI2_G_1,    MI3_B_1,   _______,     _______,    _______,    _______,      _______,      TRIKL_ALT_TGL},//MI2_OCTD  },
//                  {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//               },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// /*MEX34*/     [_MIDI] = { {   FUNC, _______,  MI_C_5,   MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,    MI_Ds_3,  _______ ,    _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                  {   _______,            MI_F_5,  MI_D_5,    MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,    MI_Gs_3, _______,    _______,      MI_OCTU},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                  {   _______,                  MI_G_5, MI_E_5, MI_D_5,   MI_As_4,   MI_G_4,   MI_E_4,   MI_D_4,    MI_As_3,    MI_G_3,    MI_E_3,    MI_Cs_4,    _______, _______, _______, MI2_OCTU},//
//                  {   _______,    _______,       _______,       _______,    MI2_Cs_1,    MI3_F_1, MI2_D_1,    MI4_F_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_3},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,      MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_D_1,    MI3_Fs_1,   _______,     _______,    _______,    _______,      _______,      MI_TRNS_0},//MI2_OCTD  },
//                  {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//                },

//      [_BELOW]= { {   FUNC,     _______,  MI_G_5, MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_Cs_3,     _______, _______,    MI_TRNSD},//MI_OCTD  },
//                  {   _______,       MI_C_6, MI_G_5,   MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_Fs_3,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                  {   _______,               MI_C_6, MI_A_5,  MI_F_5,   MI_C_5,   MI_A_4,   MI_F_4,   MI_C_4,    MI_A_3,    MI_F_3,    MI_C_3,      MI_Ds_4,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
//                  {   _______,    _______,       _______,    _______,    MI2_Cs_1,    MI3_F_1, MI2_A_1,    MI3_Cs_1,   MI2_E_1,    MI3_Gs_1,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_5},//MI_TRNSU},//KC_PGDN },
//                  {   _______,    _______,      _______,     _______,    MI2_F_1,     MI3_A_1, MI2_C_1,    MI3_E_1,    MI2_G_1,    MI3_B_1,   _______,     _______,    _______,    _______,      _______,      TRIKL_DRUM_TGL},//MI2_OCTD  },
//                  {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//                },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//     [_MIDI] = { {   FUNC, _______,      MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_Ds_3,    MI_Gs_2,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                 {   _______,     _______,      MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_Gs_3,    MI_Ds_4, _______,    _______,      MI_OCTD},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                 {   _______,               _______,  MI_D_5,   MI_As_4,   MI_G_4,   MI_E_4,   MI_D_4,    MI_As_3,    MI_G_3,    MI_E_3,  MI_D_3,  MI_Cs_4,    _______, _______, _______, MI2_OCTU},//
//                 /*{   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_Ds_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_Ds_3,    MI_Cs_3,    MI_As_2,    _______, _______, _______, MI2_OCTU},//*/
//                 /*{   _______,               MI_E_5,   MI_D_5, MI_As_4,   MI_G_4,   MI_E_4,   MI_D_4,    MI_As_3,    MI_G_3,    MI_E_3,  MI_D_3,  MI_Ds_3,    MI_Gs_2, _______, _______, MI_BENDD},//MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },*/
//                 {   _______,    _______,       _______,       _______,    MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_3},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,      _______,     _______,    MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,     _______,    _______,    _______,      _______,      MI_TRNS_0},//MI2_OCTD  },
//                 {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//               },

//     [_BELOW]= { {   FUNC,     _______,  MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_Cs_3,    MI_As_2,    _______,    MI_TRNSD},//MI_OCTD  },
//                 {   _______,     _______,      MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_As_3,    MI_Cs_4,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                 {   _______,               _______,  MI_F_5,   MI_C_5,   MI_A_4,   MI_F_4,   MI_C_4,    MI_A_3,    MI_F_3,    MI_C_3,    MI_A_2,    MI_Ds_4,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
//                 /*{   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_Ds_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_Ds_3,    MI_Cs_3,    MI_As_2,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },*/
//                 {   _______,    _______,       _______,       _______,    MI2_F_1,    MI3_A_1, MI2_D_1,    MI3_Fs_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,      _______,     _______,      TO(_MIDIALT)},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,      _______,     _______,    MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_E_1,    MI3_Gs_1,   _______,     _______,    _______,    _______,      _______,      TRIKL_DRUM_TGL},//MI2_OCTD  },
//                 {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//               },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//   [_MIDIALT] = { {   FUNC, _______,      MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_Ds_3,    MI_Gs_2,     _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                 {   _______,     _______,      MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_Gs_3,    MI_Ds_4, _______,    _______,      MI_OCTD},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                 {   _______,               _______,  MI_D_5,   MI_As_4,   MI_G_4,   MI_E_4,   MI_D_4,    MI_As_3,    MI_G_3,    MI_E_3,  MI_D_3,  MI_Cs_4,    _______, _______, _______, MI2_OCTU},//
//                  {   _______,    _______,       _______,       _______,    MI2_Bb_1,    MI3_D_1, MI2_D_1,    MI4_F_1,   MI2_A_1,    MI4_C_2,    _______,    _______,    _______,      _______,     _______,      MI_TRNS_3},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,      _______,     _______,    MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_D_1,    MI3_Fs_1,   _______,     _______,    _______,    _______,      _______,      MI_TRNS_0},//MI2_OCTD  },
//                 {   _______,       _______,    _______,    _______,   _______,      _______,   MOBELOWALT, _______, _______,    _______,    MOBELOWALT, _______,    _______,    _______,    _______,      TRIKL_REG_TGL},//MI_TRNS_N2},//MI2_OCTU }
//               },
//    [_BELOWALT]= { {   FUNC,     _______,  MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_Cs_3,    MI_As_2,    _______,    MI_TRNSD},//MI_OCTD  },
//                 {   _______,     _______,      MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_As_3,    MI_Cs_4,    _______,    _______,    MI_OCTD},//MI2_OCTD},//MI_OCTU  },
//                 {   _______,               _______,  MI_F_5,   MI_C_5,   MI_A_4,   MI_F_4,   MI_C_4,    MI_A_3,    MI_F_3,    MI_C_3,    MI_A_2,    MI_Ds_4,    _______, _______, _______, MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
//                 {   _______,    _______,    _______,    _______, MI2_Bb_1,    MI3_D_1, MI2_A_1,    MI3_Cs_1,   MI2_E_1,    MI3_Gs_1,    _______,    _______,    _______,    _______,    _______,    TO(_MIDIALT)},//MI_TRNS_6},//MI_TRNSU },
//                 {   _______,    _______,    _______,    _______, MI2_F_1,    MI3_A_1, MI2_C_1,    MI3_E_1,    MI2_G_1,    MI3_B_1,   _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//MI2_OCTD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//MI2_OCTU }
//               },//------------
/*ANDREW AUS GCalt JMC / DG / BCCs*/   
// [_R1] = { {   FUNC, _______,   MI_C_5,   MI_A_4,   MI_Fs_4,   MI_E_4,    MI_C_4,    MI_A_3,    MI_Fs_3,    MI_E_3,     MI_C_3,     MI_A_2,     MI_Fs_2,    MI_E_2,         _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                 {   _______,     _______,   MI_D_5,   MI_B_4,     MI_A_4,  MI_F_4,     MI_D_4,  MI_B_3,     MI_A_3,     MI_F_3,     MI_D_3,     MI_B_2,     MI_G_2,     _______,    _______,      MI_TRNS_0},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                 {   _______,               _______,  MI_Cs_5,   MI_As_4,   MI_Gs_4,   MI_G_4,   MI_Cs_4,    MI_As_3,    MI_Gs_3,    MI_G_3,  MI_Cs_3,  MI_As_2,    _______, _______, _______, NEXTR},//
//                 {   _______,    _______,       _______,       _______,    _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,      _______,     _______,      _______},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,      _______,     _______,    _______,    _______, _______,    _______,    _______,    _______,   _______,     _______,    _______,    _______,      _______,      MI_VELU},//MI2_OCTD  },
//                 {   _______, _______,      _______,  _______,   _______,    _______, MOB,    _______,    _______,     _______,   MOB,    _______,    _______,     _______,    _______,      TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
//               },

//     [_RB1]= { {   FUNC,     _______,  MI_G_5, MI_D_5,    MI_B_4,  MI_G_4,     MI_D_4,  MI_B_3,     MI_G_3,    MI_D_3,     MI_B_2,     MI_G_2,     MI_D_2,     MI_B_1,        _______,    MI_TRNSD},//MI_OCTD  },
//                 {   _______,     _______,    MI_G_5,  MI_E_5,    MI_C_5,  MI_G_4,     MI_E_4,  MI_C_4,     MI_G_3,    MI_E_3,     MI_C_3,     MI_G_2,     MI_E_2,        _______,    _______,    MI_TRNS_0},//MI2_OCTD},//MI_OCTU  },
//                 {   _______,               _______,  MI_Gs_5,  MI_Ds_5,  MI_As_4,  MI_Gs_4,  MI_Ds_4,   MI_As_3,   MI_Gs_3,   MI_Ds_3,   MI_As_2,   MI_Gs_2,    _______, _______, _______, NEXTR},//MI_TRNSD},//MI_TRNSD },
//                 {   _______,    _______,       _______,       _______,    _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,      _______,     _______,      _______},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,      _______,     _______,    _______,    _______, _______,    _______,    _______,    _______,   _______,     _______,    _______,    _______,      _______,      MI_VELD},//MI2_OCTD  },
//                 {   _______,     _______,      _______,     _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//PANIKO},//TRIKL_DRUM_TGL},//TRIKL_DRUM_TGL},//TO(_MIDIALT)},//MI2_OCTU }
//               },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//   [_R2] = { {   FUNC, _______,      MI_E_5,   MI_Cs_5,   MI_B_4,    MI_G_4,    MI_E_4,    MI_Cs_4,    MI_B_3,     MI_G_3,     MI_E_3,     MI_Cs_3,    MI_A_2,     MI_B_2,      _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                 {   _______,     _______,      MI_Fs_5,   MI_E_5,    MI_C_5,    MI_A_4,    MI_Fs_4,    MI_E_4,     MI_C_4,     MI_A_3,     MI_Fs_3,     MI_C_3,    MI_Gs_2, _______,    _______,      MI_TRNS_0},//MI_OCTU},//MI_OCTD},//KC_HOME },
//                 {   _______,               _______,  MI_F_5,   MI_D_5,    MI_As_4,  MI_Gs_4,  MI_F_4,    MI_D_4,     MI_As_3,   MI_Gs_3,  MI_F_3,  MI_D_3,     _______, _______, _______, NEXTR},//
//                  {   _______,    _______,       _______,       _______,    _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,      _______,     _______,      _______},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,      _______,     _______,    _______,    _______, _______,    _______,    _______,    _______,   _______,     _______,    _______,    _______,      _______,      MI_VELU},//MI2_OCTD  },
//                 {   _______,       _______,    _______,    _______,   _______,      _______,   MOB, _______, _______,    _______,    MOB, _______,    _______,    _______,    _______,      TRIKL_REG_TGL},//MI_TRNS_N2},//MI2_OCTU }
//               },
//    [_RB2]= { {   FUNC,     _______,  MI_A_5,    MI_Fs_5, MI_D_5,     MI_A_4,  MI_Fs_4,    MI_D_4,    MI_A_3,     MI_Fs_3,    MI_D_3,     MI_A_2,     MI_G_2,     MI_Fs_2,    _______,    MI_TRNSD},//MI_OCTD  },
//                 {   _______,     _______,     MI_B_5,  MI_G_5,     MI_D_5,  MI_B_4,     MI_G_4,    MI_D_4,     MI_B_3,     MI_G_3,     MI_D_3,     MI_B_2,    MI_F_2,    _______,    _______,    MI_TRNS_0},//MI2_OCTD},//MI_OCTU  },
//                 {   _______,               _______,  MI_As_5,  MI_F_5,   MI_Ds_5,  MI_As_4,  MI_F_4,    MI_Ds_4,   MI_As_3,   MI_F_3,    MI_Ds_3,   MI_As_2,    _______, _______, _______, NEXTR},//MI_TRNSD},//MI_TRNSD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//MI_TRNS_6},//MI_TRNSU },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,    _______,    _______,    _______,   _______,    _______,    _______,    _______,    _______,    MI_VELD},//MI2_OCTD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//MI2_OCTU }
//               },//------------
//   [_R3] = { {   FUNC, _______,          MI_E_5, MI_Cs_5,  MI_As_4,   MI_Gs_4,   MI_E_4,    MI_Cs_4,   MI_As_3,    MI_Gs_3,    MI_E_3,     MI_Cs_3,    MI_As_2,    MI_Gs_2,         _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                 {   _______,     _______,     MI_D_5,    MI_B_4,    MI_A_4,    MI_F_4,     MI_D_4,     MI_B_3,     MI_A_3,     MI_F_3,      MI_D_3,    MI_B_2, MI_A_2, _______,    _______,      MI_TRNS_0},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                 {   _______,               _______,  MI_C_5,   MI_As_4,  MI_Fs_4,  MI_Ds_4,  MI_C_4,    MI_As_3,   MI_Fs_3,   MI_Ds_3,   MI_C_3,    MI_As_2,    _______, _______, _______, NEXTR},//
//                  {   _______,    _______,       _______,       _______,    _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,      _______,     _______,      _______},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,      _______,     _______,    _______,    _______, _______,    _______,    _______,    _______,   _______,     _______,    _______,    _______,      _______,      MI_VELU},//MI2_OCTD  },
//                 {   _______,       _______,    _______,    _______,   _______,      _______,   MOB, _______, _______,    _______,    MOB, _______,    _______,    _______,    _______,      TRIKL_REG_TGL},//MI_TRNS_N2},//MI2_OCTU }
//               },
//    [_RB3]= { {   FUNC,     _______,     MI_B_5, MI_Fs_5,   MI_Ds_5, MI_B_4,     MI_Fs_4, MI_Ds_4,    MI_B_3,    MI_Fs_3,    MI_Ds_3,    MI_B_2,     MI_Fs_2,    MI_Ds_2,        _______,    MI_TRNSD},//MI_OCTD  },
//                 {   _______,     _______,     MI_G_5,  MI_E_5,     MI_C_5,  MI_G_4,     MI_E_4,    MI_C_4,     MI_G_3,     MI_E_3,     MI_C_3,     MI_G_2,    MI_E_2,    _______,    _______,    MI_TRNS_0},//MI2_OCTD},//MI_OCTU  },
//                 {   _______,               _______,  MI_F_5,   MI_Cs_5,  MI_Gs_4,  MI_F_4,   MI_Cs_4,   MI_Gs_3,   MI_F_3,    MI_Cs_3,   MI_Gs_2,   MI_F_2,     _______, _______, _______, NEXTR},//MI_TRNSD},//MI_TRNSD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//MI_TRNS_6},//MI_TRNSU },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,    _______,    _______,    _______,   _______,    _______,    _______,    _______,    _______,    MI_VELD},//MI2_OCTD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//MI2_OCTU }
//               },//------------
//   [_R4] = { {   FUNC, _______,          MI_E_5, MI_Cs_5,  MI_As_4,   MI_Gs_4,   MI_E_4,    MI_Cs_4,   MI_As_3,    MI_Gs_3,    MI_E_3,     MI_Cs_3,    MI_As_2,    MI_Gs_2,         _______,         MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                 {   _______,     _______,     MI_D_5,    MI_B_4,    MI_A_4,    MI_F_4,     MI_D_4,     MI_B_3,     MI_A_3,     MI_F_3,      MI_D_3,    MI_B_2, MI_A_2, _______,    _______,      MI_TRNS_0},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                 {   _______,               _______,  MI_C_5,   MI_As_4,  MI_Fs_4,  MI_Ds_4,  MI_C_4,    MI_As_3,   MI_Fs_3,   MI_Ds_3,   MI_C_3,    MI_As_2,    _______, _______, _______, NEXTR},//
//                  {   _______,    _______,       _______,       _______,    _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,      _______,     _______,      _______},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,      _______,     _______,    _______,    _______, _______,    _______,    _______,    _______,   _______,     _______,    _______,    _______,      _______,      MI_VELU},//MI2_OCTD  },
//                 {   _______,       _______,    _______,    _______,   _______,      _______,   MOB, _______, _______,    _______,    MOB, _______,    _______,    _______,    _______,      TRIKL_REG_TGL},//MI_TRNS_N2},//MI2_OCTU }
//               },
//    [_RB4]= { {   FUNC,     _______,     MI_B_5, MI_Fs_5,   MI_Ds_5, MI_B_4,     MI_Fs_4, MI_Ds_4,    MI_B_3,    MI_Fs_3,    MI_Ds_3,    MI_B_2,     MI_Fs_2,    MI_Ds_2,        _______,    MI_TRNSD},//MI_OCTD  },
//                 {   _______,     _______,     MI_G_5,  MI_E_5,     MI_C_5,  MI_G_4,     MI_E_4,    MI_C_4,     MI_G_3,     MI_E_3,     MI_C_3,     MI_G_2,    MI_E_2,    _______,    _______,    MI_TRNS_0},//MI2_OCTD},//MI_OCTU  },
//                 {   _______,               _______,  MI_F_5,   MI_Cs_5,  MI_Gs_4,  MI_F_4,   MI_Cs_4,   MI_Gs_3,   MI_F_3,    MI_Cs_3,   MI_Gs_2,   MI_F_2,     _______, _______, _______, NEXTR},//MI_TRNSD},//MI_TRNSD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//MI_TRNS_6},//MI_TRNSU },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,    _______,    _______,    _______,   _______,    _______,    _______,    _______,    _______,    MI_VELD},//MI2_OCTD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_ALT_TGL},//MI2_OCTU }
//               },//------------
//   [_L1] = { {   FUNC, _______,          _______, _______,  _______,   _______,   _______,    _______,   _______,    _______,    _______,     _______,    _______,    _______,         _______,         _______},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                 {   _______,     _______,     _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,      _______,    _______, _______, _______,    _______,      _______},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                 {   _______,               _______,  _______,   _______,  _______,  _______,  _______,    _______,   _______,   _______,   _______,    _______,    _______, _______, _______, _______},//
//                 {   _______,    _______,    MI2_D_1,    MI4_F_1, MI2_F_1,   MI3_A_1, MI2_A_1,    MI4_C_2,   MI2_Bb_1,    MI3_D_2,    MI2_Fs_1,    MI4_A_1,    _______,    _______,    _______, NEXTL},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,    MI2_C_1,    MI3_E_1, MI2_G_1,    MI3_B_1, MI2_D_1,    MI3_Fs_1,    MI2_B_1,    MI4_D_2,   MI2_Cs_1,    MI4_E_1,    _______,    _______,    _______,  _______},//MI2_OCTD },
//                 {   _______,       _______,    _______,    _______,   _______,      _______,   _______, _______, _______,    _______,    _______, _______,    _______,    _______,    _______,      _______},//MI_TRNS_N2},//MI2_OCTU }
//               },
//    [_LB1]= { {   FUNC,     _______,     _______, _______,   _______, _______,     _______, _______,    _______,    _______,    _______,    _______,     _______,    _______,        _______,    _______},//MI_OCTD  },
//                 {   _______,     _______,     _______,  _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,     _______,    _______,    _______,    _______,    _______},//MI2_OCTD},//MI_OCTU  },
//                 {   _______,               _______,  _______,   _______,  _______,  _______,   _______,   _______,   _______,    _______,   _______,   _______,     _______, _______, _______, _______},//MI_TRNSD},//MI_TRNSD },
//                 {   _______,    _______,    MI2_E_1,    MI4_G_1, MI2_F_1,    MI3_A_1, MI2_E_1,    MI3_Gs_1,   MI2_Eb_1,    MI3_G_1,   MI2_A_1,    MI3_Cs_2,    _______,    _______,    _______, NEXTL},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,    MI2_D_1,    MI3_Fs_1, MI2_C_1,     MI3_E_1, MI2_G_1,    MI3_B_1,    MI2_Ab_1,    MI3_C_2,   MI2_B_1,    MI3_Ds_2,    _______,    _______,    _______,  _______},//MI2_OCTD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//MI2_OCTU }
//               },//------------
//   [_L2] = { {   FUNC, _______,          _______, _______,  _______,   _______,   _______,    _______,   _______,    _______,    _______,     _______,    _______,    _______,         _______,         _______},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                 {   _______,     _______,     _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,      _______,    _______, _______, _______,    _______,      _______},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                 {   _______,               _______,  _______,   _______,  _______,  _______,  _______,    _______,   _______,   _______,   _______,    _______,    _______, _______, _______, _______},//
//                 {   _______,    _______,    MI2_B_1,    MI4_D_2, MI2_C_1,   MI3_E_1, MI2_E_1,    MI4_G_1,   MI2_F_1,    MI3_A_1,    MI2_Cs_1,    MI4_E_1,    _______,    _______,    _______, NEXTL},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,    MI2_G_1,    MI3_B_1, MI2_D_1,    MI3_Fs_1, MI2_A_1,    MI3_Cs_2,    MI2_Fs_1,    MI4_A_1,   MI2_Gs_1,    MI4_B_1,    _______,    _______,    _______,  _______},//MI2_OCTD },
//                 {   _______,       _______,    _______,    _______,   _______,      _______,   _______, _______, _______,    _______,    _______, _______,    _______,    _______,    _______,      _______},//MI_TRNS_N2},//MI2_OCTU }
//               },
//    [_LB2]= { {   FUNC,     _______,     _______, _______,   _______, _______,     _______, _______,    _______,    _______,    _______,    _______,     _______,    _______,        _______,    _______},//MI_OCTD  },
//                 {   _______,     _______,     _______,  _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,     _______,    _______,    _______,    _______,    _______},//MI2_OCTD},//MI_OCTU  },
//                 {   _______,               _______,  _______,   _______,  _______,  _______,   _______,   _______,   _______,    _______,   _______,   _______,     _______, _______, _______, _______},//MI_TRNSD},//MI_TRNSD },
//                 {   _______,    _______,    MI2_F_1,    MI3_A_1, MI2_C_1,    MI3_E_1, MI2_B_1,    MI4_D_2,   MI2_Bb_1,    MI3_D_2,   MI2_E_1,    MI3_Gs_1,    _______,    _______,    _______, NEXTL},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,    MI2_A_1,    MI3_Cs_2, MI2_G_1,   MI3_B_1, MI2_D_1,    MI3_Fs_1,  MI2_Eb_1,    MI3_G_1,   MI2_Fs_1,    MI3_As_1,    _______,    _______,    _______,  _______},//MI2_OCTD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//MI2_OCTU }
//               },//------------
//   [_L3] = { {   FUNC, _______,          _______, _______,  _______,   _______,   _______,    _______,   _______,    _______,    _______,     _______,    _______,    _______,         _______,         _______},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                 {   _______,     _______,     _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,      _______,    _______, _______, _______,    _______,      _______},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                 {   _______,               _______,  _______,   _______,  _______,  _______,  _______,    _______,   _______,   _______,   _______,    _______,    _______, _______, _______, _______},//
//                 {   _______,    _______,    MI2_Bb_1,    MI2_F_1, MI2_C_1,   MI2_G_1, MI2_D_1,    MI2_A_1,   MI2_E_1,    MI2_B_1,    MI2_Fs_1,    MI2_Cs_1,    _______,    _______,    _______, NEXTL},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,    MI6_D_2,    MI6_A_2, MI6_E_2,    MI6_B_2, MI6_Fs_2,    MI6_Cs_3,    MI6_Gs_2,    MI6_Ds_3,   MI6_As_2,    MI6_F_3,    _______,    _______,    _______,  _______},//MI2_OCTD },
//                 {   _______,       _______,    _______,    _______,   _______,      _______,   _______, _______, _______,    _______,    _______, _______,    _______,    _______,    _______,      _______},//MI_TRNS_N2},//MI2_OCTU }
//               },
//    [_LB3]= { {   FUNC,     _______,     _______, _______,   _______, _______,     _______, _______,    _______,    _______,    _______,    _______,     _______,    _______,        _______,    _______},//MI_OCTD  },
//                 {   _______,     _______,     _______,  _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,     _______,    _______,    _______,    _______,    _______},//MI2_OCTD},//MI_OCTU  },
//                 {   _______,               _______,  _______,   _______,  _______,  _______,   _______,   _______,   _______,    _______,   _______,   _______,     _______, _______, _______, _______},//MI_TRNSD},//MI_TRNSD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,    _______,   _______,    _______,   _______,    _______,    _______,    _______,    _______, NEXTL},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,    _______,    _______, _______,   _______, _______,    _______,  _______,    _______,   _______,    _______,    _______,    _______,    _______,  _______},//MI2_OCTD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//MI2_OCTU }
//               },//------------
//   [_L4] = { {   FUNC, _______,          _______, _______,  _______,   _______,   _______,    _______,   _______,    _______,    _______,     _______,    _______,    _______,         _______,         _______},//MI_OCTU},//MI_OCTD},//KC_DEL  },
//                 {   _______,     _______,     _______,    _______,    _______,    _______,     _______,     _______,     _______,     _______,      _______,    _______, _______, _______,    _______,      _______},//MI_OCTU},//TRIKL_ALT},TGLMI2_OCTU},//MI_OCTU},//KC_HOME },
//                 {   _______,               _______,  _______,   _______,  _______,  _______,  _______,    _______,   _______,   _______,   _______,    _______,    _______, _______, _______, _______},//
//                 {   _______,    _______,    MI2_Bb_1,    MI2_F_1, MI2_C_1,   MI2_G_1, MI2_D_1,    MI2_A_1,   MI2_E_1,    MI2_B_1,    MI2_Fs_1,    MI2_Cs_1,    _______,    _______,    _______, NEXTL},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,    MI6_D_2,    MI6_A_2, MI6_E_2,    MI6_B_2, MI6_Fs_2,    MI6_Cs_3,    MI6_Gs_2,    MI6_Ds_3,   MI6_As_2,    MI6_F_3,    _______,    _______,    _______,  _______},//MI2_OCTD },
//                 {   _______,       _______,    _______,    _______,   _______,      _______,   _______, _______, _______,    _______,    _______, _______,    _______,    _______,    _______,      _______},//MI_TRNS_N2},//MI2_OCTU }
//               },
//    [_LB4]= { {   FUNC,     _______,     _______, _______,   _______, _______,     _______, _______,    _______,    _______,    _______,    _______,     _______,    _______,        _______,    _______},//MI_OCTD  },
//                 {   _______,     _______,     _______,  _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,     _______,    _______,    _______,    _______,    _______},//MI2_OCTD},//MI_OCTU  },
//                 {   _______,               _______,  _______,   _______,  _______,  _______,   _______,   _______,   _______,    _______,   _______,   _______,     _______, _______, _______, _______},//MI_TRNSD},//MI_TRNSD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,    _______,   _______,    _______,   _______,    _______,    _______,    _______,    _______, NEXTL},//MI_TRNSU},//KC_PGDN },
//                 {   _______,    _______,    _______,    _______, _______,   _______, _______,    _______,  _______,    _______,   _______,    _______,    _______,    _______,    _______,  _______},//MI2_OCTD },
//                 {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______},//MI2_OCTU }
//               },//------------
	
#elif defined(TRIKL_IX11CR_T)  
 /*SYSTEM B  */
    [_MIDI] = { {   FUNC, _______,    MI_F_5,  MI_D_5,     MI_B_4,   MI_Gs_4,   MI_F_4,     MI_D_4,     MI_B_3,    MI_Gs_3,   MI_F_3,    MI_D_3,    MI_B_2,    MI_Gs_2,   _______,               MI_TRNSU},//MI_OCTD  },
                {   _______, _______,    MI_Ds_5,     MI_C_5,   MI_A_4,  MI_Fs_4,     MI_Ds_4,    MI_C_4,   MI_A_3,    MI_Fs_3,    MI_Ds_3,    MI_C_3,   MI_A_2,   _______, _______,           MI_OCTU},//MI2_OCTD},//MI_OCTU  },
                {   _______,           MI_E_5,      MI_Cs_5,  MI_As_4,  MI_G_4,  MI_E_4,       MI_Cs_4,    MI_As_3,  MI_G_3,     MI_E_3,     MI_Cs_3,   MI_As_2,  MI_G_2,   _______, _______,  MI2_OCTU},//MI_TRNSD},//MI_TRNSD },
                {   _______,        MI2_Eb_1,   MI2_Bb_1,   MI2_F_1,    MI2_C_1,    MI2_G_1,   MI2_D_1,    MI2_A_1,     MI2_E_1,    MI2_B_1, MI2_Fs_1, MI2_Cs_1, _______, _______, _______, MI_BENDD},//TO(_MIDIDRUM)},//MI_TRNSU },
                {   _______, _______,    MI6_G_2,   MI6_D_3,     MI6_A_2,   MI6_E_2,    MI6_B_2,    MI6_Fs_2,    MI6_Cs_3,    MI6_Gs_2,    MI6_Ds_3, MI6_As_2, MI6_F_3, _______, _______,    MI_BENDU},//MI_BENDU,}//MI_TRNSD},//MI_TRNSD },
                {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,  TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
              },

    [_BELOW]= { {   FUNC, _______,    MI_F_5,  MI_D_5,     MI_B_4,   MI_Gs_4,   MI_F_4,     MI_D_4,     MI_B_3,    MI_Gs_3,   MI_F_3,    MI_D_3,    MI_B_2,    MI_Gs_2,   _______,               MI_TRNSD},//MI_OCTD  },
                {   _______, _______,    MI_Ds_5,     MI_C_5,   MI_A_4,  MI_Fs_4,     MI_Ds_4,    MI_C_4,   MI_A_3,    MI_Fs_3,    MI_Ds_3,    MI_C_3,   MI_A_2,   _______, _______,           MI_OCTD},//MI2_OCTD},//MI_OCTU  },
                {   _______,           MI_E_5,      MI_Cs_5,  MI_As_4,  MI_G_4,  MI_E_4,       MI_Cs_4,    MI_As_3,  MI_G_3,     MI_E_3,     MI_Cs_3,   MI_As_2,  MI_G_2, _______, _______,         MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
                {   _______,         MI2_Eb_1,   MI2_Bb_1,   MI2_F_1,    MI2_C_1,    MI2_G_1,   MI2_D_1,    MI2_A_1,     MI2_E_1,    MI2_B_1,  MI2_Fs_1, MI2_Cs_1,    _______, _______, _______, MI_BENDD},//TO(_MIDIDRUM)},//MI_TRNSU },
                {   _______, _______,    MI6_G_2,   MI6_D_2,     MI6_A_2,   MI6_E_2,    MI6_B_2,    MI6_Fs_2,    MI6_Cs_3,    MI6_Gs_2,    MI6_Ds_3, MI6_As_2, MI6_F_3, _______, _______,    MI_BENDU},//MI_TRNSD},//MI_TRNSD },
                {   _______, _______,      _______,  _______,   _______,    _______, _______,    _______,    _______,     _______,   _______,    _______,    _______,     _______,    _______,  TRIKL_ALT_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
              }, 
    // SYSTEM C
    // [_MIDI] = { {   FUNC,_______,    MI_Ds_5,     MI_C_5,   MI_A_4,  MI_Fs_4,     MI_Ds_4,    MI_C_4,   MI_A_3,    MI_Fs_3,    MI_Ds_3,    MI_C_3,   MI_A_2,  MI_Fs_2,   _______,               MI_TRNSU},//MI_OCTD  },
    //             {   _______, _______,    MI_Cs_5,  MI_As_4,   MI_G_4,  MI_E_4,        MI_Cs_4,    MI_As_3,     MI_G_3,     MI_E_3,     MI_Cs_3,   MI_As_2,  MI_G_2, _______, _______,           MI_OCTU},//MI2_OCTD},//MI_OCTU  },
    //             {   _______,         MI_D_5,     MI_B_4,   MI_Gs_4,   MI_F_4,     MI_D_4,     MI_B_3,    MI_Gs_3,   MI_F_3,    MI_D_3,    MI_B_2,    MI_Gs_2, MI_F_2, _______, _______,         MI2_OCTU},//MI_TRNSD},//MI_TRNSD },
    //             {   _______,      MI2_Db_1,  MI2_Ab_1,   MI2_Eb_1,   MI2_Bb_1,   MI2_F_1,    MI2_C_1,    MI2_G_1,   MI2_D_1,    MI2_A_1,     MI2_E_1,    MI2_B_1,    _______, _______, _______, MI_BENDD},//TO(_MIDIDRUM)},//MI_TRNSU },
    //             {   _______, _______,   MI3_F_2,  MI3_C_3,   MI3_G_2,   MI3_D_3,     MI3_A_2,   MI3_E_2,    MI3_B_2,    MI3_Fs_2,    MI3_Cs_3,    MI3_Gs_2,    MI3_Ds_3,   _______, _______,    PC_UP_1},//MI_BENDU,}//MI_TRNSD},//MI_TRNSD },
    //             {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______,  TRIKL_REG_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
    //           },

    // [_BELOW]= { {   FUNC,_______,   MI_Ds_5,     MI_C_5,   MI_A_4,  MI_Fs_4,     MI_Ds_4,    MI_C_4,   MI_A_3,    MI_Fs_3,    MI_Ds_3,    MI_C_3,   MI_A_2,  MI_Fs_2,   _______,                MI_TRNSD},//MI_OCTD  },
    //             {   _______, _______,    MI_Cs_5,  MI_As_4,   MI_G_4,  MI_E_4,        MI_Cs_4,    MI_As_3,     MI_G_3,     MI_E  _3,     MI_Cs_3,   MI_As_2,  MI_G_2, _______, _______,           MI_OCTD},//MI2_OCTD},//MI_OCTU  },
    //             {   _______,         MI_D_5,     MI_B_4,   MI_Gs_4,   MI_F_4,     MI_D_4,     MI_B_3,    MI_Gs_3,   MI_F_3,    MI_D_3,    MI_B_2,    MI_Gs_2, MI_F_2, _______, _______,         MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
    //             {   _______,      MI2_Db_1,  MI2_Ab_1,   MI2_Eb_1,   MI2_Bb_1,   MI2_F_1,    MI2_C_1,    MI2_G_1,   MI2_D_1,    MI2_A_1,     MI2_E_1,    MI2_B_1,    _______, _______, _______, MI_TRNS_0},//TO(_MIDIDRUM)},//MI_TRNSU },
    //             {   _______, _______,   MI3_F_2,  MI3_C_3,   MI3_G_2,   MI3_D_3,     MI3_A_2,   MI3_E_2,    MI3_B_2,    MI3_Fs_2,    MI3_Cs_3,    MI3_Gs_2,    MI3_Ds_3,   _______, _______,    TRIKL_DRUM_TGL},//MI_TRNSD},//MI_TRNSD },
    //             {   _______, _______,      _______,  _______,   _______,    _______, _______,    _______,    _______,     _______,   _______,    _______,    _______,     _______,    _______,  TRIKL_ALT_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
    //           },//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#elif defined(TRIKL_IXLR_T)    
    [_MIDI] = { {   FUNC, _______,  MI_Fs_5,  MI_Ds_5,     MI_C_5,   MI_A_4,  MI_Fs_4,     MI_Ds_4,    MI_C_4,     MI_A_3,     MI_Fs_3,     MI_Ds_3,     MI_C_3,    MI_A_2,    _______,         MI_TRNSU},//MI_OCTD  },
                {   _______,     MI_G_5,    MI_E_5,    MI_Cs_5,  MI_As_4,   MI_G_4,  MI_E_4,   MI_Cs_4,    MI_As_3,     MI_G_3,     MI_E_3,     MI_Cs_3,    MI_As_2,    MI_G_2,  _______,      MI_OCTU},//MI2_OCTD},//MI_OCTU  },
                {   _______,        MI_F_5,  MI_D_5, MI_B_4,   MI_Gs_4,   MI_F_4,   MI_D_4,    MI_B_3,    MI_Gs_3,    MI_F_3,    MI_D_3,    MI_B_2,    MI_Gs_2, _______, _______,              MI_VELU},//MI_TRNSD},//MI_TRNSD },
                {   _______,              MI_Ds_5,  MI_C_5,   MI_A_4,   MI_Fs_4,   MI_Ds_4, MI_C_4,    MI_A_3,   MI_Fs_3,    MI_Ds_3,     MI_C_3,    MI_A_2,    _______, _______, _______,     PC_UP_1},//TO(_MIDIDRUM)},//MI_TRNSU },
                {   _______, _______,          MI_Cs_5, MI_As_4,   MI_G_4,  MI_E_4,   MI_Cs_4,    MI_As_3,   MI_G_3,     MI_E_3,     MI_Cs_3,    MI_As_2,    _______,   _______, _______,      MI_BENDU},//TRIKL_ALT_TGL},//MI2_OCTD },
                {   _______, _______,      _______,  _______,   _______,    _______, MOBELOW,    _______,    _______,     _______,   MOBELOW,    _______,    _______,     _______,    _______, MI_BENDD},//TRIKL_REG_TGL},//MI_TRNS_N2},//MI2_OCTU }
              },

    [_BELOW]= { {   FUNC,_______,  MI_Fs_5,  MI_Ds_5,     MI_C_5,   MI_A_4,  MI_Fs_4,     MI_Ds_4,    MI_C_4,     MI_A_3,     MI_Fs_3,     MI_Ds_3,     MI_C_3,    MI_A_2,    _______,         MI_TRNSD},//MI_OCTD  },
                {   _______,     MI_G_5,    MI_E_5,    MI_Cs_5,  MI_As_4,   MI_G_4,  MI_E_4,   MI_Cs_4,    MI_As_3,     MI_G_3,     MI_E_3,     MI_Cs_3,    MI_As_2,    MI_G_2,  _______,      MI_OCTD},//MI2_OCTD},//MI_OCTU  },
                {   _______,        MI_F_5,  MI_D_5, MI_B_4,   MI_Gs_4,   MI_F_4,   MI_D_4,    MI_B_3,    MI_Gs_3,    MI_F_3,    MI_D_3,    MI_B_2,    MI_Gs_2, _______, _______,              MI_VELD},//MI_TRNSD},//MI_TRNSD },
                {   _______,              MI_Ds_5,  MI_C_5,   MI_A_4,   MI_Fs_4,   MI_Ds_4, MI_C_4,    MI_A_3,   MI_Fs_3,    MI_Ds_3,     MI_C_3,    MI_A_2,    _______, _______, _______,     PC_DN_1},//MI_OCT_0,TO(_MIDIDRUM)},//MI_TRNSU },
                {   _______, _______,          MI_Cs_5, MI_As_4,   MI_G_4,  MI_E_4,   MI_Cs_4,    MI_As_3,   MI_G_3,     MI_E_3,     MI_Cs_3,    MI_As_2,    _______, _______, _______,        PC_UP_10},//TRIKL_ALT_TGL},//MI2_OCTD },
                {   _______, _______,      _______,  _______,   _______,    _______, _______,    _______,    _______,     _______,   _______,    _______,    _______,     _______,    _______, PC_DN_10},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
              },
              
#elif defined(TRIKL_IXLL_T)    
    [_MIDI] = { {      MI5_F_2, _______, MI5_C_3,  MI5_G_2,   MI5_D_3,   MI5_A_2,   MI5_E_2,    MI5_B_2,    MI5_Fs_2,    MI5_Cs_3,    MI5_Gs_2,    MI5_Ds_3,    MI5_As_2,  _______,    _______,    MI2_OCTU},//MI_OCTD  },
                {   _______,     MI4_E_2,  MI4_B_2, MI4_Gb_2,  MI4_Db_3,   MI4_Ab_2,  MI4_Eb_2,   MI4_Bb_2,    MI4_F_2,     MI4_C_3,     MI4_G_2,     MI4_D_3,    MI4_A_2,    _______,  _______,    MI2_OCTD},//MI2_OCTD},//MI_OCTU  },
                {   _______,        MI3_F_2,  MI3_C_3, MI3_G_2,   MI3_D_3,   MI3_A_2,   MI3_E_2,    MI3_B_2,    MI3_Fs_2,    MI3_Cs_3,    MI3_Gs_2,    MI3_Ds_3,    MI3_As_2, _______, _______, MI_BENDU},//MI_TRNSD},//MI_TRNSD },
                {   _______,              MI2_Db_1,  MI2_Ab_1,   MI2_Eb_1,   MI2_Bb_1,   MI2_F_1, MI2_C_1,    MI2_G_1,   MI2_D_1,    MI2_A_1,     MI2_E_1,    MI2_B_1,    MI2_Fs_1, _______, _______, MI_BENDD},//TO(_MIDIDRUM)},//MI_TRNSU },
                {   _______, _______,          MI2_F_1, MI2_C_1,   MI2_G_1,  MI2_D_1,   MI2_A_1,    MI2_E_1,   MI2_B_1,     MI2_Fs_1,     MI2_Cs_1,    MI2_Gs_1,    MI2_Ds_1,   MI2_As_1, _______, TRIKL_DRUM_TGL},//TRIKL_ALT_TGL},//MI2_OCTD },
                {   FUNC,    _______,      _______,  _______,   _______,    _______, _______,    _______,    _______,     _______,   _______,    _______,    _______,     _______,    _______,      TRIKL_ALT_TGL},//TRIKL_ALT_TGL},//MI_TRNS_N2},//MI2_OCTU }
              },
	
  #endif //IX10 or IX11 or IXLR or IXLL
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	[_MIDIDRUM] = { {  FUNC,     _______,      MI_B_2,     MI_G_2, MI_E_2,     MI_Cs_2,  MI_As_1,     MI_Fs_1,    MI_C_2,     MI_A_1,     MI_F_1,     MI_Ds_1,    MI_Cs_1,    MI_B_0,    _______,    MI_TRNSU},//MI_OCTU},//MI_OCTD},//KC_DEL  },
                {   _______,     _______,     MI_A_2,     MI_F_2,  MI_Ds_2,     MI_Fs_2,  MI_Gs_1,     MI_D_2,     MI_B_1,     MI_G_1,     MI_Gs_2,     MI_D_1,    MI_C_1,    _______,    _______,      MI_OCTU},//MI2_OCTU},//MI_OCTU},//KC_HOME },
                {   _______,      _______,    _______,    _______,  _______,   _______, _______,    _______,   _______,    _______,    _______,      _______,      _______,      _______, _______,      MI2_OCTU},//MI_TRNSU},//MI_TRNSD},//KC_PGUP },
                {   _______,    _______,       _______,       _______,    _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,      _______,     _______,      TG(_MIDIDRUM)},//MI_TRNSU},//KC_PGDN },
                {   _______,    _______,      _______,     _______,    _______,    _______, _______,    _______,    _______,    _______,   _______,     _______,    _______,    _______,      _______,      TRIKL_ALT_TGL},//MI2_OCTD  },
                {   _______,       _______,    _______,    _______,   _______,      _______,   MOBELOWDRUM, _______, _______,    _______,    MOBELOWDRUM, _______,    _______,    _______,    _______,      TRIKL_REG_TGL},//MI_TRNS_N2},//MI2_OCTU }
              },
    [_BELOWDRUM]= { {FUNC,    _______,      _______,     _______, _______,     _______,  _______,     _______,    _______,     _______,     _______,     _______,    _______,    _______,    _______,    MI_TRNSD},//MI_OCTD  },
                {   _______,    _______,    _______,     _______,  _______,     _______,  _______,     _______,     _______,     _______,     _______,     _______,    _______,    _______,    _______,   MI_OCTD},//MI2_OCTD},//MI_OCTU  },
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    MI2_OCTD},//MI_TRNSD},//MI_TRNSD },
                {   _______,    _______,    _______,    _______, _______,    _______, _______,    _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    MI_VELU},//MI_TRNS_6},//MI_TRNSU },
                {   _______,    _______,    _______,    _______, _______,    _______, _______,    _______,    _______,    _______,   _______,    _______,    _______,    _______,    _______,    MI_VELD},//MI2_OCTD },
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_DRUM_TGL},//MI2_OCTU }
              },//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif //MIDI
    // [_BASE] = { {   FUNC,     _______,       KC_F1,      KC_F2,   KC_F3,      KC_F4,   KC_F5,     KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     _______,    KC_DEL  },
    //             {   _______,          KC_1,       KC_2,       KC_3,    KC_4,       KC_5,    KC_6,      KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,     _______,      _______,    KC_HOME },
    //             {   _______,   _______,       _______,      _______,   _______,      _______,   _______,     _______,      _______,      _______,      _______,      _______,      _______,      _______,      _______,    KC_PGUP },
    //             {   _______,   _______,       _______,      _______,   KC_F,       KC_G,    KC_H,      KC_J,       KC_K,       KC_L,       _______,      _______,      _______,      _______,      _______,    KC_PGDN },
    //             {   _______,   _______,       _______,      _______,   KC_C,       KC_V,    KC_B,      KC_N,       KC_M,       KC_COMM,    _______,      _______,      _______,      _______,      _______,    KC_END  },
    //             {   _______,   _______,       _______,      _______,   _______,      _______,   KC_SPC,    _______,      _______,      _______,      KC_RALT,    _______,      _______,      _______,      _______,    KC_RGHT },
    //           },
    [_FN]   = { {   _______,    _______,    _______,    _______, _______, _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    FUNC2,    BELLOW_RGB  },//RESET},//EEPROM_RESET,
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_TOG},//on/off RGB,
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_VAI }, // Value
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_MOD },//RGB effect change
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_SAI },//SATURATION
                {   _______,    _______,    _______,    _______, _______,    _______, FUNC2,   _______,    _______,    _______,    FUNC2,    _______,    _______,    _______,    _______,    RGB_HUI }//HUE
             },
    [_FN2]   = { {   _______,    _______,    _______,    _______, _______, _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    QK_RESET,    BELLOW_RGB  },//RESET},//EEPROM_RESET,
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    TRIKL_EV_CC_TGL },//on/off RGB
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_VAD }, // Value
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_RMOD },//RGB effect change reverse
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_SAD },//SATURATION
                {   _______,    _______,    _______,    _______, _______,    _______, _______,   _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_HUD }//HUE
             }
};


uint8_t below_press_num = 0;
uint8_t rgb_matrix_current_mode = 0;
bool bass_mute = 0;

#if defined(MIDI_ENABLE) && defined(MIDI_ADVANCED)
/* Send separate bellow-state CC (0 or 127). */
static inline void trikl_send_bellow_cc(bool bellow_active) {
  midi_send_cc(&midi_device, TRIKL_BELLOW_CC_CH, TRIKL_BELLOW_CC_NUM, bellow_active ? 127 : 0);
}

/* Emit bellow CC only when state toggles between 0 and non-0. */
static bool trikl_bellow_state_last = false;
static inline void trikl_update_bellow_state_cc(void) {
  bool bellow_active = (below_press_num > 0);
  if (bellow_active != trikl_bellow_state_last) {
    trikl_bellow_state_last = bellow_active;
    trikl_send_bellow_cc(bellow_active);
  }
}

static uint8_t  bellow_swap_phase = 0;
static uint16_t bellow_swap_timer = 0;
#endif // MIDI_ENABLE && MIDI_ADVANCED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
	  //TODO, when going to MIDIALT make sure to set off BELOW??
    case MI_TRNSU:
    case MI_TRNSD:
    case MI_TRNS_0:
    case MI_OCTU:
    case MI_OCTD:
    case MI2_OCTD:
    case MI2_OCTU:
		if (record->event.pressed) {
    //return true;  
    //}else{
		  for (uint8_t i = 0; i < 5; i++) {//release first 5 rows (0 to 4)
     	  raw_matrix[i] &= (MATRIX_ROW_SHIFTER << (MATRIX_COLS-1)); //Clear all except conf buttons (last column)
      //TODO konf botoia ez! or bat in bere burukin ta 0a jarri beharreko bitakin
			//last_matrix[i] = 0;//TODO test without this//set to previously 0, so durin next check when it sees it at '1' will send noteon
			//matrix[i] = 0;//TODO: proatxen hau ta aurrekoa kenduta
		  }	       
      //return true;  
     }
    return true; 
     
  
    case BASS_MUTE: 
        if (record->event.pressed) {
          if (bass_mute == 1) {
            bass_mute = 0;
          } else {
            bass_mute = 1;
          }
        }
      return false;
    
    case PANIKO:
      process_midi_all_notes_off();
    return false;
        
    case FUNC:
		if (record->event.pressed) {
		    layer_on(_FN);
		} else {
		    layer_off(_FN);
		    layer_off(_FN2);
		}
		return false;//don't send hid report!
	    
    case FUNC2:
		if (record->event.pressed) {
		    layer_on(_FN2);
		} else {
		    layer_off(_FN2);
		}
		return false;//don't send hid report!
    
	#if defined(MIDI_TRIKL) 
    case MOB: //Momentary bellow
		if (record->event.pressed) { //pressed
			below_press_num++;
			if (below_press_num == 2)
		      return false;//if it is the 2nd press don't release notes again, and layer must be already on (TODO: no harm in setting layer on again anyway?)        
      #if defined(MIDI_ENABLE) && defined(MIDI_ADVANCED)
      trikl_update_bellow_state_cc();
      #endif
      //right hand
      if (layer_state_is(_R1)) {
        layer_on(_RB1);
      }
      else if (layer_state_is(_R2)) {
        layer_on(_RB2);
      }
      else if (layer_state_is(_R3)) {
        layer_on(_RB3);
      }
      //left hand
      if (layer_state_is(_L1)) {
        layer_on(_LB1);
      }
      else if (layer_state_is(_L2)) {
        layer_on(_LB2);
      }
      else if (layer_state_is(_L3)) {
        layer_on(_LB3);
      }
          
		} else {//depressed
			below_press_num--;
			if (below_press_num == 0) {//both depressed -> release notes
        #if defined(MIDI_ENABLE) && defined(MIDI_ADVANCED)
        trikl_update_bellow_state_cc();
        #endif
          //right hand
        if (layer_state_is(_RB1)) {
          layer_off(_RB1);
        }
        else if (layer_state_is(_RB2)) {
          layer_off(_RB2);
        }
        else if (layer_state_is(_RB3)) {
          layer_off(_RB3);
        }
        //left hand
        if (layer_state_is(_LB1)) {
          layer_off(_LB1);
        }
        else if (layer_state_is(_LB2)) {
          layer_off(_LB2);
        }
        else if (layer_state_is(_LB3)) {
          layer_off(_LB3);
        }
      }
			else //if still 1 pressed don't release notes
			  return false;
		}
	  //both, when pressing and when releasing MOB
    #if defined(MIDI_ENABLE) && defined(MIDI_ADVANCED)
		bool any_held = false;
		for (uint8_t i = 0; i <= 4; i++) {
			if (raw_matrix[i] != 0) {
				any_held = true;
				break;
			}
		}
		if (any_held) {
			trikl_bellow_swap_suppress_cc = true;
			bellow_swap_phase = 1;
			bellow_swap_timer = timer_read();
		}
#endif
		///for (uint8_t i = 0; i <= 2; i++) {//release first 3 rows to silence playing notes (keep bass)
		for (uint8_t i = 0; i <= 4; i++) {//release first 5 rows to silence playing notes
			raw_matrix[i] = 0;//TODO konf botoia ez! or bat in bere burukin ta 0a jarri beharreko bitakin
			//last_matrix[i] = 0;//TODO test without this //set to previously 0, so durin next check when it sees it at '1' will send noteon
			//matrix[i] = 0;//TODO: proatxen hau ta aurrekoa kenduta
		}
		return false;//don't send hid report!

    case NEXTR:
        if (record->event.pressed) {
            // Check which Right layer is currently active and swap to the next one
            // We turn off the old one, and turn on the new one.
            if (layer_state_is(_R1)) {
                layer_off(_R1);
                layer_on(_R2);
                if (layer_state_is(_RB1)){
                  layer_off(_RB1);
                  layer_on(_RB2);
                }
            } 
            else if (layer_state_is(_R2)) {
                layer_off(_R2);
                layer_on(_R3);
                if (layer_state_is(_RB2)){
                  layer_off(_RB2);
                  layer_on(_RB3);
                }
            } 
            else if (layer_state_is(_R3)) {
                layer_off(_R3);
                layer_on(_R4);
                if (layer_state_is(_RB3)){
                  layer_off(_RB3);
                  layer_on(_RB4);
                }
            }
            else {
                layer_off(_R4);
                layer_on(_R1); // Loop back to the first one
                if (layer_state_is(_RB4)){
                  layer_off(_RB4);
                  layer_on(_RB1);
                }
            }
        }
        return false; // Tells QMK we handled it

    case NEXTL:
        if (record->event.pressed) {
            // Check which Right layer is currently active and swap to the next one
            // We turn off the old one, and turn on the new one.
            if (layer_state_is(_L1)) {
                layer_off(_L1);
                layer_on(_L2);
                if (layer_state_is(_LB1)){
                  layer_off(_LB1);
                  layer_on(_LB2);
                }
            } 
            else if (layer_state_is(_L2)) {
                layer_off(_L2);
                layer_on(_L3);
                if (layer_state_is(_LB2)){
                  layer_off(_LB2);
                  layer_on(_LB3);
                }
            } 
            else if (layer_state_is(_L3)) {
                layer_off(_L3);
                layer_on(_L4);
                if (layer_state_is(_LB3)){
                  layer_off(_LB4);
                  layer_on(_LB4);
                }
            } 
            else {
                layer_off(_L4);
                layer_on(_L1); // Loop back to the first one
                if (layer_state_is(_LB4)){
                  layer_off(_LB4);
                  layer_on(_LB1);
                }
            }
        }
        return false; // Tells QMK we handled it
		
  //   case MOBELOWALT:
	// 	if (record->event.pressed) {
	// 	    layer_on(_BELOWALT);
	// 		below_press_num++;
  //     #if defined(MIDI_ENABLE) && defined(MIDI_ADVANCED)
  //     trikl_update_bellow_state_cc();
  //     #endif
	// 		if (below_press_num == 2)
	// 	      return false;//if 2nd press don't release notes
	// 	} else {//depressed
	// 		below_press_num--;
  //     #if defined(MIDI_ENABLE) && defined(MIDI_ADVANCED)
  //     trikl_update_bellow_state_cc();
  //     #endif
	// 		if (below_press_num == 0) //fully depressed
	// 	      layer_off(_BELOWALT);
	// 		else //if still 1 pressed don't release notes
	// 		  return false;
	// 	}
	// //both, when pressing and when releasing
	// 	for (uint8_t i = 0; i <= 4; i++) {//release first 5 rows
	// 		raw_matrix[i] = 0;//TODO konf botoia ez! or bat in bere burukin ta 0a jarri beharreko bitakin
	// 		//last_matrix[i] = 0;//TODO test without this//set to previously 0, so durin next check when it sees it at '1' will send noteon
	// 		//matrix[i] = 0;//TODO: proatxen hau ta aurrekoa kenduta
	// 	}
	// 	return false;//don't send hid report!
				
  //   case MOBELOWDRUM:
	// 	if (record->event.pressed) {
	// 	    layer_on(_BELOWDRUM);
	// 		below_press_num++;
  //     #if defined(MIDI_ENABLE) && defined(MIDI_ADVANCED)
  //     trikl_update_bellow_state_cc();
  //     #endif
	// 		if (below_press_num == 2)
	// 	      return false;//if 2nd press don't release notes
	// 	} else {//depressed
	// 		below_press_num--;
  //     #if defined(MIDI_ENABLE) && defined(MIDI_ADVANCED)
  //     trikl_update_bellow_state_cc();
  //     #endif
	// 		if (below_press_num == 0) //fully depressed
	// 	      layer_off(_BELOWDRUM);
	// 		else //if still 1 pressed don't release notes
	// 		  return false;
	// 	}
	// //both, when pressing and when releasing
	// 	for (uint8_t i = 0; i <= 4; i++) {//release first 5 rows
	// 		raw_matrix[i] = 0;//TODO konf botoia ez! or bat in bere burukin ta 0a jarri beharreko bitakin
	// 		//last_matrix[i] = 0;//TODO test without this//set to previously 0, so durin next check when it sees it at '1' will send noteon
	// 		//matrix[i] = 0;//TODO: proatxen hau ta aurrekoa kenduta
	// 	}
	// 	return false;//don't send hid report!
	    
  case BELLOW_RGB:
    if (record->event.pressed) {
      // if current RGB is not TRIKL_SOLID save current RGB mode and set to TRIKL_SOLID
      if (rgb_matrix_get_mode() != RGB_MATRIX_CUSTOM_TRIKL_SOLID) {
        rgb_matrix_current_mode = rgb_matrix_get_mode();
        rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_TRIKL_SOLID);
      } else { //current RGB is TRIKL_SOLID restore previous RGB mode
        rgb_matrix_mode_noeeprom(rgb_matrix_current_mode);
      }
    }  
	// case TG(_MIDI) :// default is KB MODE, move to MIDI
	//     //if (record->event->key.row=2 && record->event->key.column=15) {
	// 		if (record->event.pressed) {
	// 			//TODO testing base/midi switch from/to midialt: already taken into account MIDIALT -> another option was to use default_layer_set(layer_mask)
  //       if (layer_state_is(_MIDI)) {//leaving MIDI -> restore previous lighting mode
	// 				writePin(LED_CAPS_LOCK_PIN, 1);//LED Off = KB mode
  //         #if defined TRIKL_RGB
  //         	rgb_matrix_mode_noeeprom(rgb_matrix_current_mode);//Restore previous lighting mode
  //         #endif
  //       } else {//entering MIDI -> force TRIKL_SOLID
	// 				writePin(LED_CAPS_LOCK_PIN, 0);//LED on = MIDI mode
  //         rgb_matrix_current_mode = rgb_matrix_get_mode();
  //         #if defined TRIKL_RGB
  //           rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_TRIKL_SOLID);
  //         #endif
	// 				//rgb_matrix_disable_noeeprom()	)//TODO: Turn off LED effects to allow raw messages, or maybe set some custom lighting with only non-playing leds on
	// 			}
	// 		}
	// 	//}
	// 	return true;
 case TG(_MIDIDRUM) :
			if (record->event.pressed) { // TODO FIXME, if in bellow change to MIDIDRUM BELLOW, so that without needing to realease the bellow the functions are the expected ones
				if (layer_state_is(_MIDIDRUM)) {//if midi is enabled, it is about to be disabled
          enter_drum_mode(0);
					writePin(LED_CAPS_LOCK_PIN, 0); //LED OFF
				} else {
          enter_drum_mode(1);
					writePin(LED_CAPS_LOCK_PIN, 1); // LED ON
				}
			}
		//}
		return true;
#endif

	// case TG(_BASE) ://default is MIDI, move to KB MODE
	//     //if (record->event->key.row=2 && record->event->key.column=15) {
	// 		if (record->event.pressed) {
	// 			//TODO testing base/midi switch from/to midialt: already taken into account MIDIALT -> another option was to use default_layer_set(layer_mask)
  //       if (layer_state_is(_BASE)) {//leaving BASE -> entering MIDI, force TRIKL_SOLID
	// 				writePin(LED_CAPS_LOCK_PIN, 1);
  //         rgb_matrix_current_mode = rgb_matrix_get_mode();
  //         #if defined TRIKL_RGB
  //           rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_TRIKL_SOLID);
  //         #endif
  //       } else {//entering BASE -> restore previous lighting mode
  //         #if defined TRIKL_RGB
  //         	rgb_matrix_mode_noeeprom(rgb_matrix_current_mode);
  //         #endif
	// 				writePin(LED_CAPS_LOCK_PIN, 0);//led on=kb mode
	// 				//rgb_matrix_disable_noeeprom()	)//TODO: Turn off LED effects to allow raw messages, or maybe set some custom lighting with only non-playing leds on
	// 			}
	// 		}
	// 	//}
	// 	return true;
  }
  return true;
}
//TODO XAM: remove all the base layer part, and use the CAPS LED to show MIDI connection was stablished with the host (any MIDI was received)
//variable trikl_midi_message_received is already set
void keyboard_post_init_user(void) {
#ifndef MIDI_TRIKL //if basic trikeyboard -> CAPS LED always ON
	writePin(LED_CAPS_LOCK_PIN, 0);//led on=kb mode  //TODO: basikoa argikin bada re in ber du!
#endif

#ifdef INIT_AS_KB // if it is desired to start at keyboard mode
//  layer_on(_BASE);
//  layer_off(_MIDI);
	writePin(LED_CAPS_LOCK_PIN, 0);//led on=kb mode  
#endif

#if defined(TRIKL_RGB)
	// Force custom solid mode on boot so reactive modes from EEPROM cannot mask MIDI overrides.
	rgb_matrix_enable_noeeprom();
	rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_TRIKL_SOLID);
#endif
  //debug_mouse=true;
}

void matrix_scan_user(void) {
#if defined(MIDI_ENABLE) && defined(MIDI_ADVANCED)
	if (bellow_swap_phase == 1) {
		// Releases are dispatched during this scan; move to phase 2 to await hardware re-press scan
		bellow_swap_phase = 2;
	} else if (bellow_swap_phase == 2) {
		if ((matrix[0] | matrix[1] | matrix[2] | matrix[3] | matrix[4]) != 0) {
			// Hardware has read keys; re-press Note-Ons will be dispatched during this scan (suppress_cc is still true)
			bellow_swap_phase = 3;
		} else if (timer_elapsed(bellow_swap_timer) > 30) {
			// Safety timeout if keys were released during swap
			trikl_bellow_swap_suppress_cc = false;
			bellow_swap_phase = 0;
		}
	} else if (bellow_swap_phase == 3) {
		// Re-press Note-Ons finished; re-enable Event CC for subsequent events
		trikl_bellow_swap_suppress_cc = false;
		bellow_swap_phase = 0;
	}
#endif
}

/*extern rgb_config_t rgb_matrix_config;*/
extern midi_config_t   midi_config;//right
// extern volatile uint8_t midi_debug_counter;
// extern volatile uint8_t midi_debug_pulse;
// extern volatile uint8_t midi_debug_last_status;
// extern volatile uint8_t midi_debug_last_command;
// extern volatile uint8_t midi_debug_last_channel;
// extern volatile uint8_t midi_debug_last_note;
// extern volatile uint8_t midi_debug_last_value;
	int8_t transpose_z = 20;//initial bad value to force rgb refresh 
	RGB rgb = {0, 0, 0};
  
void rgb_matrix_indicators_user(void)  {
	if (midi_config.transpose != transpose_z) {
		transpose_z = midi_config.transpose;
		switch (midi_config.transpose) {
			//TODO, hau traspose ittean lekun in ta RGB bariablea izan deila bakarrik globala, hemendik soilik leda eguneratu
		//for (uint8_t i = 0;  i <= 12; i++) {
		  //switch (i-6) {
			case 0: //sol 296
      case 12:
			  rgb.r = 255;
			  rgb.g = 96;//roxa argittu moretik bereizteko
			  rgb.b = 128;
			  break;
			case 1: //sol# 300
      case 13:
			  rgb.r = 255;
			  rgb.g = 0;
			  rgb.b = 64;
			  break;
			case 2: //la 355
			  rgb.r = 255;
			  rgb.g = 0;
			  rgb.b = 0;
			  break;
			case 3: //SIb 40
      case -9:
			  rgb.r = 255;
			  rgb.g = 64;
			  rgb.b = 0;
			  break;
			case 4: //Si 55 salto txikie baiio nahiko ondo bereiztea!
      case -8:
			  rgb.r = 255;
			  rgb.g = 196;
			  rgb.b = 0;
			  break;
			case 5: //Do 75// salto haundixoa komeni
      case -7:
			  rgb.r = 128;
			  rgb.g = 255;
			  rgb.b = 0;
			  break;
			case 6: //Do# //140
			case -6: //Do#
			  rgb.r = 0;
			  rgb.g = 255;//140
			  rgb.b = 0;
			  break;
			case 7: //Re
			case -5: //Re 185
			  rgb.r = 0;
			  rgb.g = 255;
			  rgb.b = 64;
			  break;
			case 8: //Re#
			case -4: //Re# 185
			  rgb.r = 96;
			  rgb.g = 255;
			  rgb.b = 255;
			  break;
			case 9: //Mi
			case -3: //Mi 190
			  rgb.r = 0;
			  rgb.g = 140;
			  rgb.b = 255;
			  break;
			case 10: //Fa
			case -2: //Fa 210
			  rgb.r = 0;
			  rgb.g = 0;
			  rgb.b = 255;
			  break;
      case 11:
			case -1: //Fa# 260
			  rgb.r = 96;
			  rgb.g = 0;
			  rgb.b = 255;
			  break;
			  
			default:
			  rgb.r = 255;
			  rgb.g = 255;
			  rgb.b = 255;
			  break;
		  }
		}
    //rgb_matrix_set_color(i,rgb.r, rgb.g, rgb.b);
	// }
	
	///set transpose button with proper color
  
#if defined(MIDI_ADVANCED) //BASIC doesn't need to report tonality
    rgb_matrix_set_color(TRANS_BUT_NUM,rgb.r, rgb.g, rgb.b);
#endif
  for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
  // for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
    if (is_key_led(i) && i != TRANS_BUT_NUM && LED_OVERRIDES[i]) {
      rgb_matrix_set_color(i, LED_BELLOWS ? 0 : 255, LED_BELLOWS ? 255 : 0, 0);
		}
	}

//legaxyn zan:
//event=9 8 eo B 
//Data1= 0F opening below bestela closing bellow
//Data2 lednum
//Data3 argin intensidadea? 255 eo 0


  // DEBUG: LEDs 0-7 show raw bits of Data1 (red) and Data3 (blue).
  // LED 0 = bit 7 (MSB), LED 7 = bit 0 (LSB).
  // for (uint8_t b = 0; b < 8; b++) {
  //   uint8_t mask = (1 << (7 - b));
  //   uint8_t r = (midi_debug_last_status & mask) ? 255 : 0;
  //   uint8_t gr = (midi_debug_last_note & mask) ? 255 : 0;
  //   uint8_t bl = (midi_debug_last_value  & mask) ? 255 : 0;
  //   rgb_matrix_set_color(b+1, r, gr, bl);
  // }
}