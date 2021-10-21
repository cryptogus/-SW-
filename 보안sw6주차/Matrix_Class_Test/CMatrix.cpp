#include "CMatrix.h" // 클레스 마법사 이용하면 한번에 생성됨

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
	//A와 행렬의 크기가 같아야한다
	CMatrix C; //생성자 CMatrix() 호출, //counter++

	C.ReSize(row, col);
	for (int j = 0; j < row; j++) {
		for (int i = 0; i < col; i++) {
			C.SetValue(j, i, M[j][i] + A.GetValue(j, i));
		}
	}
	return C;
}

CMatrix CMatrix::Add(CMatrix A, CMatrix B) {

	CMatrix C; //생성자 CMatrix() 호출, //counter++
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