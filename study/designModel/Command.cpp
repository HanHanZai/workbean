/* Command命令模式，将一个请求封装为一个对象，从而使得你可用不同的请求对客户进行参数化 */
/* 说明：客户只需要知道向Invoker发出命令,而不是直接执行 */
/* 命令接收者 */
class ICommand;
class CInvoker{
public:
    CInvoker(void);
    virtual ~CInvoker(void);
    void SetCommand(ICommand* pCommand){
        m_pCommand = pCommand;
    };
    void Action(){
        m_pCommand->Execute();
    };
private:
    ICommand* m_pCommand;
};

/* 执行抽象基类 */
class IGroup{
public:
    IGroup(void);
    virtual ~IGroup(void);
    virtual void Find()=0;
    virtual void Add()=0;
    virtual void Delete()=0;
    virtual void Change()=0;
    virtual void Plan()=0;
};

/* 实际执行者 */
class CRequirementGroup :public IGroup{
public:
    CRequirementGroup(void);
    ~CRequirementGroup(void);
    void Find();
    void Add();
    void Delete();
    void Change();
    void Plan();
};

/* 实际执行者 */
class CPageGroup:IGroup{
public:
    CPageGroup(void);
    ~CPageGroup(void);
    void Find();
    void Add();
    void Delete();
    void Change();
    void Plan();
};

/* 命令抽象者 */
class ICommand{
public:
    ICommand(void){
        m_prg = new CRequirementGroup();
        m_pg = new CPageGroup();
    };
    virtual ~ICommand(){
        delete m_prg;
        delete m_pg;
    };

    virtual void Execute() = 0;
protected:
    CRequirementGroup* m_prg;
    CPageGroup* m_pg;
};

/* 添加执行命令 */
class CAddRequirementCommand:public ICommand{
public:
    CAddRequirementCommand(void);
    ~CAddRequirementCommand(void);
    void Execute(){
        m_prg->Find();
        m_prg->Add();
        m_prg->Plan();
    };
};

/* 删除命令 */
class CDeletePageCommand:public ICommand{
public:
    CDeletePageCommand(void);
    ~CDeletePageCommand(void);
    void Execute(){
        m_pg->Find();
        m_pg->Delete();
        m_pg->Plan();
    }
};