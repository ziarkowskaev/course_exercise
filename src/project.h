#ifndef _PROJECT__H_
#define _PROJECT__H_


struct Student{
	char student_number[7]; //one extra for the ending char
    char *first_name;
    char *last_name;
    int exercise_points[6]; //
    int total_points; //initialy zero
};

struct Student *addStudent(struct Student *collection, unsigned int size, const char *student_number, const char *last_name, const char *first_name);
void updatePoints(struct Student *students,unsigned int size, char student_number[7], int round, int points);
void printDB(struct Student *students, unsigned int size);
void saveToFile(struct Student *students,unsigned int size, const char* filename);
struct Student* loadFromFile(const char* filename, int count);
unsigned int lines(const char* filename, unsigned int size);
void releaseMemory(struct Student *students, unsigned int size);

#endif