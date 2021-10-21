#pragma once

#include <iostream>

using namespace std;

#define MAX 10

class CMatrix
{
private: //�ܺο��� ���� �Ұ�
	int row;
	int col;
	double M[MAX][MAX];
	static int counter; //�������� (Ŭ���� �̸��� �Ѱ��� �ִ�!!!!-�߰����) ������ Matrix_Class_Test.cpp����
public:
	// ������ (����Լ��̸� = Ŭ�����̸�)
	CMatrix() { IncreaseCounter(); row = 1; col = 1; M[0][0] = 0.0; };// inline �Լ�
	CMatrix(int r, int c) { IncreaseCounter(); row = r; col = c;/*M[][]�ʱ�ȭ �����༭ ����*/ }; //inline �Լ�
	void Print();
	void ReSize(int r, int c) { row = r; col = c; };
	void SetValue(int r, int c, double x) { M[r][c] = x; };
	double GetValue(int r, int c) { return M[r][c]; };
	int GetRow() { return row; };
	int GetCol() { return col; };
	CMatrix Add(CMatrix A); // C = B.Add(A) : C == A+B;
	//����� Scalar����
	static void SetCounter(int n) { counter = n; };
	static void IncreaseCounter() { counter++; };
	static int GetCounter() { return counter; };
	// �Լ� �տ� static�� counter�� ���������̱⶧���� ���ذ�!

	static CMatrix Add(CMatrix A, CMatrix B); //static ��� �Լ�

};

