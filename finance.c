#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "finance_data.txt"

typedef struct {
    char date[20];
    char type[10]; // income or expense
    char category[20];
    float amount;
} Transaction;

// Function declarations
int login();
void addTransaction();
void viewTransactions();
void monthlySummary();
void deleteTransaction();  // New function
void menu();

int main() {
    if (!login()) {
        printf("Login failed. Exiting.\n");
        return 0;
    }

    menu();
    return 0;
}

int login() {
    char username[20];
    printf("---- Welcome to Personal Finance Tracker ----\n");
    printf("Enter username to continue: ");
    scanf("%s", username);

    // Very simple check for demonstration
    if (strcmp(username, "user") == 0 || strcmp(username, "admin") == 0)
        return 1;
    else {
        printf("Unrecognized username.\n");
        return 0;
    }
}

void menu() {
    int choice;
    while (1) {
        printf("\n---- Main Menu ----\n");
        printf("1. Add Transaction\n");
        printf("2. View Transactions\n");
        printf("3. Monthly Summary\n");
        printf("4. Delete Transaction\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTransaction(); break;
            case 2: viewTransactions(); break;
            case 3: monthlySummary(); break;
            case 4: deleteTransaction(); break;
            case 5: exit(0);
            default: printf("Invalid choice. Try again.\n");
        }
    }
}

void addTransaction() {
    FILE *fp = fopen(FILENAME, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Transaction t;
    printf("Enter date (YYYY-MM-DD): ");
    scanf("%s", t.date);
    printf("Enter type (income/expense): ");
    scanf("%s", t.type);
    printf("Enter category (e.g., food, rent): ");
    scanf("%s", t.category);
    printf("Enter amount: ");
    scanf("%f", &t.amount);

    fprintf(fp, "%s %s %s %.2f\n", t.date, t.type, t.category, t.amount);
    fclose(fp);
    printf("Transaction added successfully.\n");
}

void viewTransactions() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("No transactions to display.\n");
        return;
    }

    Transaction t;
    printf("\n---- Transaction History ----\n");
    printf("Date       | Type     | Category       | Amount\n");
    printf("-----------------------------------------------\n");
    while (fscanf(fp, "%s %s %s %f", t.date, t.type, t.category, &t.amount) != EOF) {
        printf("%-10s | %-8s | %-14s | Rs. %.2f\n", t.date, t.type, t.category, t.amount);
    }
    fclose(fp);
}

void monthlySummary() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("No data available.\n");
        return;
    }

    Transaction t;
    float totalIncome = 0, totalExpense = 0;

    while (fscanf(fp, "%s %s %s %f", t.date, t.type, t.category, &t.amount) != EOF) {
        if (strcmp(t.type, "income") == 0)
            totalIncome += t.amount;
        else if (strcmp(t.type, "expense") == 0)
            totalExpense += t.amount;
    }

    printf("\n---- Monthly Summary ----\n");
    printf("Total Income  : Rs. %.2f\n", totalIncome);
    printf("Total Expenses: Rs. %.2f\n", totalExpense);
    printf("Balance       : Rs. %.2f\n", totalIncome - totalExpense);

    fclose(fp);
}

// New function to delete a transaction
void deleteTransaction() {
    char targetDate[20], targetCategory[20];
    int found = 0;

    printf("Enter date of transaction to delete (YYYY-MM-DD): ");
    scanf("%s", targetDate);
    printf("Enter category of transaction to delete: ");
    scanf("%s", targetCategory);

    FILE *fp = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Transaction t;
    while (fscanf(fp, "%s %s %s %f", t.date, t.type, t.category, &t.amount) != EOF) {
        if (strcmp(t.date, targetDate) == 0 && strcmp(t.category, targetCategory) == 0 && !found) {
            found = 1; // Skip this transaction (delete)
        } else {
            fprintf(temp, "%s %s %s %.2f\n", t.date, t.type, t.category, t.amount);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("Transaction deleted successfully.\n");
    else
        printf("Transaction not found.\n");
}