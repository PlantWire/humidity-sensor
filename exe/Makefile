EXECUTABLE = pwire-sensor
BUILD_MODE = debug

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O3
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all:	build_exe

build_prod:
	mkdir -p build/
	mkdir -p ~/Arduino/libraries/
	cp -r ../lib/build/pwire-sensor-lib ~/Arduino/libraries/
	cp -r ../lib/build/LoRa_E32_Series_Library ~/Arduino/libraries/
	arduino-cli compile --fqbn esp8266:esp8266:d1_mini src/pwire-sensor.ino -o build/pwire-sensor --config-file ../arduino-cli.yml

upload_prod:
	arduino-cli upload -p $(PORT) --fqbn esp8266:esp8266:d1_mini -i build/pwire-sensor --config-file ../arduino-cli.yml

clean:
	rm build/$(EXECUTABLE) build/$(EXECUTABLE).o
