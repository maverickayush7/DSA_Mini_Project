#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// AVL Node
struct AVLNode {
    char Name[50];
    char CNIC[15];
    char Gender;
    char Type[20];
    int Acc_Number;
    int PIN;
    int balance;
    struct AVLNode* right;
    struct AVLNode* left;
    int height;
};

struct AVLNode* avl_root = NULL;


// AVL Functions
int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(struct AVLNode* node) {
    if (node == NULL)
        return 0;
    int lh = height(node->left);
    int rh = height(node->right);
    return (lh > rh) ? lh + 1 : rh + 1;
}

struct AVLNode* left_rotation(struct AVLNode* node) {
    struct AVLNode* temp1 = node->right;
    node->right = temp1->left;
    temp1->left = node;
    node->height = max(height(node->left), height(node->right)) + 1;
    temp1->height = max(height(temp1->left), height(temp1->right)) + 1;
    return temp1;
}

struct AVLNode* right_rotation(struct AVLNode* node) {
    struct AVLNode* temp1 = node->left;
    node->left = temp1->right;
    temp1->right = node;
    node->height = max(height(node->left), height(node->right)) + 1;
    temp1->height = max(height(temp1->left), height(temp1->right)) + 1;
    return temp1;
}

struct AVLNode* right_left_rotation(struct AVLNode* node) {
    node->right = right_rotation(node->right);
    return left_rotation(node);
}

struct AVLNode* left_right_rotation(struct AVLNode* node) {
    node->left = left_rotation(node->left);
    return right_rotation(node);
}

struct AVLNode* insert_Acc(struct AVLNode* node, char* name, char* cnic, char gender, char* type, int account_no, int PIN, int balance) {
    if (node == NULL) {
        node = (struct AVLNode*)malloc(sizeof(struct AVLNode));
        if (node == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }
        strcpy(node->Name, name);
        strcpy(node->CNIC, cnic);
        node->Gender = gender;
        strcpy(node->Type, type);
        node->Acc_Number = account_no;
        node->PIN = PIN;
        node->balance = balance;
        node->left = NULL;
        node->right = NULL;
        node->height = 0;
    } else if (account_no < node->Acc_Number) {
        node->left = insert_Acc(node->left, name, cnic, gender, type, account_no, PIN, balance);
        if (height(node->left) - height(node->right) == 2) {
            if (account_no < node->left->Acc_Number) {
                node = right_rotation(node);
            } else {
                node = left_right_rotation(node);
            }
        }
    } else if (account_no > node->Acc_Number) {
        node->right = insert_Acc(node->right, name, cnic, gender, type, account_no, PIN, balance);
        if (height(node->right) - height(node->left) == 2) {
            if (account_no > node->right->Acc_Number) {
                node = left_rotation(node);
            } else {
                node = right_left_rotation(node);
            }
        }
    }
    node->height = max(height(node->left), height(node->right)) + 1;
    return node;
}


struct AVLNode* delete_Acc(struct AVLNode* root, int account_no) {
    if (root == NULL) {
        return root;
    }

    if (account_no < root->Acc_Number) {
        root->left = delete_Acc(root->left, account_no);
    } else if (account_no > root->Acc_Number) {
        root->right = delete_Acc(root->right, account_no);
    } else {
        // Node with only one child or no child
        if (root->left == NULL) {
            struct AVLNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct AVLNode* temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children: Get the inorder successor (smallest
        // in the right subtree)
        struct AVLNode* temp = root->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }

        // Copy the inorder successor's data to this node
        strcpy(root->Name, temp->Name);
        strcpy(root->CNIC, temp->CNIC);
        root->Gender = temp->Gender;
        strcpy(root->Type, temp->Type);
        root->Acc_Number = temp->Acc_Number;
        root->PIN = temp->PIN;
        root->balance = temp->balance;

        // Delete the inorder successor
        root->right = delete_Acc(root->right, temp->Acc_Number);
    }

    // Update height of the current node
    root->height = max(height(root->left), height(root->right)) + 1;

    // Perform AVL balancing
    int balance = height(root->left) - height(root->right);

    // Left Heavy
    if (balance > 1) {
        if (height(root->left->left) >= height(root->left->right)) {
            return right_rotation(root);
        } else {
            return left_right_rotation(root);
        }
    }

    // Right Heavy
    if (balance < -1) {
        if (height(root->right->right) >= height(root->right->left)) {
            return left_rotation(root);
        } else {
            return right_left_rotation(root);
        }
    }

    return root;
}

// Function to find the maximum account number
int findMax(struct AVLNode* root) {
    if (root == NULL)
        return 0;
    int res = root->Acc_Number;
    int lres = findMax(root->left);
    int rres = findMax(root->right);
    if (lres > res)
        res = lres;
    if (rres > res)
        res = rres;
    return res;
}

int search_Acc(struct AVLNode* node, int account_no) {
    while (node != NULL) {
        if (account_no > node->Acc_Number) {
            node = node->right;
        } else if (account_no < node->Acc_Number) {
            node = node->left;
        } else {
            return 1;
        }
    }
    return 0;
}

int print_data(struct AVLNode* node, int account_no) {
    while (node != NULL) {
        if (account_no > node->Acc_Number) {
            node = node->right;
        } else if (account_no < node->Acc_Number) {
            node = node->left;
        } else {
            printf("        ####################################################################################################################################\n");
            printf(" \n");
            printf("          ACC_Number                 NAME                    CNIC                      GENDER            Type          Balance\n");
            printf(" \n");
            printf("        ####################################################################################################################################\n");
            printf(" \n");
            printf("             %d            %s           %s                %c                %s             %d\n",
                   node->Acc_Number, node->Name, node->CNIC, node->Gender, node->Type, node->balance);
            return 1;
        }
    }
    return 0;
}

void pre_order(struct AVLNode* node) {
    if (node->left != NULL) {
        pre_order(node->left);
    }
    printf("             %d            %s           %s                %c                %s             %d\n",
           node->Acc_Number, node->Name, node->CNIC, node->Gender, node->Type, node->balance);
    if (node->right != NULL) {
        pre_order(node->right);
    }
}

struct AVLNode* deposit(struct AVLNode* node, int account_no, int PIN, int balance) {
    if (node == NULL) {
        printf("Account not found. Deposit failed.\n");
        return NULL; // Account not found
    }

    if (account_no < node->Acc_Number) {
        node->left = deposit(node->left, account_no, PIN, balance);
    } else if (account_no > node->Acc_Number) {
        node->right = deposit(node->right, account_no, PIN, balance);
    } else {
        // Account found
        if (PIN == node->PIN) {
            node->balance += balance;
            time_t t;
            time(&t);
            char entry[100];
            snprintf(entry, sizeof(entry), "Deposited: %d on %s", balance, ctime(&t));
            printf("Deposit successful. New balance: %d\n", node->balance);
            return NULL; // Deposit successful
        } else {
            printf("Incorrect PIN. Deposit failed.\n");
            return node;
        }
    }

    // Update height of the current node
    node->height = max(height(node->left), height(node->right)) + 1;

    // Perform AVL balancing
    int balanceFactor = height(node->left) - height(node->right);

    // Left Heavy
    if (balanceFactor > 1) {
        if (account_no < node->left->Acc_Number) {
            return right_rotation(node);
        } else {
            return left_right_rotation(node);
        }
    }

    // Right Heavy
    if (balanceFactor < -1) {
        if (account_no > node->right->Acc_Number) {
            return left_rotation(node);
        } else {
            return right_left_rotation(node);
        }
    }

    return node;
}

struct AVLNode* withdraw(struct AVLNode* node, int account_no, int PIN, int balance) {
    if (node == NULL) {
        printf("Account not found. Withdrawal failed.\n");
        return NULL; // Account not found
    }

    if (account_no < node->Acc_Number) {
        node->left = withdraw(node->left, account_no, PIN, balance);
    } else if (account_no > node->Acc_Number) {
        node->right = withdraw(node->right, account_no, PIN, balance);
    } else {
        // Account found
        if (PIN == node->PIN) {
            if (node->balance < balance) {
                printf("Insufficient balance. Withdrawal failed.\n");
                return node; // Insufficient balance
            } else {
                node->balance -= balance;
                time_t t;
                time(&t);
                char entry[100];
                snprintf(entry, sizeof(entry), "Withdrawn: %d on %s", balance, ctime(&t));
                printf("Withdrawal successful. New balance: %d\n", node->balance);
                return NULL; // Withdrawal successful
            }
        } else {
            printf("Incorrect PIN. Withdrawal failed.\n");
            return node;
        }
    }

    // Update height of the current node
    node->height = max(height(node->left), height(node->right)) + 1;

    int balanceFactor = height(node->left) - height(node->right);

    if (balanceFactor > 1) {
        if (account_no < node->left->Acc_Number) {
            return right_rotation(node);
        } else {
            return left_right_rotation(node);
        }
    }

    if (balanceFactor < -1) {
        if (account_no > node->right->Acc_Number) {
            return left_rotation(node);
        } else {
            return right_left_rotation(node);
        }
    }

    return node;
}



void displayTitle() {
    printf("                   ____              _      __  __                                                   _      _____           _                  \n");
    printf("                  |  _ \\            | |    |  \\/  |                                                 | |    / ____|         | |                 \n");
    printf("                  | |_) | __ _ _ __ | | __ | \\  / | __ _ _ __   __ _  __ _  ___ _ __ ___   ___ _ __ | |_  | (___  _   _ ___| |_ ___ _ __ ___   \n");
    printf("                  |  _ < / _` | '_ \\| |/ / | |\\/| |/ _` | '_ \\ / _` |/ _` |/ _ \\ '_ ` _ \\ / _ \\ '_ \\| __|  \\___ \\ | | / __| __/ _ \\ '_ ` _  \\    \n");
    printf("                  | |_) | (_| | | | |   <  | |  | | (_| | | | | (_| | (_| |  __/ | | | | |  __/ | | | |_   ____) | |_| \\__ \\ ||  __/ | | | | |\n");
    printf("                  |____/ \\__,_|_| |_|_|\\_\\ |_|  |_|\\__,_|_| |_|\\__,_|\\__, |\\___|_| |_| |_|\\___|_| |_|\\__| |_____/ \\__, |___/\\__\\___|_| |_| |_| \n");
    printf("                                                                      __/ |                                        __/ |                       \n");
    printf("                                                                     |___/                                        |___/                        \n");
    printf("                                                                                                                                             \n");
}

int main() {
    int option, loop = 1;
    while (loop) {
        displayTitle();
        printf(" \n");
        printf("\t \t \t \t \t \t \t \t1- CREATE NEW ACCOUNT\n");
        printf("\t \t \t \t \t \t \t \t2- DEPOSIT AMOUNT\n");
        printf("\t \t \t \t \t \t \t \t3- WITHDRAW AMOUNT\n");
        printf("\t \t \t \t \t \t \t \t4- SEARCH BY ACCOUNT NUMBER\n");
        printf("\t \t \t \t \t \t \t \t5- PRINT ALL ACCOUNTS\n");
        printf("\t \t \t \t \t \t \t \t6- DELETE ACCOUNT\n");
        printf("\t \t \t \t \t \t \t \t7- EXIT\n");
        printf(" \n");
        printf("\t \t \t \t \t \t \t Enter your option: ");
        scanf("%d", &option);
        printf(" \n");

        switch (option) {
            case 1: {
                char name[50], cnic[15], type[20];
                int account_no, PIN;
                float balance;
                char gender;

                printf("Enter the following details for account creation:\n");
                printf("Name: ");
                scanf(" %[^\n]s", name);
                printf("CNIC: ");
                scanf(" %s", cnic);
                printf("Gender (M/F/Others): ");
                scanf(" %c", &gender);
                printf("Account Type(Saving/Current): ");
                scanf(" %[^\n]s", type);
                printf("Initial Deposit: ");
                scanf("%f", &balance);

                account_no = findMax(avl_root) + 1;  // Assign a new account number

                printf("Enter a 4-digit PIN for the account: ");
                scanf("%d", &PIN);

                avl_root = insert_Acc(avl_root, name, cnic, gender, type, account_no, PIN, (int)balance);

                printf("Account created successfully. Account Number: %d\n", account_no);

                // Create a new log entry for account creation
                time_t t;
                time(&t);
                char entry[100];
                snprintf(entry, sizeof(entry), "Account Created with an initial deposit of %.2f on %s", balance, ctime(&t));

                break;
            }
            case 2: {
                int account_no, PIN, deposit_amount;
                printf("Enter Account Number: ");
                scanf("%d", &account_no);
                printf("Enter 4-digit PIN: ");
                scanf("%d", &PIN);
                printf("Enter the amount to deposit: ");
                scanf("%d", &deposit_amount);
                int result = deposit(avl_root, account_no, PIN, deposit_amount);
                if (result == 1)
                    printf("Deposit successful. New balance: %d\n", avl_root->balance);
                else if (result == -1)
                    printf("Incorrect PIN. Deposit failed.\n");
                break;
            }
            case 3: {
                int account_no, PIN, withdraw_amount;
                printf("Enter Account Number: ");
                scanf("%d", &account_no);
                printf("Enter 4-digit PIN: ");
                scanf("%d", &PIN);
                printf("Enter the amount to withdraw: ");
                scanf("%d", &withdraw_amount);
                int result = withdraw(avl_root, account_no, PIN, withdraw_amount);
                if (result == 1)
                    printf("Withdrawal successful. New balance: %d\n", avl_root->balance);
                else if (result == -1)
                    printf("Insufficient balance. Withdrawal failed.\n");
                else if (result == -2)
                    printf("Incorrect PIN. Withdrawal failed.\n");
                break;
            }
            case 4: {
                int account_no;
                printf("Enter Account Number: ");
                scanf("%d", &account_no);
                if (search_Acc(avl_root, account_no)) {
                    printf("Account found!\n");
                    print_data(avl_root, account_no);
                }
                else {
                    printf("Account not found.\n");
                }
                break;
            }
            case 5: {
                if (avl_root != NULL) {
                    printf(" \n");
                    printf("        ####################################################################################################################################\n");
                    printf(" \n");
                    printf("          ACC_Number       NAME          CNIC         GENDER            Type          Balance\n");
                    printf(" \n");
                    printf("        ####################################################################################################################################\n");
                    printf(" \n");
                    pre_order(avl_root);
                }
                else {
                    printf("No accounts found.\n");
                }
                break;
            }
            case 6: {
                int account_no, PIN;
                char cnic[15];
                printf("Enter Account Number to delete: ");
                scanf("%d", &account_no);
                printf("CNIC: ");
                scanf(" %s", cnic);
                printf("Enter 4-digit PIN: ");
                scanf("%d", &PIN);
                if(avl_root==NULL)
                {
                    printf("No records to delete");
                }
                else if (strcmp(cnic, avl_root->CNIC) == 0 && PIN == avl_root->PIN) {
                    avl_root = delete_Acc(avl_root, account_no);
                }
                break;
            }
            
            case 7: {
                loop = 0;
                break;
            }
            default: {
                printf("Invalid option. Please enter a valid option.\n");
                break;
            }
        }
    }

    return 0;
}

