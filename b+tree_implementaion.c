#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYS 7


struct Job {
    int jobId;
    char title[50];
    char company[50];
    float salary;
    struct Job *children[MAX_KEYS + 1];
    int num_keys;
};

struct Job* createJob(int jobId, const char *title, const char *company, float salary) {
    struct Job *newJob ;
    newJob=(struct Job*)malloc(sizeof(struct Job));

    if (newJob == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newJob->jobId = jobId;
    strcpy(newJob->title, title);
    strcpy(newJob->company, company);
    newJob->salary = salary;
    newJob->num_keys = 0;
    for (int i = 0; i < MAX_KEYS + 1; i++){
        newJob->children[i] = NULL;
    }
    return newJob;
}


void splitChild(struct Job *parent, int index, struct Job *child) {
    struct Job *newChild;
    newChild= createJob(0, "", "", 0);
    newChild->num_keys = MAX_KEYS / 2;


    for (int i = 0; i < MAX_KEYS / 2; i++) {
        newChild->children[i] = child->children[i + (MAX_KEYS + 1) / 2];
        newChild->jobId = child->children[i + (MAX_KEYS + 1) / 2]->jobId;
        strcpy(newChild->title, child->children[i + (MAX_KEYS + 1) / 2]->title);
        strcpy(newChild->company, child->children[i + (MAX_KEYS + 1) / 2]->company);
        newChild->salary = child->children[i + (MAX_KEYS + 1) / 2]->salary;
        newChild->num_keys++;
        child->num_keys--;
    }

    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newChild;
    parent->num_keys++;
}


void insertNonFull(struct Job *node, struct Job *job) {
    int i = node->num_keys - 1;
    if (node->children[0] == NULL) {
        while (i >= 0 && strcmp(job->title, node->children[i]->title) < 0) {
            node->children[i + 1] = node->children[i];
            i--;
        }
        node->children[i + 1] = job;
        node->num_keys++;
    } else {
        while (i >= 0 && strcmp(job->title, node->children[i]->title) < 0)
            i--;
        i++;
        if (node->children[i]->num_keys == MAX_KEYS) {
            splitChild(node, i, node->children[i]);
            if (strcmp(job->title, node->title) > 0)
                i++;
        }
        insertNonFull(node->children[i], job);
    }
}

struct Job* insertJob(struct Job *root,struct Job *job) {

    if (root == NULL) {
       // printf("%f",job->salary);
        return job;
    }
    if (root->num_keys == MAX_KEYS) {
        struct Job *newRoot = createJob(root->jobId, root->title, root->company, root->salary);
        if (newRoot == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        newRoot->children[0] = root;
        splitChild(newRoot, 0, root);
        int i = 0;
        if (strcmp(job->title, newRoot->children[0]->title) > 0)
            i++;
        insertNonFull(newRoot->children[i], job);
        return newRoot;
    } 
    else {
        insertNonFull(root, job);
        return root;
    }
}

void inorderTraversal(struct Job *root) {
    if (root != NULL) {
        int i;
        for (i = 0; i < root->num_keys; i++) {
            inorderTraversal(root->children[i]);
            printf("Job ID: %d, Title: %s, Company: %s, Salary: %.2f\n", root->children[i]->jobId, root->children[i]->title, root->children[i]->company, root->children[i]->salary);
            
        }
        inorderTraversal(root->children[i]);
    }
    // else if(root==NULL){
        
    //     printf("Parent: Job ID: %d, Title: %s, Company: %s, Salary: %.2f\n", root->jobId, root->title, root->company, root->salary);
    // }
}



void searchJob(struct Job *root, const char *title) {
    if (root == NULL) {
        printf("No jobs found with the given title.\n");
        return;
    }

    if (strcmp(title, root->title) == 0) {
        printf("Job found:\n");
        printf("Job ID: %d\n", root->jobId);
        printf("Title: %s\n", root->title);
        printf("Company: %s\n", root->company);
        printf("Salary: %.2f\n", root->salary);
        return;
    }

    int i = 0;
    while (i < root->num_keys && strcmp(title, root->children[i]->title) > 0)
        i++;

    if (i < root->num_keys && strcmp(title, root->children[i]->title) == 0) {
        printf("Job found:\n");
        printf("Job ID: %d\n", root->children[i]->jobId);
        printf("Title: %s\n", root->children[i]->title);
        printf("Company: %s\n", root->children[i]->company);
        printf("Salary: %.2f\n", root->children[i]->salary);
    } else if (root->children[0] == NULL) {
        printf("No jobs found with the given title.\n");
    } else {
        searchJob(root->children[i], title);
    }
}

int main() {
    struct Job *root = NULL;
    int choice;
    char searchTitle[50];

    do {
        printf("\n***** Job Searching Portal *****\n");
        printf("1. Input Job Details\n");
        printf("2. Inorder Traversal\n");
        printf("3. Search for a Job\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    int jobId;
                    char title[50];
                    char company[50];
                    float salary;

                    printf("Enter Job ID: ");
                    scanf("%d", &jobId);

                    printf("Enter Job Title: ");
                    scanf(" %[^\n]", title);

                    printf("Enter Company Name: ");
                    scanf(" %[^\n]", company);

                    printf("Enter Salary: ");
                    scanf("%f", &salary);

                    root = insertJob(root, createJob(jobId, title, company, salary));
                    printf("Job details added successfully.\n");
                    break;
                }
            case 2:
                printf("\nInorder Traversal:\n");
                inorderTraversal(root);
                break;
            case 3:
                printf("\nEnter the title of the job you want to search: ");
                scanf(" %[^\n]", searchTitle);
                searchJob(root, searchTitle);
                break;
            case 4:
                printf("Exiting program. \nThank you!!\n");
                break;
            default:
                printf("Invalid choice! Please enter a valid option.\n");
        }
    } while (choice != 4);

    return 0;
}
 