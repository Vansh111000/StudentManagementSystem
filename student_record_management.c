#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "student_records.txt"

struct Student {
    int rollNo;
    char name[50];
    float marks;
};

// Function Prototypes
void addStudent();
void viewStudents();
void updateStudent();
void deleteStudent();
void menu();

int main() {
    menu();
    return 0;
}

void menu() {
    int choice;
    while(1) {
        printf("\n--- Student Record Management System ---\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: exit(0); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}

void addStudent() {
    FILE *fp = fopen(FILENAME, "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    struct Student s;
    printf("Enter Roll Number: ");
    scanf("%d", &s.rollNo);
    printf("Enter Name: ");
    getchar();  // clear buffer
    fgets(s.name, 50, stdin);
    s.name[strcspn(s.name, "\n")] = 0;  // remove trailing newline
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(struct Student), 1, fp);
    fclose(fp);
    printf("Student record added successfully!\n");
}

void viewStudents() {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    struct Student s;
    printf("\n--- Student Records ---\n");
    while (fread(&s, sizeof(struct Student), 1, fp)) {
        printf("Roll Number: %d\n", s.rollNo);
        printf("Name: %s\n", s.name);
        printf("Marks: %.2f\n", s.marks);
        printf("---------------------\n");
    }
    fclose(fp);
}

void updateStudent() {
    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    int rollNo, found = 0;
    printf("Enter Roll Number of student to update: ");
    scanf("%d", &rollNo);

    struct Student s;
    while (fread(&s, sizeof(struct Student), 1, fp)) {
        if (s.rollNo == rollNo) {
            printf("Enter New Name: ");
            getchar();  // clear buffer
            fgets(s.name, 50, stdin);
            s.name[strcspn(s.name, "\n")] = 0;  // remove trailing newline
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);

            fseek(fp, -sizeof(struct Student), SEEK_CUR); // move pointer back
            fwrite(&s, sizeof(struct Student), 1, fp);
            found = 1;
            break;
        }
    }

    if (found) {
        printf("Student record updated successfully!\n");
    } else {
        printf("Student with Roll Number %d not found.\n", rollNo);
    }

    fclose(fp);
}

void deleteStudent() {
    FILE *fp = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.txt", "wb");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    int rollNo, found = 0;
    printf("Enter Roll Number of student to delete: ");
    scanf("%d", &rollNo);

    struct Student s;
    while (fread(&s, sizeof(struct Student), 1, fp)) {
        if (s.rollNo == rollNo) {
            found = 1;
        } else {
            fwrite(&s, sizeof(struct Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found) {
        printf("Student record deleted successfully!\n");
    } else {
        printf("Student with Roll Number %d not found.\n", rollNo);
    }
}
