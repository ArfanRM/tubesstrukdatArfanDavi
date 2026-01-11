#include "tubes.h"
#include <iostream>

using namespace std;

void clearLine() {
    cin.ignore(1000, '\n');
}

void inputLine(const char *label, char *buf, int size) {
    cout << label;
    cin.getline(buf, size);
}

void showMenu() {
    cout << "\n=== JADWAL KULIAH ===\n";
    cout << "1. Lihat Jadwal\n";
    cout << "2. Tambah Jadwal\n";
    cout << "3. Cari Jadwal (Hari + Jam Mulai)\n";
    cout << "4. Hapus Jadwal (Hari + Jam Mulai)\n";
    cout << "0. Keluar\n";
    cout << "Pilih: ";
}

int main() {
    TreeJadwal T;
    initTree(T);

    int pilih = -1;

    while (pilih != 0) {
        showMenu();
        cin >> pilih;
        clearLine();

        if (pilih == 1) {
            cout << "\nDAFTAR JADWAL\n";
            if (T.root == 0) cout << "(Kosong)\n";
            else inorderPrint(T.root);
        }
        else if (pilih == 2) {
            Jadwal x;

            char hariIn[20];
            char jmIn[10];
            char jsIn[10];

            inputLine("Hari (Senin/Selasa/... atau sen/sel/rab/...): ", hariIn, 20);

            int hariIdx = 0;
            if (!parseHari(hariIn, hariIdx, x.hari, MAX_HARI)) {
                cout << "Hari tidak valid.\n";
                continue;
            }
            x.hariIndex = hariIdx;

            inputLine("Jam Mulai (HH:MM): ", jmIn, 10);
            if (!parseJam(jmIn, x.mulaiMenit)) {
                cout << "Format jam mulai salah.\n";
                continue;
            }

            inputLine("Jam Selesai (HH:MM): ", jsIn, 10);
            if (!parseJam(jsIn, x.selesaiMenit)) {
                cout << "Format jam selesai salah.\n";
                continue;
            }

            if (x.selesaiMenit <= x.mulaiMenit) {
                cout << "Jam selesai harus lebih besar dari jam mulai.\n";
                continue;
            }

            inputLine("Kode MK: ", x.kodeMK, 20);
            inputLine("Nama MK: ", x.namaMK, MAX_MK);
            inputLine("Ruangan: ", x.ruang, MAX_RUANG);
            inputLine("Dosen: ", x.dosen, MAX_DOSEN);

            x.key = buatKey(x.hariIndex, x.mulaiMenit);

            char err[120];
            bool ok = insertNode(T, x, err, 120);
            if (ok) cout << "Berhasil tambah jadwal.\n";
            else cout << err << endl;
        }
        else if (pilih == 3) {
            char hariIn[20];
            char jmIn[10];

            inputLine("Hari: ", hariIn, 20);

            int hariIdx = 0;
            char hariNorm[MAX_HARI];
            if (!parseHari(hariIn, hariIdx, hariNorm, MAX_HARI)) {
                cout << "Hari tidak valid.\n";
                continue;
            }

            inputLine("Jam Mulai (HH:MM): ", jmIn, 10);
            int mulai = 0;
            if (!parseJam(jmIn, mulai)) {
                cout << "Format jam salah.\n";
                continue;
            }

            int key = buatKey(hariIdx, mulai);
            Node *p = searchByKey(T.root, key);

            if (p == 0) {
                cout << "Tidak ditemukan.\n";
            } else {
                char jm[6], js[6];
                menitToJam(p->info.mulaiMenit, jm, 6);
                menitToJam(p->info.selesaiMenit, js, 6);

                cout << "Ditemukan\n";
                cout << p->info.hari << " " << jm << "-" << js
                     << " | " << p->info.kodeMK
                     << " | " << p->info.namaMK
                     << " | " << p->info.ruang
                     << " | " << p->info.dosen
                     << endl;
            }
        }
        else if (pilih == 4) {
            char hariIn[20];
            char jmIn[10];

            inputLine("Hari: ", hariIn, 20);

            int hariIdx = 0;
            char hariNorm[MAX_HARI];
            if (!parseHari(hariIn, hariIdx, hariNorm, MAX_HARI)) {
                cout << "Hari tidak valid.\n";
                continue;
            }

            inputLine("Jam Mulai (HH:MM): ", jmIn, 10);
            int mulai = 0;
            if (!parseJam(jmIn, mulai)) {
                cout << "Format jam salah.\n";
                continue;
            }

            int key = buatKey(hariIdx, mulai);

            char err[120];
            bool ok = deleteByKey(T, key, err, 120);
            if (ok) cout << "Berhasil hapus.\n";
            else cout << err << endl;
        }
        else if (pilih == 0) {
            cout << "Keluar...\n";
        }
        else {
            cout << "Pilihan tidak valid.\n";
        }
    }

    return 0;
}
