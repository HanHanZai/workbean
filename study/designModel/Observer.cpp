/* 观察者模式，定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新 */
/* 最多允许一个对象即使观察者也是被观察者，就像数据库中的触发器一样，成为一个复杂的链就很难维护了，观察者类似于委托的处理方式 */
/* 被观察者接口 */
#include<string>
#include<vector>
using namespace std;
class IObservable{
public:
    IObservable(void){};
    virtual ~IObservable(void){};
    virtual void AddObservable(IObservable* pObserver) = 0;
    virtual void DeleteObservable(IObservable* pObserver) = 0;
    virtual void NotifyObservable(string context) = 0;
};

class IObserver;
/* 被观察者1 */
class CHanFeiZiObservable:public IObservable{
public:
    CHanFeiZiObservable(void);
    ~CHanFeiZiObservable(void);
    void AddObservable(IObserver* pObserver){
        m_observerList.push_back(pObserver);
    };
    void DeleteObservable(IObserver* pObserver){
        for (auto it:m_observerList){
            string name = it->GetName();
            if (name.compare(pObserver->GetName()) == 0){
                /* 删除节点 */
            }
        }
    };
    void NotifyObservable(string context){
        for(auto it:m_observerList){
            it->Update(context);
        }
    };
    void HaveBreakfast(){
        NotifyObservable("吃饭");
    };

    void HaveFun(){
        NotifyObservable("娱乐");
    };
private:
    vector<IObserver*> m_observerList;
    typedef vector<IObserver*>::const_iterator ObserverList_C_iterator;
};

class IObserver{
public:
    IObserver(string name){
        m_name = name;
    }
    virtual ~IObserver(){
    }
    virtual void Update(string context) = 0;
    virtual string GetName() = 0;
protected:
    string m_name;
};

/* 观察者1 */
class CLiSiObserver:public IObserver{
public:
    CLiSiObserver(void):IObserver("李斯"){};
    ~CLiSiObserver(void);
    void Update(string context){
        ReportToQinShiHuang(context);
    };
    string GetName(){
        return m_name;
    };
private:
    void ReportToQinShiHuang(string report);
};

/* 观察者2 */
class CZhouSiObserver:public IObserver{
public:
    CZhouSiObserver(void):IObserver("周斯"){};
    ~CZhouSiObserver(void);
    void Update(string context){
        Cry(context);
    };
    string GetName(){
        return m_name;
    }
private:
    void Cry(string report);
};