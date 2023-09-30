#include <Windows.h>
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
    List *ptr = head;

    if (ptr == nullptr)
    {
        cout << "Empty list.";
    };

    while (ptr != nullptr)
    {
        cout << ptr->record.full_name << "\t" << ptr->record.department << "\t" << ptr->record.post << "\t"
             << ptr->record.date_of_birth << "\t" << endl;
        ptr = ptr->next;
    }
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

void indexArrayInit(List *&head, Enterprise **index_arr)
{
    index_arr = new Enterprise *[DATABASE_SIZE] {nullptr};
    int i = 0;

    List *ptr = head;

    while (ptr != nullptr)
    {
        index_arr[i] = &ptr->record;
        ptr = ptr->next;
        ++i;
    }
}

//List *binarySearch(Enterprise **index_arr, int key)
//{
//    List *head = nullptr;
//    List *ptr = nullptr;
//
//    int left = 0;
//    int middle = 0;
//    int right = DATABASE_SIZE - 1;
//
//    while (left <= right)
//    {
//        middle = (left + right) / 2;
//    }
//}

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(866);

    List *list = fillOutList();

    digitalSort(list);

    printList(list);


    Enterprise **index_arr = nullptr;
    indexArrayInit(list, index_arr);
    //binarySearch(index_arr, 50);

    deleteList(list);

    return 0;
}