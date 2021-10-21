//행렬 기본 연산
#include <iostream>
//week 3 행렬 연산//
#define MAX 4 // 4x4 행렬

void Mat_print(double M[MAX][MAX], int row, int col) {
    for (int i = 0; i < row; i++) {
        printf("[");
        for (int j = 0; j < col; j++) {
            printf("%7.3f", M[i][j]);
        }
        printf(" ]\n");
    }
}

//행렬 곱셈
void Mat_mul(double MA[MAX][MAX], double MB[MAX][MAX], double MC[MAX][MAX], int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            MC[i][j] = 0.0;
            for (int k = 0; k < col; k++) {
                MC[i][j] += MA[i][k] * MB[k][j];
            }
        }
    }
}

void Run_matrix_test() {
    double MA[MAX][MAX] = { {1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4} };
    double MB[MAX][MAX] = { {1,0,1,1},{2,2,0,2},{3,3,0,3},{4,4,4,0} };
    double MC[MAX][MAX];
    Mat_print(MA, 4, 4);
    printf("\n");
    Mat_print(MB, 4, 4);
    Mat_mul(MA, MB, MC, 4, 4);
    printf("\n");
    Mat_print(MC, 4, 4);
}

//구조체를 이용한 행렬

#define MAX2 10

struct Matrix  //구조체도 int같은 자료형 느낌이다. reference 전달가능 (포인터?)
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

void Mat_mul2(Matrix A, Matrix B, Matrix &AB) { //Matrix &AB >> call by reference
    if (A.col != B.row) { // 구조체변수 AB를 만들어서 return AB로 쓸수있지만 조금 느릴수도 call by value
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
            MA.M[i][j] = i + j;
            MB.M[i][j] = j;
        }
    }
    Mat_mul2(MA, MB, MC);
    Mat_print2(MC);
}

// 역행렬 구하기

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
void Mat_Row_Add(Matrix & A, double scalar, int row_src, int row_target) {
    for (int i = 0; i < A.col; i++) {
        A.M[row_target][i] += scalar * A.M[row_src][i];
    }
}
Matrix Mat_inverse(Matrix A) {
    Matrix AA; //[A|I] A행렬과 단위행렬 붙여놓은 형태로 정의
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
            return A; //의미없는 출력
        }
        if (pivot_row != j) { //j번째 행 < -- > pivot 행
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
//byte GF_xtime_simple(byte gf)
//{
//    return((gf >> 7) & 0x01 == 1 ? (gf << 1) ^ 0x1b : gf << 1);
//}

void Run_matrix_test3() {
    Matrix A, invA;
    double Data[3][3] = { { 0.5, -0.25, 0}, {-0.25, 0.5, -0.25},{0, -0.25, 0.5} };
    A.row = 3;
    A.col = 3;
    for (int i = 0; i < A.row; i++) {
        for (int j = 0; j < A.col; j++) {
            A.M[i][j] = Data[i][j];
        }
    }
    Mat_print2(A);
    invA = Mat_inverse(A);
    printf("\n");
    Mat_print2(invA);
}

int main() {
    Run_matrix_test();
    Run_matrix_test2();
    printf("\n");
    Run_matrix_test3();
}