#include <iostream>

typedef unsigned int byte;

//===== week3의 GF의 행렬 연산 필요
struct GF_Matrix  //구조체도 int같은 자료형 느낌이다. reference 전달가능 (포인터?)
{
    int M[4][4];
    int row;
    int col;
};
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
void AES_print_state (byte state[16], const char *msg nullptr)
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
void AES_Round_Function(byte round_in[16], byte rkey[16], byte round_out[16]){
    byte state1[16], state2[16];
    AES_SubBytes(round_in, state1);
    AES_ShiftRpws(state1, state2);
    AES_MixColumns(state2, state1);
    AES_AddRoundKey(state1,rkey , round_out);
}
void AES_Encrypt(byte pt[16], byte rk[11][16], byte ct[16]){
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

int main() {

}