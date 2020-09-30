
flash:
	esptool.py --chip esp32 --port /dev/ttyUSB0 erase_flash
	esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 460800 write_flash -z 0x1000 base-firmware/esp32-20190207-v1.10-54-g43a894fb4.bin

build:
	ampy --port /dev/ttyUSB0 put src/main.py


remove:
	ampy --port /dev/ttyUSB0 rm main.py


all: 
	@echo "======================== Carregando o firmware Micropython V1.10 ========================"
	esptool.py --chip esp32 --port /dev/ttyUSB0 erase_flash
	esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 460800 write_flash -z 0x1000 base-firmware/esp32-20190207-v1.10-54-g43a894fb4.bin
	@sleep 1
	@echo "=========================== Carregando os códigos do DHT11-upy ============================="
	ampy --port /dev/ttyUSB0 put src/main.py

