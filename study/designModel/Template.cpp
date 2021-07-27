/* 模板方法模型Template Method */
/* 定义一个操作中的算法的骨架，将一些步骤延迟到子类，使得子类可以不修改算法结构即可重定义改算法的某些特定步骤 */
/* 主流程不变，改变的是子类特化的流程 */
class CHummerModel{
public:
    CHummerModel(void);
    virtual ~CHummerModel(void);
    void Run(){
        /* 发动汽车 */
        Start();
        /* 引擎开始轰鸣 */
        EngineBoom();
        /* 判断是否有喇叭 */
        if (IsAlarm())
            Alarm();
        /* 到达目的地，停车 */
        Stop();  
    };
protected:
    virtual void Start()=0;
    virtual void Stop()=0;
    virtual void Alarm()=0;
    virtual void EngineBoom()=0;
    virtual bool IsAlarm()=0;
};

class CHummerModel1:public CHummerModel{
public:
    CHummerModel1(void);
    ~CHummerModel1(void);
    void Start();
    void Stop();
    void Alarm();
    void EngineBoom();
    bool IsAlarm(){
        return m_IsAlarm;
    };
    void SetAlarm(bool flag){
        m_IsAlarm = flag;    
    };
private:
    bool m_IsAlarm;
};