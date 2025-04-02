#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h>
#endif

#define MAX_RANGE ULONG_MAX

/**
 * @struct SortingAlgorithm
 * @brief Represents the data of a sorting algorithm
 * @var name Name of the algorithm in title case
 * @var outputFile Name of the file where the sorted integers will be stored
 * @var time Time taken for the algorithms to sort
 * @var function Function pointer to its algorithm
 * 
 * This object gives us OOP-like features, encapsulating related data and
 * improves maintainability. It also allows us to perform more complex logic
 *  such as iteration on the algorithms, and sorting by a member.
 */
typedef struct {
    const char *name;
    const char *outputFile;
    double time;
    void (*function)(unsigned long int*, int);
} SortingAlgorithm;

/**
 * @brief Generates an array of random integers.
 * @param n The size of the resulting array.
 * @return The dynamically allocated array. It needs to be manually freed when used.
 */
unsigned long int *generateRandomIntegers(int n);
/**
 * @brief Generates an array of integers in increasing order
 * @param n The size of the resulting array
 * @param startingValue The value of the first element of the ordered array.
 * @return The dynamically allocated array. It needs to be manually freed when used.
 */
unsigned long int *generateIncreasingSequence(int n, unsigned long int startingValue);

/**
 * @brief The function that runs each algorithms and records the time
 * @param array The unsorted array used on the sorting algorithms
 * @param n The size of the unsorted array
 */
void runBenchmark(unsigned long int *array, int n);

/*
*
* SORTING ALGORITHMS
*
*/
void selectionSort(unsigned long int *array, int n);
void bubbleSort(unsigned long int *array, int n);
void insertionSort(unsigned long int *array, int n);
void merge(unsigned long int *array, int left, int mid, int right);
void mergeSortHelper(unsigned long int *array, int left, int right);
void mergeSort(unsigned long int *array, int n);
int medianOfThree(unsigned long int *array, int low, int high);
int quickSortPartition(unsigned long int *array, int low, int high);
void quickSortHelper(unsigned long int *array, int low, int high);
void quickSort(unsigned long int *array, int n);
void heapSort(unsigned long int *array, int n);

/**
 * @brief Clears the contents of a file.
 * @param filename The name of the file to be cleared.
*/
void clearFile(const char *filename);
/**
 * @brief Appends a formatted string to a file.
 * @param filename The name of the file to append to.
 * @param format The format string (supports printf-style formatting).
 * @param ... Additional arguments for formatting.
 */
void appendStringToFile(const char *filename, const char *format, ...);
/**
 * @brief Appends an array of unsigned long integers to a file.
 * @param filename The name of the file to append to.
 * @param array The array of unsigned long integers.
 * @param n The number of elements in the array.
 */
void appendArrayToFile(const char *filename, unsigned long int *array, int n);
/**
 * @brief Creates a duplicate of an unsigned long int array.
 * @param array The original array to be duplicated.
 * @param n The number of elements in the array.
 * @return A pointer to the newly allocated duplicate array.
 */
unsigned long int *duplicateArray(const unsigned long int *array, int n);
/**
 * @brief Creates a duplicate of an array of SortingAlgorithm structures.
 * @param array The original array of SortingAlgorithm structures.
 * @param n The number of elements in the array.
 * @return A pointer to the newly allocated duplicate array.
 */
SortingAlgorithm *duplicateAlgorithmsArray(SortingAlgorithm *array, int n);
unsigned long int getPositiveInteger(const char *prompt);
/**
 * @brief Gives the current time in seconds.
 * @returns The calculated time.
*/
double getTimeInSeconds(void);
/**
 * @brief The callback function used in qsort()
 * @param a Pointer to the first element.
 * @param b Pointer to the second element.
 * @return A negative value if a < b, 0 if equal, a positive value if a > b.
 */
int compareByTime(const void *a, const void *b);
/**
 * @brief Clears the terminal screen.
 */
void clearScreen(void);
/**
 * @brief Displays the program header.
 */
void displayHeader(void);
/**
 * @brief Displays a confirmation message before exiting the program.
 */
void displayConfirmExit(void);
/**
 * @brief Pauses the program execution for a specified duration.
 * @param milliseconds The duration to sleep in milliseconds.
 */
void sleepProgram(int milliseconds);

/**
 * Initialize the algorithms array. This is made global as multiple functions access it.
*/
SortingAlgorithm algorithms[] = {
    {"Selection Sort", "selection-sort.txt", 0.0, selectionSort},
    {"Bubble Sort", "bubble-sort.txt", 0.0,  bubbleSort},
    {"Insertion Sort", "insertion-sort.txt", 0.0, insertionSort},
    {"Merge Sort", "merge-sort.txt", 0.0, mergeSort},
    {"Quick Sort", "quick-sort.txt", 0.0, quickSort},
    {"Heap Sort", "heap-sort.txt", 0.0, heapSort},
};
int algorithmsSize = sizeof(algorithms)/sizeof(algorithms[0]);

int main(void) { 
    int isExit = 0;
    while(!isExit) {
        clearScreen();
        displayHeader();

        // Prompt for N
        printf("\n");
        int numOfIntegers = (int)getPositiveInteger("Number of integers (N): ");

        // Print data generation methods
        char *dataGenerationMethods[] = {
            "Random integers",
            "Increasing Sequence"
        };
        int dataGenerationMethodsSize = sizeof(dataGenerationMethods)/sizeof(dataGenerationMethods[0]);
        printf("Data Generation Method:");
        for(int i = 0; i < dataGenerationMethodsSize; i++) {
            printf("\n\t%d) %s", i+1, dataGenerationMethods[i]);
        }

        // Get data generation method
        int selectedGenerationMethod = 0;
        printf("\nPlease choose a method: ");
        scanf("%d", &selectedGenerationMethod);

        unsigned long int *unsortedArr;
        unsigned long int startingValue = 0;
        char *unsortedArrayOutputFile = "unsorted.txt";

        // After every run, clear the unsorted output file
        clearFile(unsortedArrayOutputFile);
        appendStringToFile(unsortedArrayOutputFile, "\nUnsorted Array | Number of elements (N): %d\n", numOfIntegers);

        switch (selectedGenerationMethod) {
            case 1:
                // Clear the terminal as data about the current algorithm will be displayed
                clearScreen();
                displayHeader();

                // Generate the data
                printf("\nGenerating data...");
                unsortedArr = generateRandomIntegers(numOfIntegers);

                // Output the data to a file
                appendArrayToFile(unsortedArrayOutputFile, unsortedArr, numOfIntegers);

                // Run benchmarks on the data
                runBenchmark(unsortedArr, numOfIntegers);
                free(unsortedArr);
                break;
            case 2:
                // Verify that the starting value (X) is a positive integer
                startingValue = getPositiveInteger("What is the starting value? (X): ");

                // Clear the terminal as data about the current algorithm will be displayed
                clearScreen();
                displayHeader();

                // Generate the data
                printf("\nGenerating data...");
                unsortedArr = generateIncreasingSequence(numOfIntegers, startingValue);

                // Output the data to a file
                appendArrayToFile(unsortedArrayOutputFile, unsortedArr, numOfIntegers);

                // Run benchmarks on the data
                runBenchmark(unsortedArr, numOfIntegers);
                free(unsortedArr);
                break;
            default:
                break;
        }

        // Sort the algortihms array by time 
        SortingAlgorithm *algorithmsCopy = duplicateAlgorithmsArray(algorithms, algorithmsSize);
        qsort(algorithmsCopy, algorithmsSize, sizeof(SortingAlgorithm), compareByTime);

        // Clear the terminal as the benchmark results will be displayed
        clearScreen();
        displayHeader();

        // Data generation information
        printf("\nNumber of Elements (N): %d", numOfIntegers);
        printf("\nGeneration Method: ");
        if (selectedGenerationMethod == 1) {
            printf("Random Integers");
        } else if (selectedGenerationMethod == 2) {
            printf("Increasing Sequence");
        }
        if (selectedGenerationMethod == 2) {
            printf("\nStarting Value (X): %lu", startingValue);
        }
        
        // Algorithm benchamrks
        printf("\n-----------------------------------------------");
        printf("\n| Rank |       Algorithm      |      Time     |");
        printf("\n-----------------------------------------------");
        for(int i = 0; i < algorithmsSize; i++) {
            printf("\n|%5d | %20s | %14.9lf|", i+1, algorithmsCopy[i].name, algorithmsCopy[i].time);
        }
        printf("\n-----------------------------------------------");

        displayConfirmExit();
        free(algorithmsCopy);
    }
    return 0;
}

/*
*
* DATA GENERATION METHODS
*
*/
unsigned long int *generateRandomIntegers(int n) {
    unsigned long int *array = (unsigned long int *)malloc(n * sizeof(unsigned long int));
    
    srand(time(NULL));
    for(int i = 0; i < n; i++) {
        // Generate two 32-bit random numbers
        unsigned long int high = (unsigned long int)rand();
        unsigned long int low = (unsigned long int)rand();
        // Use bitwise shifting and mask to form a larger number
        unsigned long int randomNumber = (((high & 0xFFFF) << 16) | (low & 0xFFFF)) % (MAX_RANGE - 1);
        array[i] = randomNumber;
    }
    return array;
}
unsigned long int *generateIncreasingSequence(int n, unsigned long int startingValue) {
    unsigned long int *array = (unsigned long int *)malloc(n * sizeof(unsigned long int));
    // Starting from the starting value, increment until the whole array is filled
    for(int i = 0; i < n; i++) {
        array[i] = startingValue++;
    }
    return array;
}

void runBenchmark(unsigned long int *array, int n) {
    // The benchmarks result will also be outputted in a csv
    appendStringToFile("results.csv", "\n");

    printf("\nRunning benchmarks...");
    for(int i = 0; i < algorithmsSize; i++) {
        // Duplicate the data for each sorting algorithm
        unsigned long int *arrayCopy = duplicateArray(array, n);

        printf("\n[%s] Sorting in progress...", algorithms[i].name);

        // Start the timer
        double startTime = getTimeInSeconds();
        // Run the sorting algorithm
        algorithms[i].function(arrayCopy, n);
        // End the timer
        double endTime = getTimeInSeconds();

        // Calculate the elapsed time
        algorithms[i].time = endTime - startTime;

        printf("\n[%s] Sorting finished!", algorithms[i].name);
        printf("\n[%s] Time taken: %.9lfsecs", algorithms[i].name, algorithms[i].time);

        // Clear and output the sorted data into a file per algorithm
        clearFile(algorithms[i].outputFile);
        appendStringToFile(algorithms[i].outputFile, "\n%s Sorted Array | Number of elements (N): %d | Time take: %.9lf\n\n",algorithms[i].name, n, algorithms[i].time);
        appendArrayToFile(algorithms[i].outputFile, arrayCopy, n);

        // Append the elapsed time to results.csv
        appendStringToFile("results.csv", "%.9lf,", algorithms[i].time);

        free(arrayCopy);
    }

    printf("\nBenchmarks complete!");
    sleepProgram(3500);
}

/*
*
* SORTING ALGORITHMS
*
*/
void selectionSort(unsigned long int *array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
        }
        // Swap
        unsigned long int temp = array[i];
        array[i] = array[minIndex];
        array[minIndex] = temp;
    }
}
void bubbleSort(unsigned long int *array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                // Swap
                unsigned long int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
                swapped = 1;
            }
        }
        // Stop if already sorted
        if (!swapped) break;
    }
}
void insertionSort(unsigned long int *array, int n) {
    for (int i = 1; i < n; i++) {
        // Store the current element
        unsigned long int key = array[i];
        int j = i - 1;

        // Shift elements greater than key to the right
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }

        // Insert key at the correct position
        array[j + 1] = key;
    }
}
void merge(unsigned long int *array, int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    // Allocate memory dynamically
    unsigned long int *leftArray = (unsigned long int *)malloc(leftSize * sizeof(unsigned long int));
    unsigned long int *rightArray = (unsigned long int *)malloc(rightSize * sizeof(unsigned long int));

    // Check for memory allocation failure
    if (leftArray == NULL || rightArray == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Copy data to temporary arrays
    for (int i = 0; i < leftSize; i++) {
        leftArray[i] = array[left + i];
    }
    for (int j = 0; j < rightSize; j++) {
        rightArray[j] = array[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    // Merge the temporary arrays back into the main array
    while (i < leftSize && j < rightSize) {
        if (leftArray[i] <= rightArray[j]) {
            array[k++] = leftArray[i++];
        } else {
            array[k++] = rightArray[j++];
        }
    }

    // Copy remaining elements of leftArray
    while (i < leftSize) {
        array[k++] = leftArray[i++];
    }
    // Copy remaining elements of rightArray
    while (j < rightSize) {
        array[k++] = rightArray[j++];
    }

    // Free dynamically allocated memory
    free(leftArray);
    free(rightArray);
}
void mergeSortHelper(unsigned long int *array, int left, int right) {
    if (left < right) {
        // Find the middle index
        int mid = left + (right - left) / 2;

        // Recursively sort first and second halves
        mergeSortHelper(array, left, mid);
        mergeSortHelper(array, mid + 1, right);

        // Merge the sorted halves
        merge(array, left, mid, right);
    }
}
void mergeSort(unsigned long int *array, int n) {
    mergeSortHelper(array, 0, n - 1);
}
int medianOfThree(unsigned long int *array, int low, int high) {
    int mid = low + (high - low) / 2;
    
    // Arrange low, mid, high in sorted order
    if (array[low] > array[mid]) {
        unsigned long int temp = array[low];
        array[low] = array[mid];
        array[mid] = temp;
    }
    if (array[low] > array[high]) {
        unsigned long int temp = array[low];
        array[low] = array[high];
        array[high] = temp;
    }
    if (array[mid] > array[high]) {
        unsigned long int temp = array[mid];
        array[mid] = array[high];
        array[high] = temp;
    }
    
    // Median is now at mid, swap it with high so partitioning remains the same
    unsigned long int temp = array[mid];
    array[mid] = array[high];
    array[high] = temp;
    
    return array[high]; // Pivot value
}
int quickSortPartition(unsigned long int *array, int low, int high) {
    // Choose the median-of-three as pivot
    unsigned long int pivot = medianOfThree(array, low, high);
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // If current element is smaller than the pivot, swap
        if (array[j] < pivot) {
            i++;
            unsigned long int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }

    // Place pivot at its correct position
    unsigned long int temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;

    return i + 1;
}
void quickSortHelper(unsigned long int *array, int low, int high) {
    if (low < high) {
        // Partition the array and get the pivot index
        int pivotIndex = quickSortPartition(array, low, high);
        // Recursively sort elements before and after partition
        quickSortHelper(array, low, pivotIndex - 1);
        quickSortHelper(array, pivotIndex + 1, high);
    }
}
void quickSort(unsigned long int *array, int n) {
    quickSortHelper(array, 0, n - 1);
}
void heapify(unsigned long int *array, int n, int root) {
    int largest = root;
    int leftChild = 2 * root + 1;
    int rightChild = 2 * root + 2;

    // Check if left child is larger than root
    if (leftChild < n && array[leftChild] > array[largest]) {
        largest = leftChild;
    }

    // Check if right child is larger than the largest so far
    if (rightChild < n && array[rightChild] > array[largest]) {
        largest = rightChild;
    }

    // Swap and continue heapifying if root is not the largest
    if (largest != root) {
        unsigned long int temp = array[root];
        array[root] = array[largest];
        array[largest] = temp;

        heapify(array, n, largest);
    }
}
void heapSort(unsigned long int *array, int n) {
    // Build a max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(array, n, i);
    }

    // Extract elements one by one from heap
    for (int i = n - 1; i > 0; i--) {
        // Swap root (largest) with the last element
        unsigned long int temp = array[0];
        array[0] = array[i];
        array[i] = temp;

        // Heapify the reduced heap
        heapify(array, i, 0);
    }
}

/*
*
* UTILITIES
*
*/
void clearFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    fclose(file);
}
void appendStringToFile(const char *filename, const char *format, ...) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fclose(file);
}
void appendArrayToFile(const char *filename, unsigned long int *array, int n) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%lu", array[i]);
        if (i < n - 1) {
            fprintf(file, ",");
        }
        if ((i+1) % 20 == 0) {
            fprintf(file, "\n");
        }
    }
    fprintf(file, "\n");

    fclose(file);
}
unsigned long int *duplicateArray(const unsigned long int *array, int n) {
    unsigned long int *copy = malloc(n * sizeof(unsigned long int));
    if (copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        copy[i] = array[i];
    }
    return copy;
}
SortingAlgorithm *duplicateAlgorithmsArray(SortingAlgorithm *array, int n) {
    SortingAlgorithm *copy = malloc(n * sizeof(SortingAlgorithm));
    if (copy == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        copy[i] = array[i];
    }
    return copy;
}
unsigned long int getPositiveInteger(const char *prompt) {
    while (1) {
        long long int temp;

        printf("%s", prompt);
        if (scanf("%lld", &temp) != 1) {  
            // Invalid input: Non-numeric
            printf("Invalid input. Please enter a non-negative number\n");
            while (getchar() != '\n');
            continue;
        }

        // Invalid input: Negative integer
        if (temp < 0) {
            printf("Negative integers are not allowed. Try again.\n");
        } else {
            return (unsigned long int)temp;
        }
    }
}
double getTimeInSeconds(void) {
    #ifdef _WIN32
        LARGE_INTEGER frequency, start;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);
        return (double)start.QuadPart / (double)frequency.QuadPart;
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (double)ts.tv_sec + ((double)ts.tv_nsec / 1e9);
    #endif
}
int compareByTime(const void *a, const void *b) {
    SortingAlgorithm *algorithmA = (SortingAlgorithm *)a;
    SortingAlgorithm *algorithmB = (SortingAlgorithm *)b;

    // Compare the times (ascending order)
    if (algorithmA->time < algorithmB->time) return -1;
    if (algorithmA->time > algorithmB->time) return 1;
    return 0;
}
void clearScreen(void) {
    #ifdef _WIN32
	    system("cls");
    #else
	    printf("\033[2J");
	    printf("\033[H");
    #endif
}
void displayHeader(void) {
    printf("\n-----------------------------------------------");
    printf("\n           Sorting Algorithm Analysis");
    printf("\n-----------------------------------------------");
}
void displayConfirmExit(void) {
    char response;
    do {
        printf("\nEnter Y/y to exit: ");
        scanf(" %c", &response);
        response = toupper(response);
        if (response != 'Y') {
            while (getchar() != '\n');
        }
    } while(response != 'Y');
}
void sleepProgram(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}