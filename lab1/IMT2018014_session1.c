#include<stdio.h>
#include <stdlib.h>
struct student
{
    int Roll_no;
    char Name[30];
    int Age;
};

int save_num_text( char *fileName )
{
    FILE *ptr=fopen(fileName,  "w");
    if(ptr==NULL)
    {
        return 1;
    }
    for(int i=0;i<20;i++)
    {
        fprintf(ptr,  "%d\n",   i+1);
    }
    fclose(ptr);
    return 0;
}

int read_num_text( char *fileName )
{
    FILE *ptr=fopen(fileName,  "r");
    if((ptr)==NULL)
    {
        return 1;
    }
    int x;
    for(int i=0;i<20;i++)
    {
        fscanf(ptr,  "%d",  &x);
        printf("%d ",  x);
    }
    fclose(ptr);
    return 0;
}

int save_struct_text( char *fileName )
{
    FILE *ptr=fopen(fileName,  "w");
    if((ptr)==NULL)
    {
        return 1;
    }
    struct student arr[5]={{1,  "sawlani",  10},  {2,  "panda",  20},  {3,  "gaurav",  30},  {4,  "miiya",  40},  {5,  "bongoli",  50}};
    for(int i=0;i<5;i++)
    {
        fprintf(ptr,  "%d %s %d\n",  arr[i].Roll_no,  arr[i].Name,  arr[i].Age);
    }
    fclose(ptr);
    return 0;
}

int read_struct_text( char *fileName )
{
    FILE *ptr=fopen(fileName,  "r");
    if((ptr)==NULL)
    {
        return 1;
    }
    struct student x;
    for(int i=0;i<5;i++)
    {
        fscanf(ptr,  "%d", &x.Roll_no);
        fscanf(ptr, "%s", x.Name);
        fscanf(ptr,  "%d", &x.Age);
        printf("%d %s %d\n",   x.Roll_no,   x.Name,   x.Age);
    }
    fclose(ptr);
    return 0;
}

int save_num_binary( char *fileName )
{
    FILE *ptr=fopen(fileName,  "wb");
    if((ptr)==NULL)
    {
        return 1;
    }   
    int *arr=(int *)(malloc(20*sizeof(int)));
    for(int i=0;i<20;i++)
    {
        arr[i]=i+1;
        fwrite(&arr[i],  sizeof(int),  1,  ptr);
    }
    fclose(ptr);
    return 0;
}

int read_num_binary( char *fileName )
{
    FILE *ptr=fopen(fileName,  "rb");
    if((ptr)==NULL)
    {
        return 1;
    }   
    int *x=(int *)(malloc(sizeof(int)));
    for(int i=0;i<20;i++)
    {
        fread(x,  sizeof(int),  1,  ptr);
        printf("%d ",  (*x));
    }
    fclose(ptr);
    return 0;
}

int save_struct_binary( char *fileName )
{
    FILE *ptr=fopen(fileName,  "wb");
    if((ptr)==NULL)
    {
        return 1;
    }  
    struct student arr[5]={{1,  "sawlani",  10},  {2,  "panda",  20},  {3,  "gaurav",  30},  {4,  "miiya",  40},  {5,  "bongoli",  50}};
    for(int i=0;i<5;i++)
    {
        fwrite(&arr[i].Roll_no,  sizeof(int),  1,  ptr);
        fwrite(arr[i].Name,  sizeof(char [30]),  1,  ptr);
        fwrite(&arr[i].Age,  sizeof(int),  1,  ptr);
    }
    fclose(ptr);
    return 0;
}

int read_struct_binary( char *fileName )
{
    FILE *ptr=fopen(fileName,   "rb");
    if((ptr)==NULL)
    {
        return 1;
    }  
    struct student stu;
    for(int i=0;i<5;i++)
    {
        fread(&stu.Roll_no,   sizeof(int),   1,   ptr);
        fread(stu.Name,   sizeof(char [30]),   1,   ptr);
        fread(&stu.Age,   sizeof(int),   1,   ptr);
        printf("%d %s %d\n",   stu.Roll_no,   stu.Name,   stu.Age);
    }
    fclose(ptr);
    return 0;
}