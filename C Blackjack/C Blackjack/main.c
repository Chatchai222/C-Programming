#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TRUE 1
#define FALSE 0
#define MAX_HAND 25
#define MAX_CARD 5
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
	srand(time(NULL));
	random_num = a + rand() % b;
	return random_num;
}

int deal_card(char hand[][MAX_CARD]) {
	// Generate a random card and deal it to a hand
	char card_name_arr[][3] = { "A","2","3","4","5","6","7","8","9","10","J","Q","K" };
	char card_suit_arr[][3] = { "C","H","D","S" };
	char gen_card[5];


	// Generated card = Random card name + Random card suit
	strcpy(gen_card, card_name_arr[rand_int(0, 13)]);
	strcat(gen_card, card_suit_arr[rand_int(0, 4)]);
	printf("Generated card is %s\n", gen_card);


	// Append card to hand by checking the array if it has a "0"
	int index;
	for (index = 0; index < MAX_HAND; index++) {
		if (is_same(hand[index], "0")) {
			strcpy(hand[index], gen_card);
			printf("Appended %s\n", gen_card);
			break;
		}
	}

	return 0;
}

int get_card_value(char card[]) {
	// For Ace,Jack,Queen,King
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

	return 666;
}

int get_hand_value(char hand[][MAX_CARD]) {
	int index;
	int output_value = 0;

	// Do an initial "soft value", where Ace is valued at 1
	for (index = 0; index < MAX_HAND;) {
		if (is_same(hand[index], "0")) {
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
		else if ((hand[index] == 'A') && (output_value <= 11)) {
			output_value = output_value + 10;
		}
	}

	return output_value;
}

int display_hand(char hand[][MAX_CARD], int first_card_face_down) {
	// For displaying the hand, option to hide first card
	int index;
	for (index = 0; index < MAX_HAND; index++) {
		if ((index == 0) && first_card_face_down){
		printf("XX ");
		continue;
		}
		if (is_same(hand[index], "0")) {
			printf("\n");
			break;
		}
		else {
			printf("%s ", hand[index]);
		}
	}

	return 0;
}

int input_is_all_digit(char player_input[]) {
	// Check if all input is digit
	int index = 0;
	for (index = 0; player_input[index] != '\0'; index++) {
		if (isdigit(player_input[index]) == FALSE) {
			return 0;
		}
	}
	return 1;
}

// TODO - How to check if player has not enter anything
int get_bet_from_player() {
	// Get a valid bet from the player
	char player_input[100];
	int output;
	int valid_input = FALSE;
	do {
		printf("Please enter your bet (Smallest betting chip is %d$): ", MINIMUM_BET);
		scanf("%s", player_input);
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
			printf("Invalid bet input, smallest chip is %d$\n", MINIMUM_BET);
			valid_input = FALSE;
			continue;
		}

		valid_input = TRUE;
	} while (valid_input == FALSE);

	output = atoi(player_input);
	return output;
}

char get_playing_decision_from_player() {
	// Get a valid playing decision a "character" from a player
	char player_input[100];
	int valid_input = FALSE;

	do {	
	printf("Please enter playing decision (H - Hit) (D - Double down) (S - Stand): ");
	scanf("%s", player_input);
	// Check if player has not put anything
	if (strlen(player_input) < 1) {
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
	case 'D':
		valid_input = TRUE;
	case 'S':
		valid_input = TRUE;
	default:
		printf("Invalid playing decision, %s is not a playing decision\n", player_input);
		valid_input = FALSE;
		continue;
	}
	} while (valid_input == FALSE);


	return player_input[0];
}

	

int main(){
	char hand[][MAX_CARD] = {"AH","JS", "10D", "4C", "0", "0"};
	int index, j;
	char player_input[255];
	int player_bet;
	char playing_decision;

	player_bet = get_bet_from_player();
	printf("Player decide to %d", player_bet);
	return 0;
}
