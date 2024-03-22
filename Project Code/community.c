#include "community.h"
#include <stdlib.h> // for malloc and free

// Function to initialize the community cards
void initializeCommunityCards(CommunityCards *community) {
  community->cards = malloc(MAX_COMMUNITY_CARDS * sizeof(Card *));
  if (community->cards == NULL) {
    printf("Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  community->num_cards = 0; // Initialize with no cards
}

// Function to add a card to the community cards
void addCardToCommunity(CommunityCards *community, Card *card) {
  if (community->num_cards < MAX_COMMUNITY_CARDS) {
    community->cards[community->num_cards++] = card;
  }
}

// Function to print the community cards
void printCommunityCards(const CommunityCards *community) {
  printf("\n\nCommunity Cards:\n");
  for (int i = 0; i < community->num_cards; i++) {
    printf("%d. ", i + 1);
    printCard(community->cards[i]);
  }
  printf("\n");
}

/*// Function to display community cards
void displayCommunityCards(const CommunityCards *community) {
  printf("Community Cards: ");
  printCardArray(community->cards, community->num_cards);
  printf("\n");
}*/

// Function to destroy the community cards and free memory
void destroyCommunityCards(CommunityCards *community) {
  for (int i = 0; i < community->num_cards; i++) {
    destroyCard(community->cards[i]);
  }
  free(community->cards);
}

// Function to reset the community cards for a new round
void resetCommunityCards(CommunityCards *community) {
  for (int i = 0; i < community->num_cards; i++) {
    destroyCard(community->cards[i]); // Destroy each card
  }
  community->num_cards = 0; // Reset the number of cards to 0
}

// Function to get pointers to all the cards in the community
Card **getAllCardsInCommunity(const CommunityCards *community) {
  return community->cards;
}