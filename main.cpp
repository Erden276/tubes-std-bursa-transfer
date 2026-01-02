#include "bursa_tim.h"
#include <iostream>

using namespace std;

void menuTim(List &L) {
    char pil = ' ';
    while (pil != '0') {
        cout << "\n--- MENU TIM ---" << endl;
        cout << "1. Tampilkan Semua Tim & Pemain (Detail)" << endl;
        cout << "2. Tampilkan Top 3 Tim" << endl;
        cout << "3. Cari Tim (Search)" << endl;
        cout << "4. Tambah Tim Baru" << endl;
        cout << "5. Hapus Tim (Delete Team)" << endl;
        cout << "0. Kembali" << endl;
        cout << ">> ";

        cin >> pil;

        if (pil == '1') {
            showAllData(L);
        }
        else if (pil == '2') {
            showTop3Teams(L);
        }

        //Search Tim
        else if (pil == '3') {
            string nama;
            cout << "Masukkan Nama Tim: ";
            cin.ignore();
            getline(cin, nama);
            searchTimDetail(L, nama);
        }

        //TAMBAH TIM
        else if (pil == '4') {
            string nama, coach, markas;
            int danaSuntikan;
            cout << "Nama Tim: "; cin.ignore(); getline(cin, nama);

            if (findTim(L, nama) != nullptr) {
                cout << "[GAGAL] Nama Tim sudah terdaftar!" << endl;
            } else {
                cout << "Coach: "; getline(cin, coach);
                cout << "Markas: "; getline(cin, markas);
                cout << "Dana Awal ($): " << MODAL_AWAL_TIM << endl;
                while (true) {
                    cout << "Dana Suntikan Sponsor ($): ";
                    cin >> danaSuntikan;
                    if (cin.fail() || danaSuntikan < 0) {
                        cin.clear(); cin.ignore(1000, '\n');
                        cout << "[ERROR] Masukkan angka positif!" << endl;
                    } else break;
                }
                insertTim(L, alokasiTim(nama, coach, markas, MODAL_AWAL_TIM + danaSuntikan));
                cout << "Tim berhasil dibuat." << endl;
            }
        }

        //DELETE TIM
        else if (pil == '5') {
            string nama;
            cout << "Masukkan Nama Tim yg akan dihapus: "; cin.ignore(); getline(cin, nama);
            cout << "Yakin ingin menghapus " << nama << " dan semua pemainnya? (y/n): ";
            char yakin; cin >> yakin;
            if (yakin == 'y' || yakin == 'Y') {
                deleteTim(L, nama);
            } else {
                cout << "Penghapusan dibatalkan." << endl;
            }
        }
        else if (pil != '0') {
            cout << "[ERROR] Pilihan tidak tersedia." << endl;
        }
    }
}

void menuPemain(List &L) {
    char pil = ' ';
    while (pil != '0') {
        cout << "\n--- MENU PEMAIN ---" << endl;
        cout << "1. Tampilkan Semua Tim & Pemain (Detail)" << endl;
        cout << "2. Top 5 Players (Filter Role)" << endl;
        cout << "3. Cari Pemain (Search)" << endl;
        cout << "4. Tambah Pemain Baru" << endl;
        cout << "5. Update Statistik" << endl;
        cout << "6. Hapus Pemain (Delete Player)" << endl;
        cout << "0. Kembali" << endl;
        cout << ">> ";

        cin >> pil;

        if (pil == '1') {
            showAllData(L);
        }

        //MENAMPILKA TOP PEMAIN BERDASARKAN ROLE / ALL
        else if (pil == '2') {
            char filterInput;
            cout << "Pilih Role:\n0. ALL\n1. Jungler\n2. Roamer\n3. Mid\n4. Gold\n5. Exp\nPilih: ";
            cin >> filterInput;

            if (filterInput == '0') showTop5Players(L, "ALL");
            else if (filterInput == '1') showTop5Players(L, "Jungler");
            else if (filterInput == '2') showTop5Players(L, "Roamer");
            else if (filterInput == '3') showTop5Players(L, "Mid");
            else if (filterInput == '4') showTop5Players(L, "Gold");
            else if (filterInput == '5') showTop5Players(L, "Exp");
            else cout << "[ERROR] Pilihan Role tidak valid!" << endl;
        }

        //SEARCH PEMAIN
        else if (pil == '3') {
            string ign;
            cout << "Masukkan IGN Pemain: "; cin >> ign;
            searchPemainDetail(L, ign);
        }

        //TAMBAH PEMAIN
        else if (pil == '4') {
            string timTujuan, ign, negara, role;
            cout << "Masukkan ke Tim mana: "; cin.ignore(); getline(cin, timTujuan);
            adrTim T = findTim(L, timTujuan);

            if (T == nullptr) {
                cout << "[GAGAL] Tim tidak ditemukan!" << endl;
            } else {
                cout << "IGN: "; cin >> ign;
                if (findPemain(L, ign) != nullptr) {
                    cout << "[GAGAL] IGN sudah terdaftar!" << endl;
                } else {
                    role = pilihRole();

                    // INI TABLE KODE NEGARA, JADI PEMAIN YANG DI TAMBAH ITU DARI NEGARA MANA ASALNYA
                    cout << "\n-----------------------" << endl;
                    cout << "| KODE | KETERANGAN   |" << endl;
                    cout << "|---------------------|" << endl;
                    cout << "|  ID  | Indonesia    |" << endl;
                    cout << "|  PH  | Philippines  |" << endl;
                    cout << "|  MY  | Malaysia     |" << endl;
                    cout << "|  SG  | Singapore    |" << endl;
                    cout << "-----------------------" << endl;


                    // VALIDASI INPUT NEGARA BENAR APA NGGAK
                    while (true) {
                        cout << "Masukkan Kode Negara (ID/PH/MY/SG): ";
                        cin >> negara;
                        if (negara == "ID" || negara == "PH" || negara == "MY" || negara == "SG") {
                            break;
                        } else {
                            cout << "[ERROR] Kode salah! Harap masukkan kode sesuai tabel (Huruf Besar)." << endl;
                        }
                    }

                    insertPemain(T, alokasiPemain(ign, role, negara, 0, 0, 0));
                    cout << "Pemain berhasil direkrut!" << endl;
                }
            }
        }

        //Update Statistik
        else if (pil == '5') {
            string ign;
            int m, r, f;
            cout << "Masukkan IGN Pemain: "; cin >> ign;

            while (true) {
                cout << "Tambah Match MVP (Angka >= 0): "; cin >> m;
                if (cin.fail() || m < 0) { cin.clear(); cin.ignore(1000, '\n'); cout << "[ERROR] Masukkan angka bulat positif!" << endl; }
                else break;
            }
            while (true) {
                cout << "Tambah Regular Season MVP (Angka >= 0): "; cin >> r;
                if (cin.fail() || r < 0) { cin.clear(); cin.ignore(1000, '\n'); cout << "[ERROR] Masukkan angka bulat positif!" << endl; }
                else break;
            }
            while (true) {
                cout << "Tambah Finals MVP (Angka >= 0): "; cin >> f;
                if (cin.fail() || f < 0) { cin.clear(); cin.ignore(1000, '\n'); cout << "[ERROR] Masukkan angka bulat positif!" << endl; }
                else break;
            }
            updateStatsPemain(L, ign, m, r, f);
        }

        //DELETE PEMAIN NYA
        else if (pil == '6') {
            string ign;
            cout << "Masukkan IGN Pemain yg akan dihapus: "; cin >> ign;
            deletePemainGlobal(L, ign);
        }
        else if (pil != '0') {
            cout << "[ERROR] Pilihan tidak tersedia." << endl;
        }
    }
}

void menuTransfer(List &L) {
    char pil = ' ';
    while (pil != '0') {
        cout << "\n--- MENU TRANSFER ---" << endl;
        cout << "1. Tampilkan Semua Tim & Pemain (Detail)" << endl;
        cout << "2. Proses Transfer Pemain" << endl;
        cout << "3. Lihat Riwayat Transfer Pemain" << endl;
        cout << "0. Kembali" << endl;
        cout << ">> ";

        cin >> pil;

        string ign;
        if (pil == '1') {
            showAllData(L);
        }

        // INI MENU TRANSFER PEMAINNYA
        else if (pil == '2') {
            string asal, tujuan;
            cout << "IGN Pemain: "; cin >> ign;
            cout << "Tim Asal: "; cin.ignore(); getline(cin, asal);
            cout << "Tim Tujuan: "; getline(cin, tujuan);
            transferPemain(L, ign, asal, tujuan);
        }

        //RIWAYAT TRASFER PEMAIN YANG DIPILIH
        else if (pil == '3') {
            cout << "IGN Pemain: "; cin >> ign;
            showRiwayatTransfer(L, ign);
        }
        else if (pil != '0') {
            cout << "[ERROR] Pilihan tidak tersedia." << endl;
        }
    }
}

void menuJuara(List &L) {
    char pil = ' ';
    while (pil != '0') {
        cout << "\n--- MENU SIMULASI JUARA ---" << endl;
        cout << "1. Input Juara Season Ini" << endl;
        cout << "0. Kembali" << endl;
        cout << ">> ";

        cin >> pil;

        //SIMULASI JUARA
        if (pil == '1') {
            string tim;
            int rank;

            cout << "Nama Tim Juara: ";
            cin.ignore();
            getline(cin, tim);

            // VALIDASI INPUT PERINGKAT (1-3)
            while (true) {
                cout << "Peringkat (1/2/3): ";
                cin >> rank;

                // Cek error
                if (cin.fail() || rank < 1 || rank > 3) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "[ERROR] Masukkan angka 1, 2, atau 3!" << endl;
                } else {
                    break;
                }
            }
            simulasiJuara(L, tim, rank);
        }
        else if (pil != '0') {
            cout << "[ERROR] Pilihan tidak tersedia." << endl;
        }
    }
}

void menuUtama() {
    cout << "\n=== BURSA TRANSFER MPL ID (FINAL) ===" << endl;
    cout << "1. Menu Tim" << endl;
    cout << "2. Menu Pemain" << endl;
    cout << "3. Menu Transfer" << endl;
    cout << "4. Menu Juara" << endl;
    cout << "0. Exit" << endl;
    cout << ">> ";
}

int main() {
    List L;
    createList(L);
    setupData(L);

    char pil = ' ';
    while (pil != '0') {
        menuUtama();
        cin >> pil;
        switch (pil) {
            case '1': menuTim(L); break;
            case '2': menuPemain(L); break;
            case '3': menuTransfer(L); break;
            case '4': menuJuara(L); break;
            case '0': cout << "Terima kasih sudah menggunakan program kami" << endl; break;
            default: cout << "[ERROR] Pilihan salah." << endl; break;
        }
    }
    return 0;
}
