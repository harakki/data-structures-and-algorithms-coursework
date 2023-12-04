#include <Windows.h>
#include <conio.h>
#include <iostream>

#define DATABASE_SIZE 4000
#define PAGE_SIZE 20
#define AMMOUNT_OF_PAGES ((2 * DATABASE_SIZE / PAGE_SIZE + 1) / 2)

#define TREE_SIZE 25

#define MAX_SYMBOLS 256

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

struct Tree
{
    int data = 0;
    int weight = 0;

    Tree *left = nullptr;
    Tree *right = nullptr;

    List *next = nullptr;
};

struct Symbol
{
    double probability;            // P
    double cumulative_probability; // Q
    int code_length;               // L
    int code[MAX_SYMBOLS];         // код. слово
    char symbol;                   // a
};

int V[TREE_SIZE];
int W[TREE_SIZE];

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

    cout << "-Nmb----Full name-----------------------Dep.----Post--------------------DtofBrth-\n";

    while (ptr != nullptr)
    {
        cout << " " << record_number << "\t" << ptr->record.full_name << "\t" << ptr->record.department << "\t"
             << ptr->record.post << "\t" << ptr->record.date_of_birth << "\t" << endl;
        ptr = ptr->next;
        ++record_number;
    }

    cout << "---------------------------------------------------------------------------------\n";

    system("pause");
}

void printPartOfList(List *&head, int range_of_records[])
{
    List *ptr = head;

    if (ptr == nullptr)
    {
        cout << "Empty list.";

        return;
    }

    int record_number = range_of_records[0] + 1;
    int i = 0;

    for (i; i < range_of_records[0]; ++i)
    {
        ptr = ptr->next;
    }

    cout << "-Nmb----Full name-----------------------Dep.----Post--------------------DtofBrth-\n";

    for (i; i <= range_of_records[1]; ++i)
    {
        cout << " " << record_number << "\t" << ptr->record.full_name << "\t" << ptr->record.department << "\t"
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

void digitalSortForTree(List *&head)
{
    Queue *queue = new Queue[DATABASE_SIZE];

    for (int i = 0; i < DATABASE_SIZE; ++i)
    {
        queue[i].tail = (List *)&queue[i].head;
    }

    List *ptr = head;

    while (ptr != NULL)
    {
        short int digit = ptr->record.department;
        queue[digit].tail->next = ptr;
        queue[digit].tail = ptr;

        ptr = ptr->next;
    }

    ptr = (List *)&head;

    for (int i = 0; i < DATABASE_SIZE; ++i)
    {
        if (queue[i].tail != (List *)&queue[i].head)
        {
            ptr->next = queue[i].head;
            ptr = queue[i].tail;
        }
    }
    ptr->next = nullptr;

    delete[] queue;
}

void countWeightsForTree(List *&head)
{
    List *current = head;
    int size = 0;

    short int current_department = current->record.department;
    int current_weight = 0;

    while (current != nullptr)
    {
        if (current->record.department == current_department)
        {
            current_weight++;

            current = current->next;
        }
        else
        {
            V[size] = (current_department);
            W[size] = current_weight;
            size++;

            current_department = current->record.department;
            current_weight = 0;
        }
    }

    V[size] = (current_department);
    W[size] = current_weight;
    size++;
}

List *createListInOptimalSerchTree(List *&list, int size)
{
    List *head = nullptr;
    List *tail = nullptr;

    for (int i = 0; i < size; ++i)
    {
        List *temp = new List;

        strcpy_s(temp->record.full_name, list->record.full_name);
        temp->record.department = list->record.department;
        strcpy_s(temp->record.post, list->record.post);
        strcpy_s(temp->record.date_of_birth, list->record.date_of_birth);

        temp->next = nullptr;
        list = list->next;

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

    return head;
}

void addToOptimalSearchTree(Tree *&ptr, int Data, int Weight, List *list)
{
    if (ptr == nullptr)
    {
        ptr = new Tree;
        ptr->data = Data;
        ptr->weight = Weight;
        ptr->next = list;
    }
    else if (Data <= (ptr->data))
    {
        addToOptimalSearchTree(ptr->left, Data, Weight, list);
    }
    else if (Data > (ptr->data))
    {
        addToOptimalSearchTree(ptr->right, Data, Weight, list);
    }
}

void createOptimalSearchTreeA1(Tree *&root, List *&head)
{
    List *current = head;

    for (int i = 0; i < 25; i++)
    {
        int size = W[i];

        List *list = createListInOptimalSerchTree(current, size);

        addToOptimalSearchTree(root, V[i], W[i], list);
    }
}

void deleteTree(Tree *root)
{
    if (root == NULL)
    {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);

    // delete root;
    free(root);
}

List *treeSearch(Tree *&root, int target)
{
    if (root == nullptr)
    {
        return NULL; // В дереве нет узла с заданным значением
    }

    if (target == root->data)
    {
        return root->next; // Значение найдено
    }
    else if (target < root->data)
    {
        return treeSearch(root->left, target);
    }
    else
    {
        return treeSearch(root->right, target);
    }
}

void treeInteraction()
{
    List *head = fillOutList();

    digitalSortForTree(head);

    countWeightsForTree(head);

    Tree *root = nullptr;

    createOptimalSearchTreeA1(root, head);

    printf("Search in the optimal search tree\n\n");

    int search = 0;

    system("cls");
    cout << "Enter number of the department to display a list of employees: ";
    std::cin >> search;

    List *found = treeSearch(root, search);

    if (found == nullptr)
    {
        printf("\nThis value was not found!\n\n");

        deleteList(found);
        deleteList(head);
        deleteTree(root);

        system("pause");
        return;
    }
    else
    {
        printList(found);

        deleteList(found);
        deleteList(head);
        deleteTree(root);

        return;
    }
}

void insertionSortForCoding(Symbol *symbols, int alphabet_size)
{
    int i, j;
    Symbol key;

    for (i = 1; i < alphabet_size; i++)
    {
        key = symbols[i];
        j = i - 1;

        // Перемещение элементов symbols[0..i-1], которые больше ключа,
        // на одну позицию вперед от их текущей позиции
        while (j >= 0 && symbols[j].probability < key.probability)
        {
            symbols[j + 1] = symbols[j];
            j = j - 1;
        }

        symbols[j + 1] = key;
    }
}

void gilbertMooreCoding(Symbol *symbols, int alphabet_size)
{
    double pr = 0;

    for (int i = 0; i < alphabet_size; ++i)
    {
        symbols[i].cumulative_probability = pr + symbols[i].probability / 2;

        pr += symbols[i].probability;

        symbols[i].code_length = -log2(symbols[i].probability) + 1;

        for (int j = 0; j < symbols[i].code_length; ++j)
        {
            symbols[i].cumulative_probability *= 2;
            symbols[i].code[j] = (int)symbols[i].cumulative_probability;

            if (symbols[i].cumulative_probability > 1)
            {
                symbols[i].cumulative_probability -= 1;
            }
        }
    }
}

void printSymbolCodes(Symbol *symbols, int alphabet_size)
{
    printf("-A------P---------------L-------Codeword------\n");

    for (int i = 0; i < alphabet_size; ++i)
    {
        printf(" [%c]\t", symbols[i].symbol);
        printf("%f\t", symbols[i].probability);
        printf("%i\t", symbols[i].code_length);

        for (int j = 0; j < symbols[i].code_length; ++j)
        {
            printf("%d", symbols[i].code[j]);
        }

        printf("\n");
    }
    printf("---------------------------------------------\n");
}

void calculateEntropyAndAverageCodeLength(Symbol *symbols, int alphabet_size)
{
    double entropy = 0;
    double average_code_length = 0;

    for (int i = 0; i < alphabet_size; ++i)
    {
        entropy -= symbols[i].probability * log2(symbols[i].probability);
        average_code_length += symbols[i].code_length * symbols[i].probability;
    }

    printf("Entropy: %f\n", entropy);
    printf("Average code length: %f\n\n", average_code_length);
}

void codingInteraction()
{
    FILE *file = NULL;

    fopen_s(&file, "testBase2.dat", "rb");

    system("cls");

    if (file == NULL)
    {
        printf("Cannot open the file!\n");
        system("pause");
        return;
    }

    int frequencies[MAX_SYMBOLS] = {0};
    int total = 0;
    char ch;

    while (fread(&ch, sizeof(char), 1, file))
    {
        frequencies[(unsigned char)ch]++;
        total++;
    }

    fclose(file);

    Symbol symbols[MAX_SYMBOLS];
    int alphabet_size = 0;

    for (int i = 0; i < MAX_SYMBOLS; ++i)
    {
        if (frequencies[i] > 0)
        {
            symbols[alphabet_size].symbol = (char)i;
            symbols[alphabet_size].probability = (double)frequencies[i] / total;
            alphabet_size++;
        }
    }

    insertionSortForCoding(symbols, alphabet_size);
    gilbertMooreCoding(symbols, alphabet_size);
    printSymbolCodes(symbols, alphabet_size);
    calculateEntropyAndAverageCodeLength(symbols, alphabet_size);

    system("pause");
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
        printf("Use arrow keys to change page, [F] to search for part of records, [Z] to search \nwith tree, [E] to "
               "display all records, [C] to show symbol coding or enter [ESC] \nfor exit.\n");

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

        case 'z':
            treeInteraction();

            break;

        case 'c':
            codingInteraction();

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
    SetConsoleCP(866);
    SetConsoleOutputCP(866);

    List *list = fillOutList();

    digitalSort(list);

    programInteraction(list);

    deleteList(list);

    return 0;
}
