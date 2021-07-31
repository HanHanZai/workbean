/* Iterator迭代器模式 */
/* 提供一种方法顺序访问一个聚合对象中各个元素，而不需要暴露该对象的内部实现表示，通过next函数，可以遍历这个数组的所有元素 */
#include<string>
#include<vector>
using namespace std;
class IProjectIterator;
class IProject{
public:
    IProject(void);
    virtual ~IProject(void);
    virtual void Add(string name,int num,int cost)=0;
    virtual string GetProjectInfo() = 0;
    virtual IProjectIterator* GetIterator() = 0;
    virtual void Erase() = 0;
private:
    string m_name;
    int m_num;
    int m_cost;
    vector<IProject*> m_projectList;
};

/* 迭代器接口 */
class IIterator{
public:
    IIterator(void){};
    virtual ~IIterator(void){};
    virtual bool HasNext() = 0;
    virtual IProject* Next() = 0;
};

/* 产品迭代器接口 */
class IProjectIterator:public IIterator{
public:
    IProjectIterator(void){};
    virtual ~IProjectIterator(void){};
    virtual bool hasNext() = 0;
    virtual IProject* Next() = 0;
};

/* 产品迭代器实现类 */
class CProjectIterator:public IProjectIterator{
public:
    CProjectIterator(vector<IProject*>pl):m_projectList(pl){
        m_currentItem = 0;
    };
    ~CProjectIterator(void);
    bool HasNext(){
        bool bFlag = true;
        if (m_currentItem >= m_projectList.size()){
            bFlag = false;
        }
        return bFlag;
    };
    IProject* Next(){
        IProject* pp = m_projectList[m_currentItem++];
        return pp;
    };
private:
    vector<IProject*>m_projectList;
    size_t m_currentItem;
};
