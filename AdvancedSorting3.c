/*
=====================================================
Tugas 2 Praktikum Struktur Data dan Algoritma B
Enhanced Sorting Algorithms Comparison Program
Name : Zaw Thein Htet
NPM  : 250810701100118
=====================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_WORDS  5000
#define MAX_LEN    100
#define PREVIEW    10   /* elements shown in before/after previews */

/*UTILITY*/
static void swapInt(int *a, int *b)
    { int t = *a; *a = *b; *b = t; }
static void swapStr(char a[][MAX_LEN], int i, int j) {
    char t[MAX_LEN];
    memcpy(t, a[i], MAX_LEN);
    memcpy(a[i], a[j], MAX_LEN);
    memcpy(a[j], t,    MAX_LEN);
}
static void clearBuf(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Safe integer input with range validation */
static int getInt(const char *prompt, int lo, int hi) {
    int v;
    for (;;) {
        printf("%s", prompt);
        if (scanf("%d", &v) == 1 && v >= lo && v <= hi) return v;
        printf("  Please enter a number between %d and %d.\n", lo, hi);
        clearBuf();
    }
}
static void copyInt(const int *src, int *dst, int n)
    { memcpy(dst, src, (size_t)n * sizeof(int)); }
static void copyStr(char (*src)[MAX_LEN], char (*dst)[MAX_LEN], int n)
    { for (int i = 0; i < n; i++) strcpy(dst[i], src[i]); }

/*DATA GENERATION & DISPLAY*/
static void genRandom(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % 100000;
}
static void shuffleInt(int arr[], int n) {
    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        swapInt(&arr[i], &arr[j]);
    }
}
static void shuffleStr(char arr[][MAX_LEN], int n) {
    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        swapStr(arr, i, j);
    }
}
static int loadWords(char words[][MAX_LEN], const char *fname) {
    FILE *f = fopen(fname, "r");
    if (!f) { printf("  Cannot open '%s'!\n", fname); return 0; }
    int c = 0;
    while (c < MAX_WORDS && fscanf(f, "%99s", words[c]) == 1) c++;
    fclose(f);
    printf("  Loaded %d words from '%s'.\n", c, fname);
    return c;
}
static void previewInt(const int arr[], int n, const char *label) {
    int s = n < PREVIEW ? n : PREVIEW;
    printf("  %-20s:", label);
    for (int i = 0; i < s; i++) printf(" %d", arr[i]);
    if (n > PREVIEW) printf(" ...");
    printf("\n");
}
static void previewStr(char arr[][MAX_LEN], int n, const char *label) {
    int s = n < PREVIEW ? n : PREVIEW;
    printf("  %s (first %d of %d):\n", label, s, n);
    for (int i = 0; i < s; i++) printf("    [%2d] %s\n", i+1, arr[i]);
    if (n > PREVIEW) printf("    ...\n");
}
static void printTableHeader(void) {
    printf("\n  %-22s  %-16s  %s\n",
           "Algorithm", "Time (s)", "Comparisons");
    printf("  %-22s  %-16s  %s\n",
           "----------------------", "----------------", "---------------");
}
static void printTableRow(const char *name, double t, long long c) {
    printf("  %-22s  %-16.6f  %lld\n", name, t, c);
}

/*INTEGER SORTING ALGORITHMS  (each returns comparison count)*/
static long long bubbleSortI(int a[], int n) {
    long long cmp = 0;
    for (int i = 0; i < n-1; i++) {
        int sw = 0;
        for (int j = 0; j < n-i-1; j++) {
            cmp++;
            if (a[j] > a[j+1]) { swapInt(&a[j], &a[j+1]); sw = 1; }
        }
        if (!sw) break;   /* early exit: already sorted */
    }
    return cmp;
}
static long long insertionSortI(int a[], int n) {
    long long cmp = 0;
    for (int i = 1; i < n; i++) {
        int key = a[i], j = i-1;
        while (j >= 0 && (cmp++, a[j] > key)) { a[j+1] = a[j]; j--; }
        a[j+1] = key;
    }
    return cmp;
}
static long long selectionSortI(int a[], int n) {
    long long cmp = 0;
    for (int i = 0; i < n-1; i++) {
        int m = i;
        for (int j = i+1; j < n; j++) { cmp++; if (a[j] < a[m]) m = j; }
        if (m != i) swapInt(&a[i], &a[m]);
    }
    return cmp;
}

/* Merge Sort (integer) */
static long long gCmpI;   /* global counter used by recursive calls */
static void _mergeI(int a[], int l, int m, int r) {
    int n1 = m-l+1, n2 = r-m;
    int *L = malloc((size_t)n1 * sizeof(int));
    int *R = malloc((size_t)n2 * sizeof(int));
    if (!L || !R) { free(L); free(R); return; }
    for (int i = 0; i < n1; i++) L[i] = a[l+i];
    for (int j = 0; j < n2; j++) R[j] = a[m+1+j];
    int i=0, j=0, k=l;
    while (i<n1 && j<n2) {
        gCmpI++;
        if (L[i] <= R[j]) a[k++] = L[i++];
        else               a[k++] = R[j++];
    }
    while (i < n1) a[k++] = L[i++];
    while (j < n2) a[k++] = R[j++];
    free(L); free(R);
}
static void _msI(int a[], int l, int r) {
    if (l < r) {
        int m = l + (r-l)/2;
        _msI(a, l, m);
        _msI(a, m+1, r);
        _mergeI(a, l, m, r);
    }
}
static long long mergeSortI(int a[], int n) {
    gCmpI = 0; _msI(a, 0, n-1); return gCmpI;
}

/* Quick Sort (integer) with median-of-three pivot */
static int _partitionI(int a[], int lo, int hi) {
    /* Median-of-three: sort a[lo], a[mid], a[hi] then use median as pivot */
    if (hi - lo >= 2) {
        int mid = lo + (hi-lo)/2;
        if (a[lo]  > a[mid]) swapInt(&a[lo],  &a[mid]);
        if (a[lo]  > a[hi])  swapInt(&a[lo],  &a[hi]);
        if (a[mid] > a[hi])  swapInt(&a[mid], &a[hi]);
        swapInt(&a[mid], &a[hi]);  /* move median to end as pivot */
    }
    int pivot = a[hi], i = lo-1;
    for (int j = lo; j < hi; j++) {
        gCmpI++;
        if (a[j] < pivot) swapInt(&a[++i], &a[j]);
    }
    swapInt(&a[i+1], &a[hi]);
    return i+1;
}
static void _qsI(int a[], int lo, int hi) {
    if (lo < hi) {
        int pi = _partitionI(a, lo, hi);
        _qsI(a, lo, pi-1);
        _qsI(a, pi+1, hi);
    }
}
static long long quickSortI(int a[], int n) {
    gCmpI = 0; _qsI(a, 0, n-1); return gCmpI;
}

/* Shell Sort (integer) */
static long long shellSortI(int a[], int n) {
    long long cmp = 0;
    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int tmp = a[i], j;
            for (j = i; j >= gap && (cmp++, a[j-gap] > tmp); j -= gap)
                a[j] = a[j-gap];
            a[j] = tmp;
        }
    }
    return cmp;
}

/* STRING SORTING ALGORITHMS  (same structure as integer) */
static long long bubbleSortS(char a[][MAX_LEN], int n) {
    long long cmp = 0;
    for (int i = 0; i < n-1; i++) {
        int sw = 0;
        for (int j = 0; j < n-i-1; j++) {
            cmp++;
            if (strcmp(a[j], a[j+1]) > 0) { swapStr(a, j, j+1); sw = 1; }
        }
        if (!sw) break;
    }
    return cmp;
}
static long long insertionSortS(char a[][MAX_LEN], int n) {
    long long cmp = 0;
    for (int i = 1; i < n; i++) {
        char key[MAX_LEN]; strcpy(key, a[i]);
        int j = i-1;
        while (j >= 0 && (cmp++, strcmp(a[j], key) > 0)) {
            strcpy(a[j+1], a[j]); j--;
        }
        strcpy(a[j+1], key);
    }
    return cmp;
}
static long long selectionSortS(char a[][MAX_LEN], int n) {
    long long cmp = 0;
    for (int i = 0; i < n-1; i++) {
        int m = i;
        for (int j = i+1; j < n; j++) { cmp++; if (strcmp(a[j], a[m]) < 0) m = j; }
        if (m != i) swapStr(a, i, m);
    }
    return cmp;
}

/* Merge Sort (string) */
static long long gCmpS;
static void _mergeS(char a[][MAX_LEN], int l, int m, int r) {
    int n1 = m-l+1, n2 = r-m;
    char (*L)[MAX_LEN] = malloc((size_t)n1 * sizeof(*L));
    char (*R)[MAX_LEN] = malloc((size_t)n2 * sizeof(*R));
    if (!L || !R) { free(L); free(R); return; }
    for (int i = 0; i < n1; i++) strcpy(L[i], a[l+i]);
    for (int j = 0; j < n2; j++) strcpy(R[j], a[m+1+j]);
    int i=0, j=0, k=l;
    while (i<n1 && j<n2) {
        gCmpS++;
        if (strcmp(L[i], R[j]) <= 0) strcpy(a[k++], L[i++]);
        else                          strcpy(a[k++], R[j++]);
    }
    while (i < n1) strcpy(a[k++], L[i++]);
    while (j < n2) strcpy(a[k++], R[j++]);
    free(L); free(R);
}
static void _msS(char a[][MAX_LEN], int l, int r) {
    if (l < r) {
        int m = l + (r-l)/2;
        _msS(a, l, m);
        _msS(a, m+1, r);
        _mergeS(a, l, m, r);
    }
}
static long long mergeSortS(char a[][MAX_LEN], int n) {
    gCmpS = 0; _msS(a, 0, n-1); return gCmpS;
}

/* Quick Sort (string) with median-of-three pivot */
static int _partitionS(char a[][MAX_LEN], int lo, int hi) {
    if (hi - lo >= 2) {
        int mid = lo + (hi-lo)/2;
        if (strcmp(a[lo],  a[mid]) > 0) swapStr(a, lo,  mid);
        if (strcmp(a[lo],  a[hi])  > 0) swapStr(a, lo,  hi);
        if (strcmp(a[mid], a[hi])  > 0) swapStr(a, mid, hi);
        swapStr(a, mid, hi);   /* move median to end as pivot */
    }
    char pivot[MAX_LEN]; strcpy(pivot, a[hi]);
    int i = lo-1;
    for (int j = lo; j < hi; j++) {
        gCmpS++;
        if (strcmp(a[j], pivot) < 0) swapStr(a, ++i, j);
    }
    swapStr(a, i+1, hi);
    return i+1;
}
static void _qsS(char a[][MAX_LEN], int lo, int hi) {
    if (lo < hi) {
        int pi = _partitionS(a, lo, hi);
        _qsS(a, lo, pi-1);
        _qsS(a, pi+1, hi);
    }
}
static long long quickSortS(char a[][MAX_LEN], int n) {
    gCmpS = 0; _qsS(a, 0, n-1); return gCmpS;
}

/* Shell Sort (string) */
static long long shellSortS(char a[][MAX_LEN], int n) {
    long long cmp = 0;
    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            char tmp[MAX_LEN]; strcpy(tmp, a[i]);
            int j;
            for (j = i; j >= gap && (cmp++, strcmp(a[j-gap], tmp) > 0); j -= gap)
                strcpy(a[j], a[j-gap]);
            strcpy(a[j], tmp);
        }
    }
    return cmp;
}

/* BENCHMARK WRAPPERS Each runs the sort on a fresh copy to preserve the original. */
typedef struct { double time; long long cmp; } Result;
typedef long long (*IntFn)(int[], int);
typedef long long (*StrFn)(char[][MAX_LEN], int);

/* Run an integer sort on a copy of src[0..n-1]. If outArr != NULL, write the sorted copy there (for display). 
If outArr == NULL, an internal buffer is used and freed. */
static Result runInt(IntFn fn, const int *src, int n, int *outArr) {
    int *a = outArr ? outArr : malloc((size_t)n * sizeof(int));
    if (!a) return (Result){-1.0, -1};
    copyInt(src, a, n);
    clock_t s = clock();
    long long c = fn(a, n);
    clock_t e = clock();
    if (!outArr) free(a);
    return (Result){(double)(e-s)/CLOCKS_PER_SEC, c};
}

/*Same for string sorting.*/
static Result runStr(StrFn fn, char (*src)[MAX_LEN], int n,
                     char (*outArr)[MAX_LEN]) {
    char (*a)[MAX_LEN] = outArr ? outArr : malloc((size_t)n * sizeof(*a));
    if (!a) return (Result){-1.0, -1};
    copyStr(src, a, n);
    clock_t s = clock();
    long long c = fn(a, n);
    clock_t e = clock();
    if (!outArr) free(a);
    return (Result){(double)(e-s)/CLOCKS_PER_SEC, c};
}

/* ALGORITHM NAME & FUNCTION TABLES */
static const char *ALGO_NAMES[] = {
    "Bubble Sort", "Insertion Sort", "Selection Sort",
    "Merge Sort",  "Quick Sort",     "Shell Sort"
};
static IntFn INT_FNS[] = {
    bubbleSortI, insertionSortI, selectionSortI,
    mergeSortI,  quickSortI,     shellSortI
};
static StrFn STR_FNS[] = {
    bubbleSortS, insertionSortS, selectionSortS,
    mergeSortS,  quickSortS,     shellSortS
};
#define NUM_ALGOS 6

/* INTEGER SORTING MENU */
static void integerSortMenu(void) {
    int n = getInt("\n  Data size (10 - 50000): ", 10, 50000);
    int *base = malloc((size_t)n * sizeof(int));
    int *work = malloc((size_t)n * sizeof(int));
    if (!base || !work) {
        printf("  Memory allocation failed!\n");
        free(base); free(work); return;
    }
    genRandom(base, n);
    shuffleInt(base, n);
    previewInt(base, n, "Before sorting");
    printf("\n  ===== INTEGER SORTING =====\n");
    for (int i = 0; i < NUM_ALGOS; i++)
        printf("  %d. %s\n", i+1, ALGO_NAMES[i]);
    printf("  7. Compare All\n");
    printf("  8. Back\n");
    int ch = getInt("  Choose: ", 1, 8);
    if (ch == 8) { free(base); free(work); return; }
    if (ch <= NUM_ALGOS) {
        /* Single algorithm: sort into work, display, then report */
        Result r = runInt(INT_FNS[ch-1], base, n, work);
        previewInt(work, n, "After  sorting");
        printTableHeader();
        printTableRow(ALGO_NAMES[ch-1], r.time, r.cmp);
    } else {
        /* Compare All: benchmark every algorithm on the same data */
        printf("\n  Benchmarking all algorithms on %d integers...\n", n);
        printTableHeader();
        for (int i = 0; i < NUM_ALGOS; i++) {
            Result r = runInt(INT_FNS[i], base, n, NULL);
            printTableRow(ALGO_NAMES[i], r.time, r.cmp);
        }
    }
    free(base);
    free(work);
}

/* STRING SORTING MENU*/
static void stringSortMenu(void) {
    /* Static so we don't blow the stack with MAX_WORDS * MAX_LEN bytes */
    static char baseWords[MAX_WORDS][MAX_LEN];
    int total = loadWords(baseWords, "words.txt");
    if (total == 0) return;
    shuffleStr(baseWords, total);
    previewStr(baseWords, total, "Before sorting");
    printf("\n  ===== STRING SORTING =====\n");
    for (int i = 0; i < NUM_ALGOS; i++)
        printf("  %d. %s\n", i+1, ALGO_NAMES[i]);
    printf("  7. Compare All\n");
    printf("  8. Back\n");
    int ch = getInt("  Choose: ", 1, 8);
    if (ch == 8) return;
    char (*work)[MAX_LEN] = malloc((size_t)total * sizeof(*work));
    if (!work) { printf("  Memory allocation failed!\n"); return; }
    if (ch <= NUM_ALGOS) {
        Result r = runStr(STR_FNS[ch-1], baseWords, total, work);
        previewStr(work, total, "After  sorting");
        printTableHeader();
        printTableRow(ALGO_NAMES[ch-1], r.time, r.cmp);
    } else {
        printf("\n  Benchmarking all algorithms on %d words...\n", total);
        printTableHeader();
        for (int i = 0; i < NUM_ALGOS; i++) {
            Result r = runStr(STR_FNS[i], baseWords, total, NULL);
            printTableRow(ALGO_NAMES[i], r.time, r.cmp);
        }
    }
    free(work);
}

/* MAIN*/
int main(void) {
    srand((unsigned int)time(NULL));
    for (;;) {
        printf("  ║     SORTING ALGORITHMS       ║\n");
        printf("  ║  1. Integer Sorting          ║\n");
        printf("  ║  2. String  Sorting          ║\n");
        printf("  ║  3. Exit                     ║\n");
        switch (getInt("  Choose: ", 1, 3)) {
            case 1: integerSortMenu(); break;
            case 2: stringSortMenu();  break;
            case 3: printf("  Program finished.\n"); return 0;
        }
    }
}
