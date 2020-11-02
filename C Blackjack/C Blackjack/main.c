#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>
#include <mmsystem.h>

#define TRUE 1
#define FALSE 0
#define MAX_HAND 25
#define MAX_CARD 5
#define MAX_INPUT 255
#define MINIMUM_BET 10

int is_same(char str1[], char str2[]) {
	// Check if two string are the same
	int output;
	output = strcmp(str1, str2);
	if (output == 0)
		return TRUE;
	else
		return FALSE;
}

int rand_int(int a, int b) {
	// Generate random integer between a <= num < b
	int random_num;
	random_num = a + rand() % b;
	return random_num;
}

void deal_card(char hand[MAX_HAND][MAX_CARD]) {
	// Generate a random card and deal it to a hand
	char card_name_arr[][3] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
	char card_suit_arr[][3] = { "C","H","D","S" };
	char gen_card[MAX_CARD];

	// Generated card = Random card name + Random card suit
	strcpy(gen_card, card_name_arr[rand_int(0, 13)]);
	strcat(gen_card, card_suit_arr[rand_int(0, 4)]);

	// Append card to hand by checking the array if it has a "0"
	int index;
	for (index = 0; index < MAX_HAND; index++) {
		if (is_same(hand[index], "0")) {
			strcpy(hand[index], gen_card);
			break;
		}
	}

	return;
}

int get_card_value(char card[]) {
	// For Ace, Jack, Queen, King
	if (isalpha(card[0])){ 
		switch (card[0])
		{
		case 'A':
			return 1;
			break;

		default:
			return 10;
		}
	}

	// For number 2-10 cards
	if (isdigit(card[0])) {
		return atoi(card);
	}

	return -10;
}

// TODO Flowchart
int get_hand_value(char hand[][MAX_CARD]) {
	int index;
	int output_value = 0;

	// Do an initial "soft value", where Ace is valued at 1
	for (index = 0; index < MAX_HAND;) {
		if (is_same(hand[index], "0")) { // This signify end of the card in the hand
			break;
		}
		else {
			output_value = output_value + get_card_value(hand[index]);
			index++;
		}
	}

	// For every ace you come across you add 10 as long as the 
	// soft value is <= 11
	for (index = 0; index < MAX_HAND; index++) {
		if (is_same(hand[index], "0")) {
			break;
		}
		else if ((hand[index][0] == 'A') && (output_value <= 11)) {
			output_value += 10;
		}
	}

	return output_value;
}

void display_hand(char hand[][MAX_CARD], int first_card_face_down) {
	// For displaying the hand and hand value, option to hide first card
	int index;
	for (index = 0; index < MAX_HAND; index++) {
		if ((index == 0) && first_card_face_down){
		printf("XX ");
		continue;
		}
		if (is_same(hand[index], "0")) {
			// This print the (hand value) when reach end of card in hand
			if (first_card_face_down) {
				printf("(%d) %\n", get_card_value(hand[1]));
			}
			else {
				printf("(%d) %\n", get_hand_value(hand));
			}
			break;
		}
		else {
			// This print the card 
			printf("%s ", hand[index]);
		}
	}

	return;
}

int input_is_all_digit(char player_input[MAX_INPUT]) {
	// Check if all input is digit
	int index = 0;
	for (index = 0; player_input[index] != '\0'; index++) {
		if (isdigit(player_input[index]) == FALSE) {
			return 0;
		}
	}
	return 1;
}

int get_bet_from_player() {
	// Get a valid bet from the player
	char player_input[MAX_INPUT];
	int output;
	int valid_input = FALSE;
	do {
		printf("Please enter your bet (Smallest betting unit is %d$): ", MINIMUM_BET);
		gets(player_input);
		// Check if the user has not enter anything
		if (is_same(player_input, "")) {
			printf("Invalid bet input, no input detected\n");
			valid_input = FALSE;
			continue;
		}
		// Check if all character is a digit
		if (input_is_all_digit(player_input) == FALSE) {
			printf("Invalid bet input, must be a positive integer\n");
			valid_input = FALSE;
			continue;
		}

		// Check if bet is less than minimum bet/smaller than smallest chip
		if ((atoi(player_input) < MINIMUM_BET)) {
			printf("Invalid bet, bet is smaller than minimum bet of %d$\n", MINIMUM_BET);
			valid_input = FALSE;
			continue;
		}

		// Check if bet multiple of minimum bet (smallest chip)
		if ((atoi(player_input) % MINIMUM_BET) != 0) {
			printf("Invalid bet input, bet is not a multiple of betting unit %d$\n", MINIMUM_BET);
			valid_input = FALSE;
			continue;
		}

		valid_input = TRUE;
	} while (valid_input == FALSE);

	output = atoi(player_input);
	return output;
}

int is_blackjack(char hand[MAX_HAND][MAX_CARD]){
	if (get_hand_value(hand) == 21) {
		return TRUE;
	}
	else
		return FALSE;
}

int is_busted(char hand[MAX_HAND][MAX_CARD]) {
	if (get_hand_value(hand) > 21)
		return TRUE;
	else
		return FALSE;
}

char get_playing_decision_from_player() {
	// Get a valid playing decision a "character" from a player
	char player_input[MAX_INPUT];
	int valid_input = FALSE;

	do {	
	printf("Please enter playing decision (H - Hit) (D - Double down) (S - Stand): ");
	gets(player_input);
	// Check if player has not put anything
	if (is_same(player_input, "")) {
		printf("Invalid playing decision, no playing decision given\n");
		valid_input = FALSE;
		continue;
	}
	// Check is player enter more than one playing decision
	if (strlen(player_input) > 1) {
		printf("Invalid playing decision, please enter only one playing decision\n");
		valid_input = FALSE;
		continue;
	}
	// Check if the playing decision is possible
	switch (toupper(player_input[0])) {
	case 'H':
		valid_input = TRUE;
		break;
	case 'D':
		valid_input = TRUE;
		break;
	case 'S':
		valid_input = TRUE;
		break;
	default:
		printf("Invalid playing decision, %s is not a playing decision\n", player_input);
		valid_input = FALSE;
		continue;
	}
	} while (valid_input == FALSE);


	return toupper(player_input[0]);
}

char ask_player_keep_playing() {
	// Get a valid playing decision a "character" from a player
	char player_input[MAX_INPUT];
	int valid_input = FALSE;

	do {
		printf("Would you like to keep playing (Y - Yes) (N - No): ");
		gets(player_input);
		// Check if player has not put anything
		if (is_same(player_input, "")) {
			printf("Invalid decision, no playing decision given\n");
			valid_input = FALSE;
			continue;
		}
		// Check if player enter more than one decision
		if (strlen(player_input) > 1) {
			printf("Invalid decision, please enter only one character\n");
			valid_input = FALSE;
			continue;
		}
		// Check if the answer is 'Y' or 'N'
		switch (toupper(player_input[0])) {
		case 'Y':
			valid_input = TRUE;
			break;
		case 'N':
			valid_input = TRUE;
			break;
		default:
			printf("Invalid decision, %s is not a valid decision\n", player_input);
			valid_input = FALSE;
			continue;
		}
	} while (valid_input == FALSE);


	return toupper(player_input[0]);
}

void clear_hand(char hand[][MAX_CARD]) {
	int i;
	for (i = 0; i < MAX_HAND; i++)
		strcpy(hand[i], "0");
	return;
}

int allowed_to_double_down(int player_money, int player_bet) {
	if (player_money < player_bet) {
		return FALSE;
	}
	return TRUE;
}

int main(){
	// Player variables
	int player_money = 1000,
		player_bet = 0;
	char player_hand[MAX_HAND][MAX_CARD];
	char player_hand_state = 'P';
	char player_decision;

	// Dealer variables
	float dealer_rates[3] = { 2.5, 2, 1 };
	char dealer_hand[MAX_HAND][MAX_CARD];
	char dealer_hand_state;

	// Greetings
	printf("Welcome player!\n");
	printf("Take a seat and prepare to los...I mean win some money!\n");
	printf("We will begin playing blackjack!\n");
	printf("A few things to note: \n");
	printf("   - A blackjack/naturals trumps 21\n");
	printf("   - Dealer must keep hitting until a 17 or higher, even if say dealer has 16 and you have 15\n");
	printf("\n");
	
	// The main part of the game
	char game_on = 'Y';
	char round_on = 'Y';
	srand(time(NULL));
	while (game_on == 'Y') {

		// Check for minimum bet
		if (player_money < MINIMUM_BET) {
			printf("You are broke, you only have %d$. Get out!\n", player_money);
			break;
		}
		
		// Main round
		while (round_on == 'Y') {
			
			// Pre-round set up
			clear_hand(dealer_hand);
			clear_hand(player_hand);
			player_bet = 0;
			player_hand_state = 'P'; // P means playing
			printf("You have %d$ to play\n", player_money);

			// Betting part
			while (TRUE) {
				player_bet = get_bet_from_player(); // Get int
				if (player_bet > player_money)
				{
					printf("Insufficient money for bet\n");
					continue;
				}
				player_money -= player_bet;
				break;
			}

			// Dealing cards
			deal_card(player_hand);
			deal_card(player_hand);
			deal_card(dealer_hand);
			deal_card(dealer_hand);

			printf("Dealer hand is: \n");
			display_hand(dealer_hand, TRUE);
			printf("Your hand is: \n");
			display_hand(player_hand, FALSE);

			// Checking for blackjacks
			if (is_blackjack(player_hand)) {
				printf("You got a blackjack!\n");
				printf("Dealer hidden card revealed\n");
				display_hand(dealer_hand, FALSE);
				if (is_blackjack(dealer_hand)) {
					printf("Dealer also have blacjack\n");
					printf("Bets are pushed\n");
					player_money += player_bet * dealer_rates[2];
					break;
				}
				else {
					printf("Dealer don't have blackjack\n");
					printf("You win at blackjack rate\n");
					player_money += player_bet * dealer_rates[0];
					break;
				}
			}

			if (is_blackjack(dealer_hand)) {
				printf("Dealer hidden revealed which is: \n");
				display_hand(dealer_hand, FALSE);
				printf("Ohh..unlucky the dealer has blackjack!\n");
				printf("You lose your bet\n");
				break;
			}

			// Player main playing decisions
			while (player_hand_state == 'P') {
				player_decision = get_playing_decision_from_player();

				// Player stand
				if (player_decision == 'S') {
					printf("You choose to stand\n");
					player_hand_state = 'O';
				}

				// Player hit a card
				if (player_decision == 'H') {
					printf("You choose to hit a card\n");
					deal_card(player_hand);
				}

				// Player double down
				if (player_decision == 'D') {
					if (allowed_to_double_down(player_money, player_bet)) {
						printf("You choose to double down\n");
						deal_card(player_hand);
						player_money -= player_bet;
						player_bet *= 2;
						player_hand_state = 'O';
					}
					else {
						printf("Sorry you are not allowed to double down\n");
						printf("Original bet is larger than money you have to play\n");
					}
				}

				// Show the card to player
				printf("\nDealer hand is: \n");
				display_hand(dealer_hand, TRUE);
				printf("Your hand is: \n");
				display_hand(player_hand, FALSE);
			
				// Check for busted player hand
				if (is_busted(player_hand)) {
					player_hand_state = 'B';
				}
			} // End of playing decision of player

			// Check if player is busted
			if (is_busted(player_hand)) {
				printf("Sorry your hand is busted\n");
				printf("You lose your bet\n");
				break;
			}
			
			// Dealer phase
			printf("\nDealer revealed hand is: \n");
			display_hand(dealer_hand, FALSE);
			printf("Your hand is: \n");
			display_hand(player_hand, FALSE);
			
			// Dealer forced to deal to 17 or above
			while (get_hand_value(dealer_hand) < 17) {
				printf("\nDealer deals a card\n");
				deal_card(dealer_hand);

				printf("Dealer hand is: \n");
				display_hand(dealer_hand, FALSE);
				printf("Your hand is: \n");
				display_hand(player_hand, FALSE);
			}

			// If dealer busted the round is over, player wins automatically
			if (is_busted(dealer_hand)) {
				printf("Dealer busted! You win!\n");
				player_money += player_bet * dealer_rates[1];
				break;
			}

			// Comparing player vs dealer who is closer to 21
			// Bets are pushed/Draw
			if (get_hand_value(dealer_hand) == get_hand_value(player_hand)) {
				printf("You and dealer hand have the same value\n");
				printf("Bet is pushed\n");
				player_money += player_bet * dealer_rates[2];
				break;
			} 

			// Player wins (Player hand is closer to 21 than dealer)
			if (get_hand_value(player_hand) > get_hand_value(dealer_hand)) {
				printf("Your hand is closer to 21 than dealer's hand\n");
				printf("You win!\n");
				player_money += player_bet * dealer_rates[1];
				break;
			}
			else { // Player loses (Dealer hand is close to 21 than player)
				printf("Dealer hand is closer to 21 than your hand\n");
				printf("You lose!\n");
				break;
			}
			
		} // END OF ROUND
		// Ask if the player want to keep playing
		game_on = ask_player_keep_playing();
	}

	return 0;
}
