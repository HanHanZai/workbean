//iterator_traits
#include<stdlib.h>
namespace zjh
{
//iterator_category对应迭代器的类型
struct input_iterator_tag{};//前向只读迭代器
struct output_iterator_tag{};//前向只写迭代器
struct forward_iterator_tag:public input_iterator_tag{};//单向可读写迭代器
struct bidirectional_iterator_tag:public forward_iterator_tag{}; //双向迭代器
struct random_iterator_tag:public bidirectional_iterator_tag{}; //随机访问迭代器

template<class T>
struct iterator_traits{
    typedef typename T::value_type value_type;
    typedef typename T::difference_type difference_type;
    typedef typename T::pointer pointer;
    typedef typename T::reference reference;
    typedef input_iterator_tag iterator_category;
};

template<class T>
struct iterator_traits<T*>{
    typedef typename T::value_type value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef random_iterator_tag iterator_category;
};

template<class T>
struct iterator_traits<const T*>{
    typedef typename T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef random_iterator_tag iterator_category;
};

//类型萃取机
template<class T>
struct iterator_traits{
    typedef typename T::iterator_category iterator_category; //迭代器标签
    typedef typename T::value_type value_Type; //原生类型
    typedef typename T::difference_type difference_type; //距离单位 ptrdiff_t，记录两个迭代器的距离，所以可以用来当做一个容器的最大容量
    typedef typename T::pointer; //对应指针对象
    typedef typename T::reference; //对应引用对象
};

//difference_type距离单位，记录两个迭代器的距离，因此是用来表示一个容器的最大容量
template<class T,class I>
typename iterator_traits<I>::difference_type
count(I first,T last,const T& value)
{
    iterator_traits<T>::difference_type n;
    for(;first!=last;first++)
    {
        if (*first == value)
            n++;
    }
    return n;
};

//STL规范
template<class Category,class T,class Distance = ptrdiff_t,class Pointer = T*, class Reference = T&>
struct iterator
{
    /* data */
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};


//iterator完整代码
template<class Iterator>
struct iterator_traits
{
    typedef typename Iterator::iterator_category iterator_category; //迭代器标签
    typedef typename Iterator::value_type value_type; //类型名称
    typedef typename Iterator::difference_type difference_type; //距离长度
    typedef typename Iterator::pointer pointer; //指针类型
    typedef typename Iterator::reference reference; //引用类型
};

template<class T>
struct iterator_traits<T*>
{
    typedef random_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};

template<class T>
struct iterator_traits<const T*>
{
    typedef random_iterator_tag iterator_categoty;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
};

//这个函数可以很方便的决定某个迭代器的类型
template<class Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
};

//这个函数可以很方便的决定某个迭代器的distance type
template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
};

//这个函数可以很方便的决定某个迭代器的value_type
template<class Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
};

//以下是整组distance函数
template<class InputIterator>
inline iterator_traits<InputIterator>::difference_type 
__distance(InputIterator first,InputIterator last,input_iterator_tag)
{
    iterator_traits<InputIterator>::difference_type n = 0;
    while(first!=last)
        n++;
    return n;
};

template<class RandomAccessIterator>
inline iterator_traits<RandomAccessIterator>::difference_type 
__distance(RandomAccessIterator first,RandomAccessIterator last,random_iterator_tag)
{
    return last-first;
};

template<class InputIterator>
inline iterator_traits<InputIterator>::difference_type 
distance(InputIterator first,InputIterator last)
{
    typename iterator_traits<InputIterator>::iterator_category category;
    return __distance(first,last,category());
};

//以下是整组advance函数
template<class InputIterator,class Distance>
inline void __advance(InputIterator& i,Distance n,input_iterator_tag)
{
    while(n--) i++;
};

template<class BidirectionIterator,class Distance>
inline void __advance(BidirectionIterator& i,Distance n,bidirectional_iterator_tag)
{
    if (n > 0)
        while(n--) i++;
    else if(n < 0)
        while(n++) i--;
};

template<class RandamAccessIterator,class Distance>
inline void __advance(RandamAccessIterator& i,Distance n,random_iterator_tag)
{
    i+=n;
};

template<class InputIterator,class Distance>
inline void advance(InputIterator& i,Distance n)
{
    __advance(i,n,iterator_category(i));
};

}