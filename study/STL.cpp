#include"new.cpp"
#include"type_traits.cpp"
#include<vector>
#include<algorithm>
#include<functional>
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
class deque
{
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
    static size_type buffer_size(){ return __deque_buf_size(sizeof(T));}
    size_type __deque_buffer_size(size_t sz){
        return sz < 512 ? size_t(512,sz):size_t(1);
    }

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
        //如何空间不存在了，那么就要申请额外的内存空间了
        if(nodes_to_add > start.node - map)
            remallocate_map(nodes_to_add,true);
    }

    void remallocate_map(size_type nodes_to_add,bool add_at_front)
    {
        size_type old_nums = finish.node - start.node + 1;
        size_type new_nums = old_nums + nodes_to_add;

        map_pointer new_start;
        //如果大小依旧大于2倍新节点
        if(map_size > 2 * new_nums)
        {
            new_start = map + (map_size - new_nums) / 2 +(add_at_front?nodes_to_add:0);
            if(new_start < start.node)
                copy(start.node,finish.node+1,new_start); //如果新的开始节点在当前开始节点的前方，那么将当前节点拷贝到新节点开始的地方
            else
                copy_backward(start.node,finish.node + 1, new_start+old_nums); //如果新的开始节点在当前爱是节点的后方，那么将当前节点从后往前进行拷贝
        }
        else //节点数量小于2倍新节点
        {
            //重新分配内存
            size_type new_map_size = map_size + max(map_size,nodes_to_add) + 2; //更新新的节点数量为至少2倍+2
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_start = new_map + (new_map_size - new_nums) / 2 +  (add_at_front?add_at_front:0);
            copy(start.node,finish.node+1,new_start);
            map_allocator::deallocate(map,map_size); //回收之前的内存
            map = new_map; //更新新的node
            map_size = new_map_size; //更新新的大小
        }
        start.set_node(new_start); //设置新的开始节点
        finish.set_node(new_start + old_nums - 1); //设置新的结束节点
    }

    //pop_back操作
    void pop_back(){
        if(finish.cur != finish.first)
        {
            --finish.cur;
            destory(finish.cur);
        }
        else
            pop_back_aux(); //由于最后一个缓冲区的内存不需要了，需要进行释放
    }

    void pop_back_aux()
    {
        deallocate_node(finish.first); //释放最后一个缓冲区
        finish.set_node(finish.node - 1); //设置最后一个缓冲区为上一个节点
        finish.cur = finish.last - 1; //设置当前节点为前一个节点的最后一个节点
        destory(finish.cur); //析构当前对象，这里不需要调整cur的指针嘛？
    }

    void pop_front()
    {
        if(start.cur != start.last - 1)
        {
            destory(start.cur);//析构当前对象
            ++start.cur;//调整指针为下一个指针
        }
        else
            pop_front_aux();
    }

    void pop_front_aux(){
        destory(start.cur); //析构当前对象
        deallocate(start.first); //删除当前缓冲区
        start.set_node(start.node + 1);//移动当前指针到下一个节点
        start.cur = start.first; //设置当前节点为首节点
    }
    
    /* 按照deque的设计，需要保留一个缓冲区 */
    void clear(){
        for(map_pointer node = start.node + 1;node < finish.node;node++)
        {
            //释放缓冲区中的所有节点
            destory(*node,*node + buffer_size());
            //释放缓冲区
            data_allocator::deallocate(*node,buffer_size());
        }

        //此时相当于保留了2个缓冲区
        if(start.node != finish.node)
        {
            //将头部缓冲区目前所有元素进行析构
            destory(start.cur,start.last);
            //将尾部元素全部析构
            destory(finish.first,finish.last);
            data_allocator::deallocate(finish.first,buffer_size());
        }
        else
            destory(start.cur,finish.last); //当前只有一个缓冲区，全部节点析构
        
        finish = start;
    }

    //删除迭代器当前节点
    iterator erase(iterator pos)
    {
        iterator next = pos;
        ++next;
        difference_type index = post - start; //清楚点之前的节点数量，注意-号被重载过，所有的操作都在减号里
        if(index < (size()>>1)) //如果清除点之前的节点较少，那么此时将前面的节点往后移动
        {
            copy_backward(start,index,next);
            pop_front();
        }
        else
        {
            copy(next,finish,pos); //如果后面元素较少，整体往前移动
            pop_back();
        }
        return start + index;
    }

    //删除指定迭代器区间
    iterator erase(iterator first,iterator last)
    {
        //删除所有迭代器
        if(first == start && last == finish)
        {
            clear();
            return finish;
        }
        else{
            difference_type n = last - first; //删除节点个数
            difference_type before_num = first - start;
            //如果前面的数量比较少
            if(before_num < (size() - n)/2)
            {
                copy_backward(start,first,last); //从删除点往前拷贝
                iterator new_start = start + n; //标记新的起点
                destory(start,new_start);//析构之间的节点
                //释放之间的缓冲区
                for(map_pointer cur = start.node;cur < new_start.node;cur++)
                    data_allocator::deallocate(*cur,buffer_size());
                start = new_start;
            }
            else
            {
                copy(start,last,start);
                iterator new_finish = finish - n;
                destory(new_finish,finish); //析构之间的节点
                for(map_pointer cur = new_finish.node;cur<finish.node;cur++)
                    data_allocator::deallocate(*cur,buffer_size());
                finish = new_finish;
            }
            return start + before_num;
        }
    }

    //在position处插入一个元素，其值为x
    iterator insert(iterator position,const value_type& x)
    {
        //如果插入点是deque的最前端，直接push_front
        if(position.cur == start.cur)
        {
            push_front(x);
            return start;
        }
        else if(position.cur == finish.cur) //如果插入点是deque的最尾端，直接push_back
        {
            push_back(x);
            iterator tmp = finish;
            --tmp;
            return tmp;
        }
        else
            return insert_aux(position,x); //交给insert_aux去做
    }

    iterator insert_aux(iterator position,const value_type& x)
    {
        difference_type index = position - start;
        value_type x_copy = x;
        //前面的元素比较少
        if(index < size() / 2)
        {
            push_front(front()); //将当前元素放到最前方
            iterator front1 =  start;
            ++front1;
            iterator front2 = front1;
            ++front2;
            position = start + index;
            iterator pos1 = position;
            ++pos1;
            copy(front1,pos1,front2);
        }
        else
        {
            push_back(back());
            iterator back1 = finish;
            --back1;
            iterator back2 = back1;
            --back2;
            position = start + index;
            copy_backward(position,back2,back1);
        }
        *pos = x_copy;
        return pos;
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

//Stack 是一种配接器，是根据deque来进行实现的
//stack无迭代器，所有元素符合先进后出的原则，只有栈顶元素才能使用，使用list也可以实现stack,stack<int,list<int>>
template<class T,class Sequence = deque<T,T> >
class Stack{
friend bool operator==(const stack& x,const stack& y)
{
    return x.c == y.c;
};
friend bool operator<(const stack&,const stack&)
{
    return x.c < y.c;
};
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c;
public:
    bool empty() const{return c.empty();}
    size_type size() const{return c.size();}
    reference top() {return c.back();}
    const_reference top() const {return c.back();}
    void push(const value_type& x){c.push_back(x);}
    void pop(){c.pop_back();}
};


//队列queue也是一种配接器，都是通过deque来进行实现的，先进先出的结构
//队列queue无迭代器，所有元素符合先进先出的原则，使用list也可以实现，queue<int,list<int>>
template<class T,class Sequence = deque<T,T> >
class queue{
    friend bool operator==(const queue& x,const queue& y)
    {
        return x.c == y.c;
    };
    friend bool operator<(const queue& x,const queue& y)
    {
        return x.c < y.c;
    };
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c;
public:
    bool empty()const {return c.empty();}
    size_type size()const {return c.size();}
    reference front(){return c.front();}
    const_reference front()const{return c.front();}
    reference back(){return c.back();}
    const_reference back()const{return c.back();}
    void push(const value_type& x){c.push_back(x);}
    void pop(){c.pop_front();}
};

//堆Heap，优先队列的助手，并不属于容器组件 priority queue的复杂度介于queue和二叉搜索树之间，二叉堆就产生了，二叉堆就是一颗完全二叉树，除开
//最后一层的叶子节点之外，所有的节点都是满的，所以我们能够使用数组来直接进行数据存储，如果当前节点为i，如果小于i的节点那么就位于2i，大于i的就
//位于2i+1,其父节点就位于i/2处，此处的/是高斯符号，取其整数，通过此规则，我们就很容易生成一颗完全二叉树，这种以array表述tree的方法，我们称之隐式表达法

//push_heap是将元素推入到堆中，接收2个迭代器，用来表现一个heap底部容器的头和尾，并且元素已经插入到底部容器的最尾端
template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first,RandomAccessIterator last)
{
    __push_heap_aux(first,last,distance_type(first),value_type(first));
};
 
 template<class RandomAccessIterator,class Distance,class T>
 void __push_heap_anx(RandomAccessIterator first,RandomAccessIterator last,Distance*,T*){
     __push_heap(first,Distance(last-first - 1),Distance(0),T(*(last-1)));
 };

 template<class RandomAccessIterator,class Distance,class T>
 void __push_heap(RandomAccessIterator first,Distance holeIndex,Distance topIndex,T value)
 {
    Distance parent = (holeIndex - 1)/2; //找到对应的父节点
    while(holeIndex > topIndex && *(first + parent) < value)
    {
        //当尚未到达顶端，且父节点小于新值(于是不符合heap的次序特性)
        //由于上述使用的是operator<，可知堆是一个大顶堆
        *(first + holeIndex) = *(first + parent); //令洞值为父节点
        holeIndex = parent; //更新新的洞值
        parent = (holeIndex - 1)/2; //找到新的父节点
    }

    //最后将符合条件的洞值进行替换
    *(first+holeIndex) = value;
 };

//pop_heap是将栈顶元素弹出，其实是将节点放到最后一个节点，然后比较上面最大的节点，空出来的洞就是最后节点填入的位置
template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last)
{
    __pop_heap_aux(first,last,value_type(first));
};

template<class RandomAccessIterator,class T>
void __pop_heap_aux(RandomAccessIterator first,RandomAccessIterator last,T*)
{
    __pop_heap(first,last-1,last-1,T(*(last-1)),distance_type(last - first));
    //因为最好要弹出的节点都会放在最后一个节点，所以result的地址指针就是last-1
};

template<class RandomAccessIterator,class T,class Distance>
void __pop_heap(RandomAccessIterator first,RandomAccessIterator last,RandomAccessIterator result,T value,Distance*)
{
    *result = *first; //要返回的对应
    __adjust_heap(first,Distance(0),Distance(last-first),value);
};

template<class RandomAccessIterator,class T,class Distance>
void __adjust_heap(RandomAccessIterator first,Distance holeIndex,Distance len,T value)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2* topIndex + 2; //将右节点提上来
    while(secondChild < len)
    {
        //比较洞节点的左右2个子值，然后用secondChild代表较大子节点
        if(*(first + secondChild) < *(first + secondChild - 1))
            secondChild--;
        //令较大子值为洞值，再令洞号下移到较大子节点处
        *(first + holeIndex) = *(first+secondChild);
        holeIndex = secondChild;
        secondChild = 2*(secondChild + 1);
    }
    //没有右子节点，只有左子节点
    if(secondChild == len)
    {
        //将左子值为洞值，在令洞号下移到左子节点处
        *(first+holeIndex) = *(first+secondChild-1);
        holeIndex = secondChild-1;
    }
    //将欲调整的值填入到当前的洞号
    __push_heap(first,holeIndex,topIndex,value);
    //*(first + holeIndex) = value 我感觉也行
};

//sort_heap算法，其实就是不断的调用pop_heap操作
template<class RandomAccessIterator>
void sort_heap(RandomAccessIterator first,RandomAccessIterator last)
{
    //没执行一次，最后的那个节点就是一个当前的最大值
    while(last - first > 1)
        pop_heap(first,last--);
};

//make_heap算法，将一段现有的数据转化为一个heap
template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first,RandomAccessIterator last)
{
    __make_heap(first,last,value_type(first),distance_type(first));
};

//以下make_heap不允许指定"大小比较标准"
template<class RandomAccessIterator,class T,class Distance>
void __make_heap(RandomAccessIterator first,RandomAccessIterator last,T*,Distance*)
{
    if((last - first) < 2)return;
    Distance len = last - first;
    //找出第一个需要重排的子树头部，以parent标示出，由于任何叶节点都不需要执行
    Distance holeIndex = (len - 2)/2;
    while(true){
        //这里面是跟后面的值进行比较
        __adjust_heap(first,holeIndex,len,T(*(first+holeIndex)));
        if(holeIndex == 0) return;
        holeIndex--;
    }
};

//优先队列 priority_queue
//是一种带权值观念的队列，允许底部增加元素，顶部移除元素，权值最高者越排在前面
//priority_queue完全由底部容器实现的

template<class T,class Sequence = std::vector<T>,class Compare = std::less<typename Sequence::value_type> >
class priority_queue{
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequnence::const_reference const_reference;
protected:
    Sequence c;
    Compare comp;
public:
    priority_queue():c(){};
    explicit priority_queue(const Compare& x):c(),comp(x){};

    template<class InputIterator>
    priority_queue(InputIterator first,InputIterator last,const Compare& x):c(first,last),comp(x){
        make_heap(c.begin(),c.end(),comp);
    };

    template<class InputIterator>
    priority_queue(InputIterator first,InputIterator last):c(first,last)
    {
        make_heap(c.begin(),c.end(),comp);
    };

    bool empty() const { return c.empty();}
    size_type size() const {return c.size();}
    const_reference top()const {return c.front();}

    void push(const value_type& x)
    {
        try
        {
            c.push_back(x);
            push_heap(c.begin().c.end(),comp);
        }
        catch(const std::exception& e)
        {
            c.clear();
        }
    }

    void pop()
    {
        try
        {
            pop_heap(c.begin(),c.end(),comp);
            c.pop_back();
        }
        catch(const std::exception& e)
        {
            c.clear();
        }
    };

    //优先队列是没有迭代器的，因为它在不断的刷新头节点，所以可以通过不断的遍历进行更新
};

//双向链表slist list和slist的区别，list是个(ForwardIterator)单向迭代器，slist是一个双向迭代器(BidirectorIterator)
//slit 和 list共同具有的一个相同的特色是，他们的插入，移除，结合等操作并不会造成迭代器失效，所谓的迭代器失效是++是否还能正确执行
//除了slist起点处附近的区域之外，在其他地方采用insert或erase操作函数，都属于不智之举，这就是slist较于list的最大缺点，所以slist特别提供了insert_after()和erase_after()
//slist只提供了push_front,因此元素次序会和插入进来的次序相反

//slist的节点
struct __slist_node_base
{
    __slist_node_base* next;
};

//单向链表的节点结构
template<class T>
struct __slist_node:public __slist_node_base
{
    T data;
};

//全局函数，已知某个节点，插入新节点于其后
inline __slist_node_base* __slist_make_link(__slist_node_base* prev_node,__slist_node_base* next_node)
{
    next_node->next = prev_node->next;
    prev_node->next = next_node;
    return next_node;
};

//全局函数，单向链表的大小(整体元素个数)
inline size_t __slist_size(__slist_node_base* node){
    size_t result = 0;
    for(;node!=0;node = node->next)
        ++result;
    return result;
};

//slist的迭代器
struct  __slist_iterator_base
{
    typedef size_t size_Type;
    typedef ptrdiff_t diffenrence_type;
    typedef forward_iterator_tag iterator_category; //这里是个单向的
    __slist_node_base* node;
    __slist_iterator_base(__slist_node_base* x):node(x){}
    void incr(){node = node->next;}

    bool operator==(const __slist_iterator_base& x)const
    {
        return node == x.node;
    }

    bool operator!=(const __slist_iterator_base& x)const
    {
        return node != x.node;
    }
};

//单向链表的迭代器结构
template<class T,class Ref,class Ptr>
struct __slist_iterator:public __slist_iterator_base
{
    typedef __slist_iterator<T,T&,T*> iterator;
    typedef __slist_iterator<T,const T&,const T*> const_iterator;
    typedef __slist_iterator<T,Ref,Ptr> self;

    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __list_node<T> list_node;

    //调用slist<T>::end()时会调用下面的函数
    __slist_iterator(list_node* x):__slist_iterator_base(x){};
    __slist_iterator():__slist_iterator_base(0){};
    __slist_iterator(const iterator& x):__slist_iterator_base(x.node){};

    reference operator*()const{return ((list_node*)node)->data;};
    pointer operator->()const{return &(operator*());};

    self operator++(){
        incr();
        return *this;
    };

    self operator++(int){
        self tmp = *this;
        incr();
        return tmp;
    };

    //因为是单向迭代器，所以没有--
    //由于没有重载==符号，所以就根据父类中的重载符号而定
};

//slist的数据结构
template<class T,class Alloc>
class slist
{
public:
    typedef T value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef __slist_iterator<T,T&,T*> iterator;
    typedef __slist_iterator<T,const T&,const T*> const_iterator;
private:
    typedef __slist_node<T> list_node;
    typedef __slist_node_base list_node_base;
    typedef __slist_iterator_base iterator_base;
    typedef simple_alloc<list_node,Alloc> list_node_allocator;

    static list_node* create_node(const value_type& x)
    {
        list_node* node = list_node_allocator::allocate();//配置空间
        construct(&node->data,x);
        node->next = 0;
        return node;
    }

    static void destory_node(list_node* node)
    {
        destory(&node->data); //析构元素
        list_node_allocator::deallocate(node); //释放空间
    }

private:
    list_node_base head;

public:
    slist(){head.next = 0;}
    ~slist(){clear();}

    iterator begin(){return iterator((list_node*)head.next);}
    iterator end(){return iterator(0);}
    size_type size(){return __slist_size(head.next)};
    bool empty(){return head.next == 0;}

    void swap(slist& x)
    {
        list_node_base* tmp = x.head.next;
        x.head.next = head.next;
        head.next = tmp;
    }

    reference front(){return ((list_node*)head.next)->data;}

    //从头部插入一个节点
    void push_front(const value_type& x)
    {
        __slist_make_link(&head,create_node(x));
    }

    //从头部取走元素
    void pop_front()
    {
        list_node_base* node = head.next;
        head.next = head.next->next;
        destory_node(node);
    }
};

//关联性容器
//一般的关联性容器都有一个键值和实值，所以关联性容器没有所谓头尾，只有最大元素和最小元素
//容器的内部结构可能是RB-Tree或者是hash-table,所以不会有push_front(),push_back(),pop_back(),pop_front(),begin(),end()等操作
//一般而言，关联式容器的内部结构是一个balance binary tree(平衡二叉树)，包括AVL-tree，RB-tree，AA-tree。在STL中使用的是RB-tree

//树是由边和节点构成，整棵树有一个最上端的节点，称为根节点，每个节点可以拥有具方向性的边，用来和其他节点相连，相连节点中，在上者称为父节点，
//在下者称为子节点，无子节点就称为叶节点，子节点可以存在多个，如果最多只允许2个子节点，那么就是所谓的二叉树

//二叉搜索树 -- 任何节点只允许2个子节点，这两个子节点称为左子节点和右子节点，所以二叉搜素树的运用于编译器表达书，和哈夫曼编码树
//最小节点为一直为左节点的叶子节点，最大节点为一直为右节点的叶子节点，节点的插入和删除操作都只需要对数时间

//平衡二叉搜索树：由于节点的插入会导致节点插入过深，不同的平衡条件，造就出了不同的效率表现，平衡二叉搜索树一般有AVL-tree，RB-tree,AA-tree
//由于需要达到平衡，由于需要保持某种程度的平衡，导致插入和删除节点的平均时间也比较长，但是可以避免最坏的情况（高度不平衡即为链表）
//但是他们保持着某种程度上的平衡，所以一般而言的搜索时间可以节约25%

//AVL树，加上了额外的平衡条件，其平衡条件的建立是为了确保整颗树的深度为O(logn),由于左右子树的高度无法完全达到一致，所以AVL树
//放松了对平衡树的要求，要求左右子树的高度相差不超过1

//条件不满足的情况下，有4种情况的树，分别为
//1.插入点位于X的左子节点的左子树 --- 左左
//2.插入点位于X的左子节点的右子树 --- 左右
//3.插入点位于X的右子节点的左子树 --- 右左
//4.插入点位于X的右子节点的右子树 --- 右右
//其中1，4相互对应，使用单旋转即可，2，3相互对应，使用双旋转
//单旋转 --- 根据产生相差点的节点进行右旋转，将其对应的右子节点移动到之前主节点的左子节点
//双旋转 --- 先根据插入点的父节点进行旋转，将插入节点放到父节点的父节点的右节点上，然后根据新的节点进行旋转

//STL种唯一使用的是RB-tree，也是根据单旋转和双旋转进行调整
//RB-tree的特性：
//1.每个节点不是黑色就是红色，
//2.根节点一定为黑色
//3.如果节点为红，其子节点必须为黑
//4.任意节点到NULL的任何路径，所含黑节点数必须相同

//插入节点
//新增节点为X，其父节点为P，祖父节点为G，伯父节点为S，曾祖父节点为GG
//新增节点X必为叶节点，那么X必为红色，而其父节点只能为黑色，不然就违反了3特性，那么G必定为黑，所以根据X的插入位置及外围节点的颜色
//就有以下四种情况

//1.S为黑色，X是外侧插入，此时对P，G进行一次单旋转，然后再更改P，G颜色，即可重新满足红黑树的规则3，但是红黑树可能会出现不平衡的情况，不过RB-tree没有AVI树严格，所以不在意，效率也差不太多
//2.S为黑色，X是内测插入，此时对P，X进行一次单旋转，然后更改G，X颜色，然后对G再进行一次旋转，此时就能满足条件
//3.S为红色，X是外侧插入，此时对P，G进行一次单旋转，更改X的颜色，如果GG为黑色，一切搞定，如果GG为红色，那么问题就大一些见状况4
//4.S为红色，X是外侧插入，先对P，G进行一次单旋转，改变X的颜色，如果GG为红色，继续往上旋转调整颜色，直到父子连续不为红

//为了避免状况4，父子节点皆为红色的情况持续往上发展，而导致时效上有瓶颈，假设新增节点为A，那么就沿着A的路径，只要看到有某节点X
//的两个子节点皆为红色，就把X改成红色，并且将两个子节点改成黑色
//但是如果A的父节点P亦为红色，就得像状况1一样做一次单旋转并改变颜色，活着像状况2一样做一次双旋转并改变颜色

//RB-tree的节点设计
//为了有更大的弹性，节点分为两层，

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;//红色为0
const __rb_tree_color_type __rb_tree_black = true; //黑色为1

struct __rb_tree_node_base
{
    //节点颜色
    typedef __rb_tree_color_type color_type;
    //节点指针
    typedef __rb_tree_node_base* base_ptr;

    color_type color;//节点颜色
    base_ptr parent;//节点的许多操作都和父节点有关系
    base_ptr left;//左子节点
    base_ptr right;//右子节点
    static base_ptr minimum(base_ptr ptr)
    {
        while(ptr->left) ptr = ptr->left;
        return ptr;
    }

    static base_ptr maximum(base_ptr ptr)
    {
        while(ptr->right)ptr = ptr->right;
        return ptr;
    }
};

template<class Value>
struct __rb_tree_node:public __rb_tree_node_base
{
    typedef __rb_tree_node<Value>* link_type;
    Value value_field; //节点值
};

//RB-tree的迭代器
//考虑一个迭代器，要考虑类别，++，--，&，*
//分为2个部分，一部分为基层迭代器，另一层为是实际的迭代器
//基层迭代器
struct __rb_tree_base_iterator
{
    typedef __rb_tree_node_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    base_ptr node; //它用来与容器之间产生一个连结关系
    //以下实现其实可实现于operator++内，因为再无他处会调用此函数了
    //increment直接找到比自己大一点的哪个节点
    void increment()
    {
        //如果有右节点，就向右走，然后一直往左子树走到底即为解答
        if (node->right != 0)
        {
            node = node->right;
            while(node->left!=0)
                node = node->left;
        }
        else
        {
            base_ptr y = node->parent;
            //如果没有右节点，如果现行节点本身就是个右子节点，就一直上溯，直到不为右子节点为止
            while(node == y->right)
            {
                node = y;
                y = y->parent;
            }
            //若此时的右子节点不等于此时的父节点，此时的父节点即为答案，否则此时的node为解答，这里是处理极端情况的
            if(node->right!=y)
                node = y;
        }
    }

    //以下实现其实可实现于operator--内，因为再无他处会调用此函数了
    void decrease()
    {
        //如果节点为红色，父亲节点的父节点是自己，右子节点即为解答
        //以上情况发生在当前节点为header，header之右子节点即为mostright，指向整棵树的max节点
        if (node->color == __rb_tree_red && node->parent->parent == node)
            node = node->parent;
        else if(node->left != 0)
        {
            //如果有左子节点，令y为左子节点
            base_ptr y = node->left;
            //当y有右子节点，一直往右子节点走到底即为答案
            while(y->right!=0)
                y = y->right;
            node = y;
        }
        else
        {
            //非根节点，也不存在左子节点，找出父节点，当现行节点身为左子节点，一直交替往上走，直到现行节点不为左子节点
            base_ptr y = node->parent;
            while(node == y->left)
            {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    };
};

//RB-tree正式的迭代器
template<class Value,class Ref,class Ptr>
struct __rb_tree_iterator:public __rb_tree_base_iterator
{
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __rb_tree_iterator<Value,Value&,Value*> iterator;
    typedef __rb_tree_iterator<Value,const Value&,const Value*> const_iterator;
    typedef __rb_tree_iterator<Value,Ref,Ptr> self;
    typedef __rb_tree_node<Value>* link_type;
    __rb_tree_iterator(){};
    __rb_tree_iterator(link_type x){node = x;};
    __rb_tree_iterator(const iterator& x){node = x.node;};
    //获取当前节点值
    reference operator*()const{return link_type(node)->value_field;}
    pointer operator->()const{return &(operator*());};
    self& operator++(){increment();return *this;}
    self operator++(int){
        self tmp = *this;
        increment();
        return tmp;
    }

    self& operator--(){decrease();return *this;}
    self operator--(int){
        self tmp = *this;
        decrease();
        return tmp;
    }
};

//RB-tree的数据结构
template<class Key,class Value,class KeyOfValue,class Compare,class Alloc>
class rb_tree{
protected:
    typedef void* void_pointer;
    typedef __rb_tree_node_base* base_ptr;
    typedef __rb_tree_node<Value> rb_tree_node;
    typedef simple_alloc<rb_tree_node,Alloc> rb_tree_allocator;
    typedef __rb_tree_color_type color_type;
public:
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef rb_tree_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
protected:
    //创建一个node的空间
    link_type get_node(){return rb_tree_allocator::allocate();}
    //释放一个node的空间
    void put_node(link_type p) {rb_tree_allocator::deallocate(p);}

    //创建一个节点，进行构造
    link_type create_node(const value_type& x){
        link_type tmp = get_node();
        construct(&tmp->value_field,x);
        return tmp;
    }

    //深拷贝一个节点
    link_type clone_node(link_type x){
        link_type  tmp = create_node(x);
        tmp->color = x->color;
        tmp->left = 0;
        tmp->right = 0;

        return tmp;
    }

    //销毁一个节点
    void destory_node(link_type p)
    {
        destory(&p->value_field);//析构
        put_node(p);
    }

protected:
    //RB-tree只用三笔数据进行记录
    size_type node_count;
    link_type header;
    Compare key_compare;

    //以下三个函数方便取得header的成员
    link_type root()const {return (link_type&)header->parent;}
    link_type leftmost()const{return (link_type&)header->left;}
    link_type rightmost()const{return (link_type&)header->right;}

    //以下六个函数用来方便取得节点x的成员
    //获取当前对象的左子节点
    static link_type& left(link_type x)
    {
        return (link_type&)(x->left);
    }

    //获取当前对象的右子节点
    static link_type& right(link_type x)
    {
        return (link_type&)(x->right);
    }

    //获取当前对象的父节点
    static link_type& parent(link_type x)
    {
        return (link_type&)(x->parent);
    }

    //获取当前对象的值
    static reference value(link_type x)
    {
        return x->value_field;
    }

    //获取当前对象的key值
    static const Key& key(link_type x)
    {
        return KeyOfValue()(value(x));
    }

    //获取按当前对象的颜色
    static color_type& color(link_type x)
    {
        return (color_type&)(x->color);
    }

    //以下六个函数用来方便取得节点x的成员
    static link_type& left(base_ptr& x)
    {
        return (link_type&)(x->left);
    }

    static link_type& right(base_ptr& x)
    {
        return (link_type&)(x->right);
    }

    static link_type& parent(base_ptr& x)
    {
        return (link_type&)(x->parent);
    }

    static reference value(base_ptr& x)
    {
        return ((link_type)x)->value_field;
    }

    static color_type& color(base_ptr& x)
    {
        return (color_type&)(link_type(x)->color);
    } 

    static link_type minimum(link_type x)
    {
        return (link_type) __rb_tree_node_base::minimum(x);
    }

    static link_type maximum(link_type x)
    {
        return (link_type) __rb_tree_node_base::maximum(x);
    }

public:
    typedef __rb_tree_iterator<value_type,reference,pointer> iterator;
private:
    iterator __insert(base_ptr x,base_ptr y,const value_type& v);
    link_type __copy(link_type x,link_type p);
    void __erase(link_type x);
    void init(){
        header = get_node(); //产生一个节点空间，令header指向它
        color(header) = __rb_tree_red; //设置头节点颜色为红色
        root() = 0; //设置根节点为空
        leftmost() = header; //设置最左子节点为自己本身
        rightmost() = header; //设置最右子节点为自己本身
    }
public:
    rb_tree(const Compare& comp = Compare()):node_count(0),key_compare(comp)
    {
        init();
    }

    ~rb_tree()
    {
        clear();
        put_node(header);
    }

    rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& operator=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& x);

    Compare key_comp()const{return key_compare;}
    
    iterator begin(){return leftmost();}

    iterator end(){return header;}

    bool empty(){return node_count == 0;}

    size_type size(){return node_count;}
    size_type max_size()const{return size_type(-1);}

public:
    //将x插入到RB-Tree中(保证插入的节点是独一无二的)
    std::pair<iterator,bool> insert_unique(const value_type& x);
    //将x插入都RB-Tree中，允许节点重复值
    iterator insert_equal(const value_type& x);
    iterator find(const Key& k);
};

//RB-tree的构造与内存管理
//使用init初始化的时候，将初始化一个空的header，左右都指向自己，父节点为空
//插入节点的时候，更新对应的左右节点

//RB-tree的元素操作，主要是插入和搜索
//重复插入
template<class Key,class Value,class KeyOfValue,class Compare,class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::insert_equal(const Value& v)
{
    link_type y = header;
    link_type x = root(); //从根节点开始
    while(x!=0){
        y = x;
        x = key_compare(KeyOfValue()(v),key(x))?left(x):right(x);
    }
    return __insert(x,y,v);
};

//唯一插入
template<class Key,class Value,class KeyOfValue,class Compare,class Alloc>
std::pair<typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator,bool>
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::insert_unique(const Value& v)
{
    link_type y = header;
    link_type x = root(); //从根节点开始
    bool comp = true;
    while(x!=0)
    {
        y = x;
        comp = key_compare(KeyOfValue()(v),key(x)); //v小于x
        x = comp?left(x):right(x);
    }

    //j是x的父节点
    iterator j = iterator(y);
    //如果离开while之后，comp为true，说明要插入到节点的左边
    if(comp)
    {
        //如果插入的节点是最左节点
        if (j == begin())
            return std::pair<iterator,bool>(__insert(x,y,v),true); //x是当前要插入的节点位置，y是当前节点的父节点，v是要插入的值
        else
            --j; //否则 调整j，准备后续测试，这里是作甚？
    }
    
    //小于新值，插入到右子节点
    if (key_compare(key(j.node),KeyOfValue()(v)))
        return std::pair<iterator,bool>(__insert(x,y,v),true);
    
    //到此为止，如果还没插入，说明已经存在重复的值了
    return std::pair<iterator,bool>(__insert(x,y,v),false);
};

//真正的插入函数
template<class Key,class Value,class KeyOfValue,class Compare,class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator 
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::__insert(base_ptr x_,base_ptr y_,const value_type& v)
{
    //x是插入点，y是插入点的父节点，参数v是新值
    link_type x = (link_type)__x;
    link_type y = (link_type)__y;
    link_type z;
    //key_compare是个比较函数
    //y是头节点，x不为空，v的值小于y的值
    if(y == header || x!=0 || key_compare(KeyOfValue()(v),key(y)))
    {
        z = create_node();
        left(y) = z; //当y为header时，leftmost就是z
        if(y == header)
        {
            root() = z; //当y是头节点时，此时令z为根节点
            leftmost() = z;
        }
    }
    else{
        z = create_node();
        right(y) = z;
        if(y == rightmost())
            rightmost() = z;
    }

    parent(z) = y; //设置新节点为父节点
    left(z) = 0 ;//设置新节点的左子节点
    right(z) = 0 ;//设置新节点的右子节点

    __rb_tree_rebalance(z,header->parent); //参数1为新增节点，参数2为根节点
    ++node_count;
    return iterator(z);
};

//左旋转
inline void 
__rb_tree_rotate_left(__rb_tree_node_base* x,__rb_tree_node_base*& root)
{
    //x为旋转点
    //令y为x的右子节点
    __rb_tree_node_base* y = x->right;
    x->right = y->left;
    if(y->left!=0)
        y->left->parent = x; //调整父节点为x
    y->parent = x->parent;//更新y的父节点

    //令y完全替代x的地位，把x父节点的所有相关信息进行继承
    if(x == root)
        root = y;
    else if(x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    y->left = x;
    x->parent = y;
};

inline void 
__rb_tree_rotate_right(__rb_tree_node_base* x,__rb_tree_node_base*& root)
{
    //x为旋转点
    //令y为x的左子节点
    __rb_tree_node_base* y = x->left;
    x->left = y->right; //更新x的左子节点为y的右子节点
    if(y->right != 0)
        y->right->parent = x; //回马枪设置一下
    y->parent = x->parent;

    if(x == root)
        root = y;
    else if(x == x->parent->left)
        x->parent->left = y;
    else 
        x->parent->right = y;
    
    y->right = x;
    x->parent = y;
};

//节点旋转调整
inline void 
__rb_tree_rebalance(__rb_tree_node_base* x,__rb_tree_node_base*& root)
{
    //被插入的节点一定是红色的
    x->color = __rb_tree_red;
    //父节点为红
    while(x!=root && x->parent->color == __rb_tree_red)
    {
        //如果父节点是祖父节点的左子节点
        if(x->parent == x->parent->parent->left)
        {
            //令y是祖父节点
            __rb_tree_node_base* y = x->parent->parent->right;
            //伯父是红色节点
            if(y && y->color == __rb_tree_red)
            {
                //修改父节点为黑色
                x->parent->color = __rb_tree_black;
                //修改伯父节点为黑色
                y->color = __rb_tree_black;
                //修改祖父节点为红色
                x->parent->parent->color = __rb_tree_red;
                //准备回溯
                x = x->parent->parent;
            }
            else //无伯父节点，或者伯父节点为黑色
            {
                //如果新节点是父节点的右子节点
                if (x == x->parent->right)
                {
                    x = x->parent;
                    __rb_tree_rotate_left(x,root); //第一参数为左旋点
                }
                //修改父节点为黑色
                x->parent->color = __rb_tree_black;
                //修改祖父节点为红色
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_right(x,root);
            }
        }
        else //父节点为祖父节点的右子节点
        {
            //y是伯父节点
            __rb_tree_node_base* y = x->parent->parent->left;
            //伯父节点是红色
            if(y && y->color == __rb_tree_red)
            {
                //更改父节点颜色为黑色
                x->parent->color = __rb_tree_black;
                //更改伯父节点颜色为黑色
                y->color = __rb_tree_black;
                //修改祖父节点颜色为红色
                x->parent->parent->color = __rb_tree_red;
                x = x->parent->parent;
            } 
            else //无伯父节点，或者伯父节点为黑色
            {
                //如果节点为父节点的左子节点
                if(x == x->parent->left)
                {
                    x = x->parent;
                    __rb_tree_rotate_right(x,root);
                }
                //修改父节点的颜色为黑色
                x->parent->color = __rb_tree_black;
                //修改祖父节点的颜色为红色
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_left(x,root);
            }
        }
    }
    root->color = __rb_tree_black; //根节点永远为黑色
};

//元素的查询
template<class Key,class Value,class KeyOfValue,class Compare,class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator 
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::find(const Key& k){
    link_type y = header; //最后一个不比k小的节点
    link_type x = root(); //当前节点
    while(x!=0){
        if(!key_compare(Key(x),k))
        {
            y = x;
            x = left(x); 
        }
        else
            x = right(x);
    }
    iterator j = iterator(y);
    return (j == end()||key_compare(k,Key(j.node)))?end():j;
};

//set的特性，所有元素都会根据元素的键值自动被排序，set的元素不像map那样可以同时拥有实值，实值就是键值，set不允许出现两个相同的值
//set的迭代器是一个const迭代器，不允许进行修改
//默认降序排序
template<class Key,class Compare,class Alloc>
class set{
public:
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
    template<class T>
    struct identity:public std::unary_function<T,T>
    {
        const T& operator()(const T& x)const {return x;}
    };
    typedef rb_tree<key_type,value_type,identity<value_type>,key_compare,Alloc> rep_type;
    rep_type t;    
public:
    typedef typename ret_type::const_pointer pointer;
    typedef typename ret_type::const_pointer const_pointer;
    typedef typename ret_type::const_reference reference;
    typedef typename ret_type::const_reference const_reference;
    typedef typename ret_type::const_iterator iterator;
    typedef typename ret_type::const_iterator const_iterator;
    typedef typename ret_type::size_type size_type;
    typedef typename ret_type::difference_type difference_type;
    typedef typename ret_type::const_reserver_iterator reverse_iterator;
    typedef typename ret_type::const_reserver_iterator const_reverse_iterator;
    //插入的方法只有一个insert_unique,因为set不允许重复插入数据
    set():t(Compare()){};
    explicit set(const Compare& comp):t(comp){};

    template<class InputIterator>
    set(InputIterator first,InputIterator last):t(Compare())
    {
        insert(first,last);
    }

    template<class InputIterator>
    set(InputIterator first,InputIterator last,const Compare& comp):t(comp)
    {
        insert(first,last);
    }

    set(const set<Key,Compare,Alloc>& x):t(x.t){};

    set<Key,Compare,Alloc>& operator=(const set<Key,Compare,Alloc>& x)
    {
        t = x.t;
        return *this;
    }

    //获取比较函数
    key_compare key_comp()const {return t.key_comp();};
    value_compare value_comp()const{return t.key_comp();};
    iterator begin()const { return t.begin(); }
    iterator end() const { return t.end();}
    reverse_iterator rbegin()const {return t.rbegin();}
    reverse_iterator rend()const {return t.rend();}
    bool empty()const{return t.empty();}
    size_type size()const{return t.size();}
    size_type max_size()const{return t.max_size();}
    void swap(set<Key,Compare,Alloc>& x){t.swap(x);};

    typedef std::pair<iterator,bool> pair_iterator_bool;
    //唯一插入
    pair_iterator_bool insert(const value_type& x)
    {
        std::pair<typename rep_type::iterator,bool> p = t.insert_unique(x);
        return pair_iterator_bool(p.first,p.second);
    }

    iterator insert(iterator position,const value_type& x)
    {
        typedef typename rep_type::iterator rep_iterator;
        return t.insert_unique((rep_iterator&)position,x);
    }

    template<class InputIterator>
    void insert(InputIterator first,InputIterator last)
    {
        t.insert_unique(first,last);
    }

    void erase(iterator position)
    {
        typedef typename rep_type::iterator rep_iterator;
        return t.erase((rep_iterator&)position);
    }

    size_type erase(const key_type& x)
    {
        return t.erase(x);
    }

    void erase(iterator first,iterator last)
    {
        typedef typename rep_type::iterator rep_iterator;
        t.erase((rep_iterator&)first,(rep_iterator&)last);
    }

    void clear(){t.clear();}

    iterator find(const key_type& x)const{return t.find(x);};
    size_type count(const key_type& x)const{return t.count(x);};
    //获取第一个小于等于当前值的迭代器
    iterator lower_bound(const key_type& x)const{
        return t.lower_bound(x);
    }

    //获取第一个大于当前值的迭代器
    iterator upper_bound(const key_type& x)const{
        return t.upper_bound(x);
    }

    pair<iterator,iterator> equal_range(const key_type& x) const{
        return t.equal_range(x);
    }

    friend bool operator==(const set&,const set&);
    friend bool operator<(const set&,const set&);
};

template<class Key,class Compare,class Alloc>
inline bool operator==(const set<Key,Compare,Alloc>& x,const set<Key,Compare,Alloc>& y)
{
    return x.t == y.t;
};

template<class Key,class Compare,class Alloc>
inline bool operator<(const set<Key,Compare,Alloc>& x,const set<Key,Compare,Alloc>& y)
{
    return x.t < y.t;  
};

//map是一系列的关联值，key的值是不容被修改的，value的值才能被改变
//map和list拥有相同的某些性质，当客户端对它进行元素增加操作或者删除操作，操作之前的所有迭代器，在操作之后都依然有效
template<class Key,class T,class Compare,class Alloc>
class map{
public:
    typedef Key key_type; //键的类型
    typedef T data_type; //值的类型
    typedef T mapped_type;
    typedef std::pair<const Key,T> value_type; //元素类型(键/值)
    typedef Compare key_compare; //键值比较函数
    
    //以下定义一个function，其作用就是调用元素比较函数
    class value_compare:public std::binary_function<value_type,value_type,bool>
    {
        friend class map<Key,T,Compare,Alloc>;
    protected:
        Compare comp;
        value_compare(Compare c):comp(c){};
    public:
        bool operator()(const value_type& x,const value_type& y)const{
            return comp(x.first,y.first);
        }    
    }
private:
    typedef rb_tree<key_type,value_type,selectlst<value_type>,key_compare,Alloc> rep_type;
    rep_type t;
public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_referenece const_reference;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    map():t(Compare()){};
    explicit map(const Compare& comp):t(comp){};

    template<class InputIterator>
    map(InputIterator first,InputIterator last):t(Compare())
    {
        t.insert_unique(first,last);
    }

    template<class InputIterator>
    map(InputIterator first,InputIterator last,const Compare& comp):t(comp)
    {
        t.insert_unique(first,last);
    }

    map(const map<Key,T,Compare,Alloc>& x):t(x.t){};
    map<Key,T,Compare,Alloc>& operator=()(const map<Key,T,Compare,Alloc>& x)
    {
        t = x.t;
        return *this;
    }

    key_compare key_comp()const{return t.key_comp();}
    value_compare value_comp()const{return value_compare(t.key_comp());}
    iterator begin(){return t.begin();}
    const_iterator begin()const{return t.begin();}
    iterator end(){return t.end();}
    const_iterator end()const{return t.end();}
    reverse_iterator rbegin(){return t.rbegin();}
    const_reverse_iterator rbegin()const{return t.rbegin();}
    reverse_iterator rend(){return t.rend();}
    const_reverse_iterator rend()const{return t.rend();}
    bool empty()const{return t.empty();}
    size_type size()const{return t.size();}
    size_type max_size()const{return t.max_size();}

    T& operator[](const key_type& k)
    {
        //没有先插入获取值，有直接访问获取
        //因为rb_tree插入的原则，唯一插入一个数
        //因为这个值是通过reference传递的，所以它作为左值或右值都可以
        return (*((insert(value_type(k,T()))).first)).second;
    }

    void swap(map<Key,T,Compare,Alloc>& x){t.swap(x.t);}

    //注意以下insert操作返回的型别
    std::pair<iterator,bool> insert(const value_type& x){
        return t.insert_unique(x);
    }

    std::pair<iterator,bool>insert(iterator position,const value_type& x)
    {
        return t.insert_unique(position,x);
    }

    template<class InputIterator>
    std::pair<iterator,bool>insert(iterator first,iterator last)
    {
        return t.insert_unique(first,last);
    }

    void erase(iterator position){t.erase(position);}

    size_type erase(const key_type& x){return erase(x);}

    void erase(iterator first,iterator last){t.erase(first,last);}

    void clear(){t.clear();}

    iterator find(const key_type& x){return t.find(x);}

    const_iterator find(const key_type& x)const{return t.find(x);}

    size_type count()const {return t.count();}

    iterator lower_bound(const key_type& x){return t.lower_bound(x);}
    const_iterator lower_bound(const key_type& x)const {return t.lower_bound(x);}

    iterator upper_bound(const key_type& x){return t.upper_bound(x);}
    const_iterator upper_bound(const key_type& x)const{return t.upper_bound(x);}

    std::pair<iterator,iterator>equal_range(const key_type& x){
        return t.equal_range(x);
    }

    std::pair<const_iterator,const_iterator>equal_range(const key_type& x)const{
        return t.equal_range(x);
    }

    friend bool operator==(const map&,const map&);
    friend bool operator<(const map&,const map&);
};

template<class Key,class T,class Compare,class Alloc>
inline bool operator==(const map<Key,T,Compare,Alloc>& x,const map<Key,T,Compare,Alloc>& y)
{
    return x.t == y.t;
};

template<class Key,class T,class Compare,class Alloc>
inline bool operator<(const map<Key,T,Compare,Alloc>& x,const map<Key,T,Compare,Alloc>& y)
{
    return x.t < y.t;
};

//multiset的特性和set完全一致，唯一的差别在于它允许键值重复，因此它从底层插入用的就是insert_equal()而非insert_unique()
//multimap的特性和map完全一致，唯一的差别在于插入操作使用的是底层机制的insert_equal而非insert_unique

//hashtable,在线性时间内操作元素
//核心内容是散列函数，但是需要解决的是散列冲突问题，方法是线性检测，二次探测，开链等
//线性探索 -- 负载系数，总是处于0-1之间，意指元素个数除以表格大小，除非采用开链的策略，元素的删除，实际上是对元素进行删除标记，实际
//删除则待表格重新整理时再进行彻底删除
//二次探索，按照H+1^2,H+2^2,H+3^2...H+i^2
//开链表，遇到相同的映射，直接推入到链表中

//hash_table的节点定义
template<class Value>
struct __hashtable_node
{
    __hashtable_node* next;
    Value val;
};

//hashtable的迭代器
template<class Value,class Key,class HashFun,class ExtractKey,class EqualKey,class Alloc>
struct __hashtable_iterator
{
    typedef hashtable<Value,Key,HashFun,ExtractKey,EqualKey,Alloc> hashtable;
    typedef __hashtable_iterator<Value,Key,HashFun,ExtractKey,EqualKey,Alloc> iterator;
    typedef __hashtable_const_iterator<Value,KEy,HashFun,ExtractKey,EqualKey,Alloc> const_iterator;
    typedef __hashtable_node<Value> node;

    //从迭代器类型就可以看出来，只有前向迭代器，没有反向迭代器
    typedef forward_iterator_tag iterator_category;
    typedef Value value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef Value& reference;
    typedef Value* pointer;

    node* cur; //迭代器当前所值节点
    hashtable* ht; //保存容器的连结性，因为需要从bucket跳到bucket
    __hashtable_iterator(node* n,hashtable* tab):cur(n),ht(tab){};
    __hashtable_iterator(){};
    reference operator*()const{return cur->val;}
    pointer operator->()const{return &(operator*());}
    iterator& operator++();
    iterator operator++(int);
    bool operator==(const iterator& it)const{return cur == it.cur;}
    bool operator!=(const iterator& it)const{return cur != it.cur;}
};
template<class V,class K,class HF,class ExK,class EqK,class A>
__hashtable_iterator<V,K,HF,ExK,EqK,A>&
__hashtable_iterator<V,K,HF,ExK,EqK,A>::operator++()
{
    const node* old = cur;
    cur = cur->next; //如果存在，就是当前值
    //如果已经到达底部，则下一个bucket的头就是我们的目的地
    if(!cur)
    {
        size_type bucket = ht->bkt_num(old->val);
        while(!cur && ++bucket < ht->buckets.size())
            cur = ht->buckets(bucket);
    }
    return *this;
};

template<class V,class K,class HF,class ExK,class EqK,class A>
__hashtable_iterator<V,K,HF,ExK,EqK,A>
__hashtable_iterator<V,K,HF,ExK,EqK,A>::operator++(int)
{
    iterator tmp = *this;
    ++*this;
    return tmp;
};

//hashtable的数据结构
//模板说明：Value为节点的实际值类别，Key为键值的类别，HashFun散列函数，ExtractKey从节点取出键值的方法，EqualKey判断键值是否相同的方法，Alloc空间分配器
template<class Value,class Key,class HashFun,class ExtractKey,class EqualKey, class Alloc>
class hashtable{
public:
    typedef HashFun hasher; //哈希函数对象
    typedef EqualKey key_equal; 
    typedef size_t size_type;
private:
    hasher hash;
    key_equal equals;
    ExtractKey get_key;

    typedef __hashtable_node<Value> node;
    typedef simple_alloc<node,Alloc> node_allocator;
    std::vector<node*,Alloc> buckets; //用vector保存对应的所有桶
    size_type num_elements; //总共的桶的数量
public:
    size_type bucket_count()const{return buckets.size();}

    //虽然开链法没有要求大小必须为质数，但是STL中仍然使用质数来设置大小
    static const int _stl_num_primes = 28;
    static const unsigned long __stl_prime_list[_stl_num_primes]=
    {
        53,97,193,389,769,1543,3097,6151,12289,24593,49157,98317,196613,393241,786433
    };

    inline unsigned long __stl_next_prime(unsigned long n)
    {
        const unsigned long* first = __stl_prime_list;
        const unsigned long* last = __stl_prime_list + _stl_num_primes;
        const unsigned long* pos = std::lower_bound(first,last,n);
        return pos == last ?*(last-1):*pos;
    };
    size_type max_bucket_count()const
    {
        return __stl_prime_list[_stl_num_primes-1];
    }

    node* new_node(const value_type& x)
    {
        node* n = node_allocator::allocate();
        n->next = 0;
        try
        {
            construct(&n->val,x);
            return n;
        }
        catch(const std::exception& e)
        {
            node_allocator::deallocate();
        }
    }

    void delete_node(node* n)
    {
        destroy(&n->val);
        node_allocator::deallocate(n);
    }

    size_type next_size(size_type n)const{return __stl_next_prime(n);}
    
    hashtable(size_type n,const HashFun& hf,const EqualKey& eql):hash(hf),equals(eql),get_key(ExtractKey()),num_elements(0)
    {
        initialize_buckets(n);
    }

    void initialize_buckets(size_type n)
    {
        const size_type n_buckets = next_size(n);
        buckets.reserve(n_buckets);
        buckets.insert(buckets.end(),n_buckets,(node*)0);
        num_elements = 0;
    }

    std::pair<iterator,bool> insert_unique(const value_type& obj)
    {
        resize(num_elements + 1);//判断是否需要重建表格，如需要就扩充
        return insert_unique_noresize(obj);
    }

    //292
};


};