#include <cstdio>
#include <cstring>
#include <iostream>

#define CHILDREN 26
#define MAXN 1000005

struct Node {
    int cnt;                     //是否为单词最后一个节点
    Node *next[CHILDREN];
    Node *fail;
    Node() {
        fail = NULL;
        cnt = 0;
        memset(next, NULL, sizeof(next));
    }
};

class ACAutomaton
{
public:
    Node *root;
    Node *que[MAXN];                  //队列，bfs构造失败指针
    int head, rear;
    void reset()
    {
        head = rear = 0;
        root = new Node();
    }

    void insert(char *str)
    {
        Node *tp = root;
        int idx;
        for(int i = 0; str[i]; i ++)
        {
            int idx = str[i] - 'a';
            if(tp->next[idx] == NULL)
                tp->next[idx] = new Node();
            tp = tp->next[idx];
        }
        tp->cnt ++;
    }

    void build()
    {
        root->fail = NULL;
        que[rear++] = root;
        while(head < rear)
        {
            Node *tp = que[head++];
            Node *p = NULL;
            for(int i = 0; i < 26; i ++)
            {
                if(tp->next[i])
                {
                    if(tp == root)
                        tp->next[i]->fail = root;
                    else
                    {
                        p = tp->fail;
                        while(p) {
                            if(p->next[i])
                            {
                                tp->next[i]->fail = p->next[i];
                                break;
                            }
                            p = p->fail;
                        }
                        if(!p)
                            tp->next[i]->fail = root;
                    }
                    que[rear ++] = tp->next[i];
                }
            }
        }
    }

    /** HDU 2222
    **  输入n个模式串，一个主串，AC机跑一遍，找出现了多少个单词，一个单词最多只出现一次
    **/
    int solve(char *str)
    {
        int ret = 0, idx;
        Node *p = root;
        for(int i = 0; str[i]; i ++)
        {
            idx = str[i] - 'a';
            while(!p->next[idx] && p != root)
                p = p->fail;
            p = p->next[i];
            p = !p ? root : p;
            Node *tp = p;
            while(tp != root && tp->cnt != -1)
            {
                printf("%d\n", tp->cnt);
                ret += tp->cnt;
                tp->cnt = -1;
                tp = tp->fail;
            }
        }
        return ret;
    }

    /** 测试用.. */
    char for_print[30];
    void print(Node *cur, char *str, int idx)
    {
        for(int i = 0; i < CHILDREN; i ++)
        {
            if(cur->next[i])
            {
                str[idx] = (char)(i+'a');
                if(cur->next[i]->cnt == 1) {
                    str[idx+1] = '\0';
                    printf("%s\n", str);
                    cur->next[i]->cnt = 0;
                }
                print(cur->next[i], str, idx+1);
            }
        }
    }
};
char main_str[MAXN];
ACAutomaton ac;
int main()
{
    int t; scanf("%d", &t);
    while(t --)
    {
        int n;
        scanf("%d", &n);
        getchar();

        ac.reset();
        for(int i = 0; i < n; i ++)
        {
            char str[55];
            scanf("%s", str);
            ac.insert(str);
        }
        //ac.print(ac.root, ac.for_print, 0);
        ac.build();
        scanf("%s", main_str);
        printf("%d\n", ac.solve(main_str));
    }
    return 0;
}
