/* 单例模式，系统种只会存在这个类的一个实例对象，也是用的最多的一种模式 */

/* 懒汉 */
class CSingleModel{
public:
    CSingleModel();
    virtual ~CSingleModel();
    static CSingleModel* GetInstance(){
        if (!m_instance){
            m_instance = new CSingleModel();
        }
        return m_instance;
    };
private:
    static CSingleModel* m_instance;
};
CSingleModel* CSingleModel::m_instance = nullptr;


/* 饿汉 */
class CSingleModelEx{
public:
    CSingleModelEx();
    virtual ~CSingleModelEx();
private:
    static CSingleModelEx* m_instance;
};

CSingleModelEx* CSingleModelEx::m_instance = new CSingleModelEx();

/* 确保实例对象中只有一个对象存在，但是在多线程中，如果由共同访问的资源，切记进行资源同步处理 */