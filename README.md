Tentu, berikut adalah dokumentasi lengkap untuk proyek ESP32-CAM Anda, menjelaskan cara penggunaan dan penyesuaian yang bisa dilakukan oleh pengguna lain.

***

### **Dokumentasi Proyek: Kamera ESP32 ke Google Drive**

Proyek ini memungkinkan mikrokontroler **ESP32-CAM** untuk mengambil foto secara berkala dan mengunggahnya langsung ke folder spesifik di **Google Drive** Anda. Kode ini telah disempurnakan dengan berbagai fitur untuk memastikan stabilitas dan keandalan, seperti koneksi WiFi yang tangguh, penanganan memori yang adaptif, dan umpan balik visual melalui LED.

---

### **Cara Penggunaan**

Untuk menjalankan proyek ini, ikuti langkah-langkah berikut:

#### **1. Kebutuhan Perangkat Keras**
* **ESP32-CAM**: Modul mikrokontroler dengan kamera. Pastikan Anda memiliki model dengan PSRAM untuk performa terbaik.
* **FTDI Programmer**: Diperlukan untuk mengunggah kode dari komputer ke ESP32-CAM.
* **Kabel Jumper**: Untuk menghubungkan FTDI Programmer ke ESP32-CAM.
* **Sumber Daya 5V**: Sumber daya yang stabil untuk menyalakan ESP32-CAM.

#### **2. Kebutuhan Perangkat Lunak**
* **Visual Studio Code**: Sebagai editor kode utama.
* **Ekstensi PlatformIO IDE**: Terpasang di Visual Studio Code untuk manajemen proyek, *library*, dan proses unggah.

#### **3. Konfigurasi Awal**
1.  **Unduh Proyek**: *Clone* atau unduh *file* proyek ini ke komputer Anda.
2.  **Siapkan Google Apps Script**:
    * Buat sebuah Google Apps Script baru di [script.google.com](https://script.google.com).
    * Gunakan kode yang dirancang untuk menerima data gambar dalam format JSON. Script tersebut akan mendekode Base64 dan menyimpannya sebagai file JPG di Google Drive.
    * *Deploy* script tersebut sebagai **Web App** dengan akses diatur ke **"Anyone"** (Siapa saja).
    * Salin **URL Web App** yang Anda dapatkan setelah proses *deployment*.
3.  **Konfigurasi Kode Utama (`src/main.cpp`)**:
    * Buka proyek di Visual Studio Code.
    * Buka *file* `src/main.cpp`.
    * Masukkan **nama dan kata sandi WiFi** Anda pada variabel `ssid` dan `password`.
    * Tempelkan **URL Web App** dari Google Script ke dalam variabel `scriptURL`.

#### **4. Upload dan Jalankan**
1.  Hubungkan ESP32-CAM ke FTDI Programmer dan komputer.
2.  Pastikan pin `GPIO0` terhubung ke `GND` untuk masuk ke mode *upload*.
3.  Di VS Code, gunakan PlatformIO untuk mengunggah proyek dengan menekan tombol **Upload** (ikon panah ke kanan) di bilah status bawah.
4.  Setelah selesai, lepaskan `GPIO0` dari `GND` dan tekan tombol reset pada ESP32-CAM.
5.  Perangkat akan mulai berjalan, mengambil foto, dan mengirimkannya ke Google Drive. Anda bisa memantau prosesnya melalui **Serial Monitor**.

---

### **Penyesuaian yang Bisa Dilakukan**

Kode ini dirancang agar mudah dimodifikasi sesuai kebutuhan Anda.

* **Mengubah Kredensial WiFi**: Ubah nilai `const char* ssid` dan `const char* password` di `src/main.cpp`.
* **Mengubah Target Google Script**: Ganti nilai `String scriptURL` dengan URL Web App Google Script Anda yang baru.
* **Mengatur Kualitas Gambar**: Kode secara otomatis mengatur resolusi dan kualitas gambar berdasarkan ketersediaan PSRAM (`if(psramFound())`). Anda bisa mengubah nilai `FRAMESIZE_UXGA` atau `FRAMESIZE_SVGA` dan `jpeg_quality` di dalam blok ini untuk penyesuaian manual.
* **Mengubah Interval Pengambilan Gambar**: Di bagian akhir fungsi `loop()`, ubah nilai `delay(10000);` untuk mengatur jeda antar pengambilan gambar (dalam milidetik).
* **Menyesuaikan Umpan Balik LED**: Fungsi `blinkLED(int count)` mengontrol kedipan LED. Anda bisa mengubah jumlah kedipan saat koneksi berhasil (`blinkLED(5);`) atau gagal (`blinkLED(3);`) untuk memberikan sinyal yang berbeda.
* **Mengatur Batas Waktu (Timeout)**:
    * **Timeout Koneksi WiFi**: Di `setup()`, nilai `20000` dalam `if (millis() - startTime > 20000)` adalah batas waktu koneksi WiFi dalam milidetik.
    * **Timeout Respons Server**: Di `loop()`, nilai `http.setTimeout(20000);` adalah batas waktu menunggu respons dari Google Script.

---

### **Struktur Proyek**

* `src/main.cpp`: *File* utama yang berisi semua logika program.
* `lib/`: Direktori untuk *library* pribadi atau spesifik proyek.
* `include/`: Direktori untuk *header file* (`.h`) kustom jika Anda ingin memecah kode menjadi beberapa *file*.
* `test/`: Direktori untuk *unit testing*.
* `platformio.ini`: *File* konfigurasi utama untuk PlatformIO, mendefinisikan *board*, *framework*, dan opsi lainnya.