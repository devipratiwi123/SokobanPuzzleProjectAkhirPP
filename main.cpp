#include <ncurses.h> // Library wajib untuk kontrol keyboard real-time & tampilan UI terminal

// Ukuran peta permainan
const int BARIS = 7;
const int KOLOM = 9;

// Peta arena permainan menggunakan Array 2 Dimensi
// '#' = Dinding, ' ' = Lantai, '$' = Kotak, '.' = Target, '@' = Pemain
char arena[BARIS][KOLOM] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', '@', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', '$', ' ', '.', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '.', '$', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

// Array untuk menyimpan posisi asli titik target (.) agar tidak hilang saat terinjak
char targetAsli[BARIS][KOLOM] = {
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', '.', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', '.', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
};

int pemainX = 1;
int pemainY = 1;
int langkah = 0;

// FUNCTION 1: Menggambar arena ke layar ncurses
void gambarArena() {
    clear(); // Fungsi bawaan ncurses untuk membersihkan layar tanpa kedip

    // mvprintw(baris, kolom, teks) -> Mencetak teks di koordinat layar tertentu
    mvprintw(1, 2, "=== SOKOBAN WAREHOUSE ===");
    mvprintw(2, 2, "Gunakan Tombol PANAH (Arrow Keys) untuk bergerak | Q untuk Keluar");
    mvprintw(3, 2, "Jumlah Langkah: %d", langkah);

    // Perulangan bersarang untuk menggambar array 2D
    for (int i = 0; i < BARIS; i++) {
        for (int j = 0; j < KOLOM; j++) {
            // Menggambar karakter di posisi baris (5 + i) dan kolom (4 + j*2) agar renggang rapi
            mvaddch(5 + i, 4 + (j * 2), arena[i][j]);
        }
    }

    mvprintw(13, 2, "Keterangan: [@] Pemain  [$] Kotak  [.] Target  [O] Kotak Masuk Target");
    refresh(); // Fungsi wajib ncurses untuk menampilkan perubahan ke layar
}

// FUNCTION 2: Mengecek kondisi menang
bool cekMenang() {
    for (int i = 0; i < BARIS; i++) {
        for (int j = 0; j < KOLOM; j++) {
            // Jika masih ada target asli yang belum tertutup kotak ('O')
            if (targetAsli[i][j] == '.' && arena[i][j] != 'O') {
                return false;
            }
        }
    }
    return true;
}

// FUNCTION 3: Mengembalikan simbol lantai atau target saat pemain/kotak bergeser
void pulihkanPetak(int x, int y) {
    if (targetAsli[x][y] == '.') {
        arena[x][y] = '.';
    } else {
        arena[x][y] = ' ';
    }
}

// FUNCTION 4: Logika pergerakan & dorong kotak
void gerakPemain(int arahX, int arahY) {
    int targetX = pemainX + arahX;
    int targetY = pemainY + arahY;
    char petakDepan = arena[targetX][targetY];

    if (petakDepan == '#') {
        return; // Mentok dinding
    } 
    else if (petakDepan == ' ' || petakDepan == '.') {
        pulihkanPetak(pemainX, pemainY);
        pemainX = targetX;
        pemainY = targetY;
        arena[pemainX][pemainY] = '@';
        langkah++;
    } 
    else if (petakDepan == '$' || petakDepan == 'O') {
        int belakangKotakX = targetX + arahX;
        int belakangKotakY = targetY + arahY;
        char petakBelakang = arena[belakangKotakX][belakangKotakY];

        if (petakBelakang == ' ' || petakBelakang == '.') {
            if (targetAsli[belakangKotakX][belakangKotakY] == '.') {
                arena[belakangKotakX][belakangKotakY] = 'O'; // Masuk target
            } else {
                arena[belakangKotakX][belakangKotakY] = '$';
            }

            pulihkanPetak(pemainX, pemainY);
            pemainX = targetX;
            pemainY = targetY;
            arena[pemainX][pemainY] = '@';
            langkah++;
        }
    }
}

// FUNCTION UTAMA
int main() {
    // --- INISIALISASI WAJIB NCURSES ---
    initscr();              // Memulai mode layar ncurses
    cbreak();               // Membaca input keyboard langsung TANPA perlu tekan Enter
    noecho();               // Menyembunyikan huruf yang diketik agar tidak muncul berantakan di layar
    keypad(stdscr, TRUE);   // Mengaktifkan tombol khusus seperti Tombol Panah (Arrow Keys)
    curs_set(0);            // Menyembunyikan kursor kedip-kedip

    int tombol;
    bool berjalan = true;

    while (berjalan) {
        gambarArena();

        if (cekMenang()) {
            mvprintw(15, 2, "SELAMAT! Anda menang dalam %d langkah!", langkah);
            mvprintw(16, 2, "Tekan tombol apa saja untuk keluar...");
            refresh();
            getch(); // Menunggu 1 tekan tombol sebelum menutup game
            break;
        }

        tombol = getch(); // Menangkap input keyboard secara real-time

        switch (tombol) {
            case KEY_UP:    case 'w': case 'W': gerakPemain(-1, 0); break;
            case KEY_DOWN:  case 's': case 'S': gerakPemain(1, 0);  break;
            case KEY_LEFT:  case 'a': case 'A': gerakPemain(0, -1); break;
            case KEY_RIGHT: case 'd': case 'D': gerakPemain(0, 1);  break;
            case 'q':       case 'Q': berjalan = false; break;
        }
    }

    endwin(); // WAJIB: Mengembalikan terminal ke mode normal sebelum program selesai
    return 0;
}