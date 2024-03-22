// evaluator.h

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "card.h"
#include "community.h"
#include "player.h"

// Define ranks for poker hands
#define HIGH_CARD 0
#define ONE_PAIR 1
#define TWO_PAIR 2
#define THREE_OF_A_KIND 3
#define STRAIGHT 4
#define FLUSH 5
#define FULL_HOUSE 6
#define FOUR_OF_A_KIND 7
#define STRAIGHT_FLUSH 8
#define ROYAL_FLUSH 9

// Function to print the hand rank
void printHandRank(int hand_rank);

Card **collectAllCards(Player *players, int num_players,
                       CommunityCards *community, int *total_cards);

int compareHands(const Card *hand1[], int num_cards_hand1, const Card *hand2[],
                 int num_cards_hand2);
int compareCardRanks(const void *a, const void *b);
int compareCardRanks(const void *a, const void *b);
bool isFlush(const Card *cards[], int num_cards);
bool isStraight(const Card *cards[], int num_cards);
int countDuplicates(const Card *cards[], int num_cards);
Card *getHighestCardValue(const Card *cards[], int num_cards);
int getKickerValue(const Card *cards[], int num_cards, int exclude_value);
int evaluateHand(const Card *cards[], int num_cards);

#endif
