/* 空间配置器std::alloc */
/**
 * 一般的方式是，请求对应的内存，然后进行构造，进行析构，最后再回收内存
 */
//使用placement new进行内存分配
#include <new>
template<class T1,class T2>
inline void construct(T1* p,const T2& value)
{
    new (p)T1(value);
};

template<class T,class Alloc>
class simple_alloc{
public:
    static T* allocate(size_t n)
    {
        return 0 == n ? 0:(T*)Alloc::allocate(n*sizoef(T));
    };
    
    static T* allocate(void)
    {
        return (T*)Alloc::allocate(sizeof(T));
    }

    static void deallocate(T* p,size_t n)
    {
        if(n!=0)
            Alloc::deallocate(p,n*sizoef(T));
    }

    static void deallocate(T* p)
    {
        Alloc::deallocate(p,sizeof(T));
    }
};