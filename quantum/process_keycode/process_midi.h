/* Copyright 2016 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "quantum.h"

#ifdef MIDI_ENABLE

//#    ifdef MIDI_BASIC 
#if defined(MIDI_BASIC) || defined(MIDI_ADVANCED) //XAM
void process_midi_basic_noteon(uint8_t note);
void process_midi_basic_noteoff(uint8_t note);
void process_midi_all_notes_off(void);
void set_registroa(bool ON_OFF);
#    endif

void midi_task(void);

#    ifdef MIDI_ADVANCED
typedef union {
    uint32_t raw;
    struct {
        uint8_t octave : 4;
        int8_t  transpose : 4;
        uint8_t velocity : 7;
        uint8_t channel : 4;
        uint8_t modulation_interval : 4;
    };
} midi_config_t;

extern midi_config_t midi_config;

void midi_init(void);
bool process_midi(uint16_t keycode, keyrecord_t *record);
extern bool LED_OVERRIDES[DRIVER_LED_TOTAL];
extern uint8_t LED_BELLOWS;
extern volatile bool trikl_midi_message_received;

#        define MIDI_INVALID_NOTE 0xFF
#        define MIDI_TONE_COUNT (MIDI_TONE_MAX - MIDI_TONE_MIN + 1)

uint8_t midi_compute_note(uint16_t keycode);
#    endif  // MIDI_ADVANCED

#endif  // MIDI_ENABLE
