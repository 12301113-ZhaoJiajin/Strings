#include <cstring>
#include <cstdio>
#include <iostream>
#include <cmath>

#define MAXN 1000005

using namespace std;

char str[MAXN], T[MAXN];
int next[MAXN];
int lenT, lenS;

void get_next(char *T,int lenT)
{
    next[0] = -1;       //next[i]=j,表示 str[0..j]=str[i-j..i]
    int j = next[0];
    for(int i = 1; i < lenT; i++)
    {
        while(j >= 0 && T[i] != T[j+1]) j = next[j];
        if(T[i] == T[j+1]) j ++;
        next[i] = j;
    }
}

/**
*    cnt 可用来计算出现次数
*    或者只返回第一次出现的下标
*/
int kmp(char *S, char *T)
{
    int j=next[0];
    int cnt = 0;
    for(int i = 0; i < lenS; i ++)
    {
        while(j >= 0 && S[i] != T[j+1]) j = next[j];
        if(S[i] == T[j+1]) j ++;
        if(j == lenT - 1)
        {
            return i - lenT + 1;
            // 如果要返回出现次数，或是记录多次出现的位置，不用return
            j=next[j];
            cnt ++;
        }
    }
}
/**
*    求循环节
*/
int main()
{
    while(scanf("%s", T))
    {
        if(T[0] == '.') break;
        lenT = strlen(T);
        get_next(T, lenT);
        int times = sqrt(lenT);
        int ans = 1;
        int tp = lenT - 1 - next[lenT-1];
        if(lenT % tp == 0)
            ans = lenT / tp;

        printf("%d\n", ans);
    }
    return 0;
}


/**
*   当模式串为： abababcabcabb  next[] = {-1, -1, 0, 1, 2, 3, -1, 0, 1, -1, 0, 1, -1 }
*/
