#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store student details
struct Student {
    int rollNo;
    char name[50];
    char dept[30];
    int year;
    float marks;
    char grade;
};

// Function to calculate grade from marks
char getGrade(float marks) {
    if (marks >= 90) return 'A';
    if (marks >= 75) return 'B';
    if (marks >= 60) return 'C';
    if (marks >= 40) return 'D';
    return 'F';
}

// Save all records to Binary file and Text file
void saveFiles(struct Student *list, int count) {
    // 1. Binary File (fwrite)
    FILE *binFile = fopen("students.dat", "wb");
    if (binFile != NULL) {
        fwrite(list, sizeof(struct Student), count, binFile);
        fclose(binFile);
    }

    // 2. Text File (fprintf)
    FILE *txtFile = fopen("students.txt", "w");
    if (txtFile != NULL) {
        for (int i = 0; i < count; i++) {
            fprintf(txtFile, "%d %s %s %d %.2f %c\n",
                    list[i].rollNo, list[i].name, list[i].dept,
                    list[i].year, list[i].marks, list[i].grade);
        }
        fclose(txtFile);
    }
}

// Load existing records from Binary file at startup (fread)
int loadFiles(struct Student **list, int *capacity) {
    FILE *binFile = fopen("students.dat", "rb");
    if (binFile == NULL) {
        return 0; // File doesn't exist yet
    }

    int count = 0;
    struct Student temp;
    while (fread(&temp, sizeof(struct Student), 1, binFile) == 1) {
        if (count == *capacity) {
            *capacity += 5;
            *list = realloc(*list, (*capacity) * sizeof(struct Student));
        }
        (*list)[count] = temp;
        count++;
    }
    fclose(binFile);
    return count;
}

int main() {
    int capacity = 5;
    int count = 0;
    int choice;

    // Dynamic memory allocation using malloc()
    struct Student *list = (struct Student *)malloc(capacity * sizeof(struct Student));
    if (list == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Load previously saved data
    count = loadFiles(&list, &capacity);

    while (1) {
        // Static variable to count menu operations
        static int totalActions = 0;

        printf("\n--- STUDENT RECORD SYSTEM ---\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search by Roll Number\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Count Total Students\n");
        printf("7. Filter by Marks\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        totalActions++;

        if (choice == 1) {
            // Resize memory using realloc() if full
            if (count == capacity) {
                capacity += 5;
                list = realloc(list, capacity * sizeof(struct Student));
            }

            printf("Enter Roll No: ");
            scanf("%d", &list[count].rollNo);
            printf("Enter Name (no spaces, e.g. Rahul_K): ");
            scanf("%s", list[count].name);
            printf("Enter Department (e.g. CSE): ");
            scanf("%s", list[count].dept);
            printf("Enter Year: ");
            scanf("%d", &list[count].year);
            printf("Enter Marks: ");
            scanf("%f", &list[count].marks);

            list[count].grade = getGrade(list[count].marks);
            count++;

            saveFiles(list, count);
            printf("Student added successfully!\n");

        } else if (choice == 2) {
            if (count == 0) {
                printf("No records found.\n");
            } else {
                printf("\nRoll\tName\t\tDept\tYear\tMarks\tGrade\n");
                for (int i = 0; i < count; i++) {
                    printf("%d\t%s\t\t%s\t%d\t%.2f\t%c\n",
                           list[i].rollNo, list[i].name, list[i].dept,
                           list[i].year, list[i].marks, list[i].grade);
                }
            }

        } else if (choice == 3) {
            int searchRoll, found = 0;
            printf("Enter Roll No to search: ");
            scanf("%d", &searchRoll);

            for (int i = 0; i < count; i++) {
                if (list[i].rollNo == searchRoll) {
                    printf("Found: %s | Dept: %s | Marks: %.2f | Grade: %c\n",
                           list[i].name, list[i].dept, list[i].marks, list[i].grade);
                    found = 1;
                    break;
                }
            }
            if (!found) printf("Student not found.\n");

        } else if (choice == 4) {
            int updateRoll, found = 0;
            printf("Enter Roll No to update: ");
            scanf("%d", &updateRoll);

            for (int i = 0; i < count; i++) {
                if (list[i].rollNo == updateRoll) {
                    printf("Enter New Name: ");
                    scanf("%s", list[i].name);
                    printf("Enter New Dept: ");
                    scanf("%s", list[i].dept);
                    printf("Enter New Year: ");
                    scanf("%d", &list[i].year);
                    printf("Enter New Marks: ");
                    scanf("%f", &list[i].marks);
                    list[i].grade = getGrade(list[i].marks);

                    saveFiles(list, count);
                    printf("Record updated!\n");
                    found = 1;
                    break;
                }
            }
            if (!found) printf("Student not found.\n");

        } else if (choice == 5) {
            int delRoll, found = 0;
            printf("Enter Roll No to delete: ");
            scanf("%d", &delRoll);

            for (int i = 0; i < count; i++) {
                if (list[i].rollNo == delRoll) {
                    // Shift records left
                    for (int j = i; j < count - 1; j++) {
                        list[j] = list[j + 1];
                    }
                    count--;
                    saveFiles(list, count);
                    printf("Record deleted!\n");
                    found = 1;
                    break;
                }
            }
            if (!found) printf("Student not found.\n");

        } else if (choice == 6) {
            printf("Total Students: %d\n", count);

        } else if (choice == 7) {
            float minMarks;
            printf("Show students with marks above: ");
            scanf("%f", &minMarks);

            for (int i = 0; i < count; i++) {
                if (list[i].marks > minMarks) {
                    printf("%d - %s (%.2f)\n", list[i].rollNo, list[i].name, list[i].marks);
                }
            }

        } else if (choice == 8) {
            // Free dynamic memory before exiting
            free(list);
            printf("Total menu operations performed: %d\n", totalActions);
            printf("Memory freed. Goodbye!\n");
            break;

        } else {
            printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}

output :

--- STUDENT RECORD SYSTEM ---
1. Add Student
2. Display All Students
3. Search by Roll Number
4. Update Student
5. Delete Student
6. Count Total Students
7. Filter by Marks
8. Exit
Enter choice: 1
Enter Roll No: 101
Enter Name (no spaces, e.g. Rahul_K): Rahul_Kumar
Enter Department (e.g. CSE): CSE
Enter Year: 2
Enter Marks: 92.5
Student added successfully!

--- STUDENT RECORD SYSTEM ---
1. Add Student
2. Display All Students
3. Search by Roll Number
4. Update Student
5. Delete Student
6. Count Total Students
7. Filter by Marks
8. Exit
Enter choice: 1
Enter Roll No: 102
Enter Name (no spaces, e.g. Rahul_K): Priya_S
Enter Department (e.g. CSE): ECE
Enter Year: 2
Enter Marks: 78.0
Student added successfully!

--- STUDENT RECORD SYSTEM ---
1. Add Student
2. Display All Students
3. Search by Roll Number
4. Update Student
5. Delete Student
6. Count Total Students
7. Filter by Marks
8. Exit
Enter choice: 2

Roll    Name            Dept    Year    Marks   Grade
101     Rahul_Kumar     CSE     2       92.50   A
102     Priya_S         ECE     2       78.00   B

--- STUDENT RECORD SYSTEM ---
1. Add Student
2. Display All Students
3. Search by Roll Number
4. Update Student
5. Delete Student
6. Count Total Students
7. Filter by Marks
8. Exit
Enter choice: 3
Enter Roll No to search: 101
Found: Rahul_Kumar | Dept: CSE | Marks: 92.50 | Grade: A

--- STUDENT RECORD SYSTEM ---
1. Add Student
2. Display All Students
3. Search by Roll Number
4. Update Student
5. Delete Student
6. Count Total Students
7. Filter by Marks
8. Exit
Enter choice: 7
Show students with marks above: 80
101 - Rahul_Kumar (92.50)

--- STUDENT RECORD SYSTEM ---
1. Add Student
2. Display All Students
3. Search by Roll Number
4. Update Student
5. Delete Student
6. Count Total Students
7. Filter by Marks
8. Exit
Enter choice: 8
Total menu operations performed: 6
Memory freed. Goodbye!