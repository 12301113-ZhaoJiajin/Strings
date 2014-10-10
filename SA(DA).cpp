#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

#define M 1000005 * 3

using namespace std;

/**
* rank 下标从0开始， 值0 - n-1 的排列
* sa   从1开始,因为最后一个字符(最小的)排在第0位
* high 从1开始,因为表示的是sa[i-1]和sa[i]
*/

int rank[M], sa[M], X[M], Y[M], high[M], init[M];
int buc[M];

void calhigh(int n)
{
    int i , j , k = 0;
    for(i = 1; i <= n; i++) rank[sa[i]] = i;
    for(i = 0; i < n; high[rank[i++]] = k)
        for(k ? k-- : 0, j = sa[rank[i]-1]; init[i+k] == init[j+k]; k++)
            ;
}

bool cmp(int *r,int a,int b,int l)
{
    return (r[a] == r[b] && r[a+l] == r[b+l]);
}

void suffix(int n,int m = 128) {

    int i , l , p , *x = X , *y = Y;
    for(i = 0 ; i < m ; i ++) buc[i] = 0;
    for(i = 0 ; i < n ; i ++) buc[ x[i] = init[i]  ] ++;
    for(i = 1 ; i < m ; i ++) buc[i] += buc[i-1];
    for(i = n - 1; i >= 0 ; i --) sa[ --buc[ x[i] ]] = i;
    for(l = 1,p = 1 ; p < n ; m = p , l *= 2)
    {
        p = 0;
        for(i = n-l ; i < n ; i ++) y[p++] = i;
        for(i = 0 ; i < n ; i ++) if(sa[i] >= l) y[p++] = sa[i] - l;
        for(i = 0 ; i < m ; i ++) buc[i] = 0;
        for(i = 0 ; i < n ; i ++) buc[ x[y[i]] ] ++;
        for(i = 1 ; i < m ; i ++) buc[i] += buc[i-1];
        for(i = n - 1; i >= 0 ; i --) sa[ --buc[ x[y[i]] ] ] = y[i];
        for(swap(x,y) , x[sa[0]] = 0 , i = 1 , p = 1 ; i < n ; i ++)
            x[ sa[i] ] = cmp(y,sa[i-1],sa[i],l) ? p-1 : p++;
    }
    calhigh(n-1);           //后缀数组关键是求出high,所以求sa的时候顺便把rank和high求出来
}

/*******************************************
**	n为数组长度,下标0开始
**	将初始数据,保存在init里,并且保证每个数字都比0大
**	m = max{ init[i] } + 1
**	一般情况下大多是字符操作,所以128足够了
*****************************************/

int num[M];
int n, k;
int cnt, idx[1000005];

/**
*    check: 分组check函数 注意：i <= n 即使这里 high[n]的值为0，且无实际意义，也不能漏。
*   因为当 i == n 的时候即是检查最后一个分组。
*/

bool check(int x)
{
    int cnt = 1;
    for(int i = 1; i <= n; i ++)
    {
        if(high[i] < x)
        {
            if(cnt >= k) return true;
            cnt = 1;
        }
        else
            cnt ++;
    }
    return false;
}


int main()
{
    while(~scanf("%d%d", &n, &k))
    {
        cnt = 1;
        for(int i = 0; i < n; i ++) {
            scanf("%d", num+i);
            init[i] = num[i];
        }
        sort(num, num+n);
        idx[num[0]] = cnt;
        for(int i = 1; i < n; i ++)
            idx[num[i]] = num[i] == num[i-1] ? cnt : ++ cnt;
        for(int i = 0; i < n; i ++)
            init[i] = idx[init[i]];
        init[n++] = 0;               // 最后一位补0  n表示0的下一位（用来分组） 实际只要用到 n-1
        suffix(n, cnt+1);            // n 必须是 最后一位0的下一位
        int bi;
        int l = 1, r = n, ans = 1;
        while(l <= r)
        {
            bi = (l + r) >> 1;
            bool res = check(bi);
            if(res)
            {
                l = bi + 1;
                ans = max(ans, bi);
            }
            else
                r = bi - 1;
        }
        printf("%d\n", ans);
    }

  return 0;
}
