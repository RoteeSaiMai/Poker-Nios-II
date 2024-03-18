#ifndef CARD_H
#define CARD_H

#include <stdio.h>
#include <stdlib.h> // for abs()

// Define an enum for card ranks
typedef enum {
  TWO = 2,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING,
  ACE
} Rank;

// Define an enum for card suits
typedef enum { CLUBS, DIAMONDS, HEARTS, SPADES } Suit;

// Define a struct to represent a card
typedef struct {
  Rank rank;
  Suit suit;
} Card;

// Helper function to convert rank enum value to string
const char *rankToString(Rank rank);

// Helper function to convert suit enum value to string
const char *suitToString(Suit suit);

// Function to dynamically allocate memory for a card
Card *createCard(Rank rank, Suit suit);

// Function to free memory allocated for a card
void destroyCard(Card *card);

// Function to print a card's details
void printCard(const Card *card);

void printCardArray(const Card *cards[], int num_cards);

#endif /* CARD_H */
