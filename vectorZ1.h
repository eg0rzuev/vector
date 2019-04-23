//
// Created by egor on 3/9/19.
// todo read about class and typename difference

#ifndef VECTORZUI_VECTORZ_H
#define VECTORZUI_VECTORZ_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <string> 

#define NEW    new   (__func__, __LINE__)
#define DELETE_BR(p)                                                                                \
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

template <class T> class Vector
{
    public:
        Vector      ();
        Vector      (size_t);
        Vector      (const Vector&);
	Vector      (Vector&&);
        ~Vector     ();

        int                     push                (T);
        const T         pop                 ();
        const T         peek                ();

        T&             operator []   (size_t);
        const T&       operator []   (const  size_t)  const;
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
        T*                  data_;
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


void* operator new[](size_t size, const char* func,  int line)
{
    void* ptr = calloc(1, size);
    fout << "New[] called in function "<< func << ", line is " << line << " \n";
    return ptr;
}

void* operator new(size_t size, const char* func,  int line)
{
    void* ptr = calloc(1, size);
    fout << "New called in function "<< func << ", line is " << line << " \n";
    return ptr;
}

void operator delete(void* p)   noexcept
{
    free(p);
}
void operator delete[](void* p) noexcept
{
    free(p);
}
          
void operator delete[](void* p, const char* msg)
{
	fout << msg;
	free(p);
}

template<class T> 
Vector<T>::Vector():
    elemNum_        (0),
    capacity_       (0),
    canary1_        (CANARY_ONE),
    data_           (nullptr),
    canary2_        (CANARY_TWO),
    error_          (VECTOR_UNDERFLOW),
    hash_           (0)
    {}

template<class T>
Vector<T>::Vector(size_t size):
    elemNum_        (0),
    capacity_       (size),
    canary1_        (CANARY_ONE),
    canary2_        (CANARY_TWO),
    error_          (0),
    hash_           (0)

    {
        data_ = NEW T[size];
        if(data_ == nullptr)
        {
            error_ |= VECTOR_OUTTA_MEMORY;
        }
    }

template <class T>
Vector<T>::Vector(const Vector& rhv):
    elemNum_        (rhv.elemNum_),
    capacity_       (rhv.capacity_),
    canary1_        (rhv.canary1_),
    canary2_        (rhv.canary2_),
    error_          (rhv.error_)

   {
   data_ = NEW T[rhv.capacity_];

        if(data_ == nullptr)
        {
            error_ |= VECTOR_OUTTA_MEMORY;
        }

        for(int i = 0; i <(int)rhv.elemNum_; i++)
        {
            data_[i] = rhv.data_[i];
        }
        hash_ = Hash();
     //   fout << "Copy constructor was called!\n";
    }

template <class T>
Vector<T>::Vector(Vector<T>&& rhv):
    elemNum_        (rhv.elemNum_),
    capacity_       (rhv.capacity_),
    canary1_        (rhv.canary1_),
    data_           (rhv.data_),		
    canary2_        (rhv.canary2_),
    error_          (rhv.error_)

    {
		   
	rhv.data_ = nullptr;
        fout << "MOVE constructor was called!\n";
    }


template <class T>
Vector<T>::~Vector()
{
    elemNum_   = 0;
    capacity_  = 0;
    canary1_   = BIRD_DEAD;
    canary2_   = BIRD_DEAD;
    error_     = VECTOR_OUTTA_MEMORY;
    hash_      = 0;
    DELETE_BR (data_) ;
}

template <class T>
void Vector<T>::swap(Vector& rhv)
{
    std::swap(elemNum_ , rhv.elemNum_);
    std::swap(capacity_, rhv.capacity_);
    std::swap(data_    , rhv.data_);
}

template <class T>
Vector<T>& Vector<T>::operator = (const Vector& rhv)
{
    Vector  shortLive(rhv);
    swap   (shortLive);
    hash_ = Hash();
    return  *this;
}
template <class T>
void Vector<T>::operator = (Vector<T>&& rhv)
{
    fout << "MOVE operator used!\n";	
    swap(rhv);
    hash_ = Hash();
}    

template <class T>
bool Vector<T>::operator == (const Vector<T>& rhv) const
{
    if(rhv.elemNum_ != this -> elemNum_)
    {
        return false;
    }

    for (int i = 0; i < (int)elemNum_; i++)
    {
        if(rhv.data_[i] != this -> data_[i])
            return false;
    }
    return true;
}

template <class T>
Vector<T>& Vector<T>::operator += (const Vector<T>& rhv)
{
    if(this -> elemNum_ == rhv.elemNum_)
    {	    
    	for(int i = 0; i < (int)(elemNum_); i++)
    	{
            data_[i] += rhv.data_[i];
        }
    }	
    else
    {
    	std::cout << "Vectors are not equal! Cannot do += or + operation! *this is returned" << std::endl;
	return *this;
    }	    
    hash_ = Hash();
    return *this;
}

template <class T>
Vector<T>& Vector<T>::operator *= (const Vector<T>& rhv)
{
    if(this -> elemNum_ == rhv.elemNum_)
    {
    	for(size_t i = 0; i < elemNum_; i++)
	{
            data_[i] *= rhv.data_[i];
	}		
    }
    else
    {
    	std::cout << "Vectors are not equal! Cannot do *= or * operation! *this is returned" << std::endl;
	return *this;
    }	
    hash_ = Hash();
    return *this;
}

template <class T>
Vector<T> Vector<T>::operator + (const Vector<T>& rhv)
{
    Vector result(rhv);
    result += *this;
    return result;
}

template <class T>
Vector<T> Vector<T>::operator * (const Vector<T>& rhv)
{
    Vector result(rhv);
    result *= *this;
    return result;
}

template <class T>
T& Vector<T>::operator [] (size_t index)
{
    if(index >= 0 && index <= elemNum_)
    {
        error_ |= BRACK_USED;
        return data_[index];                    //QUESTION: How can [] = live happy together if hash_ is not changed after it?
    }
    else 
    {
        std::cout << " INDEX " << index << " IS NOT ALLOWED: ";
        if(index < 0)
        {
            std::cout << "NEGATIVE INDEX" << std::endl;
        }
        else if (index > elemNum_)
        {
	    std::cout << elemNum_ << std::endl;		
            std::cout << "NOT ENOUGH ELEMENTS IN VECTOR" << std::endl;
        }
        return error_;
    }
}

template <class T>
const T& Vector<T>::operator[] (const size_t index) const
{
    if(index >= 0 && index < elemNum_)
    {
        return data_[index];
    }
    else
    {
        std::cout << " INDEX " << index << " IS NOT ALLOWED " << std::endl;
        return error_;
    }
}

template <class T>
int Vector<T>::push(T element)
{

    if(capacity_ <= elemNum_)
    {
        resize();
        hash_ = Hash();
    }	
    if(VectorOk() > VECTOR_UNDERFLOW)
    {
        std::cout << "push() ERROR OCCURED" << std::endl;
        this -> dump();
        return error_;
    }
    data_[elemNum_] = element;
    elemNum_++;
    hash_ = Hash();
    return 0;
}

template <class T>
const T Vector<T>::pop()
{
    //std::cout << "New hash is " << hash_ << std::endl;
    if(VectorOk() != VECTOR_OVERFLOW && VectorOk() != 0)
    {
        std::cout << "pop() ERROR OCCURRED" << std::endl;
        this -> dump();
        //hash_ = Hash();
        //return  error_; QUESTION:WHAT TO DO HERE?//throw something here
    }
    if(elemNum_ + BIG_DIFFER < capacity_ )
    {
        resizeDown();
        std::cout << "capacity_ is " << capacity_ << std::endl;
    }
    elemNum_--;
    hash_ = Hash();
    return data_[elemNum_];
}

template <class T>
const T Vector<T>::peek()
{
    if(elemNum_ + BIG_DIFFER < capacity_ )
    {
        resizeDown();
        std::cout << "capacity_ is " << capacity_ << std::endl;
    }

    if(VectorOk() != VECTOR_OVERFLOW && VectorOk() != 0)
    {    
	std::cout << "peek() ERROR OCCURRED" << std::endl;
        this -> dump();

        //hash_ = Hash();
        //return  error_; 
    }
    hash_ = Hash();
    return data_[elemNum_];
}

template <class T>
int Vector<T>::resize()
{

    T* temp         = data_;
    data_           = NEW T[DEFAULT_SIZE + capacity_ * MULTIPLIER];
    capacity_       = DEFAULT_SIZE + capacity_ * MULTIPLIER;
    for(size_t  i = 0; i < elemNum_; i++)
    {
        data_[i] = temp[i];
    }

    DELETE_BR (temp);
    hash_ = Hash();
    return 0;
}

template <class T>
int Vector<T>::resizeDown()
{
    T* temp         = data_;
    data_           = NEW T[capacity_ - BIG_DIFFER/2];
    capacity_      -= BIG_DIFFER/2;
    for(size_t i = 0; i < elemNum_; i++)
    {
        data_[i] = temp[i];
    }
    DELETE_BR (temp);
    hash_ = Hash();
    return 0;
}

template <class T>
int Vector<T>::VectorOk()
{

    CHECK_ERR(canary1_  != CANARY_ONE || canary2_ != CANARY_TWO, BIRD_DEAD);

    CHECK_ERR(hash_     != Hash() && error_ < BRACK_USED , VECTOR_WRONG_HASH);

    CHECK_ERR(elemNum_  == 0       , VECTOR_UNDERFLOW);

    CHECK_ERR(elemNum_  <  0       , NEGATIVE_ELEM_NUM);

    CHECK_ERR(capacity_ <= elemNum_, VECTOR_OVERFLOW);

    CHECK_ERR(capacity_ < 0        , NEGATIVE_CAPACITY);

    CHECK_ERR(data_     == nullptr , VECTOR_OUTTA_MEMORY);

    if (error_ >= BRACK_USED)
        error_ -= BRACK_USED;

    return 0;
}

template <class T>
uint64_t  Vector<T>::Hash()
{
    uint64_t hash = 0;

    for(size_t i = 0; i < elemNum_; i++)
    {
       	//hash |= (uint64_t)(data_[i]) * (uint64_t)(data_[i + 1]); //QUESTION: it does not want to transform smth to int 
	hash += (uint64_t)&data_[i];
    }
    
    return hash;
}

template <class T>
int Vector<T>::dump()
{
    using std::cout;
    using std::endl;

    cout << endl;
    cout << "*_* ''''''_____VECTOR_DUMP_____'''''' *_*" << endl;
    cout << endl;

    cout << "  STATE     = ";

    if(this -> VectorOk() > 0)
    {
        cout << "INVALID" << endl;
    }
    else
    {
        cout <<   "OK!"    << endl;
    }

    cout << "  error_    = " << error_    << endl;
    cout << "  elemNum_  = " << elemNum_  << endl;
    cout << "  capacity_ = " << capacity_ << endl;
    cout << "  hash_     = " << hash_     << endl;
    cout << "  canary1_  = " << canary1_  << endl;
    cout << "  canary2_  = " << canary2_  << endl;

    cout << endl;

    for (size_t i = 0; i < elemNum_; i++)
    {
        cout <<  "  data_[" << i << "] = " << "data_[" << &data_[i] << "] = " << this -> data_[i] << endl;
    }

    cout << "........................................" << endl;
    cout << endl;
    return error_;
}

#endif //VECTORZUI_VECTORZ_H
