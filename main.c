/*
 * =============================================
 *          SPELL CHECKER in C
 *  Algorithms Used:
 *    1. KMP String Matching  -> exact word search
 *    2. Edit Distance (DP)   -> find closest word
 * =============================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORDS     60000
#define MAX_WORD_LEN  50

/* ──────────────────────────────────────────
   STEP 1: Load dictionary from file
   ────────────────────────────────────────── */
char dictionary[60000][50];
int  dictSize = 0;

void loadDictionary(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open dictionary.txt\n");
        exit(1);
    }
    while (fscanf(fp, "%s", dictionary[dictSize]) == 1) {
        dictSize++;
        if (dictSize >= MAX_WORDS) break;
    }
    fclose(fp);
    printf("Dictionary loaded: %d words\n\n", dictSize);
}

/* ──────────────────────────────────────────
   ALGORITHM 1: KMP String Matching
   Used to: check if typed word EXISTS in dictionary
   How: builds a "failure array" to skip
        unnecessary comparisons (faster than brute force)
   ────────────────────────────────────────── */

// Build the KMP failure array (also called LPS array)
void buildFailureArray(char *pattern, int patLen, int *failure) {
    failure[0] = 0;
    int i = 1, len = 0;
    while (i < patLen) {
        if (pattern[i] == pattern[len]) {
            len++;
            failure[i] = len;
            i++;
        } else {
            if (len != 0)
                len = failure[len - 1];
            else {
                failure[i] = 0;
                i++;
            }
        }
    }
}

// KMP Search: returns 1 if pattern found in text, 0 if not
int kmpSearch(char *text, char *pattern) {
    int tLen = strlen(text);
    int pLen = strlen(pattern);

    int failure[MAX_WORD_LEN];
    buildFailureArray(pattern, pLen, failure);

    int i = 0, j = 0;
    while (i < tLen) {
        if (text[i] == pattern[j]) {
            i++; j++;
        }
        if (j == pLen) {
            return 1; // MATCH FOUND
        } else if (i < tLen && text[i] != pattern[j]) {
            if (j != 0)
                j = failure[j - 1];
            else
                i++;
        }
    }
    return 0; // NOT FOUND
}

// Check if word exists in full dictionary using KMP
int isInDictionary(char *word) {
    for (int i = 0; i < dictSize; i++) {
        // We use KMP to match word against each dictionary entry
        if (kmpSearch(dictionary[i], word) &&
            strlen(dictionary[i]) == strlen(word)) {
            return 1;
        }
    }
    return 0;
}

/* ──────────────────────────────────────────
   ALGORITHM 2: Edit Distance (Dynamic Programming)
   Used to: find the closest correct word
   How: counts min insertions/deletions/replacements
        needed to convert wrong word -> correct word
   Lower score = more similar words
   ────────────────────────────────────────── */

int minOf3(int a, int b, int c) {
    if (a < b) return (a < c) ? a : c;
    return (b < c) ? b : c;
}

int editDistance(char *s1, char *s2) {
    int m = strlen(s1);
    int n = strlen(s2);

    // Create DP table
    int dp[MAX_WORD_LEN][MAX_WORD_LEN];

    // Base cases: empty string conversions
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    // Fill the DP table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                // Characters match — no operation needed
                dp[i][j] = dp[i-1][j-1];
            } else {
                // Pick minimum of: delete, insert, replace
                dp[i][j] = 1 + minOf3(
                    dp[i-1][j],    // delete from s1
                    dp[i][j-1],    // insert into s1
                    dp[i-1][j-1]   // replace character
                );
            }
        }
    }

    return dp[m][n]; // final answer
}

// Find best matching word in dictionary
void suggestCorrection(char *word) {
    int  bestDist  = 9999;
    char bestWord[MAX_WORD_LEN];

    for (int i = 0; i < dictSize; i++) {
        int dist = editDistance(word, dictionary[i]);
        if (dist < bestDist) {
            bestDist = dist;
            strcpy(bestWord, dictionary[i]);
        }
    }

    if (bestDist <= 3) {
        printf("   Did you mean: \"%s\"? (edit distance: %d)\n",
               bestWord, bestDist);
    } else {
        printf("   No close match found in dictionary.\n");
    }
}

/* ──────────────────────────────────────────
   MAIN: User interaction loop
   ────────────────────────────────────────── */

int main() {
    printf("_________________________________\n");
    printf("       SPELL CHECKER v1.0       \n");
    printf("  Algorithms: KMP + Edit Distance (DP)\n");
    printf("__________________________________\n\n");

    loadDictionary("dictionary.txt");

    char input[MAX_WORD_LEN];

    while (1) {
        printf("Enter word (or 'quit' to exit): ");
        scanf("%s", input);

        if (strcmp(input, "quit") == 0) {
            printf("\nGoodbye!\n");
            break;
        }

        printf("\n");

        // ALGORITHM 1: KMP checks if word is correct
        if (isInDictionary(input)) {
            printf("   [KMP]  \"%s\" found in dictionary.\n", input);
            printf("   Result: CORRECT SPELLING!\n");
        } else {
            printf("   [KMP]  \"%s\" NOT found in dictionary.\n", input);
            printf("   [DP]   Running Edit Distance to find closest word...\n");
            // ALGORITHM 2: DP finds best suggestion
            suggestCorrection(input);
        }

        printf("\n");
    }

    return 0;
}