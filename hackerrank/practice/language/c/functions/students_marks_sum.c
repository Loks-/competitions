// https://www.hackerrank.com/challenges/students-marks-sum

#include <stdio.h>
#include <stdlib.h>

static int marks_summation(int *marks, int number_of_students, char gender) {
  int s = 0;
  for (int i = (gender == 'b') ? 0 : 1; i < number_of_students; i += 2)
    s += marks[i];
  return s;
}

int main_students_marks_sum() {
  int number_of_students;
  char gender;
  int sum;

  scanf("%d", &number_of_students);
  int *marks = (int *)malloc(number_of_students * sizeof(int));

  for (int student = 0; student < number_of_students; student++) {
    scanf("%d", (marks + student));
  }

  scanf(" %c", &gender);
  sum = marks_summation(marks, number_of_students, gender);
  printf("%d", sum);
  free(marks);

  return 0;
}
