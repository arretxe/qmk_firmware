Submodule backup notes (2026-11-01)

These patches capture local-only changes that could not be pushed to upstream remotes due to permissions:
- lib/chibios-contrib (commit ddc1aef7)

Changes:
- Implemented usb_lld_connect_bus and usb_lld_disconnect_bus in SN32F24x/USB/hal_usb_lld.h using mskDPPU_EN (bit 29 of SN_USB->CFG) to support hardware USB bus pull-up connect/disconnect (WAIT_FOR_USB).

Apply example:
- cd lib/chibios-contrib && git am ../../submodule-backups/2026-11-01/chibios-contrib-backup.patch
