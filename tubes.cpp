#include "tubes.h"

void initTree(TreeJadwal &T) {
    T.root = 0;
}

int strLen(const char *s) {
    int n = 0;
    while (s != 0 && s[n] != '\0') n++;
    return n;
}

void strCopy(char *dst, const char *src, int dstSize) {
    if (dst == 0 || dstSize <= 0) return;
    int i = 0;
    if (src == 0) { dst[0] = '\0'; return; }
    while (src[i] != '\0' && i < dstSize - 1) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

int strCmp(const char *a, const char *b) {
    int i = 0;
    if (a == 0 && b == 0) return 0;
    if (a == 0) return -1;
    if (b == 0) return 1;

    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i++;
    }
    if (a[i] == '\0' && b[i] == '\0') return 0;
    if (a[i] == '\0') return -1;
    return 1;
}

char toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z') return (char)(c - 'A' + 'a');
    return c;
}

bool strEqIgnoreCase(const char *a, const char *b) {
    int i = 0;
    if (a == 0 || b == 0) return false;
    while (a[i] != '\0' && b[i] != '\0') {
        if (toLowerChar(a[i]) != toLowerChar(b[i])) return false;
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

bool parseHari(const char *input, int &hariIndex, char *hariOut, int hariOutSize) {
    if (input == 0) return false;

    if (strEqIgnoreCase(input, "senin") || strEqIgnoreCase(input, "sen")) {
        hariIndex = 1; strCopy(hariOut, "Senin", hariOutSize); return true;
    }
    if (strEqIgnoreCase(input, "selasa") || strEqIgnoreCase(input, "sel")) {
        hariIndex = 2; strCopy(hariOut, "Selasa", hariOutSize); return true;
    }
    if (strEqIgnoreCase(input, "rabu") || strEqIgnoreCase(input, "rab")) {
        hariIndex = 3; strCopy(hariOut, "Rabu", hariOutSize); return true;
    }
    if (strEqIgnoreCase(input, "kamis") || strEqIgnoreCase(input, "kam")) {
        hariIndex = 4; strCopy(hariOut, "Kamis", hariOutSize); return true;
    }
    if (strEqIgnoreCase(input, "jumat") || strEqIgnoreCase(input, "jum")) {
        hariIndex = 5; strCopy(hariOut, "Jumat", hariOutSize); return true;
    }
    if (strEqIgnoreCase(input, "sabtu") || strEqIgnoreCase(input, "sab")) {
        hariIndex = 6; strCopy(hariOut, "Sabtu", hariOutSize); return true;
    }
    if (strEqIgnoreCase(input, "minggu") || strEqIgnoreCase(input, "min")) {
        hariIndex = 7; strCopy(hariOut, "Minggu", hariOutSize); return true;
    }

    return false;
}

bool parseJam(const char *input, int &menit) {
    if (input == 0) return false;
    if (strLen(input) != 5) return false;
    if (input[2] != ':') return false;

    char h1 = input[0], h2 = input[1], m1 = input[3], m2 = input[4];
    if (h1 < '0' || h1 > '9') return false;
    if (h2 < '0' || h2 > '9') return false;
    if (m1 < '0' || m1 > '9') return false;
    if (m2 < '0' || m2 > '9') return false;

    int hh = (h1 - '0') * 10 + (h2 - '0');
    int mm = (m1 - '0') * 10 + (m2 - '0');

    if (hh < 0 || hh > 23) return false;
    if (mm < 0 || mm > 59) return false;

    menit = hh * 60 + mm;
    return true;
}

void menitToJam(int menit, char *out, int outSize) {
    if (out == 0 || outSize < 6) return;
    if (menit < 0) menit = 0;
    int hh = menit / 60;
    int mm = menit % 60;

    out[0] = (char)('0' + (hh / 10));
    out[1] = (char)('0' + (hh % 10));
    out[2] = ':';
    out[3] = (char)('0' + (mm / 10));
    out[4] = (char)('0' + (mm % 10));
    out[5] = '\0';
}

int buatKey(int hariIndex, int mulaiMenit) {
    return hariIndex * 10000 + mulaiMenit;
}

bool overlap(int aMulai, int aSelesai, int bMulai, int bSelesai) {
    if (aMulai < bSelesai && bMulai < aSelesai) return true;
    return false;
}

Node* createNode(const Jadwal &x) {
    Node *p = new Node;
    p->info = x;
    p->left = 0;
    p->right = 0;
    return p;
}

bool cekBentrok(Node *p, const Jadwal &x) {
    if (p == 0) return false;

    if (p->info.hariIndex == x.hariIndex) {
        if (overlap(p->info.mulaiMenit, p->info.selesaiMenit, x.mulaiMenit, x.selesaiMenit)) {
            return true;
        }
    }

    if (cekBentrok(p->left, x)) return true;
    if (cekBentrok(p->right, x)) return true;

    return false;
}

bool insertNode(TreeJadwal &T, const Jadwal &x, char *err, int errSize) {
    if (err != 0 && errSize > 0) err[0] = '\0';

    if (cekBentrok(T.root, x)) {
        strCopy(err, "Gagal: bentrok dengan jadwal lain di hari yang sama.", errSize);
        return false;
    }

    Node *baru = createNode(x);

    if (T.root == 0) {
        T.root = baru;
        return true;
    }

    Node *cur = T.root;
    Node *par = 0;

    while (cur != 0) {
        par = cur;
        if (x.key < cur->info.key) cur = cur->left;
        else if (x.key > cur->info.key) cur = cur->right;
        else {
            delete baru;
            strCopy(err, "Gagal: key duplikat (hari + jam mulai sama).", errSize);
            return false;
        }
    }

    if (x.key < par->info.key) par->left = baru;
    else par->right = baru;

    return true;
}

Node* searchByKey(Node *p, int key) {
    if (p == 0) return 0;
    if (key == p->info.key) return p;
    if (key < p->info.key) return searchByKey(p->left, key);
    return searchByKey(p->right, key);
}

void inorderPrint(Node *p) {
    if (p == 0) return;

    inorderPrint(p->left);

    char jm[6], js[6];
    menitToJam(p->info.mulaiMenit, jm, 6);
    menitToJam(p->info.selesaiMenit, js, 6);

    cout << p->info.hari << " " << jm << "-" << js
         << " | " << p->info.kodeMK
         << " | " << p->info.namaMK
         << " | " << p->info.ruang
         << " | " << p->info.dosen
         << endl;

    inorderPrint(p->right);
}

Node* findMin(Node *p) {
    if (p == 0) return 0;
    Node *cur = p;
    while (cur->left != 0) cur = cur->left;
    return cur;
}

Node* deleteRec(Node *p, int key, bool &ok) {
    if (p == 0) {
        ok = false;
        return 0;
    }

    if (key < p->info.key) {
        p->left = deleteRec(p->left, key, ok);
        return p;
    }
    if (key > p->info.key) {
        p->right = deleteRec(p->right, key, ok);
        return p;
    }

    ok = true;

    if (p->left == 0 && p->right == 0) {
        delete p;
        return 0;
    }

    if (p->left == 0 && p->right != 0) {
        Node *temp = p->right;
        delete p;
        return temp;
    }

    if (p->left != 0 && p->right == 0) {
        Node *temp = p->left;
        delete p;
        return temp;
    }

    Node *succ = findMin(p->right);
    p->info = succ->info;

    bool ok2 = false;
    p->right = deleteRec(p->right, succ->info.key, ok2);

    return p;
}

bool deleteByKey(TreeJadwal &T, int key, char *err, int errSize) {
    if (err != 0 && errSize > 0) err[0] = '\0';

    bool ok = false;
    T.root = deleteRec(T.root, key, ok);
    if (!ok) {
        strCopy(err, "Gagal: data tidak ditemukan.", errSize);
        return false;
    }
    return true;
}
