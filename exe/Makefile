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
    ifeq ($(OS),Windows_NT)
        ifeq (exists, IF EXIST build (echo exists))
		md build/
        endif
    else
	mkdir -p build/
    endif
	../bin/arduino-cli compile --fqbn esp8266:esp8266:d1_mini src/pwire-sensor.ino -o build/pwire-sensor --config-file ../arduino-cli.yml

upload_prod:
	../bin/arduino-cli upload -p $(PORT) --fqbn esp8266:esp8266:d1_mini -i build/pwire-sensor --config-file ../arduino-cli.yml

clean:
    ifeq ($(OS),Windows_NT)
	del build/$(EXECUTABLE) build/$(EXECUTABLE).o
    else
	rm build/$(EXECUTABLE) build/$(EXECUTABLE).o
    endif
