#include <stdio.h>
#include <stdlib.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";

// lagt till
int list1[20];  // 80 bytes in total, equivalent to 20 integers (4 bytes each)
int list2[20];
int counter = 0;

// lagt till
void copycodes(const char* src, int* dst, int* count) {
    while (*src != '\0') {
        *dst = (int)(*src);  // store the ASCII value of the character
        src++;  // move to the next character in the source string
        dst++;  // move to the next integer in the destination list
        
        (*count)++;  // increment the counter
    }
}

// lagt till
void work(void) {
    copycodes(text1, list1, &counter);
    copycodes(text2, list2, &counter);
}

void printlist(const int* lst) {
    printf("ASCII codes and corresponding characters:\n");
    while (*lst != 0) {
        printf("0x%03X '%c' ", *lst, (char)*lst);
        lst++;
    }
    printf("\n");
}

void endian_proof(const char* c) {
    printf("\nEndian experiment: 0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
           (int)*c, (int)*(c + 1), (int)*(c + 2), (int)*(c + 3));
}

int main(void) {
    work();
    
    printf("\nlist1: ");
    printlist(list1);
    
    printf("\nlist2: ");
    printlist(list2);
    
    printf("\nCount = %d\n", counter);
    
    endian_proof((char*)&counter);
    
    return 0;
}
