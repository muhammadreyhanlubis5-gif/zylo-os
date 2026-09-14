#!/bin/bash
set -e

echo "1. Installing Arduino CLI..."
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

echo "2. Configuring ESP32 Core..."
./bin/arduino-cli config init
./bin/arduino-cli config set board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
./bin/arduino-cli core update-index
./bin/arduino-cli core install esp32:esp32

echo "3. Installing required libraries..."
./bin/arduino-cli lib install "Adafruit GFX Library" "Adafruit SSD1306"

echo "4. Compiling ZYLO OS Firmware..."
mkdir -p build_output
./bin/arduino-cli compile --fqbn esp32:esp32:esp32c3 firmware/zylo_os/zylo_os.ino --output-dir build_output

echo "5. Preparing Web Flasher assets..."
mkdir -p web_flasher/firmware

# Netlify Ubuntu runners might output differently, check for merged.bin first, fallback to .bin
if [ -f build_output/zylo_os.ino.merged.bin ]; then
    cp build_output/zylo_os.ino.merged.bin web_flasher/firmware/zylo_v1.0.bin
    echo "Using merged.bin"
else
    cp build_output/zylo_os.ino.bin web_flasher/firmware/zylo_v1.0.bin
    echo "Using standard .bin"
fi

echo "Build successfully completed!"
