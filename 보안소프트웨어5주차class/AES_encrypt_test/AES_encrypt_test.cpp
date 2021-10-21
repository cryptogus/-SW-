// AES_encrypt_test.cpp : AES 속도측정

#include <iostream>
#include <time.h>

typedef unsigned char byte;

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
//===== AES 암호화 라운드 함수


//암호화 중간값 출력
void AES_print_state(byte state[16], const char* msg = nullptr)
{
    if (msg != nullptr) {
        printf("%s = ", msg);
    }

    for (int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
    }
    printf("\n");
}
//====== roundkey는 문서에 있는거 가져다가 저장해서 썻다는것을 참고 가져오기 귀찮
void AES_AddRoundKey(byte in[], byte key[], byte out[]) {//void AddRoundKey(byte input[16] , byte key[16], byte out[16]) 이렇게 써도 무방
    for (int i = 0; i < 16; i++) {					 //void AddRoundKey(byte *input , byte *key, byte *out) 이렇게 써도 무방 3개다 같은것

        out[i] = in[i] ^ key[i];
    }
}

void AES_SubBytes(byte in[16], byte out[16]) {
    byte temp1, temp2;

    for (int i = 0; i < 16; i++) {
        temp1 = GF_inv(in[i]);
        temp2 = AES_Affine(temp1);
        out[i] = temp2;
    }
}
void AES_ShiftRpws(byte in[16], byte out[16]) {
    out[0] = in[0]; out[4] = in[4]; out[8] = in[8]; out[12] = in[12];
    out[1] = in[5]; out[5] = in[9]; out[9] = in[13]; out[13] = in[1];
    out[2] = in[10]; out[6] = in[14]; out[10] = in[2]; out[14] = in[6];
    out[3] = in[15]; out[7] = in[3]; out[11] = in[7]; out[15] = in[11];
}
struct GF_Matrix  //구조체도 int같은 자료형 느낌이다. reference 전달가능 (포인터?)
{
    int M[4][4];
    int row;
    int col;
};
void AES_MixCol(byte x[4], byte y[4]) {
    byte MixCol[4][4] = { {0x02, 0x03, 0x01, 0x01},
                         {0x01, 0x02, 0x03, 0x01},
                         {0x01, 0x01, 0x02, 0x03},
                         {0x03, 0x01, 0x01, 0x02} };

    GF_Matrix MC;
    MC.row = 4;
    MC.col = 4;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            MC.M[i][j] = MixCol[i][j];
    GF_Matrix X, Y;
    X.col = 1; Y.col = 1;
    X.row = 4; Y.row = 4;
    for (int i = 0; i < 4; i++)
        X.M[i][0] = x[i];
    //Y = GF_Mat_Mul(MC, X); 함수 안짜둠 ㅋ
    for (int i = 0; i < 4; i++) {
        y[i] = Y.M[i][0];
    }
}
void AES_MixColumns(byte in[16], byte out[16]) {
    byte x[4], y[4];
    for (int col = 0; col < 4; col++) {
        for (int i = 0; i < 4; i++) {
            x[i] = in[col * 4 + i];
        }
        AES_MixCol(x, y);
        for (int i = 0; i < 4; i++) {
            out[col * 4 + i] = y[i];
        }
    }
}
void AES_Round_Function(byte round_in[16], byte rkey[16], byte round_out[16]) {
    byte state1[16], state2[16];
    AES_SubBytes(round_in, state1);
    AES_ShiftRpws(state1, state2);
    AES_MixColumns(state2, state1);
    AES_AddRoundKey(state1, rkey, round_out);
}
void AES_Encrypt(byte pt[16], byte rk[11][16], byte ct[16]) {
    byte state1[16], state2[16];

    AES_AddRoundKey(pt, rk[0], state1);

    for (int i = 1; i < 10; i++) {//1R - 9R
        AES_Round_Function(state1, rk[i], state2);
        for (int j = 0; j < 16; j++) {
            state1[j] = state2[j];
        }
    }

    //10R
    AES_SubBytes(state1, state2);
    AES_ShiftRpws(state2, state1);
    AES_AddRoundKey(state1, rk[10], ct);
}

void Run_AES_speed_test() {
	byte pt[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
	byte rk[11][16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
												0xa0, 0xfa, 0xfe, 0x17, 0x88, 0x54, 0x2c, 0xb1, 0x23, 0xa3, 0x39, 0x39, 0x2a, 0x6c, 0x76, 0x5,
												0xf2, 0xc2, 0x95, 0xf2, 0x7a, 0x96, 0xb9, 0x43, 0x59, 0x35, 0x80, 0x7a, 0x73, 0x59, 0xf6, 0x7f,
												0x3d, 0x80, 0x47, 0x7d, 0x47, 0x16, 0xfe, 0x3e, 0x1e, 0x23, 0x7e, 0x44, 0x6d, 0x7a, 0x88, 0x3b,
												0xef, 0x44, 0xa5, 0x41, 0xa8, 0x52, 0x5b, 0x7f, 0xb6, 0x71, 0x25, 0x3b, 0xdb, 0x0b, 0xad, 0x00,
												0xd4, 0xd1, 0xc6, 0xf8, 0x7c, 0x83, 0x9d, 0x87, 0xca, 0xf2, 0xb8, 0xbc, 0x11, 0xf9, 0x15, 0xbc,
												0x6d, 0x88, 0xa3, 0x7a, 0x11, 0xb, 0x3e, 0xfd, 0xdb, 0xf9, 0x86, 0x41, 0xca, 0x0, 0x93, 0xfd,
												0x4e, 0x54, 0xf7, 0xe, 0x5f, 0x5f, 0xc9, 0xf3, 0x84, 0xa6, 0x4f, 0xb2, 0x4e, 0xa6, 0xdc, 0x4f,
												0xea, 0xd2, 0x73, 0x21, 0xb5, 0x8d, 0xba, 0xd2, 0x31, 0x2b, 0xf5, 0x60, 0x7f, 0x8d, 0x29, 0x2f,
												0xac, 0x77, 0x66, 0xf3, 0x19, 0xfa, 0xdc, 0x21, 0x28, 0xd1, 0x29, 0x41, 0x57, 0x5c, 0x0, 0x6e,
												0xd0, 0x14, 0xf9, 0xa8, 0xc9, 0xee, 0x25, 0x89, 0xe1, 0x3f, 0xc, 0xc8, 0xb6, 0x63, 0xc, 0xa6 };
	byte ct[16];

    clock_t start_time, end_time;
    int nitertation = 20000;
    start_time = clock();
    for (int i = 0; i < nitertation; i++) {
        AES_Encrypt(pt, rk, ct);
    }
    end_time = clock();

    double data_bits = 16 * 8. * nitertation; //데이터 처리한 비트수
    double time_sec;
    time_sec = (end_time - start_time) / CLOCKS_PER_SEC;//걸린 시간이 초 단위로 나옴
    printf("Speed = %f bits/sec\n", data_bits / time_sec);
    printf("Speed = %f Mbps\n", data_bits / 1000'000 / time_sec);
    AES_print_state(ct, "ct");

}
int main()
{
	Run_AES_speed_test();
}
