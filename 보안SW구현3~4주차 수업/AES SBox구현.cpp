
#include <iostream>

typedef unsigned char byte; //새로운 데이터 타입: byte - GF(2^8)

//GF 다항식 출력
void GF_print(byte gf) {
    int coef;

    for (int i = 7; i >= 0; i--) {
        coef = (gf >> i) & 0x01;
        if (coef == 1) {
            std::cout << " + " << "x^" << i; //출력
        }
    }
    std::cout << std::endl;// 줄 바꿈 함수
}

//GF(2^8) 덧셈
byte GF_add(byte gf1, byte gf2) {
    return gf1 ^ gf2;
}
byte GF_xtime(byte gf) {
    int msb; // x^7의 게수
    byte result;

    msb = (gf >> 7) & 0x01;
    if (msb == 1) {
        result = (gf << 1) ^ 0x1b;
    }
    else {
        result = gf << 1;
    }
    return result;
}
//GF(2^8) 곱셈
byte GF_mul(byte f, byte g) {
    byte h;
    int coef;
    h = 0x00;
    for (int i = 7; i >= 0; i--) {
        coef = (f >> i) & 0x01; //a7,a6,a5,....,a0
        h = GF_xtime(h);
        if (coef == 1) {
            h = GF_add(h, g);
        }
    }
    return h;
}
//GF(2^8) 에서의 역원 (a^256 = 1인걸 이용)ex) a^(-1) = a^(254) = a^(1111 1110) = a^2*a^4*a^8*a^16*...*a^128
// a^(1111 1110) = a^(1000 0000) * a^(0100 0000) * a^(0010 0000) * ... * a^(0000 0010)
byte GF_inv(byte f) { //확장 유클리드 알고리즘 계산을 안하고 a^256 = 1인걸 이용해 곱셈으로 구함!
    byte f_inv = 1; // 역원(결과값)
    byte temp; //중간에 곱한값(a^n)
    temp = f;
    for (int i = 0; i < 7; i++) {
        temp = GF_mul(temp, temp);
        f_inv = GF_mul(f_inv, temp);
    }
    return f_inv;
}

//AES Affine Aw+b (w = x^(-1))
byte AES_Affine(byte w) {
    byte A[8][8] = {
        {1, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 1}
    };
    byte b_vec[8] = { 1,1,0,0,0,1,1,0 };
    byte w_vec[8], y_vec[8], y;

    for (int i = 0; i < 8; i++) {
        w_vec[i] = (w >> i) & 0x01;
    }
    
    for (int i = 0; i < 8; i++) {
        y_vec[i] = b_vec[i];
        for (int j = 0; j < 8; j++) {
            y_vec[i] ^= A[i][j] * w_vec[j];
        }
    }
    y = 0;
    for (int i = 0; i < 8; i++) {
        y ^= y_vec[i] << i;
    }
    return y;
}
// AES S-box 만들기
void Get_AES_Sbox(byte sbox[256]) {
    byte temp;
    // 0^(-1) = 0
    sbox[0] = AES_Affine(0);
    for (int i = 1; i < 256; i++) {
        temp = GF_inv(i);
        sbox[i] = AES_Affine(temp);
    }
}
//AES inv S-box 만들기
void Get_AES_inv_Sbox(byte isbox[256]) {
    byte Sbox[256];
    Get_AES_Sbox(Sbox);
    for (int i = 0; i < 256; i++) {
        isbox[Sbox[i]] = i;
    }

}

int main()
{
    byte Sbox[256];
    Get_AES_Sbox(Sbox);
    std::cout << "Sbox = "<< std::endl;
    for (int i = 0; i < 256; i++) {
        if (i % 16 == 0)
            printf("\n");
        printf("%02x ", Sbox[i]);
    }
    printf("\n");
    byte inv_Sbox[256];
    Get_AES_inv_Sbox(inv_Sbox);
    std::cout << "inv_Sbox = " << std::endl;
    for (int i = 0; i < 256; i++) {
        if (i % 16 == 0)
            printf("\n");
        printf("%02x ", inv_Sbox[i]);
    }
}
