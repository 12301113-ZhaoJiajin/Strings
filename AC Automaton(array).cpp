#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>

#define MAXN 500005
#define CHILDREN 26
using namespace std;

class ACAutomaton
{
public:
    int sz;
    int val[MAXN];
    int fail[MAXN];
    int next[MAXN][CHILDREN];
    int ID[300];
    queue<int> que;

    /** 初始化 ID 映射 */
    ACAutomaton()
    {
        fail[0] = 0;
        sz = 1;
    }

    /** 初始化AC机，如sz, val[], queue */
    void reset()
    {
        sz = 1;
        memset(next[0], 0, sizeof(next[0]));
    }

    void insert(char *str, int d)
    {
        int tp = 0;
        for(int i = 0; str[i]; i ++)
        {
            int idx = str[i] - 'a';         //取映射, 如果有ID[], 则ID[str[i]]
            if(!next[tp][idx])
            {
                //printf("%c %d", str[i], sz);
                next[tp][idx] = sz;
                memset(next[sz], 0, sizeof(next[sz]));
                val[sz++] = 0;
            }
            tp = next[tp][idx];
        }
        val[tp] += d;               // 考虑重复模式串
    }

    void build()
    {
        que.push(0);
        while(!que.empty())
        {
            int cur = que.front(); que.pop();
            for(int i = 0; i < CHILDREN; i ++)
            {
                int &tp = next[cur][i];
                if(tp)
                {
                    que.push(tp);
                    fail[tp] = cur ? next[fail[cur]][i] : 0;  // 判断cur是否为root
                }
                else if(cur)
                    tp = next[fail[cur]][i];
            }
        }
    }

    /** HDU 2222 */
    int solve(char *str)
    {
        int ret = 0, tp = 0;
        for(int i = 0; str[i]; i ++)
        {
            int idx = str[i]-'a';        //OR ID[str[i]];
            while(tp && !next[tp][idx])
                tp = fail[tp];
            if(next[tp][idx])
                tp = next[tp][idx];
            for(int cur = tp; cur && val[cur] != -1; cur = fail[cur])
            {
                ret += val[cur];
                val[cur] = -1;
            }
        }
        return ret;
    }

}ac;
char str[55], main_str[1000005];
int main()
{
    int c; scanf("%d", &c);
    while(c --)
    {
        int n; scanf("%d", &n);
        ac.reset();
        while(n --)
        {
            scanf("%s", str);
            ac.insert(str, 1);
        }
        ac.build();
        scanf("%s", main_str);
        printf("%d\n", ac.solve(main_str));
    }
}
