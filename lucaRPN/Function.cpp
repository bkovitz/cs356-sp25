#include <iostream>
#include <string>
#include "Function.h"

using namespace std;

calcObj::calcObj() {
    calcVal = 0;
}

void calcObj::add(int toBeAdded) {
    calcVal += toBeAdded;
}

void calcObj::sub(int toBeSubtracted) {
    calcVal += -toBeSubtracted;
}

int calcObj::getCalcVal() {
    return calcVal;
}