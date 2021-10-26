#include <stdio.h>
#include <stdlib.h>
#include "add.h"
#include <iostream>
using namespace std;
int print_data(int a, int b) {
    printf("%d--%d\n", a, b);
    return 0;
}
int c1_data(int a, int b) {
    printf("%d**%d\n", a, b);
    return a+b;
}
int c2_data(int a, int b) {
    printf("%d&&%d\n", a, b);
    return a-b;
}
int c3_data(int a, int b) {
    printf("%d^^%d\n", a, b);
    return a*b;
}
int c4_data(int a, int b) {
    printf("%d##%d\n", a, b);
    return a/b;
}


int main(){
    int a = 1,b = 2;
    cout<<add(a,b)<<endl;
}