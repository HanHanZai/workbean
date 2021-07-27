/* 门面模式 */
/* 为子系统中的一组接口提供一个一致的界面 */
/* 将复杂的逻辑封装起来，对外公开简单的接口，由客户程序调用 */
#include<string>
class ILetterProcess{
public:
    ILetterProcess(void);
    virtual ~ILetterProcess(void);
    virtual void WriteContext(std::string context) = 0;
    virtual void FillEnvelope(std::string address) = 0;
    virtual void LetterIntoEnvelope() = 0;
    virtual void SendLetter() = 0;
};

/* 信件处理实现类 */
class CLetterProcessImp:public ILetterProcess{
public:
    CLetterProcessImp(void);
    virtual ~CLetterProcessImp(void);

    void WriteContext(std::string context);
    void FillEnvelope(std::string address);
    void LetterIntoEnvelope();
    void SendLetter();
};

/* 邮局处理实现类 */
class CLetterPolice;
class CModenPostOffice{
public:
    CModenPostOffice(void);
    ~CModenPostOffice(void);
    void SendLetter(std::string context,std::string address){
        m_pLetterProcess->WriteContext(context);
        m_pLetterProcess->FillEnvelope(address);
        m_pLetterPolice->CheckLetter(m_pLetterProcess);
        m_pLetterProcess->LetterIntoEnvelope();
        m_pLetterProcess->SendLetter();
    };
private:
    ILetterProcess* m_pLetterProcess;
    CLetterPolice* m_pLetterPolice;
};

class CLetterPolice{
public:
    CLetterPolice(void);
    ~CLetterPolice(void);
    void CheckLetter(ILetterProcess* pLetterProcess){
        /* todo */
    };
};