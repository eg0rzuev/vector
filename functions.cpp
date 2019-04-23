//
// Created by egor on 3/9/19.
//
#include "vectorZ.h"
void* operator new[](size_t size, const char* func,  int line)
{
    void* ptr = calloc(1, size);
    fout << "New[] called in function "<< func << ", line is " << line << " \n";
   // printf("function is %s", __func__);
    return ptr;
}

void* operator new(size_t size, const char* func,  int line)
{
    void* ptr = calloc(1, size);
    fout << "New called in function "<< func << ", line is " << line << " \n";
   // printf("function is %s", __func__);
    return ptr;
}

void operator delete(void* p)   noexcept
{
    free(p);
}
void operator delete[](void* p) noexcept
{
    free(p);
   // fout << "Delete called in function "<< func << ", line is " << line << " \n";
}
          
void operator delete[](void* p, const char* msg)
{
	fout << msg;
	free(p);
}

// operator new(),[] delete(),[], + , *=, move semantics
 Vector::Vector():
    elemNum_        (0),
    capacity_       (0),
    canary1_        (CANARY_ONE),
    data_           (nullptr),
    canary2_        (CANARY_TWO),
    error_          (VECTOR_UNDERFLOW),
    hash_           (0)
    {
   //     fout << "Deafult constroctor called\n";
    }

Vector::Vector(size_t size):
    elemNum_        (0),
    capacity_       (size),
    canary1_        (CANARY_ONE),
    canary2_        (CANARY_TWO),
    error_          (0),
    hash_           (0)

    {
        data_ = NEW vctrElemT[size];
        if(data_ == nullptr)
        {
            error_ |= VECTOR_OUTTA_MEMORY;
        }
//	fout << "Size constructor was called\n";
    }

Vector::Vector(const Vector& rhv):
    elemNum_        (rhv.elemNum_),
    capacity_       (rhv.capacity_),
    canary1_        (rhv.canary1_),
    canary2_        (rhv.canary2_),
    error_          (rhv.error_)

   {
   data_ = NEW vctrElemT[rhv.capacity_];

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
Vector::Vector(Vector&& rhv):
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

    
Vector::~Vector()
{
    elemNum_   = 0;
    capacity_  = 0;
    canary1_   = BIRD_DEAD;
    canary2_   = BIRD_DEAD;
    error_     = VECTOR_OUTTA_MEMORY;
    hash_      = 0;
    DELETE_BR (data_) ;
}

void Vector::swap(Vector& rhv)
{
    std::swap(elemNum_ , rhv.elemNum_);
    std::swap(capacity_, rhv.capacity_);
    std::swap(data_    , rhv.data_);
}

Vector& Vector::operator = (const Vector& rhv)
{
    Vector  shortLive(rhv);
    swap   (shortLive);
    hash_ = Hash();
    return  *this;
}
void Vector::operator = (Vector&& rhv)
{
    fout << "MOVE operator used!\n";	
    swap(rhv);
    hash_ = Hash();
}    
bool Vector::operator == (const Vector& rhv) const
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

Vector& Vector::operator += (const Vector& rhv)
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

Vector& Vector::operator *= (const Vector& rhv)
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

Vector Vector::operator + (const Vector& rhv)
{
    Vector result(rhv);
    result += *this;
    return result;
}

Vector Vector::operator * (const Vector& rhv)
{
    Vector result(rhv);
    result *= *this;
    return result;
}

vctrElemT& Vector::operator [] (size_t index)
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

const vctrElemT& Vector::operator[] (const size_t index) const
{
    if(index >= 0 && index < elemNum_)
    {
        //QUESTION where do I use Hash() here?
        return data_[index];
    }
    else
    {
        std::cout << " INDEX " << index << " IS NOT ALLOWED " << std::endl;
        return error_;
    }
}

int Vector::push(vctrElemT element)
{

    if(capacity_ <= elemNum_)
    {
        resize();
        hash_ = Hash();
        //dump();
    }
   // std::cout << "hash_ is " << hash_ << std::endl;
   // std::cout << "new hash is " << Hash() << std::endl;
    //hash_ = Hash();
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

const vctrElemT Vector::pop()
{
    //std::cout << "New hash is " << hash_ << std::endl;
    if(VectorOk() != VECTOR_OVERFLOW && VectorOk() != 0)
    {
        std::cout << "pop() ERROR OCCURRED" << std::endl;
        this -> dump();
        //hash_ = Hash();
        return  error_;
    }
    if(elemNum_ + BIG_DIFFER < capacity_ )
    {
        resizeDown();
        std::cout << "capacity_ is " << capacity_ << std::endl;
    }
    elemNum_--;
    hash_ = Hash();

    //std::cout << "HASH is " << hash_ << std::endl;
    return data_[elemNum_];
}

const vctrElemT Vector::peek()
{
    if(elemNum_ + BIG_DIFFER < capacity_ )
    {
        resizeDown();
        std::cout << "capacity_ is " << capacity_ << std::endl;
    }

    if(VectorOk() != VECTOR_OVERFLOW && VectorOk() != 0)
    {    
	std::cout << "pop() ERROR OCCURRED" << std::endl;
        this -> dump();
        //hash_ = Hash();
        return  error_;
    }
    hash_ = Hash();
    return data_[elemNum_];
}

int Vector::resize()
{

    vctrElemT* temp = data_;
    data_           = NEW vctrElemT[DEFAULT_SIZE + capacity_ * MULTIPLIER];
    capacity_       = DEFAULT_SIZE + capacity_ * MULTIPLIER;
    for(int i = 0; i < (int)elemNum_; i++)
    {
        data_[i] = temp[i];
    }

    DELETE_BR (temp);
    hash_ = Hash();
    return 0;
}

int Vector::resizeDown()
{
    vctrElemT* temp = data_;
    data_           = NEW vctrElemT[capacity_ - BIG_DIFFER/2];
    capacity_      -= BIG_DIFFER/2;
    for(int i = 0; i < (int)elemNum_; i++)
    {
        data_[i] = temp[i];
    }
    DELETE_BR (temp);
    hash_ = Hash();
    return 0;
}

int Vector::VectorOk()
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
// replace long long to u_int64t 
uint64_t  Vector::Hash()
{
    uint64_t hash = 0;

    for(uint64_t i = 0; i < (uint64_t)elemNum_; i++)
    {
        hash += (uint64_t)(data_[i]) * (uint64_t)(data_[i + 1]);

	    hash += (uint64_t)&data_[i];
    }

    return hash;
}



int Vector::dump()
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

    //cout << "  line is     " << __LINE__  << endl;
    cout << "  error_    = " << error_    << endl;
    cout << "  elemNum_  = " << elemNum_  << endl;
    cout << "  capacity_ = " << capacity_ << endl;
    cout << "  hash_     = " << hash_     << endl;
    cout << "  canary1_  = " << canary1_  << endl;
    cout << "  canary2_  = " << canary2_  << endl;

    cout << endl;

    for (int i = 0; i < (int)elemNum_; i++)
    {
        cout <<  "  data_[" << i << "] = " << "data_[" << &data_[i] << "] = " << this -> data_[i] << endl;
    }

    cout << "........................................" << endl;
    cout << endl;
    return error_;
}

/*void* operator new[](size_t size, const char* msg,  int line)
{
    void* ptr = calloc(1, size);
    fout << "new called " << "comment is " << msg << "line is" << line<< " \n";
    printf("function is %s", msg);
    return ptr;
}
*/

