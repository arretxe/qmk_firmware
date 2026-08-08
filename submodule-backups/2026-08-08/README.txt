Submodule backup notes (2026-08-08)

These patches capture local-only changes that could not be pushed to upstream remotes due to permissions:
- lib/chibios (commit e9257f4c7)
- lib/chibios-contrib (commit 6bf210ab)

Apply example:
- cd lib/chibios && git am ../../submodule-backups/2026-08-08/chibios-backup.patch
- cd lib/chibios-contrib && git am ../../submodule-backups/2026-08-08/chibios-contrib-backup.patch
