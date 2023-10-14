#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

void sah_print_FILE(int** sah, int n, FILE*file) {
    fprintf(file, "___________________________\n");
    fprintf(file, "Tabla de sah curenta: \n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "%d ", sah[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "___________________________\n");
}

void sah_print(int** sah, int n) {
    sah_print_FILE(sah, n, stdout);
}

void kill_and_swap(int ratRow, int ratCol, int pieceRow, int pieceCol, int** sah) {
    sah[pieceRow][pieceCol] = sah[ratRow][ratCol];
    sah[ratRow][ratCol] = 0;
}


void sah_placements(int** sah, int n, FILE* input) {
    char buffer[100];
    fgets(buffer, 100, input);


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(input, "%d", &sah[i][j]);
        }
        fgets(buffer, 100, input);
    }
}

int sah_cankill(int** sah, int n, int ratRow, int ratCol) {
   
    int pieceOffsets[8][2] = {
        {-1, 0},   // Up
        {1, 0},    // Down
        {0, -1},   // Left
        {0, 1},    // Right
        {-1, -1},  // Up-left
        {-1, 1},   // Up-right
        {1, -1},   // Down-left
        {1, 1}     // Down-right
    };

    for (int i = 0; i < n; i++) {
        int dx = pieceOffsets[i][0];
        int dy = pieceOffsets[i][1];


        int newRow = ratRow + dx;
        int newCol = ratCol + dy;

        while (newRow >= 0 && newRow < n && newCol >= 0 && newCol < n) {
            int targetPiece = sah[newRow][newCol];

            if (targetPiece == 1) {
                return 1;
            }

            newRow += dx;
            newCol += dy;
        }
    }

    return 0;
}

int find_Row(int ** sah, int n, int param) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (sah[i][j] == param)
                return i;
        }
    }
}

int find_Col(int** sah, int n, int param) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (sah[i][j] == param)
                return j;
        }
    }
}

int sah_noPieces(int** sah, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (sah[i][j] == 1) 
                return 0;
        }
    }
    return 1; //doar sobolanu ramane
}

int main() {
    int n;

    FILE* output = fopen("solution.txt", "w");
    if (output == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    FILE* input = fopen("input.txt", "r");
    if (input == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fscanf(input, "%d", &n);
    
    int** sah = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i < n; ++i)
        sah[i] = (int*)malloc(n * sizeof(int));
        

    sah_placements(sah, n, input);
    sah_print(sah, n);
    sah_print_FILE(sah, n, output);

    int ratRow = find_Row(sah, n, 9);
    int ratCol = find_Col(sah, n, 9);
    
    printf("Rat position: [%d] [%d] \n", ratRow+1, ratCol+1);

    int targetRow = find_Row(sah, n, 1);
    int targetCol = find_Col(sah, n, 1);
    
    while (sah_cankill(sah, n, ratRow, ratCol)) {
        printf("Sobolanu ucide o piesa! \n");
        
        ratRow = find_Row(sah, n, 9);
        ratCol = find_Col(sah, n, 9);
        targetRow = find_Row(sah, n, 1);
        targetCol = find_Col(sah, n, 1);

        kill_and_swap(ratRow, ratCol, targetRow, targetCol, sah);
        sah_print(sah, n);
        sah_print_FILE(sah, n, output);
        sleep_ms(1000);

        if (sah_noPieces(sah, n)) {
            printf("Sobolanul a castigat! \n");
            break;
        }
    }
    

    fclose(output);
    fclose(input);

    if (sah != NULL) {
        for (int i = 0; i < n; i++) {
            free(sah[i]);
        }
        free(sah);
    }

    return 0;
}

