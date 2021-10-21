#pragma once

typedef unsigned char byte;

void AES8_SubBytes(byte state[16]);
void AES8_ShiftRows(byte state[16]);
byte GF_xtime(byte gf);
void AES8_MixColumns(byte state[16]);
void AES8_AddRoundkey(byte state[16], byte roundkey[16]);
void AES8_Round(byte state[16], byte rk[16]);
void AES8_Encrypt(byte PT[16], byte RK[11][16], byte CT[16]);
void AES8_print_state(byte state[16]);

void AES8_InvSubBytes(byte state[16]);
void AES8_InvShiftRows(byte state[16]);
void AES8_InvMixColumns(byte state[16]);
void AES8_InvRound(byte state[16], byte rk[16]);
void AES8_Decrypt(byte CT[16], byte RK[11][16], byte PT[16]);
void AES8_EqDecrypt(byte CT[16], byte EqRK[11][16], byte PT[16]);
void AES8_EqInvKey(byte rk[11][16], byte eqrk[11][16]);

