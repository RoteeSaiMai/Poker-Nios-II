#ifndef HAND_H
#define HAND_H

#include "card.h"
#include <stdbool.h>

#define MAX_CARDS_PER_HAND 2

// Define a struct to represent a player's hand
typedef struct {
  Card **cards;
  int num_cards;
} Hand;

// Function to initialize a player's hand
void initializeHand(Hand *hand);

// Function to add a card to a player's hand
bool addCardToHand(Hand *hand, Card *card);

// Function to print a player's hand
void printHand(const Hand *hand);

// Function to destroy a player's hand and free memory
void destroyHand(Hand *hand);

#endif /* HAND_H */
