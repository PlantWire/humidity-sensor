ARCHIVE = libpwire-sensor
BUILD_MODE = debug

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O3
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all:	build_lib

build_lib_test:
    ifeq ($(OS),Windows_NT)
        ifeq (exists, IF EXIST build (echo exists))
		md build/
        endif
    else
	mkdir -p build/
    endif
	$(CXX) -Iinclude/ src/pwire-sensor-lib.cpp -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o build/$(ARCHIVE).o
	ar rcs build/$(ARCHIVE).a build/$(ARCHIVE).o

build_lib_prod:
    ifeq ($(OS),Windows_NT)
        ifeq (exists, IF EXIST build (echo exists))
		md build/
		md build/pwire-sensor-lib
        endif
    else
	mkdir -p build/pwire-sensor-lib
    endif	
	cp include/pwire-sensor-lib.h build/pwire-sensor-lib
	cp src/pwire-sensor-lib.cpp build/pwire-sensor-lib
	cp library.properties build/pwire-sensor-lib

clean:
    ifeq ($(OS),Windows_NT)
	del build/$(ARCHIVE).*
    else
	rm build/$(ARCHIVE).*
    endif

