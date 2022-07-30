#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::swap;
using std::pair;
using std::make_pair;

int main(){

    string players;
    int size, i=0, counterOne=0, counterTwo=0;

    cin >> players;

    size = players.size();

    while(size--){

        if(players[i]=='1')
        {
            counterOne++;
            counterTwo=0;
        }
        else if(players[i]=='0')
        {
             counterOne=0;
             counterTwo++;
        }
        if(counterOne >= 7 || counterTwo >=7)
        {
            printf("YES\n");
            return 0;
        }
        i++;
    }
    printf("NO\n");
}