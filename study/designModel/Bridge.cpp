/* 桥接模式 */
/* 将抽象和实现部分分离，使它们都可以独立变化，实现分离的一个方法就是增加一个类 */
/* 产品基类 */
class IProduct{
public:
    IProduct(void);
    virtual ~IProduct(void);
    virtual void BeProducted() = 0;
    virtual void BeSelled() = 0;
};

/* 产品 */
class CHouse:public IProduct{
public:
    CHouse(void);
    ~CHouse(void);
    void BeProducted(){

    };
    void BeSelled(){

    }
};

/* 产品 衣服 */
class CCloth:public IProduct{
public:
    CCloth();
    ~CCloth();
    void BeProducted(){

    };

    void BeSelled(){

    };
};

/* 产品ipod */
class CIpod:public IProduct{
public:
    CIpod(void);
    ~CIpod(void);
    void BeProducted(){

    };
    void BeSelled(){

    };
};

/* 桥梁类 */
class CNewCorp{
public:
    CNewCorp(IProduct* pProduct){
        m_pProduct = pProduct;
    }
    ~CNewCorp(){
    }

    void MakeMoney(){
        m_pProduct->BeProducted();
        m_pProduct->BeSelled();
    };

private:
    IProduct* m_pProduct;
};

/* 只卖房子的类 */
class CSellHouse:public CNewCorp{
public:
    CSellHouse(CHouse* pHouse):CNewCorp(pHouse){
    };
    ~CSellHouse(){

    };

    void MakeMoney(){
        CNewCorp::MakeMoney();
    };
};