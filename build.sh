#!/bin/bash
set -e

# Cari arduino-cli
if [ -f "./bin/arduino-cli" ]; then
    CLI="./bin/arduino-cli"
elif command -v arduino-cli &> /dev/null; then
    CLI="arduino-cli"
else
    echo "ERROR: arduino-cli tidak ditemukan. Silakan install arduino-cli."
    exit 1
fi

echo "Menggunakan arduino-cli di: $CLI"

echo "======================================"
echo "MENGKOMPILASI ZYLO OS GEN 1 (CORE)"
echo "======================================"
$CLI compile --fqbn esp32:esp32:esp32c3 --export-binaries firmware/gen1_core/gen1_core.ino
echo "Menyalin binary Gen 1 ke web_flasher..."
cp firmware/gen1_core/build/esp32.esp32.esp32c3/gen1_core.ino.bin web_flasher/zylo_v1.0.bin

echo "======================================"
echo "MENGKOMPILASI ZYLO OS GEN 2 (SMART)"
echo "======================================"
$CLI compile --fqbn esp32:esp32:esp32c3 --export-binaries firmware/gen2_smart/gen2_smart.ino
echo "Menyalin binary Gen 2 ke web_flasher..."
cp firmware/gen2_smart/build/esp32.esp32.esp32c3/gen2_smart.ino.bin web_flasher/zylo_v2.0.bin

echo "======================================"
echo "MENGKOMPILASI ZYLO OS GEN 3 (CELLULAR)"
echo "======================================"
$CLI compile --fqbn esp32:esp32:esp32c3 --export-binaries firmware/gen3_cellular/gen3_cellular.ino
echo "Menyalin binary Gen 3 ke web_flasher..."
cp firmware/gen3_cellular/build/esp32.esp32.esp32c3/gen3_cellular.ino.bin web_flasher/zylo_v3.0.bin

echo "Kompilasi 3 Generasi Selesai!"
