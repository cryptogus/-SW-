// GF(2^8) ��� ����, MixColumn ����
#include <iostream>
using namespace std;

// Week2 AES SBox���� �Լ���
typedef unsigned char byte; //���ο� ������ Ÿ��: byte - GF(2^8)

//GF ���׽� ���
void GF_print(byte gf) {
    int coef;

    for (int i = 7; i >= 0; i--) {
        coef = (gf >> i) & 0x01;
        if (coef == 1) {
            std::cout << " + " << "x^" << i; //���
        }
    }
    std::cout << std::endl;// �� �ٲ� �Լ�
}

//GF(2^8) ����
byte GF_add(byte gf1, byte gf2) {
    return gf1 ^ gf2;
}
byte GF_xtime(byte gf) {
    int msb; // x^7�� �Լ�
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
//GF(2^8) ����
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
//GF(2^8) ������ ���� (a^256 = 1�ΰ� �̿�)ex) a^(-1) = a^(254) = a^(1111 1110) = a^2*a^4*a^8*a^16*...*a^128
// a^(1111 1110) = a^(1000 0000) * a^(0100 0000) * a^(0010 0000) * ... * a^(0000 0010)
byte GF_inv(byte f) { //Ȯ�� ��Ŭ���� �˰��� ����� ���ϰ� a^256 = 1�ΰ� �̿��� �������� ����!
    byte f_inv = 1; // ����(�����)
    byte temp; //�߰��� ���Ѱ�(a^n)
    temp = f;
    for (int i = 0; i < 7; i++) {
        temp = GF_mul(temp, temp);
        f_inv = GF_mul(f_inv, temp);
    }
    return f_inv;
}
///��///

#define MAX2 10

struct Matrix  //����ü�� int���� �ڷ��� �����̴�. reference ���ް��� (������?)
{
    double M[MAX2][MAX2];
    int row;
    int col;
};

Matrix init() {
    Matrix M;
    M.row = MAX2;
    M.col = MAX2;
    for (int i = 0; i < M.row; i++)
        for (int j = 0; j < M.col; j++)
            M.M[i][j] = (i == j) ? 1.0 : 0.0;
    return M;
}
void Mat_print2(Matrix M) {
    for (int i = 0; i < M.row; i++) {
        printf("[");
        for (int j = 0; j < M.col; j++) {
            printf("%7.3f", M.M[i][j]);
        }
        printf(" ]\n");
    }
}

void Mat_mul2(Matrix A, Matrix B, Matrix& AB) { //Matrix &AB >> call by reference
    if (A.col != B.row) { // ����ü���� AB�� ���� return AB�� ���������� ���� �������� call by value
        cout << "Matrix size error!\n";
        return;
    }
    AB.row = A.row;
    AB.col = B.col;
    for (int i = 0; i < AB.row; i++) {
        for (int j = 0; j < AB.col; j++) {
            AB.M[i][j] = 0.0;
            for (int k = 0; k < A.col; k++) {
                AB.M[i][j] += A.M[i][k] * B.M[k][j];
            }
        }
    }
}
void Run_matrix_test2() {
    Matrix MA, MB, MC;
    MA = init();
    printf("\n");
    Mat_print2(MA);
    printf("\n");
    MA.row = 3; MA.col = 3;
    MB.row = 3; MB.col = 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            MA.M[i][j] = (i + j);
            MB.M[i][j] = j;
        }
    }
    Mat_mul2(MA, MB, MC);
    Mat_print2(MC);
}

// ����� ���ϱ�

#define NEARlY_ZERO 1e-10
void Mat_Exchange_Row(Matrix& A, int row1, int row2) {

    double temp;
    for (int i = 0; i < A.col; i++) {
        temp = A.M[row1][i];
        A.M[row1][i] = A.M[row2][i];
        A.M[row2][i] = temp;
    }
}
void Mat_Scalar_Mul(Matrix& A, double scalar, int row) {
    for (int i = 0; i < A.col; i++) {
        A.M[row][i] *= scalar;
    }
}
void Mat_Row_Add(Matrix& A, double scalar, int row_src, int row_target) {
    for (int i = 0; i < A.col; i++) {
        A.M[row_target][i] += scalar * A.M[row_src][i];
    }
}
Matrix Mat_inverse(Matrix A) {
    Matrix AA; //[A|I] A��İ� ������� �ٿ����� ���·� ����
    AA.row = A.row;
    AA.col = A.col * 2;
    for (int i = 0; i < A.row; i++) {
        for (int j = 0; j < A.col; j++) {
            AA.M[i][j] = A.M[i][j];
            AA.M[i][A.col + j] = (i == j) ? 1.0 : 0.0;
        }
    }

    int pivot_row;
    for (int j = 0; j < A.col; j++) {
        pivot_row = -1;
        for (int i = j; i < A.row; i++) {
            if (abs(AA.M[i][j]) > NEARlY_ZERO) {
                pivot_row = i;
                break;
            }
        }
        if (pivot_row == -1) {
            cout << "Not invertible\n";
            return A; //�ǹ̾��� ���
        }
        if (pivot_row != j) { //j��° �� < -- > pivot ��
            Mat_Exchange_Row(AA, j, pivot_row);

        }
        Mat_Scalar_Mul(AA, 1. / AA.M[j][j], j);
        for (int i = 0; i < A.row; i++) {
            if ((i != j) && (abs(AA.M[i][j]) > NEARlY_ZERO)) {
                Mat_Row_Add(AA, -AA.M[i][j], j, i);
            }
        }
    }
    Matrix inv;
    inv.row = A.row;
    inv.col = A.col;
    for (int i = 0; i < A.row; i++) {
        for (int j = 0; j < A.col; j++) {
            inv.M[i][j] = AA.M[i][j + A.col];
        }
    }
    return inv;
}