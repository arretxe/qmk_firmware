# project specific files
SRC = ../../../drivers/led/sn32/matrix_sn32f24xx.c
SRC += config_led.c
#ENDPOINTS
RAW_ENABLE = no #yes
CONSOLE_ENABLE = no     # Console for debug
MIDI_ENABLE = yes 
# MCU name
MCU = SN32F248BF

# Build Options
#   comment out to disable the options.
#
BACKLIGHT_ENABLE = no
MAGIC_ENABLE = no
MAGIC_KEYCODE_ENABLE = no
BOOTMAGIC_ENABLE = no #yes # Virtual DIP switch configuration # XAM reskate

MOUSEKEY_ENABLE = no    # Mouse keys
EXTRAKEY_ENABLE = no   # Audio control and System control
COMMAND_ENABLE = no     # Commands for debug and configuration
SLEEP_LED_ENABLE = no   # Breathing sleep LED during USB suspend
NKRO_ENABLE = yes        # USB Nkey Rollover XAM
AUDIO_ENABLE = no
RGBLIGHT_ENABLE = no # matrix da ta
SERIAL_LINK_ENABLE = no
WAIT_FOR_USB = no # XAM proatu aber argik eztien pizten, hala konsumona aseguratzeko ondo igual? ta konprobatu rescatek funtzionatzeula
CUSTOM_MATRIX = yes

KEYBOARD_SHARED_EP = no # yes XAM, jarrita proatu aber aldeik notatzean? no kin WIDI ondo
SHARED_EP_ENABLE = no #gehienen nokin ibiliet ta ondo jun da, nokin WIDI ondo
#keypressed at boot:
# no no KO
# yes no KO
# yes yes KO
# no yes KO


#LEADER_ENABLE #Enable leader key chording
#UNICODE_ENABLE 

# Custom RGB matrix handling
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = SN32F24xB



DEBOUNCE_TYPE = sym_eager_pk
LTO_ENABLE = no #XAM tamainue txikitze omen du baino behin proatu ta brick!