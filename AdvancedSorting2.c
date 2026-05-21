/*
=====================================================
Tugas 2 Praktikum Struktur Data dan Algoritma B
Sorting Algorithms Comparison Program 
Name : Zaw Thein Htet
NPM  : 250810701100118
=====================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SIZE 1000
#define MAX_WORDS 5000
#define MAX_LEN 100
#define BENCH_REPEAT 3

/* Utility */
void swapInt(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void swapString(char a[][MAX_LEN], int i, int j) {
    char temp[MAX_LEN];
    strcpy(temp, a[i]);
    strcpy(a[i], a[j]);
    strcpy(a[j], temp);
}
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
int getIntInput(const char *prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            if (value >= min && value <= max) return value;
            printf("Input must be between %d and %d.\n", min, max);
        } else {
            printf("Invalid input. Please enter a number.\n");
        }
        clearInputBuffer();
    }
}
int isSortedInt(const int arr[], int n) {
    for (int i = 1; i < n; i++) if (arr[i - 1] > arr[i]) return 0;
    return 1;
}
int isSortedString(char arr[][MAX_LEN], int n) {
    for (int i = 1; i < n; i++) if (strcmp(arr[i - 1], arr[i]) > 0) return 0;
    return 1;
}

/* Data */
void generateRandomData(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % 10000; /* keep original range */
}
void shuffleInt(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swapInt(&arr[i], &arr[j]);
    }
}
void shuffleString(char arr[][MAX_LEN], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swapString(arr, i, j);
    }
}
void copyIntArray(const int src[], int dst[], int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}
void copyStringArray(char src[][MAX_LEN], char dst[][MAX_LEN], int n) {
    for (int i = 0; i < n; i++) strcpy(dst[i], src[i]);
}
void displayInt(const int arr[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}
void displayString(char arr[][MAX_LEN], int n) {
    for (int i = 0; i < n; i++) printf("%s\n", arr[i]);
}

/* Integer Sorts */
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swapInt(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) if (arr[j] < arr[min]) min = j;
        if (min != i) swapInt(&arr[i], &arr[min]);
    }
}

/* String Sorts */
void merge(char arr[][MAX_LEN], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    char L[n1][MAX_LEN], R[n2][MAX_LEN];
    for (int i = 0; i < n1; i++) strcpy(L[i], arr[left + i]);
    for (int j = 0; j < n2; j++) strcpy(R[j], arr[mid + 1 + j]);
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (strcmp(L[i], R[j]) <= 0) strcpy(arr[k++], L[i++]);
        else strcpy(arr[k++], R[j++]);
    }
    while (i < n1) strcpy(arr[k++], L[i++]);
    while (j < n2) strcpy(arr[k++], R[j++]);
}
void mergeSort(char arr[][MAX_LEN], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
int partition(char arr[][MAX_LEN], int low, int high) {
    char pivot[MAX_LEN];
    strcpy(pivot, arr[high]);
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (strcmp(arr[j], pivot) < 0) {
            i++;
            swapString(arr, i, j);
        }
    }
    swapString(arr, i + 1, high);
    return i + 1;
}
void quickSort(char arr[][MAX_LEN], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
void shellSort(char arr[][MAX_LEN], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            char temp[MAX_LEN];
            strcpy(temp, arr[i]);
            int j;
            for (j = i; j >= gap && strcmp(arr[j - gap], temp) > 0; j -= gap) {
                strcpy(arr[j], arr[j - gap]);
            }
            strcpy(arr[j], temp);
        }
    }
}
void shellSortAdapter(char arr[][MAX_LEN], int left, int right) {
    (void)left;
    shellSort(arr, right + 1);
}

/* Words */
int loadWords(char words[][MAX_LEN]) {
    FILE *f = fopen("words.txt", "r");
    if (f == NULL) {
        printf("Cannot open words.txt file! Please put words.txt in same folder.\n");
        return 0;
    }
    int count = 0;
    while (count < MAX_WORDS && fscanf(f, "%99s", words[count]) == 1) count++;
    fclose(f);
    return count;
}

/* Benchmark */
double benchmarkIntSort(void (*sortFn)(int[], int), const int src[], int n, int showSample, int verify) {
    int *arr = (int *)malloc((size_t)n * sizeof(int));
    if (!arr) return -1.0;
    double total = 0.0;
    for (int r = 0; r < BENCH_REPEAT; r++) {
        copyIntArray(src, arr, n);
        clock_t start = clock();
        sortFn(arr, n);
        clock_t end = clock();
        total += ((double)(end - start)) / CLOCKS_PER_SEC;
    }
    if (showSample) {
        printf("First 10 Data After Sorting:\n");
        displayInt(arr, n < 10 ? n : 10);
    }
    if (verify) {
        printf("Verification: %s\n", isSortedInt(arr, n) ? "Sorted correctly" : "Sort FAILED");
    }
    free(arr);
    return total / BENCH_REPEAT;
}
double benchmarkStringSort(void (*sortFn)(char[][MAX_LEN], int, int), char src[][MAX_LEN], int n, int showSample, int verify) {
    char (*arr)[MAX_LEN] = malloc((size_t)n * sizeof(*arr));
    if (!arr) return -1.0;
    double total = 0.0;
    for (int r = 0; r < BENCH_REPEAT; r++) {
        copyStringArray(src, arr, n);
        clock_t start = clock();
        sortFn(arr, 0, n - 1);
        clock_t end = clock();
        total += ((double)(end - start)) / CLOCKS_PER_SEC;
    }
    if (showSample) {
        printf("First 10 Words After Sorting:\n");
        displayString(arr, n < 10 ? n : 10);
    }
    if (verify) {
        printf("Verification: %s\n", isSortedString(arr, n) ? "Sorted correctly" : "Sort FAILED");
    }
    free(arr);
    return total / BENCH_REPEAT;
}

/* Menus */
void basicSortingMenu(void) {
    int n = getIntInput("Input data size (10-50000, default 1000 recommended): ", 10, 50000);
    int *base = (int *)malloc((size_t)n * sizeof(int));
    if (!base) {
        printf("Memory allocation failed!\n");
        return;
    }
    generateRandomData(base, n);
    shuffleInt(base, n);
    printf("\nFirst 10 Data Before Sorting:\n");
    displayInt(base, n < 10 ? n : 10);
    printf("\n===== BASIC SORTING =====\n");
    printf("1. Bubble Sort\n2. Insertion Sort\n3. Selection Sort\n4. Compare All\n5. Back\n");
    int choice = getIntInput("Choose Method: ", 1, 5);
    if (choice == 5) {
        free(base);
        return;
    }
    double t;
    switch (choice) {
        case 1:
            t = benchmarkIntSort(bubbleSort, base, n, 1, 1);
            printf("Average Execution Time (%d run(s), Bubble Sort): %.6f seconds\n", BENCH_REPEAT, t);
            break;
        case 2:
            t = benchmarkIntSort(insertionSort, base, n, 1, 1);
            printf("Average Execution Time (%d run(s), Insertion Sort): %.6f seconds\n", BENCH_REPEAT, t);
            break;
        case 3:
            t = benchmarkIntSort(selectionSort, base, n, 1, 1);
            printf("Average Execution Time (%d run(s), Selection Sort): %.6f seconds\n", BENCH_REPEAT, t);
            break;
        case 4:
            printf("\nBenchmark result (average of %d run(s)):\n", BENCH_REPEAT);
            printf("Bubble Sort    : %.6f seconds\n", benchmarkIntSort(bubbleSort, base, n, 0, 1));
            printf("Insertion Sort : %.6f seconds\n", benchmarkIntSort(insertionSort, base, n, 0, 1));
            printf("Selection Sort : %.6f seconds\n", benchmarkIntSort(selectionSort, base, n, 0, 1));
            break;
    }
    free(base);
}

void advancedSortingMenu(void) {
    int choice;
    char words[MAX_WORDS][MAX_LEN];
    int totalWords = loadWords(words);
    if (totalWords == 0) return;
    shuffleString(words, totalWords);
    printf("\nFirst 10 Words Before Sorting:\n");
    displayString(words, totalWords < 10 ? totalWords : 10);
    printf("\n===== ADVANCED SORTING =====\n");
    printf("1. Merge Sort\n2. Quick Sort\n3. Shell Sort\n4. Compare All\n5. Back\n");
    choice = getIntInput("Choose Method: ", 1, 5);
    if (choice == 5) return;
    double t;
    switch (choice) {
        case 1:
            t = benchmarkStringSort(mergeSort, words, totalWords, 1, 1);
            printf("Average Execution Time (%d run(s), Merge Sort): %.6f seconds\n", BENCH_REPEAT, t);
            break;
        case 2:
            t = benchmarkStringSort(quickSort, words, totalWords, 1, 1);
            printf("Average Execution Time (%d run(s), Quick Sort): %.6f seconds\n", BENCH_REPEAT, t);
            break;
        case 3:
            t = benchmarkStringSort(shellSortAdapter, words, totalWords, 1, 1);
            printf("Average Execution Time (%d run(s), Shell Sort): %.6f seconds\n", BENCH_REPEAT, t);
            break;
        case 4:
            printf("\nBenchmark result (average of %d run(s)):\n", BENCH_REPEAT);
            printf("Merge Sort : %.6f seconds\n", benchmarkStringSort(mergeSort, words, totalWords, 0, 1));
            printf("Quick Sort : %.6f seconds\n", benchmarkStringSort(quickSort, words, totalWords, 0, 1));
            printf("Shell Sort : %.6f seconds\n", benchmarkStringSort(shellSortAdapter, words, totalWords, 0, 1));
            break;
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    while (1) {
        printf("\n===== MAIN MENU =====\n");
        printf("1. Basic Sorting\n2. Advanced Sorting\n3. Exit\n");
        switch (getIntInput("Choose Menu: ", 1, 3)) {
            case 1:
                basicSortingMenu();
                break;
            case 2:
                advancedSortingMenu();
                break;
            case 3:
                printf("Program Finished.\n");
                return 0;
        }
    }
}
