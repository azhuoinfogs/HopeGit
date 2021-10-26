#include "add.h"
int add(int a, int b) { 
    int c = a+b;
    return c;
}
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