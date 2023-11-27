#include <Windows.h>
#include <conio.h>
#include <iostream>

#define DATABASE_SIZE 4000
#define PAGE_SIZE 20
#define AMMOUNT_OF_PAGES ((2 * DATABASE_SIZE / PAGE_SIZE + 1) / 2)

using std::cout;
using std::endl;

struct Enterprise
{
    char full_name[30];
    short int department;
    char post[22];
    char date_of_birth[10];
};

struct List
{
    struct List *next;
    union {
        struct Enterprise record;
        size_t byte_number[sizeof(Enterprise::date_of_birth)];
    };
};

struct Queue
{
    List *head;
    List *tail;
};

Enterprise *getFileData()
{
    FILE *file = NULL;

    if (fopen_s(&file, "testBase2.dat", "rb") != NULL)
    {
        exit(ENOENT);
    }

    struct Enterprise *record = new Enterprise[DATABASE_SIZE];
    fread((Enterprise *)record, sizeof(Enterprise), DATABASE_SIZE, file);

    return record;
}

List *fillOutList()
{
    Enterprise *record = getFileData();

    List *head = nullptr;
    List *tail = nullptr;

    for (int i = 0; i < DATABASE_SIZE; ++i)
    {
        List *temp = new List;

        strcpy_s(temp->record.full_name, record[i].full_name);
        temp->record.department = record[i].department;
        strcpy_s(temp->record.post, record[i].post);
        strcpy_s(temp->record.date_of_birth, record[i].date_of_birth);

        temp->next = nullptr;

        if (head == nullptr)
        {
            head = temp;
            tail = temp;
        }
        else
        {
            tail->next = temp;
            tail = temp;
        }
    }

    delete[] record;
    return head;
}

void printList(List *&head)
{
    system("cls");

    List *ptr = head;

    if (ptr == nullptr)
    {
        cout << "Empty list.";
    };

    int record_number = 1;

    cout << "---------------------------------------------------------------------------------\n";

    while (ptr != nullptr)
    {
        cout << record_number << "\t" << ptr->record.full_name << "\t" << ptr->record.department << "\t"
             << ptr->record.post << "\t" << ptr->record.date_of_birth << "\t" << endl;
        ptr = ptr->next;
        ++record_number;
    }

    cout << "---------------------------------------------------------------------------------\n";

    system("pause");
}

bool printPartOfList(List *&head, int range_of_records[])
{
    List *ptr = head;

    if (ptr == nullptr)
    {
        cout << "Empty list.";

        return 1;
    }

    int record_number = range_of_records[0] + 1;
    int i = 0;

    for (i; i < range_of_records[0]; ++i)
    {
        ptr = ptr->next;
    }

    cout << "---------------------------------------------------------------------------------\n";

    for (i; i <= range_of_records[1]; ++i)
    {
        cout << record_number << "\t" << ptr->record.full_name << "\t" << ptr->record.department << "\t"
             << ptr->record.post << "\t" << ptr->record.date_of_birth << "\t" << endl;
        ptr = ptr->next;
        ++record_number;
    }

    printf("---------------------------------------------------------------------------------\n");
}

void deleteList(List *&head)
{
    List *ptr = head;

    while (ptr != nullptr)
    {
        List *clean = ptr;
        ptr = ptr->next;
        delete clean;
    }

    head = nullptr;
}

void digitalSort(List *&head)
{
    char digit_order[6] = {6, 7, 3, 4, 0, 1};

    Queue queue[256];

    for (int j = (sizeof(digit_order) / sizeof(digit_order[0])) - 1; j >= 0; --j)
    {
        for (short i = 0; i < 256; ++i)
        {
            queue[i].tail = (List *)&queue[i].head;
        }

        List *ptr = head;

        while (ptr != NULL)
        {
            char digit = ptr->record.date_of_birth[digit_order[j]];
            queue[digit].tail->next = ptr;
            queue[digit].tail = ptr;

            ptr = ptr->next;
        }

        ptr = (List *)&head;

        for (short i = 0; i < 256; ++i)
        {
            if (queue[i].tail != (List *)&queue[i].head)
            {
                ptr->next = queue[i].head;
                ptr = queue[i].tail;
            }
        }
        ptr->next = nullptr;
    }
}

void indexArrayInit(int index_array[], List *&head)
{
    List *ptr = head;

    std::string temp;

    for (int i = 0; i < DATABASE_SIZE; ++i)
    {
        temp.clear();

        for (int j = 6; j < 8; ++j)
        {
            temp += ptr->record.date_of_birth[j];
        }

        index_array[i] = std::atoi(temp.c_str());

        ptr = ptr->next;
    }
}

int binarySearch(int index_array[], int target)
{
    int left = 0;
    int right = DATABASE_SIZE - 1;

    while (left <= right)
    {
        int middle = (left + right) / 2;
        if (index_array[middle] == target)
        {
            return middle;
        }
        else if (index_array[middle] < target)
        {
            left = middle + 1;
        }
        else
        {
            right = middle - 1;
        }
    }

    return -1;
}

int indexFoundLeftmost(int index_array[], int found_index)
{
    int leftmost_index = found_index;

    while (index_array[leftmost_index - 1] == index_array[found_index])
    {
        --leftmost_index;
    }

    return leftmost_index;
}

int indexFoundRightmost(int index_array[], int found_index)
{
    int rightmost_index = found_index;

    while (index_array[rightmost_index + 1] == index_array[found_index])
    {
        ++rightmost_index;
    }

    return rightmost_index;
}

void searchList(List *&list)
{
    int index_array[DATABASE_SIZE];
    indexArrayInit(index_array, list);

    int search = 0;

    system("cls");
    cout << "Enter the year of birth to display a list of employees: ";
    std::cin >> search;

    int found_index = binarySearch(index_array, search);

    if (found_index != -1)
    {
        int range_of_records[2] = {};

        range_of_records[0] = indexFoundLeftmost(index_array, found_index);
        range_of_records[1] = indexFoundRightmost(index_array, found_index);

        printPartOfList(list, range_of_records);

        system("pause");
    }
}

void optimalSearchTree(List*& list)
{

}

void programInteraction(List *&head)
{
    bool exit_flag = false;
    int page = 0;

    while (exit_flag == 0)
    {
        system("cls");

        int range_of_records[2] = {page * PAGE_SIZE, page * PAGE_SIZE + 19};

        printf("Page %i/%i\n", range_of_records[0] / PAGE_SIZE + 1, AMMOUNT_OF_PAGES);
        printPartOfList(head, range_of_records);
        printf("Use arrow keys to change page, [F] to search for part of records, [E] to display \nall records or "
               "enter [ESC] for exit.\n");

        switch (_getch())
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

        case 'f':
            searchList(head);

            break;

        case 'e':
            printList(head);

            break;

        case '0':
        case 27: // [ESC]
            exit_flag = true;

            break;
        }
    }
}

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(866);

    List *list = fillOutList();

    digitalSort(list);

    programInteraction(list);

    deleteList(list);

    return 0;
}