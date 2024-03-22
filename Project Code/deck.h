#ifndef DECK_H
#define DECK_H

#include <stdbool.h>

#include "card.h"

// Define a struct to represent a deck of cards
#define NUM_RANKS 13
#define NUM_SUITS 4
#define NUM_CARDS (NUM_RANKS * NUM_SUITS)

// Define a struct to represent a deck of cards
typedef struct {
  Card **cards;
  int num_cards;
} Deck;

// Function to initialize a deck of cards
void initializeDeck(Deck *deck);

// Function to shuffle a deck of cards
void shuffleDeck(Deck *deck);

// Function to draw the top card from a deck
Card *drawTopCardFromDeck(Deck *deck);

// Function to print the contents of a deck
void displayDeck(const Deck *deck);

// Function to destroy a deck and free memory
void destroyDeck(Deck *deck);

#endif /* DECK_H */