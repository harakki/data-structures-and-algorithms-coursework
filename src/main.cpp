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
#define AMMOUNT_OF_PAGES ((2 * DATABASE_SIZE / PAGE_SIZE + 1) / 2)
#define PAGE_SIZE 20

void get_database(Database *record)
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

void interaction_with_program(Database *record, Processed_dates *date)
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

int main()
{
    get_database(record);

    filling_db(record, date);

    interaction_with_program(record, date);

    // Node* head;
    // head = new Node;
    //

    return 0;
}