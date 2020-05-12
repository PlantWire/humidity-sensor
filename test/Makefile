EXECUTABLE = pwire-sensor-test

CFLAGS += -g
COVFLAGS += -fprofile-arcs
COVFLAGS += -ftest-coverage
COVFLAGS += -fPIC

all:	build_test

run_test:
	cd build && ./$(EXECUTABLE)
	cd ../ && gcovr -r . -e "test/build/cute*" -e "test/build/ide_listener.h" -e "test/build/xml_listener.h" -e "spwl/*" -e "src/pwire-sensor-test.cpp" -e "test/build/pwire-sensor-test.cpp"

build_test:
    ifeq ($(OS),Windows_NT)
        ifeq (exists, IF EXIST build (echo exists))
		md build/
        endif
    else
	mkdir -p build/
    endif
	$(CXX) -I cute/ src/$(EXECUTABLE).cpp -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(COVFLAGS) -o build/$(EXECUTABLE).o
	$(CXX) build/$(EXECUTABLE).o $(COVFLAGS) -o build/$(EXECUTABLE) -L ../lib/build/ -lpwire-sensor  -L ../spwl/lib/build -lSPWL -Wall -Wextra

clean:
    ifeq ($(OS),Windows_NT)
	del build/$(EXECUTABLE) build/$(EXECUTABLE).xml build/$(EXECUTABLE).o
    else
	rm build/$(EXECUTABLE) build/$(EXECUTABLE).xml build/$(EXECUTABLE).o
    endif
