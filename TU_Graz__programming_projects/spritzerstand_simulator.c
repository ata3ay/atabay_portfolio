//---------------------------------------------------------------------------------------------------------------------
/// a1.c
/// Spritzerstand Simulator
///
/// Author: 12046429 Atabay Dinmukhamed
//---------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>  

#define GLASS_VALUE 500
#define GLASS_VALUE_2 300
#define PRICE_SMALL 1.5
#define PRICE_NORMAL 2
#define MENU_ITEMS 3

//---------------------------------------------------------------------------------------------------------------------
/// @brief struct for the Order
/// 
///
///
typedef struct _Order_
{
  int drink_order_;
  int glass_size_;
  int tip_;
  char* drinks_;
  float deposit_;
} Order;

//---------------------------------------------------------------------------------------------------------------------
/// @brief struct for Returned glasses
///
///
///
typedef struct _Return_
{
  float deposit_return_;
  int glass_size_;
  int number_glasses_;
} Return;

//---------------------------------------------------------------------------------------------------------------------
///@brief prints Welcome Message at the beginning of the Simulator
///
///
///
void welcomeMessage(void)
{
  printf("Welcome to the Spritzerstand Simulator!!!\n");
}

//---------------------------------------------------------------------------------------------------------------------
///@brief it prints menu message with questions of assisting for the user
/// It has 3 options
///
///
void menuMessage(void)
{
  printf("\nHow can I assist you today?\n");
  printf("1. I'd like to return my glasses.\n");
  printf("2. I'd like to order.\n");
  printf("3. Quit\n");
  printf(" > ");
}

//---------------------------------------------------------------------------------------------------------------------
///@brief prints tip message question
///
///
///
void tipMessage(void)
{
  printf("\nHow much would you like to tip? (in %%)\n");
  printf(" > ");
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief prints a question for the size of the glass for the user
///
///
///
void glassSizeMessage(void)
{
  printf("\nSelect a glass size (in ml)\n");
  printf("- 300\n");
  printf("- 500\n");
  printf(" > ");
}

//---------------------------------------------------------------------------------------------------------------------
///@brief prints end the Message of the Simulator
///
///
///
void endMessage(void)
{
  printf("--------------------------------\n");
  printf("Here you go... have a pleasant evening!\n");
}

//---------------------------------------------------------------------------------------------------------------------
///@brief glass validation function, where the size of the GLASS_VALUE_2 or GLASS_VALUE
///@param glass_order struct which we need to select the size in if statement
///
///@return it returns 1 if the choice is GLASS_VALUE_2 or 500, or returns 0 if the validation failed
int glassValidation(Order* glass_order)
{
  if(glass_order->glass_size_ == GLASS_VALUE_2 || glass_order->glass_size_ == GLASS_VALUE)
  {
    return 1;
  }
  else 
  {
    return 0;
  }
}


//---------------------------------------------------------------------------------------------------------------------
///@brief function of running the order of glasses
///@param glass_order struct to save the data into glass that user ordered
///@param size number of menu items
///@param drinks menu
void runOrder(char* drinks[])
{
  Order glass_order;
  float prices[] = {2.5, 3, 1};
  while(1)
  {
    printf("\nWhat are you feeling today?\n");
    for(int drinks_index = 0; drinks_index  < MENU_ITEMS; drinks_index++)
    {
      printf("%d. %s\n", drinks_index + 1, drinks[drinks_index]);
    }
    printf(" > ");
    scanf("%d", &glass_order.drink_order_);
    if(glass_order.drink_order_ >= 1 && glass_order.drink_order_ <= MENU_ITEMS)
    {
      break;
    }
    else 
    {
      printf("[Error]: Hmm I don't seem to have that drink.\n");
    }
  }
  while(1)
  {
    glassSizeMessage();
    scanf("%d", &glass_order.glass_size_);
    int glass_validation = glassValidation(&glass_order);
    if(glass_validation == 0)
    {
      printf("[Error]: Sorry that glass size is invalid.\n");
    }
    if(glass_validation == 1)
    {
      break;
    }
  }
  if(glass_order.glass_size_ == GLASS_VALUE_2)
  {
    glass_order.deposit_ = PRICE_SMALL;
  }
  if(glass_order.glass_size_ == GLASS_VALUE)
  {
    glass_order.deposit_= PRICE_NORMAL;
  }
  float deposit = glass_order.deposit_;
  while(1)
  {
    tipMessage();
    scanf("%d", &glass_order.tip_);
    int tip = glass_order.tip_;
    if(tip >= 0 && tip <= 100)
    {
      printf("\n--------------------------------\n");
      printf("- %d ml glass of %s\n", glass_order.glass_size_, drinks[glass_order.drink_order_ - 1]);
      printf("- Drink price: %.2f €\n", prices[glass_order.drink_order_ - 1]);
      printf("- Deposit: %.2f €\n", deposit);
      float sum_no_tip = deposit + prices[glass_order.drink_order_ - 1];
      printf("- Total without tip: %.2f €\n", sum_no_tip);
      printf("- Tip: %d %%\n", tip);
      float calculate_price = (sum_no_tip / 100) * tip;
      float total_price = sum_no_tip + calculate_price;
      printf("- Total: %.2f €\n", total_price);
      endMessage();
      break;
    } 
    else
    {
      printf("[Error]: I cannot accept that. Please insert an amount from 0 to 100.\n");
    }
  }
}
//---------------------------------------------------------------------------------------------------------------------
///@brief function for returning the glasses
///@param struct that saves the data from the user for returning the glass
///
///
void runReturn(void)
{
  Return glass_return;

  while(1)
  {
    printf("\nHow many glasses do you want to return?\n");
    printf(" > ");
    scanf("%d", &glass_return.number_glasses_);

    if(glass_return.number_glasses_ > 0)
    {
      break;
    } else
    {
      printf("[Error]: How do you plan on returning that?\n");
    }
  }

  while(1)
  {
    glassSizeMessage();
    scanf("%d", &glass_return.glass_size_);

    if(glass_return.glass_size_ == GLASS_VALUE || glass_return.glass_size_ == GLASS_VALUE_2)
    {
      break;
    } 
    else
    {
      printf("[Error]: Sorry that glass size is invalid.\n");
    }
  }

  if(glass_return.glass_size_ ==  GLASS_VALUE)
  {
    glass_return.deposit_return_ = PRICE_NORMAL;
  }
  if(glass_return.glass_size_ == GLASS_VALUE_2)
  {
    glass_return.deposit_return_ = PRICE_SMALL;
  }

  int glass_count = glass_return.number_glasses_;
  float deposit_refund = glass_return.deposit_return_;
  float sum = glass_count * deposit_refund;

  printf("\n--------------------------------\n");
  printf("You returned %d glasses at %.2f € each.\n", glass_count, deposit_refund);
  printf("Your total cashback is: %.2f €\n", sum);
  endMessage();
}

//---------------------------------------------------------------------------------------------------------------------
///@brief function that runs the menu from the start
///@param drinks as menu of drinks
///
void runMenu(char* drinks[])
{
  while(1)
  {
    menuMessage();
    int input;
    scanf("%d", &input);

    if(input == 1)
    {
      runReturn();
    }
    else if(input == 2)
    {
      runOrder(drinks);
    }
    else if(input == 3)
    {
      break;
    } 
    else 
    {
      printf("[Error]: Please choose a valid option!\n");
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
///@brief main function of the Simulator
///
///
///@return returns 0
int main(void)
{
  char* drinks[] = {"Virgin Cuba Libre", "Carbonas rhoncus bibendum", "Fizzy water"};
  welcomeMessage();
  runMenu(drinks);
  
  return 0;

}



