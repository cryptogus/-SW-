// week-matrix.cpp :  GF(2^8) 행렬 연산

#include <iostream>
using namespace std;

// week2-GF.cpp

//======(week2) GF연산 함수들 (시작)
typedef unsigned char byte; //새로운 데이터 타입:byte

//GF 다항식 출력
void GF_print(byte gf)
{
    int coef;

    printf("%d=%02x = ", gf, gf);
    for (int i = 7; i >= 0; i--)
    {
        coef = (gf >> i) & 0x01;
        if (coef == 1)
        {
            std::cout << " + " << "x^" << i;
        }
    }
    std::cout << std::endl;
}

//GF(2^8) 덧셈
byte GF_add(byte gf1, byte gf2)
{
    return gf1 ^ gf2;//XOR
}

//GF(2^8) xtime() 함수
byte GF_xtime(byte gf)
{
    int msb; //x^7의 개수
    byte result;//결과

    msb = (gf >> 7) & 0x01;
    if (msb == 1)
    {
        result = (gf << 1) ^ 0x1b;
    }
    else
    {
        result = gf << 1;
    }
    return result;
}

byte GF_xtime_simple(byte gf)
{
    return((gf >> 7) & 0x01 == 1 ? (gf << 1) ^ 0x1b : gf << 1);
}

//GF(2^8)곱셈
//f(x)=a0+a1*x + a2*x^2 + a3*x^3+...+a7*x^7
//h(x)=g(x)*f(x)
//    =g(x)*a0 + g(x)*a1*x + g(x)*a2*x^2+...+g(x)*a7*x^7
//    =g(x)*a0 + x*(g(x)*a1 + g(x)*a2*x+...+ g(x)*a7*x^6)
//    =g(x)*a0 + x*(g(x)*a1 + x(g(x)*a2+...+ g(x)*a7*x^5) )
//    = g(x) * a0 + x *(... +x*(g(x)*a7) ) )
byte GF_mul(byte f, byte g)
{
    byte h;//곱셈 결과 h(x)=f(x)*g(x) 
    int coef; //계수
    h = 0x00;
    int i;
    for (i = 7; i >= 0; i--)
    {
        coef = (f >> i) & 0x01;// a7, a6, a5, ..., a0
        h = GF_xtime(h);
        if (coef == 1)
        {
            h = GF_add(h, g);
        }
    }
    return h;
}

//gf(2^8) 역원 (a^255=1)임을 이용한다.
//a^(-1) = a^(254)= a^2*a^4* a^8*a^16*...* a^128
//a^(1111 1110)=a^(1000 0000)*a^(0100 0000)*...*a^(0000 0010)
byte GF_inv(byte f)
{
    byte f_inv; //역원(결과값)
    byte temp; //장간에 곱할 값(a^n):a^2*a^4* a^8*a^16*...* a^128
    f_inv = 1;
    temp = f;
    for (int i = 0; i < 7; i++)
    {
        temp = GF_mul(temp, temp);
        f_inv = GF_mul(f_inv, temp);
    }
    return f_inv;
}

//======(week2) GF연산 함수들 (끝)

//-------GF의 행렬 연산
#define MAX  10

struct GF_Matrix {
    byte M[MAX][MAX];
    int row;
    int col;
};

GF_Matrix GF_Mat_init()
{
    GF_Matrix M;
    M.row = MAX;
    M.col = MAX;
    for (int i = 0; i < M.row; i++)
    {
        for (int j = 0; j < M.col; j++)
        {
            M.M[i][j] = (i == j) ? 1.0 : 0;
        }
    }
    return M;
}

void GF_Mat_print(GF_Matrix M)
{
    for (int i = 0; i < M.row; i++)
    {
        printf("[");
        for (int j = 0; j < M.col; j++)
        {
            printf(" %02x", M.M[i][j]);
        }
        printf(" ]\n");
    }
    printf("\n");
}

GF_Matrix GF_Mat_Mul(GF_Matrix A, GF_Matrix B)
{
    GF_Matrix AB;
    byte temp;
    if (A.col != B.row)
    {
        cout << "Matrix size error! \n";
        return A;
    }
    AB.row = A.row;
    AB.col = B.col;

    for (int i = 0; i < AB.row; i++)
    {
        for (int j = 0; j < AB.col; j++)
        {
            AB.M[i][j] = 0.0;
            for (int k = 0; k < A.col; k++)
            {
                //AB.M[i][j] += A.M[i][k] * B.M[k][j];
                temp = GF_mul(A.M[i][k], B.M[k][j]);
                AB.M[i][j] = GF_add(AB.M[i][j], temp);
            }
        }
    }
    return AB;
}

void GF_Mat_Exchange_Row(GF_Matrix& A, int row1, int row2)
{
    byte temp;
    for (int i = 0; i < A.col; i++)
    {
        temp = A.M[row1][i];
        A.M[row1][i] = A.M[row2][i];
        A.M[row2][i] = temp;
    }
}

void GF_Mat_Scalar_Mul(GF_Matrix& A, byte scalar, int row)
{
    byte temp;
    for (int i = 0; i < A.col; i++)
    {
        //A.M[row][i] *= scalar;
        temp = GF_mul(A.M[row][i], scalar);
        A.M[row][i] = temp;
    }
}

void GF_Mat_Row_Add(GF_Matrix& A, byte scalar, int row_src, int row_target)
{
    byte temp;
    for (int i = 0; i < A.col; i++)
    {

        //A.M[row_target][i] += scalar * A.M[row_src][i];
        temp = GF_mul(scalar, A.M[row_src][i]);
        A.M[row_target][i] = GF_add(A.M[row_target][i], temp);
    }
}

GF_Matrix GF_Mat_inverse(GF_Matrix A)
{
    GF_Matrix AA; // [A|I]
    AA.row = A.row;
    AA.col = A.col * 2;
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < A.col; j++)
        {
            AA.M[i][j] = A.M[i][j];
            AA.M[i][A.col + j] = (i == j) ? 0X01 : 0X00;
        }
    }
    
    int pivot_row;
    int cnt = 1;
    GF_Mat_print(AA);
    for (int j = 0; j < A.col; j++)
    {
        
        
        pivot_row = -1;
        for (int i = j; i < A.row; i++)
        {

            if (AA.M[i][j] != 0x00)
            {
                pivot_row = i;

            }
            if (pivot_row == -1)
            {
                cout << "Not invertible!\n";
                return A; //의미 없는 출력
            }

            if (pivot_row != j)//j번째 행 <--> pivot 행
            {

                GF_Mat_Exchange_Row(AA, j, pivot_row);
            }
            byte temp;
            temp = GF_inv(AA.M[j][j]);

            GF_Mat_Scalar_Mul(AA, temp, j);

            for (int i = 0; i < A.row; i++)
            {
                if ((i != j) & (AA.M[i][j]) != 0)
                {

                    GF_Mat_Row_Add(AA, AA.M[i][j], j, i); //X+X=0 in GF(2^8)
                }
            }
        }
        printf("[step %d]\n", cnt++);
        GF_Mat_print(AA);
       

    }
    
    GF_Matrix Inv;
    Inv.row = A.row;
    Inv.col = A.col;
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < A.col; j++)
        {
            Inv.M[i][j] = AA.M[i][j + A.col];
        }

    }

    return Inv;
}

void Run_GF_matrix_test()
{
    byte MixCol[4][4] = { {0x02,0x03,0x05,0x01}, {0x01,0x02,0x03,0x05}
                         ,{0x05,0x01,0x02,0x03}, {0x03,0x05,0x01,0x02} };
    GF_Matrix MC, InvMC;
    MC.row = 4;
    MC.col = 4;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            MC.M[i][j] = MixCol[i][j];
    

    InvMC = GF_Mat_inverse(MC);
    GF_Mat_print(InvMC);
}

int main()
{
    Run_GF_matrix_test();
}