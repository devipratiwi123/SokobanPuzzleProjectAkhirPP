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
