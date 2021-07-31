/* 组合模式 */
/* 将对象组合成树形结构以表示"部分-整体"的层次结构，Composite使得用户对单个对象和组合的使用具有一致性 */
#include<string>
#include<vector>
using namespace std;
/* 组合模式主要是实现在CBranchNode对象里增加对其他对象的数组，如vector */
/* 实现has-a */
class CCorpNode{
public:
    CCorpNode();
    CCorpNode(string name,string _pos,int _salary):m_name(name),m_position(_pos),m_salary(_salary){};
    virtual string GetInfo();
    void SetParent(CCorpNode* _pParent){m_pParent = _pParent;};
    CCorpNode* GetParent(){return m_pParent;}
    virtual bool IsLeaf() = 0;
private:
    string m_name;
    string m_position;
    int m_salary;
protected:
    bool m_isLeaf;
    CCorpNode* m_pParent;
};

/* 树枝节点 */
class CBranchNode:public CCorpNode{
public:
    CBranchNode(void);
    CBranchNode(string name,string pos,int salary):CCorpNode(name,pos,salary){};
    ~CBranchNode(void){};
    void Add(CCorpNode* pCoreNode){m_subordinateList.push_back(pCoreNode);};
    vector<CCorpNode*>GetSubordinateInfo(){return m_subordinateList;};
    bool IsLeaf();
private:
    vector<CCorpNode*> m_subordinateList;
};

/* 叶子节点 */
class CLeafNode:public CCorpNode{
public:
    CLeafNode(void);
    CLeafNode(string name,string pos,int salary):CCorpNode(name,pos,salary){};
    ~CLeafNode(void);
    bool IsLeaf(){
        return m_isLeaf;
    };
};

