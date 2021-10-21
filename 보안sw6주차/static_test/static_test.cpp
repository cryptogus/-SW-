// static_test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>


using namespace std;


//int counter; 전역변수 바람직 X

static int f(int n) // 정적 함수 (이 파일 내에서만 접근 가능)
{
	static int counter = 0; //정적 변수 (초기화 할 수 없다(0 으로 자동 초기화))
	counter++;
	if (counter % 5 == 0) {
		cout << "counter = " << counter << endl;
	}
	if (n <= 1) {
		return 1;
	}
	else {
		return f(n - 1) + f(n - 2);
	}
}
void static_c_test() {

	int n = 10;
	cout << "f(" << n << ")=" << f(n) << endl;
	//cout << "counter = " << counter;

}

int main()
{
	static_c_test();
}

