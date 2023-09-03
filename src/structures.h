#pragma once

struct Database
{
    char full_name[30];
    short int department;
    char post[22];
    char date_of_birth[10];
};

struct Processed_dates
{
    int day;
    int month;
    int year;
};

struct Node
{
    struct Database record;
    struct Node *next;
}; 

struct Database record[4000];
struct Processed_dates date[4000];