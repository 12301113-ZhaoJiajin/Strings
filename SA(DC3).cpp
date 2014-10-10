#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <algorithm>

#define F(x) ((x) / 3 + ((x) % 3 == 1 ? 0 : tb))
#define G(x) ((x) < tb ? (x) * 3 + 1 : ((x) - tb) * 3 + 2)
#define cmp1(r, a, b) (r[a] == r[b] && r[a+1] == r[b+1] && r[a+2] == r[b+2])
#define cmp3(r, a, b) (r[a] < r[b] || r[a] == r[b] && wv[a+1] < wv[b+1])
#define cmp2(k, r, a, b) (k == 2 ? (r[a] < r[b] || r[a] == r[b] && cmp3(r, a+1, b+1)):cmp3(r, a, b))

using namespace std;
typedef long long LL;
const int M = 20;
const int N = (1<<M);

/**
*  sa数组从sa[1]到sa[n]，存储的是0到n-1的排列
*  rank数组从rank[0]到rank[n-1]，存储的是1到n的排列
*  rank[i]记录的是以i为起点的后缀的排名
*  high[i]记录 lcp(i, i-1)
*/

class SA
{
public:
    int rank[N], sa[3*N], init[3*N], high[N], n;
    int buc[N], m, wv[N], i, j, k;
    int log[N], rmq[M][N];
    SA()
    {
        log[0] = -1;
        for(int i = 1; i < N; i ++)
            log[i] = (i & (i-1)) ? log[i-1] : log[i-1] + 1;
    }

    inline void sort(int *r, int *a, int *b, int n, int m)
    {
        for(i = 0; i < n; i ++) wv[i] = r[a[i]];
        for(i = 0; i < m; i ++) buc[i] = 0;
        for(i = 0; i < n; i ++) buc[wv[i]] ++ ;
        for(i = 1; i < m; i ++) buc[i] += buc[i-1];
        for(i = n - 1; i >= 0; i --) b[-- buc[wv[i]]] = a[i];
        return ;
    }

    inline void suffix_dc3(int *r, int *sa, int n, int m)
    {
        int *rn = r + n;
        int *san = sa + n, ta = 0, tb = (n + 1) / 3;
        int tbc = 0, p, *wa = rank, *wb = high;
        r[n] = r[n+1] = 0;
        for(int i = 0; i < n; i ++)
            if(i % 3 != 0) wa[tbc++] = i;
        sort(r+2, wa, wb, tbc, m);
        sort(r+1, wb, wa, tbc, m);
        sort(r, wa, wb, tbc, m);
        for(p = 1, rn[F(wb[0])] = 0, i = 1; i < tbc; i ++)
            rn[F(wb[i])] = cmp1(r, wb[i-1], wb[i]) ? p - 1 : p ++ ;
        if(p < tbc)
            suffix_dc3(rn, san, tbc, p);
        else
            for(i = 0; i < tbc; i ++)
                san[rn[i]] = i;
        for(i = 0; i < tbc; i ++)
            if(san[i] < tb)
                wb[ta++] = san[i] * 3;
        if(n % 3 == 1)
            wb[ta++] = n - 1;
        sort(r, wb, wa, ta, m);
        for(i = 0; i < tbc; i ++)
            wv[wb[i] = G(san[i])] = i;
        for(i = 0, j = 0, p = 0; i < ta && j < tbc; p ++)
            sa[p] = cmp2(wb[j] % 3, r, wa[i], wb[j]) ? wa[i++] : wb[j++];
        for( ; i < ta; sa[p++] = wa[i++])
            ;
        for( ; j < tbc; sa[p++] = wb[j++])
            ;
    }

    inline int exec(char *in)
    {
        for(int &p = n = m = 0; in[p]; p ++)
        {
            init[p] = in[p];
            m = max(m, init[p]+1);
        }
        init[n] = 0;
        suffix_dc3(init, sa, n+1, m);
        for(i = 1; i <= n; i ++)
            rank[sa[i]] = i;
        for(i = 0, k = 0; i < n; high[rank[i++]] = k)
            for(k ? k -- : 0, j = sa[rank[i]-1]; init[i+k] == init[j+k]; k ++)
                ;
        for(i = 1; i <= n; i ++)
            rmq[0][i] = high[i];
        for(int i = 1; i <= log[n]; i ++)
            for(int j = 1; j <= n-(1<<i)+1; j ++)
                rmq[i][j] = min(rmq[i-1][j], rmq[i-1][j+(1<<i>>1)]);
        return n;
    }

    /** lcp(rank[i],rank[j]) 询问i,j后缀的最长公共前缀 */
    inline int lcp(int a, int b)
    {
        if(a == b) return n - sa[a];
        if(a > b)
            swap(a, b);
        int t = log[b-a];
        return min(rmq[t][a+1], rmq[t][b-(1<<t)+1]);
    }
} sa;

char str[N] = {"aabaaaab"};
int len = 8;
int main()
{
    sa.exec(str);
    freopen("out.txt", "w", stdout);

    for(int i = 1; i <= len; i ++)
        printf("%s\n", str+sa.sa[i]);

    for(int i = 1; i <= len; i ++)
        printf("%d ", sa.sa[i]);
    printf("\n");
    for(int i = 0; i < len; i ++)
        printf("%d ", sa.rank[i]);
    printf("\n");
    for(int i = 1; i <= len; i ++)
        printf("%d ", sa.high[i]);

    return 0;
}

/**
*   sa[1-len]     = {3 4 5 0 6 1 7 2};
*   rank[0-len-1] = {4 6 8 1 2 3 5 7};
*   high[1-len]   = {0 3 2 3 1 2 0 1};
*
*   aaaab
*   aaab
*   aab
*   aabaaaab
*   ab
*   abaaaab
*   b
*   baaaab
*/
