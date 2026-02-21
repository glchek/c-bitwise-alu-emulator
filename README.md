# Math problem with only bitwise operations

Реализовано в сентябре 2024, как решение следующей дополнительной задачи к лабораторной:
> Вычислить выражение 
$$
\left(\frac{a}{\lfloor \frac{1}{\log_2 b} \rfloor*c}\right)^4
$$
>для целых чисел, используя только побитовые операции.

## Анализ
Прежде чем писать код, проанализировал условие: 

$\log_2 b$ существует $b>0$ — ограничение

$\log_2 b$ — в знаменателе, значит $b \neq 1$

$\lfloor \frac{1}{\log_2 b} \rfloor$ в знаменателе, значит $\lfloor \frac{1}{\log_2 b} \rfloor \neq 0$, но
 $\log_2 b > 1 \implies \lfloor \frac{1}{\log_2 b} \rfloor = 0$, значит, $b \le 2$
 
 $b \neq 0, \quad b > 0 , \quad  b \neq 1, \quad b \le 2$ , значит $b = 2$

задача сводится к вычислению $\left(\frac{a}{c} \right)^4$





