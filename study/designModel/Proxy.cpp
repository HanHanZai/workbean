/* 代理模式，说明就只是一个中介，真正的执行者在代理的后面 */
/* 代理和所谓的执行者都派生于共同的接口，他们都有共同的函数名 */
/* 结构型模式 */
class IProxyModel{
public:
    IProxyModel(void);
    virtual ~IProxyModel(void);
    virtual void Init() = 0;
    virtual void Uninit() = 0;
};

class Boss:public IProxyModel{
public:
    Boss(IProxyModel* pItem):m_proxyModel(pItem){};
    ~Boss(void);
    void Init(){
        m_proxyModel->Init();
    };
    void Uninit(){
        m_proxyModel->Uninit();
    };
private:
    IProxyModel* m_proxyModel;
};

class Solder:public IProxyModel{
public:
    Solder(IProxyModel* pItem):m_proxyModel(pItem){};
    ~Solder(void);
    void Init(){
        m_proxyModel->Init();
    };
    void Uninit(){
        m_proxyModel->Uninit();
    };
private:
    IProxyModel* m_proxyModel;
};

