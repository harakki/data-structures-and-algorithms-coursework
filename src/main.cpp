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

#include <iostream>

struct company
{
    char full_name[30];
    short int department;
    char post[22];
    char date_of_birth[10];
};

struct full_date_of_birth
{
    int day;
    int month;
    int year;
};

void get_data_from_file(company *record)
{
    FILE *db = NULL;

    bool open_error = fopen_s(&db, "db.dat", "rb");
    if (open_error)
    {
        std::cout << "File is not available!\n";
        system("pause");
        exit(1);
    }
    fread((company *)record, sizeof(company), 4000, db);
    fclose(db);
}

void print_table(company *record)
{
    for (int page = 0; page < 200; ++page)
    {
        std::cout << "Page " << page + 1 << "/200" << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        for (int i = 0; i < 20; ++i)
        {
            int j = i + page * 20;
            std::cout << " " << record[j].full_name << "\t" << record[j].department << "\t" << record[j].post << "\t"
                      << record[j].date_of_birth << std::endl;
        }
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        std::cout << "Enter "
                  << "[0] for exit "
                  << "or [1] for next page " << std::endl;
        int input;
        std::cin >> input;
        if (input == 0)
        {
            break;
        }
        system("cls");
    }
}

// int get_maximum_number(company *record)
//{
//     int maximum_number = record[0].date_of_birth;
//     for (int i = 0; i < 4000; ++i)
//     {
//         if (record[i].date_of_birth > maximum_number)
//         {
//             maximum_number = record[i].department;
//         }
//     }
//     return maximum_number;
// }

// void count_sort(company *record, int exp)
//{
//     int count[10] = {0};
//     for (int i = 0; i < 4000; ++i)
//     {
//         count[(record[i].department / exp) % 10]++;
//     }
//     for (int i = 1; i < 10; ++i)
//     {
//         count[i] += count[i - 1];
//     }
//     for (int i = 4000 - 1; i >= 0; --i)
//     {
//         record[count[(record[i].department / exp) % 10] - 1] = record[i];
//         count[(record[i].department / exp) % 10]--;
//     }
//     // for (int i = 0; i < 4000; ++i)
//     //{
//     //     record[i] = record[i];
//     // }
// }

// void radix_sort(company *record)
//{
//     int record_of_maximum_number = get_maximum_number(record);
//     for (int exp = 1; record_of_maximum_number / exp > 0; exp *= 10)
//     {
//         count_sort(record, exp);
//     }
// }

int main()
{
    company record[4000];
    full_date_of_birth d_m_y[4000];

    get_data_from_file(record);
    print_table(record);
    std::string temp_conv_char_to_numb;
    for (int i = 0; i < 4000; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            temp_conv_char_to_numb += record[i].date_of_birth[j];
        }
        d_m_y[i].day = std::atoi(temp_conv_char_to_numb.c_str());
        temp_conv_char_to_numb.clear();
        for (int j = 3; j < 5; ++j)
        {
            temp_conv_char_to_numb += record[i].date_of_birth[j];
        }
        d_m_y[i].month = std::atoi(temp_conv_char_to_numb.c_str());
        temp_conv_char_to_numb.clear();
        for (int j = 6; j < 8; ++j)
        {
            temp_conv_char_to_numb += record[i].date_of_birth[j];
        }
        d_m_y[i].year = std::atoi(temp_conv_char_to_numb.c_str());
        temp_conv_char_to_numb.clear();
        //printf("%02i-%02i-%02i\n", d_m_y[i].day, d_m_y[i].month, d_m_y[i].year);
    }

     //radix_sort(record);
    return 0;
}