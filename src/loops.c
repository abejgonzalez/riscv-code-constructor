#include <stdio.h>

#define AMT_LOOPS 1000

int temp (int j){
    ++j;
    return j;
}

int main (void){
    setStats(1); // read hpm's
    int j = 0;
    for (int i = 0; i < AMT_LOOPS; i++) {
        temp(j);
    }
    setStats(0);
}
