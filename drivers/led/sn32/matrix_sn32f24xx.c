/*
Copyright 2021 Dimitris Mantzouranis <d3xter93@gmail.com>
Copyright 2021 Adam Honse <calcprogrammer1@gmail.com>
Copyright 2011 Jun Wako <wakojun@gmail.com>

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

Ported to QMK by Stephen Peery <https://github.com/smp4488/>
*/

#include <stdint.h>
#include <stdbool.h>
#include "ch.h"
#include "hal.h"

#include "config.h"

#include "wait.h"
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "quantum.h"
#include "sn32f24xb.h"
#include <LUFA/Drivers/USB/USB.h>
#include "midi.h"
#include "qmk_midi.h"
#include "process_midi.h"
extern bool keep_bending_down;
extern bool keep_bending_up;
 int32_t  bend = 0;
 extern midi_config_t   midi_config;//right
static uint16_t init_scan_count = 0;

#if defined(OPTICAL_MATRIX)
#ifndef PRESSED_KEY_PIN_STATE
#  define PRESSED_KEY_PIN_STATE 1
#endif
#endif

#ifndef PRESSED_KEY_PIN_STATE
#  define PRESSED_KEY_PIN_STATE 0
#endif

#ifndef MATRIX_KEY_SAMPLE_DELAY
#  define MATRIX_KEY_SAMPLE_DELAY 100
#endif

#if defined(MATRIX_KEY_SAMPLE_DELAY)
void sample_delay(void){
  //should give 2000/48.000.000Hz = 41.66us delay
  //we want 42 micro thus to get value
  for (int i = 0; i < MATRIX_KEY_SAMPLE_DELAY; ++i) {
  __asm__ volatile("" ::: "memory");
  }
  //wait_us(73); //does not work
}
#endif

static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;


// Función externa definida en keymap.c para identificar qué teclas son notas musicales según la variante activa
extern bool is_key_led(uint8_t i);
#ifdef RGB_MATRIX_ENABLE
extern led_config_t g_led_config;
#endif

/**
 * Determina si una tecla física (row, col) está en uso en el instrumento actual.
 * Evita escanear pines de pulsadores no conectados o no utilizados.
 *
 * Incluye:
 * 1. Teclas musicales: identificadas dinámicamente mediante is_key_led(led_index).
 * 2. Botones de control y funciones:
 *    - Columna 15: Botones laterales derechos (Octava, Transposición, Registro, etc.).
 *    - Fila 5, columnas 6 y 10: Barra espaciadora y botones de fuelle (MOBELOW).
 *    - Fila 0, columna 0: Tecla ESC / FUNC.
 *    - Variantes específicas (IXLL, IX10, IXLR): Teclas adicionales de función/bajos.
 */
static inline bool is_key_used(uint8_t row, uint8_t col) {
#ifdef RGB_MATRIX_ENABLE
    // Comprobar si la tecla pertenece al grupo de notas musicales de esta variante
    uint8_t led = g_led_config.matrix_co[row][col];
    if (led != NO_LED && is_key_led(led)) {
        return true;
    }
#endif
    // Botones de función fijos comunes a todos los modelos
    if (col == 15) return true;                           // Columna derecha (Octavas, Transpose, Registro)
    if (row == 5 && (col == 6 || col == 10)) return true; // Fuelle / Espacio (MOBELOW)
    if (row == 0 && col == 0) return true;                // Tecla ESC / FUNC

    // Teclas específicas según el modelo de instrumento
#if defined(TRIKL_IXLL_T)
    if (row == 5 && col == 0) return true;
#endif
#if defined(TRIKL_IX10_T) || defined(TRIKL_IXLR_T) || defined(TRIKL_IXLL_T)
    if (row == 0 && col == 1) return true;
#endif

    return false;
}



matrix_row_t raw_matrix[MATRIX_ROWS] = {0}; //raw values
matrix_row_t last_matrix[MATRIX_ROWS] = {0};  // raw values
matrix_row_t matrix[MATRIX_ROWS] = {0}; //debounced values

static bool matrix_changed = false;

__attribute__((weak)) void matrix_init_kb(void) { matrix_init_user(); }

__attribute__((weak)) void matrix_scan_kb(void) { matrix_scan_user(); }

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}

inline matrix_row_t matrix_get_row(uint8_t row) { return matrix[row]; }

void matrix_print(void) {}

static void init_pins(void) {
#if(DIODE_DIRECTION == ROW2COL)
  //  Unselect ROWs
  for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
  setPinInputHigh(row_pins[x]);
  }
#elif(DIODE_DIRECTION == COL2ROW) //xam ajazz ak33 rev2
  // Unselect COLs
  for (uint8_t x = 0; x < MATRIX_COLS; x++) {
  setPinInputHigh(col_pins[x]);
  }
#else
#error DIODE_DIRECTION must be one of COL2ROW or ROW2COL!
#endif
}

void matrix_scan_select(uint8_t current){ 
#if(DIODE_DIRECTION == ROW2COL)
  // Select COL
  setPinOutput(col_pins[current]);
  writePinLow(col_pins[current]);
#elif(DIODE_DIRECTION == COL2ROW)
  // Select ROW
  setPinOutput(row_pins[current]);
  writePinLow(row_pins[current]);
#endif
}

void matrix_scan_unselect(uint8_t current) {
#if(DIODE_DIRECTION == ROW2COL)
  // Select COL
  setPinInputHigh(col_pins[current]);
#elif(DIODE_DIRECTION == COL2ROW)
  // Select ROW
  setPinInputHigh(row_pins[current]);
#endif
}

void matrix_init(void) {
  // initialize key pins
  init_pins();

  // initialize matrix state: all keys off
  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
  raw_matrix[i] = 0;
  matrix[i]  = 0;
  }

  debounce_init(MATRIX_ROWS);

  matrix_init_quantum();
}

uint8_t matrix_scan(void) {
  //XAM////////////////////////////
  if (keep_bending_down == 1) {
    if (bend > -0x2000) {
      bend = bend - 10;
      midi_send_pitchbend(&midi_device, midi_config.channel, bend);
    }
  } else if(keep_bending_up == 1) {
    if (bend < 0x1fff) {   
      bend = bend + 10;
      midi_send_pitchbend(&midi_device, midi_config.channel, bend);
    }
  } else {
    if (bend < -50) {
      bend = bend + 50;
      midi_send_pitchbend(&midi_device, midi_config.channel, bend);
    } else if (bend > 50) {
      bend = bend - 50;
      midi_send_pitchbend(&midi_device, midi_config.channel, bend);
    } else if ( bend != 0) {
      bend = 0;
      midi_send_pitchbend(&midi_device, midi_config.channel, bend);
    }
  }
  
  matrix_changed = false;
  for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++) {
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
    // Determine if the matrix changed state
      if ((last_matrix[row_index] != raw_matrix[row_index])) {
        matrix_changed  = true;
        last_matrix[row_index] = raw_matrix[row_index];
      }
    }
  }
  if (init_scan_count <= 2000) {
    init_scan_count++;
    matrix_changed = false;
    memset(raw_matrix, 0, sizeof(raw_matrix)); // raw values
    memset(last_matrix, 0, sizeof(last_matrix)); // raw values
  } else {
    debounce(raw_matrix, matrix, MATRIX_ROWS, matrix_changed);
  }
  matrix_scan_quantum();

  return matrix_changed;
}

void matrix_scan_keys(matrix_row_t raw_matrix[], uint8_t current){
  #if(DIODE_DIRECTION == ROW2COL)
			//XAM comented for clarity
  /*// Read the key matrix rows on col
  uint8_t col_index = current;
  // Enable the column
  matrix_scan_select(col_index);
  sample_delay();
  for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
   // Check row pin state
   if (readPin(row_pins[row_index]) == PRESSED_KEY_PIN_STATE) {
    // Pin LO, set col bit
    raw_matrix[row_index] |= (MATRIX_ROW_SHIFTER << col_index);
   } else {
    // Pin HI, clear col bit
    raw_matrix[row_index] &= ~(MATRIX_ROW_SHIFTER << col_index);
   }
  }
  // Disable the column
  matrix_scan_unselect(col_index);
  //see https://github.com/SonixQMK/qmk_firmware/issues/157
  sample_delay();*/
  #elif(DIODE_DIRECTION == COL2ROW)//XAM ak33 rev2
  // Read the key matrix cols on row
  uint8_t row_index = current;
  // Enable the row unless NO_PIN//XAM
			//if (row_pins[row_index] != NO_PIN) {//XAM I won't disable a row so no sense for now
			matrix_scan_select(row_index);
			sample_delay(); //wait after selection
			for (uint8_t col_index = 1; col_index <= MATRIX_COLS; col_index++) {
				// Check row pin state unless NO_PIN
				if (is_key_used(row_index, MATRIX_COLS - col_index)) {//check only if used, else skip it
				  //TODO chek if its space button, if it is and has changed since last check, report that all keys need to be released
				  /*if (row_index == 5 && col_index == 6) {
						if last_matrix[row_index] = raw_matrix[row_index];
					}*/
					if (readPin(col_pins[MATRIX_COLS-col_index]) == PRESSED_KEY_PIN_STATE) {
						// Pin LO, set col bit
						raw_matrix[row_index] |= (MATRIX_ROW_SHIFTER << (MATRIX_COLS-col_index));
					} else {
						// Pin HI, clear col bit
						raw_matrix[row_index] &= ~(MATRIX_ROW_SHIFTER << (MATRIX_COLS-col_index));
					}
				}
			}
			// Disable the row
			matrix_scan_unselect(row_index);
  //}
  //see https://github.com/SonixQMK/qmk_firmware/issues/157
  //sample_delay();//XAM kentzen proatzet, COL2ROW not affected esateu ta
  #endif
}
