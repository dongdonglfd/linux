#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
typedef struct Lnode
{
    int date;
    struct Lnode *next;
}Lnode,*Linklist;
//建立链表
Linklist headinsert()
{
    Lnode * s;
    int x;
    Linklist l=(Linklist)malloc(sizeof(Lnode));
    l->next=NULL;
    scanf("%d",&x);
    while(x!=999)
    {
        s=(Linklist)malloc(sizeof(Lnode));
        s->date=x;
        s->next=l->next;
        l->next=s;
        scanf("%d",&x);
    }
    return l;
}
Linklist tailinsert()
{
    Lnode * s,*r;
    int x;
    Linklist l=(Linklist)malloc(sizeof(Lnode));
    l->next=NULL;
    r=l;
    scanf("%d",&x);
    while(x!=999)
    {
        s=(Linklist)malloc(sizeof(Lnode));
        s->date=x;
        r->next=s;
        r=s;
        scanf("%d",&x);
    }
    return l;
}
//打印链表
 void print(Lnode * head)
{
    Lnode *p=head;
    while(p->next!=NULL)
    {
        printf("%d\n",p->next->date);
        p=p->next;
    }
}
//在第i个位置插入元素e
bool listinsert(Linklist *head,int i,int e)
{
    if(i<1) return false;
    Lnode *p= *head;
    int j=0;
    while(p!=NULL&&j<i-1)
    {
        p=p->next;
        j++;
    }
    if(p==NULL) return false;
    Lnode *s=(Lnode *)malloc(sizeof(Lnode));
    s->date=e;
    s->next=p->next;
    p->next=s;
    return true;
}
//在p节点之前插入元素e
bool insertpriornode(Lnode * p,int e)
{
    if(p==NULL) return false;
    Lnode *s=(Lnode *)malloc(sizeof(Lnode));
    if(s==NULL) return false;
    s->next=p->next;
    p->next =s;
    s->date=p->date;
    p->date =e;
    return false;
}
//删除指定节点
bool deletenode(Lnode*p)
{
    if(p==NULL) return false;
    Lnode *q=p->next;
    p->date=p->next->date;
    p->next=q->next;
    free(q);
    return true;
}
//返回第i个元素
Lnode * getelem(Linklist head,int i)
{
    if(i<0) return false;
    Lnode *p=head;
    int j=0;
    while(p!=NULL&&j<i)
    {
        p=p->next;
        j++;
    }
    return p;
} 
//按值查找
Lnode *locateElem(Linklist head,int e)
{
    Lnode *p=head->next;
    while(p!=NULL&&p->date!=e)
    {
        p=p->next;
    }
    return p; 
}
//求表长度
int length(Linklist head)
{
    int len =0;
    Lnode *p=head;
    while(p->next!=NULL)
    {
        p=p->next;
        len++;
    }
    return len;
}
// 释放链表内存
void freeList(Lnode* head) {
    Lnode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
int main()
{   
    int e=100;
    Lnode *head=NULL;
    // head=headinsert();
    head=tailinsert();
    print(head);
    listinsert(&head,3,e);
    print(head);
    free(head);

    return 0;
}