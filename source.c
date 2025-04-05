#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define MAX 100
#define NUM_THREADS 2
int found = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
float best_result = 0.0;
/*
S_START: Trạng thái bắt đầu
S_OPERAND: Trạng thái nhận toán hạng
S_OPERATOR: Trạng thái nhận toán tử
S_OPEN: Trạng thái nhận dấu mở ngoặc
S_CLOSE: Trạng thái nhận dấu đóng ngoặc
S_ERROR: Trạng thái lỗi
S_END: Trạng thái kết thúc
*/
typedef enum {S_START, S_OPERAND, S_OPERATOR, S_OPEN, S_CLOSE, S_ERROR, S_END} state_t;

typedef enum {
    OPERAND,
    OPERATOR,
    VARIABLE
} TokenType;

typedef struct {
    TokenType type;
    union {
        float operand;
        char operator;
        float variable;
    } value;
} Token;

typedef struct{
    Token *postfix;
    float result;
} ThreadData;

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int precedence(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

Token *infixToPostfix(char* myFunction){
    state_t current_state = S_START;
    Token *output = (Token *)malloc(MAX * sizeof(Token));
    int outputIndex = 0;
    char stack[MAX];
    int stackTop = -1;
    //printf("%s", *myFunction);
    while (1){
        switch (current_state){
            //printf("%s", *myFunction);
            case S_START:
                if (isdigit(*myFunction) || *myFunction == '.' || *myFunction == 'x') {
                    current_state = S_OPERAND;
                }
                else if (*myFunction == '(') {
                    current_state = S_OPEN;
                }
                else if (*myFunction == 0) {
                    current_state = S_END;
                }
                else {
                    current_state = S_ERROR;
                }
                break;

            case S_OPERAND: {
                float operand = 0.0;
                int decimal_flag = 0;
                float decimal_divisor = 1.0;
                if (isdigit(*myFunction) || *myFunction == '.'){
                    while (isdigit(*myFunction) || *myFunction == '.') {
                        if (*myFunction == '.') {
                            decimal_flag = 1;
                        }
                        else {
                            if (decimal_flag == 0) {
                                operand = operand * 10 + (*myFunction - '0');
                            }
                            else {
                                decimal_divisor *= 10;
                                operand = operand + (*myFunction - '0') / decimal_divisor;
                            }
                        }
                        myFunction++;
                    }
                    output[outputIndex].type = OPERAND;
                    output[outputIndex].value.operand = operand;
                    outputIndex++;
                }
                else if (*myFunction == 'x') {
                    myFunction++;
                    output[outputIndex].type = VARIABLE;
                    output[outputIndex].value.variable = 0.0;
                    outputIndex++;                    
                }       
                if (isOperator(*myFunction)) {
                    current_state = S_OPERATOR;
                }
                else if (*myFunction == ')') {
                    current_state = S_CLOSE;
                }
                else if (*myFunction == 0) {
                    current_state = S_END;
                }
                else {
                    current_state = S_ERROR;
                }
                break;
            }

            case S_OPERATOR:
                while (stackTop >= 0 && isOperator(stack[stackTop]) && 
                       precedence(stack[stackTop]) >= precedence(*myFunction)) {
                    output[outputIndex].type = OPERATOR;
                    output[outputIndex].value.operator = stack[stackTop];
                    outputIndex++;
                    stackTop--;
                }
                stack[++stackTop] = *myFunction;
                myFunction++;
                current_state = S_START;
                break;

            case S_OPEN:
                stack[++stackTop] = *myFunction;
                myFunction++;
                current_state = S_START;
                break;

            case S_CLOSE:
                while (stackTop >= 0 && stack[stackTop] != '(') {
                    output[outputIndex].type = OPERATOR;
                    output[outputIndex].value.operator = stack[stackTop];
                    outputIndex++;
                    stackTop--;
                }
                if (stackTop >= 0) stackTop--; 
                myFunction++;
                if (isOperator(*myFunction)) {
                    current_state = S_OPERATOR;
                }
                else if (*myFunction == ')') {
                    current_state = S_CLOSE;
                }
                else if (*myFunction == 0) {
                    current_state = S_END;
                }
                else if (isdigit(*myFunction) || *myFunction == '.') {
                    current_state = S_OPERAND;
                }
                else {
                    current_state = S_ERROR;
                }
                break;

            case S_END:
                while (stackTop >= 0) {
                    output[outputIndex].type = OPERATOR;
                    output[outputIndex].value.operator = stack[stackTop];
                    outputIndex++;
                    stackTop--;
                }
                output[outputIndex].type = OPERATOR;
                output[outputIndex].value.operator = 'E';
                outputIndex++;
                return output;
                break;

            case S_ERROR:
            default:
                printf("Input function error!!!\n");
                return NULL;
        }
    }
}

float evaluatePostfix(Token *postfix, float x_value) {
    int counter = 0;
    float stack[100];
    float result = 0.0;
    int stackPointer = 0; //-1 mean stack empty;
    while (postfix->type != OPERATOR || postfix->value.operator != 'E'){
        if(postfix->type == OPERAND){
            stack[stackPointer] = postfix->value.operand;
            stackPointer += 1;
            if(stackPointer == 100){
                printf("Stack overflow");
            }
        } else if(postfix->type == OPERATOR){
            float tempOp1, tempOp2, tempResult;
            tempOp1 = stack[stackPointer-1];
            stackPointer --;
            tempOp2 = stack[stackPointer-1];
            stackPointer --;
            if(stackPointer < 0) printf("Stack Underflow");
            if(postfix->value.operator == '^'){
                tempResult = pow(tempOp2, tempOp1);
            } else if(postfix->value.operator == '*'){
                tempResult = tempOp2*tempOp1;
            } else if(postfix->value.operator == '/'){
                tempResult = tempOp2/tempOp1;
            } else if(postfix->value.operator == '+'){
                tempResult = tempOp2+tempOp1;
            } else if(postfix->value.operator == '-'){
                tempResult = tempOp2-tempOp1;
            }
            stack[stackPointer] = tempResult;
            stackPointer ++;
        } else if(postfix->type == VARIABLE){
            postfix->value.variable = x_value;
            stack[stackPointer] = postfix->value.variable;
            stackPointer += 1;
            if(stackPointer == 100){
                printf("Stack overflow");
            }
        }
        postfix ++;
    }
    return stack[0];
}

float findx(Token *postfix){
    float result = -1.2;
    float newResult = -1.2;
    int counter = 0;
    float coff = 1.0;
    float oldFx = 0;
    while ((evaluatePostfix(postfix, result) >= 1E-11) || ((evaluatePostfix(postfix, result) <= -1E-11))){
        float h = 0.00001;
        if(abs(result)*1E-3 > 0.00001){
            h = result*1E-5;
        }
        float diff = (-evaluatePostfix(postfix, result + 2*h) + 8*evaluatePostfix(postfix, result + h) 
                    - 8*evaluatePostfix(postfix, result - h) + evaluatePostfix(postfix, result - 2*h)) / (12*h);

        newResult = result - ((evaluatePostfix(postfix, result) / diff)/coff);
        //printf("Next x= %f, f(x) = %f\n", newResult, evaluatePostfix(postfix, newResult));
        result = newResult;
    }
    result = newResult;
    //printf("[STOP] Newton\n");
    //printf("[Return Newton] = %f\n", result);
    return result;
}

float findxByChiaDoi(Token *postfix){
    float array[100];
    float result = 0.0;
    for (int i = 0; i<100; i++){
        array[i] = evaluatePostfix(postfix, i-50);
    }
    float ng1, ng2;
    for (int i = 1; i<100; i++){
        if(array[i]*array[0] <= 0){
            ng1 = i-1-50;
            ng2 = i -50;
            break;
        }
        //printf("i= %d, f = %f\n", i, array[i]);
        if(i == 99){
            printf("can not find space change signed !\n");
        }
    }
    float temp = 0;
    for(int i = 0; i < 1000; i++){
        float average = (ng1 + ng2) / 2;
        if(evaluatePostfix(postfix, ng1) == 0){
            result = ng1;
            break;
        } else if(evaluatePostfix(postfix, ng2) == 0){
            result = ng2;
            break;
        }
        if(evaluatePostfix(postfix, average) * evaluatePostfix(postfix, ng1) >= 0){
            ng1 = average;
        } else {
            ng2 = average;
        }
        //printf("ng1: %f with f = %f, ng2: %f with f= %f\n", ng1, evaluatePostfix(postfix, ng1), ng2, evaluatePostfix(postfix, ng2));
        result = average;
        //printf("result: %f", result);
    }
    //printf("[result]: %f", result);
    return result;
}

void *findNewton(void *arg){
    ThreadData *data = (ThreadData *)arg;
    data->result = findx(data->postfix);
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

void *findChiaDoi(void *arg){
    ThreadData *data = (ThreadData *)arg;
    data->result = findxByChiaDoi(data->postfix);
    pthread_mutex_lock(&mutex);
    if (!found)
    {
        best_result = data->result;
        found = 1;
        printf("Chia doi tim duoc nghiem la: %f\n", best_result);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void printTokens(Token *output) {
    int i = 0;
    printf("Output Tokens: ");
    while (output[i].type != OPERATOR || output[i].value.operator != 'E') {
        if (output[i].type == OPERAND) {
            printf("%.2f ", output[i].value.operand);
        } else if (output[i].type == OPERATOR) {
            printf("%c ", output[i].value.operator);
        } else if (output[i].type == VARIABLE) {
            printf("x ");
        }
        i++;
    }
    printf("\n");
}

char *preProcess(char *myString){
    char *result = (char *) malloc(sizeof(char) * (100));
    char *tempResult = result;
    char tempChar = ' ';
    int haveRightSide = 0;
    while (*myString != 0){
        //printf("%c \n", *myString);
        if(*myString == 'x'){
            if(tempChar <= 0x39 && tempChar >= 0x30){
                *tempResult = '*';
                tempResult ++;
                *tempResult = *myString;
            }else{
                *tempResult = *myString;
            }
        }else if(*myString == '='){
            haveRightSide = 1;
            *tempResult = '-';
            tempResult ++;
            *tempResult = '(';
        }else {
            *tempResult = *myString;
        }
        tempChar = *myString;
        myString ++;
        tempResult ++;
    }
    if(haveRightSide){
        *tempResult = ')';
        tempResult ++;
        *tempResult = 0;
    }else{
        *tempResult = 0;
    }
    tempResult ++;
    return result;
}

int main(){
    struct timespec start, end;
    Token *output;
    float x;
    char prStr[MAX];
    char *str;
    printf("Enter your equal (ex: 2.3x^2-6*x-8): ");
    fgets(prStr, MAX, stdin);
    prStr[strcspn(prStr, "\n")] = 0;
    str = preProcess(prStr);
    //========================
    //
    output = infixToPostfix(str);
    //printf("Enter X value: ");
    //scanf("%f", &x);
    /*
    if (output != NULL) {
        printTokens(output);
        float result = evaluatePostfix(output, x);
        printf("Value of equal if x = %.2f is: %.2f\n", x, result);
    } */
    if(output != NULL){
        pthread_t threads[NUM_THREADS];
        ThreadData threadData[NUM_THREADS];
        clock_gettime(CLOCK_MONOTONIC, &start);
        threadData[0].postfix = output;
        pthread_create(&threads[0], NULL, findNewton, (void *)&threadData[0]);

        threadData[1].postfix = output;
        pthread_create(&threads[1], NULL, findChiaDoi, (void *)&threadData[1]);      //Tạo thread mới cho findrootSecant với đầu vào là struct threadData[1]
        while (1){
            pthread_mutex_lock(&mutex);
            if(found == 1){
                for (int i = 0; i < NUM_THREADS; i++)
                {
                    pthread_cancel(threads[i]);
                }
                break;
            }
            pthread_mutex_unlock(&mutex);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        
        printf("Time to find x is: %f second\n", elapsed);
        printf("Result is:  %.2f and F(x = %.2f) = %.3f\n", best_result, best_result, evaluatePostfix(output, best_result));
    }
    /*
    float Ng = 0.0;
    Ng = findx(output);
    if(evaluatePostfix(output, Ng) < 0){
        printf("F(x) = 0 => by Newton x = %f, double check: f(x=%f)= %f, error is: %f (10e-10)\n", Ng, Ng, -evaluatePostfix(output, Ng), evaluatePostfix(output, Ng)*10000000000);
    }else{
        printf("F(x) = 0 => by NewTon x = %f, double check: f(x=%f)= %f, error is: %f (10e-10)\n", Ng, Ng, evaluatePostfix(output, Ng), evaluatePostfix(output, Ng)*10000000000);
    }
    float ngchiadoi = findxByChiaDoi(output);
    if(evaluatePostfix(output, ngchiadoi) < 0){
        printf("F(x) = 0 => by Chia doi x = %f, double check: f(x=%f)= %f, error is: %f (10e-10)\n", ngchiadoi, ngchiadoi, -evaluatePostfix(output, ngchiadoi), evaluatePostfix(output, ngchiadoi)*10000000000);
    }else{
        printf("F(x) = 0 => by Chia doi x = %f, double check: f(x=%f)= %f, error is: %f (10e-10)\n", ngchiadoi, ngchiadoi, evaluatePostfix(output, ngchiadoi), evaluatePostfix(output, ngchiadoi)*10000000000);
    }*/
    return 0;
}