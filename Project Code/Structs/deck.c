#include "deck.h"
#include <stdbool.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For seeding srand()

// Function to initialize a deck of cards
void initializeDeck(Deck *deck) {
  deck->cards = malloc(NUM_CARDS * sizeof(Card *));
  if (deck->cards == NULL) {
    printf("Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }

  // Fill the deck with cards
  int index = 0;
  for (int rank = TWO; rank <= ACE; rank++) {
    for (int suit = CLUBS; suit <= SPADES; suit++) {
      deck->cards[index] = createCard(rank, suit);
      index++;
    }
  }
  deck->num_cards = NUM_CARDS;
}

// Function to shuffle a deck of cards
void shuffleDeck(Deck *deck) {
  srand((unsigned int)time(NULL)); // Seed the random number generator
  for (int i = 0; i < deck->num_cards; i++) {
    int j = rand() % deck->num_cards; // Generate a random index
    // Swap cards at indices i and j
    Card *temp = deck->cards[i];
    deck->cards[i] = deck->cards[j];
    deck->cards[j] = temp;
  }
}

// Function to draw the top card from a deck
Card *drawTopCardFromDeck(Deck *deck) {
  if (deck->num_cards > 0) {
    // Store a copy of the drawn card
    Card *drawnCard = deck->cards[0];

    // Shift remaining cards to fill the gap
    for (int i = 1; i < deck->num_cards; i++) {
      deck->cards[i - 1] = deck->cards[i];
    }
    deck->num_cards--; // Decrease the number of cards in the deck

    return drawnCard;
  } else {
    return NULL; // Deck is empty
  }
}

// Function to print the contents of a deck
void displayDeck(const Deck *deck) {
  printf("\nDeck Contents (%d cards left):\n", deck->num_cards);
  for (int i = 0; i < deck->num_cards; i++) {
    printf("%d. ", i + 1);
    printCard(deck->cards[i]);
  }
}

// Function to destroy a deck and free memory
void destroyDeck(Deck *deck) {
  for (int i = 0; i < deck->num_cards; i++) {
    destroyCard(deck->cards[i]);
  }
  free(deck->cards);
}
