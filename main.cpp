#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>

using namespace std;

// Konstanta ukuran maksimal peta dan jumlah level
const int MAX_BARIS = 10;
const int MAX_KOLOM = 12;
const int TOTAL_LEVEL = 5;

// ==========================================
// DATA LEVEL (ARRAY 3 DIMENSI: [Level][Baris][Kolom])
// Keterangan Simbol:
// '#' = Dinding
// '@' = Pemain
// '$' = Kotak
// '.' = Target
// '*' = Kotak di atas Target
// '+' = Pemain di atas Target
// ' ' = Lantai kosong
// ==========================================
char masterLevel[TOTAL_LEVEL][MAX_BARIS][MAX_KOLOM + 1] = {
    // LEVEL 1: Pengenalan
    {
        "############",
        "#          #",
        "#  @ $ .   #",
        "#          #",
        "#          #",
        "#          #",
        "#          #",
        "#          #",
        "#          #",
        "############"
    },
    // LEVEL 2: Dua Kotak
    {
        "############",
        "#   #      #",
        "# @ $  .   #",
        "#   #  .   #",
        "#   $      #",
        "#          #",
        "#          #",
        "#          #",
        "#          #",
        "############"
    },
    // LEVEL 3: Lorong Sempit
    {
        "############",
        "#  #####   #",
        "#  #   #   #",
        "#  # $ #   #",
        "# @  $ . . #",
        "#  #   #   #",
        "#  #####   #",
        "#          #",
        "#          #",
        "############"
    },
    // LEVEL 4: Tiga Kotak
    {
        "############",
        "# .        #",
        "# .  $     #",
        "# .  $  @  #",
        "#    $     #",
        "#          #",
        "#          #",
        "#          #",
        "#          #",
        "############"
    },
    // LEVEL 5: Tantangan Akhir
    {
        "############",
        "#   #  .   #",
        "#   $  .   #",
        "# ### ###  #",
        "#  $ @ $   #",
        "#  # . #   #",
        "#  #####   #",
        "#          #",
        "#          #",
        "############"
    }
};

// Array 2D untuk papan permainan aktif & peta target
char papan[MAX_BARIS][MAX_KOLOM];
char petaTarget[MAX_BARIS][MAX_KOLOM];
int posPemainX, posPemainY;

// ==========================================
// FUNGSI 1: MEMUAT LEVEL KE PAPAN AKTIF
// ==========================================
void muatLevel(int level) {
    for (int i = 0; i < MAX_BARIS; i++) {
        for (int j = 0; j < MAX_KOLOM; j++) {
            char sel = masterLevel[level][i][j];
            papan[i][j] = sel;

            // Simpan posisi target secara terpisah agar tidak hilang saat terinjak
            if (sel == '.' || sel == '*' || sel == '+') {
                petaTarget[i][j] = '.';
            } else {
                petaTarget[i][j] = ' ';
            }

            // Catat posisi awal pemain
            if (sel == '@' || sel == '+') {
                posPemainY = i;
                posPemainX = j;
            }
        }
    }
}

// ==========================================
// FUNGSI 2: MENAMPILKAN PAPAN (NCURSES)
// ==========================================
void gambarPapan(int level, int langkah, string namaPemain) {
    clear();
    mvprintw(1, 2, "=== SOKOBAN PUZZLE - ILKOM UNILA ===");
    mvprintw(2, 2, "Pemain: %s | Level: %d / %d | Langkah: %d", namaPemain.c_str(), level + 1, TOTAL_LEVEL, langkah);
    mvprintw(3, 2, "Kontrol: Panah / WASD = Gerak | R = Reset Level | Q = Keluar");

    for (int i = 0; i < MAX_BARIS; i++) {
        for (int j = 0; j < MAX_KOLOM; j++) {
            // Cetak karakter dengan spasi horizontal agar terlihat proporsional
            mvaddch(5 + i, 4 + (j * 2), papan[i][j]);
        }
    }

    mvprintw(16, 2, "Legenda: [@] Pemain  [$] Kotak  [.] Target  [*] Kotak Masuk Target");
    refresh();
}

// ==========================================
// FUNGSI 3: LOGIKA PERGERAKAN PEMAIN
// ==========================================
bool gerakPemain(int dx, int dy) {
    int targetX = posPemainX + dx;
    int targetY = posPemainY + dy;
    char selTujuan = papan[targetY][targetX];

    // 1. Jika menabrak dinding, tidak bisa bergerak
    if (selTujuan == '#') {
        return false;
    }

    // 2. Jika mendorong kotak ('$' atau '*')
    if (selTujuan == '$' || selTujuan == '*') {
        int diBalikKotakX = targetX + dx;
        int diBalikKotakY = targetY + dy;
        char selDiBalik = papan[diBalikKotakY][diBalikKotakX];

        // Kotak hanya bisa didorong jika di baliknya kosong atau titik target
        if (selDiBalik == ' ' || selDiBalik == '.') {
            // Update posisi kotak yang baru
            if (petaTarget[diBalikKotakY][diBalikKotakX] == '.') {
                papan[diBalikKotakY][diBalikKotakX] = '*';
            } else {
                papan[diBalikKotakY][diBalikKotakX] = '$';
            }
        } else {
            return false; // Kotak tertahan dinding atau kotak lain
        }
    }

    // 3. Kembalikan ubin lama yang ditinggalkan pemain (' ' atau '.')
    papan[posPemainY][posPemainX] = petaTarget[posPemainY][posPemainX];

    // 4. Pindahkan pemain ke posisi baru
    posPemainX = targetX;
    posPemainY = targetY;
    if (petaTarget[posPemainY][posPemainX] == '.') {
        papan[posPemainY][posPemainX] = '+';
    } else {
        papan[posPemainY][posPemainX] = '@';
    }

    return true;
}

// ==========================================
// FUNGSI 4: CEK KONDISI MENANG LEVEL
// ==========================================
bool cekMenang() {
    for (int i = 0; i < MAX_BARIS; i++) {
        for (int j = 0; j < MAX_KOLOM; j++) {
            // Jika masih ada kotak '$' yang belum berada di target '*', belum menang
            if (papan[i][j] == '$') {
                return false;
            }
        }
    }
    return true;
}

// ==========================================
// FUNGSI 5: SIMPAN SKOR KE FILE TEXT (Pertemuan 9)
// ==========================================
void simpanSkor(string nama, int totalLangkah) {
    ofstream fileSkor;
    // Menggunakan ios::app agar data lama tidak tertimpa
    fileSkor.open("highscore.txt", ios::app);
    if (!fileSkor.fail()) {
        fileSkor << nama << " " << totalLangkah << endl;
        fileSkor.close();
    }
}

// ==========================================
// FUNGSI 6: LEADERBOARD (SORTING & SEARCHING - Pertemuan 7)
// ==========================================
void tampilkanLeaderboard() {
    ifstream fileSkor;
    fileSkor.open("highscore.txt");

    string daftarNama[100];
    int daftarLangkah[100];
    int jumlahData = 0;

    if (!fileSkor.fail()) {
        while (fileSkor >> daftarNama[jumlahData] >> daftarLangkah[jumlahData]) {
            jumlahData++;
            if (jumlahData >= 100) break;
        }
        fileSkor.close();
    }

    if (jumlahData == 0) {
        cout << "\nBelum ada data skor yang tersimpan." << endl;
        return;
    }

    // BUBBLE SORT (Ascending: Langkah paling sedikit di peringkat 1)
    for (int i = 0; i < jumlahData - 1; i++) {
        for (int j = 0; j < jumlahData - i - 1; j++) {
            if (daftarLangkah[j] > daftarLangkah[j + 1]) {
                // Tukar langkah
                int tempLangkah = daftarLangkah[j];
                daftarLangkah[j] = daftarLangkah[j + 1];
                daftarLangkah[j + 1] = tempLangkah;

                // Tukar nama agar tetap sinkron
                string tempNama = daftarNama[j];
                daftarNama[j] = daftarNama[j + 1];
                daftarNama[j + 1] = tempNama;
            }
        }
    }

    cout << "\n==========================================" << endl;
    cout << "       LEADERBOARD SOKOBAN (TOP SKOR)     " << endl;
    cout << "==========================================" << endl;
    for (int i = 0; i < jumlahData; i++) {
        cout << i + 1 << ". " << daftarNama[i] << "\t- " << daftarLangkah[i] << " langkah" << endl;
    }

    // SEQUENTIAL SEARCH (Mencari peringkat pemain tertentu)
    char cari;
    cout << "\nIngin mencari peringkat pemain tertentu? (y/n): ";
    cin >> cari;
    if (cari == 'y' || cari == 'Y') {
        string targetNama;
        cout << "Masukkan nama pemain (1 kata): ";
        cin >> targetNama;

        bool ditemukan = false;
        for (int i = 0; i < jumlahData; i++) {
            if (daftarNama[i] == targetNama) {
                cout << "Pemain '" << daftarNama[i] << "' berada di peringkat ke-" << i + 1
                     << " dengan " << daftarLangkah[i] << " langkah!" << endl;
                ditemukan = true;
                break;
            }
        }
        if (!ditemukan) {
            cout << "Nama pemain tidak ditemukan di papan skor." << endl;
        }
    }
}

// ==========================================
// FUNGSI UTAMA (MAIN)
// ==========================================
int main() {
    string namaPemain;
    int pilihanMenu;

    do {
        cout << "\n======================================" << endl;
        cout << "      GAME SOKOBAN PUZZLE (C++)       " << endl;
        cout << "======================================" << endl;
        cout << "1. Mulai Bermain (5 Level)" << endl;
        cout << "2. Lihat Leaderboard & Cari Skor" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilih menu (1-3): ";
        cin >> pilihanMenu;

        if (pilihanMenu == 1) {
            cout << "Masukkan Nama Pemain (tanpa spasi): ";
            cin >> namaPemain;

            // Inisialisasi mode ncurses
            initscr();
            noecho();
            curs_set(0);
            keypad(stdscr, TRUE);

            int levelSekarang = 0;
            int totalLangkahSemuaLevel = 0;
            bool keluarGame = false;

            while (levelSekarang < TOTAL_LEVEL && !keluarGame) {
                muatLevel(levelSekarang);
                int langkahLevel = 0;
                bool levelSelesai = false;

                while (!levelSelesai && !keluarGame) {
                    gambarPapan(levelSekarang, langkahLevel, namaPemain);
                    int tombol = getch();

                    switch (tombol) {
                        case KEY_UP:
                        case 'w':
                        case 'W':
                            if (gerakPemain(0, -1)) langkahLevel++;
                            break;
                        case KEY_DOWN:
                        case 's':
                        case 'S':
                            if (gerakPemain(0, 1)) langkahLevel++;
                            break;
                        case KEY_LEFT:
                        case 'a':
                        case 'A':
                            if (gerakPemain(-1, 0)) langkahLevel++;
                            break;
                        case KEY_RIGHT:
                        case 'd':
                        case 'D':
                            if (gerakPemain(1, 0)) langkahLevel++;
                            break;
                        case 'r':
                        case 'R':
                            muatLevel(levelSekarang);
                            langkahLevel = 0;
                            break;
                        case 'q':
                        case 'Q':
                            keluarGame = true;
                            break;
                    }

                    if (cekMenang()) {
                        gambarPapan(levelSekarang, langkahLevel, namaPemain);
                        mvprintw(18, 2, "SELAMAT! Level %d selesai dalam %d langkah!", levelSekarang + 1, langkahLevel);
                        mvprintw(19, 2, "Tekan tombol apa saja untuk melanjutkan...");
                        refresh();
                        getch();

                        totalLangkahSemuaLevel += langkahLevel;
                        levelSelesai = true;
                        levelSekarang++;
                    }
                }
            }

            // Tutup mode ncurses sebelum kembali ke terminal biasa
            endwin();

            if (!keluarGame && levelSekarang == TOTAL_LEVEL) {
                cout << "\nLUAR BIASA, " << namaPemain << "! Semua level berhasil diselesaikan!" << endl;
                cout << "Total Langkah Keseluruhan: " << totalLangkahSemuaLevel << " langkah." << endl;
                simpanSkor(namaPemain, totalLangkahSemuaLevel);
                cout << "Skor berhasil disimpan ke 'highscore.txt'!" << endl;
            }

        } else if (pilihanMenu == 2) {
            tampilkanLeaderboard();
        }

    } while (pilihanMenu != 3);

    cout << "Terima kasih telah bermain!" << endl;
    return 0;
}
