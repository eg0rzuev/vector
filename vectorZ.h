//
// Created by egor on 3/9/19.
//

#ifndef VECTORZUI_VECTORZ_H
#define VECTORZUI_VECTORZ_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <string> 

#define NEW    new   (__func__, __LINE__)
#define DELETE_BR(p)                                                                                 \
    do{                                                                                             \
        fout << "Delete[] called in function " << __func__ << ", line is " << __LINE__ << " \n";    \
        delete[] p					                                       ;    \
    }while(0)	    

#define DELETE(p)                                                                                 \
    do{                                                                                           \
        fout << "Delete called in function " << __func__ << ", line is " << __LINE__ << " \n";    \
        delete p					                                     ;    \
    }while(0)											   
   

#define CHECK_ERR(CONDITION, ERROR)  \
    do{                              \
        if((CONDITION))              \
        {                            \
            error_ |= ((ERROR));     \
            return (ERROR);          \
        }                            \
    }while (0)

static std::ofstream fout("Log.txt");
enum PARAMS
{
    DEFAULT_SIZE = 10,
    MULTIPLIER   = 2,
    BIG_DIFFER   = 100,
    BRACK_USED   = 1 << 7 //128
};
enum ERRORS
{
    VECTOR_UNDERFLOW    = 1 << 0, //1
    VECTOR_OVERFLOW     = 1 << 1, //2
    VECTOR_OUTTA_MEMORY = 1 << 2, //4
    BIRD_DEAD           = 1 << 3, //8
    NEGATIVE_ELEM_NUM   = 1 << 4, //16
    NEGATIVE_CAPACITY   = 1 << 5, //32
    VECTOR_WRONG_HASH   = 1 << 6  //64

};

enum BIRDS
{
    CANARY_ONE = 282,
    CANARY_TWO = 202
};
typedef int vctrElemT;
class Vector
{
    public:
        Vector      ();
        Vector      (size_t);
        Vector      (const Vector&);
	Vector      (Vector&&);
        ~Vector     ();

        int               push                (vctrElemT);
        const vctrElemT         pop                 ();
        const vctrElemT         peek                ();

        vctrElemT&             operator []   (size_t);
        const vctrElemT&       operator []   (const  size_t)  const;
        Vector&                operator =    (const Vector&);
	void                   operator =    (      Vector&&);
        bool                   operator ==   (const Vector&) const;
        Vector&                operator +=   (const Vector&);
        Vector                 operator +    (const Vector&);
	Vector&  	       operator *=   (const Vector&);
	Vector		       operator *    (const Vector&);	
    private:
        size_t              elemNum_;
        size_t              capacity_;


        int                 canary1_;
        vctrElemT*          data_;
        int                 canary2_;

        int                 error_;
        uint64_t            hash_;
        uint64_t            Hash        ();
        int                 VectorOk    ();
        int                 resize      ();
        int                 resizeDown  ();
        void                swap        (Vector&);
        int                 dump        ();

};
    void* operator new     (size_t size, const char* func, int line);
    void* operator new[]   (size_t size, const char* func, int line);
    void  operator delete  (void* p) noexcept;
    void  operator delete[](void* p) noexcept;
#endif //VECTORZUI_VECTORZ_H
