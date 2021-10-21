#pragma once

#include <iostream>

using namespace std;

#define MAX 10

class CMatrix
{
private: //외부에서 접근 불가
	int row;
	int col;
	double M[MAX][MAX];
	static int counter; //정적변수 (클래스 이름당 한개만 있다!!!!-중간고사) 예제는 Matrix_Class_Test.cpp에서
public:
	// 생성자 (멤버함수이름 = 클래스이름)
	CMatrix() { IncreaseCounter(); row = 1; col = 1; M[0][0] = 0.0; };// inline 함수
	CMatrix(int r, int c) { IncreaseCounter(); row = r; col = c;/*M[][]초기화 안해줘서 경고뜸*/ }; //inline 함수
	void Print();
	void ReSize(int r, int c) { row = r; col = c; };
	void SetValue(int r, int c, double x) { M[r][c] = x; };
	double GetValue(int r, int c) { return M[r][c]; };
	int GetRow() { return row; };
	int GetCol() { return col; };
	CMatrix Add(CMatrix A); // C = B.Add(A) : C == A+B;
	//상수배 Scalar곱셈
	static void SetCounter(int n) { counter = n; };
	static void IncreaseCounter() { counter++; };
	static int GetCounter() { return counter; };
	// 함수 앞에 static은 counter가 정적변수이기때문에 써준것!

	static CMatrix Add(CMatrix A, CMatrix B); //static 멤버 함수

};

