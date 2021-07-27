/* 将一个复杂对象的构建与它的表示分离，使得同样的构建过程可以创建不同的表示 */
/* 和抽象工程很类似，但是抽象工厂重视生产出不同型号的产品，而建造者更注重流程 */
#include<string>
#include<vector>
class CCarModel
{
public:
    CCarModel(void);
    virtual ~CCarModel(void);
    void Run(){
        for(auto it:*m_pSequence){
            std::string actionName = it;
            if (!actionName.compare("start")){
                Start();
            }
            else if(!actionName.compare("stop")){
                Stop();
            }
            else if(!actionName.compare("alarm")){
                Alarm();
            }
        }
    };
    void SetSequence(std::vector<std::string>* pSeq){
        m_pSequence = pSeq;
    };
protected:
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Alarm() = 0;
    virtual void EngineBoom() = 0;
private:
    std::vector<std::string>* m_pSequence;
};

class CCarModel1:public CCarModel{
public:
    CCarModel1(void){};
    ~CCarModel1(void){};
protected:
    void Start();
    void Stop();
    void Alarm();
    void EngineBoom();
};

/* 建造者基类 */
class ICarBuilder{
public:
    ICarBuilder(void);
    virtual ~ICarBuilder(void);
    virtual void SetSequence(std::vector<std::string>* pSeq) = 0;
    virtual CCarModel* GetCarModel() = 0;
};

/* 实例建造者 */
class CCarBuilder:public ICarBuilder{
public:
    CCarBuilder(void){
        m_pCar = new CCarModel1();
    };
    ~CCarBuilder(void){
        delete m_pCar;
    };

    void SetSequence(std::vector<std::string>* pSeq){
        m_pCar->SetSequence(pSeq);
    };

    CCarModel* GetCarModel(){
        return m_pCar;
    };
private:
    CCarModel* m_pCar;
};


/* 指导者 */
class CDirector{
public:
    CDirector(void){
        m_pBuilder = new CCarBuilder();
    };
    ~CDirector(void){
        delete m_pBuilder;
    };
    CCarModel1* GetCarModel1(){
        return dynamic_cast<CCarModel1*>(m_pBuilder->GetCarModel());
    };
private:
    std::vector<std::string>* m_pSequence;
    CCarBuilder* m_pBuilder;
};