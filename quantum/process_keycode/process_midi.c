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
#include "process_midi.h"

#ifdef MIDI_ENABLE
#    include <LUFA/Drivers/USB/USB.h>
#    include "midi.h"
#    include "qmk_midi.h"

//#    ifdef MIDI_BASIC 
#if defined(MIDI_BASIC) || defined(MIDI_ADVANCED) //XAM

void process_midi_basic_noteon(uint8_t note) { midi_send_noteon(&midi_device, 0, note, 127); }

void process_midi_basic_noteoff(uint8_t note) { midi_send_noteoff(&midi_device, 0, note, 0); }

void process_midi_all_notes_off(void) { midi_send_cc(&midi_device, 0, 0x7B, 0); }

#    endif  // MIDI_BASIC || MIDI_ADVANCED

#    ifdef MIDI_ADVANCED

#        include "timer.h"


static uint8_t tone_status[2][MIDI_TONE_COUNT];
static uint8_t tone_status2[MIDI_TONE_COUNT];
static uint8_t tone_status3[MIDI_TONE_COUNT];
static uint8_t tone_status4[MIDI_TONE_COUNT];
//static uint8_t drum_status_kick[2];
//static uint8_t drum_status_snare[2];
static uint8_t tone_statusreg[2][MIDI_TONE_COUNT];

static uint8_t  midi_modulation;
static int8_t   midi_modulation_step;
static uint16_t midi_modulation_timer;
midi_config_t   midi_config;//right
midi_config_t   midi2_config;//bass
midi_config_t   midi3_config;//M third
midi_config_t   midi4_config;//m third
midi_config_t   midi5_config;//fifth
midi_config_t   midi6_config;//root
midi_config_t   midi7_config;//seventh
midi_config_t   midi10_config;//BASS DRUM 35/36 SNARE 38
bool registroa = 0;
bool drum_mode = 0;
bool registroa_bkp = 0;
bool drums = DRUMS_POR;
bool drums_bkp = DRUMS_POR;
bool midi_alt = 0;
bool midi_alt_bkp = 0;
bool keep_bending_down = 0;
bool keep_bending_up   = 0;
extern bool bass_mute;

inline uint8_t compute_velocity(uint8_t setting) { return setting * (128 / (MIDI_VELOCITY_MAX - MIDI_VELOCITY_MIN)); }

void midi_init(void) {
    midi_config.octave              = MI_OCT_0 - MIDI_OCTAVE_MIN;//XAM: MI_OCT_2 - MIDI_OCTAVE_MIN
    midi_config.transpose           = TRANSPOSE_POR;//0 sol, 3 si bemol //XAM TODO: hau config.h-n jarri
    midi2_config.transpose        = TRANSPOSE_POR;
    midi_config.velocity            = 127;//XAM
    midi_config.channel             = 0;
    midi_config.modulation_interval = 8;
	
    midi2_config.octave              = MI_OCT_0 - MIDI_OCTAVE_MIN;//XAM: MI_OCT_2 - MIDI_OCTAVE_MIN
    midi2_config.velocity            = 90;//XAM
    midi2_config.channel             = 1;
    midi2_config.modulation_interval = 8;
	
    midi3_config.channel             = 2; //major
    midi4_config.channel             = 3; //minor
    midi5_config.channel             = 4; //channel 5 for fifths
    midi6_config.channel             = 5; //channel 6 for roots
    //midi7_config.channel             = 6; //channel 7 for sevenths
    midi10_config.channel             = 9; //channel 10 for drums

    for (uint8_t i = 0; i < MIDI_TONE_COUNT; i++) {
        tone_status[0][i] = MIDI_INVALID_NOTE;//note without transpositions
        tone_statusreg[0][i] = MIDI_INVALID_NOTE;//note without transpositions
        tone_status[1][i] = 0;//number of times it is pressed
        tone_statusreg[1][i] = 0;//number of times it is pressed
    }

    midi_modulation       = 0;
    midi_modulation_step  = 0;
    midi_modulation_timer = 0;
}

uint8_t midi_compute_note(uint16_t keycode)  {
  if (drum_mode == 0) {
    return 12 * midi_config.octave  + (keycode - MIDI_TONE_MIN)  + midi_config.transpose;
  } else { // in drum mode don't aply octave and transpose
    return 12 * (MI_OCT_0 - MIDI_OCTAVE_MIN) + (keycode - MIDI_TONE_MIN);
  }
}
uint8_t midi2_compute_note(uint16_t keycode) { return 12 * midi2_config.octave + (keycode - MIDI2_TONE_MIN) + midi_config.transpose; }
uint8_t midi3_compute_note(uint16_t keycode) { return 12 * midi2_config.octave + (keycode - MIDI3_TONE_MIN) + midi_config.transpose; }
uint8_t midi4_compute_note(uint16_t keycode) { return 12 * midi2_config.octave + (keycode - MIDI4_TONE_MIN) + midi_config.transpose; }
uint8_t midi7_compute_note(uint16_t keycode) { return 12 * midi2_config.octave + (keycode - MIDI5_TONE_MIN) + midi_config.transpose; }
//uint8_t midir_compute_note(uint16_t keycode) { return 12 * midi2_config.octave + (keycode - MIDIr_TONE_MIN) + midi_config.transpose; }

bool process_midi(uint16_t keycode, keyrecord_t *record) {

  
  
    switch (keycode) {
        case MIDI_TONE_MIN ... MIDI_TONE_MAX: {
            uint8_t channel  = midi_config.channel + midi_alt*10;
            uint8_t tone     = keycode - MIDI_TONE_MIN;
            uint8_t velocity = midi_config.velocity;
            if (record->event.pressed) {
                uint8_t note = midi_compute_note(keycode);
                //check if not already pressed
                if (tone_status[1][tone] == 0) {//enable the note only if it hasnt already been played
                  midi_send_noteon(&midi_device, channel, note, velocity);
                    tone_status[0][tone] = note;
                  dprintf("midi noteon channel:%d note:%d velocity:%d\n", channel, note, velocity);
                }
                tone_status[1][tone] += 1; //increase number of times note is played
                //REGISTROA
                if (registroa == 1) {
                  if (tone_status[1][tone-12] == 0) {//enable the note only if it hasnt already been played{
                    midi_send_noteon(&midi_device, channel, note-12, 100);
                    tone_status[0][tone-12] = note-12;
                  }
                  tone_status[1][tone-12] += 1;
                  tone_statusreg[1][tone] += 1;
                }
            } else {//depressed
                uint8_t note = tone_status[0][tone];
                tone_status[1][tone] -= 1;
                if (tone_status[1][tone] == 0) {//disable the note only if its been played only once, else don't disable it
                    midi_send_noteoff(&midi_device, channel, note, velocity);
                    dprintf("midi noteoff channel:%d note:%d velocity:%d\n", channel, note, velocity);
                }
                //registroa
                if (registroa == 1) {
                    tone_status[1][tone-12] -= 1;
                    tone_statusreg[1][tone] -= 1;
                    if (tone_status[1][tone-12] == 0) {
                      midi_send_noteoff(&midi_device, channel, note-12, 100);
                    }
                }
            }
            return false;
        } //case
		
//
//XAM MIDI2//BASS ROOT
        case MIDI2_TONE_MIN ... MIDI2_TONE_MAX: {
            uint8_t velocity = midi2_config.velocity;
            uint8_t channel  = midi2_config.channel + midi_alt*10;;
            uint8_t tone     = keycode - MIDI2_TONE_MIN;
            uint8_t channel_drums  = midi10_config.channel;
            if (record->event.pressed) {
              if (bass_mute == 0) {
                uint8_t note = midi2_compute_note(keycode);
                tone_status2[tone] = note;
                midi_send_noteon(&midi_device, channel, note, velocity);
                if (drums)
                  midi_send_noteon(&midi_device, channel_drums, 36, 90);//bass drum
                //midi_send_noteon(&midi_device, channel, note+12, 124);
                dprintf("midi noteon channel:%d note:%d velocity:%d\n", channel, note, velocity);
              }
            } else {
                uint8_t note = tone_status2[tone];
                midi_send_noteoff(&midi_device, channel, note, velocity);
                if (drums)
                  midi_send_noteoff(&midi_device, channel_drums, 36, 90);//bass drum
				//midi_send_noteoff(&midi_device, channel, note+12, 124);
                dprintf("midi noteoff channel:%d note:%d velocity:%d\n", channel, note, velocity);
            }
            return false;
        }
//XAM MIDI3//MAJORCHORD
        case MIDI3_TONE_MIN ... MIDI3_TONE_MAX: {
            uint8_t velocity = midi_config.velocity;
            uint8_t channel  = midi3_config.channel + midi_alt*10;
            uint8_t tone     = keycode - MIDI3_TONE_MIN;
            uint8_t channel5  = midi5_config.channel + midi_alt*10;
            uint8_t channel6  = midi6_config.channel + midi_alt*10;
            //uint8_t channel7  = midi7_config.channel + midi_alt*10;
            uint8_t channel_drums  = midi10_config.channel;
            if (record->event.pressed) {
              if (bass_mute == 0) {
                uint8_t note = midi3_compute_note(keycode);
                tone_status3[tone] = note;
                midi_send_noteon(&midi_device, channel, note, velocity);
                midi_send_noteon(&midi_device, channel5, note+3, velocity);//fifth
                midi_send_noteon(&midi_device, channel6, note-4, velocity);//root
                //midi_send_noteon(&midi_device, channel7, note+6, velocity);//dominant seventh
                //if (drum_status_kick[1] == 0) {//enable the kick only if it hasnt already been played
              }
                if (drums)
                  midi_send_noteon(&midi_device, channel_drums, 38, 90);//snare drum
                dprintf("midi noteon channel:%d note:%d velocity:%d\n", channel, note, velocity);
            } else { //button depressed
              uint8_t note = tone_status3[tone];
              midi_send_noteoff(&midi_device, channel, note, velocity);
              midi_send_noteoff(&midi_device, channel5, note+3, velocity);//fifth
              midi_send_noteoff(&midi_device, channel6, note-4, velocity);//root
              //midi_send_noteoff(&midi_device, channel7, note+6, velocity);//dominant seventh
              if (drums)
                midi_send_noteoff(&midi_device, channel_drums, 38, 90);//snare drum
              dprintf("midi noteoff channel:%d note:%d velocity:%d\n", channel, note, velocity);
            }
            return false;
        }
//XAM MIDI4//MINORCHORD
        case MIDI4_TONE_MIN ... MIDI4_TONE_MAX: {
            uint8_t velocity = midi_config.velocity;
            uint8_t channel  = midi4_config.channel + midi_alt*10;
            uint8_t tone     = keycode - MIDI4_TONE_MIN;
            uint8_t channel5  = midi5_config.channel + midi_alt*10;
            uint8_t channel6  = midi6_config.channel + midi_alt*10;
            //uint8_t channel7  = midi7_config.channel + midi_alt*10;
            uint8_t channel_drums  = midi10_config.channel;
            if (record->event.pressed) {
              
              if (bass_mute == 0) {
              uint8_t note = midi4_compute_note(keycode);
              tone_status4[tone] = note;
              midi_send_noteon(&midi_device, channel, note, velocity);
              midi_send_noteon(&midi_device, channel5, note+4, velocity);//fifth
              midi_send_noteon(&midi_device, channel6, note-3, velocity);//root
              //midi_send_noteon(&midi_device, channel7, note+7, velocity);//dominant seventh
              }
              if (drums)
                midi_send_noteon(&midi_device, channel_drums, 38, 90);//snare drum
              dprintf("midi noteon channel:%d note:%d velocity:%d\n", channel, note, velocity);
            } else {
              uint8_t note = tone_status4[tone];
              midi_send_noteoff(&midi_device, channel, note, velocity);
              midi_send_noteoff(&midi_device, channel5, note+4, velocity);
              midi_send_noteoff(&midi_device, channel6, note-3, velocity);//root
              //midi_send_noteoff(&midi_device, channel7, note+7, velocity);//dominant seventh
              if (drums)
                midi_send_noteoff(&midi_device, channel_drums, 38, 90);//snare drum
              dprintf("midi noteoff channel:%d note:%d velocity:%d\n", channel, note, velocity);
            }
            return false;
        }
//XAM MIDI5//7thCHORD
        case MIDI5_TONE_MIN ... MIDI5_TONE_MAX: {
            uint8_t velocity = midi_config.velocity;
            uint8_t channel  = midi3_config.channel + midi_alt*10;
            uint8_t tone     = keycode - MIDI5_TONE_MIN;
            uint8_t channel7  = midi7_config.channel + midi_alt*10;
            uint8_t channel6  = midi6_config.channel + midi_alt*10;
            uint8_t channel_drums  = midi10_config.channel;
            if (record->event.pressed) {
              
              if (bass_mute == 0) {
              uint8_t note = midi7_compute_note(keycode);
              tone_status4[tone] = note;
              midi_send_noteon(&midi_device, channel, note, velocity);
              midi_send_noteon(&midi_device, channel7, note+7, velocity);//seventh
              midi_send_noteon(&midi_device, channel6, note-4, velocity);//root
              }
              if (drums)
                midi_send_noteon(&midi_device, channel_drums, 38, 90);//snare drum
              dprintf("midi noteon channel:%d note:%d velocity:%d\n", channel, note, velocity);
            } else {
              uint8_t note = tone_status4[tone];
              midi_send_noteoff(&midi_device, channel, note, velocity);
              midi_send_noteoff(&midi_device, channel7, note+7, velocity);
              midi_send_noteoff(&midi_device, channel6, note-4, velocity);//root
              if (drums)
                midi_send_noteoff(&midi_device, channel_drums, 38, 90);//snare drum
              dprintf("midi noteoff channel:%d note:%d velocity:%d\n", channel, note, velocity);
            }
            return false;
        }
//XAM MIDI repeated notes
        /*case MIDIr_TONE_MIN ... MIDIr_TONE_MAX: {
            uint8_t channel  = midi_config.channel;
            uint8_t tone     = keycode - MIDIr_TONE_MIN;
            uint8_t velocity = midi_config.velocity;
            if (record->event.pressed) {
                uint8_t note = midir_compute_note(keycode);
                midi_send_noteon(&midi_device, channel, note, velocity-1);
                dprintf("midi noteon channel:%d note:%d velocity:%d\n", channel, note, velocity);
                tone_status[1][tone] += 1;
                if (tone_status[0][tone] == MIDI_INVALID_NOTE) {
                    tone_status[0][tone] = note;
                }
				if (registroa == 1) {
					midi_send_noteon(&midi_device, channel, note-12, 99);
					tone_statusreg[1][tone] += 1;
					if (tone_statusreg[0][tone] == MIDI_INVALID_NOTE) {
						tone_statusreg[0][tone] = note;
					}
				}
            } else {//depressed
                uint8_t note = tone_status[0][tone];
                tone_status[1][tone] -= 1;
                if (tone_status[1][tone] == 0) {//disable the note only if its been played only once, else don't disable it
                    midi_send_noteoff(&midi_device, channel, note, velocity);
                    dprintf("midi noteoff channel:%d note:%d velocity:%d\n", channel, note, velocity);
                    tone_status[0][tone] = MIDI_INVALID_NOTE;
                }
				note = tone_statusreg[0][tone];
                tone_statusreg[1][tone] -= 1;
				if (registroa == 1 && tone_statusreg[1][tone] == 0) {
					midi_send_noteoff(&midi_device, channel, note-12, 99);
                    tone_statusreg[0][tone] = MIDI_INVALID_NOTE;
				}
            }
            return false;
        }*/
//
        case MIDI_OCTAVE_MIN ... MIDI_OCTAVE_MAX:
            if (record->event.pressed) {
                midi_config.octave = keycode - MIDI_OCTAVE_MIN;
                midi_send_cc(&midi_device, midi_config.channel, 0x7B, 0);//alloff
                dprintf("midi octave %d\n", midi_config.octave);
            }
            return false;
        case MI_OCTD:
            if (record->event.pressed && midi_config.octave > 0) {//TODO XAM erabaki nola in oktaba jetxi ahal izateko, minimon dao ta. Igual 0 ez iztea minimoa, signed izatea ta minimoa -2 adibidez. Ordun compute ntoe re signed.
                midi_config.octave--;
                midi_send_cc(&midi_device, midi_config.channel, 0x7B, 0);//alloff
                dprintf("midi octave %d\n", midi_config.octave);
            }
            return false;
        case MI_OCTU:
            if (record->event.pressed && midi_config.octave < (MIDI_OCTAVE_MAX - MIDI_OCTAVE_MIN)) {
                midi_config.octave++;
                midi_send_cc(&midi_device, midi_config.channel, 0x7B, 0);//alloff
                dprintf("midi octave %d\n", midi_config.octave);
            }
            return false;
			//XAM
        case MI2_OCTD:
            if (record->event.pressed && midi2_config.octave > 0) {//TODO XAM erabaki nola in oktaba jetxi ahal izateko, minimon dao ta. Igual 0 ez iztea minimoa, signed izatea ta minimoa -2 adibidez. Ordun compute ntoe re signed.
                midi2_config.octave--;
                midi_send_cc(&midi_device, midi_config.channel, 0x7B, 0);//alloff
                dprintf("midi octave %d\n", midi2_config.octave);
            }
            return false;
        case MI2_OCTU:
            if (record->event.pressed && midi2_config.octave < (MIDI_OCTAVE_MAX - MIDI_OCTAVE_MIN)) {
                midi2_config.octave++;
                midi_send_cc(&midi_device, midi_config.channel, 0x7B, 0);//alloff
                dprintf("midi octave %d\n", midi2_config.octave);
            }
            return false;
        case MIDI_TRANSPOSE_MIN ... MIDI_TRANSPOSE_MAX:
            if (record->event.pressed) {
                midi_config.transpose = keycode - MI_TRNS_0;
                midi_send_cc(&midi_device, midi_config.channel, 0x7B, 0);//alloff
                midi_send_cc(&midi_device, midi_config.channel, 80, midi_config.transpose);
                dprintf("midi transpose %d\n", midi_config.transpose);
            }
            return false;
        /* case MI_TRNS_7:
            if (record->event.pressed) {
                midi_config.transpose = 7;
                midi_send_cc(&midi_device, midi_config.channel, 0x7B, 0);//alloff
                midi_send_cc(&midi_device, midi_config.channel, 80, midi_config.transpose);
                dprintf("midi transpose %d\n", midi_config.transpose);
            }
            return f alse;*/
        case MI_TRNSD:
            if (record->event.pressed && midi_config.transpose > (MIDI_TRANSPOSE_MIN-5 - MI_TRNS_0)) {
                midi_config.transpose--;
                midi_send_cc(&midi_device, midi_config.channel, 0x7B, 0);//alloff
                midi_send_cc(&midi_device, midi_config.channel, 80, midi_config.transpose);
                dprintf("midi transpose %d\n", midi_config.transpose);
            }
            return false;
        case MI_TRNSU:
            if (record->event.pressed && midi_config.transpose < (MIDI_TRANSPOSE_MAX+5 - MI_TRNS_0)) {//XAM
                // const bool positive = midi_config.transpose > 0;
                midi_config.transpose++;
                midi_send_cc(&midi_device, midi_config.channel, 0x7B, 0);//alloff
                midi_send_cc(&midi_device, midi_config.channel, 80, midi_config.transpose);
                // if (positive && midi_config.transpose < 0) midi_config.transpose--;
                dprintf("midi transpose %d\n", midi_config.transpose);
            }
            return false;
        case MIDI_VELOCITY_MIN ... MIDI_VELOCITY_MAX:
            if (record->event.pressed) {
                midi_config.velocity = compute_velocity(keycode - MIDI_VELOCITY_MIN);
                dprintf("midi velocity %d\n", midi_config.velocity);
            }
            return false;
        case MI_VELD:
            if (record->event.pressed && midi_config.velocity > 0) {
                if (midi_config.velocity == 127) {
                    midi_config.velocity -= 10;
                } else if (midi_config.velocity > 12) {
                    midi_config.velocity -= 13;
                } else {
                    midi_config.velocity = 0;
                }

                dprintf("midi velocity %d\n", midi_config.velocity);
            }
            return false;
        case MI_VELU:
            if (record->event.pressed && midi_config.velocity < 127) {
                if (midi_config.velocity < 115) {
                    midi_config.velocity += 13;
                } else {
                    midi_config.velocity = 127;
                }
                dprintf("midi velocity %d\n", midi_config.velocity);
            }
            return false;
        case MIDI_CHANNEL_MIN ... MIDI_CHANNEL_MAX:
            if (record->event.pressed) {
                midi_config.channel = keycode - MIDI_CHANNEL_MIN;
                dprintf("midi channel %d\n", midi_config.channel);
            }
            return false;
        case MI_CHD:
            if (record->event.pressed) {
                midi_config.channel--;
                dprintf("midi channel %d\n", midi_config.channel);
            }
            return false;
        case MI_CHU:
            if (record->event.pressed) {
                midi_config.channel++;
                dprintf("midi channel %d\n", midi_config.channel);
            }
            return false;
			//xam
        case MI2_CHD:
            if (record->event.pressed) {
                midi2_config.channel--;
                dprintf("midi channel %d\n", midi_config.channel);
            }
            return false;
        case MI2_CHU:
            if (record->event.pressed) {
                midi2_config.channel++;
                dprintf("midi channel %d\n", midi_config.channel);
            }
            return false;
        case MI_ALLOFF:
            if (record->event.pressed) {
                midi_send_cc(&midi_device, midi_config.channel, 0x7B, 0);
                dprintf("midi all notes off\n");
            }
            return false;
        case MI_SUS:
            midi_send_cc(&midi_device, midi_config.channel, 0x40, record->event.pressed ? 127 : 0);
            dprintf("midi sustain %d\n", record->event.pressed);
            return false;
        case MI_PORT:
            midi_send_cc(&midi_device, midi_config.channel, 0x41, record->event.pressed ? 127 : 0);
            dprintf("midi portamento %d\n", record->event.pressed);
            return false;
        case MI_SOST:
            midi_send_cc(&midi_device, midi_config.channel, 0x42, record->event.pressed ? 127 : 0);
            dprintf("midi sostenuto %d\n", record->event.pressed);
            return false;
        case MI_SOFT:
            midi_send_cc(&midi_device, midi_config.channel, 0x43, record->event.pressed ? 127 : 0);
            dprintf("midi soft %d\n", record->event.pressed);
            return false;
        case MI_LEG:
            midi_send_cc(&midi_device, midi_config.channel, 0x43, record->event.pressed ? 127 : 0);
            dprintf("midi legato %d\n", record->event.pressed);
            return false;
        case MI_MOD:
            midi_modulation_step = record->event.pressed ? 1 : -1;
            return false;
        case MI_MODSD:
            if (record->event.pressed) {
                midi_config.modulation_interval++;
                // prevent overflow
                if (midi_config.modulation_interval == 0) midi_config.modulation_interval--;
                dprintf("midi modulation interval %d\n", midi_config.modulation_interval);
            }
            return false;
        case MI_MODSU:
            if (record->event.pressed && midi_config.modulation_interval > 0) {
                midi_config.modulation_interval--;
                dprintf("midi modulation interval %d\n", midi_config.modulation_interval);
            }
            return false;
        case MI_BENDD:/* 
            if (record->event.pressed) {
                midi_send_pitchbend(&midi_device, midi_config.channel, -0x2000);
                dprintf("midi pitchbend channel:%d amount:%d\n", midi_config.channel, -0x2000);
            } else {
                midi_send_pitchbend(&midi_device, midi_config.channel, 0);
                dprintf("midi pitchbend channel:%d amount:%d\n", midi_config.channel, 0);
            }
            return false; */
            if (record->event.pressed) {
              keep_bending_down = 1;
            } else {
              keep_bending_down = 0;
            }
            return false;
        case MI_BENDU:
            if (record->event.pressed) {
              keep_bending_up = 1;
            } else {
              keep_bending_up = 0;
            }
            return false;
        //Registroa
		case TRIKL_REG_TGL:
            if (record->event.pressed) {
				if (registroa == 0) set_registroa(1);
				else set_registroa(0);
			}
			return false;
		case TRIKL_REG_ON:
            if (record->event.pressed) {
				set_registroa(1);
			}
			return false;
		case TRIKL_REG_OFF:
            if (record->event.pressed) {
				set_registroa(0);
			}
			return false;
			
			//DRUMS
		case TRIKL_DRUM_TGL:
            if (record->event.pressed) {
				if (drums == 0) 
					drums = 1;
				else {
					drums = 0;
					midi_send_noteoff(&midi_device, midi10_config.channel, 36, 90);//bass drum				
					midi_send_noteoff(&midi_device, midi10_config.channel, 38, 90);//snare drum
				}
			}
			return false;
		case TRIKL_DRUM_ON:
            if (record->event.pressed) {
				drums = 1;
			}
			return false;
		case TRIKL_DRUM_OFF:
            if (record->event.pressed) {
				drums = 0;
				midi_send_noteoff(&midi_device, midi10_config.channel, 36, 90);//bass drum				
				midi_send_noteoff(&midi_device, midi10_config.channel, 38, 90);//snare drum
			}
			return false;
      
    //MIDI_ALT
		case TRIKL_ALT_TGL:
        if (record->event.pressed) {
				  if (midi_alt == 0) 
            midi_alt = 1;
				  else 
            midi_alt = 0; 
			   }
			return false;
			
    };

    return true;
}

#    endif  // MIDI_ADVANCED

void enter_drum_mode(bool IN_OUT) {
  if (IN_OUT) {
    drum_mode = 1;
    registroa_bkp = registroa;
    registroa = 0;
    drums_bkp = drums;
    drums = 0;//disable drums on left hand
    //midi_alt_bkp = midi_alt;
    //midi_alt = 0;
    //midi_config.octave              = MI_OCT_0 - MIDI_OCTAVE_MIN;//XAM: MI_OCT_2 - MIDI_OCTAVE_MIN
    //midi_config.transpose           = 0;//0 sol, 3 si bemol //XAM TODO: hau config.h-n jarri
    midi_config.velocity            = 90;//XAM
    //midi_config.velocity            = 127;//XAM
    //midi_config.channel             = 10;
  } else {    
    drum_mode = 0;
    registroa = registroa_bkp;
    drums = drums_bkp;
    //midi_alt = midi_alt_bkp;
    //midi_config.channel = 0;
  }
}

void set_registroa(bool ON_OFF) {
    uint8_t channel  = midi_config.channel;
    if (ON_OFF) {
	    registroa = 1;
	    for (uint8_t i = 0; i < MIDI_TONE_COUNT; i++) {
			//for all pressed notes, activate their reg
		    if (tone_status[1][i] != 0){
			    midi_send_noteon(&midi_device, channel, tone_status[0][i]-12, 100); //with vel=100
			    tone_status[0][i-12]    = tone_status[0][i]-12;//save the actual note
				tone_statusreg[1][i] = tone_status[1][i]; //increase number of times played
				tone_status[1][i-12] = tone_status[1][i];
		    }
		}  
    } else {
	    registroa = 0;
		//for all pressed regs, shut them
	    for (uint8_t i = 0; i < MIDI_TONE_COUNT; i++) {
		    if (tone_statusreg[1][i] != 0){// for regs pressed
			    tone_statusreg[1][i] -= 1;
			    tone_status[1][i-12] -= 1;
				if (tone_status[1][i-12] == 0) {
  			        midi_send_noteoff(&midi_device, channel, tone_status[0][i-12], 100);
				}
				tone_statusreg[1][i] = 0;
		    }
		}  
    }
}

void midi_task(void) {
    midi_device_process(&midi_device);
#    ifdef MIDI_ADVANCED
    if (timer_elapsed(midi_modulation_timer) < midi_config.modulation_interval) return;
    midi_modulation_timer = timer_read();

    if (midi_modulation_step != 0) {
        dprintf("midi modulation %d\n", midi_modulation);
        midi_send_cc(&midi_device, midi_config.channel, 0x1, midi_modulation);

        if (midi_modulation_step < 0 && midi_modulation < -midi_modulation_step) {
            midi_modulation      = 0;
            midi_modulation_step = 0;
            return;
        }

        midi_modulation += midi_modulation_step;

        if (midi_modulation > 127) midi_modulation = 127;
    }
#    endif
}

#endif  // MIDI_ENABLE
