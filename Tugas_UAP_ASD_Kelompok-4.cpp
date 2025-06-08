#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <unistd.h> 

using namespace std;

enum Arah {UTARA, TIMUR, SELATAN, BARAT};
enum WarnaLampu {MERAH, HIJAU};

struct Mobil {
    string plat;
    string warna;
    Arah arahDatang;

    Mobil(string p, string w, Arah a) : plat(p), warna(w), arahDatang(a) {}
};

struct Pelanggar {
    string plat;
    string warna;
    string waktu;
    string arah;

    Pelanggar(string p, string w, string t, string a)
        : plat(p), warna(w), waktu(t), arah(a) {}
};

string waktuSekarang() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[100];
    sprintf(buffer, "%02d/%02d/%04d - %02d:%02d",
            ltm->tm_mday, ltm->tm_mon+1, 1900+ltm->tm_year,
            ltm->tm_hour, ltm->tm_min);
    return string(buffer);
}

string warnaAcak() {
    string warna[] = {"Merah", "Putih", "Hitam", "Biru", "Silver"};
    return warna[rand() % 5];
}

string platAcak() {
    int angka = rand() % 9000 + 1000;
    char huruf = 'A' + rand() % 26;
    return to_string(angka) + " " + huruf;
}

string arahKeString(Arah a) {
    if (a == UTARA) return "Utara";
    if (a == TIMUR) return "Timur";
    if (a == SELATAN) return "Selatan";
    return "Barat";
}

class LampuLaluLintas {
public:
    WarnaLampu lampu[4];
    int timer;

    LampuLaluLintas() {
        timer = 0;
        for (int i = 0; i < 4; ++i)
            lampu[i] = MERAH;
        lampu[UTARA] = HIJAU;
        lampu[SELATAN] = HIJAU;
    }

    void updateLampu() {
        timer++;
        if (timer % 5 == 0) {
            if (lampu[UTARA] == HIJAU) {
                lampu[UTARA] = MERAH;
                lampu[SELATAN] = MERAH;
                lampu[TIMUR] = HIJAU;
                lampu[BARAT] = HIJAU;
            } else {
                lampu[TIMUR] = MERAH;
                lampu[BARAT] = MERAH;
                lampu[UTARA] = HIJAU;
                lampu[SELATAN] = HIJAU;
            }
        }
    }

    WarnaLampu getLampu(Arah arah) {
        return lampu[arah];
    }

    void tampilkanLampu() {
        cout << "Status Lampu: ";
        cout << "Utara: " << (lampu[UTARA] == HIJAU ? "Hijau" : "Merah") << ", ";
        cout << "Timur: " << (lampu[TIMUR] == HIJAU ? "Hijau" : "Merah") << ", ";
        cout << "Selatan: " << (lampu[SELATAN] == HIJAU ? "Hijau" : "Merah") << ", ";
        cout << "Barat: " << (lampu[BARAT] == HIJAU ? "Hijau" : "Merah") << endl;
    }
};

void tampilkanMenuCRUD(vector<Pelanggar> &dataPelanggar) {
    int pilihan;
    do {
        cout << "\n===== MENU CRUD =====\n";
        cout << "1. Tampilkan Data\n";
        cout << "2. Update Data\n";
        cout << "3. Hapus Data\n";
        cout << "4. Cari Data\n";
        cout << "5. Lanjutkan Pengambilan Data\n";
        cout << "6. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;

        cin.ignore();
        if (pilihan == 1) {
            cout << "\n== Data Pelanggar ==\n";
            for (auto &p : dataPelanggar)
                cout << p.waktu << " - " << p.plat << " (" << p.warna
                     << ") dari arah " << p.arah << endl;
        } else if (pilihan == 2) {
            string cariPlat;
            cout << "Masukkan plat yang ingin di-update: ";
            getline(cin, cariPlat);
            bool ketemu = false;
            for (auto &p : dataPelanggar) {
                if (p.plat == cariPlat) {
                    cout << "Plat ditemukan. Masukkan warna baru: ";
                    getline(cin, p.warna);
                    ketemu = true;
                    break;
                }
            }
            if (!ketemu) cout << "Data tidak ditemukan.\n";
        } else if (pilihan == 3) {
            string cariPlat;
            cout << "Masukkan plat yang ingin dihapus: ";
            getline(cin, cariPlat);
            bool ketemu = false;
            for (int i = 0; i < dataPelanggar.size(); ++i) {
                if (dataPelanggar[i].plat == cariPlat) {
                    dataPelanggar.erase(dataPelanggar.begin() + i);
                    cout << "Data berhasil dihapus.\n";
                    ketemu = true;
                    break;
                }
            }
            if (!ketemu) cout << "Data tidak ditemukan.\n";
        } else if (pilihan == 4) {
            string cariPlat;
            cout << "Masukkan plat yang dicari: ";
            getline(cin, cariPlat);
            bool ketemu = false;
            for (auto &p : dataPelanggar) {
                if (p.plat == cariPlat) {
                    cout << "Ditemukan: " << p.waktu << " - " << p.plat << " (" << p.warna
                         << ") dari arah " << p.arah << endl;
                    ketemu = true;
                    break;
                }
            }
            if (!ketemu) cout << "Data tidak ditemukan.\n";
        } else if (pilihan == 5) {
            return; 
        } else if (pilihan == 6) {
            exit(0);
        } else {
            cout << "Pilihan tidak valid.\n";
        }
    } while (true);
}

void jalankanSimulasi(vector<Pelanggar> &dataPelanggar) {
    LampuLaluLintas lampu;
    vector<Mobil> antrean[4];
    const int menitSimulasi = 10;

    for (int menit = 0; menit < menitSimulasi; menit++) {
        cout << "\n========== Menit ke-" << menit + 1 << " ==========\n";
        lampu.updateLampu();
        lampu.tampilkanLampu();

        for (int i = 0; i < 4; i++) {
            if (rand() % 2 == 0) {
                Mobil m(platAcak(), warnaAcak(), static_cast<Arah>(i));
                antrean[i].push_back(m);
            }
        }

        for (int i = 0; i < 4; i++) {
            if (!antrean[i].empty()) {
                Mobil m = antrean[i].front();
                bool melanggar = false;

                if (lampu.getLampu(m.arahDatang) == MERAH && rand() % 4 == 0)
                    melanggar = true;

                if (lampu.getLampu(m.arahDatang) == HIJAU || melanggar) {
                    cout << "Mobil " << m.plat << " (" << m.warna << ") dari "
                         << arahKeString(m.arahDatang) << " sedang jalan.\n";
                    antrean[i].erase(antrean[i].begin());

                    if (melanggar) {
                        string waktuNow = waktuSekarang();
                        dataPelanggar.push_back(Pelanggar(m.plat, m.warna, waktuNow, arahKeString(m.arahDatang)));
                        cout << ">>> MELANGGAR LAMPU MERAH!\n";
                    }
                } else {
                    cout << "Mobil " << m.plat << " (" << m.warna << ") dari "
                         << arahKeString(m.arahDatang) << " menunggu lampu hijau.\n";
                }
            }
        }

        sleep(2); 
    }
}

int main() {
    srand(time(0));
    vector<Pelanggar> dataPelanggar;

    while (true) {
        jalankanSimulasi(dataPelanggar);
        tampilkanMenuCRUD(dataPelanggar);
    }

    return 0;
}
