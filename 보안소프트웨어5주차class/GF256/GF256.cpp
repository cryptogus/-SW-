// GF256.cpp 

#include <iostream>
#include "CGF256.h"

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
    // 0^(-1) = 0으로 간주한다
    CGF256 gf;
    sbox[0] = AES_Affine(0);
    for (int i = 1; i < 256; i++) {
        gf.SetValue(i);
        temp = gf.Inv().GetValue();
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
void GF_test() {
    CGF256 gf1, gf2, gf3;
    gf1.SetValue(0x17);
    gf2.SetValue(0x1b);

    printf("gf1 = ");
    gf1.PrintAsHex();
    printf(" = ");
    gf1.PrintAsPolynomial();

    printf("gf2 = ");
    gf2.PrintAsHex();
    printf(" = ");
    gf2.PrintAsPolynomial();

    printf("\n");

    gf3 = gf1.Add(gf2); //gf3 = gf1 + gf2
    printf("gf3 = ");
    gf3.PrintAsHex();
    printf(" = ");
    gf3.PrintAsPolynomial();

    printf("\n");

    printf("xtime...\n");
    gf1.SetValue(0x07);
    gf2 = gf1.xtime();
    printf("gf1 = ");
    gf1.PrintAsHex();
    printf(" = ");
    gf1.PrintAsPolynomial();
    printf("gf2 = ");
    gf2.PrintAsHex();
    printf(" = ");
    gf2.PrintAsPolynomial();
    printf("\n");
    gf1.SetValue(0x10);
    gf2.SetValue(0x08);
    gf3 = gf1.Mul(gf2);
    
    printf("multiplication...\n");
    printf("gf1 = ");    gf1.PrintAsHex(); printf("\n");
    printf("gf2 = ");    gf2.PrintAsHex(); printf("\n");
    printf("gf3 = ");    gf3.PrintAsHex(); printf("\n");
    
    printf("\n");
    printf("inverse...\n");

    gf2 = gf1.Inv();
    gf3 = gf1.Mul(gf2);

    printf("gf1 = ");    gf1.PrintAsHex(); printf("\n");
    printf("gf2 = ");    gf2.PrintAsHex(); printf("\n");
    printf("gf3 = ");    gf3.PrintAsHex(); printf("\n");

}
int main()
{
    /*byte Sbox[256];
    Get_AES_Sbox(Sbox);
    std::cout << "Sbox = " << std::endl;
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
    }*/
    GF_test();
}