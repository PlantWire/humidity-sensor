run_test:
	cd test/ && make run_test

build_test: build_lib_test
	cd test/ && make build_test

clean_test:
	cd test/ && make clean

build_lib_test:
	cd lib/ && make build_lib_test

build_prod: build_lib_prod
	cd exe/ && make build_prod

build_lib_prod: build_spwl
	cd lib/ && make build_lib_prod

upload_prod: 
	cd exe/ && make upload_prod

prepare:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
	arduino-cli core update-index --config-file arduino-cli.yml
	arduino-cli core install esp8266:esp8266 --config-file arduino-cli.yml
	git clone https://github.com/xreef/LoRa_E32_Series_Library.git lib/build/LoRa_E32_Series_Library

lint:
	cpplint --filter=-legal/copyright --extensions=c,cc,h,hpp,c++,h++,hh,cu,cpp,hxx,cxx,cuh,ino --recursive --exclude=test/cute --exclude=.metadata .

build_spwl:
	mkdir -p spwl/build/spwl
	cp spwl_library.properties spwl/build/spwl/library.properties
	cp spwl/lib/include/crc16.h spwl/build/spwl
	cp spwl/lib/include/SPWL.h spwl/build/spwl
	cp spwl/lib/src/SPWL.cpp spwl/build/spwl
	sed -i 's/..\/include\/SPWL.h/SPWL.h/' spwl/build/spwl/SPWL.cpp
	sed -i 's/..\/include\/crc16.h/crc16.h/' spwl/build/spwl/SPWL.cpp
