# GameTank Flashing, Overhauled

command line tool for flashing gametank cartridges with arduino-based flashing unit

feature goals:

* auto erase necessary flash sectors before flashing

* checksum sectors and flash only changed sectors by default

* ability to read cartridge and dump to file

* if file loaded has BANKXX extension then infer XX as target sector

* compatibility with EEPROM cartridges

* maybe add a GUI, who knows