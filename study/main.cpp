#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<string>
#include<stack>
using namespace std;

class Solution
{
public:
    vector<vector<string> > partition(string s) {
        int nSize = s.size();
        vector<vector<bool> > status(nSize,vector<bool>(nSize,true));

        //获取回文子串的状态
        for(int i = nSize-1;i>=0;i--)
            for(int j = i+1;j<nSize;j++)
                status[i][j] = status[i+1][j-1] && (s[i] == s[j]);
        
        //保存临时子串
        vector<string> item;
        //保存符合一次遍历的结果
        vector<vector<string> > vecRes;
        dfs(s,0,nSize,status,item,vecRes);
        return vecRes;
    }

    void dfs(string s,int nIndex,int nSize,vector<vector<bool> > status,vector<string>nVal,vector<vector<string> >& vecRes)
    {
        if (nIndex == nSize)
        {
            vecRes.push_back(nVal);
            return;
        }

        for(int i = nIndex;i < nSize;i++)
        {
            //如果当前子串符合一个回文子串，那么此时就继续往下进行遍历
            if (status[nIndex][i])
            {
                nVal.push_back(s.substr(nIndex,i-nIndex+1));
                dfs(s,i+1,nSize,status,nVal,vecRes);
                nVal.pop_back();
            }
        }
    }

    /* 计算器包含+,-,*,/,(,)符号*/
    /* 根据优先级,括号的优先级要高于乘除法。最后其实就是一个加法的合并，唯一要确定的是，括号之前可以包含符号的 */
    /* 括号前面总是存在对应的符号，因为存在嵌套的括号，所以嵌套的符号需要进行翻转 */
    int calculate(string s) {
        stack<int> res;
        int sign = 1;
        int i = 0;
        int nSize = s.size();
        /* 记录的是当前的总值 */
        int nRet = 0; 
        int num = 0;
        while(i < nSize)
        {
            if(s[i] == ' ') continue;
            if(isdigit(s[i]))
            {
                num = 0;
                while(i < nSize && s[i] >= '0' && s[i] <= '9')
                {
                    num = 10*num + s[i] - '0';
                    i++;
                }
                nRet += num*sign;
            }
            else if(s[i] == '+')
            {
                sign = 1;
                i++;
            }
            else if(s[i] == '-')
            {
                sign = -1;
                i++;
            }
            else if(s[i] == '(')
            {
                res.push(nRet);
                nRet = 0;
                res.push(sign);
                sign = 1;
                i++;
            }
            else if(s[i] == ')')
            {
                int nFirst = res.top();
                res.pop();
                int nSecond = res.top();
                res.pop();
                nRet = nFirst * nRet + nSecond;
                i++;
            }
        }
        return nRet;
    }
    vector<int> spiralOrder(vector<vector<int> >& matrix) {
        int nVerSize = matrix.size();
        if (nVerSize == 0) return {};
        int nHorSize = matrix[0].size();
        vector<vector<int> >flag(nVerSize,vector<int>(nHorSize,0));
        int nDirection = 0;
        int nCount = nVerSize * nHorSize;
        int i =0;
        int j = 0;
        vector<int> res;
        while(nCount)
        {
            //向右移动
            if (nDirection == 0)
            {
                if(flag[i][j])
                {
                    nDirection = (nDirection+1)%4;
                    i++;
                    continue;
                }
                flag[i][j] = 1;
                res.push_back(matrix[i][j]);
                nCount--;
                if (j == nHorSize - 1)
                {
                    nDirection = (nDirection + 1) % 4;
                    i++;
                }
                else
                    j++;
            }
            else if(nDirection == 1)
            {
                if(flag[i][j])
                {
                    nDirection =(nDirection+1)%4;
                    j--;
                    continue;
                }
                flag[i][j] = 1;
                res.push_back(matrix[i][j]);
                nCount--;
                if(i == nVerSize - 1)
                {
                    nDirection = (nDirection + 1) % 4;
                    j--;
                }
                else
                    i++;
            }
            else if(nDirection == 2)
            {
                if(flag[i][j])
                {
                    nDirection = (nDirection + 1) % 4;
                    i--;
                    continue;
                }
                flag[i][j] = 1;
                nCount--;
                res.push_back(matrix[i][j]);
                if (j == 0)
                {
                    nDirection = (nDirection + 1) %4;
                    i--;
                }
                else
                    j--;
            }
            else if(nDirection == 3)
            {
                if(flag[i][j])
                {
                    nDirection = (nDirection + 1) %4;
                    j++;
                    continue;
                }
                flag[i][j] = 1;
                nCount--;
                res.push_back(matrix[i][j]);
                if (i == 0)
                {
                    nDirection = (nDirection+1)%4;
                    j++;
                }
                else
                    i--;
            }
        }
        return res;
    }
};
#include<deque>
int main()
{
    Solution s;
    vector<vector<int> > vecRes;
    for(int i=0;i<3;i++)
    {
        vector<int> item;
        for(int j = 0;j<4;j++)
            item.push_back((i+1)*(j+1));
        vecRes.push_back(item);
    }
    s.spiralOrder(vecRes);
    system("pause");
    return 0;
}