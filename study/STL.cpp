//iterator_traits
#include<stdlib.h>
#include"new.cpp"

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

//list--链表 双向链表
//双向链表，前驱节点和后继节点
template<class T>
struct __list_node{
    typedef void* void_pointer;  //其实可以定义为 __list_node<T>* 
    void_pointer prev;
    void_pointer next;
    T data;
};

//list迭代器--iterator
template<class T,class Ref,class Ptr>
struct  __list_iterator
{
    typedef __list_iterator<T,T&,T*> iterator;
    typedef __list_iterator<T,Ref,Ptr> self;

    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __list_node<T>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    
    link_type node;//实际的节点对面
    __list_iterator(link_type x):node(x){}
    __list_iterator(){}
    __list_iterator(const iterator& x):node(x.node){}

    bool operator==(const self& x) const{node == x.node;}
    bool operator!=(const self& x) const{node!=x.node;}

    reference operator*()const{return (*node).data;}
    pointer operator->()const{return &(operator*());}

    self operator++(){
        node = (link_type)((*node).next;)
        return *this;
    }

    self operator++(int){
        self tmp = *this;
        ++*this;
        return tmp; 
    }

    self operator--(){
        node = (link_type)((*node).prev);
        return *this;
    }

    self operator--(int){
        self tmp = *this;
        --*this;
        return tmp;
    }
};

//list的数据结构
template<class T,class Alloc/* =alloc */>
class list{
protected:
    typedef __list_node<T> list_node;
    /* list的分配器 */
    typedef simple_alloc<list_node,Alloc> list_node_alloc;
public: 
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef list_node* link_type;
    typedef size_t size_type;
protected:
    link_type node; //只要一个指针，即可指向整个双链表
public:
    //当前节点是头节点
    iterator begin(){return (link_type)((*node).next);}
    //当前节点是尾节点
    iterator end(){return node;}

    bool empty()const{return node->next == node;}
    size_type size()const{
        size_type result = 0;
        distance(begin(),end(),result);
        return result;
    }

    //取头节点的内容(元素值)
    reference front() { return *begin();}
    //取尾节点的内容(元素值)
    reference back(){return *(--end());}

protected:
    //获取一个节点
    link_type get_node(){return list_node_alloc::allocate();}

    //释放一个节点
    void put_node(link_type p){list_node_alloc::deallocate(p);}

    //产生一个节点，并带有元素值
    link_type create_node(const T& x)
    {
        link_type p =get_node();
        construct(&p->data,x);
        return p;
    }

    //析构一个节点
    void destory_node(link_type p)
    {
        destory(&p->data);
        put_node(p);
    }
public:
    void empty_initialize()
    {
        node = get_node();
        node->next = node;
        node->prev = node;
    }

    iterator insert(iterator position,const T& x)
    {
        link_type tmp = create_node(x);
        tmp->next = position.node;
        tmp->prev = position.node->prev;
        (link_type(position.node->prev))->next = tmp;
        position.node->prev = tmp;
        return tmp;
    }

    //list的元素操作
    //push_front,push_back,erase,pop_front,pop_back,clear,remove,unique,splice,merge,reverse,sort

    void push_back(const T& x){insert(end(),x);}

    void push_front(const T& x){insert(begin(),x);}

    /* 删除指定位置的迭代器 */
    iterator erase(iterator position)
    {
        link_type next_node = link_type(position.node)->next;
        link_type prev_node = link_type(position.node)->prev;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destory_node(position.node);
        return (iterator)next_node;
    }

    //删除头部节点
    void pop_front(){ erase(begin());}

    //删除尾部节点
    void pop_back(){
        iterator tmp = end();
        erase(--tmp);
    }

    void clear(){
        link_type cur = (link_type)node->next; //begin()对象
        while(cur!=node)
        {
            link_type tmp = cur;
            cur = (link_type)cur->next;
            destory_node(tmp);
        }

        //修改节点原始状态
        node->next = node;
        node->prev = node;
    }

    void remove(const T& x)
    {
        iterator first = begin();
        iterator last = end();
        if(first == last) return;
        while(first!=last)
        {
            iterator next = first;
            ++next;
            if(*first == x) erase(first);
            first = next;
        }
    }

    //移除连续相同的连续元素
    void unique()
    {
        iterator first = begin();
        iterator last = end();
        if (first == last) return;
        iterator next = first;
        while(++next!=last)
        {
            if(*first == *next)
                erase(first);
            else    
                first = next;
            next = first;
        }
    }

    //将[first,last)移动到指定的position之前,transfer并非公开接口，提供的接口其实是(splice)
    void transfer(iterator position,iterator first,iterator last)
    {
        if(position!=last)
        {
            //将position的前置指针修改到调整的节点
            (link_type((*last.node).prev))).next = position.node; //修改最后节点的下一个节点为当前移动节点
            (link_type((*first.node).prev))).next = last.node; //将移动节点的前继节点指向最后的节点
            (link_type((*position.node).prev)).next = first.node; //将当前节点的前继节点的下一个节点为当前节点
            link_type tmp = link_type((*position.node).prev); //保存当前节点的前继节点
            (link_type((*position.node).prev)) = (*last.node).prev; //当前节点的前继节点指向最后节点的前继节点
            (*last.node).prev = (*first.node).prev; //最后节点的前继节点为首节点的前节点
            (*first.node).prev = tmp; //首节点的前节点时
        }
    }

    //将x接合于position所指位置之前，x必须不同于*this
    void splice(iterator position,list& x)
    {
        if(!x.empty()) transfer(position,x.begin(),x.end());
    }

    //将i所指元素结合于position所指位置之前，position和i可能会同指一个list
    void splice(iterator position,list&,iterator i)
    {
        iterator j =i;
        j++;
        if(position == j || position == i)return;
        transfer(position,i,j);
    }    

    //将[first,last)内所有元素接合于position所指位置之前
    void splice(iterator position,iterator first,iterator last)
    {
        if (first!=last) transfer(position,first,last);
    }

    //merge操作，将两个递增的排序合并到*this上
    void merge(list& x)
    {
        iterator first = begin();
        iterator last = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();
        while(first!=last && first2 != last2)
        {
            if (*first < *first2)
            {
                iterator next = first2;
                transfer(first,first2,++next);
            }
            else{
                first++;
            }
            if (first2 != last2)transfer(first,first2,last2);
        }
    }
};

}