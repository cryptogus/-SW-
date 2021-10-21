// 보안소프트웨어5주차struct.cpp
//

#include <iostream>
#define MAX_SIZE 10

struct st_Vector //구조체 - 사용자가 정의한 데이터 타입
{
    int size;
    double v[MAX_SIZE];
};

void stVector_print(st_Vector vec) {
    printf("[ ");
    for (int i = 0; i < vec.size; i++) {
        printf("%7.3f", vec.v[i]);
    }
    printf(" ]\n");
}

int main()
{
    st_Vector my_vector;

    my_vector.size = 3;
    my_vector.v[0] = 1.0;
    my_vector.v[1] = 2.0;
    my_vector.v[2] = 3.0;

    stVector_print(my_vector);
}

