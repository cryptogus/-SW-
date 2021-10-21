#include "CMatrix.h" // Ŭ���� ������ �̿��ϸ� �ѹ��� ������

void CMatrix::Print() {
	for (int i = 0; i < row; i++) {
		printf("[");
		for (int j = 0; j < col; j++) {
			printf("%7.3f", M[i][j]);
		}
		printf(" ]\n");
	}
	printf("\n");
}

CMatrix CMatrix::Add(CMatrix A) {
	//A�� ����� ũ�Ⱑ ���ƾ��Ѵ�
	CMatrix C; //������ CMatrix() ȣ��, //counter++

	C.ReSize(row, col);
	for (int j = 0; j < row; j++) {
		for (int i = 0; i < col; i++) {
			C.SetValue(j, i, M[j][i] + A.GetValue(j, i));
		}
	}
	return C;
}

CMatrix CMatrix::Add(CMatrix A, CMatrix B) {

	CMatrix C; //������ CMatrix() ȣ��, //counter++
	int r, c;
	r = A.GetRow();
	c = A.GetCol();

	C.ReSize(r, c);
	for (int j = 0; j < r; j++) {
		for (int i = 0; i < c; i++) {
			C.SetValue(j, i, A.GetValue(j, i) + B.GetValue(j, i));
		}
	}
	return C;
}