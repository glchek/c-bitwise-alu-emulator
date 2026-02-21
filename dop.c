#include <stdio.h>
// #define FASTMULT
/*
чтобы побитово менять знак числа пользуюсь доп кодом и алгоритмом ~a + 1
log b существует b>0 — ограничение
log b — в знаменателе
значит b != 1
floor(1 / log b) в знаменателе, значит floor(1 / log b) != 0, но
 если log b > 1, то floor(1 / log b) = 0, значит, b <= 2
 b!= 0, b > 0 , b != 1 , b <= 2 , значит b = 2

я написал весь этот код, комментарии, оддебажил его, и только сейчас до меня
дошло, что про отрицательные можно было вообще не думать, так как 4 — чётное но
зато я написал, вроде бы универсальные функции, по крайней мере на нужном
отрезке дают корректный результат

*/

int _iss(int, int);
int _sum(int, int);

// обёртки для проверки отрицательных чисел
int iss(int, int);
int sum(int, int);
void mdebug();
void adebug(int, int, int, int);

// возвращает
// 1, если a<b
// 0, если !(a<b)
int iss(int a, int b) {
    if (a & (1 << 31) && b & (1 << 31))
        return _iss(~b + 1, ~a + 1);
    else if (!(a & (1 << 31)) && b & (1 << 31))
        return 0;
    else if (a & (1 << 31) && !(b & (1 << 31)))
        return 1;
    return _iss(a, b);
}

// тоже самое, но a и b только положительные
int _iss(int a, int b) {
    int cb = 1 << 30;
    while (cb) {  // сравниваю от старшего рахряда к младшему, если у a в бите
                  // 1, а у b в бите 0, а все предыдущие биты совпали, то a<b
        if (a & cb && !(b & cb)) return 0;
        if (!(a & cb) && b & cb) return 1;
        cb >>= 1;
    }
    return 0;
}

// складывает только такие числа, что результат неотрицаителен, потому что при
// сложении двух отрицательных или (маленького по модулю полож. и большого по
// модулю отр) возникают проблемы с переполнением из-за доп кода(в cf переход из
// знакового разряда) та же ошибка при сложении слишком больших чисел
int _sum(int a, int b) {
    int val = 0;  // ответ будет здесь
    // в каждом разряде per отмечено единицей, есть ли не учтённый перенос в
    // этот разряд
    int per = 0;
    int cb = 1;  // 1 стоит в текущем рассматриваемом бите чисел

    while (!iss(cb, 0)) {
        // когда 1 уйдёт в самый правый край двоичного представления,
        // оно станет отрицательным из определения доп. кода
        // printf("%d %d\n", cb, val);
        // сложение в столбик
        if (!per) {
            // a&cb — отвечает на вопрос, есть ли в числе a, на той позиции,
            // которую мы рассматриваем единица val|= cb, так как в начале
            // итерации рассматриваемый бит равнен нулю, то val|= cb записывает
            // единицу в текущий бит в val
            //  a | b — побитовое или, a&b — побитовое и
            // если только в одном из чисел на текущем бите стоит единица
            if ((a & cb && !(b & cb)) || !(a & cb) && (b & cb)) val |= cb;
            // если ни в одном из чисел на текущем нет единицы
            if (!(a & cb) && !(b & cb))
                ;
            // если в обоих числа на текущем бите стоят единицы
            if ((a & cb) && (b & cb))
                per |= cb << 1;  // если в обоих числах единицы в текущем бите,
                                 // то на следующем возникает переполнение
        } else if (per) {
            // если только одна единица
            if ((a & cb) && !(b & cb) || !(a & cb) && (b & cb)) per <<= 1;
            // если нет единиц
            if (!(a & cb) && !(b & cb)) {
                val |= per;
                per = 0;
            }
            // если в обоих единицы
            if ((a & cb) && (b & cb)) {
                val |= cb;
                per <<= 1;
            }
        }
        cb <<= 1;  // в конце итерации меняем рассматриваемый бит на последующий
    }
    return val;
}

// функция управления суммой
//np — отр
//p — пол
int sum(int a, int b) {
    // обработка отрицательных чисел

    if (a & (1 << 31) && b & (1 << 31))  // a -np | b - np
        return ~_sum(~a + 1, ~b + 1) + 1;
    else if (a & (1 << 31) && !(b & (1 << 31))) {      // a - np | b - p
        if (iss(~a + 1, b))                            //  5 - 4
            return _sum(a, b);                         // p + p
        else                                           // 4 - 5 = -(5 - 4)
            return ~_sum(~a + 1, ~b + 1) + 1;        
    } else if (!(a & (1 << 31)) && (b & (1 << 31))) {  // a - p | b - np
        if (iss(~b + 1, a))
            return _sum(a, b);
        else
            return ~_sum(~a + 1, ~b + 1) + 1;
    }

    return _sum(a, b);
}

#ifndef FASTMULT
int mult(int a, int b) {  // умножаю по определению из начальной школы
    int flag = (iss(a, 0) && !iss(b, 0) ||
                !iss(a, 0) && iss(b, 0));  // отвечает на вопрос, должен ли
                                           // результат быть отрицательным
    if (iss(a, 0)) a = ~a + 1;
    if (iss(b, 0)) b = ~b + 1;
    int val = 0;
    while (b) {
        val = sum(val, a);
        b = sum(b, -1);
    }
    if (flag) return ~val + 1;
    return val;
}
#endif

// более асимптотически простое умножение
//  работает так как a * b = a * (2^a_1 + 2^a_2 + ... + 2^a_n) <- двоичное
//  предстваление b
#ifdef FASTMULT
int mult(int a, int b) {
    int val = 0;
    int flag = (iss(a, 0) && !iss(b, 0) || !iss(a, 0) && iss(b, 0));
    if (iss(a, 0)) a = ~a + 1;
    if (iss(b, 0)) b = ~b + 1;
    while (b) {
        if (b & 1) val = sum(val, a);
        a = a << 1;
        b = b >> 1;
    }
    if (flag) return ~val + 1;
    return val;
}
#endif

// manual debug простых операций
void mdebug() {
    int a, b;
    scanf("%d %d", &a, &b);
    printf("%d %d %d\n", a + b == sum(a, b), a + b, sum(a, b));
    printf("%d %d %d\n", a * b == mult(a, b), a * b, mult(a, b));
    printf("%d %d %d\n", a < b == iss(a, b), a < b, iss(a, b));
}
// auto debug простых операций
void adebug(int l, int r, int i, int j) {
    int check;
    check = 1;
    for (int a = l; a <= r; a++) {
        printf("a \t%d\n", a);
        for (int b = i; b <= j; b++) {
            check = (a + b == sum(a, b)) && (a * b == mult(a, b)) &&
                    ((a < b) == iss(a, b));
            if (!check) {
                printf("%d %d", a, b);
                return;
            }
        }
    }
    printf("ok");
}

int main() {
    int a, b, c, check, temp;
    // из-за ограничений типа int
    printf("поддерживаются числа [-215; 215]\n");
    // mdebug();
    // adebug(-215, 215, -215, 215);
    // return 0;

    for (;;) {
        check = 0;
        printf("Введите a: ");
        check = scanf("%d", &a);
        if (check)
            if (!iss(a, -215) && iss(a, 215 + 1)) break;
        printf("Неверный ввод\n");
        do {
            temp = getchar();
        } while ((temp != '\n' && temp != EOF));
    }
    for (;;) {
        check = 0;
        printf("Введите b: ");
        check = scanf("%d", &b);
        if (check)
            if (!iss(b, 2) && !iss(2, b)) break;
        printf("Неверный ввод\n");
        do {
            temp = getchar();
        } while ((temp != '\n' && temp != EOF));
    }
    for (;;) {
        check = 0;
        printf("Введите c: ");
        check = scanf("%d", &c);
        if (check)
            //(c >= 0 && 0 >= c) => 0 = c
            if (!(!iss(c, 0) && !iss(0, c)) && !iss(c, -215) && iss(c, 215 + 1))
                break;
        printf("Неверный ввод\n");
        do {
            temp = getchar();
        } while ((temp != '\n' && temp != EOF));
    }
    double val =
        ((double)mult(mult(a, a), mult(a, a))) / (mult(mult(c, c), mult(c, c)));
    printf("%g", val);
    return 0;
}
