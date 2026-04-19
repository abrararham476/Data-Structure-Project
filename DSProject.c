#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Volunteer
{
    int id;
    char name[50];
    char activity[50];
    struct Volunteer* next;
    struct Volunteer* left;
    struct Volunteer* right;
} Volunteer;


typedef struct Task
{
    int id;
    char description[100];
    struct Task* next;
} Task;


typedef struct Queue
{
    Task* front;
    Task* rear;
} Queue;


Volunteer* head = NULL;
Volunteer* bstRoot = NULL;
Queue taskQueue = {NULL, NULL};


Volunteer* insertBST(Volunteer* root, Volunteer* newNode);
void rebuildBST();
void addVolunteer(int id, const char* name, const char* activity);
void searchVolunteer(int id);
void removeVolunteer(int id);
void enqueueTask(int id, const char* desc);
void assignTask();
void saveToFile();
void loadFromFile();


Volunteer* insertBST(Volunteer* root, Volunteer* newNode)
{
    if (root == NULL) return newNode;
    if (newNode->id < root->id)
        root->left = insertBST(root->left, newNode);
    else if (newNode->id > root->id)
        root->right = insertBST(root->right, newNode);
    return root;
}

int exists(int id)
{
    Volunteer* temp = head;
    while (temp != NULL)
    {
        if (temp->id == id)return 1;
        temp = temp->next;
    }
    return 0;
}

void rebuildBST()
{
    bstRoot = NULL;
    Volunteer* curr = head;
    while (curr != NULL)
    {
        curr->left = curr->right = NULL;
        bstRoot = insertBST(bstRoot, curr);
        curr = curr->next;
    }
}


void addVolunteer(int id, const char* name, const char* activity)
{
    Volunteer* newVol = (Volunteer*)malloc(sizeof(Volunteer));
    newVol->id = id;
    strcpy(newVol->name, name);
    strcpy(newVol->activity, activity);
    newVol->next = NULL;
    newVol->left = NULL;
    newVol->right = NULL;
    if (exists(id))
    {
        printf("Volunteer with ID %d already exists!\n", id);
        return;
    }
    if (head == NULL)
    {
        head = newVol;
    }

    else
    {
        Volunteer* temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newVol;
    }
    bstRoot = insertBST(bstRoot, newVol);
    printf("Successfully added Volunteer ID %d.\n", id);
}


void searchVolunteer(int id)
{
    Volunteer* temp = bstRoot;
    while (temp != NULL)
    {
        if (id == temp->id)
        {
            printf("\n--- Volunteer Found ---\n");
            printf("ID: %d\nName: %s\nActivity: %s\n-----------------------\n",
                   temp->id, temp->name, temp->activity);
            return;
        }
        else if (id < temp->id)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    printf("Volunteer with ID %d not found.\n", id);
}


void removeVolunteer(int id)
{
    Volunteer *temp = head, *prev = NULL;

    if (temp != NULL && temp->id == id)
    {
        head = temp->next;
        free(temp);
        rebuildBST();
        printf("Volunteer ID %d removed.\n", id);
        return;
    }

    while (temp != NULL && temp->id != id)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Volunteer ID %d not found.\n", id);
        return;
    }

    prev->next = temp->next;
    free(temp);
    rebuildBST();
    printf("Volunteer ID %d removed.\n", id);
}


void enqueueTask(int id, const char* desc)
{
    Task* newTask = (Task*)malloc(sizeof(Task));
    newTask->id = id;
    strcpy(newTask->description, desc);
    newTask->next = NULL;

    if (taskQueue.rear == NULL)
    {
        taskQueue.front = taskQueue.rear = newTask;
    }
    else
    {
        taskQueue.rear->next = newTask;
        taskQueue.rear = newTask;
    }
    printf("Task '%s' added to the queue.\n", desc);
}


void assignTask()
{
    if (taskQueue.front == NULL)
    {
        printf("No tasks currently pending in the queue.\n");
        return;
    }
    Task* temp = taskQueue.front;
    taskQueue.front = taskQueue.front->next;

    if (taskQueue.front == NULL) taskQueue.rear = NULL;

    printf("Assigned Task ID %d: %s\n", temp->id, temp->description);
    free(temp);
}


void saveToFile()
{
    FILE *fp = fopen("volunteers_data.txt", "w");
    if (!fp)
    {
        printf("Error opening file for saving.\n");
        return;
    }
    Volunteer* temp = head;
    while (temp)
    {
        fprintf(fp, "%d,%s,%s\n", temp->id, temp->name, temp->activity);
        temp = temp->next;
    }
    fclose(fp);
    printf("Data saved successfully to 'volunteers_data.txt'.\n");
}


void loadFromFile()
{
    FILE *fp = fopen("volunteers_data.txt", "r");
    if (!fp)
    {
        printf("No existing data file found. Starting fresh.\n");
        return;
    }

    int id;
    char name[50], activity[50];


    head = NULL;
    bstRoot = NULL;

    while (fscanf(fp, "%d,%49[^,],%49[^\n]\n", &id, name, activity) == 3)
    {
        addVolunteer(id, name, activity);
    }
    fclose(fp);
    printf("Data loaded successfully.\n");
}


int main()
{
    int choice, id;
    char name[50], activity[50], taskDesc[100];

    while(1)
    {
        printf("                                               \n=== Volunteer Management System ===\n");



        printf("\n");
        printf("\n");
        printf("\n");
        printf("1. Load Data\n");
        printf("2. Save Data\n");
        printf("3. Add Volunteer & Activity\n");
        printf("4. Search Volunteer Activity\n");
        printf("5. Add a Pending Task\n");
        printf("6. Assign Task (FIFO)\n");
        printf("7. Remove a Specific Volunteer\n");
        printf("8. Exit Program\n");
        printf("\n");
        printf("\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice)
        {
        case 1:
            loadFromFile();
            break;
        case 2:
            saveToFile();
            break;
        case 3:
            printf("Enter Volunteer ID: ");
            scanf("%d", &id);
            getchar();
            printf("Enter Volunteer Name: ");
            fgets(name, 50, stdin);
            name[strcspn(name, "\n")] = 0; // remove newline
            printf("Enter Volunteer Activity/Event: ");
            fgets(activity, 50, stdin);
            activity[strcspn(activity, "\n")] = 0;
            addVolunteer(id, name, activity);
            break;
        case 4:
            printf("Enter Volunteer ID to search: ");
            scanf("%d", &id);
            searchVolunteer(id);
            break;
        case 5:
            printf("Enter Task ID: ");
            scanf("%d", &id);
            getchar();
            printf("Enter Task Description: ");
            fgets(taskDesc, 100, stdin);
            taskDesc[strcspn(taskDesc, "\n")] = 0;
            enqueueTask(id, taskDesc);
            break;
        case 6:
            assignTask();
            break;
        case 7:
            printf("Enter Volunteer ID to remove: ");
            scanf("%d", &id);
            removeVolunteer(id);
            break;
        case 8:
            printf("Exiting Program...\n");
            exit(0);
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}
