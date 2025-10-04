#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
    char name[50];
    char phone_num[100];
    int priority_num;
}p_info;

void menu(void);
int start(p_info* arr, FILE* infp);
void swap(p_info* A, p_info* B);
int find_max(p_info* arr, int i, int j);
void heapify(p_info *arr, int parent_index, int last_index);
int add_new_patient(p_info *arr, int* N, p_info ToAdd);
p_info serve_most_urgent_element(p_info *arr, int *n_element);
int to_del(p_info *arr, int *n, char *name);
void print_list_patient(p_info* arr, int N);
int shift_end(p_info *arr, int n, FILE *infp);

int main(){

    p_info arr[10];
    p_info p_served;
    p_info ToAdd;
    FILE*infp;
    int choice, result, n_patients, flag = 0;
    char name[50], name_served[50];
    
    
    infp = fopen("data.txt", "r");

    if(infp == NULL){
        printf("\n\t\t File Not Found\n");
    }else{
        do{
            menu();
            printf("\n\t\tEnter your choice (1 to 7): ");
            scanf("%d", &choice);

            switch(choice){
                case 1:
                    if(flag == 1){
                        printf("\n\t\tFile is empty!!, check your heap\n");
                    }else{
                        n_patients = start(arr, infp);
                        printf("\n\t\tUpload Successful\n");
                        flag = 1;
                        fclose(infp);
                    }
                    break;
                case 2:
                    if(flag == 1){
                        printf("\n\t\tEnter the name of the patient:");
                        getchar();
                        fgets(ToAdd.name, 50, stdin);
                        int len = strlen(ToAdd.name);
                        if (ToAdd.name[len - 1] == '\n')
                        {
                            ToAdd.name[len - 1] = '\0';
                        }
                        printf("\n\t\tEnter the phone number:");
                        getchar();
                        fgets(ToAdd.phone_num, 50, stdin);
                        len = strlen(ToAdd.phone_num);
                        if (ToAdd.phone_num[len - 1] == '\n')
                        {
                            ToAdd.phone_num[len - 1] = '\0';
                        }
                        printf("\n\t\tWhat is the reason for the visit\n");
                        printf("\n\t\tFor A Medical checkup, press 5\n \n\t\tFor a an appointment, press 10\n \n\t\tFor an Emergency Case, press 15\n \n\t\tFor a Friendly visit, press 0\n");
                        scanf("%d", &ToAdd.priority_num);
                        result = add_new_patient(arr, &n_patients, ToAdd);
                        if(result == (-1)){
                            printf("\n\t\tThe list is Full\n");
                        }else{
                            printf("\n\t\t Patient is located at slot %d\n", (result + 1));
                        }
                    }else{
                        printf("\n\t\tOption 1 should be executed first!!\n");
                    }
                    
                    break;
                case 3:
                    if(flag == 1){
                        print_list_patient(arr, n_patients);
                    }else{
                        printf("\n\t\tOption 1 should be executed first!!\n");
                    }
                    
                    break;
                case 4:
                    if(flag == 1){
                        p_served = serve_most_urgent_element(arr, &n_patients);
                        printf("\n\t\t%s is served\n", p_served.name);
                    }
                    break;
                case 5:
                    if (flag == 1){
                        char ch = getchar();
                        printf("\n\t\tEnter the name of patient you want to delete\n");
                        fgets(name, 50, stdin);
                        int  len = strlen(name);
                        if (name[len - 1] == '\n')
                        {
                            name[len - 1] = '\0';
                        }
                        result = to_del(arr, &n_patients, name);
                        if (result == 1){
                            printf("\n\t\tthe patient deleted successfully\n");
                        }else
                            printf("\n\t\tthe patient does not exist !!!!\n");
                    }else{
                        printf("\n\t\tOption 1 should be executed first!!\n");
                    }
                break;
                    break;
                case 6:
                    if (flag == 1){
                    result = shift_end(arr, n_patients, infp);
                    if (result == 1)
                    {
                        printf("\n\t\tpatients uplaod to the file with success\t\t\n");
                    }
                    else
                        printf("\n\t\t the list is empty\n");
                    printf("\n\t\t see you !!!! \t\t\n");
                    }else{
                    printf("\n\t\tOption 1 should be executed first!!\n");
                    }
                    break;
                case 7:
                    printf("\n\t\tYou Quit!!\n");
                    break;
                default:
                    printf("\n\t\tWrong choice!!\n");
                    break;
            }
        }while(choice != 7);
    }

    return 0;
}

void menu(void){
    printf("\n\t\t---------------Menu---------------\n");
    printf("\n\t\t1. Start\n");
    printf("\n\t\t2. Add new patient\n");
    printf("\n\t\t3. Print the list of patient\n");
    printf("\n\t\t4. Serve a patient\n");
    printf("\n\t\t5. Drop my visit\n");
    printf("\n\t\t6. Doctor visits OVER\n");
    printf("\n\t\t----------------------------------\n");
}

int start(p_info* arr, FILE* infp){
    int i = 0, last_index;
    char line[100];
    p_info data;

    while(!feof(infp)){

        fgets(arr[i].name, 50, infp);
        int len = strlen(arr[i].name);
        if (arr[i].name[len - 1] == '\n'){
            arr[i].name[len - 1] = '\0';
        }
        char ch = getchar();
        fgets(arr[i].phone_num, 50, infp);
        len = strlen(arr[i].phone_num);
        if (arr[i].phone_num[len - 1] == '\n'){
            arr[i].phone_num[len - 1] = '\0';
        }
        fscanf(infp, "%d", &arr[i].priority_num);
        ch = fgetc(infp);
        fgets(line, 100, infp);
        i++;
    }
    last_index = i - 1;
    for (i = (last_index - 1) / 2; i >= 0; i--)
        heapify(arr, i, last_index);
    return (last_index + 1);
}

void heapify(p_info *arr, int parent_index, int last_index){

  int left_child, right_child, largest;
  //base case
  if (parent_index > ((last_index - 1) / 2))
    return;
  else
  {
    left_child = (2 * parent_index) + 1;
    right_child = (2 * parent_index) + 2;

    if (right_child > (last_index))
      largest = find_max(arr, parent_index, left_child);
    else
    {
      largest = find_max(arr, parent_index, left_child);
      largest = find_max(arr, largest, right_child);
    }
    if (largest != parent_index)
    {
      swap(&arr[parent_index], &arr[largest]);
      heapify(arr, largest, last_index);
    }
  }
}

void swap(p_info* A, p_info* B){
    p_info temp;

    temp = *A;
    *A = *B;
    *B = temp;
}

int find_max(p_info* arr, int i, int j){
  if (arr[i].priority_num > arr[j].priority_num)
    return (i);
  else
    return (j);
}

int add_new_patient(p_info *arr, int* N, p_info ToAdd){
    int index, parent_index;
    
    arr[(*N)] = ToAdd;
    index = (*N);
    (*N) = (*N)+1;
    
    while(index >= 1){
        parent_index = ((index - 1)/2) - 1/2;
        if(arr[index].priority_num > arr[parent_index].priority_num){
            swap(&arr[index], &arr[parent_index]);
            index = parent_index;
        }else{
            return (index);
        }
    }
    return(index);
}

p_info serve_most_urgent_element(p_info *arr, int *n_element){
    p_info urgent_patient;
    int last_index;

    if (*n_element == 0){
        strcpy(urgent_patient.name,"Empty heaps");
        return(urgent_patient);
    }else{
        strcpy(urgent_patient.name, arr[0].name);
        arr[0] = arr[--(*n_element)];
        last_index = (*n_element) - 1;
        if (last_index < 0)
            return (urgent_patient);
        heapify(arr, 0, last_index);
        return (urgent_patient);
    }
}

int to_del(p_info *arr, int *n, char *name){
    int last_index, u, flag = 0;

    for (int i = 0; i < (*n); i++){
        if (strcmp(name, arr[i].name) == 0){
            for (int j = i; j < (*n); j++){
                arr[j] = arr[j + 1];
            }
            (*n)--;
            flag = 1;
            break;
        }
    }
    u = (*n);
    last_index = u - 1;
    for (u = (last_index - 1) / 2; u >= 0; u--){
        heapify(arr, u, last_index);
    }
    return (flag);
}

void print_list_patient(p_info*arr, int N){
    int i;
    p_info temp[10];

    if (N == 0){
        printf("\n\t\tHeap is empty\n");
    }else{
        int i;
        if (N == 0){
            printf("\n\t\tThe heap is EMPTY!\n");
        }else{
            printf("\n\t\tYour current heap has the following:\n\n\t\t");
            for (i = 0; i < N; i++){
                printf("\n\t\t%s", arr[i].name);
                printf("\n\t\t%s", arr[i].phone_num);
                printf("\n\t\t%d", arr[i].priority_num);
                printf("\n\t\t-----------------\n");
            }
        }
    }     
}

int shift_end(p_info *arr, int n, FILE *infp){
    int i;
    p_info checker;

    if (n != 0)
    {
        infp = fopen("data.txt", "w");
        for (i = 0; i < n; i++)
        {
            while (arr[i].priority_num == 15)
            {
                checker = serve_most_urgent_element(arr, &n);
                printf("\n\t\t %s come in \t\t\n", checker.name);
            }
            fprintf(infp, "\n%s \n %s \n 10\n", arr[i].name, arr[i].phone_num);
            fprintf(infp, "------------------------");
        }
        fclose(infp);
        return 1;
    }
    else
        return 0;
}