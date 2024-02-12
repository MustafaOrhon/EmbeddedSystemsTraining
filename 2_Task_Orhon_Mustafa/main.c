/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/* DEFINES */
#define MAX_LENGTH 30
/* TYPEDEFS */

/* FUNCTION DECLARATIONS */
void ClearInputBuffer(void);
void GetInputString(char *destination, int maxLength);
void MemcpyDemo(void);
void MemmoveDemo(void);
void StrcpyDemo(void);
void StrncpyDemo(void);
void StrcatDemo(void);
void StrncatDemo(void);
void MemcmpDemo(void);
void StrcmpDemo(void);
void StrncmpDemo(void);
void MemchrDemo(void);
void StrchrDemo(void);
void StrcspnDemo(void);
void StrpbrkDemo(void);
void StrrchrDemo(void);
void StrstrDemo(void);
void StrtokDemo(void);
void MemsetDemo(void);
void StrlenDemo(void);
void PrintfDemo(void);
void FPrintfDemo(void);
void SscanfDemo(void);
void AtofDemo(void);
void AtoiDemo(void);
void AtolDemo(void);
void IsdigitDemo(void);
void IsalnumDemo(void);
void IsalphaDemo(void);
void IsupperDemo(void);

int main() {


    MemcpyDemo();
    MemmoveDemo();
    StrcpyDemo();
    StrncpyDemo();
    StrcatDemo();
    StrncatDemo();
    MemcmpDemo();
    StrcmpDemo();
    StrncmpDemo();
    MemchrDemo();
    StrchrDemo();
    StrcspnDemo();
    StrpbrkDemo();
    StrrchrDemo();
    StrstrDemo();
    StrtokDemo();
    MemsetDemo();
    StrlenDemo();
    PrintfDemo();
    FPrintfDemo();
    SscanfDemo();
    AtofDemo();
    AtoiDemo();
    AtolDemo();
    IsdigitDemo();
    IsalnumDemo();
    IsalphaDemo();
    IsupperDemo();


    return 0;
}


/* FUNCTION DEFINITIONS */
void ClearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void GetInputString(char *destination, int maxLength) {
    fgets(destination, maxLength, stdin);
    // Remove newline character if present
    size_t len = strlen(destination);
    if (len > 0 && destination[len - 1] == '\n') {
        destination[len - 1] = '\0';
    } else {
        // Clear input buffer if it exceeds the maximum length
        ClearInputBuffer();
    }
}
void MemcpyDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    int copy_size = 0;
    char *return_val = NULL;

    // Using memcpy to copy string with specified size
    printf("Function: memcpy\n");
    printf("Please enter the string that you want to copy:");
    GetInputString(input_string, MAX_LENGTH);
    printf("Please enter how many bytes you want to copy:");
    scanf("%d", &copy_size);
    ClearInputBuffer();
    printf("Parameters: user_string='%s', destination_string='%s', copy_size=%d\n", input_string, destination_string, copy_size);

    return_val = (char *)memcpy((void *)destination_string, (void *)input_string, copy_size);
    printf("Resulted string: \"%s\"\nReturn value of memcpy: %p\n\n", destination_string, return_val);

}
void MemmoveDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    int copy_size = 0;
    char *return_val = NULL;

    // Using memmove to copy string with specified size
    printf("Function: memmove\n");
    printf("Please enter the string that you want to copy:");
    GetInputString(input_string, MAX_LENGTH);

    printf("Please enter how many bytes you want to copy:");
    scanf("%d", &copy_size);
    ClearInputBuffer();
    printf("Parameters: user_string='%s', destination_string='%s', copy_size=%d\n", input_string, destination_string, copy_size);

    return_val = (char *)memmove((void *)destination_string, (void *)input_string, copy_size);
    printf("Resulted string: \"%s\"\nReturn value of memmove: %p\n\n", destination_string, return_val);

}
void StrcpyDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    char *return_val = NULL;
    // Using strcpy to copy string
    printf("Function: strcpy\n");
    printf("Please enter the source string that you want to copy:");
    GetInputString(input_string, MAX_LENGTH);
    printf("Parameters: user_string='%s', destination_string='%s'\n", input_string, destination_string);

    return_val = (char *)strcpy((void *)destination_string, (void *)input_string);
    printf("Resulted string: \"%s\"\nReturn value of strcpy: %p\n\n", destination_string, return_val);

}
void StrncpyDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    int copy_size = 0;
    char *return_val = NULL;
    // Using strncpy to copy string with specified size
    printf("Function: strncpy\n");
    printf("Please enter the source string that you want to copy:");
    GetInputString(input_string, MAX_LENGTH);
    printf("Please enter a example destination string that you want to copy:");
    GetInputString(destination_string, MAX_LENGTH);
    printf("Please enter how many bytes you want to copy:");
    scanf("%d", &copy_size);
    ClearInputBuffer();
    printf("Parameters: user_string='%s', destination_string='%s', copy_size=%d\n", input_string, destination_string, copy_size);

    return_val = (char *)strncpy((void *)destination_string, (void *)input_string, copy_size);
    printf("Resulted string: \"%s\"\nReturn value of strncpy: %p\n\n", destination_string, return_val);


}
void StrcatDemo(void) {   // Using strcat to concatenate strings
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    char *return_val = NULL;
    printf("Function: strcat\n");
    printf("Please enter the source string that you want to concatenate:");
    GetInputString(input_string, MAX_LENGTH);
    printf("Please enter a example destination string that you want to be concatenated:");
    GetInputString(destination_string, MAX_LENGTH);
    printf("Parameters: user_string='%s', destination_string='%s'\n", input_string, destination_string);

    return_val = (char *)strcat((void *)destination_string, (void *)input_string);
    printf("Resulted string in destination: \"%s\"\nReturn value of strcat: %p\n\n", destination_string, return_val);


}
void StrncatDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    int copy_size = 0;
    char *return_val = NULL;

    // Using strncat to concatenate strings with specified size
    printf("Function: strncat\n");
    printf("Please enter the source string that you want to concatenate:");
    GetInputString(input_string, MAX_LENGTH);
    printf("Please enter a example destination string that you want to be concatenated:");
    GetInputString(destination_string, MAX_LENGTH);
    printf("Please enter how many bytes you want to concatenate:");
    scanf("%d", &copy_size);
    ClearInputBuffer();
    printf("Parameters: user_string='%s', destination_string='%s', copy_size=%d\n", input_string, destination_string, copy_size);

    return_val = (char *)strncat((void *)destination_string, (void *)input_string, copy_size);
    printf("Resulted string in destination: \"%s\"\nReturn value of strncat: %p\n\n", destination_string, return_val);

}
void MemcmpDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    int copy_size = 0;
    int result = 0;
    // Using memcmp to compare strings with specified size
    printf("Function: memcmp\n");
    printf("Please enter the first string that you want to compare:");
    GetInputString(input_string, MAX_LENGTH);
    printf("Please enter the second string that you want to compare:");
    GetInputString(destination_string, MAX_LENGTH);
    printf("Please enter how many characters you want to compare:");
    scanf("%d", &copy_size);
    ClearInputBuffer();
    printf("Parameters: user_string='%s', destination_string='%s',  copy_size=%d\n", input_string, destination_string,copy_size);

    result = memcmp(input_string, destination_string, copy_size);
    printf("Output: ");
    if (result > 0) {
        printf("'%s' is greater than '%s'.\n", input_string, destination_string);
    } else if (result < 0) {
        printf("'%s' is less than '%s'.\n", input_string, destination_string);
    } else {
        printf("The first %d characters of '%s' are the same as '%s'.\n", copy_size, input_string, destination_string);
    }

}
void StrcmpDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    int result = 0;
    // Using strcmp to compare strings with specified size
    printf("Function: strcmp\n");
    printf("Please enter the first string that you want to compare:");
    GetInputString(input_string, MAX_LENGTH);
    printf("Please enter the second string that you want to compare:");
    GetInputString(destination_string, MAX_LENGTH);
    printf("Parameters: user_string='%s', destination_string='%s'\n", input_string, destination_string);

    result = strcmp(input_string,destination_string);
    printf("Output: ");
    if (result > 0) {
        printf("'%s' is greater than '%s'.\n", input_string, destination_string);
    } else if (result < 0) {
        printf("'%s' is less than '%s'.\n", input_string, destination_string);
    } else {
        printf("The '%s' is the same as '%s'.\n", input_string, destination_string);
    }


}
void StrncmpDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    int copy_size = 0;
    int result = 0;
    // Using strncmp to compare strings with specified size
    printf("Function: strncmp\n");
    printf("Please enter the first string that you want to compare:");
    GetInputString(input_string, MAX_LENGTH);
    printf("Please enter the second string that you want to compare:");
    GetInputString(destination_string, MAX_LENGTH);
    printf("Please enter how many characters you want to compare:");
    scanf("%d", &copy_size);
    ClearInputBuffer();
    printf("Parameters: user_string='%s', destination_string='%s',  copy_size=%d\n", input_string, destination_string,copy_size);

    result = strncmp(input_string, destination_string, copy_size);
    printf("Output: ");
    if (result > 0) {
        printf("'%s' is greater than '%s'.\n", input_string, destination_string);
    } else if (result < 0) {
        printf("'%s' is less than '%s'.\n", input_string, destination_string);
    } else {
        printf("The first %d characters of '%s' are the same as '%s'.\n", copy_size, input_string, destination_string);
    }

}
void MemchrDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    int copy_size = 0;
    int result = 0;
    // Using memchr to search for a character within a string
    printf("Function: memchr\n");
    printf("Please enter the string in which you want to search:");
    GetInputString(input_string, MAX_LENGTH);
    printf("Please enter the character you want to search for:");
    GetInputString(destination_string, 2); // We only need a single character
    printf("Please enter how many characters you want to search:");
    scanf("%d", &copy_size);
    ClearInputBuffer();
    printf("Parameters: user_string='%s', character_string='%c',search_size=%d\n", input_string, destination_string[0],copy_size);
    // Using memchr to search for the character
    char *ptr = memchr(input_string, destination_string[0], copy_size);
    if (ptr != NULL) {
        result = ptr - input_string + 1;
        printf("Output: The character '%c' is found at position %d in the string '%s'.\n", destination_string[0], result, input_string);
    } else {
        printf("Output: The character '%c' is not found in the string '%s'.\n", destination_string[0], input_string);
    }

}
void StrchrDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    char *result = NULL;

    printf("Function: strchr\n");
    printf("Please enter the string in which you want to search:");
    GetInputString(input_string, MAX_LENGTH);
    printf("Please enter the character you want to search for:");
    GetInputString(destination_string, 2); // We only need a single character
    printf("Parameters: user_string='%s', character='%c'\n", input_string, destination_string[0]);

    // Using strchr to search for a character within a string
    result = strchr(input_string, destination_string[0]);
    if (result != NULL) {
        int position = result - input_string + 1;
        printf("Output: The character '%c' is found at position %d in the string '%s'.\n", destination_string[0], position, input_string);
    } else {
        printf("Output: The character '%c' is not found in the string '%s'.\n", destination_string[0], input_string);
    }
}
void StrcspnDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    printf("Function: strcspn\n");
    printf("Please enter the string in which you want to search:");
    GetInputString(input_string, MAX_LENGTH);

    printf("Please enter the characters you want to reject:");
    GetInputString(destination_string, MAX_LENGTH);

    // Using strcspn to find the length of the initial segment of user_string that does not contain any character from destination_string
    printf("Parameters: user_string='%s', reject_characters='%s'\n", input_string, destination_string);

    size_t length = strcspn(input_string, destination_string);



}
void StrpbrkDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    char *return_val = NULL;
    printf("Please enter the string in which you want to search:");
    GetInputString(input_string, MAX_LENGTH);

    printf("Please enter the characters you want to search for:");
    GetInputString(destination_string, MAX_LENGTH);

    // Using strpbrk to find the first occurrence of any character from destination_string in user_string
    printf("Function: strpbrk\n");
    printf("Parameters: user_string='%s', search_characters='%s'\n", input_string, destination_string);

    return_val = strpbrk(input_string, destination_string);
    if (return_val != NULL) {
        printf("Output: The first occurrence of any character from '%s' in '%s' is at position %d.\n", destination_string, input_string, return_val - input_string + 1);
    } else {
        printf("Output: None of the characters from '%s' are found in '%s'.\n", destination_string, input_string);
    }


}
void StrrchrDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    char *return_val = NULL;
    printf("Please enter the string in which you want to search:");
    GetInputString(input_string, MAX_LENGTH);

    printf("Please enter the character you want to search for:");
    GetInputString(destination_string, 2); // We only need a single character

    // Using strrchr to find the last occurrence of the character in the string
    printf("Function: strrchr\n");
    printf("Parameters: user_string='%s', search_character='%c'\n", input_string, destination_string[0]);

    return_val = strrchr(input_string, destination_string[0]);
    if (return_val != NULL) {
        printf("Output: The last occurrence of '%c' in '%s' is at position %d.\n", destination_string[0], input_string, return_val - input_string + 1);
    } else {
        printf("Output: The character '%c' is not found in '%s'.\n", destination_string[0], input_string);
    }

}
void StrstrDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    char *return_val = NULL;
    printf("Function: strstr\n");
    printf("Please enter the main string:");
    GetInputString(input_string, MAX_LENGTH);

    printf("Please enter the substring you want to search for:");
    GetInputString(destination_string, MAX_LENGTH);

    // Using strstr to find the first occurrence of the substring in the main string
    printf("Function: strstr\n");
    printf("Parameters: main_string='%s', substring='%s'\n", input_string, destination_string);

    return_val = strstr(input_string, destination_string);
    if (return_val != NULL) {
        printf("Output: The substring '%s' is found in the main string '%s' at position %d.\n", destination_string, input_string, return_val - input_string + 1);
    } else {
        printf("Output: The substring '%s' is not found in the main string '%s'.\n", destination_string, input_string);
    }

}
void StrtokDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    char destination_string[MAX_LENGTH] = {0};
    // Prompting for user input
    printf("Please enter the string to tokenize:");
    GetInputString(input_string, MAX_LENGTH);

    printf("Please enter the delimiters:");
    GetInputString(destination_string, MAX_LENGTH);

    // Using strtok to tokenize the input string
    printf("Function: strtok\n");
    printf("Parameters: input_string='%s', delimiter='%s'\n", input_string, destination_string);

    // Using strtok to tokenize the input string
    char *token = strtok(input_string, destination_string);
    int token_index = 1;

    while (token != NULL) {
        printf("Output: Token %d: %s\n", token_index++, token);
        token = strtok(NULL, destination_string);
    }

}
void MemsetDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    int copy_size = 0;
    int result = 0;

    // Prompting for user input
    printf("Please enter the value that you want to fill of the array:");
    scanf("%d", &result);

    printf("Please enter how many value that you want to fill:");
    scanf("%d", &copy_size);

    if(copy_size > 30) {
        printf("To prevent exceeding max lenght copy size reduced to 30.\n");
        copy_size = MAX_LENGTH;
    }

    printf("Function: memset\n");
    printf("Parameters: array_address=%p, value=%d, size=%d\n", (void *)input_string, result, copy_size);
    // Initializing the array with the specified value
    memset(input_string, result,copy_size);
    // Printing the initialized array
    printf("Output: The array after memset:\n");
    for (int i = 0; i < (MAX_LENGTH-1); ++i) {
        printf("%d ", input_string[i]);
    }
}
void StrlenDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    // Prompting for user input
    printf("Please enter a string:");
    GetInputString(input_string, MAX_LENGTH);

    // Printing the calculated length directly
    printf("Function: strlen\n");
    printf("Parameters: input_string='%s'\n", input_string);
    printf("Output: The length of the string '%s' is %lu.\n", input_string, (unsigned long)strlen(input_string));

}
void PrintfDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    int result = 0;
    // Using printf to display the values of variables
    printf("Please enter a string:");
    GetInputString(input_string, MAX_LENGTH);
    result = printf("%s\n",input_string);
    printf("Function: printf\n");
    printf("Parameters: const char* format='%s'\n", input_string);
    printf("Return value of printf = %d with printed string %s + new line", result,input_string);

}
void FPrintfDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    int result = 0;
    printf("Please enter a string:");
    GetInputString(input_string, MAX_LENGTH);
    result = fprintf(stdout, "%s\n",input_string);
    // Using fprintf to print to stdout (console)
    fprintf(stdout, "Function: fprintf\n");
    fprintf(stdout, "Parameters: const char* format='%s'\n", input_string);
    fprintf(stdout, "Output: %s\n", input_string);

    // Printing the result of fprintf
    fprintf(stdout, "Return value of fprintf = %d with printed string '%s' + new line\n",result, input_string);

}
void SscanfDemo(void) {

    char input_string[] = "42 24";
    int integer_variables[2];

    // Using sscanf to extract two integer values from a string
    int sscanf_result = sscanf(input_string, "%d %d",&integer_variables[0],&integer_variables[1]);

    // Printing the input string and extracted values
    printf("Function: sscanf\n");
    printf("Parameters: const char* str='%s', const char* format='%s'\n", input_string, "%d %d");
    printf("Input string: '%s'\n", input_string);
    printf("Extracted integer values: %d, %d\n", integer_variables[0], integer_variables[1]);
    printf("Return value of sscanf = %d\n", sscanf_result);
}
void AtofDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    // Asking the user to input a string
    printf("Please enter a floating-point number string: ");
    GetInputString(input_string,MAX_LENGTH);

    // Using atof to convert the user input string to a double
    float result_f = atof(input_string);

    // Printing the input string and converted double value
    printf("Function: atof\n");
    printf("Parameters: const char* str='%s'\n", input_string);
    printf("Input string: '%s'\n", input_string);
    printf("Converted double value: %.2f\n", result_f);

}
void AtoiDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    int result = 0;

    printf("Please enter an integer string: ");
    GetInputString(input_string,MAX_LENGTH);


    // Using atoi to convert the user input string to an integer
    result = atoi(input_string);

    // Printing the input string and converted integer value
    printf("Function: atoi\n");
    printf("Parameters: const char* str='%s'\n", input_string);
    printf("Input string: '%s'\n", input_string);
    printf("Converted integer value: %d\n", result);

}
void AtolDemo(void) {
    char input_string[MAX_LENGTH] = {0};
    printf("Please enter a long integer string: ");
    GetInputString(input_string,MAX_LENGTH);


    // Using atol to convert the user input string to a long integer
    long result_l = atol(input_string);

    // Printing the input string and converted long integer value
    printf("Function: atol\n");
    printf("Parameters: const char* str='%s'\n", input_string);
    printf("Input string: '%s'\n", input_string);
    printf("Converted long integer value: %ld\n", result_l);

}
void IsdigitDemo(void) {
    char input_string[MAX_LENGTH] = {0};

    printf("Please enter a character: ");
    GetInputString(input_string, MAX_LENGTH);

    // Using isdigit to check if the input character is a digit
    int is_digit_result = isdigit((int)input_string[0]);

    // Printing the input character and the result of isdigit
    printf("Function: isdigit\n");
    printf("Parameters: int c='%c'\n", input_string[0]);
    if (is_digit_result)
        printf("The input character '%c' is a digit.\n", input_string[0]);
    else
        printf("The input character '%c' is not a digit.\n", input_string[0]);

}
void IsalnumDemo(void) {
    char input_string[MAX_LENGTH] = {0};

    printf("Please enter a character: ");
    GetInputString(input_string, MAX_LENGTH);

    // Using isalnum to check if the input character is an alphanumeric character
    int is_alnum_result = isalnum((int)input_string[0]);

    // Printing the input character and the result of isalnum
    printf("Function: isalnum\n");
    printf("Parameters: int c='%c'\n", input_string[0]);
    if (is_alnum_result)
        printf("The input character '%c' is alphanumeric.\n", input_string[0]);
    else
        printf("The input character '%c' is not alphanumeric.\n", input_string[0]);

}
void IsalphaDemo(void) {
    char input_string[MAX_LENGTH] = {0};

    printf("Please enter a character: ");
    GetInputString(input_string, MAX_LENGTH);

    // Using isalpha to check if the input character is an alphabetic character
    int is_alpha_result = isalpha((int)input_string[0]);

    // Printing the input character and the result of isalpha
    printf("Function: isalpha\n");
    printf("Parameters: int c='%c'\n", input_string[0]);
    if (is_alpha_result)
        printf("The input character '%c' is alphabetic.\n", input_string[0]);
    else
        printf("The input character '%c' is not alphabetic.\n", input_string[0]);
}
void IsupperDemo(void) {
    char input_string[MAX_LENGTH] = {0};

    printf("Please enter a character: ");
    GetInputString(input_string, MAX_LENGTH);

    // Using isupper to check if the input character is uppercase
    int is_upper_result = isupper((int)input_string[0]);

    // Printing the input character and the result of isupper
    printf("Function: isupper\n");
    printf("Parameters: int c='%c'\n", input_string[0]);
    if (is_upper_result)
        printf("The input character '%c' is uppercase.\n", input_string[0]);
    else
        printf("The input character '%c' is not uppercase.\n", input_string[0]);
}
