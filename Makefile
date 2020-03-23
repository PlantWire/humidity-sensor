run_test:
	cd test/ && make run_test

build_test: build_lib_test
	cd test/ && make build_test

clean_test:
	cd test/ && make clean

build_lib_test:
	cd lib/ && make build_lib_test

build_prod: 
	cd exe/ && make build_prod

upload_prod: 
	cd exe/ && make upload_prod

prepare:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
	bin/arduino-cli core update-index --config-file arduino-cli.yml
	bin/arduino-cli core install esp8266:esp8266 --config-file arduino-cli.yml

lint:
	cpplint --filter=-legal/copyright --recursive --exclude=test/cute --exclude=.metadata .
