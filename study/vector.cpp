/*Alloc=alloc 默认使用分配器*/

#include"new.cpp"
template<class T,class Alloc>
class vector{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type* iterator; /* vector的迭代器就是指针对象 */
    typedef value_type& reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
protected:
    typedef simple_alloc<value_type,Alloc> data_allocator;
    iterator start;
    iterator finish;
    iterator end_of_storage;
    void insert_aux(iterator position,const T& x){};
    void deallocate(){
        if (start) data_allocator::deallocate(start,end_of_storage - start);
    }
    void fill_initialize(size_type n,const T& value){
        start = alloc_and_fill(n,value);
        finish = start + n;
        end_of_storage = finish;
    }

public:
    iterator begin(){return start;}
    iterator end(){return finish;}
    size_type size(){return size_type(end()-start());}
    size_type capacity(){return size_type(end_of_storage - start());}   
    bool empty()const{return begin() == end();}
    reference operator[](size_type n){return *(begin()+n);}

    vector():start(0),end(0).,end_of_storage(0){}
    vector(size_type n,const T& value){fill_initialize(n,value);}
    vector(int n,const T& value){fill_initialize(n,value);}
    vector(long n,const T& value){fill_initialize(n,value);}
    explicit vector(size_type n){fill_initialize(n,T());}

    ~vector(){
        destroy(start,finish);
        deallocate();
    }

    reference front(){return *begin();}
    reference back(){return *end();}

    /* 新增节点会导致迭代器失效，因为有内存分配 */
    void push_back(const T& x)
    {
        if (finish != end_of_storage)
        {
            construct(finish,x);     
            finish++;   
        }
        else
            insert_aux(end(),x);    
    }

    void pop_back()
    {
        --finish;
        destroy(finish);
    }

    /* 删除不会造成迭代器失效 */
    iterator erase(iterator position)
    {
        if(position + 1 != end())
            copy(position+1,finish,position);
        --finish;
        destroy(finish);
        return position;
    }

    void resize(size_type new_size,const T& x)
    {
        if (new_size < size())
            erase(begin()+new_size,end());
        else
            insert(end(),new_size() - size(),x);
    }

    void resize(size_type new_size) { return resize(new_size,T());}
    void clear(){erase(begin(),end());}

    iterator allocate_and_fill(size_type n,const T& x){
        iterator result = data_allocator::allocate(n);
        uninitialzed_fill_n(result,n,x);
        return result;
    }

    //最主要的内存分配的函数
    void insert_anx(iterator position,const T& x){
        if (finish != end_of_storage)
        {
            //在备用空间起始处构造一个元素，并以vector最后一个值当作初值
            construct(finish,*(finish-1));
            ++finish;
            T x_copy = x;
            copy_backward(position,finish-2,finish-1);
            *position = x_copy;
        }
        else
        {
            //无备用空间使用
            const size_type old_size = size();
            const size_type len = old_size != 0:2*old_size:1;
            //原则为如果原大小为0，那么初始大小为1，如果大小存在，那么为原大小的2倍
            //前半段保留原数据，后半段保留新数据
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try
            {
                //拷贝原vector内容拷贝到新vector
                new_finish = uninitialized_copy(start,position,new_start);
                construct(new_finish,x);
                ++new_finish;
                //将原vector的备用空间进行拷贝，不是已经无备用空间了嘛
                new_finish = uninitialized_copy(position,finish,new_finish);
            }
            catch(const std::exception& e)
            {
                destory(new_start,new_finish);
                data_allocator::deallocate(new_start,len);
                throw;
            }
            
            //析构原vector
            destory(start(),end());
            deallocate();

            //调整迭代器，指向新vector
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }

    void insert(iterator position,size_type n,const T& x){
        //如果n不等于0才进行以下操作
        if(n!=0)
        {
            //备用空间大于新增元素个数
            if(size_type(end_of_storage -finish) >= n){
                T x_copy = x;
                //计算差入节点之后的现有元素个数
                const size_type element_after = finish - position;
                iterator old_finish = finish;
                if(element_after > n){
                    //插入节点之后的现有元素个数大于新增的节点个数
                    uninitialzed_copy(finish-n,finish,finish);
                    finish+=n;
                    copy_backward(position,old_finish - n,old_finish);
                    fill(position,position+n,x_copy); //从插入点开始填入新值
                }
            }
        }
    }
};