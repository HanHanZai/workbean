/* Strategy策略模式 */
/* 提供一个抽象接口类，通过动态多态，子类实例转换为父类，通过统一调用父类接口 */
/* 纯抽象类，用于继承 */
/* 一个袋子只装一种水果 */
/* 对象行为模式 */
class IStrategy{
public:
    IStrategy(void);
    virtual ~IStrategy();
    virtual void Operator(void) = 0;
};

/* 实例1 */
class CBackDoor:public IStrategy{
public:
    CBackDoor(void);
    ~CBackDoor();
    void Operator(void) override;
};

/* 实例2 */
class CGivenGreenLigh:public IStrategy{
public:
    CGivenGreenLigh(void);
    ~CGivenGreenLigh();
    void Operator(void) override;
};

class Context{
public:
    Context(IStrategy* pItem):m_strategy(pItem){};
    ~Context();
    void Operator(void){
        if (m_strategy){
            m_strategy->Operator();
        } 
    };
private:
    IStrategy* m_strategy;
};