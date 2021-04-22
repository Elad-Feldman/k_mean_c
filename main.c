#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
float find_distance(float dot[],float center[], int d){
    float dis = 0;
    for (int i = 0; i < d; i++)
        dis += (dot[i] - center[i]) * (dot[i] - center[i]);
    return  dis;

    }

void init_dots_list(float** dot_list, int start,int end,int d){
    for(int i = start; i < end; i++)
        dot_list[i] = (float*) calloc(d,sizeof(float));


}
void print_Arr(float arr[], int d) {
    printf("[");
    for (int i = 0; i < d; i++) {
        printf("%0.2f,", arr[i]);

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
    int d = count_dim(line)+1;
    int n=k;
   ;
    // initialize dot list and cluster list
    float** dot_list = (float **)malloc(k *sizeof(float*));
    float** cluster_list = (float **)malloc(k *sizeof(float*));
    init_dots_list(dot_list,0,k,d);
    init_dots_list(cluster_list,0,k,d);





    do{
        string_to_floats(line, dot_list[count], d);
        count++;
        if (count==n){
            n =n * 2;
            dot_list = (float **) realloc(dot_list, n * sizeof(dot_list[0]));
            init_dots_list(dot_list,n/2,n,d);
        } // double the size of array if needed
    } while ((scanf("%s", line)) == 1);

    printf("Summery:Got %d dots, each dot have %d values \n",count,d);
    dot_list = (float **)realloc(dot_list,count *sizeof(float *));

    // initialize loctions ans cluster's size
    int dot_at[count];
    int move_dot_to[count];
    int cluster_size[count];
    for(int i = 0; i < count; i++){
        if (i<k){
            dot_at[i]=i;
            cluster_size[i] = 1;
        }
        else
            dot_at[i]=-1;
        move_dot_to[i] = 0 ;
    }



    print_matrix(dot_list,count,d);
    init_cluster_values(dot_list,cluster_list,k,d);
    print_matrix(cluster_list,k,d);




}