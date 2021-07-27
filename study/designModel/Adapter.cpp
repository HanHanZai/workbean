/* 主要用于不同系统之间的处理，是将一个类的接口转换成客户希望的另外一个接口 */
#include<string>
class IUserInfo{
public:
    IUserInfo(void);
    virtual ~IUserInfo(void);

    virtual std::string GetUserName() = 0 ;
    virtual std::string GetHomeAddress() = 0;
    virtual std::string GetMobileNumber() = 0;
    virtual std::string GetOfficeTelNumber() = 0;
    virtual std::string GetJobPosition() = 0;
    virtual std::string GetHomeTelNumber() = 0;
};

/* 系统内部实体类 */
class CUserInfo:public IUserInfo{
public:
    CUserInfo(void);
    virtual ~CUserInfo(void);

    std::string GetUserName();
    std::string GetHomeAddress();
    std::string GetMobileNumber();
    std::string GetOfficeTelNumber();
    std::string GetJobPosition();
    std::string GetHomeTelNumber();
};

class COuterUserBaseInfo;
class COuterUserHomeInfo;
class COuterUserOfficeInfo;
/* 外部系统实体 */
class COuterUser{
public:
    COuterUser(void){};
    ~COuterUser(void){};
    COuterUserBaseInfo * GetUserBaseInfo();
    COuterUserHomeInfo * GetUserHomeInfo();
    COuterUserOfficeInfo * GetUserOfficeInfo();
};

/* 外部系统实体基本信息类 */
class COuterUserBaseInfo{
public:
    COuterUserBaseInfo(void);
    ~COuterUserBaseInfo(void);
    std::string GetUserName();
    std::string GetMobileNumber();
};

/* 外部系统实体家庭信息类 */
class COuterUserHomeInfo{
public:
    COuterUserHomeInfo(void);
    ~COuterUserHomeInfo(void);
    std::string GetHomeAddress();
    std::string GetHomeTelNumber();
};

/* 外部系统实体办公信息类 */
class COuterUserOfficeInfo{
public:
    COuterUserOfficeInfo(void);
    ~COuterUserOfficeInfo(void);
    std::string GetOfficeTelNumber();
    std::string GetJobPosition();
};

/* 由UserInfo接口派生的实体类，并引入外部系统实体的实例，起到适配作用 */
class COuterUserInfo:public IUserInfo{
public:
    COuterUserInfo(void);
    ~COuterUserInfo(void);
    std::string GetUserName(){
        COuterUserBaseInfo* pBaseInfo = m_pOuterUser->GetUserBaseInfo();
        std::string name = pBaseInfo->GetUserName();
        delete pBaseInfo;
        pBaseInfo = nullptr;
        return name;
    };
    std::string GetHomeAddress(){
        COuterUserHomeInfo* pBaseHome = m_pOuterUser->GetUserHomeInfo();
        std::string address = pBaseHome->GetHomeAddress();
        delete pBaseHome;
        pBaseHome = nullptr;
        return address;
    };
    std::string GetMobileNumber();
    std::string GetOfficeTelNumber();
    std::string GetJobPosition(){
        COuterUserOfficeInfo* pBaseOffice = m_pOuterUser->GetUserOfficeInfo();
        std::string job = pBaseOffice->GetJobPosition();
        delete pBaseOffice;
        pBaseOffice = nullptr;
        return job;
    };
    std::string GetHomeTelNumber();
private:
    COuterUser* m_pOuterUser;
}

/* 适配器模式属于结构型模型，当数据接口出现不一致的情况下才使用 */