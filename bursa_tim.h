#ifndef BURSA_TIM_H
#define BURSA_TIM_H

#include <iostream>
#include <string>

using namespace std;

const int JUARA_1 = 85000;
const int JUARA_2 = 53000;
const int JUARA_3 = 35000;

const int MODAL_AWAL_TIM = 1000000;

typedef struct ElmTim *adrTim;
typedef struct ElmPemain *adrPemain;

struct InfotypePemain {
    string ign;
    string role;
    string negara;
    int matchMVP;
    int regularSeasonMVP;
    int finalsMVP;
    int hargaPasar;
    string riwayatTransfer;
};

struct ElmPemain {
    InfotypePemain info;
    adrPemain next;
    adrPemain prev;
};

struct InfotypeTim {
    string namaTim;
    string coach;
    string markas;
    int budget;
    int totalTrophy;
};

struct ElmTim {
    InfotypeTim info;
    adrTim next;
    adrTim prev;
    adrPemain firstPemain;
};

struct List {
    adrTim first;
    adrTim last;
};

struct TimVal {
    string nama;
    int val;
    string topP;
    int pVal;
    int wins;
};

void createList(List &L);
adrTim alokasiTim(string nama, string coach, string markas, int budget);
adrPemain alokasiPemain(string ign, string role, string negara, int mMVP, int rMVP, int fMVP);
int hitungHarga(int mMVP, int rMVP, int fMVP);

adrTim findTim(List L, string namaTim);
adrPemain findPemain(List L, string ign);
adrTim findTimDariPemain(List L, string ign);

void insertTim(List &L, adrTim P);
void insertPemain(adrTim P_Tim, adrPemain P_Pemain);

void deletePemainSpesifik(adrTim P_Tim, string ign, adrPemain &P_Hapus);
void deletePemainPermanen(adrTim P_Tim, string ign);
void deletePemainGlobal(List &L, string ign);
void deleteTim(List &L, string namaTim);

void transferPemain(List &L, string ign, string timAsal, string timTujuan);
void simulasiJuara(List L, string namaTim, int peringkat);
void updateStatsPemain(List &L, string ign, int addMatchMVP, int addRegMVP, int addFinalMVP);

void showAllData(List L);
void showTop5Players(List L, string filterRole);
void showTop3Teams(List L);
void showRiwayatTransfer(List L, string ign);

void searchTimDetail(List L, string namaTim);
void searchPemainDetail(List L, string ign);

void setupData(List &L);
string pilihRole();

#endif
