#include<string>
/* 工厂模式，定义一个创建产品对象的工厂接口，将实际创建的工作推迟到子类中，核心工厂类不再负责产品的创建，让核心类成为一个抽象工厂对象 */
class IHuman{
public:
    IHuman(){};
    virtual ~IHuman(void){};
    virtual void Laugth() = 0;
    virtual void Cry() = 0;
    virtual void Talk() = 0;
};

class IMen:public IHuman{
public:
    IMen(void){};
    ~IMen(void){};
    void Laugth(){};
    void Cry(){};
    void Talk(){};
};

class IWomen:public IHuman{
public:
    IWomen(void){};
    ~IWomen(void){};
    void Laugth(){};
    void Cry(){};
    void Talk(){};
};

/* 简单工厂 */
class CSimpleFactory{
public:
    CSimpleFactory(void);
    virtual ~CSimpleFactory(void);
    virtual IHuman* CreateHuman(std::string strType){
        if (strType.compare("Women") == 0){
            return new IWomen();
        }
        else if(strType.compare("Men") == 0){
            return new IMen();
        }
    };
};

void Dosomething(){
    CSimpleFactory* pSimple = new CSimpleFactory();

    /* men */
    IHuman* pMen = pSimple->CreateHuman("Men");
    pMen->Talk();
    delete pMen;

    /* Women */
    IHuman* pWomen = pSimple->CreateHuman("Women");
    pWomen->Talk();
    delete pWomen;

    delete pSimple;
}
