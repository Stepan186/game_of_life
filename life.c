#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define COLONY "0"

int **createMatrix(int row, int column);
void freeMatrix(int **matrix, int row);
void initFirstGen(int **matrix);
int numberNeighbors(int **matrix, int i_row, int j_column, int row, int column);
void changeStateCell(int **gen, int val_cell, int i, int j, int neig);
int copyMatrix(int **first_matrix, int **second_matrix, int row, int column);
int notEqual(int **first_matrix, int **second_matrix, int row, int column);
void nextGen(int **gen1, int **gen2, int row, int column);
void clearScreen();
void renderGen(int **matrix, int row, int column);

int main() {
    int **gen_I = createMatrix(HEIGHT, WIDTH);
    int **gen_II = createMatrix(HEIGHT, WIDTH);
    int **gen_tmp = createMatrix(HEIGHT, WIDTH);

    int genNumber = 1;
    int isGame = 1;

    initFirstGen(gen_I);

    while (isGame) {
        renderGen(gen_I, HEIGHT, WIDTH);
        if (genNumber % 2 == 0) {
            nextGen(gen_I, gen_II, HEIGHT, WIDTH);
            copyMatrix(gen_II, gen_I, HEIGHT, WIDTH);
            // renderGen(gen_II, HEIGHT, WIDTH);
        } else {
            nextGen(gen_II, gen_I, HEIGHT, WIDTH);
            copyMatrix(gen_I, gen_II, HEIGHT, WIDTH);
            // renderGen(gen_I, HEIGHT, WIDTH);
        }
        usleep(1000000);
        genNumber++;
    }

    freeMatrix(gen_I, HEIGHT);
    freeMatrix(gen_II, HEIGHT);
    freeMatrix(gen_tmp, HEIGHT);
    return 0;
}

// генерация матрицы row x column
int **createMatrix(int row, int column) {
    int **matrix = malloc(row * sizeof(int *));
    for (int i = 0; i < row; i++) {
        matrix[i] = calloc(column, sizeof(int));
    }
    return matrix;
}

// освобождение памяти матрицы
void freeMatrix(int **matrix, int row) {
    for (int i = 0; i < row; i++) free(matrix[i]);
    free(matrix);
}

// считывание с stdin первых поколений в формате row_column, при вводе -1_-1 ввод отсанавливается
void initFirstGen(int **matrix) {
    int row, column;
    int err = 0;
    if (scanf("%d_%d", &row, &column) != 2 || row < 0 || column < 0) err = 1;

    while (row != -1 && column != -1) {
        if (!err) {
            matrix[row % HEIGHT][column % WIDTH] = 1;
        }
        if (scanf("%d_%d", &row, &column) != 2 || row < 0 || column < 0) {
            err = 1;
        } else {
            err = 0;
        }
    }
}

// подсчет соседей у ячейки
int numberNeighbors(int **matrix, int i_row, int j_column, int row, int column) {
    int count = 0;
    for (int i = i_row - 1; i <= i_row + 1; i++) {
        for (int j = j_column - 1; j <= j_column + 1; j++) {
            if (i == i_row && j == j_column) continue;
            int tmp_i = i;
            int tmp_j = j;
            if (tmp_i == -1) tmp_i = row - 1;
            if (tmp_i == row) tmp_i = 0;
            if (tmp_j == -1) tmp_j = column - 1;
            if (tmp_j == column) tmp_j = 0;
            count += matrix[tmp_i][tmp_j];
        }
    }
    return count;
}

// изменение состояния клетки в зависимости от соседей
void changeStateCell(int **gen, int val_cell, int i, int j, int neig) {
    if (val_cell) {
        if (neig < 2 || neig > 3) gen[i][j] = 0;
    } else {
        if (neig == 3) gen[i][j] = 1;
    }
}

// перенос значений с одной матрицы в другую с подсчетов живых ячеек
int copyMatrix(int **first_matrix, int **second_matrix, int row, int column) {
    int count = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (first_matrix[i][j]) count++;
            second_matrix[i][j] = first_matrix[i][j];
            // first_matrix[i][j] = 0;
        }
    }
    return count;
}

// проверка на неравенство матрниц
int notEqual(int **first_matrix, int **second_matrix, int row, int column) {
    int result = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (second_matrix[i][j] != first_matrix[i][j]) {
                result = 1;
                break;
            }
        }
    }
    return result;
}

// генерация следующего поколения
void nextGen(int **prev_gen, int **next_gen, int row, int column) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            int neig = numberNeighbors(prev_gen, i, j, row, column);
            changeStateCell(next_gen, prev_gen[i][j], i, j, neig);
        }
    }
}

// очистка экрана
void clearScreen() {
    printf("\033[2J");
    printf("\033[0;0f");
}

// вывод матрицы на экран
void renderGen(int **matrix, int row, int column) {
    clearScreen();
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            if (matrix[i][j]) {
                printf("%s", COLONY);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}
