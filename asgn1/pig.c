#include <stdio.h>
#include <stdlib.h>
#include "names.h"

#define SEED 2021

typedef enum { SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER } Position;
const Position pig[7] = {
	SIDE ,
	SIDE ,
	RAZORBACK ,
	TROTTER ,
	SNOUTER ,
	JOWLER ,
	JOWLER
};

int main(void){
	//Asks for total players playing game (Checks for errors too)
	int totalplayers;
	printf("How many players? ");
	int playercheck = scanf("%d", &totalplayers);
	if (playercheck != 1 && (totalplayers < 2 || totalplayers > 10)) {
		totalplayers = 2;
		fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
	}
	
	//Asks for random seed input and makes the seed	(Checks for invalid input too)
	int iseed;
	printf("Random seed: ");
	int seedcheck = scanf("%d", &iseed);
	if (seedcheck != 1) {
		srand(SEED);
		fprintf(stderr, "Invalid random seed. Using 2021 instead.\n");
	}
	else {
		srand(iseed);
	}

	//makes points array holding each player's points starting with 0s
	int points[totalplayers];
	int size;

	for (size = 0; size < totalplayers; size++) {
		points[size] = 0;
	}	
	
	//Starts the game
	int currentplayer = 0;
	int sameplayer = 0;
	while (points[currentplayer] < 100) {
		int roll = rand() % 7;		
		if (sameplayer == 0) {
		printf("\n%s rolls the pig... ", names[currentplayer]);
		}
		if (pig[roll] == SIDE) {
			printf("pig lands on side ");
			currentplayer++;
			sameplayer = 0;
			currentplayer = currentplayer % totalplayers;
		}
		if (pig[roll] == RAZORBACK){
			printf("pig lands on back ");
			points[currentplayer] += 10;
			sameplayer = 1;
		}
		if (pig[roll] == TROTTER) {
			printf("pig lands on upright ");
			points[currentplayer] += 10;
			sameplayer = 1;
		}
		if (pig[roll] == SNOUTER) {
			printf("pig lands on snout ");
			points[currentplayer] += 15;
			sameplayer = 1;
		}
		if (pig[roll] == JOWLER) {
			printf("pig lands on ear ");
			points[currentplayer] += 5;
			sameplayer = 1;
		}
	}
	fprintf(stdout, "\n%s wins with %d points!\n", names[currentplayer], points[currentplayer]);
}