
// Soltane Loudhabachi && Hector Becerra

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_RAND 107

typedef struct card_s {
	char color[10];
	int value;
	char action[15];
	struct card_s* nextPtr;
	struct card_s* prevPtr;
} card;

typedef struct hand_s {
	// card cards[7];
	struct card_s* headPtr;
	struct card_s* tailPtr;
	int cardCount;
}hand;

// Linked List Functions Section

//Constructor
void createNode(card* thisNode, card* data, card* nextNode, card* prevNode)
{
	strcpy(thisNode->color, data->color);
	thisNode->value = data->value;
	strcpy(thisNode->action, data->action);

	thisNode->nextPtr = nextNode;
	thisNode->prevPtr = prevNode;
}

//return attributes of the card
/*card* getData(card* thisNode)
{
	card* temp = (card*)malloc(sizeof(card));
	strcpy(temp->color, thisNode->color);
	temp->value = thisNode->value;
	strcpy(temp->action, thisNode->action);
	return temp;
}*/

void printNode(card* thisNode)
{
	if (thisNode->value == 2)
		printf("%d %s", thisNode->value, thisNode->action);
	else if (thisNode->value == 11)
		printf("# %s", thisNode->color);
	else
		printf("%d %s", thisNode->value, thisNode->color);
}

card* getNext(card* thisNode)
{
	return thisNode->nextPtr;
}

card* getPrev(card* thisNode)
{
	return thisNode->prevPtr;
}

///////////////////////////////////////

void createHand(hand* thisHand)
{
	thisHand->headPtr = NULL;
	thisHand->tailPtr = NULL;
	thisHand->cardCount = 0;
}

bool isEmpty(hand* thisHand)
{
	return (thisHand->cardCount == 0);
}

int getLength(hand* thisHand)
{
	return thisHand->cardCount;
}

card* recursiveGetNodeAt(hand* thisHand, int position, card* curPtr)
{
	if (position == 1)
		return curPtr;
	else
		return recursiveGetNodeAt(thisHand, position - 1, getNext(curPtr));
}

card* getNodeAt(hand* thisHand, int position)
{
	card* curPtr = NULL;
	curPtr = thisHand->headPtr;
	curPtr = recursiveGetNodeAt(thisHand, position, curPtr);
	return curPtr;
}

void printHand(hand* thisHand)
{
	card* curPtr = thisHand->headPtr;
	while (curPtr != NULL)
	{
		printNode(curPtr);

		if (getNext(curPtr) == NULL)
			printf("\n");
		else
			printf(", ");

		curPtr = getNext(curPtr);
	}
}

void addCard(hand* thisHand, card* newNode)
{
	if (isEmpty(thisHand))
	{
		thisHand->headPtr = newNode;
		thisHand->tailPtr = newNode;
	}
	else
	{
		(thisHand->tailPtr)->nextPtr = newNode;
		newNode->prevPtr = thisHand->tailPtr;
		thisHand->tailPtr = newNode;
		newNode->nextPtr = NULL;
	}

	thisHand->cardCount += 1;
}

void removeCard(hand* thisHand, int position)
{
	card* curPtr = NULL;

	if (position == 1)
	{
		// Removing the first node
		curPtr = thisHand->headPtr;
		thisHand->headPtr = getNext(thisHand->headPtr);
		if (thisHand->headPtr != NULL)
		{
			thisHand->headPtr->prevPtr = NULL;
			curPtr->nextPtr = NULL;
		}
	}
	else if (position == thisHand->cardCount)
	{
		// Removing the last node
		curPtr = thisHand->tailPtr;
		thisHand->tailPtr = getPrev(thisHand->tailPtr);
		thisHand->tailPtr->nextPtr = NULL;
		curPtr->prevPtr = NULL;
	}
	else
	{
		// Removing a node in the middle
		card* prevPtr = getNodeAt(thisHand, position - 1);
		card* postPtr = getNodeAt(thisHand, position + 1);

		curPtr = getNext(prevPtr);

		prevPtr->nextPtr = getNext(curPtr);
		postPtr->prevPtr = getPrev(curPtr);

		curPtr->prevPtr = NULL;
		curPtr->nextPtr = NULL;
	}

	thisHand->cardCount -= 1;
}


// End of Linked List Functions Section
////////////////////////////////////////////////////////////////////////
//Beginning of Game functions Sections

void readDeck(card deck[], FILE* inp)
{
	int i;
	if (inp == NULL)
	{
		printf("Error opening file.");
	}
	while (!feof(inp))
	{
		for (i = 0; i < 108; i++)
		{
			fscanf(inp, "%s %d %s", deck[i].color, &deck[i].value, deck[i].action);
		}
	}
}

void shuffleDeck(card deck[])
{
	int i, j;
	card temp;

	for (i = 0; i < 107; i++)
	{
		j = rand() % (i + 1);

		temp = deck[i];
		deck[i] = deck[j];
		deck[j] = temp;
	}
}


//if control is 0: then it's centerline; else if 1, then it's  player
//void handOutCards(hand players[], int numPlayers, card deck[], int control)
void handOutCards(hand* player, card deck[], int control, int* k)
{
	int i, j, numOfCards;
	if (control == 0)
		numOfCards = 2;
	else
		numOfCards = 7;

	createHand(player);

	for (j = 0; j < numOfCards; j++)
	{
		int index = *k;
		card* newNode = (card*)malloc(sizeof(card));
		createNode(newNode, &(deck[index]), NULL, NULL);

		addCard(player, newNode);
		(*k) += 1;
	}

	/*for (i = 0; i < numPlayers; i++)
	{
		createHand(&players[i]); // Create an empty hand for each player
		for (j = 0; j < numOfCards; j++)
		{
			card* newNode = (card*)malloc(sizeof(card));
			createNode(newNode, &deck[k], NULL, NULL);
			addCard(&players[i], newNode);
			k++;
		}
	}*/

}

bool singleColorMatch(card* user, card* centerline)
{
	int exception = 2;
	if (strcmp(user->color, centerline->color) == 0)
		return true;
	else if (user->value == exception || centerline->value == exception)
		return true;
	else
		return false;
}

bool doubleColorMatch(card* user1, card* user2, card* centerline)
{
	int exception = 2;
	bool toReturn = false;

	if (strcmp(user1->color, centerline->color) == 0 && strcmp(user2->color, centerline->color) == 0)
		toReturn = true;
	else if (centerline->value == 2)
	{
		if (strcmp(user1->color, user2->color) == 0)
			toReturn = true;
		else if (user1->value == exception || user2->value == exception)
			toReturn = true;
	}
	else if (user1->value == exception && user2->value == exception)
		toReturn = true;
	else if (user1->value == exception || user2->value == exception)
	{
		if (user1->value == exception)
			toReturn = (strcmp(user2->color, centerline->color) == 0);
		else
			toReturn = (strcmp(user1->color, centerline->color) == 0);
	}
	return toReturn;
}

bool singleNumMatch(card* user, card* centerline)
{
	int exception = 11;
	if (user->value == centerline->value)
		return true;
	else if (user->value == exception || centerline->value == exception)
		return true;
	else
		return false;
}

bool doubleNumMatch(card* user1, card* user2, card* centerline)
{
	int exception = 11;
	bool toReturn = false;


	if ((user1->value + user2->value) == centerline->value)
		toReturn = true;
	else if (centerline->value == 11)
	{
		if ((user1->value + user2->value) < 11)
			toReturn = true;
	}
	else if (user1->value == 11 && user2->value == 11)
	{
		toReturn = true;
	}
	else if (user1->value == 11 || user2->value == 11)
	{
		if (user1->value == 11)
			toReturn = (user2->value < 10);
		else
			toReturn = (user1->value < 10);
	}
	return toReturn;
}

//take one card from player, compare to centerline, determine matches and Bonuses.
bool playOneCard(hand* player, hand* centerLine, card** curCenterPtr, int centerlinePosition, int* singleBonus)
{
	int userPosition;
	scanf("%d", &userPosition);

	card* selectedCard = getNodeAt(player, userPosition);
	card* centerlineCard = getNodeAt(centerLine, centerlinePosition);

	printf("\nPlayer One selected card ");
	printNode(selectedCard);
	printf("\n");

	bool isNumMatch = singleNumMatch(selectedCard, centerlineCard);

	if (isNumMatch)
	{
		//printf("\n ****color or value match");
		if (singleColorMatch(selectedCard, centerlineCard))
			(*singleBonus) += 1;

		*curCenterPtr = getNext(*curCenterPtr);

		removeCard(player, userPosition);
		removeCard(centerLine, centerlinePosition);

		return true;
	}
	else
		return false;
}

bool playTwoCards(hand* player, hand* centerLine, card** curCenterPtr, int centerlinePosition, int* doubleBonus)
{
	int position1;
	int position2;
	scanf("%d", &position1);
	scanf("%d", &position2);

	card* selectedCard1 = getNodeAt(player, position1);
	card* selectedCard2 = getNodeAt(player, position2);
	card* centerlineCard = getNodeAt(centerLine, centerlinePosition);

	bool isNumMatch = doubleNumMatch(selectedCard1, selectedCard2, centerlineCard);

	//printf("TEST: card 1 value: %d\n", selectedCard1->value);
	//printf("TEST: card 2 value: %d\n", selectedCard2->value);
	//printf("TEST: centerCard value: %d\n", centerlineCard->value);

	if (isNumMatch)
	{
		//printf("\n ***color or value match");

		if (doubleColorMatch(selectedCard1, selectedCard2, centerlineCard))
			(*doubleBonus) += 1;

		*curCenterPtr = getNext(*curCenterPtr);

		removeCard(player, position1);
		removeCard(player, position2 - 1);
		removeCard(centerLine, centerlinePosition);

		return true;
	}
	else
		return false;
}


int getScore(hand* thePlayer)
{
	int score = 0, value;
	card* curPtr = thePlayer->headPtr;

	while (curPtr != NULL)
	{
		value = curPtr->value;
		if (value == 11)//# card
			score += 40;
		else if (value == 2) //dos card
			score += 20;
		else
			score += value;

		curPtr = getNext(curPtr);
	}
	return score;
}


//main flow functions

void makeChoice(FILE* inp, card thisDeck[108])
{
	int choice;

	printf("Lets play a game of DOS!\n\nPress 1 to shuffle the deck of DOS or 2 to open from a file: ");
	scanf("%d", &choice);

	while (choice != 1 && choice != 2)
	{
		printf("Please enter a valid input: ");
		scanf("%d", &choice);
	}
	if (choice == 1)
	{
		//read deck from original file && shuffle
		inp = fopen("CardDeck.txt", "r");
		readDeck(thisDeck, inp);
		shuffleDeck(thisDeck);

	}
	else if (choice == 2)
	{
		//read from user file && shuffle
		char fileName[50];
		printf("Enter File Name: ");
		scanf("%s", fileName);
		inp = fopen(fileName, "r");

		readDeck(thisDeck, inp);
		for (int i = 0; i < 20; i++)
		{
			shuffleDeck(thisDeck);
		}
	}
}

void displayOneOrTwo(int turn)
{
	if (turn == 1)
		printf("One");
	else
		printf("Two");
}

hand* p1_p2(hand* p1, hand* p2, int turn)
{
	if (turn == 1)
		return p1;
	else
		return p2;
}

void displayP(hand* player, int turn)
{
	printf("\nPlayer ");

	displayOneOrTwo(turn);

	printf("'s hand: ");
	printHand(player);
}

//displays player's hand and centerline
void displayPandC(hand* player, hand* centerline, int turn)
{
	displayP(player, turn);
	
	printf("Centerline: ");
	printHand(centerline);
}


int main(void)
{
	int numOfCardsPlayed, centerlinePos, cardPosition;
	int k = 0, turn = 1, score = 0;
	int singleBonus, doubleBonus;

	FILE* inFile = NULL;
	card deck[108];

	bool didMatch = false;
	bool playerWon = false;
	bool deckEmpty = false;
	bool endOfGame = false;

	hand* player1 = (hand*)malloc(sizeof(hand));
	hand* player2 = (hand*)malloc(sizeof(hand));
	hand* centerline = (hand*)malloc(sizeof(hand));

	srand(time(0));

	makeChoice(inFile, deck);

	handOutCards(player1, deck, 1, &k);
	handOutCards(player2, deck, 1, &k);
	handOutCards(centerline, deck, 0, &k);

	while (!endOfGame)
	{
		printf("Player ");
		displayOneOrTwo(turn);
		printf("'s turn.\n");

		singleBonus = 0;
		doubleBonus = 0;
		centerlinePos = 1;

		hand* thePlayer = p1_p2(player1, player2, turn);

		card* curPtrCenterline = centerline->headPtr;
		while (curPtrCenterline != NULL)
		{
			displayPandC(thePlayer, centerline, turn);

			printf("Player ");
			displayOneOrTwo(turn);
			printf(", how many cards do you wish to play on ");
			printNode(getNodeAt(centerline, centerlinePos));
			printf(": ");
			scanf("%d", &numOfCardsPlayed);

			if (numOfCardsPlayed == 1)
			{
				printf("\nSelect a card from 1 to %d: ", getLength(thePlayer));
				didMatch = playOneCard(thePlayer, centerline, &curPtrCenterline, centerlinePos, &singleBonus);
			}
			else if (numOfCardsPlayed == 2)
			{
				// for when adding values in two cards
				printf("\nSelect two cards from 1 to %d (in ascending order): ", getLength(thePlayer));
				didMatch = playTwoCards(thePlayer, centerline, &curPtrCenterline, centerlinePos, &doubleBonus);
			}
			else if (numOfCardsPlayed == 0)
			{
				didMatch = true;
				curPtrCenterline = getNext(curPtrCenterline);
				centerlinePos++;
			}

			while (!didMatch)
			{
				printf("That selection does not total to the center row card. Select again.\n");
				displayP(thePlayer, turn);

				printf("Player ");
				displayOneOrTwo(turn);
				printf(", how many cards do you wish to play on ");
				printNode(getNodeAt(centerline, centerlinePos));
				printf(": ");
				scanf("%d", &numOfCardsPlayed);

				if (numOfCardsPlayed == 1)
				{
					printf("\nSelect a card from 1 to %d: ", getLength(thePlayer));
					didMatch = playOneCard(thePlayer, centerline, &curPtrCenterline, centerlinePos, &singleBonus);
				}
				else if (numOfCardsPlayed == 2)
				{
					// for when adding values in two cards
					printf("\nSelect two cards from 1 to %d (in ascending order): ", getLength(thePlayer));
					didMatch = playTwoCards(thePlayer, centerline, &curPtrCenterline, centerlinePos, &doubleBonus);
				}
				else if (numOfCardsPlayed == 0)
				{
					didMatch = true;
					curPtrCenterline = getNext(curPtrCenterline);
					centerlinePos++;
				}
			}
		}

		printf("There are no more center row cards to match.\n");

		if (singleBonus > 0)
		{
			printf("You have %d Single Color Match Bonus(es) and get to play %d card(s) on the center row.\n", singleBonus, singleBonus);

			if (singleBonus >= getLength(thePlayer))
			{
				printf("You have more bonuses than cards left. You won the game!");
				playerWon = true;
			}
			else
			{
				displayP(thePlayer, turn);

				printf("Select %d card(s) from 1 to %d to discard and place on the center row (Ascending order): \n", singleBonus, getLength(thePlayer));
				for (int i = 0; i < singleBonus; i++)
				{
					scanf("%d", &cardPosition);
					card* theCard = getNodeAt(thePlayer, cardPosition - i);
					addCard(centerline, theCard);
					removeCard(thePlayer, cardPosition - i);
				}
				displayPandC(thePlayer, centerline, turn);
			}
		}

		if (doubleBonus > 0)
		{
			printf("You have %d Double Color Match Bonus(es) and get to play %d card(s) on the center row.\n", doubleBonus, doubleBonus);

			if (doubleBonus >= getLength(thePlayer))
			{
				printf("You have more bonuses than cards left. You won the game!");
				playerWon = true;
			}
			else
			{
				displayP(thePlayer, turn);

				printf("Select %d card(s) from 1 to %d to discard and place on the center row (Ascending order): \n", doubleBonus, getLength(thePlayer));
				for (int i = 0; i < doubleBonus; i++)
				{
					scanf("%d", &cardPosition);
					card* theCard = getNodeAt(thePlayer, cardPosition - i);
					addCard(centerline, theCard);
					removeCard(thePlayer, cardPosition - i);

					card* newNode = (card*)malloc(sizeof(card));
					createNode(newNode, &(deck[k]), NULL, NULL);
					if (turn == 1)
						addCard(player2, newNode);
					else
						addCard(player1, newNode);
					k++;
				}
				displayPandC(thePlayer, centerline, turn);
			}
		}

		//switch player turns
		if (turn == 1)
			turn++;
		else
			turn--;

		//determine if one of players' hand is empty
		if (playerWon == true || getLength(player1) == 0 || getLength(player2) == 0)
		{
			playerWon = true;
			if (getLength(player1) == 0)
			{
				score = getScore(player2);
				printf("Player 1 wins with %d points!\n", score);
			}
			else
			{
				score = getScore(player1);
				printf("Player 2 wins with %d points!\n", score);
			}
		}
		//determine if ran out of deck
		else if (k >= 105)
		{
			deckEmpty = true;
			int score1 = getScore(player2);
			int score2 = getScore(player1);

			if (score1 > score2)
				printf("Player 1 wins with %d points!\n", score1);
			else if (score1 < score2)
				printf("Player 2 wins with %d points!\n", score2);
			else
				printf("It's a draw. Both players had a score of %d points.\n", score1);
		}

		if (playerWon || deckEmpty)
			endOfGame = true;
		else
		{
			if (getLength(centerline) < 2)
			{
				card* newNode1 = (card*)malloc(sizeof(card));
				createNode(newNode1, &(deck[k]), NULL, NULL);
				addCard(centerline, newNode1);
				k++;
				card* newNode2 = (card*)malloc(sizeof(card));
				createNode(newNode2, &(deck[k]), NULL, NULL);
				addCard(centerline, newNode2);
				k++;
			}
		}
	}

	return 0;
}