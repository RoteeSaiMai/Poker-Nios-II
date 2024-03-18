#include "card.h"

// Helper function to convert rank enum value to string
const char *rankToString(Rank rank) {
  switch (rank) {
  case TWO:
    return "TWO";
  case THREE:
    return "THREE";
  case FOUR:
    return "FOUR";
  case FIVE:
    return "FIVE";
  case SIX:
    return "SIX";
  case SEVEN:
    return "SEVEN";
  case EIGHT:
    return "EIGHT";
  case NINE:
    return "NINE";
  case TEN:
    return "TEN";
  case JACK:
    return "JACK";
  case QUEEN:
    return "QUEEN";
  case KING:
    return "KING";
  case ACE:
    return "ACE";
  default:
    return "INVALID_RANK";
  }
}

// Helper function to convert suit enum value to string
const char *suitToString(Suit suit) {
  switch (suit) {
  case CLUBS:
    return "CLUBS";
  case DIAMONDS:
    return "DIAMONDS";
  case HEARTS:
    return "HEARTS";
  case SPADES:
    return "SPADES";
  default:
    return "INVALID_SUIT";
  }
}

// Function to dynamically allocate memory for a card
Card *createCard(Rank rank, Suit suit) {
  Card *card = (Card *)malloc(sizeof(Card));
  if (card != NULL) {
    card->rank = rank;
    card->suit = suit;
  } else {
    printf("Memory allocation failed.\n");
  }
  return card;
}

// Function to free memory allocated for a card
void destroyCard(Card *card) { free(card); }

// Function to print a card's details
void printCard(const Card *card) {
  printf("%s of %s\n", rankToString(card->rank), suitToString(card->suit));
}

// somehow main can't find this function? (it's also in card.c)
void printCardArray(const Card *cards[], int num_cards) {
  printf("Cards:\n");
  for (int i = 0; i < num_cards; i++) {
    printf("%d. %s of %s\n", i + 1, rankToString(cards[i]->rank),
           suitToString(cards[i]->suit));
  }
}