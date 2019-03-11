//
// Created by egor on 3/9/19.
//

#ifndef VECTORZUI_VECTORZ_H
#define VECTORZUI_VECTORZ_H
#include <iostream>

// use brackets with params in macro


// why do you write like
error_ = ERROR;
// ????

// if you have your ERORRS enum using binary shifts?
// then why do you use binary shifts?
// think about It, bro

#define CHECK_ERR(CONDITION, ERROR)  \
    do{                              \
        if(CONDITION)                \
        {                            \
            error_ = ERROR;          \
            return error_;           \
        }                            \
    }while (0)

enum PARAMS
{
    DEFAULT_SIZE = 10,
    MULTIPLIER   = 2
};
enum ERRORS
{
    VECTOR_UNDERFLOW    = 1 << 0,
    VECTOR_OVERFLOW     = 1 << 1,
    VECTOR_OUTTA_MEMORY = 1 << 2,
    BIRD_DEAD           = 1 << 3,
    NEGATIVE_ELEM_NUM   = 1 << 4,
    NEGATIVE_CAPACITY   = 1 << 5,
    VECTOR_WRONG_HASH   = 1 << 6
};

// interesting Constants

enum BIRDS
{
    CANARY_ONE = 282,
    CANARY_TWO = 202
};
typedef int vctrElemT;

class Vector
{
    public:
        Vector();
        Vector(size_t);
        Vector(const Vector&);
        ~Vector();

    
    //----------------------------------------------------------------
    
        int push(vctrElemT);
        int pop();
        int peek();

    //----------------------------------------------------------------
    
        vctrElemT& operator [] (size_t);
        Vector& operator = (Vector&);
        // too few operators
        // you have to override ~8 operators

    private:
        size_t elemNum_;
        size_t capacity_;


    //----------------------------------------------------------------
        int canary1_;
        vctrElemT* data_;
        int canary2_;
    
    //----------------------------------------------------------------

        int error_;
        unsigned int hash_;
    
    //----------------------------------------------------------------
        unsigned int    Hash();
        int             VectorOk();
        int             resize();
        void            swap(Vector&);
        int             dump();

};
#endif //VECTORZUI_VECTORZ_H
