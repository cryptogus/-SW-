#pragma once
typedef unsigned char byte;
typedef unsigned int u32;

//바이트열 <--> 정수
#define GETU32(b) ((u32)b[0] << 24) ^ ((u32)b[1] << 16) ^ ((u32)b[2] << 8) ^ ((u32)b[3])
#define PUTU32(b,x) {\
(b)[0] = (byte)((x) >> 24);\
(b)[1] = (byte)((x) >> 16);\
(b)[2] = (byte)((x) >> 8);\
(b)[3] = (byte)(x);}
void byte2state(byte b[16], u32 state[4]);
void state2byte(u32 state[4], byte b[16]);
void AES_print_state(byte state[16], const char* msg = nullptr);
void AES32_print_state(u32 state[4]);
void AES32_round(u32 state[4], u32 rk[4]);
void AES32_Encrypt(byte pt[16], u32 rk[11][4], byte ct[16]);