//------------------------------------------------------------------------------
// a1.c
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

#define NUM_MOVIES 6
#define STUDENT_DISCOUNT 0.30
#define MEMBERSHIP_DISCOUNT 0.15
#define MAX_CHOICE 7
#define MIN_CHOICE 0
#define NUM_POPCORN 4
#define NUM_DRINKS 5

const float MOVIE_PRICES[] = {0.0, 92.10, 77.42, 59.75, 43.30, 28.00, 3.50};
const float POPCORN_PRICES[] = {0.0, 9.99, 7.80, 5.50, 3.50};
const float DRINK_PRICES[] = {0.0, 2.95, 2.60, 2.10, 2.00, 1.10};

//------------------------------------------------------------------------------
///
/// The main program.
/// Reads two numbers from stdin and prints their sum to stdout.
/// The whole proccess is repeated the number of times given
/// in LOOP_REPETITIONS.
///
/// @param argc not used
/// @param argv not used
///
/// @return always zero
//

void welcome(void) {
    printf("=== Welcome to Cinema Booking System ===\n");
    printf("Today in our cinema:\n");
}

void movies(void) {
    printf("1. Highway to Hell ESP Edition - €92.08\n");
    printf("2. The Hitchhiker's Guide to the Galaxy - €77.42\n");
    printf("3. Flow - €59.74\n");
    printf("4. The Place - €43.28\n");
    printf("5. Interstate 60 - €28.00\n");
    printf("6. Truth seeking at TU Graz - €3.46\n");
} 

void popcorn_flavors(void) {

    printf("1. Spicy Popcorn - €9.99\n");
    printf("2. Cheese Popcorn - €7.80\n");
    printf("3. Caramel Popcorn - €5.50\n");
    printf("4. Salted Popcorn - €3.50\n");

}

void av_drinks(void){
    printf("Our available drinks:");
    printf("1. Orange Juice - €2.95");
    printf("2. Ice Tea - €2.60");
    printf("3. Cola - €2.10");
    printf("4. Sprite - €2.00");
    printf("5. Water - €1.10");
}

int user_choice(void) {
    int choice;

    printf("\nWhat do you want?:\n");
    printf("- leave (enter 0)\n");
    printf("- to proceed choosing a movie on your own\n");
    printf("- to be recommended some movie-treats-combos accroding to your budget (enter 7)\n");
    printf(">");
    scanf("%d", &choice);
    getchar();

    return choice;
}

int popcorn_option(void) {
    int option;
    printf("Select an option (1-4) > \n");
    scanf("%d", &option);
    getchar();
    return option; 
}

int drink_option(void) {
    int dr_option;
    printf("Select an option (1-5) > \n");
    scanf("%d", &dr_option);
    getchar();
    return dr_option; 
}
char question_student(void) {
    char student;
    printf("Are you a student? (enter y for \"Yes\", n for \"No\") > ");
    scanf(" %c", &student);
    return student;
}

char question_membership(void) {
    char membership;
    printf("Do you have a membership card? (enter y for \"Yes\", n for \"No\") > ");
    scanf(" %c", &membership);
    getchar();
    return membership;
}

char question_popcorn(void) {
    char popcorn;
    printf("Would you like a popcorn? (enter y for \"Yes\", n for \"No\") > ");
    scanf(" %c", &popcorn);
    getchar();
    return popcorn;
}

char question_drink(void) {
    char drink;
    printf("Would you like a drink? (enter y for \"Yes\", n for \"No\") > ");
    scanf(" %c", &drink);
    getchar();
    return drink;
} 

//------------------------------------------------------------------------------
///
/// The main program.
/// Reads two numbers from stdin and prints their sum to stdout.
/// The whole proccess is repeated the number of times given
/// in LOOP_REPETITIONS.
///
/// @param argc not used
/// @param argv not used
///
/// @return always zero
//

int main(void){

    int choice;
    int option; // Do i need a function? Or do i have to to do it without
    int drink_opt = 0; //using as a default

    welcome();
    movies();

    printf("\nWhat do you want?:\n");
    printf("\n- leave (enter 0)\n"); 
    printf("- to proceed choosing a movie on your own\n");
    printf("- to be recommended some movie-treats-combos accroding to your budget (enter 7)\n");
    printf(">");
    scanf("%d", &choice);
    getchar();

    if(choice == 0) {
        printf("Thanks for your visit! Looking forward to seeing you again!\n");
        return 0;
    }
    
    char arrmovies[7][45] = {
        "", // 0 is empty cause i need from 1
        "Highway to Hell ESP Edition",
        "The Hitchhiker's Guide to the Galaxy",
        "Flow",
        "The Place",
        "Interstate 60",
        "Truth seeking at TU Graz"
    };

    if (choice >= 1 && choice <= NUM_MOVIES) {
        printf("Chosen Film: %s\n", arrmovies[choice]);  // Pointer delete
    } else if (choice == 7) {
        printf("Welcome to the recommender system!\n");
    } else {
        printf("Invalid input. Please enter number in [0, 7]!\n");
  }

  char student = question_student();
  char membership = question_membership();
  char popcorn = question_popcorn();
  char drink = question_drink();

    char arrpopcorn[5][40] = {
        "",
        "Spicy Popcorn - €9.99",
        "Cheese Popcorn - €7.80",
        "Caramel Popcorn - €5.50",
        "Salted Popcorn - €3.50"
    };

    char arrdrinks[6][35] = {
        "",
        "Orange Juice - €2.95",
        "Ice Tea - €2.60",
        "Cola - €2.10",
        "Sprite - €2.00",
        "Water - €1.10"
    };

    if(popcorn == 'y') {
        popcorn_flavors();
        option = popcorn_option();
        if(option >= 1 && option <= 4) {
            printf("Chosen flavour: %s\n", arrpopcorn[option]);
        } else {
            printf("Invalid option\n");
        }
    }
    if (drink == 'y') {
        av_drinks();
        printf("Select an option (1-5) > \n");
        drink_opt = drink_option();
        printf("Chosen drink: %s\n", arrdrinks[drink_opt]);

    } 

    printf("\n=== Your Receipt ===\n");
    printf("Movie: %s\n", arrmovies[choice]);

    int popcorn_choice = option;
    int drink_choice = drink_opt;
    
    float ticket_price = MOVIE_PRICES[choice];
    float discount = 0.0; // as a default code for users without membership or stud.card
    float popcorn_price = 0.0;
    float drink_price = 0.0;

    if (student == 'y') {
        discount = STUDENT_DISCOUNT;
    } else if (membership == 'y') {
        discount = MEMBERSHIP_DISCOUNT;
    }
    if (popcorn_choice > 0) {
        popcorn_price = POPCORN_PRICES[popcorn_choice];
    } else {
        popcorn_price = 0.0;
    }
    if (drink_choice > 0) {
        drink_price = DRINK_PRICES[drink_choice];
    }

    float final_price = (ticket_price - ticket_price * discount) + popcorn_price + drink_price;

    printf("Ticket price: €%.2f\n", ticket_price);
    if (discount > 0.0) {
        printf("Applicable discount: €-%.2f\n", ticket_price * discount);
    }
    if (popcorn_choice > 0) {
        printf("Popcorn (%s): €%.2f\n", arrpopcorn[popcorn_choice], popcorn_price);
    }
    if (drink_choice > 0) {
        printf("Drink (%s): €%.2f\n", arrdrinks[drink_choice], drink_price);
    }
    printf("Total: €%.2f\n", final_price);
    printf("\nCongratulations on your purchase! Enjoy the movie!\n");

return 0;

}
