#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void) {
    //init data base 
    struct Student *students = NULL;

    //keep track for number of students
    unsigned int size = 0;

    //for commands
    char input[100];
    int loop = 1;
    while (loop) {
        
        fgets(input, sizeof(input), stdin);
        
        if (input[0] == 'A') {

            char student_number[50], last_name[50], first_name[50];
            //checking if A is followed by 3 arguments 
            if(sscanf(input, "A %s %s %s", student_number, last_name, first_name)==3){
                struct Student *newarray = addStudent(students, size, student_number, last_name, first_name);
                if(newarray != NULL){
                    students = newarray;
                    size++;
                }
            }else{
                printf("A should be followed by exactly 3 arguments.\n");
            }
          
        } else if (input[0] == 'U') {

            char student_number[7];
            int round, points;
            //checking if U is followed by 3 arguments 
            if (sscanf(input, "U %s %d %d", student_number, &round, &points) != 3) {
                printf("U should be followed by exactly 3 arguments.\n");
               
            }else{
                updatePoints(students, size, student_number, round, points);
            }
            
        } else if (input[0] == 'L') {

            // The database entries should be printed in the order of total points such
            // that the student with higher points will be printed first.

            printDB(students, size);

        } else if (input[0] == 'W') {

            char filename[50];
            sscanf(input, "W %s", filename);
            saveToFile(students, size,filename);

        } else if (input[0] == 'O') {
            char filename[50];
            sscanf(input, "O %s", filename);
            int l = lines(filename, size);
            struct Student *newarray = loadFromFile(filename, l);
            //if reading the file was ok uodate the database
            if(newarray != NULL){
                releaseMemory(students, size);
                students = newarray;
                size = l;
            }
           
        } else if (input[0] == 'Q') {

            releaseMemory(students, size);
            printf("SUCCESS\n");
            //exit the loop
            loop = 0;
        } else {
            printf("Invalid command %s\n", input);
        }
      
    }
   
}

struct Student *addStudent(struct Student *collection, unsigned int size, const char *student_number, const char *last_name, const char *first_name) {
    
    //scan if student number is already in a database
    for (unsigned int i = 0; i < size; i++) {
        if (strcmp(collection[i].student_number, student_number) == 0) {
            printf("Student %s is already in the database.\n",student_number);
            return NULL;
        }
    }

    //student number cant be more than 6 characters
    if(strlen(student_number)>6 ){
        printf("Student number %s cannot be more than 6 digits.\n",student_number);
        return NULL;
    }

    // Reallocate memory for the collection
    collection = (struct Student*)realloc(collection, (size + 1) * sizeof(struct Student));
    if (collection == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    collection[size].first_name = (char *)malloc((strlen(first_name) + 1) * sizeof(char));
    collection[size].last_name = (char *)malloc((strlen(last_name) + 1) * sizeof(char));

    strcpy(collection[size].student_number, student_number);
    strcpy(collection[size].first_name, first_name);
    strcpy(collection[size].last_name, last_name);

    //init all points as zero
      for (int i = 0; i < 6; i++) {
        collection[size].exercise_points[i] = 0;
    }

    collection[size].total_points = 0;


    printf("SUCCESS\n");
    return collection;
}



void updatePoints(struct Student *students,unsigned int size, char student_number[7], int round, int points){
    if(round <1 || round >6) {
        printf("Round cannot be less than 1 or larger than 6\n");
        return;
    }

    if(points <0){
        printf("Student cannot have negative points.\n");
        return; 
    }
    
    //scan for a student and update points if student exists
    for (unsigned int i = 0; i < size; i++) {
                if (strcmp(students[i].student_number, student_number) == 0) {
                    //updating the points for a round
                    students[i].exercise_points[round - 1] = points;
                    students[i].total_points = 0;
                    //counting total number of points based on the exercises
                    for (int j = 0; j < 6; j++) {
                        students[i].total_points += students[i].exercise_points[j]; 
                    }
                    printf("SUCCESS\n");
                    return;
                }
            }
    
    //student doesnt exist
    printf("Student %s is not in the database.\n",student_number);

};

void printDB(struct Student *students, unsigned int size){

    // Sort students by total points in descending order based on number of total points
   if(size >1){ 
    for (unsigned int i = 0; i < size - 1; i++) {
        for (unsigned int j = 0; j < size - i - 1; j++) {
            if (students[j].total_points < students[j + 1].total_points) {
                //swap students
                struct Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    }

    if(students != NULL || size < 1 ){
    //print sorted database
      for (unsigned int i = 0; i < size; i++) {
        printf("%s %s %s %d %d %d %d %d %d %d\n", students[i].student_number, students[i].last_name, students[i].first_name,
               students[i].exercise_points[0], students[i].exercise_points[1], students[i].exercise_points[2],
               students[i].exercise_points[3], students[i].exercise_points[4], students[i].exercise_points[5],
               students[i].total_points);
    }
    
    }else{
        printf("Database is empty.\n");
    }

    printf("SUCCESS\n");
};

void saveToFile(struct Student *students,unsigned int size, const char* filename){
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    for (unsigned int i = 0; i < size; i++) {
        fprintf(file, "%s %s %s %d %d %d %d %d %d %d\n", students[i].student_number, students[i].last_name, students[i].first_name,
                students[i].exercise_points[0], students[i].exercise_points[1], students[i].exercise_points[2],
                students[i].exercise_points[3], students[i].exercise_points[4], students[i].exercise_points[5],
                students[i].total_points);
    }

    fclose(file);
    printf("SUCCESS\n");
};



/// additional function for counting the files/students in the file
/// needed for updating the size of students array easily
unsigned int lines(const char* filename, unsigned int size){
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        return size; 
	}

    unsigned int count = 0;    
    int ch;

    //counting lines
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    return count;

}

struct Student* loadFromFile(const char* filename, int count){

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Cannot open file %s for reading.\n",filename);
        return NULL; 
	}

    struct Student* students = NULL;
 
    students = (struct Student*)malloc((count + 1) * sizeof(struct Student));
    
    //allocation failed
    if (!students) {
        fclose(fp);
        return NULL; 
    }

    fseek(fp, 0, SEEK_SET);


    for (int i = 0; i < count; i++) {
        // Allocate memory for names
        char temp_last_name[50];
        char temp_first_name[50];
        students[i].last_name = (char *)malloc(50 * sizeof(char));
        students[i].first_name = (char *)malloc(50 * sizeof(char));

        //save students 
        fscanf(fp, "%s %s %s %d %d %d %d %d %d %d\n",
               students[i].student_number, temp_last_name, temp_first_name,
               &students[i].exercise_points[0], &students[i].exercise_points[1],
               &students[i].exercise_points[2], &students[i].exercise_points[3],
               &students[i].exercise_points[4], &students[i].exercise_points[5],
               &students[i].total_points);

        strcpy(students[i].last_name, temp_last_name);
        strcpy(students[i].first_name, temp_first_name);
    }

    fclose(fp);
    printf("SUCCESS\n");
    return students;
};

void releaseMemory(struct Student *students, unsigned int size) {

    //release dynamicly allocated memory units 
    for (unsigned int i = 0; i < size; i++) {
     
        free(students[i].last_name);
        free(students[i].first_name);
        
    }

    free(students);
    
}

