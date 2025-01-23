#include <stdio.h>
#include <stdlib.h>
 
// 降序比较函数
int compare_desc(const void *a, const void *b) {
    return (*(int*)b - *(int*)a); // 注意这里是b - a，实现降序
}
int main() {
    int arr[] = {5, 2, 9, 1, 5, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    qsort(arr, n, sizeof(int), compare_desc);
    
    // 打印排序后的数组
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}