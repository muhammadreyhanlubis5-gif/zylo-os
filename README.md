# ZYLO OS - Mini Robot Firmware

Proyek firmware dan web flasher untuk robot mini ZYLO berbasis ESP32-C3.

## Struktur Folder
```text
zylo-os/
├── firmware/
│   └── zylo_os/
│       └── zylo_os.ino      <-- Kode sumber C++ (Arduino IDE)
├── web_flasher/
│   ├── index.html           <-- UI Web Flasher
│   ├── manifest.json        <-- Konfigurasi ESP Web Tools
│   └── firmware/
│       └── zylo_v1.0.bin    <-- File binari hasil kompilasi yang siap diflash
└── README.md                <-- Panduan ini
```

---

## Panduan Kompilasi & Export Binary (Arduino IDE)

Untuk menghasilkan file `zylo_v1.0.bin` dari kode `.ino`:

1. Buka Arduino IDE dan buka file `firmware/zylo_os/zylo_os.ino`.
2. Pastikan Anda sudah menginstal library berikut melalui **Library Manager** (`Ctrl+Shift+I`):
   - `Adafruit GFX Library`
   - `Adafruit SSD1306`
3. Pilih board **ESP32C3 Dev Module** dari menu **Tools -> Board**.
4. Di menu **Tools**, pastikan pengaturan standar berikut (sesuaikan dengan board spesifik Anda jika berbeda):
   - **USB CDC On Boot**: Enabled (Jika menggunakan koneksi USB langsung ke chip ESP32-C3)
   - **Flash Size**: 4MB
5. Untuk mendapatkan file `.bin`, pilih menu **Sketch -> Export compiled Binary** (`Ctrl+Alt+S`).
6. Arduino IDE akan mengompilasi kode dan menghasilkan file `.bin` di dalam folder `firmware/zylo_os/` yang sama dengan file `.ino`.
7. Cari file `.bin` tersebut (biasanya bernama `zylo_os.ino.bin` atau `zylo_os.ino.merged.bin`). **Direkomendasikan** menggunakan versi `.merged.bin` karena sudah berisi bootloader dan partition table.
8. Ganti nama file tersebut menjadi `zylo_v1.0.bin` dan pindahkan ke folder `web_flasher/firmware/`.

---

## Panduan Deployment Web Flasher ke GitHub Pages

Untuk membuat web flasher bisa diakses publik (agar orang bisa nge-flash dari browser):

1. Buat repository baru di GitHub.
2. Commit dan push isi folder `web_flasher` ke root directory repository Anda.
   *(Pastikan file `index.html`, `manifest.json`, dan folder `firmware` berada di tingkat paling atas/root).*
3. Buka repository Anda di GitHub, pergi ke **Settings -> Pages**.
4. Di bagian **Source**, pilih branch `main` atau `master` dan folder `/(root)`, lalu klik **Save**.
5. Tunggu beberapa saat, GitHub akan memberikan URL publik (contoh: `https://username.github.io/zylo-os/`).
6. Buka URL tersebut menggunakan **Google Chrome** atau **Microsoft Edge** di PC/Laptop untuk mulai mem-flash ZYLO OS ke board ESP32-C3.
