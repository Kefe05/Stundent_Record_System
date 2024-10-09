#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Student Record System in C

// Define the Student structure
typedef struct {
    int roll_number;
    char name[20];
    float marks;
    char success[10]; // Field to hold "Passed" or "Failed"
} Student;

// Global variable to hold the current filename
char currentFilename[100] = "No file loaded"; // Default message

// 1. Display a welcome message and greet the user by name
void welcome() {
    char user_name[50]; // Array to hold the user's name

    printf("Welcome to the Student Record System\n");
    printf("Enter your name: ");
    fgets(user_name, sizeof(user_name), stdin);

    // Remove newline character if present
    user_name[strcspn(user_name, "\n")] = '\0';

    printf("Hello, %s!\n", user_name);
}

// 2. Input student details and determine pass/fail status
void inputStudent(Student *student) {
    char buffer[100];

    printf("Enter roll number: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &student->roll_number);

    printf("Enter name: ");
    fgets(student->name, sizeof(student->name), stdin);
    // Remove newline character if present
    student->name[strcspn(student->name, "\n")] = 0;

    printf("Enter marks: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%f", &student->marks);

    strcpy(student->success, student->marks > 40 ? "Passed" : "Failed");
}

// 3. Display student details
void displayStudent(const Student *student) {
    printf("Roll Number: %d, Name: %s, Marks: %.2f, Success: %s\n",
           student->roll_number, student->name, student->marks, student->success);
}

// 4. Display all students
void displayAllStudents(const Student *students, int count) {
    for (int i = 0; i < count; i++) {
        displayStudent(&students[i]);
    }
}

// 5. Add a student
void addStudent(Student **students, int *count, int *capacity) {
    if (*count >= *capacity) {
        *capacity *= 2;
        *students = (Student *)realloc(*students, *capacity * sizeof(Student));
        if (*students == NULL) {
            printf("Memory reallocation failed\n");
            exit(1);
        }
    }
    inputStudent(&(*students)[*count]);
    (*count)++;
}

// 6. Remove a student by roll number
void removeStudent(Student **students, int *count, int *capacity, int roll_number) {
    int found = 0;
    for (int i = 0; i < *count; i++) {
        if ((*students)[i].roll_number == roll_number) {
            found = 1;
            for (int j = i; j < *count - 1; j++) {
                (*students)[j] = (*students)[j + 1];
            }
            (*count)--;
            break;
        }
    }
    if (!found) {
        printf("Student with roll number %d not found.\n", roll_number);
    } else {
        printf("Student with roll number %d removed.\n", roll_number);
        // Resize array if necessary
        if (*count > 0 && *count <= *capacity / 4) {
            *capacity /= 2;
            *students = (Student *)realloc(*students, *capacity * sizeof(Student));
            if (*students == NULL) {
                printf("Memory reallocation failed\n");
                exit(1);
            }
        }
    }
}

// 7. Modify a student by roll number
void modifyStudent(Student *students, int count, int roll_number) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].roll_number == roll_number) {
            found = 1;
            printf("Modifying student with roll number %d:\n", roll_number);
            inputStudent(&students[i]);
            break;
        }
    }
    if (!found) {
        printf("Student with roll number %d not found.\n", roll_number);
    }
}

// 8. Save students to a file (modified)
void saveStudentsToFile(const Student *students, int count) {
    char filename[100];

    printf("Enter filename to save to (leave blank to create a new file): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline

    // If the filename is left blank, ask user for new file name
    if (strlen(filename) == 0) {
        printf("No file name provided. Creating a new file.\n");
        printf("Enter new filename (e.g., 'newfile.txt'): ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = 0; // Remove newline
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n%s\n%.2f\n%s\n", students[i].roll_number, students[i].name, students[i].marks, students[i].success);
    }
    fclose(file);
    printf("Students saved to %s\n", filename);
}

// 9. Load students from a file (modified)
void loadStudentsFromFile(Student **students, int *count, int *capacity) {
    char filename[100];

    printf("Enter filename to load from (leave blank to create a new file): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline

    // If the filename is left blank, ask user for new file name
    if (strlen(filename) == 0) {
        printf("No file name provided. Creating a new file.\n");
        printf("Enter new filename (e.g., 'newfile.txt'): ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = 0; // Remove newline
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return;
    }
    Student temp;
    while (fscanf(file, "%d\n", &temp.roll_number) != EOF) {
        fgets(temp.name, sizeof(temp.name), file);
        temp.name[strcspn(temp.name, "\n")] = 0;
        fscanf(file, "%f\n", &temp.marks);
        fgets(temp.success, sizeof(temp.success), file);
        temp.success[strcspn(temp.success, "\n")] = 0;
        if (*count >= *capacity) {
            *capacity *= 2;
            *students = (Student *)realloc(*students, *capacity * sizeof(Student));
            if (*students == NULL) {
                printf("Memory reallocation failed\n");
                fclose(file);
                return;
            }
        }
        (*students)[*count] = temp;
        (*count)++;
    }
    fclose(file);
    printf("Students loaded from %s\n", filename);

    // Update the current filename
    strcpy(currentFilename, filename);
}

// 10. Display all students from a file (new function)
void displayStudentsFromFile() {
    char filename[100];
    Student temp;

    printf("Enter filename to display students from: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return;
    }

    printf("Displaying students from file %s:\n", filename);
    while (fscanf(file, "%d\n", &temp.roll_number) != EOF) {
        fgets(temp.name, sizeof(temp.name), file);
        temp.name[strcspn(temp.name, "\n")] = 0;
        fscanf(file, "%f\n", &temp.marks);
        fgets(temp.success, sizeof(temp.success), file);
        temp.success[strcspn(temp.success, "\n")] = 0;
        displayStudent(&temp);
    }
    fclose(file);
}

// 11. Calculate and display average marks
void calculateAverageMarks(const Student *students, int count) {
    if (count == 0) {
        printf("No students available to calculate average marks.\n");
        return;
    }

    float totalMarks = 0.0;
    for (int i = 0; i < count; i++) {
        totalMarks += students[i].marks;
    }

    float averageMarks = totalMarks / count;
    printf("Average Marks of All Students: %.2f\n", averageMarks);
}

// 12. Compare students by marks in ascending order
int compareMarksAscending(const void *a, const void *b) {
    float markA = ((Student *)a)->marks;
    float markB = ((Student *)b)->marks;
    return (markA > markB) - (markA < markB);
}

// 13. Compare students by marks in descending order
int compareMarksDescending(const void *a, const void *b) {
    float markA = ((Student *)a)->marks;
    float markB = ((Student *)b)->marks;
    return (markB > markA) - (markB < markA);
}

// 14. Sort students by marks
void sortStudentsByMarks(Student *students, int count, int ascending) {
    if (ascending) {
        qsort(students, count, sizeof(Student), compareMarksAscending);
    } else {
        qsort(students, count, sizeof(Student), compareMarksDescending);
    }
    printf("Students sorted by marks in %s order.\n", ascending ? "ascending" : "descending");
}

// 15. Search for a student by roll number
void searchStudent(const Student *students, int count, int roll_number) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].roll_number == roll_number) {
            found = 1;
            printf("Student found:\n");
            displayStudent(&students[i]);
            break;
        }
    }
    if (!found) {
        printf("Student with roll number %d not found.\n", roll_number);
    }
}

// Main function
int main() {
    Student *students = NULL;
    int count = 0, capacity = 2;
    students = (Student *)malloc(capacity * sizeof(Student));

    if (students == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    welcome();

    int choice;
    char buffer[100];

    do {
        printf("\nCurrent File: %s\n", currentFilename); // Display the current file
        printf("Menu:\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Remove Student by Roll Number\n");
        printf("4. Modify Student by Roll Number\n");
        printf("5. Save Students to File\n");
        printf("6. Load Students from File\n");
        printf("7. Display Students from File\n");
        printf("8. Calculate Average Marks\n");
        printf("9. Sort Students by Marks\n");
        printf("10. Search Student by Roll Number\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        switch (choice) {
            case 1:
                addStudent(&students, &count, &capacity);
                break;
            case 2:
                displayAllStudents(students, count);
                break;
            case 3: {
                int roll_number;
                printf("Enter roll number of student to remove: ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &roll_number);
                removeStudent(&students, &count, &capacity, roll_number);
                break;
            }
            case 4: {
                int roll_number;
                printf("Enter roll number of student to modify: ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &roll_number);
                modifyStudent(students, count, roll_number);
                break;
            }
            case 5:
                saveStudentsToFile(students, count);
                break;
            case 6:
                loadStudentsFromFile(&students, &count, &capacity);
                break;
            case 7:
                displayStudentsFromFile();
                break;
            case 8:
                calculateAverageMarks(students, count);
                break;
            case 9: {
                int ascending;
                printf("Enter 1 for ascending order or 0 for descending order: ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &ascending);
                sortStudentsByMarks(students, count, ascending);
                break;
            }
            case 10: {
                int roll_number;
                printf("Enter roll number to search: ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &roll_number);
                searchStudent(students, count, roll_number);
                break;
            }
            case 11:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 11);

    free(students);
    return 0;
}
