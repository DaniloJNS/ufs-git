#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::swap;
using std::pair;
using std::make_pair;

int main(){

    int casos, acumulador, temp;
    int case1=0, case2=0, case3=0;

    scanf("%d", &casos);
    temp = casos;
    while(temp--){
        cin >> acumulador;
        case1 += acumulador;
    }
    temp = casos-1;
    while(temp--){
        cin >> acumulador;
        case2 += acumulador;
    }
    temp = casos-2;
    while(temp--){
        cin >> acumulador;
        case3 += acumulador;
    }
    printf("%d\n", abs(case1-case2));
    printf("%d\n", abs(case2-case3));
    
    return 0;
}