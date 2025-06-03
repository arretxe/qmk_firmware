/* Copyright 2020 Adam Honse <calcprogrammer1@gmail.com>
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

#include "config_common.h"

#define TRIKL_IX11_T //IX10 or IX11 or IX11CR or IXLR or IXLL
#define MIDI_TRIKL //komentatu basikoa bada MIDI kapak ez generatzeko
//#define TRIKL_RGB // define to enable RGB, don't define to disable RGB
//#define INIT_AS_KB // define this if you want to start as keyboard
#define DRUMS_POR 0 // power_on value for drums
#define TRANSPOSE_POR 0 // power_on value for transpose (0=G/C) 3=Bb 5=C 7=D
#define TRANS_BUT_NUM 14 //Transpose button LED number, to know which LED to assign color to

#define MIDI_ADVANCED //MIDI_BASIC


#if defined(TRIKL_IX10_T)
 #define PRODUCT IX10
 #elif defined(TRIKL_IX11_T)
  #define PRODUCT IX11
 #elif defined(TRIKL_IX11CR_T)
  #define PRODUCT IX11CR
 #elif defined(TRIKL_IXLR_T)
 #define PRODUCT IXLR
#elif defined(TRIKL_IXLL_T)
 #define PRODUCT IXLL
#else
  #error "unexpected PRODUCT!"
#endif

/* USB Device descriptor parameter */
#define VENDOR_ID       0x0C45
#define PRODUCT_ID      0x5004
#define DEVICE_VER      0x0002

#define MANUFACTURER    SoinuTxiki

#define DESCRIPTION     Triklatua

#define USB_MAX_POWER_CONSUMPTION 100 //XAM 500 ustet dala berez aber 100ekin gutxio perlatzeun
//#define USB_SUSPEND_WAKEUP_DELAY 500 //XAM test2
#define RGB_DISABLE_WHEN_USB_SUSPENDED false // true // turn off effects when suspended


/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 16

#define DIODE_DIRECTION COL2ROW

#define MATRIX_COL_PINS { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, B0 }
#define MATRIX_ROW_PINS { C15, D11, D10, D9, D8, D7 }

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 0 //no debounce
#define USB_POLLING_INTERVAL_MS 1 //1ms
#define QMK_KEYS_PER_SCAN 32
//#define STRICT_LAYER_RELEASE //proatzeko


#define BOOTMAGIC_LITE_ROW 0
#define BOOTMAGIC_LITE_COLUMN 0

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
//#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
//#define LOCKING_RESYNC_ENABLE

#define LED_CAPS_LOCK_PIN   B14
#define LED_PIN_ON_STATE    0 //

#include "config_led.h"

#define FORCE_NKRO 

//#define RAW_IN_CAPACITY 0 // RAW FROM HOST ONLY
//#define RAW_OUT_CAPACITY 2



#define RGB_MATRIX_HUE_STEP 4
#define RGB_MATRIX_SAT_STEP 6
#define RGB_MATRIX_STARTUP_HUE  20//  125//cyan//25//yellow
#define RGB_MATRIX_STARTUP_VAL 128 //brillo medio
//#define RGB_MATRIX_STARTUP_SAT  0//white

//#define RGB_DISABLE_AFTER_TIMEOUT 0 // number of ticks to wait until disabling effects
#define RGB_MATRIX_LED_PROCESS_LIMIT 4 //(DRIVER_LED_TOTAL + 4) / 5 // limits the number of LEDs to process in an animation per task run (increases keyboard responsiveness)
#define RGB_MATRIX_LED_FLUSH_LIMIT 16 //16 limits in milliseconds how frequently an animation will update the LEDs. 16 (16ms) is equivalent to limiting to 60fps (increases keyboard responsiveness)
//#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200 // limits maximum brightness of LEDs to 200 out of 255. If not defined maximum brightness is set to 255


#define RGB_MATRIX_CUSTOM_KB // to enable custom effects too
#if defined TRIKL_RGB 
//#define ENABLE_RGB_MATRIX_SOLID_COLOR
//  #define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CUSTOM_TRIKL_SOLID // Sets the default mode, if none has been set
  #define RGB_MATRIX_FRAMEBUFFER_EFFECTS 
  #define RGB_MATRIX_KEYPRESSES
//#define RGB_MATRIX_KEYRELEASES // reacts to keyreleases (instead of keypresses)
  #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE     // Pulses keys hit to hue & value then fades value out
  #define ENABLE_RGB_MATRIX_SOLID_REACTIVE            // Static single hue pulses keys hit to shifted hue then fades to current hue
//#define ENABLE_RGB_MATRIX_TYPING_HEATMAP	//Enables RGB_MATRIX_TYPING_HEATMAP
  #define RGB_MATRIX_TYPING_HEATMAP_SPREAD 20
  #define RGB_MATRIX_TYPING_HEATMAP_SLIM
//#define ENABLE_RGB_MATRIX_DIGITAL_RAIN

  //#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE       // Hue & value pulse near a single key hit then fades value out
  //#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE  // Hue & value pulse near multiple key hits then fades value out
  #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS      // Hue & value pulse the same column and row of a single key hit then fades value out
  //#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS // Hue & value pulse the same column and row of multiple key hits then fades value out
  #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS      // Hue & value pulse away on the same column and row of a single key hit then fades value out
  //#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS // Hue & value pulse away on the same column and row of multiple key hits then fades value out
  #define ENABLE_RGB_MATRIX_SPLASH              // Full gradient & value pulse away from a single key hit then fades value out
  //#define ENABLE_RGB_MATRIX_MULTISPLASH         // Full gradient & value pulse away from multiple key hits then fades value out
  #define ENABLE_RGB_MATRIX_SOLID_SPLASH        // Hue & value pulse away from a single key hit then fades value out
  #define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH   // Hue & value pulse away from multiple key hits then fades value out*/

  #define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
  #define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON

  #define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
  #define ENABLE_RGB_MATRIX_CYCLE_ALL

  #define ENABLE_RGB_MATRIX_RAINDROPS
  #define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
  #define ENABLE_RGB_MATRIX_BREATHING
  //Custom effects:
  #define ENABLE_TRIKL_HEATMAP
  #define ENABLE_TRIKL_SOLID
#else
  #define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CUSTOM_TRIKL_NONE  // Sets the default mode, if none has been set  
  #define ENABLE_TRIKL_NONE // custom mode with all off
#endif



//#define RGB_MATRIX_CUSTOM_EFFECT_IMPLS//LED_MATRIX?



#ifndef NO_DEBUG
  #define NO_DEBUG
#endif // !NO_DEBUG
#if !defined(NO_PRINT) && !defined(CONSOLE_ENABLE)
  #define NO_PRINT
#endif // !NO_PRINT

//#define NO_ACTION_TAPPING //disable tap dance and other tapping features //XAM DISABLES LAYERS!!!!
#define NO_ACTION_ONESHOT //disable one-shot modifiers*/