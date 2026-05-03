#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.dat"
#define MAX_NAME 50

typedef struct {
    int roll;
    char name[MAX_NAME];
    float marks[3];
} Student;

void addStudent() {
    FILE *fp = fopen(FILENAME, "ab");
    if (!fp) { printf("Error opening file!\n"); return; }

    Student s;
    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter Marks for 3 subjects:\n");
    for (int i = 0; i < 3; i++) {
        printf("  Subject %d: ", i + 1);
        scanf("%f", &s.marks[i]);
    }

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Record added successfully!\n");
}

void displayAll() {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { printf("No records found!\n"); return; }

    Student s;
    int count = 0;
    printf("\n%-6s %-20s %-10s %-10s %-10s %-10s\n",
           "Roll", "Name", "Sub1", "Sub2", "Sub3", "Total");
    printf("--------------------------------------------------------------\n");

    while (fread(&s, sizeof(Student), 1, fp)) {
        float total = s.marks[0] + s.marks[1] + s.marks[2];
        printf("%-6d %-20s %-10.2f %-10.2f %-10.2f %-10.2f\n",
               s.roll, s.name, s.marks[0], s.marks[1], s.marks[2], total);
        count++;
    }

    if (count == 0) printf("No records found!\n");
    fclose(fp);
}

int compareStudents(const void *a, const void *b) {
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;
    float t1 = s1->marks[0] + s1->marks[1] + s1->marks[2];
    float t2 = s2->marks[0] + s2->marks[1] + s2->marks[2];
    return (t2 > t1) - (t2 < t1); // descending
}

void generateRankList() {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { printf("No records found!\n"); return; }

    // Count records
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    int n = fileSize / sizeof(Student);
    rewind(fp);

    if (n == 0) { printf("No records found!\n"); fclose(fp); return; }

    Student *students = malloc(n * sizeof(Student));
    if (!students) { printf("Memory error!\n"); fclose(fp); return; }

    fread(students, sizeof(Student), n, fp);
    fclose(fp);

    qsort(students, n, sizeof(Student), compareStudents);

    printf("\n%-6s %-6s %-20s %-10s %-10s %-10s %-10s\n",
           "Rank", "Roll", "Name", "Sub1", "Sub2", "Sub3", "Total");
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        float total = students[i].marks[0] + students[i].marks[1] + students[i].marks[2];
        printf("%-6d %-6d %-20s %-10.2f %-10.2f %-10.2f %-10.2f\n",
               i + 1, students[i].roll, students[i].name,
               students[i].marks[0], students[i].marks[1], students[i].marks[2], total);
    }

    free(students);
}

int main() {
    int choice;
    printf("=== Student Record Management System ===\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Generate Rank List\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent();      break;
            case 2: displayAll();      break;
            case 3: generateRankList(); break;
            case 4: printf("Goodbye!\n"); return 0;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}