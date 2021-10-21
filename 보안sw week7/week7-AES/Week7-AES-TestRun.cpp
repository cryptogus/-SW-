//---------------------------------
// Week7-AES: AES8, AES32, 키스케줄
//---------------------------------

#include <iostream>
#include "Table32_Gen.h"
#include "AES32.h"
#include "AES8.h"

//=====
// 키스케줄 테스트 (32비트, 8비트)
void AES_Key_schedule_test() {
    //byte key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    //                     0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

    //byte pt[16] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
    //                0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
    byte key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                     0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    u32 rk[11][4];

    AES32_Enc_KeySchedule(key, rk);

    for (int i = 0; i < 11; i++) {
        AES32_print_state(rk[i]);
    }

    byte rk8[11][16];

    AES8_KeySchedule(key, rk8);
    for (int i = 0; i < 11; i++) {
        AES8_print_state(rk8[i]);
    }
}


void AES8_Encrypt_test() {
    byte pt[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    byte key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    byte rk[11][16];
    byte ct[16];
    
    AES8_KeySchedule(key, rk);
    AES8_Encrypt(pt, rk, ct);

    printf("\n== AES8 Encryption Test ==\n");
    printf("PT = ");
    AES8_print_state(pt);
    printf("CT = ");
    AES8_print_state(ct);

    byte eqrk[11][16];
    AES8_EqInvKey(rk, eqrk);
        
    AES8_EqDecrypt(ct, eqrk, pt);
    
    printf("\n== AES8 Decryption Test ==\n");
    printf("CT = ");
    AES8_print_state(ct);
    printf("PT = "); 
    AES8_print_state(pt);
}

void AES32_Encrypt_test() {
    byte pt[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    byte key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    
    u32 rk[11][4];
    byte ct[16];
    
    AES32_Enc_KeySchedule(key, rk);
    AES32_Encrypt(pt, rk, ct);

    printf("\n== AES32 Encryption Test ==\n");
    printf("PT = ");
    AES8_print_state(pt);
    printf("CT = ");
    AES8_print_state(ct);

    u32 eqrk[11][4];
    AES32_Dec_KeySchedule(key, eqrk);

    AES32_EqDecrypt(ct, eqrk, pt);

    printf("\n== AES32 Decryption Test ==\n");
    printf("CT = ");
    AES8_print_state(ct);
    printf("PT = ");
    AES8_print_state(pt);
}


int main()
{
    //AES32_Enc_Table_generation();
    //AES32_Dec_Table_generation();

    //AES_Key_schedule_test();

    AES8_Encrypt_test();

    AES32_Encrypt_test();


}
