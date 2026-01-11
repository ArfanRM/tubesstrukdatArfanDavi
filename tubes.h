#ifndef JADWAL_H
#define JADWAL_H

#include <iostream>

using namespace std;

const int MAX_MK    = 60;
const int MAX_RUANG = 30;
const int MAX_DOSEN = 60;
const int MAX_HARI  = 10;

struct Jadwal {
    int key;
    int hariIndex;
    char hari[MAX_HARI];
    int mulaiMenit;
    int selesaiMenit;
    char kodeMK[20];
    char namaMK[MAX_MK];
    char ruang[MAX_RUANG];
    char dosen[MAX_DOSEN];
};

struct Node {
    Jadwal info;
    Node *left;
    Node *right;
};

struct TreeJadwal {
    Node *root;
};

void initTree(TreeJadwal &T);

int  strLen(const char *s);
void strCopy(char *dst, const char *src, int dstSize);
int  strCmp(const char *a, const char *b);
char toLowerChar(char c);
bool strEqIgnoreCase(const char *a, const char *b);

bool parseHari(const char *input, int &hariIndex, char *hariOut, int hariOutSize);
bool parseJam(const char *input, int &menit);
void menitToJam(int menit, char *out, int outSize);
int  buatKey(int hariIndex, int mulaiMenit);

bool overlap(int aMulai, int aSelesai, int bMulai, int bSelesai);

Node* createNode(const Jadwal &x);

bool cekBentrok(Node *p, const Jadwal &x);
bool insertNode(TreeJadwal &T, const Jadwal &x, char *err, int errSize);

Node* searchByKey(Node *p, int key);

void inorderPrint(Node *p);

Node* findMin(Node *p);

bool deleteByKey(TreeJadwal &T, int key, char *err, int errSize);
Node* deleteRec(Node *p, int key, bool &ok);

#endif
