// AES8.cpp :

#include <iostream>
#include "AES8.h"

static byte Sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static byte ISbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

void AES8_SubBytes(byte state[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] = Sbox[state[i]];
    }
}

void AES8_InvSubBytes(byte state[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] = ISbox[state[i]];
    }
}

void AES8_ShiftRows(byte state[16]) {
    byte temp;

    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;

    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    temp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = temp;
}

void AES8_InvShiftRows(byte state[16]) {
    byte temp;

    temp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = temp;

    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    temp = state[15];
    state[15] = state[3];
    state[3] = state[7];
    state[7] = state[11];
    state[11] = temp;

    // 수식을 이용한 ShiftRows/InvShiftRows FIPS-197에서 찾아보기!
}



//GF(2^8) xtime() 함수
inline byte GF_xtime(byte gf) {
    return ((((gf >> 7) & 0x01) == 1) ? (gf << 1) ^ 0x1b : gf << 1);
}

void AES8_InvMixColumns(byte state[16]) {
    byte sum8, tmp0, tmp1, tmp2, tmp3;

    // 원리를 확인하세요!
    sum8 = GF_xtime(GF_xtime(GF_xtime(state[0] ^ state[1] ^ state[2] ^ state[3])));
    tmp0 = sum8 ^ GF_xtime(GF_xtime(state[0]) ^ state[0]) ^ GF_xtime(state[1]) ^ state[1]
        ^ GF_xtime(GF_xtime(state[2])) ^ state[2] ^ state[3];
    tmp1 = sum8 ^ state[0] ^ GF_xtime(GF_xtime(state[1]) ^ state[1])
        ^ GF_xtime(state[2]) ^ state[2] ^ GF_xtime(GF_xtime(state[3])) ^ state[3];
    tmp2 = sum8 ^ GF_xtime(GF_xtime(state[0])) ^ state[0] ^ state[1]
        ^ GF_xtime(GF_xtime(state[2]) ^ state[2]) ^ GF_xtime(state[3]) ^ state[3];
    tmp3 = sum8 ^ GF_xtime(state[0]) ^ state[0] ^ GF_xtime(GF_xtime(state[1])) ^ state[1]
        ^ state[2] ^ GF_xtime(GF_xtime(state[3]) ^ state[3]);
    state[0] = tmp0;
    state[1] = tmp1;
    state[2] = tmp2;
    state[3] = tmp3;

    sum8 = GF_xtime(GF_xtime(GF_xtime(state[4] ^ state[5] ^ state[6] ^ state[7])));

    tmp0 = sum8 ^ GF_xtime(GF_xtime(state[4]) ^ state[4]) ^ GF_xtime(state[5]) ^ state[5]
        ^ GF_xtime(GF_xtime(state[6])) ^ state[6] ^ state[7];
    tmp1 = sum8 ^ state[4] ^ GF_xtime(GF_xtime(state[5]) ^ state[5])
        ^ GF_xtime(state[6]) ^ state[6] ^ GF_xtime(GF_xtime(state[7])) ^ state[7];
    tmp2 = sum8 ^ GF_xtime(GF_xtime(state[4])) ^ state[4] ^ state[5]
        ^ GF_xtime(GF_xtime(state[6]) ^ state[6]) ^ GF_xtime(state[7]) ^ state[7];
    tmp3 = sum8 ^ GF_xtime(state[4]) ^ state[4] ^ GF_xtime(GF_xtime(state[5])) ^ state[5]
        ^ state[6] ^ GF_xtime(GF_xtime(state[7]) ^ state[7]);
    state[4] = tmp0;
    state[5] = tmp1;
    state[6] = tmp2;
    state[7] = tmp3;

    sum8 = GF_xtime(GF_xtime(GF_xtime(state[8] ^ state[9] ^ state[10] ^ state[11])));
    tmp0 = sum8 ^ GF_xtime(GF_xtime(state[8]) ^ state[8]) ^ GF_xtime(state[9]) ^ state[9]
        ^ GF_xtime(GF_xtime(state[10])) ^ state[10] ^ state[11];
    tmp1 = sum8 ^ state[8] ^ GF_xtime(GF_xtime(state[9]) ^ state[9])
        ^ GF_xtime(state[10]) ^ state[10] ^ GF_xtime(GF_xtime(state[11])) ^ state[11];
    tmp2 = sum8 ^ GF_xtime(GF_xtime(state[8])) ^ state[8] ^ state[9]
        ^ GF_xtime(GF_xtime(state[10]) ^ state[10]) ^ GF_xtime(state[11]) ^ state[11];
    tmp3 = sum8 ^ GF_xtime(state[8]) ^ state[8] ^ GF_xtime(GF_xtime(state[9])) ^ state[9]
        ^ state[10] ^ GF_xtime(GF_xtime(state[11]) ^ state[11]);
    state[8] = tmp0;
    state[9] = tmp1;
    state[10] = tmp2;
    state[11] = tmp3;

    sum8 = GF_xtime(GF_xtime(GF_xtime(state[12] ^ state[13] ^ state[14] ^ state[15])));
    tmp0 = sum8 ^ GF_xtime(GF_xtime(state[12]) ^ state[12]) ^ GF_xtime(state[13]) ^ state[13]
        ^ GF_xtime(GF_xtime(state[14])) ^ state[14] ^ state[15];
    tmp1 = sum8 ^ state[12] ^ GF_xtime(GF_xtime(state[13]) ^ state[13])
        ^ GF_xtime(state[14]) ^ state[14] ^ GF_xtime(GF_xtime(state[15])) ^ state[15];
    tmp2 = sum8 ^ GF_xtime(GF_xtime(state[12])) ^ state[12] ^ state[13]
        ^ GF_xtime(GF_xtime(state[14]) ^ state[14]) ^ GF_xtime(state[15]) ^ state[15];
    tmp3 = sum8 ^ GF_xtime(state[12]) ^ state[12] ^ GF_xtime(GF_xtime(state[13])) ^ state[13]
        ^ state[14] ^ GF_xtime(GF_xtime(state[15]) ^ state[15]);
    state[12] = tmp0;
    state[13] = tmp1;
    state[14] = tmp2;
    state[15] = tmp3;
}

void AES8_InvRound(byte state[16], byte rk[16]) {

    AES8_InvShiftRows(state);
    AES8_InvSubBytes(state);
    AES8_AddRoundkey(state, rk);
    AES8_InvMixColumns(state);
}


void AES8_MixColumns(byte state[16]) {

    byte sum, tmp0, tmp1, tmp2, tmp3;

    sum = state[0] ^ state[1] ^ state[2] ^ state[3];
    tmp0 = GF_xtime(state[0] ^ state[1]);
    tmp1 = GF_xtime(state[1] ^ state[2]);
    tmp2 = GF_xtime(state[2] ^ state[3]);
    tmp3 = GF_xtime(state[3] ^ state[0]);

    state[0] ^= tmp0 ^ sum;
    state[1] ^= tmp1 ^ sum;
    state[2] ^= tmp2 ^ sum;
    state[3] ^= tmp3 ^ sum;

    sum = state[4] ^ state[5] ^ state[6] ^ state[7];
    tmp0 = GF_xtime(state[4] ^ state[5]);
    tmp1 = GF_xtime(state[5] ^ state[6]);
    tmp2 = GF_xtime(state[6] ^ state[7]);
    tmp3 = GF_xtime(state[7] ^ state[4]);

    state[4] ^= tmp0 ^ sum;
    state[5] ^= tmp1 ^ sum;
    state[6] ^= tmp2 ^ sum;
    state[7] ^= tmp3 ^ sum;

    sum = state[8] ^ state[9] ^ state[10] ^ state[11];
    tmp0 = GF_xtime(state[8] ^ state[9]);
    tmp1 = GF_xtime(state[9] ^ state[10]);
    tmp2 = GF_xtime(state[10] ^ state[11]);
    tmp3 = GF_xtime(state[11] ^ state[8]);

    state[8] ^= tmp0 ^ sum;
    state[9] ^= tmp1 ^ sum;
    state[10] ^= tmp2 ^ sum;
    state[11] ^= tmp3 ^ sum;

    sum = state[12] ^ state[13] ^ state[14] ^ state[15];
    tmp0 = GF_xtime(state[12] ^ state[13]);
    tmp1 = GF_xtime(state[13] ^ state[14]);
    tmp2 = GF_xtime(state[14] ^ state[15]);
    tmp3 = GF_xtime(state[15] ^ state[12]);

    state[12] ^= tmp0 ^ sum;
    state[13] ^= tmp1 ^ sum;
    state[14] ^= tmp2 ^ sum;
    state[15] ^= tmp3 ^ sum;
}


void AES8_AddRoundkey(byte state[16], byte roundkey[16]) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= roundkey[i];
    }
}

void AES8_Round(byte state[16], byte rk[16]) {

    AES8_SubBytes(state);
    AES8_ShiftRows(state);
    AES8_MixColumns(state);
    AES8_AddRoundkey(state, rk);
}

void AES8_Encrypt(byte PT[16], byte RK[11][16], byte CT[16]) {
    byte state[16];

    for (int j = 0; j < 16; j++) {
        state[j] = PT[j];
    }
    AES8_AddRoundkey(state, RK[0]);
    for (int i = 1; i < 10; i++) {
        AES8_Round(state, RK[i]);
    }

    AES8_SubBytes(state);
    AES8_ShiftRows(state);
    AES8_AddRoundkey(state, RK[10]);

    for (int j = 0; j < 16; j++) {
        CT[j] = state[j];
    }
}

void AES8_Decrypt(byte CT[16], byte RK[11][16], byte PT[16]) {
    byte state[16];

    for (int j = 0; j < 16; j++) {
        state[j] = CT[j];
    }
    AES8_AddRoundkey(state, RK[10]);
    for (int i = 9; i > 0; i--) {
        AES8_InvRound(state, RK[i]);
    }

    AES8_InvShiftRows(state);
    AES8_InvSubBytes(state);
    AES8_AddRoundkey(state, RK[0]);

    for (int j = 0; j < 16; j++) {
        PT[j] = state[j];
    }
}

//=====
// 대칭성을 이용하여 AES8 암호화 과정과 동일한 순서를 만드는 경우
// 라운드키(1R-9R)에 InvMixColumn()을 적용한다. (강의 슬라이드 참고)
void AES8_EqInvKey(byte rk[11][16], byte eqrk[11][16]) {
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 16; j++) {
            eqrk[i][j] = rk[i][j];
        }
    }
    for (int i = 1; i < 10; i++) {
        AES8_InvMixColumns(eqrk[i]);
    }
}

//=====
// 대칭성을 이용한 AES 복호화 (AES8 암호와 동일한 순서)
void AES8_EqDecrypt(byte CT[16], byte EqRK[11][16], byte PT[16]) {
    byte state[16];

    for (int j = 0; j < 16; j++) {
        state[j] = CT[j];
    }
    AES8_AddRoundkey(state, EqRK[10]);

    for (int i = 9; i > 0; i--) {
        //AES8_InvRound(state, RK[i]);
        AES8_InvSubBytes(state);
        AES8_InvShiftRows(state);
        AES8_InvMixColumns(state);
        AES8_AddRoundkey(state, EqRK[i]);
    }

    AES8_InvSubBytes(state);
    AES8_InvShiftRows(state);
    AES8_AddRoundkey(state, EqRK[0]);

    for (int j = 0; j < 16; j++) {
        PT[j] = state[j];
    }
}


void AES8_print_state(byte state[16]) {
    for (int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
    }
    printf("\n");
}