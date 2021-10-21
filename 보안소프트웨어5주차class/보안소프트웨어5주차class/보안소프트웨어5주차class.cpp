// 보안소프트웨어5주차class.cpp
//

#include <iostream>
#define MAX_SIZE 10

//struct st_Vector //구조체 - 사용자가 정의한 데이터 타입
//{
//    int size;
//    double v[MAX_SIZE];
//};

// C++ OOP (Object Oriented Programming): Encapsulation, Polymorphism, Inheritance
//Encapsulation: Data (Set) + Operation (Structure, relation...)

class CVector //클래스 벡터
{
	//3가지 선택: private[기본값], public, protected  
private:	//private는 외부에서 접근할수없다.(my_vector.size = 3; 변수.변수 로 접근 불가) 내부에서 바꿀수있다.
	//멤버 변수들 (Data) -> 구조체는 여기까지만 가능
	int mSize;
	double mVec[MAX_SIZE];
public:  
	//생성자(constructor) : 변수가 생설될 때 호출되는 함수 ex)CVector my_vector(6);
	
	CVector() { mSize = 1; };//CVector my_vector만 쓰면 자동으로 1로 초기회(기본 생성자)
	CVector(int size) { mSize = size; };//CVector my_vector(2); 이런식으로 써야함
	CVector(int size, double arr[]);
	void SetValue(int nPos, double value);//private에 있는 변수에 접근가능하게 해주는 함수
	double GetValue(int nPos);
	// 멤버 함수들 (function, operation)
	void Print(); //아래에 함수 정의
	void SetSize(int size) { mSize = size; };
	int Size() { return mSize; };

	void ScalarMul(double value);
	CVector Add(CVector cv);  //(현재) v3 - v1.Add(v2):  >(개선)
};
//:: 스코프 연산자
void CVector::Print() { //(리턴타입) (클레스이름)::(멤버함수이름)(...){}
	printf("[ ");
	for (int i = 0; i < mSize; i++) {
		printf("%7.3f", mVec[i]);
	}
	printf(" ]\n");
}
void CVector::ScalarMul(double value) {
	for (int i = 0; i < mSize; i++) {
		mVec[i] *= value;
	}
}
CVector CVector::Add(CVector cv) {
	CVector result;
	if (cv.Size() != mSize) {
		printf("Vector size mismatch!\n");
		return result; //의미없는 결과
	}
	result.SetSize(mSize);
	for (int i = 0; i < mSize; i++) {
		result.SetValue(i, mVec[i] + cv.GetValue(i));
	}

	return result;
}
void CVector::SetValue(int nPos, double value) {
	if (nPos > mSize) {
		printf("Range error\n");
		return;
	}
	mVec[nPos] = value;
}
double CVector::GetValue(int nPos) {
	if (nPos > mSize) {
		printf("Range error\n");
		return -1.0;
	}
	return mVec[nPos];
}
CVector::CVector(int size, double arr[]) {
	mSize = size;
	for (int i = 0; i < size; i++) {
		mVec[i] = arr[i];
	}
}
//void CVector_print(C{Vector vec) {
//    printf("[ ");
//    for (int i = 0; i < vec.size; i++) {
//        printf("%7.3f", vec.v[i]);
//    }
//    printf(" ]\n");
//}

void class_test() {
	CVector my_vector(3); //변수가 생설될 때 호출되는 함수가 정의되어있다.
	// CVector : 클래스명 (int, double)
	// my_vector : 변수(인스턴스, 객체 object)



	//my_vector.mSize = 3;
	my_vector.SetValue(0, 1.0);
	my_vector.SetValue(1, 2.0);
	my_vector.SetValue(2, 3.0);
	

	//CVector_print(my_vector);
	my_vector.Print();

	printf("Start class_test() : 2nd test...\n");
	
	CVector vec2; //기본 생성자
	vec2.SetSize(5);

	double a[5] = { 0.1, 0.2, 0.3, 0.4, 0.5 };
	CVector vec3(5, a);

	vec3.Print();
	vec3.ScalarMul(10);
	vec3.Print();
	CVector vec4(5, a);
	CVector vec5;

	vec5 = vec3.Add(vec4);
	vec5.Print();
}


int main()
{
	class_test();
}

