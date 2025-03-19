#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "postfix.h"
#include "findroot.h"
#include <pthread.h>
#define NUM_THREADS 2

int found = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
float best_result = 0.0;

typedef struct
{
    Token *postfix;
    float result;
} ThreadData;

void *findrootNewton(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    data->result = newtonRaphson(data->postfix);

    pthread_mutex_lock(&mutex);
    if (!found)
    {
        best_result = data->result;
        found = 1;
        printf("Newton Raphson tim duoc nghiem la: %f\n", best_result);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *findrootBisection(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    data->result = bisectionMethod(data->postfix);

    pthread_mutex_lock(&mutex);
    if (!found)
    {
        best_result = data->result;
        found = 1;
        printf("BiSection tim duoc nghiem la: %f\n", best_result);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *findrootSecant(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    data->result = secantMethod(data->postfix);

    pthread_mutex_lock(&mutex);
    if (!found)
    {
        best_result = data->result;
        found = 1;
        printf("Secant tim duoc nghiem la: %f\n", best_result);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main()
{
    struct timespec start, end;
    Token *output;
    char str[MAX];

    printf("Nhập biểu thức: ");
    fgets(str, MAX, stdin);
    str[strcspn(str, "\n")] = 0;

    output = infixToPostfix(str);
    if (output != NULL)
    {
        printTokens(output);
        pthread_t threads[NUM_THREADS];     //sau khi thread được tạo thì ID được lưu vào đây
        ThreadData threadData[NUM_THREADS]; //input cho thread

        clock_gettime(CLOCK_MONOTONIC, &start);

        threadData[0].postfix = output;
        pthread_create(&threads[0], NULL, findrootNewton, (void *)&threadData[0]);      //Tạo thread mới cho findrootNewton với đầu vào là struct threadData[0]

        threadData[1].postfix = output;
        pthread_create(&threads[1], NULL, findrootSecant, (void *)&threadData[1]);      //Tạo thread mới cho findrootSecant với đầu vào là struct threadData[1]

        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_cancel(threads[i]);
        }

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Thời gian tìm nghiệm: %f giây\n\n", elapsed);

        printf("Kết quả với nghiệm %.2f là: %.3f\n", best_result, evaluatePostfix(output, best_result));
    } 
    return 0;
}
//
//(x^14-3*x^12+7*x^9)-(5*x^8+2*x^6)+(4*x^5-11*x^3+6*x^2)-(20*x-50)
//x^10+2*x+sin(x^2+2*x-10)