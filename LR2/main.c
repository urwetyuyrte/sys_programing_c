#include <stdio.h>
#include <math.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

int group_number = 80;
int variant_number = 22;
#define NAME_NUMBER 11

void check_sqrt(int group, const char *name) {
    double root = sqrt(group);
    size_t length = 0;
    for (size_t i = 0; i < strlen(name); ++i) {
        if (((unsigned char)name[i] & 0xC0) != 0x80)
            ++length;
    }
    printf("sqrt(%d) = %.6f; довжина імені = %zu\n", group, root, length);
    if (root >= length)
        printf("s = %.6f\n", root + length);
    if (root <= length)
        printf("n = %.6f\n", root * length);
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    char name[] = "Гліб";
    printf("Мій номер за журналом %d. Його Sin = %.6f\n",
           variant_number, sin(variant_number));
    printf("Група: %d; варіант: %d; ім'я: %s\n",
           group_number, variant_number, name);
    double product = variant_number * 3.14;
    int converted = (int)product;
    printf("Добуток: %.2f; ціле: %d\n", product, converted);
    int sum = 0;
    for (int i = variant_number + NAME_NUMBER; i > 1; --i)
        sum += i - 1;
    printf("NAME_NUMBER = %d; сума = %d\n", NAME_NUMBER, sum);
    check_sqrt(group_number, name);
    int group;
    printf("Введіть номер групи: ");
    if (scanf("%d", &group) != 1) {
        puts("Некоректне введення.");
        return 1;
    }
    switch (group) {
        case 92: case 93: case 94: case 95:
        case 96: case 97: case 98: case 99:
            puts("I курс"); break;
        case 87: case 88: case 89: case 90: case 91:
            puts("II курс"); break;
        case 81: case 82: case 83: case 84: case 85: case 86:
            puts("III курс"); break;
        case 75: case 76: case 77: case 78: case 79: case 80:
            puts("IV курс"); break;
        default: puts("Такої групи немає."); break;
    }
    return 0;
}
