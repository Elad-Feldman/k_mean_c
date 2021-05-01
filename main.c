#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// run by gcc main.c && gcc  -o main main.c && main 3 200 < input1.txt

void init_cluster_values(float** dot_list,float** cluster_list, int k, int d)
{
    for(int i = 0; i < k; i++)
    {
        for(int j = 0; j < d; j++)
        {
            cluster_list[i][j] = dot_list[i][j];
        }
    }

}
void init_dots_list(float** dot_list, int start,int end,int d){
    for(int i = start; i < end; i++)
        dot_list[i] = (float*) calloc(d,sizeof(float));


}
float find_distance(float *dot, float *center, int d){
    float dis = 0;
    for (int i = 0; i < d; i++)
        dis += (dot[i] - center[i]) * (dot[i] - center[i]);
    return  dis;

}
int get_index_of_closest_cluster(float* dot, float** cluster_list, int d, int k )
    {
    int j = 0;
    float min_dis = find_distance(dot, cluster_list[0], d);
    float tmp_dis;
    for (int i = 1; i < k; i++)
    {
        tmp_dis = find_distance(dot, cluster_list[i], d);
        if (tmp_dis <= min_dis)
        {
            min_dis = tmp_dis;
            j = i;
        }
    }
    return j;
}
void update_cluster_center(float* dot, float * center,int cluster_size,int d,int sign) {
    if (cluster_size+sign==0)
        printf("error \n ");
    float center_temp[d];
    for (int i = 0; i < d; i++)
        center_temp[i] = (center[i] * (cluster_size));

    for (int i = 0; i < d; i++){
        center_temp[i] += (dot[i]*sign);
        center[i] = center_temp[i] / (cluster_size+sign);

        }
}
float *string_to_floats(char line[],float arr[],int d )
{
    int i = 0;
    char* ptr = strtok(line, ",");
    while(ptr != NULL)
    {

        arr[i] = atof(ptr);
        ptr = 	strtok(NULL, ",");
        i++;


    }
   // print_Arr(arr, d);
    return arr;
}
int count_dim(char line[])
{

    char c=',';
    int i,count=0;


    for(i=0;line[i];i++)
    {
        if(line[i]==c)
        {
            count++;
        }
    }
    return count;
}
void print_Arr_int(int* arr, int d) {
    printf("[");
    for (int i = 0; i < d; i++) {
        printf("%d,", arr[i]);

    }
    printf("],\n ");
}

void print_Arr(float* arr, int d) {
    printf("[");
    for (int i = 0; i < d; i++) {
        printf("%0.4f,", arr[i]);

    }
    printf("],\n ");
}
void print_matrix(float** mat,int n,int d){
    printf("\n========================\n");
    printf("[");
    for (int i = 0; i < n; i++) {
        print_Arr(mat[i],d);
    }
    printf("]");
    printf("\n========================\n");

}
int main(int argc, char* argv[]) {
    int k, max_iter=200;
    char line[256];
    if (1)
    {
        if (argc > 1)
            k = atoi(argv[1]);
        if (argc > 2)
            max_iter= atoi(argv[2]);
        printf("k = %d \n",k);
        printf("max_iter= %d \n",max_iter);
    }

    int count = 0;

    scanf("%s", line);
    int d = count_dim(line) + 1;
    int n = 500;
    // initialize dot list.size=n and cluster list.size = k
    float** dot_list = (float **)malloc(n *sizeof(float*));
    init_dots_list(dot_list, 0, n, d);
    float** cluster_list = (float **)malloc(k *sizeof(float*));
    init_dots_list(cluster_list, 0, k, d);

     //load dots into list

    do{
        string_to_floats(line, dot_list[count], d);
        count++;
        if (count==n){
            n =n * 2;
            dot_list = (float **) realloc(dot_list, n * sizeof(dot_list[0]));
            init_dots_list(dot_list,n/2, n, d);
        } // double the size of array if needed
    } while ((scanf("%s", line)) == 1);

    printf("Summery:Got %d dots, each dot have %d values \n", count, d);
    dot_list = (float **)realloc(dot_list,count *sizeof(float *));
    n = count;
    // initialize loctions ans cluster's size
    int dot_at[n];
    int move_dot_to[n];
    int cluster_size[k];
    for(int i = 0; i < n; i++){
        dot_at[i]=-1;
        move_dot_to[i] = 0 ;
    }
    for(int i = 0; i < k; i++) {
        dot_at[i] = i;
        cluster_size[i] = 1;

    }

  //  print_matrix(dot_list, count, d);
    init_cluster_values(dot_list, cluster_list, k, d);

    int is_a_cluster_changed=1;
    int count_iter = 0;

    while (count_iter<max_iter && is_a_cluster_changed) {
        is_a_cluster_changed = 0;
        count_iter++;
        for (int i = 0; i < n; i++) //find nearest clusters
            move_dot_to[i] = get_index_of_closest_cluster(dot_list[i], cluster_list, d, k);

        for (int j = 0; j < n; j++) {// update clusters
            if (dot_at[j] == -1) {
                dot_at[j] = move_dot_to[j];
                update_cluster_center(dot_list[j], cluster_list[move_dot_to[j]], cluster_size[move_dot_to[j]], d, 1); //add dot to center
                cluster_size[move_dot_to[j]]++;
                is_a_cluster_changed = 1;
            } else {
                if (dot_at[j] != move_dot_to[j]) {
                    update_cluster_center(dot_list[j], cluster_list[dot_at[j]], cluster_size[dot_at[j]], d,-1); //remove dot from center
                    update_cluster_center(dot_list[j], cluster_list[move_dot_to[j]], cluster_size[move_dot_to[j]], d,1); //add dot to center
                    cluster_size[dot_at[j]]--;
                    cluster_size[move_dot_to[j]]++;
                    dot_at[j] = move_dot_to[j];
                    is_a_cluster_changed = 1;
                }
            }
        }
    }

    print_matrix(cluster_list, k, d);

    /// free memory
    for(int i = 0; i < count; i++){
        free(dot_list[i]);
        if (i<k)
            free(cluster_list[i]);
        }
    free(dot_list);free(cluster_list);



}