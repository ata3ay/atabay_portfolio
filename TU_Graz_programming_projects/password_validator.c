//------------------------------------------------------------------------------
// a2.c
//
// A small example program for the sole purpose of demonstrating
// the coding standard.
//
// Atabay Dinmukhamed
//
// Author: 12046429
//------------------------------------------------------------------------------
//
#include <stdio.h>

#define MAX_LENGTH 100 
#define MIN_LENGTH 8
#define MAX_PASSWORDS 5

//------------------------------------------------------------------------------
///
/// Prints welcome message to the Password manager.
/// 
///
//------------------------------------------------------------------------------
void welcome(void)
{
  printf("Welcome to the ESP password manager!\n\n");
}

//------------------------------------------------------------------------------
///
/// Prints Please enter a password message.
/// 
///
//------------------------------------------------------------------------------

void enterPassword(void)
{
  printf("Please enter a password\n");
}

//------------------------------------------------------------------------------
///
/// Prints error message about 15 characters to the user after printing a password
/// 
///
//------------------------------------------------------------------------------

void maxLength(void)
{
  printf("Password must not be more than 15 characters long.\n");
}

//------------------------------------------------------------------------------
///
/// Prints error message about at least 8 characters to 
/// the user after printing a password
/// 
///
//------------------------------------------------------------------------------

void minLength(void)
{
  printf("Password must be at least 8 characters long.\n");
}

//------------------------------------------------------------------------------
///
/// Prints error message about at least one lowercase 
/// to the user after printing a password
/// 
///
//------------------------------------------------------------------------------

void oneLowerCase(void)
{
  printf("Password must contain at least one lowercase character.\n");
}

//------------------------------------------------------------------------------
///
/// Prints error message about at least one uppercase 
/// to the user after printing a password
/// 
///
//------------------------------------------------------------------------------

void oneUpperCase(void)
{
  printf("Password must contain at least one uppercase character.\n");
}

//------------------------------------------------------------------------------
///
/// Prints error message about at least one digit 
/// to the user after printing a password
/// 
///
//------------------------------------------------------------------------------

void oneDigit (void)
{
  printf("Password must contain at least one digit.\n");
}

//------------------------------------------------------------------------------
///
/// Prints error message about at least one special character 
/// to the user after printing a password
/// 
///
//------------------------------------------------------------------------------

void oneSpecialCharacter (void)
{
  printf("Password must contain at least one special character.\n");
}

//------------------------------------------------------------------------------
///
/// Prints error message that password must not be a palindrome 
/// to the user after printing a password
/// 
///
//------------------------------------------------------------------------------
void notPalindrome (void)
{
  printf("Password must not be a palindrome.\n");
}

//------------------------------------------------------------------------------
///
/// Prints error message that the password has been used
/// to the user after printing a password
/// 
///
//------------------------------------------------------------------------------

void noRepeatPassword (void)
{
  printf("This password has already been used.\n");
}

//------------------------------------------------------------------------------
///
/// Prints success set message
/// to the user after printing a password
/// 
///
//------------------------------------------------------------------------------

void successPassword (void)
{
  printf("Password set successfully.\n");
}

//------------------------------------------------------------------------------
///
/// Cleans the input by removing newline characters
/// and flushing remaining characters in buffer stdin if necessary
/// 
//------------------------------------------------------------------------------

void inputCleaning(char *input_password)
{
  int new_line = 0; // it helps the pgrogramm to indicate if \n is found in the string
  for (int i_symbol_password = 0; input_password[i_symbol_password] != '\0'; i_symbol_password++)
  { // looping to find newline in the input
    if(input_password[i_symbol_password] == '\n') // if it was found
    {
      input_password[i_symbol_password] = '\0'; // programm replaces with a terminator null
      new_line = 1; // then it is true
      break; // it ends checking with the string
    }
  }  
  if(new_line == 0) //if no new line was found, then the input is probably too long 
  { // flush the remaining characters in the input buffer
    int input_char;
    while ((input_char = getchar()) != '\n' && input_char != EOF); //until a newline or end-of-file is reached
  }
}

//------------------------------------------------------------------------------
///
/// Asks the user if they want to set another password (yes/no)
/// the program accepts the answers yes/no, and handles the invalid inputs
/// Rekursively asks again if the input is invalid
/// @return 1 if "yes"
/// @return 0 if "no"
/// 
//------------------------------------------------------------------------------
char passwordYesNo(void)
{
  char yes_no[5]; // buffer to store the answer, max 4 chars + null terminator
  printf("Do you want to set another password? (yes/no)");
  printf("\n > ");
  scanf("%3s", yes_no); 
  int white_space;
  while ((white_space = getchar()) != '\n' && white_space != EOF); // clears the newline character from stdin buffer

  for (int i_symbol = 0; yes_no[i_symbol] != '\0'; i_symbol++) // index of symbols in a line code  [0]Y[1]E[2]S[3]'\0'
  {
    if (yes_no[i_symbol] >= 'A' && yes_no[i_symbol] <= 'Z')  // converting any uppercase to the lowercase
    {
      yes_no[i_symbol] = yes_no[i_symbol] + ('a' - 'A');
      }
  } // then it had to check rekursiv
  if (yes_no[0] == 'y' && yes_no[1] == 'e' && yes_no[2] == 's' && yes_no[3] == '\0') // checking if the input is yes
  {
    return 1; // returns Yes
    } 
    else if (yes_no[0] == 'n' && yes_no[1] == 'o' && yes_no[2] == '\0') 
    {
        return 0; // Returns no
    } else 
    {
        printf("Invalid input. Please enter 'yes' or 'no'.\n");
        return passwordYesNo(); // asking again
    }
}

//------------------------------------------------------------------------------
///
/// Validates the length of the password.
/// Checks if the password has at least 8 and at most 15 characters.
/// if no prints the error message
/// @param current_password Pointer points to password[numberOfPassword] to the password string to validate
/// @return 1 if valid, 
/// @return 0 if invalid
///
//------------------------------------------------------------------------------

int lengthValidator (char *current_password)
{
  int length = 0;
  for(int i_symbol = 0; current_password[i_symbol] != '\0'; i_symbol++) // counting the number of characters in the string until null term is found
  {
    length++;
  }

  if(length < 8) // checks if the password has at least 8 characters
  {
    minLength(); // prints min legnth 8 characters mesage
    return 0;
  }
  if(length > 15) // checks if the password has at most 15 characters
  {
    maxLength(); // prints max legnth 15 characters mesage
    return 0;
  }
  return 1; // valid length
}

//------------------------------------------------------------------------------
///
/// Checks if the password has at least one uppercase.
/// if no prints the error message
/// @param current_password Pointer points to password[numberOfPassword] to the password string to validate
/// @return 1 if valid, 
/// @return 0 if invalid
///
//------------------------------------------------------------------------------
int upperCaseValidator(char *current_password)
{
  int upper = 0;
  for(int i_symbol = 0; current_password[i_symbol] != '\0'; i_symbol++) // counting the number of characters in the string until null term is found
  {
    if (current_password[i_symbol] >= 'A' && current_password[i_symbol] <= 'Z') 
    {
      upper = 1; // check for the upper case character, if so then upper = 1
      break; // it needs only one, thats why break
      }
      }
      if (upper == 0) // no uppercase found
        {
          oneUpperCase(); // prints the error message
          return 0;
        }
  return 1; // valid
}

//------------------------------------------------------------------------------
///
/// Checks if the password has at least one lowercase.
/// if no prints the error message
/// @param current_password Pointer points to password[numberOfPassword] to the password string to validate
/// @return 1 if valid, 
/// @return 0 if invalid
///
//------------------------------------------------------------------------------

int lowerCaseValidator(char *current_password)
{
  int lower = 0;
  for(int i_symbol = 0; current_password[i_symbol] != '\0'; i_symbol++)// counting the number of characters in the string until null term is found
  {
  if (current_password[i_symbol] >= 'a' && current_password[i_symbol] <= 'z')
  {
    lower = 1;// check for the lower case character, if so then upper = 1
    break; // it needs only one, thats why break
  }
  }
  if (lower == 0) // no lowercase found
        {
          oneLowerCase(); // prints the error message
          return 0;
        }
  return 1; // valid
}

//------------------------------------------------------------------------------
///
/// Checks if the password has at least one digit.
/// if no prints the error message
/// @param current_password Pointer points to password[numberOfPassword] to the password string to validate
/// @return 1 if valid, 
/// @return 0 if invalid
///
//------------------------------------------------------------------------------
int oneDigitValidator(char *current_password)
{
  int one_digit = 0;
  for (int i_symbol = 0; current_password[i_symbol] != '\0'; i_symbol++) // counting the number of characters in the string until null term is found
  {
  if (current_password[i_symbol] >= '0' && current_password[i_symbol] <= '9') // Check if the current character is a digit ASCII 0-9
      {
        one_digit = 1; // digit found
        break; // break, cause i need only one digit
      }
    }
      if(one_digit == 0) // no digit found
      {
        oneDigit();//prints the error message
        return 0;
        }
  return 1; //valid
}

//------------------------------------------------------------------------------
///
/// Validates whether the password contains at least one special character.
/// Special characters are all printable ASCII characters except letters and digits.
/// If no special character is found, prints an error message.
///
/// @param current_password Pointer points to password[numberOfPassword] to the password string to validate
///
/// @return 1 if valid, 
/// @return 0 if no special character found
///
//------------------------------------------------------------------------------
int oneSpecialValidator(char *current_password)
{
  int one_special = 0;
  for(int i_symbol = 0; current_password[i_symbol] != '\0'; i_symbol++) // counting the number of characters in the string until null term is found
  {
    if ((current_password[i_symbol] >= 32 && current_password[i_symbol] <= 47) || // Check if the current character is a speacial character ASCII
        (current_password[i_symbol] >= 58 && current_password[i_symbol] <= 64) || 
        (current_password[i_symbol] >= 91 && current_password[i_symbol] <= 96) || 
        (current_password[i_symbol] >= 123 && current_password[i_symbol] <= 126))
        {
          one_special = 1; // if so, then break
          break;
          }
  }
  if(one_special == 0)
  {
    oneSpecialCharacter(); // prints the error message
    return 0;
    }
  return 1;
}


//------------------------------------------------------------------------------
///
/// Validates if the password was already used.
/// compares the current password with all previous stored passwords.
/// If used password is found, prints an error message.
/// @param current_password Pointer to the password being checked
/// @param password 2D array containing all previously accepted passwords
/// @param count Number of valid passwords currently stored
/// @return 1 if valid, 
/// @return 0 if used password is found
///
//------------------------------------------------------------------------------
int usedPassword(char *current_password, char password[MAX_PASSWORDS][MAX_LENGTH], int count)
{
  for (int previous_index = 0; previous_index < count; previous_index++) 
  {
    int i_symbol = 0;
    
    while (password[previous_index][i_symbol] == current_password[i_symbol] && 
    current_password[i_symbol] != '\0') 
    {
      i_symbol++;
    }
    if (password[previous_index][i_symbol] == '\0' && current_password[i_symbol] == '\0') 
    {
      noRepeatPassword();
      return 0;
    }
    }
  return 1;
}

//------------------------------------------------------------------------------
///
/// Validates if the password has a palindrom.
/// A palindrome is a string that reads the same forward and backward.
/// If password is palindrom, prints an error message.
///
/// @param current_password Pointer to the password string to validate
///
/// @return 1 if valid, 
/// @return 0 if palindrom is found
///
//------------------------------------------------------------------------------
int mirrorPassword(char *current_password)
{
  int length = 0;
  while (current_password[length] != '\0') // This part i need to know how long the line is
  {
    length++;
  }
  for (int i_symbol = 0; i_symbol < length / 2; i_symbol++) 
  {
    if (current_password[i_symbol] != current_password[length - 1 - i_symbol]) 
    {
      return 1; // it gives info, thats not a palindrome password
      }
    }
    notPalindrome();
    return 0;
}

//------------------------------------------------------------------------------
///
/// Validates if the password has a repeating numeric patterns.
/// A repeating numeric patterns is defined by the group of 3 digits, like 123abc123.
/// If password has a repeating numeric pattern, prints an error message.
///
/// @param current_password Pointer to the password string to validate
///
/// @return 1 if valid, 
/// @return 0 if palindrom is found
///
//------------------------------------------------------------------------------
int repeatingPassword(char *current_password) //Tc 22 numeric patterns verbessern
{
  int length = 0;
  while (current_password[length] != '\0') 
  {
    length++;
  }
  for (int first_group_digits = 0; first_group_digits <= length - 3; first_group_digits++)
  {
    if (current_password[first_group_digits] < '0' || current_password[first_group_digits] > '9') 
    continue; // checking if the characters from i_symbol are the digits
    if (current_password[first_group_digits + 1] < '0' || current_password[first_group_digits + 1] > '9') 
    continue; // If any of them is not a digit, skip to the next position
    if (current_password[first_group_digits + 2] < '0' || current_password[first_group_digits + 2] > '9') 
    continue;

    char three_digits_pattern[4];
    three_digits_pattern[0] = current_password[first_group_digits];
    three_digits_pattern[1] = current_password[first_group_digits + 1];
    three_digits_pattern[2] = current_password[first_group_digits + 2];
    three_digits_pattern[3] = '\0';
    
    for (int second_group_digits = first_group_digits + 3; second_group_digits <= length - 3; second_group_digits++) 
    {
      if (current_password[second_group_digits] == three_digits_pattern[0] &&
      current_password[second_group_digits + 1] == three_digits_pattern[1] &&
      current_password[second_group_digits + 2] == three_digits_pattern[2]) 
      {
        printf("Password must not contain repeating numeric patterns.\n");
        return 0;
        }
    }
  }
  return 1; // Everything ok
}

//------------------------------------------------------------------------------
///
/// Validates if maximum number of allowed passwords has been reached
/// If the number of stored passwords equals MAX_PASSWORDS
/// prints a message and indicates that no more passwords can be added..
///
/// @param current_password Current count of successfully stored passwords
///
/// @return 1 if valid, 
/// @return 0 if palindrom is found
///
//------------------------------------------------------------------------------
int capacityExhausted(int numberOfPassword)
{
  if (numberOfPassword == MAX_PASSWORDS) 
  {
    printf("Capacity exhausted! Go touch some grass!\n");
    return 1; 
  }
  return 0;
} 

//------------------------------------------------------------------------------
///
/// Main function of the password manager.
/// It collects up to 5 secure passwords from the user
/// validating each password through.
/// Ends when the user prints "no" or when maximum capacity is reached.
///
/// @return 0 if user exits
/// @return 5 if capacity is full 
//------------------------------------------------------------------------------
int main(void) 
{
  welcome();
  char password[MAX_PASSWORDS][MAX_LENGTH];
  int numberOfPassword = 0; // count of passwords at the beginning
  while (numberOfPassword < MAX_PASSWORDS) // 
  {
  enterPassword();
  printf(" > ");
  fgets(password[numberOfPassword], MAX_LENGTH, stdin); // it reads a password and stores it in the first slot of the array
  inputCleaning(password[numberOfPassword]);
  char *current_password = password[numberOfPassword];
  int length = lengthValidator(current_password);
  if (length == 0)
  {
    continue;// not return 0, because it must to ask again
  }
  int lower_case = lowerCaseValidator(current_password);
  if (lower_case == 0)
  {
    continue;// not return 0, because it must to ask again
  }
  int upper_case = upperCaseValidator(current_password);
  if (upper_case == 0)
  {
    continue;// not return 0, because it must to ask again
  }
  int one_digit = oneDigitValidator(current_password);
  if (one_digit == 0)
  {
    continue;// not return 0, because i have to ask again
  }
  int one_special_character = oneSpecialValidator(current_password);
  if (one_special_character == 0)
  {
    continue; // not return 0, because i have to ask again
  }
  int not_palindrom = mirrorPassword(current_password);
  if (not_palindrom == 0) 
  {
    continue; // not return 0, because it must to ask again
  }
  int no_repeat = repeatingPassword(current_password);
  if (no_repeat == 0)
  {
    continue; // not return 0, because i have to ask again
  }
  int already_used = usedPassword(current_password, password, numberOfPassword);
  if (already_used == 0) 
  {
    continue; // not return 0, because it must to ask again
  }
  successPassword(); // prints that password was succesfully set
  numberOfPassword++; // increase the counter
  if(capacityExhausted(numberOfPassword))
  {
    return 5; // Exit code for full capacity
  }
  int question = passwordYesNo(); // asking if the user wants another password
  if (question == 0)
  {
    printf("Exiting password manager. Goodbye!\n");
    return 0;
  }
  }
  return 0;
}

