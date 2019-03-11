//
// Created by egor on 3/9/19.
//
#include "vectorZ.h"

 Vector::Vector():
    elemNum_        (0),
    capacity_       (0),
    canary1_        (CANARY_ONE),
    data_           (nullptr),
    canary2_        (CANARY_TWO),
    error_          (VECTOR_UNDERFLOW),
    hash_           (0)
    {};

Vector::Vector(size_t size):
    elemNum_        (0),
    capacity_       (size),
    canary1_        (CANARY_ONE),
    canary2_        (CANARY_TWO),
    error_          (0),
    hash_           (0)

    {
        data_ = new vctrElemT[size];
        if(data_ == nullptr)
        {
            error_ = VECTOR_OUTTA_MEMORY;
        }
    };

Vector::Vector(const Vector& rhv):
    elemNum_        (rhv.elemNum_),
    capacity_       (rhv.capacity_),
    canary1_        (rhv.canary1_),//QUESTION: do I have to set canary1_ like in rhv or CANARY_ONE ?
    canary2_        (rhv.canary2_),
    error_          (rhv.error_),
    hash_           (rhv.hash_)

    {
        data_ = new vctrElemT[rhv.capacity_];

        if(data_ == nullptr)
        {
            error_ = VECTOR_OUTTA_MEMORY;
        }

        for(int i = 0; i < rhv.elemNum_; i++)//looks bad
        {
            data_[i] = rhv.data_[i];
        }
    };

Vector::~Vector()
{
    elemNum_   = 0;
    capacity_  = 0;
    canary1_   = BIRD_DEAD;
    canary2_   = BIRD_DEAD;
    error_     = VECTOR_OUTTA_MEMORY;
    hash_      = 0;
    delete       data_;
}

void Vector::swap(Vector& rhv)
{
    std::swap(capacity_, rhv.capacity_);
    std::swap(data_    , rhv.data_);
    //do I need any checks here?
}

Vector& Vector::operator= (Vector& rhv)
{
    Vector shortLive(rhv);
    swap  (shortLive);
    return *this;
}

vctrElemT& Vector::operator[] (size_t index)
{

    return data_[index];
}

int Vector::push(vctrElemT element)
{

    if(capacity_ <= elemNum_)//QUESTION do i need to resize if i.e.capacity_ is 1000 and elemNum is 10?
    {
        resize();
    }

    if(VectorOk() > VECTOR_UNDERFLOW)
    {
        std::cout << "something is wrong with push" << std::endl;
        this -> dump();
        return error_;
    }
    data_[elemNum_++] = element;
    hash_ = Hash();
    return 0;
}

int Vector::pop()
{
    if(VectorOk() == VECTOR_UNDERFLOW)
    {
        this -> dump();
        return  error_;
    }
    return data_[--elemNum_];
}

int Vector::resize()
{

    vctrElemT* temp = data_;
    data_           = new vctrElemT[DEFAULT_SIZE + capacity_ * MULTIPLIER];
    capacity_       = DEFAULT_SIZE + capacity_ * MULTIPLIER;
    for(int i = 0; i < elemNum_; i++)
    {
        data_[i] = temp[i];
    }

    delete temp;
    hash_ = Hash();
    return 0;
}

int Vector::VectorOk()
{

    CHECK_ERR(canary1_  != CANARY_ONE || canary2_ != CANARY_TWO, BIRD_DEAD);

    CHECK_ERR(elemNum_  == 0       , VECTOR_UNDERFLOW);

    CHECK_ERR(elemNum_  <  0       , NEGATIVE_ELEM_NUM);

    CHECK_ERR(capacity_ <= elemNum_, VECTOR_OVERFLOW);

    CHECK_ERR(capacity_ < 0        , NEGATIVE_CAPACITY);

    CHECK_ERR(data_     == nullptr , VECTOR_OUTTA_MEMORY);

    CHECK_ERR(hash_     != Hash()  , VECTOR_WRONG_HASH);

    return 0;
}

unsigned int Vector::Hash()
{
    unsigned int hash = 0;

    for(int i = 0; i < elemNum_; i++)
    {
        hash += data_[i];
    }

    return hash;
}

int Vector::dump()
{
    std::cout << std::endl;
    std::cout << "*_* ''''''_____VECTOR_DUMP_____'''''' *_*" << std::endl;
    std::cout << std::endl;

    std::cout << "  STATE     = ";
    if(this -> VectorOk() > 0)
    {
        std::cout << "INVALID" << std::endl;
    }
    else
    {
        std::cout <<   "OK!"    << std::endl;
    }

    std::cout << "  ERROR:      " << error_    << std::endl;
    std::cout << "  elemNum_  = " << elemNum_  << std::endl;
    std::cout << "  capacity_ = " << capacity_ << std::endl;
    std::cout << "  hash_     = " << hash_     << std::endl;
    std::cout << "  canary1_  = " << canary1_  << std::endl;
    std::cout << "  canary2_  = " << canary2_  << std::endl;

    std::cout << std::endl;

    for (int i = 0; i < elemNum_; i++)
    {
        std::cout <<  "  data_[" << i << "] = " << "data_[" << &data_[i] << "] = " << this -> data_[i] << std::endl;
    }

    std::cout << "........................................" << std::endl;
    std::cout << std::endl;
    return this -> error_;
}
