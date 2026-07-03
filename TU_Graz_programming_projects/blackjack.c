//------------------------------------------------------------------------------
// a3.c
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
#include <stdlib.h>  
#include <string.h>
#include <ctype.h>

#define CARD_PER_DECK 52

enum SUITS
{
  CLUBS, //0
  DIAMONDS, // 1
  SPADES, // 2
  HEARTS// 3
};

enum SCORE_RESULT
{
  BLACKJACK_DEALER,
  BLACKJACK_PLAYER,
  BUSTED
};
//------------------------------------------------------------------------------
///
/// Enum definitions for suits, commands, game results
/// 
///
//------------------------------------------------------------------------------
enum COMMANDS
{
  HIT, // 0
  QUIT,
  STAND,
  INVALID, // 3
  DELETE
};
//------------------------------------------------------------------------------
///
/// Enum definitions for suits, commands, game results
/// 
///
//------------------------------------------------------------------------------
enum RESULTS
{
  LOSE,
  WIN,
  DRAW,
  BUST
};
//------------------------------------------------------------------------------
///
/// Enum definitions for suits, commands, game results
/// 
///
//------------------------------------------------------------------------------
struct Argument
{
  int amount_of_players_;
  int amount_of_decs_;
  int seed;
  char* welcome_file_;
};

struct Card
{
  int suit_;
  int value_;
  int symbol_;
  int card_is_down_; // up - 0, down - 1
};

struct Player
{
  int last_command_;
  int cards_handed_;
  struct Card** deck_;
};

//------------------------------------------------------------------------------
///
/// Prints welcome message to the Password manager.
/// 
///
//------------------------------------------------------------------------------
void welcomeMessage(char* filename)
{
  FILE* file = fopen(filename, "r"); //r for read
  if(file == NULL)
  {
    printf("Error: Failed to open file!\n");
    return;
  }
  char buffer[256];
  while (fgets(buffer, sizeof(buffer), file) != NULL)
  {
    printf("%s", buffer);
  }
  fclose(file);
}
//------------------------------------------------------------------------------
///
/// frees deck of cards
/// 
///
//------------------------------------------------------------------------------
void freeCardDeck(struct Card** cards, int cards_in_deck) 
{
  for(int i = 0; i < cards_in_deck; i++) // i has to be smaller that cards_in_deck
  {
    if(cards[i])
    {
      free(cards[i]); // for one card
    }
  }
  free(cards); //for deck
}
//------------------------------------------------------------------------------
///
/// frees table
/// 
///
//------------------------------------------------------------------------------
void freeTable(struct Player** bj_table, int amount_of_players)
{
  for (int i = 0; i < amount_of_players; i++)
  {
    freeCardDeck(bj_table[i]->deck_, bj_table[i]->cards_handed_);
    free(bj_table[i]);
  }
  free (bj_table);
}
//------------------------------------------------------------------------------
///
/// checks if the string contains digit
/// 
///
//------------------------------------------------------------------------------
int stringIsDigit(char* string)
{
  for(int i = 0; string[i] != '\0'; i++)
  {
    if(isdigit(string[i]) == 0)
    {
      return 0;
    }
  }
  return 1;
}

struct Argument parseArguments(char* argv[], int count)
{
    struct Argument arguments = {
        .amount_of_players_ = 1,
        .amount_of_decs_ = 1,
        .seed = 0,
        .welcome_file_ = "welcome_message.txt"
    };

    if (count > 5) {
        // Too many arguments
        arguments.amount_of_players_ = -2; // use -2 for "too many arguments"
        return arguments;
    }

    if (count >= 2) {
        if (stringIsDigit(argv[1])) {
            arguments.amount_of_players_ = atoi(argv[1]);
        } else {
            arguments.amount_of_players_ = -1;
            return arguments;
        }
    }

    if (count >= 3) {
        if (stringIsDigit(argv[2])) {
            arguments.amount_of_decs_ = atoi(argv[2]);
        } else {
            arguments.amount_of_players_ = -1;
            return arguments;
        }
    }

    if (count >= 4) {
        if (stringIsDigit(argv[3])) {
            arguments.seed = atoi(argv[3]);
        } else {
            arguments.amount_of_players_ = -1;
            return arguments;
        }
    }

    if (count == 5) {
        arguments.welcome_file_ = argv[4];
    }

    return arguments;
}

//------------------------------------------------------------------------------
///
/// calculates the score
/// 
///
//------------------------------------------------------------------------------
int score(struct Card** player_hand, int cards_handed)
{
  int score = 0;
  int ace_in_hand = 0;

  for (int i = 0; i < cards_handed; i++)
  {
    int value = player_hand[i]->value_;
    score += value;
    if (value == 11)
    {
      ace_in_hand++;
    }
  }

  while (score > 21 && ace_in_hand > 0)
  {
    score -= 10;
    ace_in_hand--;
  }
  return score;
}
//------------------------------------------------------------------------------
///
/// validates command
/// 
///
//------------------------------------------------------------------------------
int command(char* user_input) //strcmp, and use enum 
{
  if (strcmp("quit", user_input) == 0)
  {
    return QUIT;
  } else if (strcmp("hit", user_input) == 0)
  {
    return HIT;
  } else if (strcmp("stand", user_input) == 0)
  {
    return STAND;
  } else if (strcmp("delete", user_input) == 0)
  {
    return DELETE;
  } else
  {
    return INVALID;
  }
}
//------------------------------------------------------------------------------
///
/// gets the input, allocates and reallocates the memory
/// 
///
//------------------------------------------------------------------------------
char* getInput(void)
{
  int buffer = 50;
  int index = 0;
  char* get_input = malloc(buffer * sizeof(char));
  char* symbol_now = get_input; // temp variable to scroll through the string
  while (1)
  {
    char ch = getchar();// takes the symbols from the input buffer
    if(ch == '\n') // it ends when the new line appears
    {
      break;
    }
    if(index + 1 > buffer)
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
  printf("%s", get_input);
  return get_input;
}
//------------------------------------------------------------------------------
///
/// handouts the card
/// 
///
//------------------------------------------------------------------------------
void deleteCard(struct Player* player)
{
  free(player->deck_[player->cards_handed_ - 1]);
  player->cards_handed_--;
  
}
void handOutCard(struct Card** shuffled_cards, struct Card** player_hand, int cards_handed, int amount_of_cards, 
int card_is_down)
{
  int index = cards_handed;
  if(card_is_down == 1)
  {
    shuffled_cards[0]->card_is_down_ = 1;
  }
  player_hand[index] = shuffled_cards[0];
  for(int i = 1; i < amount_of_cards; i++)
  {
    shuffled_cards[i-1] = shuffled_cards[i]; 
  }
}
//------------------------------------------------------------------------------
///
/// generates the table of players
/// 
///
//------------------------------------------------------------------------------
struct Player** generateTable(int amount_of_players, int amount_of_decks)
{
  struct Player** players = malloc(amount_of_players * sizeof(struct Player*));
  if(!players)
  {
    return NULL;
  }
  for(int i = 0; i < amount_of_players; i++)
  {
    struct Player* player = malloc(sizeof(struct Player));
    if(!player)
    {
      return NULL;
    }
    player->cards_handed_ = 0;
    player->last_command_ = -1; // not to confuse with valid commands
    player->deck_ = malloc(CARD_PER_DECK * amount_of_decks * sizeof(struct Card*));// no cards, but memory
    players[i] = player;
  }
  return players;
}
//------------------------------------------------------------------------------
///
/// prints the lines for the cards
/// 
///
//------------------------------------------------------------------------------
void printLine(char** card_line, int cards_in_hand)
{
  for(int i = 0; i < cards_in_hand; i++)
  {
    if(i == 0)
    {
      printf(" %s  ", card_line[i]);
    } else if (i == cards_in_hand - 1)
    {
      printf("%s", card_line[i]);
    } else
    {
    printf("%s  ", card_line[i]);
    }
  }
  printf("\n");
}
//------------------------------------------------------------------------------
///
/// displays the cards of the players
/// 
///
//------------------------------------------------------------------------------
void displayCards(struct Card** player_hand, int cards_in_hand)
{
  char** border_line = malloc(cards_in_hand * sizeof(char*));
  char** suits_line = malloc(cards_in_hand * sizeof(char*));
  char** symbol_line = malloc(cards_in_hand * sizeof(char*));
  char* line = "+-----+";
  char* symbol_string = "|  x  |";
  for(int i = 0; i < cards_in_hand; i++)
  {
    border_line[i] = line;
    int suit = player_hand[i]->suit_;
    int symbol_int = player_hand[i]->symbol_;
    int card_is_down = player_hand[i]->card_is_down_;
    if(suit == DIAMONDS)
    {
      char* suit_line = "|♦️   ♦️|";
      suits_line[i] = suit_line;
    } else if (suit == CLUBS) 
    {
      char* suit_line = "|♣️   ♣️|";
      suits_line[i] = suit_line;
    } else if (suit == SPADES)
    {
      char* suit_line = "|♠️   ♠️|";
      suits_line[i] = suit_line;
    } else if (suit == HEARTS)
    {
      char* suit_line = "|♥️   ♥️|";
      suits_line[i] = suit_line;
    }
    char symbol_char = '\0';
    if(symbol_int < 10)
    {
      symbol_char = symbol_int + '0'; //to make int - char
    } else if (symbol_int == 11)
    {
      symbol_char = 'J';
    } else if (symbol_int == 12)
    {
      symbol_char = 'Q';
    } else if (symbol_int == 13)
    {
      symbol_char = 'K';
    } else if (symbol_int == 14)
    {
      symbol_char = 'A';
    }
    symbol_line[i] = malloc(8 * sizeof(char));//memory allocation for 1 string
    if(symbol_int == 10)
    {
      strcpy(symbol_line[i], "|  10 |");
    } else
    {
      strcpy(symbol_line[i], symbol_string); // from stack to heap
      symbol_line[i][3] = symbol_char; // to put symbol for  x
    }
    if(card_is_down == 1)
    {
      char* down_card_line = "|#####|";
      suits_line[i] = down_card_line;
      strcpy(symbol_line[i], down_card_line);
    }
  }
  printLine(border_line, cards_in_hand);
  printLine(suits_line, cards_in_hand);
  printLine(symbol_line, cards_in_hand);
  printLine(suits_line, cards_in_hand);
  printLine(border_line, cards_in_hand);
  for(int i = 0; i < cards_in_hand; i++)
    {
      free(symbol_line[i]);
    }
  free(symbol_line);
  free(border_line);
  free(suits_line);
}
//------------------------------------------------------------------------------
/// this functions was generated by chatGPT 
/// shuffling the cards
///
///
//------------------------------------------------------------------------------
void shuffledShue(struct Card** cards, int amount_of_cards, int seed)
{
    srand(seed); // exactly once
    for (int i = amount_of_cards - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        struct Card* temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}
//------------------------------------------------------------------------------
///
/// generates the deck
/// 
///
//------------------------------------------------------------------------------
struct Card** generateDeck(int amount_of_decks)// i have to return struct
{
  int amount_of_cards = 52 * amount_of_decks;

  struct Card** cards = malloc(amount_of_cards * sizeof(struct Card*)); 
  // Card** for an array of pointers because every card is a represented as a pointer
  if (!cards)
  {
    return NULL;
  }
  int index = 0; //to know where i have put the card
  for(int i = 0; i < amount_of_decks; i++)//because every card is a represented as a pointer
  { // for decks
    for (int k = 0; k < 4; k++)
     // symbol
    {
      for (int j = 2; j < 15; j++) // suit
      {
        struct Card* new_card = malloc(sizeof(struct Card)); // memory allocation for 1 card
        if (!new_card)
        {
          freeCardDeck(cards, index);
          return NULL;
        }
        new_card->suit_ = k;
        new_card->symbol_ = j;
        new_card->card_is_down_= 0;
        if (j > 10 && j < 14)
        {
          new_card->value_ = 10; // for JQK
        } else if (j == 14)
        {
          new_card->value_ = 11; // for A
        } else {
          new_card->value_ = j; // for cards with number
        } 
        cards[index] = new_card; // to store in array
        index++;
      }
    }
  }
  return cards;
}
//------------------------------------------------------------------------------
///
/// ends the round
/// 
///
//------------------------------------------------------------------------------
int endOfRound(struct Player* player, struct Player* dealer)
{
  int dealer_score = score(dealer->deck_, dealer->cards_handed_);
  int player_score = score(player->deck_, player->cards_handed_);

  if (player_score > 21)
  {
    return LOSE;
  }
  if (dealer_score < player_score)
  {
    return WIN;
  } else if (dealer_score > player_score)
  {
    return LOSE;
  } else 
  {
    return DRAW;
    }
}
//------------------------------------------------------------------------------
///
/// displays all the cards of players
/// 
///
//------------------------------------------------------------------------------
void displayCardsAllPlayers(struct Player** bj_table, int amount_of_players, int hide_score, int number_of_round, 
int show_result)
{
  printf("\n---- ROUND %d ----\n\n", number_of_round);
  printf("      DEALER\n");
  displayCards(bj_table[amount_of_players - 1]->deck_, bj_table[amount_of_players - 1]->cards_handed_);
  int dealer_score = score(bj_table[amount_of_players - 1]->deck_, bj_table[amount_of_players - 1]->cards_handed_);
  if(hide_score == 1) //flags
  {
    printf(" score: ?\n");
  } else
  {
    printf(" score: %d\n", dealer_score);
  }
  for(int i = 0; i < amount_of_players - 1; i++)
  {
    printf("\n     PLAYER %d\n", i + 1);
    displayCards(bj_table[i]->deck_, bj_table[i]->cards_handed_);
    int my_score = score(bj_table[i]->deck_, bj_table[i]->cards_handed_);
    printf(" score: %d\n", my_score);
    if (show_result == 1)
    {
    int result = endOfRound(bj_table[i], bj_table[amount_of_players - 1]);
    if(result == WIN)
    {
      printf(" Player has won!\n");
    } else if (result == LOSE)
    {
      printf(" Dealer has won!\n");
    } else
    {
      printf(" Hand was drawn!\n");
    }
    }
  }
}

int anyPlayerStand(struct Player** bj_table, int amount_of_players) // to check if any player has put STAND
{
  for (int i = 0; i < amount_of_players - 1; i++)
  {
    if(bj_table[i]->last_command_ == STAND)
    {
      return 1;
    }
  }
  return 0;
}
//------------------------------------------------------------------------------
///
/// Dealers turn after the player
/// 
///
//------------------------------------------------------------------------------
void dealersTurn(struct Player* dealer, struct Card** shuffled_shue, int amount_of_cards)
{
  for(int i = 0; i < dealer->cards_handed_; i++)
  {
    dealer->deck_[i]->card_is_down_ = 0; // turn the hidden card up
  }

  int dealer_score = score(dealer->deck_, dealer->cards_handed_);
  while(dealer_score < 17)
  {
    handOutCard(shuffled_shue, dealer->deck_, dealer->cards_handed_++, (amount_of_cards)--, 0);
    dealer_score = score(dealer->deck_, dealer->cards_handed_);
  }
}
//------------------------------------------------------------------------------
///
/// if not enough card
/// 
///
//------------------------------------------------------------------------------
int notEnough(int amount_of_players, int amount_of_cards)
{
  if(amount_of_cards < amount_of_players * 2)
  {
    return 0;
  } else 
  {
    return 1;
  }
}
//------------------------------------------------------------------------------
///
/// round logic and gaming
/// 
///
//------------------------------------------------------------------------------
int roundPlay(struct Player** bj_table, int number_of_round, struct Card** shuffled_shue, int amount_of_players, 
int amount_of_cards)
{
  for(int i = 0; i < amount_of_players; i++)
  {
    handOutCard(shuffled_shue, bj_table[i]->deck_, bj_table[i]->cards_handed_++, amount_of_cards--, 0);
  }
  for (int i = 0; i < amount_of_players; i++)
  {
    if(i == amount_of_players - 1)
    {
      handOutCard(shuffled_shue, bj_table[i]->deck_, bj_table[i]->cards_handed_++, amount_of_cards--, 1);
    } else
    {
      handOutCard(shuffled_shue, bj_table[i]->deck_, bj_table[i]->cards_handed_++, amount_of_cards--, 0);
    }
  }
    int print_cards = 0;
    int busted = 0;
    int bj_player = 0;
    for(int i = 0; i < amount_of_players - 1; i++)// Turn for each player
    {
      while (1) // for 1 turn 
      {
        if(print_cards == 0)
        {
          for(int i = 0; i < amount_of_players; i++)
          {
            int my_score = score(bj_table[i]->deck_, bj_table[i]->cards_handed_);
            if (my_score == 21 && i == 0)
            {
              bj_player = 1;
              bj_table[amount_of_players - 1]->deck_[1]->card_is_down_ = 0;
              break;
            }
          }
          if (bj_player != 1)
          {
            displayCardsAllPlayers(bj_table, amount_of_players, 1, number_of_round, 0);
          } else
          {
            displayCardsAllPlayers(bj_table, amount_of_players, 0, number_of_round, 1);
            return 4;
          }
        }
        printf("\nPLAYER %d: What do you want to do?\n", i + 1);
        printf(" > ");
        char* input = getInput(); // to receive the input
        int command_line = command(input); // to check and return the command
        if(command_line == QUIT)
        {
          return QUIT;
        } else if (command_line == INVALID)
        {
          printf("Error: Unknown command!");
          print_cards = 1;
        } else if (command_line == HIT)
        {
          print_cards = 0; 
          bj_table[i]->last_command_ = HIT;
          handOutCard(shuffled_shue, bj_table[i]->deck_, bj_table[i]->cards_handed_++, amount_of_cards--, 0);
          int my_score = score(bj_table[i]->deck_, bj_table[i]->cards_handed_);
          if(my_score > 21)
          {
            busted++;
            break; // if player busted
          }
        } else if (command_line == STAND)
        {
          print_cards = 0;
          bj_table[i]->last_command_ = STAND;
          break;
        } else if (command_line == DELETE)
        {
          deleteCard(bj_table[i]);
        }
      }
    }
    if (busted != amount_of_players - 1)
    {
      dealersTurn(bj_table[amount_of_players - 1], shuffled_shue, amount_of_cards);
    } else 
    {
      bj_table[amount_of_players - 1]->deck_[1]->card_is_down_ = 0; // to take dealers card
    }
  displayCardsAllPlayers(bj_table, amount_of_players, 0, number_of_round, 1);
  return 4;
}


//------------------------------------------------------------------------------
///
/// all the game blackjack
/// parsing the arguments
///
//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
  struct Argument arguments = parseArguments(argv, argc);
  if(arguments.amount_of_players_ == -1)
  {
    printf("Invalid type for argument!\n");
    return 3;
  } 
  if(arguments.amount_of_players_ == -2)
  {
    printf("Invalid number of parameters given!\n");
    return 2;
  } 
  welcomeMessage(arguments.welcome_file_);
  int amount_of_decks = arguments.amount_of_decs_;
  int amount_of_cards = CARD_PER_DECK * amount_of_decks;
  int amount_of_players = arguments.amount_of_players_ + 1;
  struct Player** bj_table = generateTable(amount_of_players, amount_of_decks);
  struct Card** shoe = generateDeck(amount_of_decks);
  if (!shoe) 
  {
    printf("Out of memory!\n");
    return 1;
    }
    
  shuffledShue(shoe, amount_of_decks * CARD_PER_DECK, arguments.seed);
  int enough_cards = notEnough(amount_of_players, amount_of_cards);
  if(enough_cards == 0)
  {
    printf("Not enough cards left to deal another round!\n");
    return 4;
  } else
  {
    int number_of_round = 1;
    while (1)
    {
      int result_round = roundPlay(bj_table, number_of_round, shoe, amount_of_players, amount_of_cards);
      if(result_round == QUIT)
      {
        break;
      }
      for (int i = 0; i < amount_of_players; i++) 
      {
        freeCardDeck(bj_table[i]->deck_, bj_table[i]->cards_handed_);
        bj_table[i]->cards_handed_ = 0;
        bj_table[i]->last_command_ = -1;
        }
      number_of_round++;
      printf("\n");
    }
  }
  freeTable(bj_table, amount_of_players);
  freeCardDeck(shoe, amount_of_cards);
  return 0;
}


