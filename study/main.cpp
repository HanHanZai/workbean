#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<string>
#include<stack>
#include<deque>
#include<queue>
#include<map>
#include<unordered_map>
#include<unordered_set>
#include<list>
#include<algorithm>
#include<set>
#include<numeric>
using namespace std;
#define EPSINON 1e-6
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() :val(0), left(NULL), right(NULL) {}
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
    TreeNode(int x, TreeNode* pLeft, TreeNode* pRight) :val(x), left(pLeft), right(pRight) {}
};

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

bool compare(vector<int> a, vector<int>b)
{
    return a[0] < b[0];
}

bool compareEx(vector<int>a, vector<int>b)
{
    return a[1] < b[1];
}

bool compareStr(string a, string b)
{
    return a < b;
}

bool CompareVec(vector<int> a, vector<int> b)
{
    return a[0] < b[0];
}

struct HashFunc {
    template<typename a,typename b>
    size_t operator()(const std::pair<a, b>& p)const {
        return std::hash<a>()(p.first) ^ std::hash<b>()(p.second);
    }
};

struct EqualKey {
    template<typename a,typename b>
    bool operator()(const std::pair<a, b>& p1, const std::pair<a, b>& p2)const {
        return p1.first == p2.first && p1.second == p2.second;
    }
};

class Solution
{
public:

    string predictPartyVictory(string senate) {
        int nSize = senate.length();
        if (nSize == 0) return "";
    }

    //买到的最大利润，需要产生到的最大利润
    //1 3 2 8 4 9
    //使用动态规划来解题把，最大利润依赖前面的选择，此时2种情况
    //分为2种情况，第i天不持有股票的最大利润，和第i天持有股票的最大利润
    //val[i][0] i天不持有股票 val[i][1] i天持有股票
    int maxProfit(vector<int>& prices, int fee) {
        int nSize = prices.size();
        if (nSize <= 1)return 0;
        vector<vector<int>> nVal(nSize, vector<int>(2, 0));
        //第一天不持有的话，说明利润为0，持有的话，利润为-price
        nVal[0][0] = 0;
        nVal[0][1] = -prices[0];
        for (int i = 1; i < nSize; i++)
        {
            nVal[i][0] = max(nVal[i - 1][0], prices[i]  - fee + nVal[i-1][1]);
            nVal[i][1] = max(nVal[i - 1][1], nVal[i - 1][0] - prices[i]);
        }
        //单调递增的话就只需要交易一次
        return max(nVal[nSize-1][0],nVal[nSize-1][1]);
    }

    int minCostClimbingStairs(vector<int>& cost)
    {
        int nSize = cost.size();
        vector<int> vecVal(nSize + 1,0);
        for (int i = 2; i <= nSize; i++)
            vecVal[i] = min(vecVal[i - 1] + cost[i - 1], vecVal[i - 2] + cost[i - 2]);
        return vecVal[nSize];
    }

    //锯齿形层序遍历，通过一个标记位来进行记录和转换
    //true从右往左，false为从左到右
    //如何判断当前层数的数据已经遍历完成,当前层遍历完成之后再推入NULL节点
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        if (!root)return{};
        queue<TreeNode*> qVal;
        qVal.push(root);
        qVal.push(NULL);
        bool bFlag = true;
        vector<vector<int>> vRes;
        vector<int> vItem;
        while (!qVal.empty())
        {
            TreeNode* pItem = qVal.front();
            qVal.pop();
            if (pItem == NULL)
            {
                if (!bFlag)
                    reverse(vItem.begin(), vItem.end());
                vRes.push_back(vItem);
                if (qVal.empty())
                    break;
                vItem.clear();
                bFlag = !bFlag;
                qVal.push(NULL);
                continue;
            }
            else
            {
                vItem.push_back(pItem->val);
                if (pItem->left)
                    qVal.push(pItem->left);
                if (pItem->right)
                    qVal.push(pItem->right);
            }
        }
        return vRes;
    }

    //有一堆石头，每块石头的重量都是正整数。每次选择2个最重的石头，如果x == y，同时粉碎，如果x!=y，此时剩下的新的石头为y-x。
    //最后只会剩下一个石头或者什么都不剩下。
    void findMaxValue(vector<int>& stones, int& a, int& b)
    {
        int nFirstIndex = -1;
        int nSecondIndex = -1;
        a = -1;
        b = -1;
        int nSize = stones.size();
        for (int i = 0; i < nSize; i++)
        {
            if (stones[i] == -1)
                continue;
            if (stones[i] >= a)
            {
                nSecondIndex = nFirstIndex;
                nFirstIndex = i;
                b = a;
                a = stones[i];
            }
            else if(stones[i] > b)
            {
                nSecondIndex = i;
                b = stones[i];
            }
        }
        if (nFirstIndex >= 0 && nFirstIndex < nSize)
            stones[nFirstIndex] = -1;
        if (nSecondIndex >= 0 && nSecondIndex < nSize)
            stones[nSecondIndex] = -1;
    }

    int lastStoneWeight(vector<int>& stones) {
        while (true)
        {
            int nFirst, nSecond;
            findMaxValue(stones, nFirst, nSecond);
            if (nFirst == -1)
                return 0;
            else if (nSecond == -1)
                return nFirst;
            else if(nFirst - nSecond > 0)
                stones.push_back(nFirst - nSecond);
        }
        return 0;
    }

    vector<vector<int>> largeGroupPositions(string s) {
        int nSize = s.length();
        int nStart = 0;
        int nEnd = -1;
        vector<vector<int>> nRes;
        for (int i = 1; i < nSize; i++)
        {
            if (s[i] == s[i - 1])
            {
                nEnd = i;
            }
            else
            {
                if (nEnd - nStart >= 2)
                    nRes.push_back(vector<int>{nStart, nEnd});
                else
                    nEnd = -1;
                nStart = i;
            }
        }
        if (nEnd - nStart >= 2)
            nRes.push_back(vector<int>{nStart, nEnd});
        return nRes;
    }

    vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values, vector<vector<string>>& queries) {
        int nSize = values.size();
        map<string, map<string, double>> nVal;
        for (int i = 0; i < nSize; i++)
        {
            auto item = equations[i];
            auto itor = nVal.find(item[0]);
            auto itor2 = nVal.find(item[1]);
            map<string, double> it;
            if (itor == nVal.end())
            {
                it.insert(make_pair(item[0], values[i]));
                nVal.insert(make_pair(item[0], it));
            }
            if (itor2 == nVal.end())
            {
                it.insert(make_pair(item[1], 1 / values[i]));
                nVal.insert(make_pair(item[1], it));
            }
        }

        int nResSize = queries.size();
        vector<double>nResult;
        for (int i = 0; i < nResSize; i++)
        {
            vector<string> it = queries[i];
            auto itor = nVal.find(it[0]);
            auto itor2 = nVal.find(it[1]);
            if (it[0] == it[1])
            {
                nResult.push_back(1);
                continue;
            }
            else if (itor == nVal.end() || itor2 == nVal.end())
            {
                nResult.push_back(-1);
                continue;
            }
            else
            {
                double dVal = 1;
                map<string, int> flag;
                int bFlag = 1;
                for (auto item : nVal[it[0]])
                {
                    flag.clear();
                    for (auto it : nVal)
                        flag.insert(make_pair(it.first, 0));
                    dVal = item.second;
                    GetCalValue(nVal, item.first, it[1], flag, dVal, bFlag);
                    if (bFlag) break;
                }
                nResult.push_back(dVal);
            }
        }
        return nResult;
    }

    void GetCalValue(map<string,map<string,double>>& nVal,string src,string& dst,map<string,int> flag, double dRes,int& bFlag)
    {
        if (bFlag)return;
        if (flag[src] == 1) return;
        if (src == dst)
        {
            bFlag = true;
            return;
        }

        auto it = nVal.find(src);
        if (it != nVal.end())
            return;
        map<string, double> mVal = nVal[src];
        flag[src] = 1;
        for (auto it : mVal)
        {
            dRes *= it.second;
            GetCalValue(nVal, it.first, dst, flag, dRes, bFlag);
            dRes /= it.second;
        }
    }

    int findCircleNum(vector<vector<int>>& isConnected) 
    {
        int nSize = isConnected.size();
        map<int, vector<int>> nVal;
        vector<int> bFlag(nSize, 0);
        for (int i = 0; i < nSize; i++)
        {
            for (int j = 0; j < nSize; j++)
            {
                if (isConnected[i][j] == 1)
                {
                    auto itor = nVal.find(i);
                    if (itor == nVal.end())
                        nVal.insert(make_pair(i, vector<int>{ j }));
                    else
                        nVal[i].push_back(j);
                }
            }
        }

        int nIndex = 0;
        int nCount = 0;
        while (!IsFull(bFlag, nSize))
        {
            if (bFlag[nIndex])
            {
                nIndex++;
                continue;
            }
            GetTotalCity(nVal, nIndex, bFlag);
            nCount++;
        }
        return nCount;
    }

    bool IsFull(vector<int>& bFlag,int nSize)
    {
        for (int i = 0; i < nSize; i++)
            if (!bFlag[i])return false;
        return true;
    }

    void GetTotalCity(map<int, vector<int>>& nVal, int nIndex, vector<int>& bFlag)
    {
        if (bFlag[nIndex]) return;

        bFlag[nIndex] = 1;
        auto itor = nVal.find(nIndex);
        if (itor == nVal.end()) return;
        
        int nSize = itor->second.size();
        for (int i = 0; i < nSize; i++)
            GetTotalCity(nVal, itor->second[i], bFlag);
    }

    /*
    不适用额外空间 O(1)
    [1,2,3,4,5,6,7]
    向右旋转 1 步: [7,1,2,3,4,5,6]
    向右旋转 2 步: [6,7,1,2,3,4,5]
    向右旋转 3 步: [5,6,7,1,2,3,4]
    注意超时
    */
    void rotate(vector<int>& nums, int k) {
        int nSize = nums.size();
        int nK = k % nSize;
        /* 如果为0，直接返回 */
        if (nK == 0) return;
        /* 下面只需要交换前面的顺序 */
        int nCount = 0;
        for (int i = nSize - k; i < nSize; i++)
        {
            for (int j = i; j > nCount; j--)
            {
                ::swap(nums[j], nums[j-1]);
            }
            nCount++;
        }
    }
    struct cmp
    {
       bool operator() (char a,char b)
       {
           return a > b;
       }
    };
    //交换字符获取最小的字符串
    string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
        int nSize = s.length();
        vector<int> vecRank(nSize, 0);
        vector<int> vecFather(nSize, 0);
        for (int i = 0; i < nSize; i++)
        {
            vecRank[i] = 1;
            vecFather[i] = i;
        }
        //归入并集
        int nLen = pairs.size();
        for (int i = 0; i < nLen; i++)
            merge(vecFather, vecRank, pairs[i][0], pairs[i][1]);

        for (int i = 0; i < nSize; i++)
            find(vecFather, i);
        //分离对应的集合
        map<int, priority_queue<char,vector<char>,cmp>> mapVal;
        for (int i = 0; i < nSize; i++)
        {
            auto it = mapVal.find(vecFather[i]);
            if (it == mapVal.end())
            {
                priority_queue<char, vector<char>, cmp> item;
                item.push(s[i]);
                mapVal.insert(make_pair(vecFather[i], item));
            }
            else
                mapVal[vecFather[i]].push(s[i]);
        }

        string res;
        for (int i = 0; i < nSize; i++)
        {
            char cTop = mapVal[vecFather[i]].top();
            mapVal[vecFather[i]].pop();
            res.push_back(cTop);
        }
        return res;
    }

    void merge(vector<int>& vecFather,vector<int>&vecRanker, int i, int j)
    {
        //按秩排序
        int x = find(vecFather,i);
        int y = find(vecFather, j);
        
        if (vecRanker[x] <= vecRanker[y])
            vecFather[x] = y;
        else
            vecFather[y] = x;

        if (vecRanker[x] == vecRanker[y] && x != y)
            vecRanker[y]++;
    }

    int find(vector<int>& vecFather, int i)
    {
        return vecFather[i] == i ? i : (vecFather[i] = find(vecFather,vecFather[i]));
    }

    //根据题意所得，就是寻找出第一个产生环的那条边
    vector<int> findRedundantConnection(vector<vector<int>>& edges)
    {
        int nSize = edges.size();
        vector<int> vecFather(nSize+1, 0);
        //初始化并集
        for (int i = 1; i <= nSize; i++)
            vecFather[i] = i;

        for (int i = 0; i < nSize; i++)
        {
            if (find(vecFather, edges[i][0]) != find(vecFather, edges[i][1]))
                unionObj(vecFather, edges[i][0], edges[i][1]);
            else
                return edges[i];
        }
        return{};
    }

    void unionObj(vector<int>& vecFather, int i, int j)
    {
        int x = find(vecFather, i);
        int y = find(vecFather, j);
        vecFather[y] = x;
    }

    /*
        n 块石头放置在二维平面中的一些整数坐标点上。每个坐标点上最多只能有一块石头。
        如果一块石头的 同行或者同列 上有其他石头存在，那么就可以移除这块石头。
        给你一个长度为 n 的数组 stones ，其中 stones[i] = [xi, yi] 表示第 i 块石头的位置，返回 可以移除的石子 的最大数量。
    */

    /* 使用并查集，从不同根节点中取出对应的节点数-1就是答案 */
    int removeStones(vector<vector<int>>& stones)
    {
        int nSize = stones.size();
        if (nSize < 0)return 0;
        vector<int>father;
        vector<int>ranker;
        for (int i = 0; i < nSize; i++)
        {
            father.push_back(i);
            ranker.push_back(1);
        }

        for (int i = 0; i < nSize; i++)
        {
            for (int j = i + 1; j < nSize; j++)
            {
                if (stones[i][0] == stones[j][0] || stones[i][1] == stones[j][1])
                    merge(father, ranker, i, j);
            }
        }

        int nCount = 0;
        for (int i = 0; i < nSize; i++)
        {
            if (i == father[i]) nCount++;
        }
        return nSize - nCount;
    }
    
    int maximumProduct(vector<int>& nums) {
        int nSize = nums.size();
        sort(nums.begin(), nums.end());
        int nMaxVal = INT_MIN;
        int nProv = nums[0] * nums[1];
        int nLast = nums[nSize - 1] * nums[nSize - 2];
        for (int i = 2; i < nSize; i++)
        {
            int nVal = nProv * nums[i];
            nMaxVal = nVal > nMaxVal ? nVal : nMaxVal;
        }

        for (int i = 0; i < nSize - 2; i++)
        {
            int nVal = nLast*nums[i];
            nMaxVal = nVal > nMaxVal ? nVal : nMaxVal;
        }
        return nMaxVal;
    }

    /* 在一个1*1矩阵中划分N*N个矩阵，判断当前1*1矩阵中有多少个区域 */
    /* 包括2次合并，一次是单元格内的合并，一次是单元格间的合并，当为空格时，合并4个小三角形，当为斜杠时，0，3合并，1，2合并，为反斜杠时，0，1合并，2，3合并，空格时，合并所有小三角形 */
    int SpliteMatrix(vector<string>& grid)
    {
        int nSize = grid.size();
        if (nSize == 0) return 0;
        /* 如果有N个项，说明存在N*N个单元格，一共存在4*N*N个三角形 */
        int nTotalsize = 4 * nSize * nSize;
        vector<int>vecfather;
        vector<int>vecranker;
        /* 初始化每个节点 */
        for (int i = 0; i < nTotalsize; i++)
        {
            vecfather.push_back(i);
            vecranker.push_back(1);
        }

        for (int i = 0; i < nSize; i++)
        {
            for (int j = 0; j < nSize; j++)
            {
                char item = grid[i][j];
                /* 单元格内合并 */
                if (item == '/ ')
                {

                }
                else if (item == '\\')
                {

                }
                else
                {

                }
            }
        }

        return 0;
    }

    int numEquivDominoPairs(vector<vector<int>>& dominoes)
    {
        int nSize = dominoes.size();
        unordered_map<int, int> mapVal;
        for (int i = 0; i < nSize; i++)
        {
            int nFirst = dominoes[i][0] * 10 + dominoes[i][1];
            int nSecond = dominoes[i][1] * 10 + dominoes[i][0];
            auto firstItem = mapVal.find(nFirst);
            auto secondItem = mapVal.find(nSecond);
            if (firstItem == mapVal.end() && secondItem == mapVal.end())
                mapVal.insert(make_pair(nFirst, 1));
            else if (firstItem != mapVal.end())
                mapVal[nFirst]++;
            else if (secondItem != mapVal.end())
                mapVal[nSecond]++;
        }

        int nCount = 0;
        for (auto it : mapVal)
        {
            if (it.second > 1)
                nCount += (it.second * (it.second - 1)) / 2;
        }
        return nCount;
    }

    int pivotIndex(vector<int>& nums)
    {
        int nSize = nums.size();
        vector<int>prev(nSize,0);
        vector<int>next(nSize,0);
        prev[0]=nums[0];
        next[nSize-1]=nums[nSize - 1];
        for (int i = 1; i < nSize; i++)
        {
            prev[i] = prev[i - 1] + nums[i];
            next[nSize-1-i] = next[nSize - i] + nums[nSize - 1 - i];
        }
        int i = 0;
        for (; i < nSize; i++)
            if (prev[i] == next[i])
                break;

        return i >= nSize ? -1 : i;
    }

    /* 最小耗费体力的方式 */
    /* 从(0,0)开始到(width-1,height-1) */
    /* 使用dfs+减支 */
    /* 不能只考虑右下，需要考虑上下左右 */
    int minimumEffortPath(vector<vector<int>>& heights) {
        int nRowSize = heights.size();
        if (nRowSize == 0) return 0;
        int nColSize = heights[0].size();
        //是否已经访问
        vector<vector<int>>vecFlag( nRowSize,vector<int>(nColSize,0) );
        int nRes = INT_MAX;
        dfs(heights, vecFlag, nRowSize, nColSize, 0, 0, nRes, INT_MIN);
        return nRes;
    }

    void dfs(vector<vector<int>>& heights, vector<vector<int>> vecFlag, int nRow, int nCol, int nX, int nY, int& nCount, int nCurrentCount)
    {
        if (vecFlag[nX][nY]) return;
        if (nX == nRow - 1 && nY == nCol - 1)
        {
            nCount = nCurrentCount > nCount?nCount:nCurrentCount;
            return;
        }

        vecFlag[nX][nY] = 1;
        vector<vector<int>> val{ {1,0},{0,1},{-1,0},{0,-1} };
        for (auto it : val)
        {
            int x = nX + it[0];
            int y = nY + it[1];
            if (x >= 0 && x < nRow && y >= 0 && y < nCol)
            {
                int nVal = abs(heights[x][y] - heights[nX][nY]);
                nCurrentCount = nCurrentCount < nVal?nVal:nCurrentCount;
                dfs(heights, vecFlag, nRow, nCol, x, y, nCount, nCurrentCount);
            }
        }
    }

    /* 平均分配糖果，返回值是一个vector，其中vector[0]是alice要提供的，vector[1]是bob要提供的 */
    vector<int> fairCandySwap(vector<int>& A, vector<int>& B) {
        int nASize = A.size();
        int nBisze = B.size();
        unordered_map<int, int> mapA;
        unordered_map<int, int> mapB;
        int nATotal = 0;
        int nBTotal = 0;
        for (int i = 0; i < nASize; i++)
        {
            nATotal += A[i];
            if (mapA.find(A[i]) == mapA.end())
                mapA.insert(make_pair(A[i],1));
        }

        for (int i = 0; i < nBisze; i++)
        {
            nBTotal += B[i];
            if (mapB.find(B[i]) == mapB.end())
                mapB.insert(make_pair(B[i],1));
        }

        vector<int>nRes;
        if (nATotal < nBTotal)
            FindVal(mapA, mapB, nATotal, nBTotal, nRes);
        else
        {
            FindVal(mapB, mapA, nBTotal, nATotal, nRes);
            reverse(nRes.begin(), nRes.end());
        }
        return nRes;
    }

    void FindVal(unordered_map<int, int>& mapA, unordered_map<int, int>& mapB, int nATotal,int nBTotal, vector<int>& vecRes)
    {
        int nHalf = (nATotal + nBTotal) >> 1;
        for (auto it : mapA)
        {
            int nVal = nHalf - (nATotal - it.first);
            if (mapB.find(nVal) != mapB.end())
            {
                vecRes.push_back(it.first);
                vecRes.push_back(nVal);
                return;
            }
        }
    }

    /* 可以替换k次，得到一个最大的连续字符串 */
    int GetMaxReplace(string& str, int k)
    {

    }

    //使用滑动窗口进行遍历，X就是窗口大小
    //获取到窗口中最大的那个值大小
    //increase[i] = increase[i-1] + customers[i]*grumpy[i] - customers[i-x]*grumpy[i-x];
    int maxSatisfied(vector<int>& customers, vector<int>& grumpy, int X) 
    {
        int nSize = customers.size();
        int nTotal = 0;
        for (int i = 0; i < nSize; i++) nTotal += customers[i] * (1 - grumpy[i]);
        int increase = 0;
        for (int i = 0; i < X; i++) increase += customers[i]*grumpy[i];
        int nMax = increase;
        for (int i = X; i < nSize; i++)
        {
            increase = increase + customers[i] * grumpy[i] - customers[i - X] * grumpy[i - X];
            nMax = max(nMax, increase);
        }
        return nMax + nTotal;
    }

    /* 困难题 */
    /* 猜字谜游戏，有谜面和谜底两个部分，谜底中包含谜面的第一个字符，谜底的字符在谜面中都存在 */
    /* 先将谜底进行划分 */
    vector<int> findNumOfValidWords(vector<string>& words, vector<string>& puzzles) 
    {
        map<string, vector<int>>mapWord;
    }

    //给定一个非负整数 num。对于 0 ≤ i ≤ num 范围中的每个数字 i ，计算其二进制数中的 1 的数目并将它们作为数组返回。
    //要求时间复杂度为O(n),空间复杂度为O(n);
    //可以根据动态规划来进行划分

    vector<int> countBits(int num) {
        vector<int> vecRes(num+1,0);
        int highBit = 0;
        for (int i = 1; i <= num; i++)
        {
            if ((i&(i - 1)) == 0)
                highBit = i;
            vecRes[i] = vecRes[i - highBit] + 1;  
        }
        return vecRes;
    }

    /* 俄罗斯套娃 
    /* 先按照w升序，h降序的条件进行排序，此时就可以忽略w，直接在h中寻找符合条件的最大子序列 */
    int maxEnvelopes(vector<vector<int>>& envelopes) {
        /* 用来记录比当前节点大的图集 */
        sort(envelopes.begin(), envelopes.end(), [](vector<int>a,vector<int>b) {
            if (a[0] < b[0])
                return true;
            else if (a[0] == b[0])
                return a[1] > b[1];
            return false;
        });
        /* 使用动态规划，用a[i]表示在i之前符合递增条件的个数，一旦符合j>i且a[j] > a[i],那么当前的值为当前值和a[i]+1中的最大值 */
        int nSize = envelopes.size();
        vector<int> nRes(nSize, 1);
        for (int i = 1; i < nSize; i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (envelopes[j][1] < envelopes[i][1])
                    nRes[i] = max(nRes[j] + 1, nRes[i]);
            }
        }

        int nCount = 0;
        for (int i = 0; i < nSize; i++)
            nCount = nCount > nRes[i] ? nCount : nRes[i];

        return nCount;
    }

    /* 获取最少的分割次数 */
    int minCut(string s) {
        //先用动态组记录当前区间的字符串是否为回文串
        m_nSize = s.size();
        flag.assign(m_nSize, vector<bool>(m_nSize, true));
        for (int i = m_nSize - 1; i >= 0; i--)
            for (int j = i + 1; j < m_nSize; j++)
                flag[i][j] = flag[i + 1][j - 1] && (s[i] == s[j]);

        dfs(s, 0);
        int nSize = ans.size();
        int nVal = INT_MAX;
        for (int i = 0; i < nSize; i++)
        {
            int nLen = ans[i].size() - 1;
            nVal = nLen < nVal ? nLen : nVal;
        }

        return nVal == INT_MAX ? 0 : nVal;
    }

    void dfs(string s, int Index)
    {
        if (Index == m_nSize)
        {
            ans.push_back(item);
            return;
        }
        for (int i = Index; i < m_nSize; i++)
        {
            if (flag[Index][i])
            {
                item.push_back(s.substr(Index, i - Index + 1));
                dfs(s, i + 1);
                item.pop_back();
            }
        }
    }

    /* 使用栈的方式来进行操作 */
    string removeDuplicates(string S) {
        string str;
        for (auto ch : S)
        {
            if (!str.empty() && str.back() == ch)
                str.pop_back();
            else
                str.push_back(ch);
        }
        return str;
    }

    int calculate(string s) {
        vector<int> stk;
        char preSign = '+';
        int num = 0;
        int nSize = s.size();
        for (int i = 0; i < nSize; i++)
        {
            if (isdigit(s[i]))
                num = 10 * num + (s[i] - '0');
            if ((!isdigit(s[i]) && s[i] != ' ') || i == nSize - 1)
            {
                switch (preSign)
                {
                case '+':
                    stk.push_back(num);
                    break;
                case '-':
                    stk.push_back(-num);
                    break;
                case '*':
                    stk.back() *= num;
                    break;
                case '/':
                    stk.back() /= num;
                    break;
                default:
                    break;
                }
                num = 0;
                preSign = s[i];
            }
        }
        return accumulate(stk.begin(), stk.end(), 0);
    }

    //出入度的差值为0说明是一个正常的二叉树
    bool isValidSerialization(string preorder) {
        int nDiff = 1;
        vector<string> vecStr;
        int nFirst = 0;
        size_t nSecond = preorder.find(',');
        while (nSecond != string::npos)
        {
            string item = preorder.substr(nFirst, nSecond - nFirst);
            vecStr.push_back(item);
            nFirst = nSecond + 1;
            nSecond = preorder.find(',', nFirst);
        }
        if (nFirst != string::npos)
            vecStr.push_back(preorder.substr(nFirst));
        /* vector保存了所有的节点信息 */
        int nSize = vecStr.size();
        for (int i = 0; i < nSize; i++)
        {
            nDiff -= 1;
            if (vecStr[i][0] == '#')
                nDiff += 0;
            else
                nDiff += 2;
            if (nDiff < 0) return false;
        }
        return nDiff == 0;
    }

    /* 螺旋矩阵m*n矩阵 */
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int nVerSize = matrix.size();
        if (nVerSize == 0) return{};
        int nHorSize = matrix[0].size();
        vector<vector<int>> flag(nVerSize, vector<int>(nHorSize, 0));
        vector<int> result;
        int nCount = nVerSize * nHorSize;
        int nDirection = 0;
        int i = 0, j = 0;
        while (nCount)
        {
            //向左
            if (nDirection == 0)
            {
                if (flag[i][j])
                {
                    nDirection = (nDirection + 1) % 4;
                    i++;
                    continue;
                }
                flag[i][j] = 1;
                nCount--;
                result.push_back(matrix[i][j]);
                if (j == nHorSize - 1)
                {
                    nDirection = (nDirection + 1) % 4;
                    i++;
                }
                else
                {
                    if (flag[i][j + 1])
                    {
                        nDirection = (nDirection + 1) % 4;
                        i++;
                        continue;
                    }
                    j++;
                }
            }
            else if (nDirection == 1) //向下
            {
                if (flag[i][j])
                {
                    nDirection = (nDirection + 1) % 4;
                    j--;
                    continue;
                }
                flag[i][j] = 1;
                nCount--;
                result.push_back(matrix[i][j]);
                if (i == nVerSize - 1)
                {
                    nDirection = (nDirection + 1) % 4;
                    j--;
                }
                else
                {
                    if (flag[i + 1][j])
                    {
                        j--;
                        nDirection = (nDirection + 1) % 4;
                        continue;
                    }
                    i++;
                }
            }
            else if (nDirection == 2)//向左
            {
                if (flag[i][j])
                {
                    nDirection = (nDirection + 1) % 4;
                    i--;
                    continue;
                }
                flag[i][j] = 1;
                nCount--;
                result.push_back(matrix[i][j]);
                if (j == 0)
                {
                    nDirection = (nDirection + 1) % 4;
                    i--;
                }
                else
                {
                    if (flag[i][j - 1])
                    {
                        nDirection = (nDirection + 1) % 4;
                        i--;
                        continue;
                    }
                    j--;
                }
            }
            else if (nDirection == 3) //向上
            {
                if (flag[i][j])
                {
                    nDirection = (nDirection + 1) % 4;
                    j++;
                    continue;
                }
                flag[i][j] = 1;
                nCount--;
                result.push_back(matrix[i][j]);
                if (i == 0)
                {
                    nDirection = (nDirection + 1) % 4;
                    j++;
                }
                else
                {
                    if (flag[i - 1][j])
                    {
                        nDirection = (nDirection + 1)%4;
                        j++;
                        continue;
                    }
                    i--;
                }
            }
        }

        return result;
    }

    vector<vector<int>> generateMatrix(int n) {
        if (n == 0) return{};
        int nCount = n*n;
        vector<vector<bool>> flag(n, vector<bool>(n, false));
        int nDirection = 0;
        vector<vector<int>>add{ { 0,1 },{ 1,0 },{ 0,-1 },{ -1,0 } };
        vector<vector<int>> vecRes(n, vector<int>(n, 0));
        int i = 0;
        int j = 0;
        int nIndex = 1;
        vecRes[0][0] = nIndex;
        flag[0][0] = true;
        nIndex++;
        nCount--;
        while (nCount)
        {
            i += add[nDirection][0];
            j += add[nDirection][1];
            if (i <0 || i >= n || j < 0 || j >= n || flag[i][j])
            {
                i -= add[nDirection][0];
                j -= add[nDirection][1];
                nDirection = (nDirection + 1) % 4;
                continue;
            }
            flag[i][j] = true;
            vecRes[i][j] = nIndex++;
            nCount--;
        }
        return vecRes;
    }

    /* 链表反转 */
    /* head是头节点,left是翻转位置的起点，right是翻转位置的终点 */
    /* a->b->c->d->e => a->d->c->b->e*/
    /* 稍微优化一下，在head前面再添加一个节点 */
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode* pList = new ListNode(-1);
        pList->next = head;
        ListNode* pre = pList->next;
        for (int i = 0; i < left - 1; i++)
        {
            //定位到第一个前序节点位置
            pre = pre->next;
        }
        ListNode* cur = pre->next; //当前节点
        ListNode* next;
        for (int i = 0; i < right - left; i++)
        {
            next = cur->next;
            cur->next = next->next;
            next->next = pre->next;
            pre->next = next;
        }
        return pList->next;
    }

    int evalRPN(vector<string>& tokens) {
        //用栈实现就好
        int nSize = tokens.size();
        if (nSize == 0) return 0;
        stack<int> nVal;
        for (int i = 0; i < nSize; i++)
        {
            if (IsNumber(tokens[i]))
                nVal.push(atoi(tokens[i].c_str()));
            else
            {
                int nSecond = nVal.top();
                nVal.pop();
                int nFirst = nVal.top();
                nVal.pop();
                switch (tokens[i][0])
                {
                case '*' :
                    nVal.push(nSecond*nFirst);
                    break;
                case '/':
                    nVal.push(nFirst / nSecond);
                    break;
                case '+':
                    nVal.push(nFirst + nSecond);
                    break;
                case '-':
                    nVal.push(nFirst / nSecond);
                    break;
                default:
                    break;
                }
            }

        }
        return nVal.size() ? nVal.top() : 0;
    }

    bool IsNumber(string& token)
    {
        return token == "*" || token == "/" || token == "+" || token == "-";
    }

    //'I','V','X','L','C','D','M'
    // 1   5   10  50  100 500 1000
    int romanToInt(string s) {
        map<char, int> mapVal;
        mapVal.insert(make_pair('I', 1));
        mapVal.insert(make_pair('V',5));
        mapVal.insert(make_pair('X', 10));
        mapVal.insert(make_pair('L', 50));
        mapVal.insert(make_pair('C', 100));
        mapVal.insert(make_pair('D', 500));
        mapVal.insert(make_pair('M', 1000));

        int nSize = s.size();
        int nTotal = 0;
        for (int i = 0; i < nSize - 1; i++)
        {
            if (IsSmall(s[i], s[i + 1]))
                nTotal -= mapVal[s[i]];
            else
                nTotal += mapVal[s[i]];
        }
        return nTotal;
    }   

    bool IsSmall(char a, char b)
    {
        if (a == 'I' && (b == 'X' || b == 'V'))
            return true;
        if (a == 'X' && (b == 'L' || b == 'C'))
            return true;
        if (a == 'C' && (b == 'D' || b == 'M'))
            return true;
        return false;
    }

    //132模式，使得i<j<k的时候，满足nums[i] < nums[k] < nums[j]
    bool find132pattern(vector<int>& nums) {
        int nSize = nums.size();
        if (nSize <= 2) return false;
        multiset<int> right;//保存j+1~n-1的值即为1值，j是当前的第三个值，从左往右进行维护就能得到最大的值，而0~j-1则是1值
        int left = nums[0];
        //保存j+1 ~ n-1的值
        for (int i = 2; i < nSize; i++)
            right.insert(nums[i]);
        for (int i = 1; i < nSize - 1; i++)
        {
            if (nums[i] > left)
            {
                auto item = right.upper_bound(nums[i]);
                if (item != right.end() && (*item) > left)
                    return true;
            }
            left = min(left, nums[i]);
            right.erase(right.find(nums[i] + 1));
        }
        return false;
    }

    uint32_t reverseInt(uint32_t nVal)
    {
        uint32_t res = 0;
        vector<int> vecRes(32,0);
        int nIndex = 0;
        while (nVal)
        {
            vecRes[nIndex] = nVal % 2;
            nVal >>= 1;
        }

        int nSize = vecRes.size();
        for (int i = 0; i < nSize; i++)
        {
            res += vecRes[i];
            res <<= 1;
        }
        return res;
    }

    //1.矩阵的每一行都是升序排列的
    //2.矩阵的第二行的第一个数要大于第一行的最后一个数
    //使用二分来确定
    //先确定列
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int nVerSize = matrix.size();
        if (nVerSize <= 0) return false;
        int nHorSize = matrix[0].size();
        if (nHorSize <= 0) return false;

        int nVerIndex = -1;
        
        //获取纵坐标
        for (int i = 0; i < nVerSize; i++)
        {
            if (matrix[i][0] == target)
                return true;
            if (matrix[i][0] < target)
                nVerIndex = i;
        }

        if (nVerIndex == -1) return false;

        int i = 0;
        int j = nHorSize - 1;
        while (i < j)
        {
            if (matrix[nVerIndex][i] == target || matrix[nVerIndex][j] == target)
                return true;
            int nMidIndex = i + (j - i) / 2;
            if (matrix[nVerIndex][nMidIndex] == target)
                return true;
            else if (matrix[nVerIndex][nMidIndex] < target)
                i = nMidIndex + 1;
            else
                j = nMidIndex - 1;
        }
        return false;
    }

    //枚举所有子集，因为有可能出现相同的数值，如果和之前的节点相同，直接跳过
    //先排序，如果遇到和之前相同的数据就不继续执行下去了，后递归
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> nRes;
        int nSize = nums.size();
        //空子集是所有集合的子集合
        vector<int> item;
        dfs(nums, false, 0, nRes, item);
        return nRes;
    }

    void dfs(vector<int>&nums, bool bUsePre, int nIndex, vector<vector<int>>& nRes, vector<int> item)
    {
        if (nIndex == nums.size())
        {
            nRes.push_back(item);
            return;
        }

        //首先考虑不选择当前对象
        dfs(nums, false, nIndex + 1, nRes, item);
        if (!bUsePre && nums[nIndex] == nums[nIndex - 1])
            return;
        item.push_back(nums[nIndex]);
        dfs(nums, true, nIndex + 1, nRes, item);
        item.pop_back();
    }

    //笨阶乘，按照加减乘除的方式进行计算
    int clumsy(int N) {
        if (N == 0) return 0;
        if (N == 1) return 1;
        //用来保存对应的正数和
        vector<int> vecRes;
        //0代表乘法，1代表除法，2代表加法，3代表减法
        int nSign = 0;
        vecRes.push_back(N);
        for (int i = N - 1; i > 0; i--)
        {
            switch (nSign)
            {
            case 0:
            {
                int nFirst = vecRes.back();
                vecRes.pop_back();
                vecRes.push_back(nFirst * i);
            }
                break;
            case 1:
            {
                int nFirst = vecRes.back();
                vecRes.pop_back();
                vecRes.push_back(nFirst / i);
            }
                break;
            case 2:
                vecRes.push_back(i);
                break;
            case 3:
                vecRes.push_back(-i);
                break;
            default:
                break;
            }
            nSign = (nSign + 1) % 4;
        }

        return accumulate(vecRes.begin(), vecRes.end(),0);
    }

    //删除重复元素
    int removeDuplicates(vector<int>& nums) {
        int nSize = nums.size();
        if (nSize == 0) return 0;
        int nCount = 1;
        int nVal = nums[0];
        for (int i = 1; i < nSize;)
        {
            if (nums[i] == nVal)
            {
                if (nCount == 2)
                {
                    //在这里移动对应的值
                    for (int j = i; j < nSize - 1; j++)
                        swap(nums[j], nums[j+1]);
                    nSize--;
                    continue;
                }
                nCount++;
            }
            else
            {
                nVal = nums[i];
                nCount = 1;
            }
            i++;
        }
        return nSize;
    }

    //从非降序数组中查找对应的值
    bool search(vector<int>& nums, int target) {
        int nSize = nums.size();
        int nIndex = 0;
        for (int i = 1; i < nSize; i++)
        {
            if (nums[i - 1] > nums[i])
            {
                nIndex = i;
                break;
            }
        }

        return findValue(nums, 0, nIndex - 1, target) || findValue(nums, nIndex, nSize - 1, target);
    }

    bool findValue(vector<int>& nums, int nStart, int nEnd,int nVal)
    {
        while (nStart <= nEnd)
        {
            if (nVal == nums[nStart] || nVal == nums[nEnd])
                return true;
            int nIndex = nStart + ((nEnd - nStart) >> 1);
            if (nums[nIndex] > nVal)
                nEnd = nIndex - 1;
            else if (nums[nIndex] < nVal)
                nStart = nIndex + 1;
            else
                return true;
        }
        return false;
    }

    //获取数组中的最小值，里面旋转过好几次
    //直接二分
    int findMin(vector<int>& nums) {
        int nSize = nums.size();
        if (nSize == 1) return nums[0];
        int nStart = 0;
        int nEnd = nSize - 1;
        while (nStart < nEnd)
        {
            int nMid = (nStart + nEnd) >> 1;
            if (nums[nMid] < nums[nEnd])
                nEnd = nMid;
            else
                nStart = nMid + 1;
        }
        return nums[nStart];
    }

    bool isUgly(int n)
    {
        if (n < 1) return false;
        //先用5整除
        while (!n % 5)
            n = n / 5;
        while (!n % 3)
            n = n / 3;
        while (!n % 2)
            n = n / 2;
        return n == 1;
    }

    //劫富济贫,用a(n)记录当前节点的最大值
    //如果没有限制，直接获取偷取的最大金额
    //通过题目要求可得，最后一间房可以选择偷窃或者不偷窃，那么此时的在该点的总值可能来源于2个部分
    //1.选择n,a(n) =  a(n-2)+nums(n) 2.不选择n,a(n) = a(n-1)
    //所以状态方程为：a(n) = max(a(n-1),a(n-2)+nums[n]);
    //但是本题中有个限制条件，如果选择了头节点，那么此时就无法选择最后一个节点，如果选择了最后一个节点就无法选择头节点
    //假设选择了第一个节点，那么最后能选择的一个节点就是a(n-2);
    int rob(vector<int>& nums) {
        int nSize = nums.size();
        if (nSize == 0) return 0;
        if (nSize == 1) return nums[0];
        if (nSize == 2) return max(nums[0], nums[1]);
        vector<vector<int>> vecVal(2,vector<int>(nSize,0));
        //vecVal[0]表示选择了第一个节点，vecVal[1]表示不选择第一个节点
        vecVal[0][0] = nums[0];
        vecVal[0][1] = max(nums[0], nums[1]);
        vecVal[1][1] = nums[1];
        vecVal[1][2] = max(nums[1], nums[2]);
        for (int i = 2; i < nSize - 1; i++)
            vecVal[0][i] = max(vecVal[0][i - 2] + nums[i], vecVal[0][i - 1]);
        for (int i = 3; i < nSize; i++)
            vecVal[1][i] = max(vecVal[1][i - 2] + nums[i], vecVal[1][i - 1]);

        return max(vecVal[0][nSize - 2], vecVal[1][nSize - 1]);
    }

    //从数组中移除指定的值
    //原地算法说的是不使用额外的空间进行操作
    int removeElement(vector<int>& nums, int val) {
        int nSize = nums.size();
        int left = 0;
        int right = nSize;
        while (left < right)
        {
            if (nums[left] == val)
            {
                nums[left] = nums[right - 1];
                right--;
            }
            else
                left++;
        }
        return left;
    }

    //暴力虽然能解决问题，但是可能会超时，这里需要额外的调整
    //使用KMP算法来解题
    int strStr(string haystack, string needle) {
        int nSubSize = needle.size(); //子串长度
        int nMainSize = haystack.size(); //主串长度
        if (nSubSize == 0) return 0; //字串为空，返回0
        if (nMainSize == 0) return -1; //主串为空，返回-1
        if (nSubSize > nMainSize) return -1; //字串比主串大，返回-1
        auto it = haystack.find(needle[0]);
        while (it < nMainSize)
        {
            int j = 0;
            int i = it;
            while (j < nSubSize)
            {
                if (haystack[i] == needle[j])
                {
                    i++;
                    j++;
                }
                else
                    break;
            }
            if (j == nSubSize)return it;
            it = haystack.find(needle[0], it + 1);
        }
        return -1;
    }

    //求能出现的矩阵最大值
    //这里有额外的条件限制，那就是最大值要小于k
    int maxSumSubmatrix(vector<vector<int>> &matrix, int k) {
        int ans = INT_MIN;
        int m = matrix.size(), n = matrix[0].size();
        for (int i = 0; i < m; ++i) { // 枚举上边界
            vector<int> sum(n);
            for (int j = i; j < m; ++j) { // 枚举下边界
                for (int c = 0; c < n; ++c) {
                    sum[c] += matrix[j][c]; // 更新每列的元素和
                }
                set<int> sumSet{ 0 };
                int s = 0;
                for (int v : sum) {
                    s += v;
                    auto lb = sumSet.lower_bound(s - k);
                    if (lb != sumSet.end()) {
                        ans = max(ans, s - *lb);
                    }
                    sumSet.insert(s);
                }
            }
        }
        return ans;
    }

    //weights 记录的是所有重量，其中对应的每份货物的重量
    //按照给出的重量进行出货，说明不允许排序
    //D是对应的记录天数，这是一个限制条件
    //最小值是所有包裹中最小的那个值，最大值是所有包裹的和

    int shipWithinDays(vector<int>& weights, int D) {
        int nSize = weights.size();
        if (nSize == 0) return 0;
        //这个是所有元素的总和
        int nMax = 0; //最大值
        nMax = accumulate(weights.begin(), weights.end(), 0);
        int nMin = *max_element(weights.begin(), weights.end());
        while(nMin < nMax)
        {
            int nMid = nMin + ((nMax - nMin) >> 1);
            int nNeed = 1;
            int nCur = 0;
            for (int i = 0; i < nSize; i++)
            {
                if (nCur + weights[i] > nMid)
                {
                    nNeed++;
                    nCur = 0;
                }
                nCur += weights[i];
            }
            if (nNeed <= D)
                nMax = nMid;
            else
                nMin = nMid + 1;
        }
        return nMin;
    }

    //下限是0，上限是小于等于根号c的整数
    bool judgeSquareSum(int c) {
        int nMax = sqrt(c);
        int nMin = 0;
        while (nMin <= nMax)
        {
            long nVal = nMin*nMin + nMax*nMax;
            if (nVal > c)
                nMax--;
            else if (nVal < c)
                nMin++;
            else
                return true;
        }
        return false;
    }
    
    /* 只出现一次的数字，只使用线性时间，并且不适用额外空间 */
    /* 限制条件 */
    /* 无法使用额外空间记录对应存在的数据是否存在 */
    /* 无法通过额外时间来进行搜索，只能是线性时间n */
    /* 可以利用原始的数组来进行保存 */
    /* 问题就是如何将异或操作调整为三次亦或操作，异或操作用于处理2个数据，新的算法要处理n个以上的数据 */
    int singleNumber(vector<int>& nums) {
        int a = 0, b = 0;
        for (int num : nums) {
            b = ~a & (b ^ num);
            a = ~b & (a ^ num);
        }
        return b;
    }

    /* 解码异或后的排列 */
    /* 还有一个条件，所有数都小于等于n，且不重复 */
    /* 因为已知a~b的值了，所以a^b^c^d^e的值 */
    /* n是个偶数 */
    vector<int> decode(vector<int>& encoded) {
        int nSize = encoded.size();
        int a = 0;
        /* 获取a^b^c^d^e */
        for (int i = 1; i <= nSize + 1; i++){
            a ^= i;
        }
        
        for (int i = nSize - 1; i >= 0; i -= 2) {
            a ^= encoded[i];
        }

        vector<int> nRes;
        nRes.push_back(a);
        for (int i = 0; i < nSize; i++) {
            nRes.push_back(encoded[i] ^ nRes.back());
        }
        return nRes;
    }

    /*  */
    vector<int> xorQueries(vector<int>& arr, vector<vector<int>>& queries) {
        int nSize = arr.size();
        if (nSize == 0) return{};
        vector<int> vecVal(nSize, 0);
        vecVal[0] = arr[0];
        for (int i = 1; i < nSize; i++) {
            vecVal[i] = vecVal[i - 1] ^ arr[i];
        }
        vector<int> result;
        for (int i = 0; i < queries.size(); i++) {
            if (queries[i][0] == queries[i][1]) {
                result.push_back(arr[i]);
            }
            else if (queries[i][0] == 0) {
                result.push_back(vecVal[queries[i][1]]);
            }
            else {
                result.push_back(vecVal[queries[i][0] - 1] ^ vecVal[queries[i][1]]);
            }
        }
        return result;
    }

    //停在原地的方案数，数据总长度为arrlen，可执行的方案有3种，左移动，右移动，不动，求可以达到的方案数，答案取10^9+7的模
    //参考爬楼梯的方式，方案来源于前一步向右的值，前一步向左的值，前一步原地不动的值
    //由于每次的步数无法超过数组长度，所以长度要大于0小于数组长度，取步数和数组长度中最小的那个
    //dp[i][j] = dp[i-1][j-1] + dp[i-1][j+1] + dp[i-1][j]
    int numWays(int steps, int arrLen) {
        int nColume = min(steps, arrLen - 1);
        const int module = 1000000007;
        vector<vector<int>> rec(steps + 1, vector<int>(nColume + 1, 0));
        rec[0][0] = 1;
        for (int i = 1; i <= steps; i++) {
            for (int j = 0; j < nColume; j++) {
                rec[i][j] = rec[i - 1][j];
                if (j - 1 >= 0) {
                    rec[i][j] = (rec[i][j] + rec[i][j - 1]) % module;
                }
                if (j + 1 < nColume) {
                    rec[i][j] = (rec[i][j] + rec[i][j + 1]) % module;
                }
            }
        }

        return rec[steps][0];
    }

    int kthLargestValue(vector<vector<int>>& matrix, int k) {
        int nVerSize = matrix.size();
        if (nVerSize == 0) return 0;
        int nHorSize = matrix[0].size();
        vector<vector<int>> vecSum(nVerSize + 1,vector<int>(nHorSize+1,0));
        for (int i = 1; i <= nVerSize; i++) {
            for (int j = 1; j <= nHorSize; j++) {
                vecSum[i][j] = vecSum[i-1][j-1] ^ vecSum[i - 1][j] ^ vecSum[i][j - 1] ^ matrix[i - 1][j - 1];
            }
        }
        vector<int> vecVal;
        for (int i = 1; i <= nVerSize; i++) {
            for (int j = 1; j <= nHorSize; j++) {
                vecVal.push_back(vecSum[i][j]);
            }
        }
        sort(vecVal.rbegin(), vecVal.rend());
        return vecVal[k - 1];
    }

    int minChanges(vector<int>& nums, int k) {
        
    }

    //反转每个括号中的字符串
    //时间复杂度为O(n^2),开销老来自于反转和循环
    //时间复杂度为O(n)，可以记录2个成对括号的index值
    string reverseParentheses(string s) {
        int nSize = s.size();
        string result;
        stack<int> sVal;
        vector<int> vecVal(nSize,0); //记录成对括号的位置
        for (int i = 0; i < nSize; i++) {
            if (s[i] = '(') {
                sVal.push(i);
            }
            else if (s[i] == ')') {
                int j = sVal.top();
                sVal.pop();
                vecVal[i] = j;
                vecVal[j] = i;
            }
        }

        int nIndex = 0;
        int nStep = 1;
        while (nIndex < nSize) {
            if (s[nIndex] == '(' || s[nIndex] == ')') {
                nIndex = vecVal[nIndex];
                nStep = -nStep;
            }
            else {
                result.push_back(s[nIndex]);
            }
            nIndex += nStep;
        }
        return result;
    }

    //汉明距离的和f(x,y)代表汉明距离
    //[1,2,3,4]= f(1,2) + f(1,3) + f(1,4) + f(2,3) + f(2,4) + f(3,4)
    //方法1就是进行遍历，时间复杂度可能会达到O(n^2 * logm)
    //因为数据最大不会超过10^9，所以不会超过30个数
    //计算当前位置i的汉明距离，就是该位置出现1的次数*0的次数，依次累加即能得到对应的汉明距离总和
    int totalHammingDistance(vector<int>& nums) {
        int nSize = nums.size();
        int nAns = 0;
        for (int i = 0; i < 30; i++) {
            int nCount = 0;
            for (int j = 0; j < nSize; j++) {
                nCount += (nums[j] >> i) & 1;
            }
            nAns += nCount * (nSize - nCount);
        }
        return nAns;
    }

    //判断是否是4的幂，4^x =2^(2x),如果不是2的幂，不用循环进行处理
    //如果是2的幂，4的幂都是奇数为1，所以可以先判断是否是2的幂次方，然后确定是否在奇数位上
    bool isPowerOfFour(int n) {
        return (n > 0 && (n&(n - 1)) == 0) ? (n & 0x55555555) == n : false;
    }

    //candiesCount保存所有类型的糖果数量
    //queries保存的是一个列表[type,day,count]
    //到达期望的那天总共要吃的糖果数量 nTotalCandies = type1+type2...+typen;
    // 满足 (day - 1) <= nCatCount <= nTotalCandies 
    //可能出现的情况
    //最小值应是type1+..+typen-1 + 1,最大值为tpye1+...+typen
    //实际的吃糖的数量为 [day,nMax * day]，只要有交集就是true
    vector<bool> canEat(vector<int>& candiesCount, vector<vector<int>>& queries) {
        int nTypeCount = candiesCount.size();
        vector<bool> result;
        vector<int> candy(nTypeCount,0); //前缀和
        candy[0] = candiesCount[0];
        for (int i = 1; i < nTypeCount; i++) {
            candy[i] = candy[i - 1] + candiesCount[i];
        }
        for (auto item : queries) {
            int nType = item[0]; /* 所需要到达的糖果类型 */
            int nDay = item[1]; /* 所需要消耗的日子 */
            int nMax = item[2]; /* 每日最大的消耗值 */
            pair<int, int> first(nType == 0?1: (candy[nType] - candiesCount[nType] + 1), candy[nType]);
            pair<int, int> second(nDay, nMax*nDay);
            result.push_back(IsIntersection(first, second));
        }
    }

    bool IsIntersection(pair<int, int>& a, pair<int, int>& b) {
        return (a.first <= b.second) && (a.second >= b.first);
    }

    bool checkSubarraySum(vector<int>& nums, int k) {
        int nSize = nums.size();
        vector<vector<int>> vecVal(nSize+1,vector<int>(nSize+1,0));
        /* 获取对应的前缀和数组 */
        /* O(n) */
        for (int i = 1; i <= nSize; i++) {
            vecVal[0][i] = nums[i - 1] + vecVal[0][i - 1];
            if (vecVal[0][i] % k == 0 && i >= 2){
                return true;
            }
        }
        
        /* n + n-2 + n-4+...+1 */
        for (int i = 1; i <= nSize; i++) {
            for (int j = i + 1; j <= nSize; j++) {
                int nTmp = vecVal[0][j] - vecVal[0][i] + nums[i - 1];
                if (nTmp % k == 0) return true;
            }
        }

        return false;
    }

    /* 由于是前缀和处理方式，数据量为10^5，所以暴力指定超时 */
    /* 前缀和f(i)代表0~i-1区间的的值 f(j)代表的0~j-1区间的值，所以求i~j-1的区间就是f(j-i)*/
    /* 要求得最长的区间，那么只需要记录前缀和已经出现过的值，记录第一次出现的区间下标，此时就是对应的区间长度 */
    int findMaxLength(vector<int>& nums) {
        int nSize = nums.size();
        if (nSize <= 1) return 0;
        unordered_map<int, int> mapVal;
        mapVal[0] = -1;
        int nTotalCount = 0;
        int nMax = 0;
        for (int i = 0; i < nSize; i++) {
            if (nums[i] == 0) {
                nTotalCount++;
            }
            else {
                nTotalCount--;
            }
            if (mapVal.find(nTotalCount) != mapVal.end()) {
                int nTmp = mapVal[nTotalCount];
                nMax = max(nMax, i - nTmp);
            }
            else {
                mapVal.insert(make_pair(nTotalCount, i));
            }
        }
        return nMax;
    }

    /* 使用递归会超时 */
    /* 改用动态规划，用栈来实现 */
    /* 令所有选择负数的和为 a,所有选择正数的值为b b = sums - a,所以和为b - a = target*/
    /* b=sums-a,那么sums - a -a = target,a = (sum - target)/2 所以所有的负数和为偶数*/
    /* 令f[i][j]表示前i个数的和为j的个数，所以最后求解的值为f[i][target] */
    int findTargetSumWays(vector<int>& nums, int target) {
        int nSize = nums.size();
        if (nSize == 0)return 0;
        int nSum = 0;
        /* 获取所有的元素和 */
        for (auto it : nums) {
            nSum += it;
        }
        
        //负数和
        int nTmp = nSum - target;
        
       //如果不是非负偶数，直接返回
        if (nTmp < 0 || nTmp % 2 != 0) {
            return 0;
        }

        /* 最后要得到的负数总和 */
        int nIndex = nTmp / 2;
        vector<vector<int>> vecVal(nSize + 1, vector<int>(nTmp + 1, 0));
        vecVal[0][0] = 1; /* 不选择任何值，总值为0的个数 */

        for (int i = 1; i <= nSize; i++) {
            for (int j = 0; j <= nTmp; j++) {
                if (j < nums[i-1]) {
                    vecVal[i][j] = vecVal[i - 1][j]; /* 如果小于，只能不选择 */
                }
                else {
                    vecVal[i][j] = vecVal[i - 1][j] + vecVal[i - 1][j - nums[i-1]]; /* 要么不选择，要么选择 */
                }
            }
        }
        return vecVal[nSize][nIndex];
    }

    /* 使用递归解题，确实能够解题，但是时间复杂度为2^n */
    /* 使用动态规划，和昨天的题目一致，所有的负数和为a，那么正数和为b，a = (sum-target)/2,所以在sum = 2*a - target a的值不超过sum/2 */
    /* 用f(i,j)前面0~i-1能够达到j，所以当i = 0,只有j = 0 为true */
    int lastStoneWeightII(vector<int>& stones) {
        int nSize = stones.size();
        int nSum = 0;
        for (int i = 0; i < nSize; i++) {
            nSum += stones[i];
        }
        int m = nSum / 2;
        vector<vector<int>> vecVal(nSize + 1, vector<int>( m+ 1, 0));
        vecVal[0][0] = 1;
        for (int i = 1; i <= nSize; i++) {
            for (int j = 0; j <= m; j++) {
                if (j < stones[i-1]) {
                    vecVal[i][j] = vecVal[i - 1][j];
                }
                else {
                    vecVal[i][j] = vecVal[i - 1][j] || vecVal[i - 1][j - stones[i - 1]];
                }
            }
        }
        int nResult = 0;
        for (int i = m; i >= 0; i--) {
            if (vecVal[nSize][i]) {
                nResult = nSum - 2 * i;
                break;
            }
        }
        return nResult;
    }

    /* 用递归整一下 已超时 */
    /* 使用动态规划，找到子状态 */
    int change(int amount, vector<int>& coins) {
        int nSize = coins.size();
        if (nSize == 0)return 0;
        /* f(i,j)表示前i种硬币能够达到j重量的方案数 */
        vector<vector<int>> vecVal(nSize + 1, vector<int>(amount+1, 0));
        for (int i = 0; i <= nSize; i++) {
            vecVal[i][0] = 1;
        }

        for (int i = 1; i <= nSize; i++) {
            for (int j = 1; j <= amount; j++) {
                if (j - coins[i - 1] >= 0) {
                    vecVal[i][j] = vecVal[i - 1][j] + vecVal[i][j - coins[i - 1]]; /* 使用前i-1种硬币达到j的方案+使用前i种硬币能够得到j-coins[i-1]重量的方案数 */
                }
                else {
                    vecVal[i][j] = vecVal[i - 1][j];
                }
            }
        }

        return vecVal[nSize][amount];
    }

    /* n是由多个平方数组成的和，求最少的和使用到的最小平方数 */
    /* 可以先计算下能够达到的最大平方根 */
    /* 使用递归处理，需要sqrt(n)的全排，超时 */
    /* 条件使用前i个值能够达到目标值的最少次数 */ 
    /* f(i,j)表示使用前i个对象能够达到j值的最小次数 */
    /* f(i,j) = min(f(i-1,j),f(i,j-n))+1 */
    int numSquares(int n) {
        int nIndex = sqrt(n);
        if (n == 0) return 0;
        int nMin = INT_MAX;
        vector<int> vecVal(n + 1, INT_MAX);
        vecVal[0] = 0;

        for (int i = 1; i <= nIndex; i++) {
            for (int j = 0; j <= n; j++) {
                if (j >= i*i) {
                    vecVal[j] = min(vecVal[j], vecVal[j - i*i]+1);
                }
            }
        }
        return vecVal[n];
    }

    //arr 是个先增后减的区间
    //其实就是找最大值，要么通过遍历来进行枚举，要么通过二分来确定
    //ans > a[i-1] 
    //ans > a[i+1]
    //所以从后面开始枚举就好了
    int peakIndexInMountainArray(vector<int>& arr) {
        int i = 0, j = arr.size();
        int nAns = 0;
        while (i <= j) {
            int nMid = i + ((j - i) >> 1);
            if (arr[nMid] > arr[nMid + 1]) {
                nAns = nMid;
                j = nMid - 1;
            }
            else {
                i = nMid + 1;
            }
        }
        return arr[nAns];
    }

    string largestNumber(vector<int>& cost, int target) {
        int nSize = cost.size();
        /* 用a[i][j]表示使用前n个数能够达到的最大值 */
        vector<vector<int>> vecVal(nSize + 1, vector<int>(target + 1, INT_MIN));
        /* 使用另一个数组来记录当前对应的路径信息 */
        /* from[i][j] 表示前i个值到达j的当前值 */
        vector<vector<int>> vecFrom(nSize + 1, vector<int>(target + 1));

        vecVal[0][0] = 0;
        for (int i = 1; i <= nSize; i++) {
            for (int j = 0; j <= target; j++) {
                if (j < cost[i - 1]) {
                    vecVal[i][j] = vecVal[i - 1][j];
                    vecFrom[i][j] = j;
                }
                else {
                    if (vecVal[i - 1][j] > vecVal[i][j - cost[i - 1]] + 1) {
                        vecFrom[i][j] = j;
                        vecVal[i][j] = vecVal[i - 1][j];
                    }
                    else {
                        vecFrom[i][j] = j - cost[i - 1];
                        vecVal[i][j] = vecVal[i][j - cost[i - 1]] + 1;
                    }
                }
            }
        }

        if (vecVal[nSize][target] <= 0) {
            return "0";
        }

        string str;
        int i = nSize;
        int j = target;
        while (i > 0) {
            if (j == vecFrom[i][j]) {
                i--;
            }
            else {
                j -= cost[i-1];
                str += '0' + i;
            }
        }
        return str;
    }

    //石头的总数是奇数，所以不会出现相同石头的情况
    //使用贪心的方式，所以第一个用户一定总是拿到最大的石头
    //dp[i][j] 石堆i~j对应的两者中间的最大差值,
    //dp[i][j] = max(piles[i] - dp[i+1][j],piles[i] - dp[i][j-1]); //要么取第一个石头，要么取最后一个石头
    //dp[i][j]记录的就是i到j石堆的数据差值
    bool stoneGame(vector<int>& piles) {
        int nSize = piles.size();
        vector<vector<int>> vecVal(nSize+1,vector<int>(nSize+1,0));
        for (int i = 1; i <= nSize; i++) {
            vecVal[i][i] = piles[i-1];
        }
        for (int i = nSize - 2; i >= 0; i--) {
            for (int j = i + 1; j <= nSize; j++) {
                vecVal[i][j] = max(piles[i] - vecVal[i + 1][j], piles[j] - vecVal[i][j - 1]);
            }
        }

        return vecVal[0][nSize] > 0;
    }

    /* 这道题目考察的就是不同的条件讨论,所以可以通过有限状态机来进行解题 */
    enum State {
        STATE_INITIVAL, /* 初始化状态 */
        STATE_INT_SIGN, /* 符号部分 */
        STATE_INTEGER, /* 整数状态 */
        STATE_POINT, /* 小数点左边带数字状态 */
        STATE_POINT_WITHOUT_INT, /* 小数点左边不带数字状态 */
        STATE_FRACTION, /* 小数状态 */
        STATE_EXP, /* 指数状态 */
        STATE_EXP_SIGN, /* 指数符号状态 */
        STATE_EXP_NUMBER,/* 指数数字状态 */
        STATE_END /**/
    };

    enum CharType {
        CHAR_NUMBER, /* 整数 */
        CHAR_EXP, /* 指数 */
        CHAR_POINT, /* 小数点 */
        CHAR_SIGN, /* 符号 */
        CHAR_ILLEGAL /* 无效 */
    };

    CharType toCharType(char ch) {
        if (ch >= '0' && ch <= '9')
            return CHAR_NUMBER;
        else if (ch == 'E' || ch == 'e')
            return CHAR_EXP;
        else if (ch == '.')
            return CHAR_POINT;
        else if (ch == '-' || ch == '+')
            return CHAR_SIGN;
        return CHAR_ILLEGAL;
    }

    bool isNumberEx(string s) {
        unordered_map<State, unordered_map<CharType, State>> transfer{
            {STATE_INITIVAL,{{CHAR_NUMBER,STATE_INTEGER},{CHAR_POINT,STATE_POINT_WITHOUT_INT},{CHAR_SIGN,STATE_INT_SIGN}}},
            {STATE_INT_SIGN,{{CHAR_NUMBER,STATE_INTEGER},{CHAR_POINT,STATE_POINT_WITHOUT_INT}}},
            {STATE_INTEGER,{{CHAR_NUMBER,STATE_INTEGER},{CHAR_POINT,STATE_POINT},{CHAR_EXP,STATE_EXP}}},
            {STATE_POINT,{{CHAR_NUMBER,STATE_FRACTION },{CHAR_EXP,STATE_EXP }}},
            {STATE_POINT_WITHOUT_INT,{{CHAR_NUMBER,STATE_FRACTION }}},
            {STATE_FRACTION,{{CHAR_NUMBER,STATE_FRACTION},{CHAR_EXP,STATE_EXP}}},
            {STATE_EXP,{{CHAR_SIGN,STATE_EXP_SIGN},{CHAR_NUMBER,STATE_EXP_NUMBER }}},
            {STATE_EXP_SIGN,{{CHAR_NUMBER,STATE_EXP_NUMBER}}},
            {STATE_EXP_NUMBER,{{CHAR_NUMBER,STATE_EXP_NUMBER}}},
        };
        int len = s.length();
        State st = STATE_INITIVAL;

        for (int i = 0; i < len; i++) {
            CharType ct = toCharType(s[i]);
            if (transfer[st].find(ct) == transfer[st].end()) {
                return false;
            }
            else {
                st = transfer[st][ct];
            }
        }
        return st == STATE_INTEGER || st == STATE_POINT || st == STATE_FRACTION || st == STATE_EXP_NUMBER || st == STATE_END;
    }

    /* 求字符串的全排，0/1背包问题 */
    vector<string> permutation(string s) {
        /* 结果集 */
        set<string> vecRes;
        calc(s, s.size(), 0, vector<int>(s.size(), 0), "", vecRes);
        vector<string> strRes(vecRes.begin(), vecRes.end());
        return strRes;
    }

    void calc(string&s, int nSize, int nCount,vector<int>status, string str, set<string>& vecRes) {
        if (nCount == nSize) {
            vecRes.insert(str);
            return;
        }
        for (int i = 0; i < nSize; i++) {
            if (status[i]) continue;
            str.push_back(s[i]);
            status[i] = 1;
            calc(s, nSize, nCount + 1, status, str, vecRes);
            str.pop_back();
            status[i] = 0;
        }
    }

    /* BFS来求最短的车程 */
    /* 可以用一个数组表示所以已经访问过的车站，如果已经访问过就不处理了 */
    /* source肯定是根节点root */
    /* 如果没有source，直接返回-1 */
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        int nVerSize = routes.size();
        if (nVerSize == 0) return 0;
        int nHorSize = routes[0].size();
        unordered_map<int, set<int>> mapVal;
        for (int i = 0; i < nVerSize; i++) {
            for (int j = 0; j < nHorSize; j++) {
                auto item = mapVal.find(routes[i][j]);
                if (item == mapVal.end())
                    mapVal.insert(make_pair(routes[i][j], set<int>(routes[i].begin(), routes[i].end())));
                else
                    mapVal[routes[i][j]].insert(routes[i].begin(), routes[i].end());
            }
        }

        if (!mapVal.count(source)) {
            return -1;
        }

        unordered_map<int, int> mapFlag;
        /* 初始化状态 */
        for (auto it : mapVal) {
            mapFlag.insert(make_pair(it.first, 0));
        }

        queue<pair<int,int>> que;
        que.push(pair<int,int>(source,0));
        while (!que.empty())
        {
            auto it = que.front();
            que.pop();
            if (mapFlag[it.first])continue;
            mapFlag[it.first] = 1;
            if (it.first == target) {
                return it.second;
            }
            set<int> tmp = mapVal[it.first];
            for (auto item : tmp) {
                if (mapFlag[item])continue;
                que.push(pair<int,int>(item,it.second+1));
            }
        }
        return -1;
    }

    /* 一共有n个用户 */
    /* 一共k轮，所以需要k次循环，另一个条件就是存在n个节点 */
    /* 令a[i][j]为第i轮选择j用户的次数 */
    int numWays(int n, vector<vector<int>>& relation, int k) {
        vector<vector<int>>vecVal(k+1,vector<int>(n,0));
        /* 起点位置在(0,0)位置 */
        vecVal[0][0] = 1;
        for (int i = 1; i <= k; i++) {
            for (auto it : relation) {
                int nSrc = it[0];
                int nDst = it[1];
                vecVal[i][nDst] += vecVal[i - 1][nSrc];
            }
        }
        return vecVal[k][n - 1];
    }

    /* 一共有coins快，有costs种雪糕，求能够买到的最大雪糕数量 */
    /* 一道很明显的动态规划问题 */
    /* a[i][j] 表示使用j快钱，能够买到前i-1种类型的雪糕最大数量*/
    /* a[i][j] = max(a[i-1][j],a[i][j-costs[i]] + 1);  */
    /* 只有0/1选择 */
    /* 数据量太大，不能用动态规划 */
    int maxIceCream(vector<int>& costs, int coins) {
        sort(costs.begin(), costs.end());
        int nSize = costs.size();
        int nCount = 0;
        for (int i = 0; i < nSize; i++) {
            if (coins - costs[i] >= 0) {
                nCount++;
                coins -= costs[i];
            }
            else {
                break;
            }
        }

        return nCount;
    }

    /* 原子数量输出 */
    /* 去掉括号，算出对应的数量 */
    /* Mg(OH)2 -> H2MgO2*/
    /* 如果是存在小写字母应当和前面字符保持一个整体 */
    string countOfAtoms(string formula) {
        int nSize = formula.size();
        stack<string> sVal;
        for (int i = 0; i < nSize; i++) {
            if (formula[i] == '(' || (formula[i] >= 'A' && formula[i] <= 'Z')) {
                string str;
                str.push_back(formula[i]);
                sVal.push(str);
            }
            else if (formula[i] == ')')
            {
                string strNum;
                while (true) {
                    if (i + 1 < nSize && (formula[i+1] >= '0' && formula[i+1] <= '9')) {
                        strNum.push_back(formula[i + 1]);
                        i++;
                    }
                    else {
                        break;
                    }
                }
                int nNum = strNum.size() == 0 ? 1 : atoi(strNum.c_str());
                stack<string> sTmp;
                while (!sVal.empty()) {
                    if (sVal.top() == "(") {
                        sVal.pop();
                        break;
                    }
                    else {
                        string str = sVal.top();
                        sVal.pop();
                        if (str.back() >= '0' && str.back() <= '9') {
                            str.push_back('*');
                        }
                        str += to_string(nNum);
                        sTmp.push(str);
                    }
                }
                while (!sTmp.empty()) {
                    sVal.push(sTmp.top());
                    sTmp.pop();
                }
            }
            else {
                if (!sVal.empty()) {
                    sVal.top().push_back(formula[i]);
                }
            }
        }

        /* 这里涉及到很多元素的相乘 */
        int nItemSize = sVal.size();
        map<string, int> mapVal;
        while (!sVal.empty()) {
            string item = sVal.top();
            sVal.pop();
            int i = 0;
            for (; i < item.size(); i++) {
                if (item[i] >= '0' && item[i] <= '9')
                    break;
            }
            string first = item.substr(0, i);
            auto findItem = item.find('*',i);
            int nNum = 0;
            if (findItem == string::npos) {
                if (i < item.size()) {
                    nNum = atoi(item.substr(i).c_str());
                }
            }
            else {
                nNum = 1;
                int nIndex = i;
                while (findItem!=string::npos) {
                    nNum *= atoi(item.substr(nIndex, findItem - nIndex).c_str());
                    nIndex = findItem + 1;
                    findItem = item.find('*', nIndex);
                }
                if (nIndex < item.size()) {
                    nNum *= atoi(item.substr(nIndex).c_str());
                }
            }
            auto mapItem = mapVal.find(first);
            if (mapItem == mapVal.end())
                mapVal.insert(make_pair(first, nNum));
            else
                mapVal[first] += nNum;
        }
        string strRes;
        for (auto it : mapVal) {
            strRes += it.first + to_string(it.second);
        }
        return strRes;
    }

public:
    vector<vector<string>> ans;
    vector<string> item;
    vector<vector<bool>> flag;
    int m_nSize;
};

int main()
{
    system("pause");
    return 0;
}