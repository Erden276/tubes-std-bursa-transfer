#include "bursa_tim.h"

void createList(List &L) {
    L.first = nullptr;
    L.last = nullptr;
}

int hitungHarga(int mMVP, int rMVP, int fMVP) {
    int basePrice = 50000;
    int priceMatch = 5000;
    int priceReg = 50000;
    int priceFinal = 100000;
    return basePrice + (mMVP * priceMatch) + (rMVP * priceReg) + (fMVP * priceFinal);
}

adrTim alokasiTim(string nama, string coach, string markas, int budget) {
    adrTim P = new ElmTim;
    P->info.namaTim = nama;
    P->info.coach = coach;
    P->info.markas = markas;
    P->info.budget = budget;
    P->info.totalTrophy = 0;
    P->next = nullptr;
    P->prev = nullptr;
    P->firstPemain = nullptr;
    return P;
}

adrPemain alokasiPemain(string ign, string role, string negara, int mMVP, int rMVP, int fMVP) {
    adrPemain P = new ElmPemain;
    P->info.ign = ign;
    P->info.role = role;
    P->info.negara = negara;
    P->info.matchMVP = mMVP;
    P->info.regularSeasonMVP = rMVP;
    P->info.finalsMVP = fMVP;
    P->info.hargaPasar = hitungHarga(mMVP, rMVP, fMVP);
    P->info.riwayatTransfer = "- Awal Karir di MPL ID\n";
    P->next = nullptr;
    P->prev = nullptr;
    return P;
}

adrTim findTim(List L, string namaTim) {
    adrTim P = L.first;
    while (P != nullptr) {
        if (P->info.namaTim == namaTim) {
            return P;
        }
        P = P->next;
    }
    return nullptr;
}

adrPemain findPemain(List L, string ign) {
    adrTim T = L.first;
    while (T != nullptr) {
        adrPemain P = T->firstPemain;
        while (P != nullptr) {
            if (P->info.ign == ign) {
                return P;
            }
            P = P->next;
        }
        T = T->next;
    }
    return nullptr;
}

adrTim findTimDariPemain(List L, string ign) {
    adrTim T = L.first;
    while (T != nullptr) {
        adrPemain P = T->firstPemain;
        while (P != nullptr) {
            if (P->info.ign == ign) {
                return T;
            }
            P = P->next;
        }
        T = T->next;
    }
    return nullptr;
}

void insertTim(List &L, adrTim P) {
    if (L.first == nullptr) {
        L.first = P;
        L.last = P;
    } else {
        P->prev = L.last;
        L.last->next = P;
        L.last = P;
    }
}

void insertPemain(adrTim P_Tim, adrPemain P_Pemain) {
    if (P_Tim->firstPemain == nullptr) {
        P_Tim->firstPemain = P_Pemain;
    } else {
        adrPemain Q = P_Tim->firstPemain;
        while (Q->next != nullptr) {
            Q = Q->next;
        }
        Q->next = P_Pemain;
        P_Pemain->prev = Q;
    }
}

void deletePemainSpesifik(adrTim P_Tim, string ign, adrPemain &P_Hapus) {
    adrPemain P = P_Tim->firstPemain;
    while (P != nullptr && P->info.ign != ign) {
        P = P->next;
    }
    if (P == nullptr) {
        P_Hapus = nullptr;
    }
    P_Hapus = P;

    if (P == P_Tim->firstPemain) {
        P_Tim->firstPemain = P->next;
        if (P_Tim->firstPemain != nullptr) {
            P_Tim->firstPemain->prev = nullptr;
        }
    } else {
        P->prev->next = P->next;
        if (P->next != nullptr) {
            P->next->prev = P->prev;
        }
    }
    P_Hapus->next = nullptr;
    P_Hapus->prev = nullptr;
}

void deletePemainPermanen(adrTim P_Tim, string ign) {
    adrPemain P;
    deletePemainSpesifik(P_Tim, ign, P);
    if (P != nullptr) {
        delete P;
    }
}

void deletePemainGlobal(List &L, string ign) {
    adrTim T = findTimDariPemain(L, ign);
    if (T == nullptr) {
        cout << "[GAGAL] Pemain dengan IGN '" << ign << "' tidak ditemukan." << endl;
    } else {
        deletePemainPermanen(T, ign);
        cout << "[SUKSES] Pemain '" << ign << "' berhasil dihapus dari tim " << T->info.namaTim << "." << endl;
    }
}

void deleteTim(List &L, string namaTim) {
    adrTim P = findTim(L, namaTim);
    if (P == nullptr) {
        cout << "[GAGAL] Tim tidak ditemukan." << endl;
    }else{
        while (P->firstPemain != nullptr) {
            string namaP = P->firstPemain->info.ign;
            deletePemainPermanen(P, namaP);
        }

        if (P == L.first && P == L.last) {
            L.first = nullptr;
            L.last = nullptr;
        } else if (P == L.first) {
            L.first = P->next;
            L.first->prev = nullptr;
        } else if (P == L.last) {
            L.last = P->prev;
            L.last->next = nullptr;
        } else {
            P->prev->next = P->next;
            P->next->prev = P->prev;
        }
        delete P;
        cout << "[SUKSES] Tim " << namaTim << " beserta seluruh pemainnya telah dihapus." << endl;
    }
}

void transferPemain(List &L, string ign, string namaTimAsal, string namaTimTujuan) {
    adrTim Asal = findTim(L, namaTimAsal);
    adrTim Tujuan = findTim(L, namaTimTujuan);

    if (!Asal || !Tujuan) {
        cout << "[GAGAL] Tim tidak ditemukan." << endl;
    }else{
        adrPemain P = Asal->firstPemain;
        while (P != nullptr && P->info.ign != ign) {
            P = P->next;
        }
        if (!P) {
            cout << "[GAGAL] Pemain " << ign << " tidak ada di " << namaTimAsal << endl;
        }else{
            if (Tujuan->info.budget >= P->info.hargaPasar) {
                Tujuan->info.budget -= P->info.hargaPasar;
                Asal->info.budget += P->info.hargaPasar;

                adrPemain Pindah;
                deletePemainSpesifik(Asal, ign, Pindah);

                string log = "- Ditransfer dari " + namaTimAsal + " ke " + namaTimTujuan + " (Harga: $" + to_string(Pindah->info.hargaPasar) + ")\n";
                Pindah->info.riwayatTransfer += log;

                insertPemain(Tujuan, Pindah);
                cout << "[SUKSES] Transfer " << ign << " Berhasil!" << endl;
            } else {
                cout << "[GAGAL] Budget " << namaTimTujuan << " Kurang!" << endl;
            }
        }
    }
}

void simulasiJuara(List L, string namaTim, int peringkat) {

    adrTim T = findTim(L, namaTim);
    if (!T) {
        cout << "[GAGAL] Tim '" << namaTim << "' tidak ditemukan." << endl;
    }else {
        int bonus = 0;
        if (peringkat == 1) {
            bonus = JUARA_1;
        } else if (peringkat == 2) {
            bonus = JUARA_2;
        } else {
            bonus = JUARA_3;
        }

        T->info.budget += bonus;

        if (peringkat == 1) {
            T->info.totalTrophy++;
            cout << "\n----------------------------------------" << endl;
            cout << "           CONGRATULATIONS!             " << endl;
            cout << "----------------------------------------" << endl;
            cout << "Tim " << namaTim << " Menjadi Juara 1 MPL ID!" << endl;
            cout << "[INFO] Trophy bertambah +1 (Total: " << T->info.totalTrophy << ")" << endl;
        } else {
            cout << "\n[INFO] Tim " << namaTim << " mendapatkan peringkat " << peringkat << "." << endl;
        }
        cout << "[INFO] Budget bertambah sebesar $" << bonus << endl;
        cout << "----------------------------------------" << endl;
    }
}

void updateStatsPemain(List &L, string ign, int addMatchMVP, int addRegMVP, int addFinalMVP) {
    adrPemain P = findPemain(L, ign);
    if (!P) {
        cout << "Pemain tidak ditemukan." << endl;
    }else{
        P->info.matchMVP += addMatchMVP;
        P->info.regularSeasonMVP += addRegMVP;
        P->info.finalsMVP += addFinalMVP;

        int hargaBaru = hitungHarga(P->info.matchMVP, P->info.regularSeasonMVP, P->info.finalsMVP);
        cout << "\n[UPDATE SUKSES] Statistik " << ign << " diperbarui." << endl;
        cout << "Harga Lama: $" << P->info.hargaPasar << " -> Harga Baru: $" << hargaBaru << endl;
        P->info.hargaPasar = hargaBaru;
    }
}

void showAllData(List L) {
    cout << "\n=== DATA TIM MPL INDONESIA ===" << endl;
    adrTim T = L.first;
    if (T == nullptr) {
        cout << "Data Kosong." << endl;
    }
    while (T != nullptr) {
        cout << "------------------------------------------------" << endl;
        cout << "[TIM] " << T->info.namaTim << endl;
        cout << "Coach   : " << T->info.coach << endl;
        cout << "Markas  : " << T->info.markas << endl;
        cout << "Trophy  : " << T->info.totalTrophy << " Piala" << endl;
        cout << "Budget  : $" << T->info.budget << endl;
        cout << "------------------------------------------------" << endl;

        adrPemain P = T->firstPemain;
        if (P == nullptr) {
            cout << "   (Belum ada pemain)" << endl;
        }
        while (P != nullptr) {
            cout << "   > " << P->info.ign << " (" << P->info.role << ") - $" << P->info.hargaPasar << " | MVP(M/R/F): " << P->info.matchMVP << "/" << P->info.regularSeasonMVP << "/" << P->info.finalsMVP << endl;
            P = P->next;
        }
        cout << endl;
        T = T->next;
    }
}

void showTop5Players(List L, string filterRole) {
    adrPemain arr[100];
    int n = 0;
    adrTim T = L.first;
    while (T != nullptr) {
        adrPemain P = T->firstPemain;
        while (P != nullptr) {
            if (filterRole == "ALL" || P->info.role == filterRole) {
                arr[n] = P;
                n++;
            }
            P = P->next;
        }
        T = T->next;
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j]->info.hargaPasar < arr[j + 1]->info.hargaPasar) {
                adrPemain temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    cout << "\n=== TOP 5 PLAYERS (" << filterRole << ") ===" << endl;
    int limit;
    if (n < 5) {
        limit = n;
    } else {
        limit = 5;
    }
    if (limit == 0) {
        cout << "Tidak ada data pemain." << endl;
    }
    for (int i = 0; i < limit; i++) {
        cout << i + 1 << ". " << arr[i]->info.ign << " ($" << arr[i]->info.hargaPasar << ")" << endl;
    }
}

void showTop3Teams(List L) {
    TimVal arr[20];
    int n = 0;

    adrTim T = L.first;
    while (T != nullptr) {
        int total = T->info.budget;
        string bestP = "-";
        int maxP = 0;
        adrPemain P = T->firstPemain;
        while (P != nullptr) {
            total += P->info.hargaPasar;
            if (P->info.hargaPasar > maxP) {
                maxP = P->info.hargaPasar;
                bestP = P->info.ign;
            }
            P = P->next;
        }
        arr[n].nama = T->info.namaTim;
        arr[n].val = total;
        arr[n].topP = bestP;
        arr[n].pVal = maxP;
        arr[n].wins = T->info.totalTrophy;
        n++;
        T = T->next;
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].val < arr[j + 1].val) {
                TimVal temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    cout << "\n=== TOP 3 TIM (VALUASI & PRESTASI) ===" << endl;
    int limit;
    if (n < 3) {
        limit = n;
    } else {
        limit = 3;
    }

    for (int i = 0; i < limit; i++) {
        cout << i + 1 << ". " << arr[i].nama << " | Wins: " << arr[i].wins << endl;
        cout << "   Valuasi Total: $" << arr[i].val << endl;
        cout << "   Top Player: " << arr[i].topP << " ($" << arr[i].pVal << ")" << endl;
    }
}

void showRiwayatTransfer(List L, string ign) {
    adrPemain P = findPemain(L, ign);
    if (P) {
        cout << "\n=== RIWAYAT TRANSFER: " << ign << " ===" << endl;
        cout << P->info.riwayatTransfer << endl;
    } else {
        cout << "Pemain tidak ditemukan." << endl;
    }
}

void searchTimDetail(List L, string namaTim) {
    adrTim T = findTim(L, namaTim);
    cout << "\n=== HASIL PENCARIAN TIM ===" << endl;
    if (T == nullptr) {
        cout << "Tim '" << namaTim << "' tidak terdaftar." << endl;
    } else {
        cout << "Nama Tim : " << T->info.namaTim << endl;
        cout << "Coach    : " << T->info.coach << endl;
        cout << "Markas   : " << T->info.markas << endl;
        cout << "Trophy   : " << T->info.totalTrophy << endl;
        cout << "Budget   : $" << T->info.budget << endl;
        int count = 0;
        adrPemain P = T->firstPemain;
        while (P != nullptr) {
            count++;
            P = P->next;
        }
        cout << "Jumlah Pemain: " << count << " orang." << endl;
    }
}

void searchPemainDetail(List L, string ign) {
    adrPemain P = findPemain(L, ign);
    cout << "\n=== HASIL PENCARIAN PEMAIN ===" << endl;
    if (P == nullptr) {
        cout << "Pemain dengan IGN '" << ign << "' tidak ditemukan." << endl;
    } else {
        adrTim T = findTimDariPemain(L, ign);
        cout << "IGN       : " << P->info.ign << endl;
        cout << "Role      : " << P->info.role << endl;
        cout << "Negara    : " << P->info.negara << endl;
        if (T != nullptr) {
            cout << "Tim Saat Ini: " << T->info.namaTim << endl;
        } else {
            cout << "Tim Saat Ini: Free Agent" << endl;
        }
        cout << "Harga     : $" << P->info.hargaPasar << endl;
        cout << "Statistik (M/R/F): " << P->info.matchMVP << "/" << P->info.regularSeasonMVP << "/" << P->info.finalsMVP << endl;
    }
}

string pilihRole() {
    char p;
    while (true) {
        cout << "\nPilih Role:\n1. Jungler\n2. Roamer\n3. Mid\n4. Gold\n5. Exp\n>> ";
        cin >> p;

        if (p == '1') {
            return "Jungler";
        } else if (p == '2') {
            return "Roamer";
        } else if (p == '3') {
            return "Mid";
        } else if (p == '4') {
            return "Gold";
        } else if (p == '5') {
            return "Exp";
        } else {
            cout << "[ERROR] Input salah! Harap pilih angka 1-5." << endl;
        }
    }
}

void setupData(List &L) {
    adrTim T1 = alokasiTim("ONIC", "Yeb", "Jakarta", 2000000);
    T1->info.totalTrophy = 6;
    insertTim(L, T1);
    insertPemain(T1, alokasiPemain("Kairi", "Jungler", "PH", 30, 2, 1));
    insertPemain(T1, alokasiPemain("Sanz", "Mid", "ID", 25, 2, 1));
    insertPemain(T1, alokasiPemain("Kiboy", "Roamer", "ID", 20, 0, 1));
    insertPemain(T1, alokasiPemain("CW", "Gold", "ID", 22, 0, 2));
    insertPemain(T1, alokasiPemain("Lutpiii", "Exp", "ID", 10, 0, 0));
    insertPemain(T1, alokasiPemain("Albert", "Jungler", "ID", 15, 2, 1));

    adrTim T2 = alokasiTim("RRQ", "Khezcute", "Jakarta", 2500000);
    T2->info.totalTrophy = 4;
    insertTim(L, T2);
    insertPemain(T2, alokasiPemain("Skylar", "Gold", "ID", 28, 0, 0));
    insertPemain(T2, alokasiPemain("Sutsujin", "Jungler", "ID", 18, 0, 0));
    insertPemain(T2, alokasiPemain("Rinz", "Mid", "ID", 12, 0, 0));
    insertPemain(T2, alokasiPemain("Idok", "Roamer", "ID", 10, 0, 0));
    insertPemain(T2, alokasiPemain("Dyrennn", "Exp", "ID", 15, 0, 0));
    insertPemain(T2, alokasiPemain("Lemon", "Mid", "ID", 5, 3, 0));

    adrTim T3 = alokasiTim("EVOS", "Age", "Jakarta", 1500000);
    T3->info.totalTrophy = 2;
    insertTim(L, T3);
    insertPemain(T3, alokasiPemain("Anavel", "Jungler", "ID", 15, 0, 0));
    insertPemain(T3, alokasiPemain("Dreams", "Roamer", "ID", 12, 0, 0));
    insertPemain(T3, alokasiPemain("Claw Kun", "Mid", "ID", 8, 0, 0));
    insertPemain(T3, alokasiPemain("Branz", "Gold", "ID", 10, 1, 0));
    insertPemain(T3, alokasiPemain("Fluffy", "Exp", "ID", 11, 0, 0));

    adrTim T4 = alokasiTim("TLID", "SaintDeLucaz", "Jakarta", 800000);
    T4->info.totalTrophy = 0;
    insertTim(L, T4);
    insertPemain(T4, alokasiPemain("Yawi", "Roamer", "PH", 18, 0, 0));
    insertPemain(T4, alokasiPemain("Aeronn", "Gold", "ID", 14, 0, 0));
    insertPemain(T4, alokasiPemain("Yehezkiel", "Mid", "ID", 10, 0, 0));
    insertPemain(T4, alokasiPemain("Kabuki", "Gold", "ID", 12, 0, 0));
    insertPemain(T4, alokasiPemain("Aran", "Exp", "ID", 15, 0, 0));

    adrTim T5 = alokasiTim("BTR", "Wurz", "Jakarta", 1000000);
    T5->info.totalTrophy = 0;
    insertTim(L, T5);
    insertPemain(T5, alokasiPemain("Super Kenn", "Jungler", "ID", 20, 0, 0));
    insertPemain(T5, alokasiPemain("Kyy", "Roamer", "ID", 15, 0, 0));
    insertPemain(T5, alokasiPemain("Moreno", "Mid", "ID", 16, 0, 0));
    insertPemain(T5, alokasiPemain("Emann", "Gold", "PH", 22, 0, 0));
    insertPemain(T5, alokasiPemain("Luke", "Exp", "ID", 14, 0, 0));

    adrTim T6 = alokasiTim("GEEK", "Bluffzy", "Jakarta", 600000);
    T6->info.totalTrophy = 0;
    insertTim(L, T6);
    insertPemain(T6, alokasiPemain("Veldora", "Exp", "ID", 8, 0, 0));
    insertPemain(T6, alokasiPemain("Vincent", "Jungler", "ID", 10, 0, 0));
    insertPemain(T6, alokasiPemain("Aboy", "Mid", "ID", 12, 0, 0));
    insertPemain(T6, alokasiPemain("Caderaa", "Gold", "ID", 15, 0, 0));
    insertPemain(T6, alokasiPemain("Baloyskie", "Roamer", "PH", 20, 0, 0));
}
