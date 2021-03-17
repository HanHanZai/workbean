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

namespace zjh{
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
template<class T,class Alloc>
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

    //list的sort函数只能使用自己的，因为算法中的sort只支持随机迭代器,而list是一个双向迭代器
    //主要运行了merge函数，通过merge来进行合并，参考归并的方法进行合并操作，1-2-4-8-..依次类推
    void sort()
    {
        //如果只有一个元素或者没有元素，就不需要进行操作了,虽然size()也可以判断，但是速度太慢了
        if(node->next == node || link_type(node->next)->next == node)
            return;
        
        //一些新的lists,作为中介数据存放区
        list<T,Alloc> carry;
        list<T,Alloc> counter[64];

        int fill = 0;
        while (!empty())
        {
            carry.splice(carry.begin(),*this,begin());
            int i = 0;
            while(i < fill && !counter[i].empty())
            {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
                if(i == fill) fill++;
            }

            for(int i = 1; i < fill;i++)
                counter[i].merge(counter[i-1]);
            swap(counter[fill-1]);
        }
    }

    /* 使用全局的指针交换即可 */
    void swap(list<T,Alloc>& list)
    {
        using namespace swap;
        ::swap(*this,list);
    }
};

//deque的迭代器
template<class T,class Ref,class Ptr,size_t BufSiz>
struct __deque_iterator
{
    typedef __deque_iterator<T,T&,T*> iterator;
    typedef __deque_iterator<T,const T&,const T*,BufSiz> const_iterator;
    static size_t buffer_size(){ return __deque_buf_size(BufSiz,sizeof(T));}
    typedef random_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T** map_pointer;

    typedef __deque_iterator self;

    //保持与容器的联结
    T* cur; //此迭代器所指缓存区中的现行元素
    T* first; //此迭代器所指之缓冲区的头
    T* last; //此迭代器所指缓冲区的尾，包含对应的未使用空间
    map_pointer node;

    inline size_t __deque_buf_size(size_t n,size_t sz)
    {
        return n != 0 ? n:(sz < 512 ? size_t(512,sz):size_t(1););
    }

    void set_node(map_pointer new_node)
    {
        node = new_node;
        first = *new_node;
        last = first + difference_type(buffer_size());
    }

    //返回当前的指向
    reference operator*()const{return *cur;}
    //返回当前指向的指针地址，即指针对象
    pointer operator->()const{return &(operator*());}

    difference_type operator-(const self& x){
        return difference_type(buffer_size())*(node-x.node - 1) + (cur - first) + (x.last - x.cur);
    }

    self& operator++(){
        ++cur;
        if(cur == last)
        {
            set_node(node+1);
            cur = first;
        }
        return *this;
    }

    self& operator++(int){
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--(){
        if(cur == first)
        {
            set_node(node- 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self& operator--(int){
        self tmp = *this;
        --*this;
        return tmp;
    }

    //实现随机存取,迭代器可以直接条约n个距离
    self& operator+=(difference_type n){
        difference_type offset = n + (cur - first);
        //如果存在在同一个内存空间中
        if(offset > 0 && offset < difference_type(buffer_size()))
            cur += n;
        else
        {
            //节点的偏差值
            difference_type node_offset = offset > 0 ? offset/difference_type(buffer_size()): -difference_type((-offset - 1)/buffer_size())-1;
            //切换到正确的节点
            set_node(node_offset);
            //切换到正确的元素
            cur = first + (offset - node_offset*difference_type(buffer_size()));
        }
        return *this;
    }

    self operator+(difference_type n)const{
        self tmp = *this;
        return tmp+=n;
    }

    self& operator-=(difference_type n)const{
        return *this += -n;
    }

    self operator-(difference_type n) const{
        self tmp = *this;
        return tmp-=n;
    }

    //实现随机迭代器取数据
    reference operator[](difference_type n)const{
        return *(this + n);
    }

    bool operator==(const self& x)const{return cur == x.cur;}
    bool operator!=(const self& x)const{return !(*this == x);}
    bool operator<(const self& x)const
    {
        return (node == x.node)?(cur < x.cur):(node < x.node;);
    }
};

//deque双向队列，也是一个连续的线性空间不断拼接在一起的，所以迭代器很复杂
//最后一个参数指向缓冲区大小，默认为512Byte
template<class T,class Alloc,size_t BufSiz = 0>
class deque{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef __deque_iterator<T,T&,T*,BufSiz> iterator;

protected:
    typedef pointer* map_pointer; //指向T指针的指针
    iterator start;
    iterator finish;
    map_pointer map;
    size_type map_size; //map中可容纳指针的数量

//专属空间分配器
    typedef simple_alloc<value_type,Alloc> data_allocator; //类型分配器
    typedef simple_alloc<pointer,Alloc> map_allocator; //指针分配器

public:

    deque(int n,const value_type& value):start(),finish(),map(0),map_size(0)
    {
        fill_initialize(n,value);
    };

    void fill_initialize(size_type n,const value_type& value)
    {
        create_map_and_nodes();
        map_pointer cur;
        for(cur = start.node;cur!=finish.node;cur++)
            uninitialized_fill(*cur,*cur + buffer_size(),value);
        uninitialized_fill(finish.first,finish.last,value);
    }

    /* 创建map和对应节点 */
    void create_map_and_nodes(size_type num_elements)
    {
        //需要的节点数(元素个数/每个缓存器可容纳的节点个数)+1;
        //如果刚好整除，会多配一个节点
        size_type num_nodes = num_elements / buffer_size() + 1;

        //一个map要管理几个节点，最少8个，最多所需节点数+2
        /* 前后都预留一个，支持扩充 */
        map_size = max(initial_map_size(),num_nodes + 2);
        map = map_allocator::allocate(map_size);

        //以下令nstart和nfinish指向map所拥有之全部节点的最中央区域
        //保持在最中央，可使头尾两端的扩充能量一样大，每个节点可对应一个缓冲区
        map_pointer nstart = map + (map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;
        
        map_pointer cur;
        //为map内的每个现用节点都配置缓冲区，所有缓冲区加起来就是deque可用的总缓冲区
        for(cur = nstart;cur!=nfinish;cur++)
            *cur = allocate_node();

        //为deque的两个迭代器对象设置节点
        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur = finish.first + num_elements % buffer_size();
        //之前解释过，当节点刚好被整除，那么此时的finish的cur指向多出来的那个节点的头节点
    }

    void push_back(const value_type& t){
        //当前缓冲区还没有满
        if(finish.cur != finish.last - 1)
        {
            construct(finish.cur,t);
            finish++;
        }
        else
            push_back_aux(t); //当前缓冲区已经满了
    }

    void push_back_anx(const value_type& t)
    {
        value_type t_copy = t;
        reserve_map_at_back(); //若符合某种条件就需要替换一个map
        *(finish.node+1) = allocate_node(); //配置一个新的缓冲区
        construct(finish.cur,t_copy); //针对标的元素设值
        finish.set_node(finish.node+1); //改变finish，令其指向新节点
        finish.cur = finish.first; //设定finish的状态
    }
    
    void push_front(const value_type& t){
        if (start.cur != start.first)
        {
            construct(start.cur,t);
            start.cur--;
        }
        else
            push_front_anx(t);
    }

    void push_back_anx(const value_type& t)
    {
        value_type t_copy = t;
        reserve_map_at_front();//若符合某种条件，需要对map进行重新替换
        *(start.node - 1) = allocate_node();
        __STL_TRY{
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            construct(start.cur,t_copy);
        }
        cathc(){
            // commit or rollback
            start.set_node(start.node + 1);
            start.cur = start.first;
            deallocate_node(*(start.node - 1));
            throw;
        }
    }

    void reserve_map_at_back(size_type nodes_to_add = 1)
    {
        if(nodes_to_add + 1 > map_size - (finish.node - map))
            reallocate_map(nodes_to_add,false);
    }

    void reserve_map_at_front(size_type nodes_to_add = 1)
    {

    }


    iterator begin(){return start;}
    iterator end(){return finish;}

    reference operator[](size_type n){
        //调用迭代器的随机存取函数
        return start[difference_type(n)]; 
    }

    reference front(){return *start;} //调用迭代器的*
    reference back(){
        iterator tmp = finish;
        --tmp; //调用迭代器operator--
        return *tmp; //调用迭代器operator*
    }

    size_type size()const {return finish - start;;}
    size_type max_size()const{return size_type(-1);}
    bool empty()const{return start == finish;}
};

};