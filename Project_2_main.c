#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define size 3

typedef struct
{
    char name[30];
    char id[35];
    double salary;
} a_info;

typedef struct nd
{
    a_info X;
    struct nd *next;
} w_info;

int N = 0;

void menu(void)
{
    printf("\n\t\t--------------Menu---------------\n");
    printf("\n\t\t1. Start Calling for Applications\n");
    printf("\n\t\t2. Apply to SSE\n");
    printf("\n\t\t3. Drop an Application\n");
    printf("\n\t\t4. View list of Applicants\n");
    printf("\n\t\t5. Calling for Applications is Over\n");
    printf("\n\t\t6. Quit!!\n");
    printf("\n\t\t---------------------------------\n");
    printf("\n\t\t Enter your choice (1 to 6)\n");
}

void id_generation(char *name, char *id)
{

    sprintf(id, "%d_%s", ++N, name);

    if (id[strlen(id) - 1] == '\n')
    {
        id[strlen(id) - 1] = '\0';
    }
}

void inqueue_o(a_info *o_queue, int **rear, int n, a_info ToEnqueue)
{
    if ((**rear) == size - 1)
        return;
    else
    {
        o_queue[++(**rear)] = ToEnqueue;
    }
}

a_info dequeue_o(a_info *o_queue, int **rear)
{
    a_info served;
    int i;
    if (**rear == -1)
    {
    }
    else
    {
        (served) = o_queue[0];
        for (i = 1; i <= **rear; i++)
            o_queue[i - 1] = o_queue[i];
        (**rear)--;
        return served;
    }
}
void inqueue_w(w_info ***pfront, w_info ***prear, w_info *ToEnqueue)
{
    if ((**pfront) == NULL)
    {
        (**pfront) = ToEnqueue;
    }
    else
    {
        if (((**prear) == NULL))
            (**prear) = (**pfront);
        (**prear)->next = ToEnqueue;
        (**prear) = (**prear)->next;
    }
}

a_info dequeue_w(w_info ***pfront, w_info ***prear)
{
    w_info *temp;
    a_info served;
    if ((**pfront) == NULL)
        ;
    else
    {
        served = (**pfront)->X;
        temp = (**pfront);
        (**pfront) = (**pfront)->next;
        free(temp);
    }
    if ((**pfront) == NULL)
    {
        (**prear) = NULL;
    }
    return served;
}
a_info get_data(FILE *infp)
{
    int len;
    char line[50], ch;
    a_info data;

    fgets(data.name, 30, infp);
    len = strlen(data.name);
    if (data.name[len - 1] == '\n')
    {
        data.name[len - 1] = '\0';
    }
    fscanf(infp, "%lf", &data.salary);
    ch = fgetc(infp);
    fgets(line, 50, infp);
    id_generation(data.name, data.id);

    return data;
}

void print_position(a_info data, int n)
{
    if (n == 1)
    {
        printf("\n\t\tDear %s, your file was received. You are#%d in the official queue. Thanks for applying!\n", data.name, N);
    }
    else
        printf("\n\t\tDear %s, your file was received. You are#%d in the waiting queue. Thanks for applying!\n", data.name, N);
}

void start_call_app(a_info *o_queue, w_info **w_pfront, w_info **w_prear, int *rear)
{
    FILE *infp;
    a_info data;
    w_info *data_w;
    int chek;

    infp = fopen("Applicants.txt", "r");
    if (infp == NULL)
    {
        printf("\n\t\tFile does not exist\n");
    }
    else
    {
        while (!feof(infp))
        {
            if (*rear != size - 1)
            {
                data = get_data(infp);
                inqueue_o(o_queue, &rear, size, data);
                chek = 1;
                print_position(data, chek);
            }
            else
            {
                data_w = (w_info *)malloc(sizeof(w_info));
                data_w->X = get_data(infp);
                data_w->next = NULL;
                inqueue_w(&w_pfront, &w_prear, data_w);
                chek = 2;
                print_position(data_w->X, chek);
            }
        }
    }
    fclose(infp);
}

void apply_to_sse(a_info *o_queue, int *rear, w_info **w_front, w_info **w_rear, a_info data)
{
    int flag;
    w_info *new_node;
    if (*rear != size - 1)
    {
        inqueue_o(o_queue, &rear, size, data);
        flag = 1;
    }
    else
    {
        new_node = (w_info *)malloc(sizeof(w_info));
        strcpy(new_node->X.name, data.name);
        strcpy(new_node->X.id, data.id);
        new_node->X.salary = data.salary;
        new_node->next = NULL;
        inqueue_w(&w_front, &w_rear, new_node);
    }
    print_position(data, flag);
}

int drop_applicant(a_info *o_queue, int *rear, w_info **w_front, w_info **w_rear, char *name)
{
    a_info data;
    w_info *temp;
    w_info *walker;
    int flag = 0;
    int s= size;

    for (int i = 0; i <= (*rear); i++)
    {
        if (strcmp(name, o_queue[i].name) == 0)
        {
            for (int j = i; j < (*rear); j++)
            {
                o_queue[j] = o_queue[j + 1];
            }
            (*rear)--;

            if((*w_front) != NULL){
                data = dequeue_w(&w_front, &w_rear);
                inqueue_o(o_queue, &rear, size, data);
            }

            flag = 1;


            break;
        }
    }

    if(flag == 1){
        return flag;
    }else{
        if (strcmp((*w_front)->X.name, name) == 0){
        temp = *w_front;
        (*w_front) = (*w_front)->next;
        flag = 1;
        free(temp);
    }else{
        while(walker != NULL){
            if(strcmp((walker->next->X.name), name) == 0){
                temp = walker->next;
                walker->next = temp->next;
                flag = 1;
                free(temp);
                break;
            }
        }
    }

}
    return flag;
}

void traverse_o(a_info *o_queue, int rear)
{
    int i;

    for (i = 0; i <= rear; i++)
    {
        printf("\n\t\t---Official list----\n");
        printf("\n\t\t%s", o_queue[i].name);
        printf("\n\t\t%s", o_queue[i].id);
        printf("\n\t\t%lf", o_queue[i].salary);
        printf("\n\t\t----------------------------\n");
    }
}

void traverse_w(w_info *w_front)
{

    if (w_front == NULL)
    {
        printf("\n\t\tWaiting list is empty\n");
    }
    else
    {
        while (w_front != NULL)
        {
            printf("\n\t\t----Waiting list----\n");
            printf("\n\t\t%s", w_front->X.name);
            printf("\n\t\t%s", w_front->X.id);
            printf("\n\t\t%lf", w_front->X.salary);
            printf("\n\t\t----------------------------\n");
            w_front = w_front->next;
        }
    }
}

void generate_locale_time(char *time_gen, size_t Size)
{
    time_t t;
    struct tm *timeinfo;

    time(&t);
    timeinfo = localtime(&t);

    strftime(time_gen, Size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

int serve_applicantes(a_info *o_queue, int *rear, w_info **w_front, w_info **w_rear, double ex_salary)
{
    FILE *outfp;
    int checker = 0;
    char filename[100];
    a_info served;
    char buffer[60];

    do
    {
        if (*rear != -1)
        {
            served = dequeue_o(o_queue, &rear);
        }
        else if (*w_front != NULL)
        {
            served = dequeue_w(&w_front, &w_rear);
        }

        if (served.salary <= ex_salary)
        {
            strcpy(filename, "Acceptance_");
            strcat(filename, served.name);
            strcat(filename, ".txt");
            outfp = fopen(filename, "w");
            fprintf(outfp, "Congratulations, you have been selected for the job. We hope that you will have a great time working with SSE!!\n");
            generate_locale_time(buffer, sizeof(buffer));
            fprintf(outfp, "%s\n", buffer);
            fclose(outfp);
            checker++;
        }
        else
        {
            strcpy(filename, "Rejection_");
            strcat(filename, served.name);
            strcat(filename, ".txt");
            outfp = fopen(filename, "a");
            fprintf(outfp, "Sorry, but you did not get selected because the salary you are expecting is too much higher than expected!!\n");
            generate_locale_time(buffer, sizeof(buffer));
            fprintf(outfp, "%s\n", buffer);
            fclose(outfp);
        }

    } while (checker < 3 && (*rear != -1 || (*rear == -1 && *w_front != NULL)));
    return 1;
}

int main()
{
    int choise, list, status, flag = 0, checker;
    a_info o_queue[size];
    a_info data;
    w_info *w_front = NULL, *w_rear = NULL;
    int rear = -1, front = 0, stat = 0;
    double expected_salary;
    char a_name[50];

    do
    {
        menu();
        scanf("%d", &choise);
        switch (choise)
        {
        case 1:
            if (flag == 0)
            {
                start_call_app(o_queue, &w_front, &w_rear, &rear);
                printf("\n\t\tLoad successfully from the file\n");
                flag = 1;
            }
            else
            {
                printf("\n\t\tFile Already opened\n");
            }
            break;
        case 2:
            if (flag == 1)
            {
                getchar();
                printf("\n\t\tEnter the name of the applicant\n");
                fgets(data.name, 30, stdin);
                int len = strlen(data.name);
                if (data.name[len - 1] == '\n')
                {
                    data.name[len - 1] = '\0';
                }
                printf("\n\t\tEnter the expected salary of the applicant\n");
                scanf("%lf", &data.salary);
                id_generation(data.name, data.id);
                apply_to_sse(o_queue, &rear, &w_front, &w_rear, data);
            }
            else
            {
                printf("\n\t\tChoice 1 should be excused first\n");
            }
            break;
        case 3:
            if (flag == 1)
            {
                getchar();
                printf("\n\t\tEnter the name of the applicant who want to drop his application\n");
                fgets(a_name, 50, stdin);
                int len = strlen(a_name);
                if (a_name[len - 1] == '\n')
                {
                    a_name[len - 1] = '\0';
                }
                status = drop_applicant(o_queue, &rear, &w_front, &w_rear, a_name);
                if (status == (0))
                {
                    printf("\n\t\tApplicant Not Found\n");
                }
                else
                {
                    printf("\n\t\tApplicant droped with success\n");
                }
            }
            else
            {
                printf("\n\t\tChoice 1 should be excused first\n");
            }
            break;
        case 4:
            if (flag == 1)
            {
                printf("\n\t\tWhat list you want to see: \n\t\t 1 for official list and 2 for waiting list\n");
                scanf("%d", &list);
                if (list == 1)
                {
                    traverse_o(o_queue, rear);
                }
                else
                {
                    traverse_w(w_front);
                }
            }
            else
            {
                printf("\n\t\tChoice 1 should be excused first\n");
            }
            break;
        case 5:
            if (flag == 1)
            {
                printf("\n\t\tEnter the expected salary amout for the tree applicant you are going to accepte them:\n");
                scanf("%lf", &expected_salary);
                checker = serve_applicantes(o_queue, &rear, &w_front, &w_rear, expected_salary);
                if (checker == 1)
                {
                    printf("\n\t\tYour files has been created please check them!!!\n");
                }
                else
                    printf("\n\t\tYour files didn't get created it may due the queues wasn't filled\n");
            }
            else
            {
                printf("\n\t\tChoice 1 should be excused first\n");
            }
            break;
        case 6:
            printf("\n\t\tYou dicide to QUIT!!\n");
            break;
        default:
            printf("\n\t\tInvalid Choice!\n");
        }
    } while (choise != 6);
    return 0;
}