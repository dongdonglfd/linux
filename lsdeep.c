#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <locale.h>

#define COLOR_RESET "\x1B[0m"
#define COLOR_DIR "\x1B[34m"
#define COLOR_LINK "\x1B[36m"
#define COLOR_EXE "\x1B[32m"

typedef struct {
    int a, l, R, t, r, i, s;
    int dir_count;  // 需要显示目录名的目录数量
} Options;

typedef struct {
    char *path; //申请结构体数组，将目录打开后的文件存入path中，先排序在打印
    struct stat st;//读取文件信息
} Entry;

// 新增比较函数：按文件名排序
int compare_name(const void *a, const void *b) {
    const Entry *ea = a;
    const Entry *eb = b;
    const char *name_a = strrchr(ea->path, '/') ? strrchr(ea->path, '/') + 1 : ea->path;// 字符串中查找最后一个出现的 '/' 字符
    const char *name_b = strrchr(eb->path, '/') ? strrchr(eb->path, '/') + 1 : eb->path;
    return strcoll(name_a, name_b);//比较字符串的函数
}
//根据文件类型返回相应的颜色。
const char* get_color(const char *path) {
    struct stat st;
    if (lstat(path, &st) == -1) return COLOR_RESET;

    if (S_ISLNK(st.st_mode)) return COLOR_LINK;
    if (S_ISDIR(st.st_mode)) return COLOR_DIR;
    if (st.st_mode & S_IXUSR) return COLOR_EXE;
    return COLOR_RESET;
}
//将文件模式转换为字符串表示。
char* mode_to_str(mode_t mode, char *str) {
    strcpy(str, "----------");
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISLNK(mode)) str[0] = 'l';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';
    if (S_ISFIFO(mode)) str[0] = 'p';
    if (S_ISSOCK(mode)) str[0] = 's';

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';
    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
    return str;
}
//按修改时间排序的比较函数。
int compare_entries(const void *a, const void *b) {
    const Entry *ea = a, *eb = b;
    if (ea->st.st_mtime == eb->st.st_mtime)
        return strcoll(ea->path, eb->path);
    return (ea->st.st_mtime > eb->st.st_mtime) ? -1 : 1;
}
//打印文件或目录的详细信息。
void print_entry(const char *path, const char *name, Options opts) {
    struct stat st;
    if (lstat(path, &st) == -1) return;

    if (opts.i) printf("%8lu ", st.st_ino);
    if (opts.s) printf("%4ld ", st.st_blocks/2);

    if (opts.l) {
        char modestr[11];
        mode_to_str(st.st_mode, modestr);
        printf("%s %2ld %-8s %-8s %8ld ",
               modestr,
               st.st_nlink,
               getpwuid(st.st_uid)->pw_name,
               getgrgid(st.st_gid)->gr_name,
               st.st_size);

        char timebuf[80];
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&st.st_mtime));
        printf("%s ", timebuf);
    }

    const char *color = get_color(path);
    printf("%s%s%s", color, name, COLOR_RESET);
    //检查一个文件是否为符号链接，并打印出该符号链接指向的目标路径
    if (opts.l && S_ISLNK(st.st_mode)) {
        char linkbuf[PATH_MAX];
        ssize_t len = readlink(path, linkbuf, sizeof(linkbuf)-1);
        if (len != -1) {
            linkbuf[len] = '\0';
            printf(" -> %s", linkbuf);
        }
    }
    putchar('\n');
}

void process_path(const char *path, Options opts, int is_explicit_dir);
//处理目录，包括读取目录内容、排序、打印，并递归处理子目录（如果指定了-R选项）。
void process_dir(const char *dirpath, Options opts, int is_explicit_dir) {
    if (opts.dir_count++ > 0) putchar('\n');
    if (is_explicit_dir) printf("%s:\n", dirpath);

    DIR *dir = opendir(dirpath);
    if (!dir) {
        perror(dirpath);
        return;
    }

    Entry *entries = NULL;//可以看作申请了结构体Entry数组
    int count = 0;
    struct dirent *ent;
    //先把目录中的文件存入结构体path中，最后进入函数print_entry打印
    while ((ent = readdir(dir)) != NULL) {
        if (!opts.a && ent->d_name[0] == '.') continue;//用于处理隐藏文件

        char fullpath[PATH_MAX];
        if(strcmp(dirpath,"/")==0)
        {
            snprintf(fullpath, sizeof(fullpath), "/%s",  ent->d_name);//补充路径
        }
        else{
            snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, ent->d_name);//补充路径
        }
        // snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, ent->d_name);//补充路径
        entries = realloc(entries, (count+1)*sizeof(Entry));
        entries[count].path = strdup(fullpath);//用于复制一个字符串到新的内存空间
        lstat(fullpath, &entries[count].st);//将文件打开读取信息
        count++;
    }
    closedir(dir);
    //如果参数存在-t，-r，先进行排序
    if (opts.t) {
        qsort(entries, count, sizeof(Entry), compare_entries);
    } 
    else {
        qsort(entries, count, sizeof(Entry), compare_name);
    }
    if (opts.r) {
        for (int i = 0; i < count/2; i++) {
            Entry tmp = entries[i];
            entries[i] = entries[count-1-i];
            entries[count-1-i] = tmp;
        }
    }
    //依次进入print_entry打印
    for (int i = 0; i < count; i++) 
    {
        //用于查找一个字符在字符串中最后一次出现的位置，并返回指向该字符的指针及其后面的部分
        char *name = strrchr(entries[i].path, '/') + 1;
        print_entry(entries[i].path, name, opts);
    }
    //如果存在参数-R，进入process_path再进入该函数
    if (opts.R) {
        for (int i = 0; i < count; i++) {
            //判断是否为目录并且跳过当前目录和上级目录
            if (S_ISDIR(entries[i].st.st_mode) && 
                strcmp(entries[i].path + strlen(dirpath)+1, ".") != 0 &&
                strcmp(entries[i].path + strlen(dirpath)+1, "..") != 0) 
            {
                process_path(entries[i].path, opts, 1);
            }
        }
    }
    //释放内存
    for (int i = 0; i < count; i++) free(entries[i].path);
    free(entries);
}
//处理文件，即打印文件信息。
void process_file(const char *path, Options opts) {
    print_entry(path, path, opts);
}
//根据路径指向的是文件还是目录，调用process_file或process_dir。
void process_path(const char *path, Options opts, int is_explicit_dir) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        perror(path);
        return;
    }

    if (S_ISDIR(st.st_mode)) //判断是否为目录
    {
        process_dir(path, opts, is_explicit_dir);
    } else //是文件 
    {
        process_file(path, opts);
    }
}

int main(int argc, char **argv) {
    setlocale(LC_ALL, "");//用来设置程序的当前区域设置（locale）为环境变量所指定的值。
    Options opts = {0};//初始化
    char **paths = NULL;
    int path_count = 0;

    // 参数解析
    for (int i = 1; i < argc; i++) 
    {
        if (argv[i][0] == '-') {
            char *p = argv[i] + 1;//跳过-，读后面字母
            while (*p) {
                switch (*p) {
                    case 'a': opts.a = 1; break;
                    case 'l': opts.l = 1; break;
                    case 'R': opts.R = 1; break;
                    case 't': opts.t = 1; break;
                    case 'r': opts.r = 1; break;
                    case 'i': opts.i = 1; break;
                    case 's': opts.s = 1; break;
                    default: 
                        fprintf(stderr, "无效选项: -%c\n", *p);
                        exit(EXIT_FAILURE);
                }
                p++;
            }
        } 
        else //如果后面是文件路径，存入paths中
        {
            paths = realloc(paths, (path_count+1)*sizeof(char*));
            paths[path_count++] = argv[i];
        }
    }

    // 默认处理当前目录
    if (path_count == 0) {
        paths = realloc(paths, sizeof(char*));
        paths[path_count++] = ".";
    }

    // 处理多个路径
    opts.dir_count = 0;
    for (int i = 0; i < path_count; i++) {
        process_path(paths[i], opts, path_count > 1);
    }

    free(paths);
    return 0;
}