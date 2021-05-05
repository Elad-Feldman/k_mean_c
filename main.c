#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// run by gcc main.c && gcc  -o main main.c && main 3 200 < input1.txt

void init_cluster_values(double** dot_list,double** cluster_list, int k, int d)
{
    for(int i = 0; i < k; i++)
    {
        for(int j = 0; j < d; j++)
        {
            cluster_list[i][j] = dot_list[i][j];
        }
    }

}
void init_dots_list(double** dot_list, int start,int end,int d){
    for (int i = start; i < end; i++) {
        dot_list[i] = (double*)calloc(d, sizeof(double));
        assert(dot_list[i] != NULL);
    }
        

}
double find_distance(double *dot, double *center, int d){
    double dis = 0;
    for (int i = 0; i < d; i++)
        dis += (dot[i] - center[i]) * (dot[i] - center[i]);
    return  dis;

}
int get_index_of_closest_cluster(double* dot, double** cluster_list, int d, int k )
    {
    int j = 0;
    double min_dis = find_distance(dot, cluster_list[0], d);
    double tmp_dis;
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
void update_cluster_center(double* dot, double * center,int cluster_size,int d,int sign) {
    if (cluster_size+sign==0)
        printf("error \n ");
    double center_temp[d];
    for (int i = 0; i < d; i++)
        center_temp[i] = (center[i] * (cluster_size));

    for (int i = 0; i < d; i++){
        center_temp[i] += (dot[i]*sign);
        center[i] = center_temp[i] / (cluster_size+sign);

        }
}
void string_to_doubles(char *line,double arr[],int d )
{
    int i = 0;
    char* ptr = strtok(line, ",");
    while(ptr != NULL)
    {       
        arr[i] = atof(ptr);
        ptr = 	strtok(NULL, ",");
        i++;

    }   
    
    
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
    for (int i = 0; i < d; i++) {
        printf("%d,", arr[i]);

    }
    printf(",\n ");
}

void print_Arr(double* arr, int d) { 
    for (int i = 0; i < d; i++) {
        printf("%.4f",arr[i]);
        if (i < d - 1) {
            printf("%c", ',');
        }

    }
    
    printf("\n");
}
void print_matrix(double** mat,int n,int d){
    for (int i = 0; i < n; i++) {
        print_Arr(mat[i],d);
    }

}

int dynamic_scan(char *str, int size) {
    //The size is extended by the input with the value of the provisional
    char ch=getchar();
    int len = 0;
    int doubled_size = size;
    while (ch != '\n' && ch != EOF) {
        str[len] = ch;
        len++;
        if (len == (doubled_size)) {
            doubled_size = doubled_size*2;
            str = (char *) realloc ( str, (sizeof(char))* doubled_size);
            assert(str!=NULL);
        }
        ch = getchar();
        

    }
    if (len > size) {
        
        str = (char*)realloc(str, sizeof(ch) * (len));
        assert(str != NULL);
        }
    return len+10;
}
int main(int argc, char* argv[]) {
    int k, max_iter = 200;
    
    if (1)
    {   
        if (argc > 1)
            k = atoi(argv[1]);
        if (argc > 2)
            max_iter = atoi(argv[2]);
    }
    assert(k>0 && max_iter>0);
    int line_length = 17;
    
    char *line = malloc(line_length*sizeof(char));
    assert(line != NULL);
    
       line_length = dynamic_scan(line, line_length);
       int max_line_length = line_length;
    int count = 0;
    int d = count_dim(line) + 1;
    int n = 500;
    // initialize dot list.size=n and cluster list.size = k
    double** dot_list = (double **)malloc(n *sizeof(double*));
    assert(dot_list != NULL);
    init_dots_list(dot_list, 0, n, d);
    double** cluster_list = (double **)malloc(k *sizeof(double*));
    assert(dot_list != NULL);
    init_dots_list(cluster_list, 0, k, d);
     //load dots into list
    do{
        string_to_doubles(line, dot_list[count], d);
        memset(line, 0, max_line_length);
        count++;
        if (count == n) {
            n = n * 2;
            dot_list = (double**)realloc(dot_list, n * sizeof(dot_list[0]));
            assert(dot_list!=NULL);
            init_dots_list(dot_list, n / 2, n, d);
            
        }
        line_length = dynamic_scan(line, max_line_length);
        if (line_length > max_line_length) {
            max_line_length = line_length;
        }
    } while ( line_length> 10);
        free(line);
    dot_list = (double**)realloc(dot_list, count * sizeof(double*));
    assert(dot_list != NULL);
    n = count;
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
    free(dot_list);
    free(cluster_list);
    
}

