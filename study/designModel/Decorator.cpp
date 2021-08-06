/* Decorator装饰器模式 */
/* 装饰模式，动态的给一个对象额外添加一些额外的职责，增加功能来说，Decorator模式相对于子类更为灵活 */
/* 子类中新增额外的一些需求接口 */
/* 单例抽象类 */
#include<string>
class ISchoolReport
{
public:
    ISchoolReport(void){};
    virtual ~ISchoolReport(void){};

    virtual void Report() = 0;
    virtual void Sign(std::string name) = 0;
};

/* 四年级的成绩单 */
class CFouthGradeSchoolReport:public ISchoolReport{
public:
    CFouthGradeSchoolReport(void);
    ~CFouthGradeSchoolReport(void);
    void Report();
    void Sign(std::string name);
};

/* 成绩单装饰器基类 */
class CReportDecorator:public ISchoolReport{
public:
    CReportDecorator(ISchoolReport* psr):m_pSchoolReport(psr){};
    virtual ~CReportDecorator(void);
    void Report(){
        m_pSchoolReport->Report();
    };

    void Sign(std::string name){
        m_pSchoolReport->Sign(name);
    };
private:
    ISchoolReport* m_pSchoolReport;
};

/* 最高分装饰器 */
class CHighScoreDecorator:public CReportDecorator{
public:
    CHighScoreDecorator(ISchoolReport* psr):CReportDecorator(psr){};
    ~CHighScoreDecorator(void);
    void Report(){
        ReportHighScore();
        CReportDecorator::Report(); 
    };
private:
    void ReportHighScore();
};

/* 成绩排名装饰器 */
class CSortDecorator:public CReportDecorator{
public:
    CSortDecorator(ISchoolReport* psr):CReportDecorator(psr){};
    ~CSortDecorator();
    void Report(){
        CReportDecorator::Report();
        ReportSort();
    };
private:
    void ReportSort();
};