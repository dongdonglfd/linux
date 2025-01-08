//-a、-l、-R、-t、-r、-i、-s 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<libgen.h>
 
// ANSI转义序列定义颜色
#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define BLUE    "\x1B[34m"
// 判断文件类型并返回相应的颜色
const char* get_color(const char *p) {
    struct stat statbuf;
    if (stat(p, &statbuf) != 0) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if (S_ISDIR(statbuf.st_mode)) {
        return BLUE; // 目录为蓝色
    } else {
        return GREEN; // 文件为绿色
    }
}
//函数
int do_ls(char pathname[]);
int panduan(char canshu[]);
char *find_user_name_by_uid(uid_t uid);
char *find_group_name_by_gid(gid_t gid);
int do_l(char filename[]);
char*change_mode_to_string(mode_t mode);
int do_i(char filename[]);
int do_s(char filename[]);
int do_t(char*names[]);
int do_r(char*names[]);
int do_name(char pathname[]);

int a_flag=0;
int l_flag=0;
int R_flag=0;
int t_flag=0;
int r_flag=0;
int i_flag=0;
int s_flag=0;
int ls=0;
int x=0;
int count_start =0;
// int cnt=0;
int c=0;
char *  names[1000000]={NULL};
char p[1000];
int main(int argc,char *argv[] )
{ 
    
    if(argc==1)
    {
        ls=1;
        do_ls(".");
    }
    
    else
    {
        for(int i=1;i<=argc-1;i++)
        {
            if(argv[i][0]=='-')
            {
                panduan(argv[i]);
            }
        }
        
        for(int i=1;i<=argc-1;i++)
        {
            if(argv[i][0]!='-')
            {   
                x=1;
                ls=1;
                do_ls(argv[i]);
                for(int i=0;i<c;i++)
                {
                    free(names[i]);
                    names[i]=NULL;
                }
                c=0;
            }
            
        }
        if(x==0)
        { 
            do_ls(".");
        }
    }
    // for(int i=0;i<c;i++)
    // {
    //     free(names[i]);
    // }
    
    return 0;
}
int do_ls(char pathname[])
{   
     
    // count_start = c;
    DIR* dir;
    struct dirent *entry;
    int cnt=0;
    strcpy(p,pathname);
    int l=strlen(p);
    p[l]='\0';
    
    dir=opendir(pathname);
    if(dir==NULL)
    {
        
        perror("opendir");
    }
    // printf("aaa=%s\n",pathname);
    while((entry=readdir(dir))!=NULL)
    {
        if ((a_flag==0)&&(strncmp(entry->d_name, ".",1) == 0 || strncmp(entry->d_name, "..",2) == 0) ){ 
            continue;
        }
        
        // strcpy(tm,entry->d_name);
        // names[c]=(char*)malloc(4000*sizeof(char));
        // // names[c++]=tm;
        // strcpy(names[c],tm);
        // c++;
        
        // free(tm);
        // tm=NULL;
        // char tm[1000]={0};     
        // strcpy(tm,entry->d_name);
        // names[c++]=tm;
        char * tm=(char*)malloc(4000*sizeof(char));
        strcpy(tm,pathname);
        int l=strlen(pathname);
        if(strcmp(pathname,"/")!=0)
        {
            
            
            strcpy(&tm[l],"/");
            strcpy(&tm[l+1],entry->d_name);
        names[c]=(char*)malloc(4000*sizeof(char));
        // names[c++]=tm;
        strcpy(names[c],tm);
        c++;
       
        }
        else
        {
            
        
        strcpy(&tm[l],entry->d_name);
        names[c]=(char*)malloc(4000*sizeof(char));
        // names[c++]=tm;
        strcpy(names[c],tm);
        c++;
        }
         free(tm);
        
    }   
    closedir(dir);
    if(t_flag==1)
    {
        do_t(names);
    }
    if(r_flag==1)
    {
        do_r(names);
    }
    // printf("count=%d\n",count_start);
    for(int i=0;i<c;i++)
    {  
        struct stat info;
        //  char t[128];
        //     strcpy(t,p);
        //     size_t len=strlen(p);
        //     strcpy(&t[len],"/");
        //     strcpy(&t[len+1],names[i]);
        char t[1000];
        strcpy(t,pathname);
        size_t len=strlen(pathname);
        strcpy(&t[len],"/");
        strcpy(&t[len+1],names[i]);   
        // strcpy(names[i],t);
        lstat(names[i], &info);  // 拉进 info
        
        //  printf("v=%s\n",names[i]);
        if (strstr(names[i],".")==NULL&&strstr(names[i],"..")==NULL&&S_ISLNK(info.st_mode)==0&&S_ISDIR(info.st_mode) && R_flag==1 )
        {
            
            // char tmp[128];
            // strcpy(tmp,pathname);
            // size_t len=strlen(pathname);
            // strcpy(&tmp[len],"/");
            // strcpy(&tmp[len+1],names[i]);
            // strcpy(names[i],)
            // printf("shadkjhaskdh\n");    
            // do_ls(names[i]);

            // char* t = (char*)malloc(sizeof(char) * 4096);
            // strcpy(t, names[i]);
            // int len = strlen(t);
            // strcpy(&t[len], "/");
            // strcpy(&t[len + 1], names[i]);
            // names[i] = t;
            do_name(names[i]);


        }
      
        // char t[128];
        // strcpy(t,pathname);
        // size_t len=strlen(pathname);
        // strcpy(&t[len],"/");
        // strcpy(&t[len+1],names[i]);             //有问题
        // if ((a_flag==0)&&(strncmp(names[i], ".",1) == 0 || strncmp(names[i], "..",2) == 0) ){ printf("2");
        //     continue;
        // }
        if(i_flag==1&&l_flag==0)
        { 
            do_i(names[i]);
            printf("%s",names[i]);
            printf("\n");
            continue;
        }
        if(s_flag==1&&l_flag==0)
        { 
            do_s(names[i]);
            printf("%s",names[i]);
            printf("\n");
            continue;
        }
        if(l_flag==1)
        { 
            do_l(names[i]);
            continue;
        }
        if((ls==1&&l_flag==0)||(a_flag==1&&l_flag==0)||r_flag==1||t_flag==1||R_flag==1)
        {
            printf("\n");
            if(++cnt%5==0)
            {
                printf("\n");
            }
            const char *color = get_color(pathname);
            printf("%s%-15s%s", color,names[i], RESET);
        } 
    }
    printf("\n");
    // closedir(dir);
    // for(int i=0;i<c;i++)
    // {
    //     names[i]="\0";
        
    // }
    
   
    return 0;
    
}
int panduan(char *canshu)
{
    canshu++;
	while (*canshu != '\0')
	{
		switch (*canshu)
		{
			case 'a':
				a_flag = 1;
                
				break;
			case 'l':
				l_flag = 1;
				break;
			case 'R':
				R_flag = 1;
				break;
			case 't':
				t_flag = 1;
				break;
            case 'r':
				r_flag = 1;
				break;
            case 'i':
				i_flag = 1;
				break;
            case 's':
				s_flag = 1;
				break;
			default:
				return -1;
		}
	    canshu++;
	}
	return 0;
}
char *find_user_name_by_uid(uid_t uid)
{
    struct passwd*ptr;
    ptr=getpwuid(uid);
    if(ptr==NULL)
    {
        static char string[10];
        sprintf(string,"%d",uid);
        return string;
    }
    return ptr->pw_name;
}
char *find_group_name_by_gid(gid_t gid)
{
    struct group*ptr;
    ptr=getgrgid(gid);
    if(ptr==NULL)
    {
        static char string[10];
        sprintf(string,"%d",gid);
        return string;
    }
    return ptr->gr_name;
}
int do_l(char filename[])
{

    struct stat buf;
    if(stat(filename,&buf)<0)
    {
        perror("stat");
        return -1;
    }
    if(i_flag==1)
    {
        do_i(filename);
    }
    if(s_flag==1)
    {
        do_s(filename);
    }
    printf("%-10s ",basename(filename));
    printf("%s ",change_mode_to_string(buf.st_mode));
    printf("%ld ",buf.st_nlink);
    printf("%s ",find_user_name_by_uid(buf.st_uid));
    printf("%s ",find_group_name_by_gid(buf.st_gid));
    printf("%ld ",buf.st_size);
    printf("%s",ctime(&buf.st_atime));
    return 0;
}
char*change_mode_to_string(mode_t mode)
{
    static char mod[11];
    strcpy(mod,"----------");
    if(S_ISDIR(mode)) mod[0]='d';
    if(S_ISCHR(mode)) mod[0]='c';
    if(S_ISBLK(mode)) mod[0]='b';
    if(S_ISLNK(mode)) mod[0]='l';
    if(S_ISFIFO(mode)) mod[0]='f';
    if(S_ISSOCK(mode)) mod[0]='s';
    if(mode&S_IRUSR) mod[1]='r';
    if(mode&S_IWUSR) mod[2]='w';
    if(mode&S_IXUSR) mod[3]='x';
    if(mode&S_IRGRP) mod[4]='r';
    if(mode&S_IWGRP) mod[5]='w';
    if(mode&S_IXGRP) mod[6]='x';
    if(mode&S_IROTH) mod[7]='r';
    if(mode&S_IWOTH) mod[8]='w';
    if(mode&S_IXOTH) mod[9]='x';
    return mod;
}
int do_i(char filename[])
{
    
    struct stat buf;
    if(stat(filename,&buf)<0)
    {
        perror("stat");
        return -1;
    }
    printf("%lu     ",buf.st_ino);
    return 0;
}
int do_s(char filename[])
{
    struct stat buf;
    if(stat(filename,&buf)<0)
    {
        perror("stat");
        return -1;
    }
    long long size=buf.st_size/1024;
    if(size<4)
    {
        printf("4   ");
    }
    else
    {
        printf("%-4lld",size);
    }
    return 0;
}
int do_t(char*names[])
{  
    // char t[3000]={0};
    struct stat buf1;
    struct stat buf2;
    for(int i=0;i<c-1;i++)
    {
        for(int j=0;j<c-1-i;j++)
        {  
             char t[128];
            // strcpy(t,p);
            // size_t len=strlen(p);
            // strcpy(&t[len],"/");
            // strcpy(&t[len+1],names[j]);
            // char m[128];
            // strcpy(m,p);
            // size_t len1=strlen(p);
            // strcpy(&m[len1],"/");
            // strcpy(&m[len1+1],names[j+1]);
            if (stat(names[j], &buf1) == -1 || stat(names[j+1], &buf2) == -1) {
                perror("stat");
                return -1; 
            }
            if(buf1.st_mtime<buf2.st_mtime)
            {  
                
                strcpy(t,names[j]);
                strcpy(names[j],names[j+1]);
                strcpy(names[j+1],t);
            }
        }
    }
    return 0;

}
int do_r(char*names[])
{
    int start = 0;
    int end = c - 1;
    while (start < end) {
        char *t = names[start];
        names[start] = names[end];
        names[end] = t;
        start++;
        end--;
    }
    return 0;
}
int do_name(char pathname[])
{
    // DIR* dir;
    // struct dirent *entry;
    // dir=opendir(pathname);
    // if(dir==NULL)
    // {
    //     printf("lslslslss%s\n",pathname);
    //     perror("opendir");
    //     printf("%s\n",pathname);
    // }
    // while((entry=readdir(dir))!=NULL)
    // {
    //     char * tm=(char*)malloc(4000*sizeof(char));
    //     strcpy(tm,pathname);
    //     int l=strlen(pathname);
    //     strcpy(&tm[l],"/");
    //     strcpy(&tm[l+1],entry->d_name);
    //     names[c]=(char*)malloc(4000*sizeof(char));
    //     // names[c++]=tm;
    //     strcpy(names[c],tm);
    //     c++;
        
    //     free(tm);
        
    // }
    // closedir(dir);
    // return 0;
    DIR *dir;
    struct dirent *entry;
    dir = opendir(pathname);
    if (dir == NULL) {
        perror("opendir");
        return -1; // 返回错误码
    }
     while ((entry = readdir(dir)) != NULL) {
        if ((a_flag==0)&&(strncmp(entry->d_name, ".",1) == 0 || strncmp(entry->d_name, "..",2) == 0) ){ 
            continue;
        }
        size_t len = strlen(pathname) + strlen(entry->d_name) + 2; // +2 for '/' and '\0'
        char *tm = malloc(len * sizeof(char));
        if (tm == NULL) {
            perror("malloc");
            closedir(dir);
            return -1; // 返回错误码
        }
 
        snprintf(tm, len, "%s/%s", pathname, entry->d_name);
 
        names[c] = malloc(len * sizeof(char));
        if (names[c] == NULL) {
            perror("malloc");
            free(tm);
            closedir(dir);
            return -1; // 返回错误码
        }
 
        strcpy(names[c], tm);
        c++;
 
        free(tm);

    }
 
    closedir(dir);
    return 0; // 成功返回
}


