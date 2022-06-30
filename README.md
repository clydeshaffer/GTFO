# GameTank Flashing, Overhauled

command line tool for flashing gametank cartridges with arduino-based flashing unit

feature goals:

* auto erase necessary flash sectors before flashing

* checksum sectors and flash only changed sectors by default

* ability to read cartridge and dump to file

* if file loaded has BANKXX extension then infer XX as target sector

* compatibility with EEPROM cartridges

* maybe add a GUI, who knows


progress:

* whole chip is erased prior to flashing, need to check the lookup table for sector numbers and address ranges (it's not linear)

* if one or more files are loaded with a .BANKxx extension where XX is a hexadecimal number then they will be flashed to the appropriate banks

* uh it works and is built around a totes cool state machine framework that was definitely a worthwhile time investment yes