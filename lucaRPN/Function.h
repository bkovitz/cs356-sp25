#ifndef FUNCTION_H
#define FUNCTION_H

class calcObj {

    private: 
    int calcVal;

    public:
    calcObj();

    void add(int toBeAdded);

    void sub(int toBeSubtracted);

    int getCalcVal();

};

int addOne(int toBeAddedTo) {
    return ++toBeAddedTo;
}

#endif