// Matrix_Class_Test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "CMatrix.h"

void matrix_test() {
    CMatrix A;       //counter++
    CMatrix B(2, 2); //counter++
    CMatrix C; //counter++
    CMatrix D; //counter++
   

    A.ReSize(2, 2);
    double a[2][2] = { 1,2,3,4 };
    double b[2][2] = { 10,20,30,40 };

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            A.SetValue(i, j, a[i][j]);
            B.SetValue(i, j, b[i][j]);
        }
    } 

    A.Print();
    B.Print();
    C = A.Add(B);
    C.Print();  // A,B,C 이렇게 생성해두면 각자의 row, col변수는 다를수 있지만 static변수는 클래스당 한개만 사용한다! 즉 각자 고유의 값을 가지는게 아니다
    

    D = CMatrix::Add(A, B);
    D.Print();

    cout << "Matrix Counter = " << CMatrix::GetCounter() << endl; //모든 cpp파일에서의 CMatrix 함수 호출 횟수
}
//전역변수처럼 설정이 필요 (정적변수라서)
int CMatrix::counter = 0;

int main()
{
    matrix_test();
}

