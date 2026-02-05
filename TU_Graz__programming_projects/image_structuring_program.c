//---------------------------------------------------------------------------------------------------------------------
/// a4.c
/// Image Structuring Program
///
/// Author: 12046429 Atabay Dinmukhamed
//---------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include "bmp.h"

//---------------------------------------------------------------------------------------------------------------------
/// @brief constant values for the project
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
#define MAX_TOKENS 6
#define OFFSET_WIDTH 18 // 0x12
#define OFFSET_HEIGHT 22 // 0X16
#define SIZE_IN_BYTES 4
#define PIXEL_DATA_OFFSET 10 // 0x0A
#define BPP_OFFSET 28 // 0x1C
#define ONE_PIXEL 4 // 1 pixel = 4 bytes
#define ALL_WHITE_PIXELS_WITH_ALPHA 255
#define BMP_ID_AT_ROOT -1

//---------------------------------------------------------------------------------------------------------------------
/// @brief return values for the program
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
enum
{
  SUCCESS = 0,
  ERROR_MEMORY_ALLOCATION = 1,
  ERROR_INVALID_AMOUNT_PARAMETERS = 2,
  ERROR_INVALID_CANVAS_SIZE = 3,
  QUIT = 4,
  FAIL = -1,
  ERROR_COMMAND_UNKNOWN = -4,
  ERROR_WRONG_NUMBER_OF_ARGUMENTS = -5,
  ERROR_BMP_ID_NOT_FOUND = -6,
  ERROR_CANNOT_OPEN_FILE = -7,
  ERROR_INVALID_FILE = -8,
  ERROR_CROP_COORDINATES_OUTSIDE = -9,
  ERROR_CROP_COORDINATES_DONT_VALID_RECTANGLE = -10,
  ERROR_CANVAS_COORDINATES_INVALID = -11,
  ERROR_BMP_DOESNT_FIT = -12,
  ERROR_INVALID_BLEND_MODE = -13,
  ERROR_ALREADY_ROOT_LAYER = -14,
  ERROR_LAYER_ID_NOT_FOUND = -15,
  ERROR_INVALID_FILE_PATH = -16
};

//---------------------------------------------------------------------------------------------------------------------
///@brief function values 
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
enum 
{
  HELP = 5,
  LOAD = 6,
  CROP = 7,
  PLACE = 8,
  UNDO = 9,
  PRINT = 10,
  SWITCH = 11,
  TREE = 12,
  BMPS = 13,
  SAVE = 14
};

//---------------------------------------------------------------------------------------------------------------------
/// @struct BMP where the bmp image is saved
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
typedef struct _BMP_
{
  int height_;
  int width_;
  int BMP_id_;
  uint8_t* pixels_; 
  uint8_t** pixels_2d_;
} BMP;

//---------------------------------------------------------------------------------------------------------------------
/// @brief BMP LIbrary where the images are saved
/// it contains a pointer to the array of bmps
///
/// 
//---------------------------------------------------------------------------------------------------------------------
typedef struct _BMP_Library_
{
  int count_;
  int capacity_;
  BMP** bmps_;
} BMP_Library;

//---------------------------------------------------------------------------------------------------------------------
/// @brief struct Layer where i store the layers
/// it contains also pointer to parent and pointer to array of children
///
/// 
//---------------------------------------------------------------------------------------------------------------------
typedef struct _Layer_
{
  int layer_id_;
  int bmp_id_;
  int x_;
  int y_;
  char blend_mode_;
  struct _Layer_* parent_;
  struct _Layer_** children_;
  int children_count_;
  int children_capacity_;
} Layer;

//---------------------------------------------------------------------------------------------------------------------
/// @brief struct canvas where i store the info and it contains pointer to the root, and pointer to
/// active layer
///
/// 
//---------------------------------------------------------------------------------------------------------------------
typedef struct _Canvas_
{
  int width_;
  int height_;
  Layer* root_; // 
  Layer* active_layer_; // layer im currently in
  int next_layer_id_;
} Canvas;

//---------------------------------------------------------------------------------------------------------------------
/// @brief struct of bgra which means coloring
/// it has info to the colours b - blue g - green r - red, a is white
///
/// 
//---------------------------------------------------------------------------------------------------------------------
typedef struct _Bgra_
{
  uint8_t b_; 
  uint8_t g_;
  uint8_t r_;
  uint8_t a_;
} Bgra;

//---------------------------------------------------------------------------------------------------------------------
/// @brief struct of gathered layers where i store info and pointer to the array of layers
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
typedef struct _Gathered_layers_
{
  int count_;
  Layer** array_of_layers_;
} GatheredLayers;

//---------------------------------------------------------------------------------------------------------------------
/// 
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
// typedef struct _Rectangle_
// {
//   int x0_;
//   int y0_;
//   int x1_;
//   int y1_;
//   int is_valid_;
// } Rectangle;

//---------------------------------------------------------------------------------------------------------------------
/// @brief welcome message function. It will print it everytime the program starts
/// @param width parameter which is given by the user in the beginning
/// @param height parameter which is given by the user in the beginning
/// 
//---------------------------------------------------------------------------------------------------------------------
void welcomeMessage(int width, int height)
{
  printf("\nWelcome to Image Structuring Program!\n");
  printf("The canvas is %d x %d pixels.\n\n", width, height);
}

//---------------------------------------------------------------------------------------------------------------------
///@brief help message when the user chooses helo function
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
void helpMessage(void)
{
  printf("\nAvailable commands:\n");
  printf(" help\n");
  printf(" load <PATH>\n"); 
  printf(" crop <BMP_ID> <TOP_X> <TOP_Y> <BOTTOM_X> <BOTTOM_Y>\n");
  printf(" place <BMP_ID> <CANVAS_X> <CANVAS_Y> <BLEND_MODE>\n");
  printf(" undo\n");
  printf(" print\n");
  printf(" switch <LAYER_ID>\n");
  printf(" tree\n");
  printf(" bmps\n");
  printf(" save <FILE_PATH>\n");
  printf(" quit\n\n");
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief prints the corresponding error message according to the enum value
/// @param error_code
///
/// 
//---------------------------------------------------------------------------------------------------------------------
void printError(int error_code)
{
  if(error_code == ERROR_MEMORY_ALLOCATION)
  {
    printf("[ERROR] Memory allocation failed!\n");
  }
  if(error_code == ERROR_INVALID_AMOUNT_PARAMETERS)
  {
    printf("[ERROR] Invalid amount of command line parameters!\n");
  }
  if(error_code == ERROR_INVALID_CANVAS_SIZE)
  {
    printf("[ERROR] Invalid canvas size specified!\n");
  }
  if(error_code == ERROR_COMMAND_UNKNOWN)
  {
    printf("[ERROR] Command unknown!\n");
  }
  if(error_code == ERROR_WRONG_NUMBER_OF_ARGUMENTS)
  {
    printf("[ERROR] Wrong number of arguments!\n");
  }
  if(error_code == ERROR_BMP_ID_NOT_FOUND)
  {
    printf("[ERROR] BMP ID not found!\n");
  }
  if(error_code == ERROR_CANNOT_OPEN_FILE)
  {
    printf("[ERROR] Cannot open file!\n");
  }
  if(error_code == ERROR_INVALID_FILE)
  {
    printf("[ERROR] Invalid file!\n");
  }
  if(error_code == ERROR_CROP_COORDINATES_OUTSIDE)
  {
    printf("[ERROR] Crop coordinates are outside the BMP!\n");
  }
  if(error_code == ERROR_CROP_COORDINATES_DONT_VALID_RECTANGLE)
  {
    printf("[ERROR] Crop coordinates do not form a valid rectangle!\n");
  }
  if(error_code == ERROR_CANVAS_COORDINATES_INVALID)
  {
    printf("[ERROR] Canvas coordinates are invalid!\n");
  }
  if(error_code == ERROR_BMP_DOESNT_FIT)
  {
    printf("[ERROR] BMP does not fit on the canvas!\n");
  }
  if(error_code == ERROR_INVALID_BLEND_MODE)
  {
    printf("[ERROR] Invalid blend mode!\n");
  }
  if(error_code == ERROR_ALREADY_ROOT_LAYER)
  {
    printf("[ERROR] Already at root layer!\n");
  }
  if(error_code == ERROR_LAYER_ID_NOT_FOUND)
  {
    printf("[ERROR] Layer ID not found!\n");
  }
  if(error_code == ERROR_INVALID_FILE_PATH)
  {
    printf("[ERROR] Invalid file path!\n");
  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief gets input from the user dynamically
/// @return it returns the dynamically allocated input
///
/// 
//---------------------------------------------------------------------------------------------------------------------
char* getInput(void) 
{
  int buffer = 50;
  int index = 0;
  char* get_input = malloc(buffer * sizeof(char));
  if(get_input == NULL)
  {
    printError(ERROR_MEMORY_ALLOCATION);
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
/// @brief it checks if the string contains the numbers
/// @param string_to_check string i wan to check 
/// @return 
/// 
//---------------------------------------------------------------------------------------------------------------------
int isNumberCheckString(char* string_to_check)
{
  int length = strlen(string_to_check);
  for(int i = 0; i < length; i++)
  {
    if(!isdigit(string_to_check[i]))
    return FAIL;
  }
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief parses the arguments 
/// @param arg count number in the command line
/// @param char* argv 
/// @param canvas canvas where i store the info of height and width
/// @return return Success or Eror value
//---------------------------------------------------------------------------------------------------------------------
int parseArguments(int argc, char* argv[], Canvas* canvas)
{
  if(argc != 3)
  {
    return ERROR_WRONG_NUMBER_OF_ARGUMENTS;
  }
  if(argc == 3) // but i have to make sure that the string contains only digits
  {
    if(isNumberCheckString(argv[1]) == FAIL || isNumberCheckString(argv[2]) == FAIL)
    {
      return ERROR_INVALID_CANVAS_SIZE;
    }
    int width = atoi(argv[1]);//width
    int height = atoi(argv[2]);//height
    if(width < 1 || height < 1)
    {
      return ERROR_INVALID_CANVAS_SIZE;
    }
    canvas->width_ = width;
    canvas->height_ = height;
  }
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief tokenisez the string. It checks if there is a space in between the words and parameters and igrnores
/// if there is \0 it breaks 
/// @param string
/// @param tokens from the input
/// @param max_tokens from the input
/// @return count of the tokens exactly
//---------------------------------------------------------------------------------------------------------------------
int tokenizeStringInput(char* string, char* tokens[], int max_tokens)
{
  int count = 0;
  char* symbol = string; 
  while(*symbol != '\0' && count < max_tokens)
  {
    while(isspace(*symbol))
    {
      symbol++; // in order to ignore the whitespaces inbetween the tokens
    }
    if(*symbol == '\0')
    {
      break;
    }
    tokens[count++] = symbol; // for example place -> p l a c e  3  10 10 (it goes character by character)
    
    while(*symbol != '\0' && !isspace(*symbol))
    {
      symbol++;
    }
    if(*symbol != '\0')
    {
      *symbol = '\0';
      symbol++;
    }
    // after the first word there is whitespace (place 3 10 10) -> instead of whitespace it puts (\0) to tokenize it
  }
  return count;
}

//---------------------------------------------------------------------------------------------------------------------
///@brief Removes leading and trailing whitespace and compresses internal. 
/// whitespace sequences to a single space. "    a1   b1   " -> "a1 b1"
/// @param char *string
/// 
///
/// 
//---------------------------------------------------------------------------------------------------------------------
void normalizeInput(char *my_string)
{
  int read = 0;
  int write = 0;
  int space_pending = 0;

  // Skip leading whitespace
  while (my_string[read] != '\0' && isspace((unsigned char)my_string[read]))
  {
    read++;
  }

  while (my_string[read] != '\0')
  {
    if (isspace((unsigned char)my_string[read]))
    {
        space_pending = 1;
    }
    else
    {
        // only add a single space between tokens
      if(space_pending && write > 0)
      {
        my_string[write++] = ' ';
      }
      space_pending = 0;
      my_string[write++] = my_string[read];
    }
    read++;
  }
  my_string[write] = '\0';
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief checks the input from the user, which function the user has chosen or if there is Error
/// in the input
/// @param tokens array of tokens from the input
/// @param count_words the number of words in the input
/// @return Success, or according ERror value
//---------------------------------------------------------------------------------------------------------------------
int checkInput(char* tokens[], int count_words)
{
  if(count_words == 0)
  {
    return SUCCESS;
  }
  if(strcmp(tokens[0], "quit") == 0)
  {
    return QUIT;
  }
  if(strcmp(tokens[0], "help") == 0)
  {
    return HELP;
  }
  if(strcmp(tokens[0], "undo") == 0)
  {
    return UNDO;
  }
  if(strcmp(tokens[0], "print") == 0)
  {
    return PRINT;
  }
  if(strcmp(tokens[0], "tree") == 0)
  {
    return TREE;
  }
  if(strcmp(tokens[0], "bmps") == 0)
  {
    return BMPS;
  }
  if(strcmp(tokens[0], "load") == 0)
  {
    if(count_words != 2)
    {
      printError(ERROR_WRONG_NUMBER_OF_ARGUMENTS);
      return ERROR_WRONG_NUMBER_OF_ARGUMENTS;
    }
    return LOAD;
  }
  if(strcmp(tokens[0], "crop") == 0)
  {
    if(count_words != 6)
    {
      printError(ERROR_WRONG_NUMBER_OF_ARGUMENTS);
      return ERROR_WRONG_NUMBER_OF_ARGUMENTS;
    }
    return CROP;
  }
  if(strcmp(tokens[0], "place") == 0)
  {
    if(count_words != 5)
    {
      printError(ERROR_WRONG_NUMBER_OF_ARGUMENTS);
      return ERROR_WRONG_NUMBER_OF_ARGUMENTS;
    }
    return PLACE;
  }
  if(strcmp(tokens[0], "switch") == 0)
  {
    if(count_words != 2)
    {
      printError(ERROR_WRONG_NUMBER_OF_ARGUMENTS);
      return ERROR_WRONG_NUMBER_OF_ARGUMENTS;
    }
    return SWITCH;
  }
  if(strcmp(tokens[0], "save") == 0)
  {
    if(count_words != 2)
    {
      printError(ERROR_WRONG_NUMBER_OF_ARGUMENTS);
      return ERROR_WRONG_NUMBER_OF_ARGUMENTS;
    }
    return SAVE;
  }
  return ERROR_COMMAND_UNKNOWN;
}


//---------------------------------------------------------------------------------------------------------------------
///@brief creates the BMP LIbrary
/// it allocates the library named bib on the heap
/// 
/// @return library (bib)
//---------------------------------------------------------------------------------------------------------------------
BMP_Library* createLibraryOnHeap(void)
{
  BMP_Library* bib = malloc(sizeof(BMP_Library));
  if(bib == NULL)
  {
    return NULL;
  }
  bib->count_ = 0;
  bib->capacity_ = 0;
  bib->bmps_ = NULL;

  return bib;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief it creates a root on the heap for the tree structure
/// 
///
/// @return it returns the root
//---------------------------------------------------------------------------------------------------------------------
Layer* createRoot(void) //initialize root 0
{
  Layer* root_layer = malloc(sizeof(Layer)); //
  if(root_layer == NULL)
  {
    return NULL;
  }
  root_layer->layer_id_ = 0;
  root_layer->bmp_id_= BMP_ID_AT_ROOT; // -1 
  root_layer->x_ = 0;
  root_layer->y_ = 0;
  root_layer->blend_mode_ = 0; 
  root_layer->parent_ = NULL;
  root_layer->children_ = NULL;
  root_layer->children_count_ = 0;
  root_layer->children_capacity_ = 0;

  return root_layer;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief it reallocates the children if there is not enough space on the heap
/// and adds the children to the root
/// @param parent layer
/// @param child layer
/// @return Success if everything went ok, or error value
//---------------------------------------------------------------------------------------------------------------------
int addChildToParentLayer(Layer* parent, Layer* child)
{
  int new_capacity;
  if(parent->children_count_ >= parent->children_capacity_)
  {
    if(parent->children_capacity_ == 0)
    {
      new_capacity = 2;
    }
    else
    {
      new_capacity = parent->children_capacity_ * 2;
    }
    Layer** temp = realloc(parent->children_, new_capacity * sizeof(Layer*));
    if(temp == NULL)
    {
      return ERROR_MEMORY_ALLOCATION;
    }
    parent->children_ = temp;
    parent->children_capacity_ = new_capacity;
  }
  parent->children_[parent->children_count_++] = child;
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
///@brief creates a node on the heap
/// @param parent layer
/// @param layer_id
/// @param bmp_id
/// @param x coordinated
/// @param y coordinate
/// @param blend_mode
/// @return node or NULL
//---------------------------------------------------------------------------------------------------------------------
Layer* createLayer(Layer* parent, int layer_id, int bmp_id, int x, int y, char blend_mode)
{
  Layer* node = malloc(sizeof(Layer)); // 
  if(node == NULL)
  {
    return NULL;
  }
  node->layer_id_ = layer_id;
  node->bmp_id_ = bmp_id;
  node->x_ = x;
  node->y_ = y;
  node->blend_mode_ = blend_mode;
  node->parent_ = parent;
  node->children_ = NULL;
  node->children_capacity_ = 0;
  node->children_count_ = 0;

  return node;
}

//---------------------------------------------------------------------------------------------------------------------
///@brief it prints the whitespace for tree sturcture
/// @param depth_of_layer according to Readme
///
///
//---------------------------------------------------------------------------------------------------------------------
void printWhiteSpaceForTree(int depth_of_layer)
{
  for(int i = 0; i < depth_of_layer * 3; i++)
  {
    printf(" ");
  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief prints tree message for layers
/// @param node
/// @param depht_of_layer
/// 
//---------------------------------------------------------------------------------------------------------------------
void printTreeLayer(Layer* node, int depth_of_layer)
{
  printWhiteSpaceForTree(depth_of_layer);
  if(node->bmp_id_ < 0)
  {
    printf("Layer %d\n", node->layer_id_); //Layer <LAYER_ID>
  }
  else
  {
    printf("Layer %d renders BMP %d at %d %d\n", node->layer_id_, node->bmp_id_, node->x_, node->y_); 
    //Layer <LAYER_ID> renders BMP <BMP_ID> at <CANVAS_X> <CANVAS_Y>
  }

  for(int i = 0; i < node->children_count_; i++)
  {
    printTreeLayer(node->children_[i], depth_of_layer + 1);
  }
}

//---------------------------------------------------------------------------------------------------------------------
///@brief it creates a tree on canvas with rood and gives info for the enxt layer id
/// @param canvas
/// @return success or error value
/// 
//---------------------------------------------------------------------------------------------------------------------
int createCanvasTree(Canvas* canvas)
{
  canvas->root_ = createRoot();
  if(canvas->root_ == NULL)
  {
    return ERROR_MEMORY_ALLOCATION;
  }
  canvas->active_layer_ = canvas->root_;
  canvas->next_layer_id_ = 1;
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief finds the layer/node through id in the tree
/// @param node
/// @param searching_id, this is the node im searching
/// @return NULL or the searching_layer i m trying to find
//---------------------------------------------------------------------------------------------------------------------
Layer* findLayerThroughIdInTree(Layer* node, int searching_id)
{
  if(node->layer_id_ == searching_id) // 2 != 5
  {
    return node;
  }
  for(int i = 0; i < node->children_count_; i++) 
  {
    Layer* searching_layer = findLayerThroughIdInTree(node->children_[i], searching_id);
    if(searching_layer != NULL)
    {
      return searching_layer;
    }
  }
  return NULL;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief tree function from the commands
/// @param canvas
/// @return success
/// 
//---------------------------------------------------------------------------------------------------------------------
int treeFunction(Canvas* canvas)
{
  printTreeLayer(canvas->root_, 0);
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief undo function from the commands 
/// if there is no children, then it gives error value, but if there is at least 1 child then
/// it writes that the active layer now is the parent layer and prints the corresponding messages
/// @param canvas
/// @return success of error value
//---------------------------------------------------------------------------------------------------------------------
int undoFunction(Canvas* canvas)
{
  if(canvas->active_layer_ == canvas->root_)
  {
    return ERROR_ALREADY_ROOT_LAYER;
  }
  canvas->active_layer_ = canvas->active_layer_->parent_; /// active layer -> parent (active layer)
  printf("Switched to layer %d\n", canvas->active_layer_->layer_id_); //Switched to layer <LAYER_ID>\n
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief switch function from the command
/// it checks if the layer id in input is number and then searches the layer id in the tree
/// trhgough switch function i cann access to the active layer, if there is no such layer it prints error value
/// @param canvas
/// @param tokens
/// @return success or error value
//---------------------------------------------------------------------------------------------------------------------
int switchFunction(Canvas* canvas, char* tokens[])
{
  int layer_id = atoi(tokens[1]);
  Layer* searching_layer = findLayerThroughIdInTree(canvas->root_, layer_id);
  if(searching_layer == NULL) 
  {
    return ERROR_LAYER_ID_NOT_FOUND;
  }
  canvas->active_layer_ = searching_layer;
  printf("Switched to layer %d\n", canvas->active_layer_->layer_id_); // Switched to layer <LAYER_ID>\n

  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief opens the file according to the path
/// rb opens the file in binary form
/// @param file_path from the input
/// @param return_file the write the file i have opened through the path
/// @return Success or the error value
//---------------------------------------------------------------------------------------------------------------------
int openFileFromPath(char* file_path, FILE** return_file)
{
  if(file_path == NULL || file_path[0] == '\0') // to check if the file path exists
  {
    return ERROR_INVALID_FILE_PATH;
  }
  FILE* file = fopen(file_path, "rb"); // to open the file, which is in the file_path and in binary form rb
  if(file == NULL)
  {
    return ERROR_CANNOT_OPEN_FILE; //-5
  }
  *return_file = file;
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief reallocates the library if it is needed
/// it checks if capacity is enough or not and then reallocates to the new array 
/// @param bib library of bmps
/// @return success of error value
//---------------------------------------------------------------------------------------------------------------------
int reallocateLibraryIfNeeded(BMP_Library* bib)
{
  if(bib->count_ < bib->capacity_)
  {
    return SUCCESS;
  }
  int new_capacity = 0;
  if(bib->capacity_ == 0)
  {
    new_capacity = 4; // capacity as default is 4
  }
  else
  {
    new_capacity = bib->capacity_ * 2;
  }
  BMP** new_arr_bmps = realloc(bib->bmps_, new_capacity * sizeof(BMP*));//reallocating the array
  if(new_arr_bmps == NULL)
  {
    return ERROR_MEMORY_ALLOCATION;
  }
  bib->bmps_ = new_arr_bmps;
  bib->capacity_ = new_capacity;

  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief prints the message in load function
/// @param file_path from the input
/// @param image BMP image i have uploaded to the program
/// 
//---------------------------------------------------------------------------------------------------------------------
void loadedMessage(char* file_path, BMP* image)
{
  printf("Loaded %s with ID %d and dimensions %d %d\n", file_path, image->BMP_id_, image->width_, image->height_);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief function for reading and loading pixels. This function was written with the help of ChatGPT
/// it validates the width and height and saves them in BGRA format in 1D array
/// @param image_file the image file where i read the pixels
/// @param pixel_offset where the pixels are situated
/// @param pixels_out pointer where i give the info
/// @param width
/// @param height
/// @return it returns Success or error value 
//---------------------------------------------------------------------------------------------------------------------
int loadPixels(FILE* image_file, uint32_t pixel_offset, uint8_t** pixels_out, int width, int height)
{
  if(pixels_out == NULL)
  {
    return ERROR_INVALID_FILE;
  }
  *pixels_out = NULL;
  int image_height;
  if(height < 0)
  {
    image_height = -height;
  }
  else
  {
    image_height = height;
  }
  if(width <= 0 || height == 0)
  {
    return ERROR_INVALID_FILE;
  }
  // one row of pixel data
  size_t row_bytes = (size_t)width * ONE_PIXEL; //row bytes without padding because some files contain not enough bytes
  // width f.e = 10 , here the pixels are 30 for example but u cannot divide it to 4
  size_t total_bytes_memory = row_bytes * (size_t)image_height;

  uint8_t*  pixel = malloc(total_bytes_memory);
  if(pixel == NULL)
  {
    return ERROR_MEMORY_ALLOCATION;
  }
  if(fseek(image_file, pixel_offset, SEEK_SET) != 0)
  {
    free(pixel);
    return ERROR_INVALID_FILE;
  }
  for(int file_row = 0; file_row < image_height; file_row++)
  {
    int mem_row;
    
    if(height > 0)
    {
      mem_row = image_height - 1 - file_row; // bottom->top file => top->bottom memory
    }
    else
    {
      mem_row = file_row;
    }
    uint8_t* destination_row = pixel + (size_t)mem_row * row_bytes;

    if(fread(destination_row, 1, row_bytes, image_file) != row_bytes)
    {
      free(pixel);
      return ERROR_INVALID_FILE;
    }
  }
  *pixels_out = pixel;
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief it reads the info from the image file i have uploaded
/// first two numbers are the signatures that it is BM, then with the help of offsets i can access to height and width
/// pixel_data_offset 10, it means that the pixels begin from the 10th position
/// @param image_file
/// @param width pointer
/// @param height pointer
/// @param pixel_offset 
/// @param bits_per_pixel 
/// @return Success or error value
//---------------------------------------------------------------------------------------------------------------------
int readBmp(FILE* image_file, int* width, int* height, uint32_t* pixel_offset, uint16_t* bits_per_pixel)
{
  char bm_sign_in_file[2];
  if(fread(bm_sign_in_file, 1, 2, image_file) != 2)
  {
    return ERROR_INVALID_FILE;
  }
  if(bm_sign_in_file[0] != 'B' || bm_sign_in_file[1] != 'M')
  {
    return ERROR_INVALID_FILE;
  }
  if(fseek(image_file, PIXEL_DATA_OFFSET, SEEK_SET)!= 0)
  {
    return ERROR_INVALID_FILE;
  }
  if(fread(pixel_offset, 4, 1, image_file) != 1)
  {
    return ERROR_INVALID_FILE;
  }
  if(fseek(image_file, OFFSET_WIDTH, SEEK_SET) != 0)
  {
    return ERROR_INVALID_FILE;
  }
  if(fread(width, SIZE_IN_BYTES, 1, image_file) != 1)
  {
    return ERROR_INVALID_FILE;
  }
  if(fseek(image_file, OFFSET_HEIGHT, SEEK_SET) != 0)
  {
    return ERROR_INVALID_FILE;
  }
  if(fread(height, SIZE_IN_BYTES, 1, image_file) != 1)
  {
    return ERROR_INVALID_FILE;
  }
  if(*width < 1 || *height == 0)
  {
    return ERROR_INVALID_FILE;
  }
  if(fseek(image_file, BPP_OFFSET, SEEK_SET) != 0)
  {
    return ERROR_INVALID_FILE;
  }
  if(fread(bits_per_pixel, 2, 1, image_file) != 1)
  {
    return ERROR_INVALID_FILE;
  }
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief it creates and stores the image i have uploaded and saves it in bmp library
/// @param image_file
/// @param bib library of bmps
/// @param file_path
/// @param width
/// @param height
/// @param pixel_offset
/// @return Success of error value
//---------------------------------------------------------------------------------------------------------------------
int createAndStoreBmp(FILE* image_file, BMP_Library* bib, char* file_path, 
                      int width, int height, uint32_t pixel_offset)
{
  int realloc_space = reallocateLibraryIfNeeded(bib);
  if(realloc_space != SUCCESS)
  {
    return realloc_space;
  }
  BMP* image = malloc(sizeof(BMP));
  if(image == NULL)
  {
    return ERROR_MEMORY_ALLOCATION;
  }
  image->width_ = width;
  image->height_ = height;
  image->BMP_id_ = bib->count_;
  image->pixels_ = NULL;

  uint8_t* pixels = NULL;
  int loading = loadPixels(image_file, pixel_offset, &pixels, width, height);
  if(loading != SUCCESS)
  {
    free(image);
    return loading;
  }
  image->pixels_ = pixels;
  bib->bmps_[bib->count_] = image;
  bib->count_++;
  loadedMessage(file_path, image);
  return SUCCESS;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief the load function from the command
/// it opens the file, read the file i have uploaded, then stores the BMP Image on the heap and closes it
/// @param tokens
/// @param bib
/// @return Success or error value
//---------------------------------------------------------------------------------------------------------------------
int loadFunction(char* tokens[], BMP_Library* bib)
{
  FILE* image_file = NULL;
  char* file_path = tokens[1];
  int width = 0;
  int height = 0;
  uint32_t pixel_offset = 0;
  uint16_t bits_per_pixel = 0;
  int check = openFileFromPath(file_path, &image_file);
  if(check != SUCCESS)
  {
    return check;
  }
  int command = readBmp(image_file, &width, &height, &pixel_offset, &bits_per_pixel);
  if(command != SUCCESS)
  {
    fclose(image_file);
    return command;
  }
  command = createAndStoreBmp(image_file, bib, file_path, width, height, pixel_offset);
  fclose(image_file);
  return command;
}

//---------------------------------------------------------------------------------------------------------------------
///@brief function for showing bmps from the command
/// if there is no bmps, the function stops
/// @param bib library of bmps
/// 
/// 
//---------------------------------------------------------------------------------------------------------------------
void showBmps(BMP_Library* bib)
{
  if(bib->count_ == 0)
  {
    return; // no bmps yet
  }
  for(int i = 0; i < bib->count_; i++)
  {
    printf("BMP %d has dimensions %d x %d\n", bib->bmps_[i]->BMP_id_, bib->bmps_[i]->width_, 
                                                                bib->bmps_[i]->height_);
  }
}

//---------------------------------------------------------------------------------------------------------------------
///@brief function for finding the bmp image works the same with the nodes 
/// @param bib library
/// @param bpm_id 
/// @return bmp image
//---------------------------------------------------------------------------------------------------------------------
BMP* findAndReturnBmpThroughId(BMP_Library* bib, int bmp_id)
{
  if(bib == NULL)
  {
    return NULL;
  }
  for(int i = 0; i < bib->count_; i++)
  {
    BMP* searching_image = bib->bmps_[i]; // image i need
    if(searching_image != NULL && searching_image->BMP_id_ == bmp_id)
    {
      return searching_image;
    }
  }
  return NULL; // if it comes here, then it is not found
}

//---------------------------------------------------------------------------------------------------------------------
///@brief crop message when user gives command crop
/// @param image the image i have cropped
/// @param old_id the id of the image used previously
/// 
//---------------------------------------------------------------------------------------------------------------------
void croppedMessage(BMP* cropped_image, int old_id)
{
  printf("Cropped ID %d to new ID %d with dimensions %d x %d\n", old_id, cropped_image->BMP_id_, cropped_image->width_,
                                                                cropped_image->height_);
}

//---------------------------------------------------------------------------------------------------------------------
///@brief crop function. It validates the input , finds the bmp image, allocates space for cropped image
/// copyies the pixels from the old one to the new one and reallocates it if it is needed
/// @param tokens input 
/// @param bib library of bmps
/// @return Success or error value
//---------------------------------------------------------------------------------------------------------------------
int cropFunction(char* tokens[], BMP_Library* bib)
{
  int bmp_id = atoi(tokens[1]);
  int top_x = atoi(tokens[2]) - 1;
  int top_y = atoi(tokens[3]) - 1;
  int bottom_x = atoi(tokens[4]) - 1;
  int bottom_y = atoi(tokens[5]) - 1;

  BMP* image = findAndReturnBmpThroughId(bib, bmp_id);
  if(image == NULL)
  {
    return ERROR_BMP_ID_NOT_FOUND;
  }
  if(top_x < 0 || top_y < 0 || bottom_x >= image->width_ || bottom_y >= image->height_ )
  {
    return ERROR_CROP_COORDINATES_OUTSIDE;
  }
  if(bottom_x < top_x || bottom_y < top_y)
  {
    return ERROR_CROP_COORDINATES_DONT_VALID_RECTANGLE;
  }
  BMP* cropped_image = malloc(sizeof(BMP));
  if(cropped_image == NULL)
  {
    printError(ERROR_MEMORY_ALLOCATION);
    return ERROR_MEMORY_ALLOCATION;
  }
  cropped_image->width_ = (bottom_x - top_x) + 1; 
  cropped_image->height_ = (bottom_y - top_y) + 1;
  cropped_image->BMP_id_ = bib->count_; 
  // After that i need to allocate the memory for new cropped_image
  int pixels = cropped_image->width_ * cropped_image->height_ * ONE_PIXEL;
  cropped_image->pixels_ = malloc(pixels);
  if(!cropped_image->pixels_)
  {
    free(cropped_image);
    return ERROR_MEMORY_ALLOCATION;
  }
  //copying the pixels from the old one to the new crop
  for(int destination_y = 0; destination_y < cropped_image->height_; destination_y++) //new coordinates in croppedimage
  {
    for(int destination_x = 0; destination_x < cropped_image->width_; destination_x++)
    {
      int old_x = top_x + destination_x;
      int old_y = top_y + destination_y;
      uint8_t* old_pixel_address = image->pixels_ + (old_y * image->width_ + old_x) * ONE_PIXEL;
      uint8_t* new_pixel_address = cropped_image->pixels_ + (destination_y * cropped_image->width_ + destination_x) 
                                  * ONE_PIXEL; 
      memcpy(new_pixel_address, old_pixel_address, ONE_PIXEL);
    }
  }
  int check = reallocateLibraryIfNeeded(bib);
  if(check != SUCCESS)
  {
    free(cropped_image->pixels_);
    free(cropped_image);
    return check;
  }
  bib->bmps_[bib->count_] = cropped_image;
  bib->count_++;
  croppedMessage(cropped_image, bmp_id);
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
///@brief place function. it checks the input , creates a new layer and adds it to the parent with function adding
/// then in canvas the new layer becomes the active layer im working now currently
/// @param bib library of bmps
/// @param tokens input
/// @return Success or error valie
//---------------------------------------------------------------------------------------------------------------------
int placeFunction(BMP_Library* bib, Canvas* canvas, char* tokens[])
{
  //place <BMP_ID> <CANVAS_X> <CANVAS_Y> <BLEND_MODE>
  int bmp_id = atoi(tokens[1]);
  int canvas_x = atoi(tokens[2]);
  int canvas_y = atoi(tokens[3]);
  BMP* bmp_image = findAndReturnBmpThroughId(bib, bmp_id);
  if(bmp_image == NULL)
  {
    return ERROR_BMP_ID_NOT_FOUND;
  }
  int bmp_width = bmp_image->width_;
  int bmp_height;
  // 10.01 TODO put this validation into another function
  if(canvas_x < 1 || canvas_y < 1)
  {
    return ERROR_CANVAS_COORDINATES_INVALID;
  }
   if(bmp_image->height_ < 0)
  {
    bmp_height = -bmp_image->height_;
  }
  else
  {
    bmp_height = bmp_image->height_;
  }
   if(canvas_x + bmp_width - 1 > canvas->width_ || canvas_y + bmp_height - 1 > canvas->height_)
  {
    return ERROR_BMP_DOESNT_FIT;
  }
  if(tokens[4] == NULL || strlen(tokens[4]) != 1) // it has to be only one character
  {
    return ERROR_INVALID_BLEND_MODE;
  }
  char blend_mode = tokens[4][0];
  //TO DO It does not check for other characters
  if(blend_mode != 'n' && blend_mode != 'm' && blend_mode != 's')
  {
    return ERROR_INVALID_BLEND_MODE;
  }
  
  Layer* new_layer = createLayer(canvas->active_layer_, canvas->next_layer_id_, bmp_id, canvas_x, canvas_y,
                    blend_mode);
  if(new_layer == NULL)
  {
    return ERROR_MEMORY_ALLOCATION;
  }

  int add_new_layer = addChildToParentLayer(canvas->active_layer_, new_layer);
  if(add_new_layer != SUCCESS)
  {
    free(new_layer);
    return add_new_layer;
  }
  canvas->active_layer_ = new_layer;
  canvas->next_layer_id_++;
  printf("Switched to layer %d\n", new_layer->layer_id_); //Switched to layer <LAYER_ID>\n
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
///@brief function for freeing the library in order not to receive the leaks
/// @param bib library of bmps
///
/// 
//---------------------------------------------------------------------------------------------------------------------
void freeLibrary(BMP_Library* bib)
{
  for(int i = 0; i < bib->count_; i++)
  {
    free(bib->bmps_[i]->pixels_);
    free(bib->bmps_[i]);
  }
  free(bib->bmps_);
  free(bib);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief function for freeing the bgra in 2d arrays
/// @param array with data bgra
/// @param height
/// 
//---------------------------------------------------------------------------------------------------------------------
void freeBgra2D(Bgra** array, int height)
{
  for(int y = 0; y < height; y++)
  {
    free(array[y]);
  }
  free(array);

}

//---------------------------------------------------------------------------------------------------------------------
///@brief function for freeing the tree
/// @param node
///
/// 
//---------------------------------------------------------------------------------------------------------------------
void freeTree(Layer* node)
{
  for(int i = 0; i < node->children_count_; i++)
  {
    freeTree(node->children_[i]);
  }
  free(node->children_);
  free(node);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief blend mode normal function
/// there is some calculation according to readme
/// @param pixel_a the pixel color that is already on the canvas (the background or lower layer)
/// @param pixel_b the pixel color from the layer currently being drawn (the foreground or top layer)
/// @param pixel_alpha_b transparency of B
/// @return uint8_t result_pixel
//---------------------------------------------------------------------------------------------------------------------
uint8_t blendModeNormal(int pixel_a, int pixel_b, double pixel_alpha_b)
{
  double alpha = pixel_alpha_b / 255.0; // alpha_b / 255
  // R=α⋅B+(1−α)⋅A
  double result_pixel = alpha * pixel_b + (1 - alpha) * pixel_a;
  return (uint8_t)result_pixel;
}

//---------------------------------------------------------------------------------------------------------------------
///@brief blendmode multiply with the calculation according to the Readme
/// @param pixel_a the pixel color that is already on the canvas (the background or lower layer)
/// @param pixel_b the pixel color from the layer currently being drawn (the foreground or top layer)
/// @return uint8_t result_pixel
//---------------------------------------------------------------------------------------------------------------------
uint8_t blendModeMultiply(int pixel_a, int pixel_b)
{
  int result_pixel = (pixel_a * pixel_b) / 255;
  return (uint8_t)result_pixel;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief blendmode sub according to the readme
/// @param pixel_a the pixel color that is already on the canvas (the background or lower layer)
/// @param pixel_b the pixel color from the layer currently being drawn (the foreground or top layer)
/// @return result_pixel
//---------------------------------------------------------------------------------------------------------------------
uint8_t blendModeSub(int pixel_a, int pixel_b)
{
  int max_pixel;
  int min_pixel;

  if(pixel_a > pixel_b)
  {
    max_pixel = pixel_a;
    min_pixel = pixel_b;
  }
  else
  {
    max_pixel = pixel_b;
    min_pixel = pixel_a;
  }
  int result_pixel = max_pixel - min_pixel;
  return (uint8_t)result_pixel;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief blending the pixel according to the mode
/// @param pixel_a the pixel color that is already on the canvas (the background or lower layer)
/// @param pixel_b the pixel color from the layer currently being drawn (the foreground or top layer)
/// @param blend_mode
/// @return result bgra
//---------------------------------------------------------------------------------------------------------------------
Bgra blendModePixel(Bgra pixel_a, Bgra pixel_b, char blend_mode)
{
  Bgra result;
  result.a_ = ALL_WHITE_PIXELS_WITH_ALPHA; // 255
  if(blend_mode == 'n')
  {
    result.b_ = blendModeNormal(pixel_a.b_, pixel_b.b_, pixel_b.a_);
    result.g_ = blendModeNormal(pixel_a.g_, pixel_b.g_, pixel_b.a_);
    result.r_ = blendModeNormal(pixel_a.r_, pixel_b.r_, pixel_b.a_);
  } else if(blend_mode == 'm')
  {
    result.b_ = blendModeMultiply(pixel_a.b_, pixel_b.b_);
    result.g_ = blendModeMultiply(pixel_a.g_, pixel_b.g_);
    result.r_ = blendModeMultiply(pixel_a.r_, pixel_b.r_);
  } else if(blend_mode == 's')
  {
    result.b_ = blendModeSub(pixel_a.b_, pixel_b.b_);
    result.g_ = blendModeSub(pixel_a.g_, pixel_b.g_);
    result.r_ = blendModeSub(pixel_a.r_, pixel_b.r_);  
  }
  return result;
}

//TODO Write a function that finds pixel intersections between the layers
//after that i could store them in the array


//---------------------------------------------------------------------------------------------------------------------
///@brief function for gathering all the layers 
/// @param canvas
///
/// @return layers with the type gatheredLayers
//---------------------------------------------------------------------------------------------------------------------
GatheredLayers gatherAllLayers(Canvas* canvas)
{
  GatheredLayers result;
  result.count_ = 0;
  result.array_of_layers_ = NULL;
  int buffer = 5;
  int count_layers = 0;
  Layer* current_node = canvas->active_layer_;
  Layer** array_of_layers = malloc(buffer * sizeof(Layer*));
  if(array_of_layers == NULL)
  {
    return result;
  }
  while(current_node != canvas->root_)
  {
    count_layers++;
    if(count_layers >= buffer)
    {
      buffer = buffer * 2;
      Layer** temp = realloc(array_of_layers, buffer * sizeof(Layer*));
      if(temp == NULL)
      {
        printError(ERROR_MEMORY_ALLOCATION);
        free(array_of_layers);
        return result;
      }
      array_of_layers = temp;
    }
    array_of_layers[count_layers - 1] = current_node;
    current_node = current_node->parent_;
  }
  array_of_layers[count_layers] = current_node; // this is my root node
  count_layers++;
  
  for(int left = 0, right = count_layers - 1; left < right; left++, right--)
  {
    Layer* temp = array_of_layers[left]; // root
    array_of_layers[left] = array_of_layers[right]; // my root became the last one
    array_of_layers[right] = temp; // my last node became the first one
  }
  GatheredLayers gathered_layers;
  gathered_layers.count_ = count_layers;
  gathered_layers.array_of_layers_ = array_of_layers;

  return gathered_layers;
}

//---------------------------------------------------------------------------------------------------------------------
///@brief function for converting bmp pixels from 1d to 2d array. The function is made 
/// @param image
/// 
/// @return bgra 2d 
//---------------------------------------------------------------------------------------------------------------------
Bgra** bmpPixelsTo2D(BMP* image)
{
  int width = image->width_;
  int height = image->height_;

  Bgra** pixels2d = malloc(height * sizeof(Bgra*));
  if(pixels2d == NULL) 
  {
    return NULL;
  }

  for(int y = 0; y < height; y++)
  {
    pixels2d[y] = malloc(width * sizeof(Bgra));
    if(pixels2d[y] == NULL)
    {
      for(int k = 0; k < y; k++) 
      {
        free(pixels2d[k]);
      }
      free(pixels2d);
      return NULL;
    }
    int my_index = 0;
    for(int x = 0; x < width; x++)
    {
      my_index = (y * width + x) * ONE_PIXEL;
      pixels2d[y][x].b_ = image->pixels_[my_index + 0];
      pixels2d[y][x].g_ = image->pixels_[my_index + 1];
      pixels2d[y][x].r_ = image->pixels_[my_index + 2];
      pixels2d[y][x].a_ = image->pixels_[my_index + 3];
    } // now i work with 2d pixels array
  }

  return pixels2d;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief creating empty 2d canvas all white
/// @param canvas
///
/// @return bgra canvas_2d array
//---------------------------------------------------------------------------------------------------------------------
Bgra** createEmpty2DCanvas(Canvas* canvas)
{
  int width = canvas->width_;
  int height = canvas->height_;
  Bgra** canvas_2d = malloc(height * sizeof(Bgra*));
  if(canvas_2d == NULL)
  {
    return NULL;
  }
  for(int y = 0; y < height; y++)
  {
    canvas_2d[y] = malloc(width * sizeof(Bgra));
    if(canvas_2d[y] == NULL)
    {
      for(int j = 0; j < y; j++)
      {
        free(canvas_2d[j]);
      }
      free(canvas_2d);
      return NULL;
    }
    for(int x = 0; x < width; x++)
    {
      canvas_2d[y][x].a_ = ALL_WHITE_PIXELS_WITH_ALPHA;
      canvas_2d[y][x].b_ = ALL_WHITE_PIXELS_WITH_ALPHA;
      canvas_2d[y][x].g_ = ALL_WHITE_PIXELS_WITH_ALPHA;
      canvas_2d[y][x].r_ = ALL_WHITE_PIXELS_WITH_ALPHA;
    }
  }
  return canvas_2d;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief rendering the canvas to 2d, it creates empty canvas and gathers all the layers,
/// converts pixels to 2d and blends the layers between each other, in which one layer is top and another under 
/// @param canvas
/// @param bib library
/// @return rendered canvas
//---------------------------------------------------------------------------------------------------------------------
Bgra** renderCanvasTo2D(Canvas* canvas, BMP_Library* bib)
{
  Bgra** empty_canvas = createEmpty2DCanvas(canvas);
  if(empty_canvas == NULL)
  {
    return NULL;
  }
  GatheredLayers all_layers = gatherAllLayers(canvas);
  
  for(int i = 1; i < all_layers.count_; i++) // i = 1, because i = 0 is my root
  {
    Layer* each_layer = all_layers.array_of_layers_[i];
    BMP* bmp_image = findAndReturnBmpThroughId(bib, each_layer->bmp_id_);
    int width = bmp_image->width_;
    int height = bmp_image->height_;
    // place 2 4 3 n
    int coordinate_x = each_layer->x_ - 1; // 4 - 1 = 3
    int coordinate_y = each_layer->y_ - 1; // 0 based // 3 - 1 = 2
    Bgra** bmp_2d = bmpPixelsTo2D(bmp_image);
    if(bmp_2d == NULL)
    {
      free(all_layers.array_of_layers_);
      freeBgra2D(empty_canvas, canvas->height_);
      return NULL;
    }
    for(int y = 0; y < height; y++)
    {
      for(int x = 0; x < width; x++)
      {
        //these coodrinates are on the canvas
        int canvas_x = coordinate_x + x; // x = 3 + 0 = 3; 3 + 1 = 4
        int canvas_y = coordinate_y + y; // y = 2 + 0 = 2; 2 + 1 = 3
        Bgra lower = empty_canvas[canvas_y][canvas_x];
        Bgra upper = bmp_2d[y][x];
        empty_canvas[canvas_y][canvas_x] = blendModePixel(lower, upper, each_layer->blend_mode_);
      }
    }
    freeBgra2D(bmp_2d, height);
  }

  free(all_layers.array_of_layers_);
  return empty_canvas;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief function for printing from the command, it renders the canvas to 2d
/// then prints the output accroding the readme and frees it
/// @param canvas
/// @param bib library of bmps
/// @return success
//---------------------------------------------------------------------------------------------------------------------
int printFunction(Canvas* canvas, BMP_Library* bib)
{
  Bgra** rendered_canvas = renderCanvasTo2D(canvas, bib);
  if(rendered_canvas == NULL)
  {
    return ERROR_MEMORY_ALLOCATION;
  }
  printf("    ");
  for(int x = 1; x <= canvas->width_; x++)
  {
    printf("%02d", x);
    if(x < canvas->width_)
    {
      printf(" ");
    }
  }
  printf("\n");
  for(int y = 1; y <= canvas->height_; y++)
  {
    printf("%02d|", y);
    for(int x = 1; x <= canvas->width_; x++)
    {
      Bgra pixel = rendered_canvas[y - 1][x - 1];
      printf("\033[38;2;%u;%u;%um███\033[0m", pixel.r_, pixel.g_, pixel.b_); // \033[38;2;<R>;<G>;<B>m███\033[0m
    }
    printf("|\n");
  }
  printf("  ");
  int dashes = canvas->width_ * 3 + 2;
  for(int i = 0; i < dashes; i++)
  {
    printf("-");
  }
  printf("\n");
  freeBgra2D(rendered_canvas, canvas->height_);
  return SUCCESS;
}
//---------------------------------------------------------------------------------------------------------------------
///@brief function from the repo document
/// @param BmpHeader
/// @param width
/// @param height
//---------------------------------------------------------------------------------------------------------------------
void fillBmpHeaderDefaultValues(BmpHeader*, int width, int height);

//---------------------------------------------------------------------------------------------------------------------
///@brief function for saving the edited image with the help of bmp.c. The function was written with help of AI
/// @param bgra rendered image
/// @param file_path
/// @param width
/// @param height
/// @return SUccess or error value
//---------------------------------------------------------------------------------------------------------------------
int saveBmp(Bgra** rendered, char* file_path, int width, int height)
{
  FILE* save_file = fopen(file_path, "wb");
  if(!save_file)
  {
    return ERROR_INVALID_FILE_PATH;
  }
  BmpHeader header;
  fillBmpHeaderDefaultValues(&header, width, height);
  // take one elemnt sizeof BmpHeader from Header and save it to file
  if(fwrite(&header, sizeof(BmpHeader), 1, save_file) != 1) 
  {
    fclose(save_file);
    return ERROR_INVALID_FILE;
  }
  //BMP stores pixels from bottom-up
  // i need to write rows from bottom to top
  for(int y = height - 1; y >= 0; y--)
  {
    for(int x = 0; x < width; x++)
    {
      Bgra rendered_pixels = rendered[y][x];
      uint8_t pixels[4] = {rendered_pixels.b_, rendered_pixels.g_, rendered_pixels.r_, rendered_pixels.a_};
      if(fwrite(pixels, 1, 4, save_file) != 4) // write 4 elements with 1 bite into save_File
      {
        fclose(save_file);
        return ERROR_INVALID_FILE;
      }
    }
  }
  if (fclose(save_file) != 0)
  {
    return ERROR_INVALID_FILE;
  }
  return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
///@brief save function from the command, it checks the input, saves the file, and then prints the message
/// @param canvas
/// @param bib library of bmps
/// @param tokens input
/// @return success or error value
//---------------------------------------------------------------------------------------------------------------------
int saveFunction(Canvas* canvas, BMP_Library* bib, char* tokens[])
{
  char* file_path = tokens[1]; // file path i need
  Bgra** rendered  = renderCanvasTo2D(canvas, bib);
  if(rendered == NULL)
  {
    return ERROR_MEMORY_ALLOCATION;
  }
  int bmp = saveBmp(rendered, file_path, canvas->width_, canvas->height_);
  freeBgra2D(rendered, canvas->height_);
  if(bmp == SUCCESS)
  {
    printf("Successfully saved image to %s\n", file_path); //Successfully saved image to <FILE_PATH>\n
  }
  return bmp;
  
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief it runs the commands from the user and switches on the corresponding command
/// @param canvas
/// @param bib library of bmps
/// @param command from the user
/// @param tokens input
//---------------------------------------------------------------------------------------------------------------------
int runCommands(Canvas* canvas, BMP_Library* bib, int command, char* tokens[])
{
  if(command == QUIT) 
  return QUIT;
  if(command == HELP)
  {
    helpMessage();
    return SUCCESS;
  }
  int function = SUCCESS;
  if(command == LOAD)
  {
    function = loadFunction(tokens, bib);
    if(function != SUCCESS)
    printError(function);
  }
  if(command == CROP)
  {
    function = cropFunction(tokens, bib);
    if(function != SUCCESS)
    printError(function);
  }
  if(command == PLACE)
  {
    function = placeFunction(bib, canvas, tokens);
    if(function != SUCCESS)
    printError(function);
  }
  if(command == BMPS)
  {
    showBmps(bib);
    function = SUCCESS;
  }
  if(command == UNDO)
  {
    function = undoFunction(canvas);
    if(function != SUCCESS)
    printError(function);
  }
  if(command == SWITCH)
  {
    function = switchFunction(canvas, tokens);
    if(function != SUCCESS)
    printError(function);
  }
  if(command == SAVE)
  {
    function = saveFunction(canvas, bib, tokens);
    if(function != SUCCESS)
    printError(function);
  }
  if(command == PRINT)
  {
    function = printFunction(canvas, bib);
    if(function != SUCCESS)
    printError(function);
  }
  if(command == TREE)
  {
    function = treeFunction(canvas);
    if(function != SUCCESS)
    printError(function);
  }
  if(command == ERROR_COMMAND_UNKNOWN)
  {
    printError(ERROR_COMMAND_UNKNOWN);
    return command;
  }
  return function;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief main loop of commands
/// @param bib library of bmps
/// @param canvas
/// @return SUccess or error value
//---------------------------------------------------------------------------------------------------------------------
int commandLoop(BMP_Library* bib, Canvas* canvas)
{
  char* tokens[MAX_TOKENS]; 
  while(1)
  {
    printf(" > ");
    char* input_prompt = getInput();
    if(input_prompt == NULL)
    {
      return SUCCESS;
    }
    normalizeInput(input_prompt);
    int count_words = tokenizeStringInput(input_prompt, tokens, MAX_TOKENS);
    int validation_of_input = checkInput(tokens, count_words);
    int choice = runCommands(canvas, bib, validation_of_input, tokens);
    free(input_prompt);
    if(choice == QUIT)
    {
      return SUCCESS;
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief main function of this programm. it allocates canvas on the stack, parses the arguments
/// creates the library omn the heap, handles the error messages and frees the memory
/// @param argc 
/// @param argv
/// @return corresponding enum value
//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  Canvas canvas = {0}; // is ok got advised by mentor
  int arguments = parseArguments(argc, argv, &canvas);
  if(arguments != SUCCESS)
  {
    printError(arguments);
    return arguments;
  }

  BMP_Library* bib = createLibraryOnHeap();
  if(bib == NULL)
  {
    printError(ERROR_MEMORY_ALLOCATION);
    return ERROR_MEMORY_ALLOCATION;
  }

  welcomeMessage(canvas.width_, canvas.height_);
  int tree = createCanvasTree(&canvas);
  if(tree != SUCCESS)
  {
    printError(tree);
    return tree;
  }
  int photoshop_editor = commandLoop(bib, &canvas);
  if(photoshop_editor != SUCCESS)
  {
    return photoshop_editor;
  }
  // runSelfTests();
  freeTree(canvas.root_);
  canvas.root_ = NULL;
  canvas.active_layer_ = NULL;
  freeLibrary(bib);


  return 0;
}

