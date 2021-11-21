// SHA256_test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
using namespace std;
typedef unsigned char byte;
typedef unsigned int u32;

#define ROTR(x, n) (((x)>>(n)) | ((x)<<(32-(n))))

static u32 K[64] = {
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

struct MBlock {
    byte data[64];
};

//바이트열(msg) --> 메시지블록  msg_block[]={M1, M2, .... ,Mn} (heap)
int Msg2Block(byte msg[], int len, MBlock* ptr_msg_block[] ) {
    int num_blocks;         //입력 메시지에 포함되는 블록 수 
    int remaining_bytes;    //블록을 만들고 남은 바이트 (0,1,...,63)
    // 예: 70바이트 --> num_blocks = 1, remaining_bytes = 6;
    int new_blocks;         //새로 만드는 블록의 개수
    num_blocks = len / 64;
    remaining_bytes = len % 64;
    new_blocks = (remaining_bytes == 0) ? num_blocks : num_blocks + 1;
    MBlock* msg_block;
    msg_block = (MBlock *) malloc(new_blocks * sizeof(MBlock));  // msg_block[]

    for (int b_idx = 0; b_idx < num_blocks; b_idx++) {
        for (int i = 0; i < 64; i++) {
            msg_block[b_idx].data[i] = msg[b_idx * 64 + i];
        }
    }
    if (new_blocks > num_blocks) {
        for (int i = 0; i < 64; i++) {
            if (num_blocks * 64 + i < len) {
                msg_block[num_blocks].data[i] = msg[num_blocks * 64 + i];
            }
            else {
                msg_block[num_blocks].data[i] = 0x00;
            }
        }
    }
    *ptr_msg_block = msg_block;
    return new_blocks;
}

int SHA256_BM_Padding(byte msg[], unsigned long long int len, MBlock* ptrBM[]) {
    int msg_blocks, remaining_bytes, new_blocks;
    byte len64[8];
    MBlock last_block;
    MBlock* new_BM;
    unsigned long long int bit_length;

    msg_blocks = len / 64;
    remaining_bytes = len % 64;
    //기본 패딩 길이 = 9바이트 = (0x80) + (64비트 길이정보)
    // (1) [------] [------] [------] [------] [--****]
    // (2) [------] [------] [------] [------] [-----*] [******] 
    // (remaining_bytes + 9) > 64 ----> (2)
    new_blocks = (remaining_bytes + 9 > 64) ? msg_blocks + 2 : msg_blocks + 1;
    new_BM = (MBlock*)malloc(new_blocks * sizeof(MBlock));

    for (int i = 0; i < msg_blocks; i++) {
        for (int j = 0; j < 64; j++) {
            new_BM[i].data[j] = msg[i * 64 + j];
        }
    }

    // 마지막 블록 { ...... (64비트 길이정보)}
    bit_length = len * 8;
    for (int i = 0; i < 8; i++) {
        len64[i] = (bit_length >> (56 - 8 * i)) & 0xff;
    }
    for (int j = 0; j < 64 - 8; j++) {
        last_block.data[j] = 0x00;
    }
    for (int j = 0; j < 8; j++) {
        last_block.data[56 + j] = len64[j];
    }

    if (new_blocks == msg_blocks + 1) { // (1)
        for (int j = 0; j < remaining_bytes; j++) {
            last_block.data[j] = msg[msg_blocks * 64 + j];
        }
        last_block.data[remaining_bytes] = 0x80; // 1000 0000
    }
    else { // (2) remaining_bytes : 0,1,...,63
        for (int j = 0; j < remaining_bytes; j++) {
            new_BM[msg_blocks].data[j] = msg[msg_blocks * 64 + j];
        }
        new_BM[msg_blocks].data[remaining_bytes] = 0x80;
        for (int j = remaining_bytes + 1; j < 64; j++) {
            new_BM[msg_blocks].data[j] = 0x00;
        }
    }
    for (int j = 0; j < 64; j++) {
        new_BM[new_blocks - 1].data[j] = last_block.data[j];
    }
    *ptrBM = new_BM;
    return new_blocks;
}

void print_byte_msg(byte msg[], int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", msg[i]);
        if ((i % 16) == 15) printf(" ");
        if ((i % 64) == 63) printf("\n");
    }
    printf("\n");
}

void print_MBlock(MBlock MB[], int num_blocks) {
    for (int i = 0; i < num_blocks; i++) {
        for (int j = 0; j < 64; j++) {
            printf("%02x", MB[i].data[j]);
            if ((j % 16) == 15) printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

void SHA256_init(u32 H[8]) {
    H[0] = 0x6a09e667;
    H[1] = 0xbb67ae85;
    H[2] = 0x3c6ef372;
    H[3] = 0xa54ff53a;
    H[4] = 0x510e527f;
    H[5] = 0x9b05688c;
    H[6] = 0x1f83d9ab;
    H[7] = 0x5be0cd19;
}

u32 Ch(u32 x, u32 y, u32 z) {
    return (x & y) ^ (~x & z);
}

u32 Maj(u32 x, u32 y, u32 z) {
    return (x & y) ^ (y & z) ^ (z & x);
}

u32 SIGMA0(u32 x) {
    return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
}

u32 SIGMA1(u32 x) {
    return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
}

u32 sigma0(u32 x) {
    return ROTR(x, 7) ^ ROTR(x, 18) ^ (x>>3);
}

u32 sigma1(u32 x) {
    return ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10);
}

void MsgSchedule(u32 M[16], u32 W[64]) {
    for (int t = 0; t < 16; t++) {
        W[t] = M[t];
    }
    for (int t = 16; t < 64; t++) {
        W[t] = sigma1(W[t - 2]) + W[t - 7] + sigma0(W[t - 15]) + W[t - 16];
    }
}

void MBlock2U32(MBlock MB, u32 M[16]) {
    byte b_array[4];

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            b_array[j] = MB.data[4 * i + j];
        }
        M[i] = b_array[0] << 24 | b_array[1] << 16 | b_array[2] << 8 | b_array[3];
    }
}

void SHA256_update_single_block(u32 H[8], MBlock MB) {
    u32 W[64], M[16];
    u32 a, b, c, d, e, f, g, h;
    u32 T1, T2;

    MBlock2U32(MB, M);
    MsgSchedule(M, W);

    a = H[0]; b = H[1]; c = H[2]; d = H[3];
    e = H[4]; f = H[5]; g = H[6]; h = H[7];

    for (int t = 0; t < 64; t++) {
        T1 = h + SIGMA1(e) + Ch(e, f, g) + K[t] + W[t];
        T2 = SIGMA0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;

        u32 state[8];
        state[0] = a; state[1] = b; state[2] = c; state[3] = d;
        state[4] = e; state[5] = f; state[6] = g; state[7] = h;
        printf("t = %d: ", t);
        for (int j = 0; j < 8; j++) {
            printf("%08x ", state[j]);
        }
        printf("\n");
    }

    H[0] += a; H[1] += b; H[2] += c; H[3] += d;
    H[4] += e; H[5] += f; H[6] += g; H[7] += h;
}

void SHA256_update(u32 H[8], MBlock MB[], int num_blocks) {
    for (int i = 0; i < num_blocks; i++) {
        SHA256_update_single_block(H, MB[i]);
    }
}

void SHA256_finalize(u32 H[8], byte hash_value[32]) {
    for (int i = 0; i < 8; i++) {
        hash_value[i * 4] = H[i] >> 24;
        hash_value[i * 4 + 1] = (H[i] >> 16) & 0xff;
        hash_value[i * 4 + 2] = (H[i] >> 8) & 0xff;
        hash_value[i * 4 + 3] = H[i] & 0xff;
    }
}

void SHA256(byte byte_msg[], int byte_len, byte hash_value[32]) {
    int num_blocks;
    MBlock* MB; //MB[]

    print_byte_msg(byte_msg, byte_len);
    num_blocks = SHA256_BM_Padding(byte_msg, byte_len, &MB);
    print_MBlock(MB, num_blocks);

    u32 H[8];
    SHA256_init(H);
    SHA256_update(H, MB, num_blocks);
    SHA256_finalize(H, hash_value);

    free(MB);
}


void Padding_test()
{
    byte msg[5] = { 0, 1, 2, 3, 4 };
    MBlock* ptrMB;

    print_byte_msg(msg, 5);
    //Msg2Block(msg, 5, &ptrMB);
    SHA256_BM_Padding(msg, 5, &ptrMB);
    print_MBlock(ptrMB, 1);

}

void SHA256_test() {
    //const char* str_msg = "abc";
    const char* str_msg = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    byte byte_msg[256];
    byte hash_value[32];
    int byte_len;

    byte_len = strlen(str_msg); // 문자열의 길이
    for (int i = 0; i < byte_len; i++) {
        byte_msg[i] = str_msg[i];
    }

    printf("SHA256 test...\n");
    SHA256(byte_msg, byte_len, hash_value);
    printf("Hash Value = ");
    print_byte_msg(hash_value, 32);
}

int main() {
    SHA256_test();
}
