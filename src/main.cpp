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

#include <conio.h> // для getch()
#include <iostream>

struct original_db
{
    char full_name[30];
    short int department;
    char post[22];
    char date_of_birth[10];
};

struct processed_db
{
    int day;
    int month;
    int year;
    struct original_db employee;
};

struct temporary
{
    char full_name[30];
    short int department;
    char post[22];
    int day;
    int month;
    int year;
};

original_db *original_record = new original_db[4000];
processed_db *processed_record = new processed_db[4000];

void get_data_from_file(original_db *original_record)
{
    FILE *db = NULL;
    if (fopen_s(&db, "db.dat", "rb"))
    {
        std::cout << "File is not available!\n";
        system("pause");
        exit(1);
    }
    fread((original_db *)original_record, sizeof(original_db), 4000, db);
    fclose(db);
}

void print_table(processed_db *processed_db, int page)
{
    std::cout << "Page " << page + 1 << "/200" << std::endl;
    std::cout << "--------------------------------------------------------------------------" << std::endl;
    for (int i = 0; i < 20; ++i)
    {
        int j = i + page * 20;
        std::cout << " " << processed_db[j].employee.full_name << "\t" << processed_db[j].employee.department << "\t"
                  << processed_db[j].employee.post << "\t";
        printf("%02i-%02i-%02i\n", processed_db[j].day, processed_db[j].month, processed_db[j].year);
    }
    std::cout << "--------------------------------------------------------------------------" << std::endl;
    std::cout << "Enter "
              << "[1] for next page "
              << "or [0] for exit" << std::endl;
}

void i_o(processed_db* processed_db)
{
    int page = 0;
    int exit_flag = 0;

    while (exit_flag == 0)
    {
        system("cls");
        print_table(processed_record, page);

        switch (int input = _getch())
        {
        case '0': // exit code
            exit_flag = 1;
            input = 0;
            break;

        case 27: // ESC
            exit_flag = 1;
            input = 0;
            break;

        case 75: // <-
            page -= 1;
            if (page < 0)
            {
                page += 1;
            }
            input = 0;
            break;

        case 77: // ->
            page += 1;
            if (page > 200 - 1)
            {
                page -= 1;
            }
            input = 0;
            break;
        }
    }
}

void filling_in_db(original_db *original_record, processed_db *processed_db)
{
    std::string temp_conv_char_to_numb;
    for (int i = 0; i < 4000; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            temp_conv_char_to_numb += original_record[i].date_of_birth[j];
        }
        processed_db[i].day = std::atoi(temp_conv_char_to_numb.c_str());
        temp_conv_char_to_numb.clear();
        for (int j = 3; j < 5; ++j)
        {
            temp_conv_char_to_numb += original_record[i].date_of_birth[j];
        }
        processed_db[i].month = std::atoi(temp_conv_char_to_numb.c_str());
        temp_conv_char_to_numb.clear();
        for (int j = 6; j < 8; ++j)
        {
            temp_conv_char_to_numb += original_record[i].date_of_birth[j];
        }
        processed_db[i].year = std::atoi(temp_conv_char_to_numb.c_str());
        temp_conv_char_to_numb.clear();
        processed_db[i].employee = original_record[i];
    }
}

int get_maximum_number(processed_db *processed_record)
{
    int maximum_number = processed_record[0].year;
    for (int i = 0; i < 4000; ++i)
    {
        if (processed_record[i].year > maximum_number)
        {
            maximum_number = processed_record[i].year;
        }
    }
    return maximum_number;
}

// void digital_sort(listDataBase *(&S))
//{
//     int KDI[32];
//     for (int i = 0; i < 30; i++)
//         KDI[i] = i;
//     KDI[30] = 31;
//     KDI[31] = 30;
//     int L = 32;
//
//     queue q[256];
//     listDataBase *p;
//     unsigned char d;
//     int k;
//
//     for (int j = L - 1; j >= 0; j--)
//     {
//         for (int i = 0; i <= 255; i++)
//         {
//             q[i].tail = (listDataBase *)&(q[i].head);
//         }
//         p = S;
//         k = KDI[j];
//         while (p != nullptr)
//         {
//             d = p->Digit[k];
//             q[d].tail->next = p;
//             q[d].tail = p;
//             p = p->next;
//         }
//
//         p = (listDataBase *)&S;
//
//         int i = 0;
//         int sign = 1;
//
//         while ((i > -1) && (i < 256))
//         {
//             if (q[i].tail != (listDataBase *)&(q[i].head))
//             {
//                 p->next = q[i].head;
//                 p = q[i].tail;
//             }
//             i += sign;
//         }
//         p->next = nullptr;
//     }
// }

int main()
{
    get_data_from_file(original_record);
    filling_in_db(original_record, processed_record);
    i_o(processed_record);

    for (int j = 0; j < 4000; ++j)
    {
        printf("%02i\n", processed_record[j].year);
        //std::cout << processed_record[j].year << std::endl;
    }

    system("pause");
    return 0;
}