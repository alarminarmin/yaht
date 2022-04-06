
MAIN.C
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "yahtzee.h"


int main(int argc, char** argv)
{
	srand(time(0));

	while (1)
	{
		switch (poll_menu()) 
		{
		case MENU_PRINT_RULES:
		{
			print_rules();
		}
		break;
		case MENU_START_GAME:
		{
			play_game();
		}
		break;
		case MENU_EXIT:
		{
			exit_game();
		}
		break;
		}
	}
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
YAHTZEE.C
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "yahtzee.h"


// returns menu option selected
int poll_menu()
{
	clrscr();
	printf("Yahtzee:\n");
	printf("1. Print Yahtzee's rules\n");
	printf("2. Start game!\n");
	printf("3. Exit\n");

	int option = 0;
	do {
		scanf("%d", &option);
		printf("%d", option);
	} while (option < 1 && option > 3);

	return option;
}

void clrscr()
{
	system("cls");
}

void play_round(int score_card[13])
{
	int rolls[5];

	int saved_rolls[5] = { -1, -1, -1, -1, -1 };
	for (int roll_count = 3; roll_count > 0; roll_count--)
	{
		printf("You have %d rolls left\n", roll_count);
		printf("Rolling! ");
		enter_key_stall();
		roll_5_dice(rolls);
		printf("Rolls: ");
		for (int i = 0; i < 5; i++)
		{
			if (saved_rolls[i] != -1)
			{
				rolls[i] = saved_rolls[i];
			}
			printf("%d", rolls[i]);
			if (i + 1 < 5)
				printf(", ");
		}
		printf("\n");

		if (roll_count - 1 > 0)
		{
			printf("Do you want to reroll?\nEnter Y/N: ");
			char ch;
			do {
				scanf(" %c", &ch);
			} while (ch != 'Y' && ch != 'N');

			if (ch == 'N')
			{
				int option = choose_combination(score_card);
				score_card[option] = get_combination_score(rolls, option);
				roll_count = 0;
			}
			else if (ch == 'Y')
			{
				for (int i = 0; i < 5; i++)
				{
					printf("Save roll? %d\nY/N: ", rolls[i]);
					char ch;
					do {
						scanf(" %c", &ch);
					} while (ch != 'Y' && ch != 'N');

					if (ch == 'Y')
						saved_rolls[i] = rolls[i];
					else if (ch == 'N')
						saved_rolls[i] = -1;
				}
				printf("\n");
			}
		}
		else
		{
			printf("\nYou are out of rolls.\n");
			enter_key_stall();
			int option = choose_combination(score_card);
			score_card[option] = get_combination_score(rolls, option);
		}
	}

	printf("\nScore Card:\n");
	display_score_card(score_card);
	printf("\n");
	enter_key_stall();
}

void enter_key_stall()
{
	printf("\n");
	system("pause");
	//printf("\nEnter any key: ");
	//while (getchar() == '\n');
}

void print_rules()
{
	clrscr();
	printf(
		"The scorecard used for Yahtzee is composed of two sections.\n \
        A upper section and a lower section.\n \
        A total of thirteen boxes or thirteen scoring combinations are divided amongst the sections.\n \
        The upper section consists of boxes that are scored by summing the value of the dice matching the faces of the box.\n \
        If a player rolls four 3's, then the score placed in the 3's box is the sum of the dice which is 12.\n \
        Once a player has chosen to score a box, it may not be changed and the combination is no longer in play for future rounds.\n \
        If the sum of the scores in the upper section is greater than or equal to 63, then 35 more points are added to the players overall score as a bonus.\n \
        The lower section contains a number of poker like combinations.\n");

	enter_key_stall();
}

int get_combination_score(int rolls[5], int combo_type)
{
	int sum_of_face = 0;
	for (int i = 0; i < 5; i++)
	{
		sum_of_face += rolls[i];
	}

	int alike[6] = { 0,0,0,0,0,0 };
	for (int i = 0; i < 5; i++)
	{
		alike[rolls[i] - 1]++;
	}

	int score = 0;
	switch (combo_type)
	{
	case SUM_OF_1S:
		for (int i = 0; i < 5; i++)
		{
			if (rolls[i] == 1)
				score += 1;
		}
		break;

	case SUM_OF_2S:
		for (int i = 0; i < 5; i++)
		{
			if (rolls[i] == 2)
				score += 2;
		}
		break;

	case SUM_OF_3S:
		for (int i = 0; i < 5; i++)
		{
			if (rolls[i] == 3)
				score += 3;
		}
		break;

	case SUM_OF_4S:
		for (int i = 0; i < 5; i++)
		{
			if (rolls[i] == 4)
				score += 4;
		}
		break;

	case SUM_OF_5S:
		for (int i = 0; i < 5; i++)
		{
			if (rolls[i] == 5)
				score += 5;
		}
		break;

	case SUM_OF_6S:
		for (int i = 0; i < 5; i++)
		{
			if (rolls[i] == 6)
				score += 6;
		}
		break;

	case THREE_OF_A_KIND:
		for (int i = 0; i < 6; i++)
		{
			if (alike[i] >= 3)
				score = sum_of_face;
		}
		break;

	case FOUR_OF_A_KIND:
		for (int i = 0; i < 6; i++)
		{
			if (alike[i] >= 4)
				score = sum_of_face;
		}
		break;

	case FULL_HOUSE:
	{
		int pair = 0, three = 0;
		for (int i = 0; i < 6; i++)
		{
			if (alike[i] == 3)
				three = 1;

			if (alike[i] == 2)
				pair = 1;
		}

		if (pair && three)
		{
			score = 25;
		}
	} break;
	case SMALL_STRAIGHT:
	{
		int count = 0;
		for (int i = 0; i < 6 && count != 4; i++)
		{
			if (alike[i] == 0)
				count = 0;
			else
				count++;
		}
		if (count == 4)
		{
			score = 30;
		}
	} break;
	case LARGE_STRAIGHT:
	{
		int count = 0;
		for (int i = 0; i < 6 && count != 5; i++)
		{
			if (alike[i] == 0)
				count = 0;
			else
				count++;
		}
		if (count == 5)
		{
			score = 40;
		}
	} break;
	case YAHTZEE:
		for (int i = 0; i < 6; i++)
		{
			if (alike[i] == 5)
				score = 50;
		}
		break;
	case CHANCE:
		score = sum_of_face;
		break;
	}
	return score;
}

void display_score_card(int score_card[13])
{
	printf("1. Sum of 1's");
	if (score_card[SUM_OF_1S] != -1)
		printf(" (%d)", score_card[SUM_OF_1S]);
	printf("\t7. Three-of-a-kind");
	if (score_card[THREE_OF_A_KIND] != -1)
		printf(" (%d)", score_card[THREE_OF_A_KIND]);
	printf("\n2. Sum of 2's");
	if (score_card[SUM_OF_2S] != -1)
		printf(" (%d)", score_card[SUM_OF_2S]);
	printf("\t8. Four-of-a-kind");
	if (score_card[FOUR_OF_A_KIND] != -1)
		printf(" (%d)", score_card[FOUR_OF_A_KIND]);
	printf("\n3. Sum of 3's");
	if (score_card[SUM_OF_3S] != -1)
		printf(" (%d)", score_card[SUM_OF_3S]);
	printf("\t9. Full house");
	if (score_card[FULL_HOUSE] != -1)
		printf(" (%d)", score_card[FULL_HOUSE]);
	printf("\n4. Sum of 4's");
	if (score_card[SUM_OF_4S] != -1)
		printf(" (%d)", score_card[SUM_OF_4S]);
	printf("\t10. Small straight");
	if (score_card[SMALL_STRAIGHT] != -1)
		printf(" (%d)", score_card[SMALL_STRAIGHT]);
	printf("\n5. Sum of 5's");
	if (score_card[SUM_OF_5S] != -1)
		printf(" (%d)", score_card[SUM_OF_5S]);
	printf("\t11. Large straight");
	if (score_card[LARGE_STRAIGHT] != -1)
		printf(" (%d)", score_card[LARGE_STRAIGHT]);
	printf("\n6. Sum of 6's");
	if (score_card[SUM_OF_6S] != -1)
		printf(" (%d)", score_card[SUM_OF_6S]);
	printf("\t12. Yahtzee");
	if (score_card[YAHTZEE] != -1)
		printf(" (%d)", score_card[YAHTZEE]);
	printf("\n13. Chance");
	if (score_card[CHANCE] != -1)
		printf(" (%d)", score_card[CHANCE]);

}

int choose_combination(int score_card[13])
{
	display_score_card(score_card);
	printf("\nWhich combination to use? ");
	int option = 0;
	do {
		scanf(" %d", &option);
	} while (option < 1 || option > 13);
	option--;
	return option;
}

void play_game()
{
	// scores for sums of rolls of 1, 2, 3, 4, 5, 6
	// three-of-a-kind, four-of-a-kind, full-house, small-straight, large-straight, yahtzee, chance
	int p1_score_card[13] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };
	int p2_score_card[13] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

	int turns = 13 * 2;
	int round = 1;
	int players_turn = 1;

	for (int i = 0; i < turns; i++)
	{
		clrscr();

		if (players_turn == 1)
		{
			printf("Round number: %d\n", round);
			printf("It is now P1's turn.\n");
			enter_key_stall();
			printf("\n");
			play_round(p1_score_card);
			players_turn = 2;
		}
		else if (players_turn == 2)
		{
			printf("Round number: %d\n", round);
			printf("It is now P2's turn.\n");
			enter_key_stall();
			printf("\n");
			play_round(p2_score_card);
			players_turn = 1;

			round++;
		}
	}

	int p1_score, p2_score;
	calculate_scores(&p1_score, &p2_score, p1_score_card, p2_score_card);

	printf("P1 Total Score: %d, P2 Total Score: %d\n", p1_score, p2_score);
	if (p1_score > p2_score)
	{
		printf("Player 1 wins!\n");
	}
	else if (p1_score < p2_score)
	{
		printf("Player 2 wins!\n");
	}
	else
	{
		printf("Tie!\n");
	}

	enter_key_stall();
}

void calculate_scores(int* p1_score, int* p2_score, int p1_score_card[], int p2_score_card[])
{
	for (int i = SUM_OF_1S; i < SUM_OF_6S; i++)
	{
		*p1_score += p1_score_card[i];
		*p2_score += p2_score_card[i];
	}

	// Add bonuses
	if (*p1_score >= 63)
		*p1_score += 35;
	if (*p2_score >= 63)
		*p2_score += 35;

	for (int i = THREE_OF_A_KIND; i < CHANCE; i++)
	{
		*p1_score += p1_score_card[i];
		*p2_score += p2_score_card[i];
	}
}

void roll_5_dice(int rolls[5])
{
	for (int i = 0; i < 5; i++)
	{
		rolls[i] = (rand() % 6) + 1;

	}
	printf("\n");

}

void exit_game()
{
	clrscr();
	printf("Thanks for playing! Goodbye!\n");
	getchar();

	exit(0);
}

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
YAHTZEE.H
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef YAHTZEE_H
#define YAHTZEE_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <time.h>

// Menu options
#define MENU_PRINT_RULES 1

#define MENU_START_GAME 2

#define MENU_EXIT 3

// indices into scorecard array
#define SUM_OF_1S 0

#define SUM_OF_2S 1

#define SUM_OF_3S 2

#define SUM_OF_4S 3

#define SUM_OF_5S 4

#define SUM_OF_6S 5

#define THREE_OF_A_KIND 6 

#define FOUR_OF_A_KIND 7

#define FULL_HOUSE 8

#define SMALL_STRAIGHT 9

#define LARGE_STRAIGHT 10

#define YAHTZEE 11

#define CHANCE 12
//close and stall
void clrscr();

void enter_key_stall();

// returns menu option selected
int poll_menu();

//scoring the game
int get_combination_score(int rolls[5], int combo_type);

void display_score_card(int score_card[13]);

int choose_combination(int score_card[13]);

void calculate_scores(int* p1_score, int* p2_score, int p1_score_card[], int p2_score_card[]);

//rules and playing
void print_rules();

void play_round(int score_card[13]);

void play_game();

void exit_game();

void roll_5_dice(int rolls[5]);

#endif // FUNCTION_H


