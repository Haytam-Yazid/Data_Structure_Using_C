#include <stdio.h>
#include <string.h>

typedef struct
{
    char city_name[60];
    char country_name[60];
    double num_vis;
    char most_vist_place[60];
} city_info;

void menu(void);
int content_file_screen(void);
int load_file_to_array(city_info *arr);
int get_ranking(city_info *arr, char *name_city, int n);
city_info get_city_info(city_info *arr, const char *name_city, int n);
int update_tourist(city_info *arr, char *name_city, int n, double num_vis);
void sort_city(city_info *arr, int n);
int find_most_vis_cities(city_info *arr, char *name_country, int n);
int remove_city(city_info *arr, int* n, char *name_city);

int main(){
    int menu_choice, checker, num_coun, rank, A, B, C, rtn;
    int flag = 0, f_case = 0;
    char name_city[60], name_country[60];
    double new_num;
    city_info arr[100], info;

    do
    {
        menu();
        scanf("%d", &menu_choice);
        getchar();
        switch (menu_choice)
        {
        case 1: // View File Content
            num_coun = content_file_screen();
            printf("\n\t\tThe file contains %d cities\n", num_coun);
            f_case = 1;
            break;
        case 2: // Load File to Array
            if(f_case == 1){
                checker = load_file_to_array(arr);
                if (checker == 1){
                    printf("\n\t\tLoad Successfully!!!!!");
                    flag = 1;
                }else{
                    printf("\n\t\tLoad Unsuccessfully!!!!!");
                }
            }else{
                printf("\n\t\tChoice 1 should be excused first!!!!\n");
            }
            break;
        case 3: // Search ranking of a city
            if (flag == 1)
            {
                printf("\n\t\tEnter the name of city, to see it's rank: ");
                fgets(name_city, 60, stdin);
                rank = get_ranking(arr, name_city, num_coun);
                if (rank == (-1))
                {
                    printf("\n\t\tCity not found!!!");
                }
                else
                {
                    A = (strlen(name_city));
                    if (name_city[A - 1] == '\n')
                    {
                        name_city[A - 1] = '\0';
                    }
                    printf("\n\t\t%s has the rank#%d", name_city, (rank + 1));
                }
            }
            else
            {
                printf("\n\t\tChoice 2 should be exucted first!!!\n");
            }
            break;
        case 4: // Search Info about a city
            if (flag == 1)
            {
                printf("\n\t\tEnter the name of city to see its info: ");
                fgets(name_city, 100, stdin);
                info = get_city_info(arr, name_city, num_coun);
                rank = get_ranking(arr, name_city, num_coun);
                if (rank == (-1))
                {
                    printf("\n\t\tCity Not Found\n");
                }
                else
                {
                    A = (strlen(info.city_name));
                    if (info.city_name[A - 1] == '\n')
                    {
                        info.city_name[A - 1] = '\0';
                    }

                    B = (strlen(info.country_name));
                    if (info.country_name[B - 1] == '\n')
                    {
                        info.country_name[B - 1] = '\0';
                    }

                    C = (strlen(info.most_vist_place));
                    if (info.most_vist_place[C - 1] == '\n')
                    {
                        info.most_vist_place[C - 1] = '\0';
                    }

                    printf("\n\t\t%s is ranked#%d located in %s receiving %f tourists yearly and the most visited place is %s.", info.city_name, (rank + 1), info.country_name, info.num_vis, info.most_vist_place);
                }
            }
            else
            {
                printf("\n\t\tChoice 2 should be exucted first!!!\n");
            }
            break;
        case 5: // Updating num of tourists of a city
            if (flag == 1){
                printf("\n\t\tEnter the name of city to update the num of visitors: ");
                fgets(name_city, 60, stdin);
                printf("\n\t\tEnter the new number of the visitors: ");
                scanf("%lf", &new_num);
                checker = update_tourist(arr, name_city, num_coun, new_num);
                if (checker = 1){
                    sort_city(arr, num_coun);
                    printf("\n\t\tUpdate successfully");
                }else{
                    printf("\n\t\tUpdate unsuccessfully!!!!!");
                }
            }else{
                printf("\n\t\tChoice 2 should be exucted first!!!\n");
            }
            break;
        case 6: // Find most visited cities in a country
            if (flag == 1){
                printf("\n\t\tEnter the name of country to find most visited cities in it: ");
                fgets(name_country, 60, stdin);
                rtn = find_most_vis_cities(arr, name_country, num_coun);
                if (rtn == 1){
                    printf("\n\t\tPlease check the content of your output file");
                }else{
                    printf("\n\t\tCountry Not Found!!!");
                }
            }else{
                printf("\n\t\tChoice 2 should be exucted first!!!\n");
            }
            break;
        case 7: // Remove a city
            if (flag == 1){
                printf("\n\t\tEnter the city that you wanna remove: ");
                fgets(name_city, 60, stdin);
                checker = remove_city(arr, &num_coun, name_city);
                if (checker == 1){
                    printf("\n\t\tCity removed successfully!!");
                }else{
                    printf("\n\t\tRemove unsuccessfully!!!!");
                }
            }else{
                printf("\n\t\tChoice 2 should be exucted first!!!\n");
            }
            break;
        default:
            printf("\n\t\tError!!, Enter a number from 1 to 8\n");
        }
    } while (menu_choice != 8);
    printf("\n\t\tYou decide to quit!!!\n");

    return 0;
}

void menu(void)
{
    printf("\n\t\t---------------Menu---------------\n");
    printf("\n\t\t1. View File Content\n");
    printf("\n\t\t2. Load File Data to Array\n");
    printf("\n\t\t3. Get ranking of a city X\n");
    printf("\n\t\t4. Get Information about a city X\n");
    printf("\n\t\t5. Update the number of tourist of a city X\n");
    printf("\n\t\t6. Find the most visited cities in a country\n");
    printf("\n\t\t7. Remove a city from the ranking\n");
    printf("\n\t\t8. Quit\n");
    printf("\n\t\t----------------------------------\n");
    printf("\n\t\tEnter your choice:");
}

int content_file_screen(void){
    FILE *infp;
    int i, flag, count = 0;
    char line[50], ch;

    infp = fopen("data.txt", "r");
    if (infp == NULL){
        printf("file does not exist");
    }
    else
        while (fgets(line, 50, infp) != NULL)
        {
            count++;
        }
    fseek(infp, 0, 0);

    do
    {
        flag = 0;
        if (!feof(infp))
        {
            flag = 1;
            for (i = 0; i < 25; i++)
            {
                fgets(line, 50, infp);
                printf("\n\t\t%s", line);
            }

            printf("\n\t\tPress enter to view the next five cities: ");
            scanf("%c", &ch);
        }
        if (flag == 0)
            break;
    } while (ch == '\n');

    fclose(infp);
    return (count / 5);
}

int load_file_to_array(city_info *arr)
{
    FILE *infp;
    int i = 0;
    char line[50];

    infp = fopen("data.txt", "r");
    if (infp == NULL)
    {
        printf("\n\t\t File not found\n");
    }
    else
        while (!feof(infp))
        {
            fgets(arr[i].city_name, 50, infp);
            fgets(arr[i].country_name, 50, infp);
            fscanf(infp, "%lf", &arr[i].num_vis);
            getc(infp);
            fgets(arr[i].most_vist_place, 50, infp);
            fgets(line, 50, infp);
            i++;
        }
    fclose(infp);
    return 1;
}

int get_ranking(city_info *arr, char *name_city, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (strcmp(name_city, arr[i].city_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

city_info get_city_info(city_info *arr, const char *name_city, int n)
{
    int i;
    city_info X;
    for (i = 0; i < n; i++)
    {
        if (strcmp(name_city, arr[i].city_name) == 0)
        {
            return arr[i];
            break;
        }
    }
    strcmp(X.city_name, "NOT FOUND");
    return (X);
}

int update_tourist(city_info *arr, char *name_city, int n, double num_vis)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (strcmp(name_city, arr[i].city_name) == 0)
        {
            arr[i].num_vis = num_vis;
            return 1;
        }
    }
}

void sort_city(city_info *arr, int n)
{
    int i, j, flag;
    city_info comp;

    for (i = 0; i < n - 1; i++){
        flag = 0;
        for (j = 0; j < n - 1 - i; j++){
            if (arr[j].num_vis < arr[j + 1].num_vis){
                comp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = comp;
                flag = 1;
            }
        }
        if (flag == 0){
            break;
        }
    }
}

int find_most_vis_cities(city_info *arr, char *name_country, int n)
{
    int i, flag = 0;
    FILE *outfp;

    for (i = 0; i < n; i++){
        if (strcmp(name_country, arr[i].country_name) != 0){
            continue;
        }else{
            outfp = fopen("file.txt", "a+");
            fprintf(outfp, "%s", arr[i].city_name);
            fprintf(outfp, "%s", arr[i].most_vist_place);
            fprintf(outfp, "-------------------------------------\n");
            flag = 1;
            fclose(outfp);
        }
    }
    if (flag == 1){
        return 1;
    }else{
        return (-1);
    }
}

int remove_city(city_info *arr, int* n, char *name_city)
{
    int i, j;
    for (i = 0; i < (*n); i++){
        if (strcmp(arr[i].city_name, name_city) == 0){
            for (j = i; j < (*n); j++){
                arr[j] = arr[j + 1];
            }
            (*n)--;
            return 1;
        }
    }
    return 0;
}