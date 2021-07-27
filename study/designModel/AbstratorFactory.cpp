/* 抽象接口，提供一个创建一系列相关或互相依赖对象的接口，而无须指定具体的类 */
class IHuman{
public:
    IHuman(void){};
    virtual ~IHuman(void){};

    virtual void Laught() = 0;
    virtual void Cry() = 0;
    virtual void Talk() = 0;
};

class IMen:public IHuman{
public:
    IMen(void);
    ~IMen(void);
    void Laught();
    void Cry();
    void Talk();
    virtual void Sexy() = 0;
};

class IWomen:public IHuman{
public:
    IWomen(void);
    ~IWomen(void);
    void Laught();
    void Cry();
    void Talk();
    virtual void Sexy() = 0;
};

class IGary:public IMen{
public:
    IGary(void);
    ~IGary(void);
    void Sexy();
};

/* 抽象工厂 */
class IAbstratorFactory{
public:
    IAbstratorFactory(void){};
    virtual ~IAbstratorFactory(void){};

    virtual IHuman* CreateMen() = 0;
    virtual IHuman* CreateWomen() = 0;
};

/* 抽象工程基类 */
template<class T>
class CStandardHumanFactory:public IAbstratorFactory{
public:
    CStandardHumanFactory(void);
    ~CStandardHumanFactory(void);

    IHuman* CreateHuman(){
        return new T;
    };
};

/* 男人工厂 */
template<class T>
class CMaleHumanFactoryL:public CStandardHumanFactory<T>{
public:
    CMaleHumanFactory(void);
    ~CMaleHumanFactory (void);
    IHuman* CreateMen(){return CreateHuman();};
    IHuman* CreateWomen(){return CreateWomen();};
};