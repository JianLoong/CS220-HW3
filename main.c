#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_SIZE 10

int populate_grid(char grid[][MAX_SIZE], char filename_to_read_from[]);

void find_all(char grid[][MAX_SIZE], int n, char word[], FILE *write_to);

int find_right(char grid[][MAX_SIZE], int n, char word[], FILE *write_to);

int find_left(char grid[][MAX_SIZE], int n, char word[], FILE *write_to);

int find_down(char grid[][MAX_SIZE], int n, char word[], FILE *write_to);

int find_up(char grid[][MAX_SIZE], int n, char word[], FILE *write_to);

void display_grid(char grid[][MAX_SIZE], int n);

void display_grid_coor(char grid[][MAX_SIZE], int n);

int find_number(char haystack[], char needle[], int row, int column, char direction, int size, FILE *write_to);


int main(int argc, char *argv[]) {

    char grid[][MAX_SIZE] = {{0},
                             {0},
                             {0},
                             {0},
                             {0},
                             {0},
                             {0},
                             {0},
                             {0},
                             {0}};

    char *grid_file = argv[1];
    int size = populate_grid(grid, grid_file);

    char buffer[255];
    //https://stackoverflow.com/questions/3919009/how-to-read-from-stdin-with-fgets
    FILE *fp = fopen("output.txt", "a+");

    while (fgets(buffer, 255, stdin)) /* break with ^D or ^Z */
    {
        for (char *p = strtok(buffer, " "); p != NULL; p = strtok(NULL, " ")) {
            //https://cboard.cprogramming.com/c-programming/70320-how-remove-newline-string.html
            fflush(stdin);
            int len = strlen(p);
            if (p[len - 1] == '\n')
                p[len - 1] = 0;
            find_all(grid, size, p, fp);
        }
    }
    fclose(fp);
    return 0;
}

void find_all(char grid[][MAX_SIZE], int n, char word[], FILE *write_to) {
    int total = 0;
    total += find_right(grid, n, word, write_to);
    total += find_left(grid, n, word, write_to);
    total += find_down(grid, n, word, write_to);
    total += find_up(grid, n, word, write_to);

    if (total == 0) {
        printf("%s - not found\n", word);
        fprintf(write_to, "%s - not found\n", word);
    }
}

void display_grid_coor(char grid[][MAX_SIZE], int n) {
    printf("\nThe Grid is\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d%d ", i, j);
        printf("\n");
    }
}


void display_grid(char grid[][MAX_SIZE], int n) {
    printf("\nThe Grid is\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%c ", grid[i][j]);
        printf("\n");
    }
}

int populate_grid(char grid[][MAX_SIZE], char filename_to_read_from[]) {
    char const *const fileName = filename_to_read_from; /* should check that argc > 1 */
    FILE *file = fopen(fileName, "r"); /* should check the result */
    char line[256];
    int i = 0;
    int j = 0;
    int size = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        strtok(line, "\n");
        size = strlen(line);
        for (j = 0; j < size; ++j) {
            if (i > MAX_SIZE - 1 || j > MAX_SIZE - 1)
                exit(2);
            grid[i][j] = line[j];
        }
        i++;
    }

    if (i != j)
        exit(2);

    fclose(file);
    return size;
}


int find_number(char haystack[], char needle[], int row, int column, char direction, int size, FILE *write_to) {
    int total = 0;
    char *mem_pos = haystack;

    while ((haystack = strstr(haystack, needle)) != NULL) {
        //Increment the haystack to the next position based on the length
        unsigned long pos = haystack - mem_pos;
        if (direction == 'U')
            row = (int) (size - pos - 1);
        if (direction == 'D')
            row = (int) pos;
        if (direction == 'R')
            column = (int) pos;
        if (direction == 'L')
            column = (int) (size - pos - 1);

        //Move haystack one position instead of length
        haystack++;
        printf("%s %d %d %c\n", needle, row, column, direction);
        fprintf(write_to, "%s %d %d %c\n", needle, row, column, direction);
        total++;
    }

    //printf("\nTotal in row %d ", total);
    return total;
}

int find_right(char grid[][MAX_SIZE], int n, char word[], FILE *write_to) {
    char right[n];
    int num_found = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            right[j] = grid[i][j];
        }
        num_found += find_number(right, word, i, -1, 'R', n, write_to);
    }
    return num_found;
}

int find_left(char grid[][MAX_SIZE], int n, char word[], FILE *write_to) {
    char left[n];
    int num_found = 0;
    for (int i = 0; i < n; i++) {
        for (int j = n - 1; j >= 0; j--) {
            left[n - j - 1] = grid[i][j];
        }
        num_found += find_number(left, word, i, -1, 'L', n, write_to);
    }
    return num_found;
}

int find_down(char grid[][MAX_SIZE], int n, char word[], FILE *write_to) {
    //00,10,20,30
    //01,11,21,31
    char down[n];
    int num_found = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            down[j] = grid[j][i];
        }
        num_found += find_number(down, word, -1, i, 'D', n, write_to);
    }
    return num_found;
}

int find_up(char grid[][MAX_SIZE], int n, char word[], FILE *write_to) {
    //30,20,10,00
    //31,21,11,01
    char up[n];
    int num_found = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            up[j] = grid[n - j - 1][i];
        }
        num_found += find_number(up, word, -1, i, 'U', n, write_to);
    }
    return num_found;
}