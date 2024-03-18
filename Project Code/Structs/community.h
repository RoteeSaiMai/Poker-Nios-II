#ifndef COMMUNITY_H
#define COMMUNITY_H

#include "card.h"

#define MAX_COMMUNITY_CARDS 5

// Define a struct to represent the community cards
typedef struct {
  Card **cards;
  int num_cards;
} CommunityCards;

// Function to initialize the community cards
void initializeCommunityCards(CommunityCards *community);

// Function to add a card to the community cards
void addCardToCommunity(CommunityCards *community, Card *card);

// Function to print the community cards
void printCommunityCards(const CommunityCards *community);

// Function to destroy the community cards and free memory
void destroyCommunityCards(CommunityCards *community);

void resetCommunityCards(CommunityCards *community);

// Function to get pointers to all the cards in the community
Card **getAllCardsInCommunity(const CommunityCards *community);

#endif /* COMMUNITY_H */
