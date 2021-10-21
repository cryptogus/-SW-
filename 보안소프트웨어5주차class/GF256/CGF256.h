#pragma once

typedef unsigned char byte; //���ο� ������ Ÿ��: byte - GF(2^8)

class CGF256 { // GF(2^8) class
private:
    byte mData; //GF(2^8) ����
public:
    CGF256() { mData = 0x00; };
    CGF256(byte data) { mData = data; };

    void SetValue(byte data) { mData = data; };
    byte GetValue() { return mData; };
    void PrintAsHex();
    void PrintAsPolynomial();
    CGF256 Add(CGF256 x);
    CGF256 Mul(CGF256 x);
    CGF256 xtime();
    CGF256 Inv();

};