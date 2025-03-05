#include <iostream>
#include <cstdlib>
#include <time.h>

int main(){
    srand(time(NULL));
    int arraySize = 10;
    int life[arraySize][arraySize];

    for(int i = 0; i < arraySize; i++) {
        for(int j = 0; j < arraySize ; j++){
            life[i][j] = (rand() % 2);
        }
    }
    for(int i = 0; i < arraySize; i++) {
        for(int j = 0; j < arraySize ; j++){
          std::cout << life[i][j] << " ";
        }
        std::cout << std::endl;
    }


    return 0;
}