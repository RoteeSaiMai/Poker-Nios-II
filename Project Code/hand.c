#include "hand.h"
#include <stdlib.h> // for malloc and free

// Function to initialize a player's hand
void initializeHand(Hand *hand) {
  hand->cards = malloc(MAX_CARDS_PER_HAND * sizeof(Card *));
  if (hand->cards == NULL) {
    printf("Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  hand->num_cards = 0; // Initialize with no cards
}

// Function to add a card to a player's hand
bool addCardToHand(Hand *hand, Card *card) {
  if (hand->num_cards < MAX_CARDS_PER_HAND) {
    hand->cards[hand->num_cards++] = card;
    return true; // Card added successfully
  } else {
    return false; // Hand is already full
  }
}

// Function to print a player's hand
void printHand(const Hand *hand) {
  printf("Player's Hand:\n");
  for (int i = 0; i < hand->num_cards; i++) {
    printf("%d. ", i + 1);
    printCard(hand->cards[i]);
  }
}

// Function to destroy a player's hand and free memory
void destroyHand(Hand *hand) {
  for (int i = 0; i < hand->num_cards; i++) {
    if (hand->cards[i]) {
      destroyCard(hand->cards[i]);
      hand->cards[i] = NULL;
    }
  }

  if (hand->cards) {
    free(hand->cards);
    hand->cards = NULL;
  }

  hand->num_cards = 0;
}
