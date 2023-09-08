/*
 * Copyright 2023 Dementiy Shipilov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <conio.h>
#include <iostream>

#include "structures.h"

#define DATABASE_SIZE 4000
#define AMMOUNT_OF_PAGES ((2 * DATABASE_SIZE / PAGE_SIZE + 1) / 2) // Работает только с положительными числами
#define PAGE_SIZE 20

void get_db(Database *record)
{
    FILE *input = NULL;

    if (fopen_s(&input, "testBase2.dat", "rb"))
    {
        std::cout << "File is not available!\n";

        exit(1);
    }

    fread((Database *)record, sizeof(Database), DATABASE_SIZE, input);
    fclose(input);
}

void filling_db(Database *record, Processed_dates *date)
{
    std::string temp;

    for (int i = 0; i < DATABASE_SIZE; ++i)
    {
        temp.clear();

        for (int j = 0; j < 2; ++j)
        {
            temp += record[i].date_of_birth[j];
        }
        date[i].day = std::atoi(temp.c_str());

        temp.clear();

        for (int j = 3; j < 5; ++j)
        {
            temp += record[i].date_of_birth[j];
        }
        date[i].month = std::atoi(temp.c_str());

        temp.clear();

        for (int j = 6; j < 8; ++j)
        {
            temp += record[i].date_of_birth[j];
        }
        date[i].year = std::atoi(temp.c_str());
    }
}

void print_table(Database *record, Processed_dates *date, int page)
{
    printf("Page %i/%i\n---------------------------------------------------------------------------------\n", page + 1,
           AMMOUNT_OF_PAGES);

    for (int i = 0; i < PAGE_SIZE; ++i)
    {
        int j = i + page * PAGE_SIZE;
        {
            printf("% 5i.\t", j + 1);
        }

        printf("% 30s\t%i\t% 22s\t%02i-%02i-%02i\n", record[j].full_name, record[j].department, record[j].post,
               date[j].day, date[j].month, date[j].year);
    }

    printf("---------------------------------------------------------------------------------\nUse arrow keys to "
           "change page or enter [ESC] for exit.\n");
}

void program_interaction(Database *record, Processed_dates *date)
{
    bool exit_flag = false;
    int page = 0;

    while (exit_flag == 0)
    {
        system("cls");
        print_table(record, date, page);

        switch (int input = _getch())
        {
        case 'd':
        case 77: // [>]
            page += 1;
            if (page > AMMOUNT_OF_PAGES - 1)
            {
                page -= 1;
            }

            break;

        case 'a':
        case 75: // [<]
            page -= 1;
            if (page < 0)
            {
                page += 1;
            }

            break;

        case '0':
        case 27: // [ESC]
            exit_flag = true;

            break;
        }
    }
}

// void digital_sort(Node *&head, int queueLength)
//{
//     int KDI[4] = {3, 2, 1, 0};
//     int j, i, d, k;
//
//     Node *head;
//     tle Q[256];
//     for (j = queueLength - 1; j >= 0; j--)
//     { // инициализация очередей Q
//         for (i = 0; i < 256; i++)
//             Q[i].tail = (spis *)&Q[i].head;
//         p = S;
//         k = KDI[j];
//         while (p)
//         { // расстановка элементов из списка S в очереди Q по j - ой цифре
//             d = p->Digit[k];
//             Q[d].tail->next = p;
//             Q[d].tail = p;
//             p = p->next;
//         }
//         p = (spis *)&S; // соединение очередей Q в список S
//         for (i = 0; i < 256; i++)
//         {
//             if (Q[i].tail != (spis *)&Q[i].head)
//             {
//                 p->next = Q[i].head;
//                 p = Q[i].tail;
//             }
//         }
//         p->next = NULL;
//     }
// }

void print_node(Node *&head)
{
    // Цикл вывода всех значений односвязного списка
    Node *output_cycle = head;
    while (output_cycle != NULL)
    {
        for (int i = 0; i < DATABASE_SIZE; ++i)
        {

            std::cout << i + 1 << "."
                      << "\t" << output_cycle->record.full_name << "\t" << output_cycle->record.department << "\t"
                      << output_cycle->record.post << "\t" << output_cycle->record.date_of_birth << std::endl;
            output_cycle = output_cycle->next;
        }
    }
}

void node_initialization(Node *&head)
{
    // Инициализация указателей
    Node *current; // текущий
    Node *tail;    // конец

    head = new Node;
    current = head;
    tail = current;

    strcpy_s(current->record.full_name, record[0].full_name);
    current->record.department = record[0].department;
    strcpy_s(current->record.post, record[0].post);
    strcpy_s(current->record.date_of_birth, record[0].date_of_birth);

    // Цикл добавления элементов в односвязный список
    for (int i = 1; i < DATABASE_SIZE; ++i)
    {
        tail = new Node;
        current->next = tail;
        current = tail;

        strcpy_s(current->record.full_name, record[i].full_name);
        current->record.department = record[i].department;
        strcpy_s(current->record.post, record[i].post);
        strcpy_s(current->record.date_of_birth, record[i].date_of_birth);
    }

    // Объявление конца односвязного списка
    tail->next = NULL;

    //print_node(head);
}

int main()
{
    get_db(record);

    filling_db(record, date);

    program_interaction(record, date);

    Node *head;
    node_initialization(head);

    //digital_sort(head);

    return 0;
}