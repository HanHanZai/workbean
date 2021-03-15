//iterator_traits
#include"new.cpp"
#include"type_traits.cpp"
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
public:
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
    typedef __list_iterator<T,T&,T*>::iterator iterator;
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

    //逆转矩阵
    void reverse()
    {
        //如果只有一个元素或者没有元素，就不进行任何操作
        if(node->next == node || link_type(node->next)->next == node)
            return;
        iterator first = begin();
        ++first;
        while(first != end())
        {
            iterator old = first;
            first++;
            transfer(begin(),old,first);
        }
    }

    //list的sort函数只能使用自己的，因为算法中的sort只支持随机迭代器
};