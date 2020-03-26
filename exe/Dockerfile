FROM debian:latest

RUN apt update
RUN apt install make curl python3 -y
RUN printf "board_manager:\n  additional_urls:\n    - http://arduino.esp8266.com/stable/package_esp8266com_index.json" > arduino-cli.yml
ARG BINDIR=/bin
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
RUN arduino-cli core update-index --config-file arduino-cli.yml
RUN arduino-cli core install esp8266:esp8266 --config-file arduino-cli.yml
