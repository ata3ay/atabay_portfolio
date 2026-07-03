//---------------------------------------------------------------------------------------------------------------------
/// a3.c
/// Stringtango
///
/// Author: 12046429 Atabay Dinmukhamed
//---------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//---------------------------------------------------------------------------------------------------------------------
/// @brief enum values for returning
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
enum RETURN_VALUES
{
  QUIT = 0,
  QUIT_FULL = 2,
  ERROR_MEMORY_ALLOCCATION = 1,
  ERROR_UKNOWN_COMMAND = -2,
  ERROR_STRING_CANT_BE_EMPTY = -3,
  ERROR_SUBSTRING_NOT_FOUND = -4,
  ERROR_NO_WORDS_TO_SORT = -5,
  ERROR_ALL_WORDS_UNIQUE = -6,
  APPEND_TEXT = 3,
  SEARCH_REPLACE = 4,
  SPLIT_SORT = 5,
  UNIQUE = 6,
  SUCCESS = 7,
  FAIL = 8
};

//---------------------------------------------------------------------------------------------------------------------
/// @brief prints the welcome message at the beginning of the program
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
void welcomeMessage(void)
{
  printf("\nWelcome to the\n");
  printf("Stringtango Text Editor!\n\n");
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief asks a user to enter a text to the program
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
void enterTextPrompt(void)
{
  printf("Enter a text:\n");
  printf(" > ");
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief menu of commands after the user writes the text
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
void editText(void)
{
  printf("\nChoose a command:\n");
  printf(" a: append text\n");
  printf(" r: search and replace\n");
  printf(" s: split and sort\n");
  printf(" u: unique\n");
  printf(" q: quit\n");
  printf("\n > ");
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief prints the corresponding error message according to the enum value
/// @param error_code that is where the enum value goes
///
/// 
//---------------------------------------------------------------------------------------------------------------------
void printError(int error_code)
{
  if(error_code == ERROR_STRING_CANT_BE_EMPTY)
  {
    printf("[ERROR] String can't be empty!\n");
  }
  if(error_code == ERROR_UKNOWN_COMMAND)
  {
    printf("[ERROR] Command unknown!\n");
  }
  if(error_code == ERROR_SUBSTRING_NOT_FOUND)
  {
    printf("[ERROR] Substring not found!\n");
  }
  if(error_code == ERROR_NO_WORDS_TO_SORT)
  {
    printf("[ERROR] No words to sort!\n");
  }
  if(error_code == ERROR_ALL_WORDS_UNIQUE)
  {
    printf("[ERROR] All words are already unique\n");
  }
  if(error_code == ERROR_MEMORY_ALLOCCATION)
  {
    printf("[ERROR] Memory allocation failed!\n");
  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief gets input from the user dynamically. 
/// It makes the reallocation to get)input, but before, it checks in temp if it is possible to reallocate it
///
/// @return it returns the exact size of the input string
//---------------------------------------------------------------------------------------------------------------------
char* getInput(void) 
{
  int buffer = 50;
  int index = 0;
  char* get_input = mall oc(buffer * sizeof(char));
  if(get_input == NULL)
  {
    printError(ERROR_MEMORY_ALLOCCATION);
    return NULL;
  }
  char* symbol_now = get_input; // temp variable to scroll through the string
  while (1)
  {
    int ch = getchar();// takes the symbols from the input buffer
    if(ch == EOF)
    {
      if(index == 0)
      {
        free(get_input);
        return NULL;
      }
      else
      {
        break;
      }
    }
    if(ch == '\n') // it ends when the new line appears
    {
      break;
    }
    if(index + 1 >= buffer)
    {
      buffer = buffer * 2;
      char* temp = realloc(get_input, buffer * sizeof(char));/// we need temp for safety and to avoid memory leak
      if(temp == NULL) /// if reallocation fails we lose our pointer
      {
        printf("Reallocation failed\n");
        free(get_input);
        return NULL;
      }
      get_input = temp; ///overwriting after making sure that it is safe to allocate memory
      symbol_now = get_input + index; // 
    }
    *symbol_now++ = ch;// ptr goes to next symbol, postincremeant, first operation
    index++;
  } // symbol_now points to the last character at the moment
  *symbol_now = '\0';///i have to write null terminator, to finish the string
  
  char* temp_exact_size = realloc(get_input, (index + 1) * sizeof(char));
  if(temp_exact_size == NULL)
  {
    return get_input;
  }
  
  return temp_exact_size;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief input validation according to the rules, for letters a,r,s,q,u and also quit 
/// if the user gives something that validation doesnt know, then it gives error enum value
/// @param input from the user 
/// @return it returns quit enum value, or the command values or error value
/// 
//---------------------------------------------------------------------------------------------------------------------
int checkInput(char* input)
{
  int length = strlen(input);

  if(length == 4)
  {
    if(strcmp("quit", input) == 0)
    {
      return QUIT_FULL; //quit -> sofort ; command line q -> current text
    }
  }

  if(length == 1 && input[0] == 'q')
  {
    return QUIT;
  }

  if(input[0] == 'a' && input[1] == '\0')
  {
    return APPEND_TEXT;
  } else if(input[0] == 'r' && input[1] == '\0')
  {
    return SEARCH_REPLACE;
  } else if(input[0] == 's' && input[1] == '\0')
  {
    return SPLIT_SORT;
  } else if(input[0] == 'u' && input[1] == '\0')
  {
    return UNIQUE;
  }
  else 
  {
    return ERROR_UKNOWN_COMMAND;
  }
  
  return 1;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief function for reallocating the input
/// @param input from the user
/// @param size to reallocate
/// @return temp which is reallocated
//---------------------------------------------------------------------------------------------------------------------
char* reallocateInput(char* input, int size_to_realloc) // int size
{
  int length = strlen(input); // hello  + world = 12 (6) length = 6
  int new_length = length + size_to_realloc + 1; // +1 because of null Term
  char* temp = realloc(input, new_length * sizeof(char));
  if(temp == NULL)
  {
    printError(ERROR_MEMORY_ALLOCCATION);
    return NULL;
  }
  // TO DO Pointers, do i work with the original or should i return the new original one?
  return temp; //Success
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief function to append the text into the input string from the user
/// it asks for the text which user wants to append
/// @param input from the user
/// @return it returns corresponding enum value: Error, Quit, or Append text as success
//---------------------------------------------------------------------------------------------------------------------
int appendText(char** input) // Hello World -> world - welt -> 
{
  printf("\nPlease enter the string that should be appended:\n");
  printf(" > ");
  
  char* append_input = getInput();
  if(append_input == NULL)
  {
    printError(ERROR_MEMORY_ALLOCCATION);
    free(append_input);
    return ERROR_MEMORY_ALLOCCATION;
  }
  int append_input_length = strlen(append_input);
  if(append_input_length == 0)
  {
    printError(ERROR_STRING_CANT_BE_EMPTY);
    free(append_input);
    return ERROR_STRING_CANT_BE_EMPTY;
  }
  int check = checkInput(append_input);
  if(check == QUIT || check == QUIT_FULL)
  {
    free(append_input);
    return QUIT;
  }
  char* new_text = reallocateInput(*input, append_input_length);
  if(new_text == NULL)
  {
    free(append_input);
    return ERROR_MEMORY_ALLOCCATION;  
  }
  strcat(new_text, append_input); 
  free(append_input);
  *input = new_text;
  printf("\nCurrent text:\n");
  printf("%s\n", *input);
  return APPEND_TEXT;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief function for finding a substring for the function search and replace
/// it loops for the whole string, and searches for the substring which was inputed from the user
/// @param input from the user, or main string from the user
/// @param old_substring which is the word we are searching for
/// For example: Hello world (world) - world is the old susbtring
/// @return it returns i, the location where exactly the old susbtring in the whole strin is
/// it can also return Error value
//---------------------------------------------------------------------------------------------------------------------
int findSubString(char* input, char* old_substring)
{
  for(int i = 0; input[i] != '\0'; i++) // loop for the whole string of input (hello world)
  {
    if(old_substring[0] == input[i]) // here i search for the first letter of my substring with the input (w)
    {
      int j;
      for(j = 0; old_substring[j] != '\0' && input[i + j] != '\0'; j++)
      {
        if(old_substring[j] != input[i + j])
        {
          break; // if the words are not the same we are searching for
        } // world - w6+1 - 6 + 2
        //hello world // world = 
      }
      //break or null term
      // world != worrd
      if(old_substring[j] == '\0')
      {
        return i;//this is where my old substring is located 
      }
    }
  }
  return ERROR_SUBSTRING_NOT_FOUND;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief finds the substring i need, allocates and reallocates memory for that
/// @param input from the user
/// @param old_substring the word we are searching for
/// @param new_substring the word we want to replace it with
/// @return it returns the number of occurences in the string or enum value with error
//---------------------------------------------------------------------------------------------------------------------
int checkSearchReplace(char** input, char* old_substring, char* new_substring)
{
  int length_string = strlen(*input); // The weather today is very nice!
  int length_old_substring = strlen(old_substring); // nice
  int length_new_substring = strlen(new_substring); // bad
  int occurences = 0;
  int position = 0;
  while(1)
  {
    int index = findSubString(*input + position, old_substring); 
    if(index == ERROR_SUBSTRING_NOT_FOUND || index == ERROR_STRING_CANT_BE_EMPTY)
    {
      break;
    }
    occurences++;

    position += index + length_old_substring; //going further to find next old susbtring
  }
  int new_length = length_string + occurences * (length_new_substring - length_old_substring);

  if(length_old_substring == 0)
  {
    return ERROR_STRING_CANT_BE_EMPTY;
  }

  if(occurences == 0)
  {
    return ERROR_SUBSTRING_NOT_FOUND;
  }

  char* original_to_read = malloc((length_string + 1) * sizeof(char));
  if(original_to_read == NULL)
  {
    printError(ERROR_MEMORY_ALLOCCATION);
    return ERROR_MEMORY_ALLOCCATION;
  }
  strcpy(original_to_read, *input);
  
  char* temp = realloc(*input, (new_length + 1) * sizeof(char));
  if(temp == NULL)
  {
    free(original_to_read);
    printError(ERROR_MEMORY_ALLOCCATION);
    return ERROR_MEMORY_ALLOCCATION;
  }
  *input = temp;
  char* new_text = *input; // reallocating space for the new_text

  int read = 0;   // from where we read 
  int write = 0;  // in this aprt we write into that string

  while (original_to_read[read] != '\0')
  {
    if (strncmp(&original_to_read[read], old_substring, length_old_substring) == 0) 
    //checking if the old substring begins here
    {
      // copy new_substring at the position write
      for (int k = 0; k < length_new_substring; k++)
      {
        new_text[write + k] = new_substring[k];
      }

      read  += length_old_substring;
      write += length_new_substring;
    }
    else
    {
      // copy one symbol
      new_text[write] = original_to_read[read];
      read++;
      write++;
    }
  }
  new_text[write] = '\0';
  free(original_to_read);

  return occurences;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief main function for the command search and replace. It searches for the word we want to replace, and asks
/// for another new word. For example Hello world -> world - welt -> Hello welt
/// @param input original input string from the user
/// @return it returns enumv value with error, quit or search_replace as success
/// 
//---------------------------------------------------------------------------------------------------------------------
int searchReplace(char** input)
{
  printf("\nPlease enter the substring to search for:\n");
  printf(" > ");
  char* old_substring = getInput();
  if(old_substring == NULL)
  {
    printError(ERROR_MEMORY_ALLOCCATION);
    free(old_substring);
    return ERROR_MEMORY_ALLOCCATION;
  }
  if(old_substring[0] == '\0')
  {
    printError(ERROR_STRING_CANT_BE_EMPTY);
    free(old_substring);
    return ERROR_STRING_CANT_BE_EMPTY;
  }
  int check = checkInput(old_substring);
  if(check == QUIT_FULL)
  {
    free(old_substring);
    return QUIT_FULL;
  }
  check = findSubString(*input, old_substring);
  if(check == ERROR_SUBSTRING_NOT_FOUND)
  {
    printError(ERROR_SUBSTRING_NOT_FOUND);
    free(old_substring);
    return ERROR_SUBSTRING_NOT_FOUND;
  }
  printf("\nPlease enter the new substring:\n");
  printf(" > ");
  char* new_substring = getInput();
  if(new_substring == NULL)
  {
    printError(ERROR_MEMORY_ALLOCCATION);
    free(old_substring);
    return ERROR_MEMORY_ALLOCCATION; 
  }
  check = checkInput(new_substring);
  if(check == QUIT_FULL)
  {
    free(old_substring);
    free(new_substring);
    return QUIT_FULL;
  }
  
  int check_function = checkSearchReplace(input, old_substring, new_substring);
  if(check_function == ERROR_SUBSTRING_NOT_FOUND)
  {
    free(old_substring);
    free(new_substring);
    printError(ERROR_SUBSTRING_NOT_FOUND);
    return ERROR_SUBSTRING_NOT_FOUND;
  }
  free(old_substring);
  free(new_substring);
  printf("\nCurrent text:\n");
  printf("%s\n", *input);

  //hello world - 12 char allocated 
  //hello welt - 11

  return SEARCH_REPLACE;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief it validates the prefixes of the words between each other
/// dishwasher - dish
/// @param first string
/// @param second string to compare with
/// @return 1 if character in the first string is bigger than in the seconds, -1 otherwise and 0
//---------------------------------------------------------------------------------------------------------------------
int checkPrefixOfWords(char* string_a, char* string_b) // dish dishwasher  // strcmp dish = dishwasher == negativ // konnte mit strcmp gleich machen
{
  int index = 0;
  while(string_a[index] != '\0' && string_b[index] != '\0' && string_a[index] == string_b[index])
  {
    index++;
  }
  char character_a = string_a[index];
  char character_b = string_b[index];
  if(character_a < character_b)
  {
    return -1; //  'a' < 'b' 'a' = 97 'b' = 98
  }
  if(character_a > character_b)
  {
    return 1; // 'z' > 'b' 
  }
  return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief sorts the words according to the aplhabet with Insertionsort
/// @param array of words
/// @param count the size
/// 
//---------------------------------------------------------------------------------------------------------------------
void sortWords(char* words[], int count) // apfel banana  orange
{
  for (int i = 1; i < count; i++) // abcdf // 345219
  {
    char* key = words[i];
    int position = i - 1;

    // move bigger elements to the right
    while (position >= 0 && checkPrefixOfWords(words[position], key) > 0)
    {
      words[position + 1] = words[position];
      position--;
    }
    words[position + 1] = key;
  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief main function for the command split and sort. it tokenizes the input of the user, so every word is a token
/// and then it builds another sorted string of these tokens
/// @param input of the user
/// @return enum error value, or split_sort as success
//---------------------------------------------------------------------------------------------------------------------
int splitSort(char** input)
{
  int length_string = strlen(*input);
  int count = 0;
  char** words = NULL;
  char* copy = malloc (length_string + 1);
  if(copy == NULL)
  {
    printError(ERROR_MEMORY_ALLOCCATION);
    return ERROR_MEMORY_ALLOCCATION;
  }
  strcpy(copy, *input);
  char* token = strtok(copy, " \t\n"); // delimeter - das Zeichen // 1. Initialisierung // Heap, but if copy is freed then the token is also freed
  while(token != NULL)
  {
    char** temp = realloc(words, (count + 1) * sizeof(char*)); // heap
    if(temp == NULL)
    {
      free(words);
      free(copy);
      printError(ERROR_MEMORY_ALLOCCATION);
      return ERROR_MEMORY_ALLOCCATION;
    }
    words = temp;
    words[count] = token;
    count++;
    token = strtok(NULL, " \t\n"); // 2. Null pointer gives info that we have to continue with other words, if there is no other words we receive null pointer
  }
  if(count == 0)
  {
    free(copy);
    free(words);
    printError(ERROR_NO_WORDS_TO_SORT);
    return ERROR_NO_WORDS_TO_SORT;
  }
  //check for the lexicography and prefix rule
  sortWords(words, count);
  //after that i have to build my string, cause my copy was crushed because of strtok
  int total_length = 0;
  for(int i = 0; i < count; i++)
  {
    total_length = total_length + strlen(words[i]); // 0+5; 5+6; 11+5; the whole length 16 but without '\0'
  }
  total_length = total_length + (count - 1); // spaces between the words; 
  // 16+ (3 - 1)= 18 if 3 words, there are 2 whitespace
  total_length += 1; // space for '\0'
  char* new_text = malloc(total_length);
  if(new_text == NULL)
  {
    free(words);
    free(copy);
    printError(ERROR_MEMORY_ALLOCCATION);
    return ERROR_MEMORY_ALLOCCATION;
  }
  int position = 0;
  for (int i = 0; i < count; i++)
  {
    // copy characters of words[i]
    int k = 0; // 
    while (words[i][k] != '\0')
    {
      new_text[position] = words[i][k]; // strcpy bessere losung
      position++;
      k++;
    }
    // add space after each word except the last one
    if (i < count - 1)
    {
      new_text[position] = ' ';
      position++;
    }
  }
  new_text[position] = '\0'; // terminate string
  free(*input);
  *input = new_text;
  printf("\nCurrent text:\n");
  printf("%s\n", *input);
  free(copy);
  free(words);
  return SPLIT_SORT;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief it checks if BANana = banana the same word
/// @param first word 
/// @param second word to compare with the first word
/// @return 0 if it is not equal or 
//---------------------------------------------------------------------------------------------------------------------
int equalIgnoreCaps(char* first_word, char* second_word) // Banana BaNANA APFEL 
{
  while(*first_word != '\0' && *second_word != '\0')
  {
    char character_first_word = *first_word;
    char character_second_word = *second_word;
    if(tolower(character_first_word)!= tolower(character_second_word))
    {
      return 0; // not equal
    }
    first_word++;
    second_word++;
  }
  return (*first_word == '\0' && *second_word == '\0');
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief it tokenizes the string and then reallocates the memory for the new string
/// @param input from the user
/// @param words array of words
/// @param count_out the size of the array
/// @param duplicate flag
/// @param copy copy array
/// @return Error value or success value as enum
//---------------------------------------------------------------------------------------------------------------------
int tokenizingString(char* input, char*** words_out, int* count_out, int* duplicates_found_out, char** copy_out)
{
  int string_length = strlen(input);
  if(string_length == 0)
  {
    printError(ERROR_ALL_WORDS_UNIQUE);
    return ERROR_ALL_WORDS_UNIQUE;
  }
  char* copy = malloc(string_length + 1);
  if(copy == NULL)
  {
    printError(ERROR_MEMORY_ALLOCCATION);
    return ERROR_MEMORY_ALLOCCATION;
  }
  strcpy(copy, input);
  int count = 0;
  int duplicate_is_found = 0;
  char** words = NULL; // array for words
  char* token = strtok(copy, " \t\n");
  while(token != NULL)
  {
    int not_unique_exist = 0;
    for(int i = 0; i < count; i++)
    {
      if(equalIgnoreCaps(words[i], token))
      {
        not_unique_exist = 1;
        duplicate_is_found = 1;
        break;
      }
    }
    if(!not_unique_exist)
    {
      char** temp = realloc (words, (count + 1)* sizeof(char*));
      if(temp == NULL)
      {
        free(words);
        free(copy);
        printError(ERROR_MEMORY_ALLOCCATION);
        return ERROR_MEMORY_ALLOCCATION; 
      }
      words = temp;
      words[count] = token;
      count++;
    }
    token = strtok(NULL, " \t\n");
  }
  *words_out = words;
  *count_out = count;
  *duplicates_found_out = duplicate_is_found;
  *copy_out = copy;

  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief it checks if the user input string is unique or not
/// @param input from the user
/// @return it returns enum value of error, or success
/// 
//---------------------------------------------------------------------------------------------------------------------
int checkUniqueString(char** input)
{
  char** words = NULL;
  int count = 0;
  int duplicate_is_found = 0;
  char* copy = NULL;
  int token = tokenizingString(*input, &words, &count, &duplicate_is_found, &copy);
  if(token != SUCCESS)
  {
    return token;
  }
  if(!duplicate_is_found)
  {
    free(words);
    free(copy);
    printError(ERROR_ALL_WORDS_UNIQUE);
    return ERROR_ALL_WORDS_UNIQUE;
  }
  int new_total_length = 0;
  for(int i = 0; i < count; i++)
  {
    new_total_length += strlen(words[i]);
  }
  if(count > 0)
  {
    new_total_length += (count - 1);
  }
  new_total_length += 1;
  char* new_text = malloc(new_total_length);
  if(new_text == NULL)
  {
    free(copy);
    free(words);
    printError(ERROR_MEMORY_ALLOCCATION);
    return ERROR_MEMORY_ALLOCCATION;
  }
  int position = 0;
  for(int i = 0; i < count; i++)
  {
    int k = 0;
    while(words[i][k] != '\0')
    {
      new_text[position++] = words[i][k++];
    }
    if(i < count - 1)
    {
      new_text[position++] = ' ';
    }
  }
  new_text[position] = '\0';
  free(*input);
  *input = new_text;
  free(words);
  free(copy);
  printf("\nCurrent text:\n");
  printf("%s\n", *input);

  return UNIQUE;
  
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief main function of unique command
/// @param input user input string
/// @return returns the check
/// 
//---------------------------------------------------------------------------------------------------------------------
int uniqueFunction(char** input)
{
  int check = checkUniqueString(input);
  return check;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief function for handling the command from the user. appending text, search and replace an etc.
/// @param check_command validates what kind of command was entered bu the user
/// @param text_input the text of the user
/// @param command the command itself
/// @return returns the enum values Success, quit or Error
//---------------------------------------------------------------------------------------------------------------------
int handleCommand(int check_command, char** text_input, char* command)
{
  if(check_command == APPEND_TEXT)
  {
    int append = appendText(text_input);
    free(command);
    if(append == QUIT || append == QUIT_FULL)
    {
      free(*text_input);
      return QUIT;
    }
    return SUCCESS;
  }
  if(check_command == SEARCH_REPLACE)
  {
    int search_replace = searchReplace(text_input);
    free(command);
    if(search_replace == QUIT || search_replace == QUIT_FULL)
    {
      free(*text_input);
      return QUIT;
    }
    return SUCCESS;
  }
  if(check_command == SPLIT_SORT)
  {
    int split_sort = splitSort(text_input);
    free(command);
    if(split_sort == QUIT || split_sort == QUIT_FULL)
    {
      free(*text_input);
      return QUIT;
    }
    return SUCCESS;
  }

  if(check_command == UNIQUE)
  {
    int unique = uniqueFunction(text_input);
    free(command);
    if(unique == QUIT || unique == QUIT_FULL)
    {
      free(*text_input);
      return QUIT;
    }
    return SUCCESS;
  }

  if(check_command == QUIT)
  {
    free(command);
    printf("\nCurrent text:\n");
    printf("%s\n", *text_input);
    free(*text_input);
    return QUIT;
  }
  if(check_command == ERROR_UKNOWN_COMMAND)
  {
    free(command);
    printError(ERROR_UKNOWN_COMMAND);
    return SUCCESS;
  }
  if(check_command == QUIT_FULL)
  {
    free(command);
    free(*text_input);
    return QUIT;
  }
  free(command);
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief command loop of the whole program
/// takes the text input from the user and mmodifies it according to the function user has chosen
/// 
/// @return returns enum values quit, Success or error
//---------------------------------------------------------------------------------------------------------------------
int editingTextLoop(void)
{
  char* text_input = getInput(); // my original and it has to be reallocated everytime i use it
  if(text_input == NULL)
  {
    printError(ERROR_MEMORY_ALLOCCATION);
    return ERROR_MEMORY_ALLOCCATION;
  }
  int check_text = checkInput(text_input);
  if(check_text == QUIT)
  {
    free(text_input);
    return QUIT;
  }
  if(check_text == QUIT_FULL)
  {
    free(text_input);
    return QUIT_FULL;
  }
  while(1)
  {
    editText();
    char* command = getInput();
    if(command == NULL)
    {
      printError(ERROR_MEMORY_ALLOCCATION);
      free(text_input);
      return ERROR_MEMORY_ALLOCCATION;
    }
    int check_command = checkInput(command);
    int function = handleCommand(check_command, &text_input, command);
    if(function != SUCCESS)
    {
      return function;
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief main of this program
/// it prints the welcome message, asks for the text and goes to the command loop
/// @return it returns 0, quit or full quit if the user input is quit
/// 
//---------------------------------------------------------------------------------------------------------------------
int main(void)
{
  welcomeMessage();
  enterTextPrompt();
  int edit = editingTextLoop();
  if(edit == QUIT)
  {
    return QUIT;
  }
  if(edit == QUIT_FULL)
  {
    return QUIT;
  }

  return 0;

}

