#include <stdio.h>
#include <math.h>

double coff[100];
double power[100];
int pointerCoff = 0;
int pointerPower = 0;
void call(char string[]){
    double result = 0.0;
    int counter = -1;
    char tempInt[100] = "";
    int counterTempInt = 0;
    char tempFloat[100] = "";
    int counterTempFloat = 0; 
    int signedFunction = 0;
    int error = 0;
    while (1){
        if(counter != -1){
            if(string[counter] == '-'){
                signedFunction = 1;
            }else if(string[counter] == '+'){
                signedFunction = 0;
            }
        }
        counter += 1;
        if (string[counter] == ' ') break;
        if (string[counter] == '-'){
            signedFunction = 1;
        }
        if (string[counter] <= 0x39 && string[counter] >= 0x30){
            int lengNumberInt = 0;
            int lengNumberFloat = 0;
            counterTempInt = 0;
            counterTempFloat = 0;
            while(1){
                tempInt[counterTempInt] = string[counter];
                if(string[counter] <= 0x39 && string[counter] >= 0x30){
                    counterTempInt += 1;
                    lengNumberInt += 1;
                }else{
                    break;
                }
                counter += 1;
            }
            if(string[counter] == '.'){
                counter += 1;  //pass '.' symbol
                while(1){
                    tempFloat[counterTempFloat] = string[counter];
                    if(string[counter] <= 0x39 && string[counter] >= 0x30){
                        counterTempFloat += 1;
                        lengNumberFloat += 1;
                    }else{
                        break;
                    }
                    counter += 1;
                }
            }
            if(string[counter] == ' '){
                double numTemp = 0.0;
                if(lengNumberInt > 0){
                    for(int i = 0; i < lengNumberInt; i++){
                        numTemp += 1.0*((tempInt[i]-0x30)*pow(10, lengNumberInt-1-i));
                    }
                }
                if(lengNumberFloat > 0){
                    for(int i = 0; i < lengNumberFloat; i++){
                        numTemp += 1.0*((tempFloat[i]-0x30)*pow(10, -(i+1)));
                    }
                }
                if(signedFunction){
                    coff[pointerCoff] = -numTemp;
                    pointerCoff += 1;
                    printf("add coff: %f\n",-numTemp);
                } else {
                    coff[pointerCoff] = numTemp;
                    pointerCoff += 1;
                    printf("add coff: %f\n",numTemp);
                }
                power[pointerPower] = 0;
                pointerPower += 1;
                counter += 1;
            }
            if(string[counter] == 'x'){
                double numTemp = 0.0;
                if(lengNumberInt > 0){
                    for(int i = 0; i < lengNumberInt; i++){
                        numTemp += 1.0*((tempInt[i]-0x30)*pow(10, lengNumberInt-1-i));
                    }
                }
                if(lengNumberFloat > 0){
                    for(int i = 0; i < lengNumberFloat; i++){
                        numTemp += 1.0*((tempFloat[i]-0x30)*pow(10, -(i+1)));
                    }
                }
                if(signedFunction){
                    coff[pointerCoff] = -numTemp;
                    pointerCoff += 1;
                    printf("add coff: %f\n",-numTemp);
                } else {
                    coff[pointerCoff] = numTemp;
                    pointerCoff += 1;
                    printf("add coff: %f\n",numTemp);
                }
                counter += 1;
                if(string[counter] == '^'){
                    counterTempInt = 0;
                    lengNumberInt = 0;
                    counter += 1;
                    while(string[counter] <= 0x39 && string[counter] >= 0x30){
                        tempInt[counterTempInt] = string[counter];
                        counter += 1;
                        lengNumberInt += 1;
                        counterTempInt += 1;
                    }
                    if(lengNumberInt > 0){
                        double tempPower = 0;
                        if(lengNumberInt > 0){
                            for(int i = 0; i < lengNumberInt; i++){
                                tempPower += 1.0*((tempInt[i]-0x30)*pow(10, lengNumberInt-1-i));
                            }
                        }
                        power[pointerPower] = tempPower;
                        printf("add power: %f\n",tempPower);
                        pointerPower += 1;
                    }
                } else if (string[counter] == '+' || string[counter] == '-'){
                    power[pointerPower] = 1;
                    pointerPower += 1;
                    printf("add power: %f\n",1);
                }
            } else if(string[counter] == '+' || string[counter] == '-'){
                double numTemp = 0.0;
                if(lengNumberInt > 0){
                    for(int i = 0; i < lengNumberInt; i++){
                        numTemp += 1.0*((tempInt[i]-0x30)*pow(10, lengNumberInt-1-i));
                    }
                }
                if(lengNumberFloat > 0){
                    for(int i = 0; i < lengNumberFloat; i++){
                        numTemp += 1.0*((tempFloat[i]-0x30)*pow(10, -(i+1)));
                    }
                }
                if(signedFunction){
                    coff[pointerCoff] = -numTemp;
                    pointerCoff += 1;
                    printf("add coff: %f\n",-numTemp);
                } else {
                    coff[pointerCoff] = numTemp;
                    pointerCoff += 1;
                    printf("add coff: %f\n",numTemp);
                }
                power[pointerPower] = 0;
                pointerPower += 1;
            }
        }
    }
}

double subb(double x){
    double result = 0.0;
    for(int i = 0; i < pointerCoff; i++){
        result += 1.0*(pow(x, power[i]))*coff[i];
        printf("x = %f, power = %f, step power: %f\n", x, power[i], pow(x, power[i]));
        printf("step: %f", result);
    }
    return result;
}

int main(){
    printf("Enter string: ");
    char string[100] = "";
    for(int i = 0; i< 100; i++){
        string[i] = ' ';
    }
    int counter = 0;
    while (1){
        scanf("%c", &string[counter]);
        if(string[counter] == ' '){
            break;
        } else {
            counter += 1;
        }
    }
    counter = 0;
    printf("String entered is: ");
    while(1){
        if(string[counter] == ' ') break;
        printf("%c", string[counter]);
        counter += 1;
    }
    printf("\n");
    call(string);
    printf("Tong he so la: %d\n", pointerCoff - 1);
    printf("Tong he so mu: %d\n", pointerPower - 1);
    printf("phan nguyen: ");
    for(int i = 0; i < pointerCoff; i ++){
        printf("%f  ", coff[i]);
    }
    printf("\nPhan thuc: ");
    for(int i = 0; i < pointerPower; i++){
        printf("%f  ", power[i]);
    }
    double x = 0.0;
    printf("nhap x= ");
    scanf("%lf", &x);
    printf("\nBan da nhap x = %f", x);
    printf("ket qua la: %f\n",subb(x));
    return 0;
}