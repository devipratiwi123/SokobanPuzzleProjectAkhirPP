# 📦 Sokoban Puzzle (C++ Terminal Game)

Proyek game teka-teki klasik berbasis konsol (*Text-based User Interface*) yang dibangun menggunakan bahasa **C++** dan library **Ncurses**.

---

## 🎯 Latar Belakang

Game ini dibuat sebagai pemenuhan **Tugas Akhir / Final Project** mata kuliah Pemrograman Dasar. Pemilihan konsep *Sokoban* (mendorong kotak ke titik tujuan di dalam gudang) bertujuan untuk menerapkan beberapa konsep pemrograman inti:

1. **Pemrograman Berorientasi Objek (OOP):** Pemisahan logika peta, karakter, dan alur permainan ke dalam *class* yang terstruktur.
2. **Manipulasi Array 2D / Grid:** Mengelola posisi dinding, pemain, kotak, dan target dalam koordinat matriks.
3. **Manajemen File Eksternal:** Membaca susunan peta dari file teks dan menyimpan rekor penyelesaian tercepat.
4. **Antarmuka Terminal Interaktif:** Pemanfaatan *Ncurses* untuk merender tampilan permainan secara *real-time* tanpa berkedip (*flicker-free*).

---

## 🚀 Fitur Permainan

* **Beragam Tingkat Kesulitan:** Tersedia beberapa pilihan level dengan tata letak rintangan yang semakin menantang.
* **Sistem Dorong Kotak & Validasi:** Pemain hanya dapat mendorong satu kotak pada satu waktu dan tidak bisa menariknya kembali.
* **Penghitung Langkah & Reset Cepat:** Mencatat efisiensi gerakan pemain serta tombol pintas untuk mengulang level jika kotak terjebak di sudut.
* **Penyimpanan Skor Terbaik:** Otomatis mencatat rekor jumlah langkah paling sedikit ke dalam file eksternal.
* **Efek Suara & Musik Latar:** Dukungan audio berformat `.wav` untuk menambah suasana bermain.

---

## ⚙️ Prasyarat & Cara Kompilasi

### Kebutuhan Sistem
* Sistem Operasi **Windows** (mendukung Windows API untuk audio)
* Compiler **MinGW (g++)**
* Library **Ncurses** yang sudah terpasang di lingkungan MSYS2 / MinGW

### Langkah Menjalankan

1. **Buka terminal** dan arahkan ke folder utama proyek ini.
2. **Kompilasi kode sumber** dengan menjalankan perintah berikut:
   ```bash
   g++ main.cpp game.cpp map.cpp player.cpp -lncurses -lwinmm -o sokoban
   ```
3. **Jalankan game** yang sudah dikompilasi:
   ```bash
   ./sokoban
   ```

---

## 🎮 Panduan Kontrol Singkat

* **W / A / S / D** atau **Tombol Panah**: Menggerakkan karakter
* **R**: Mengulang (*restart*) level saat ini
* **Q**: Keluar dari permainan
* **Enter**: Memilih opsi pada menu

---

## 👥 Tim Pengembang

**Universitas Lampung**
* Muhammad Abizar (`2617051043`)
* Devi Pratiwi (`2617051016`)
* Dechasshyfa Adharisma (`2617051029`)
