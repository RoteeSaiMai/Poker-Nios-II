#include "evaluator.h"

#include "card.h"
#include "community.h"
#include "player.h"

// how to use:  Card **all_cards = collectAllCards(players, 2, &community,
// &total_cards);
Card **collectAllCards(Player *players, int num_players,
                       CommunityCards *community, int *total_cards) {
  // Calculate the total number of cards
  *total_cards = num_players * MAX_CARDS_PER_HAND + community->num_cards;

  // Allocate memory for the array of pointers to cards
  Card **allCards = (Card **)malloc(*total_cards * sizeof(Card *));
  if (allCards == NULL) {
    // Error handling: Memory allocation failed
    return NULL;
  }

  // Collect player cards
  int cardIndex = 0;
  for (int i = 0; i < num_players; i++) {
    Card **playerCards = getAllCardsInHand(&players[i]);
    for (int j = 0; j < MAX_CARDS_PER_HAND; j++) {
      allCards[cardIndex++] = playerCards[j];
    }
  }

  // Collect community cards
  for (int i = 0; i < community->num_cards; i++) {
    allCards[cardIndex++] = &(community->cards[i]);
  }

  return allCards;
}

// Helper function to compare card ranks (for sorting)
int compareCardRanks(const void *a, const void *b) {
  return (*(Card **)b)->rank - (*(Card **)a)->rank;
}

// Helper function to check if at least 5 cards are of the same suit
bool isFlush(const Card *cards[], int num_cards) {
  int suits[4] = {0}; // Initialize an array to count cards for each suit
  for (int i = 0; i < num_cards; i++) {
    suits[cards[i]->suit]++;
  }
  for (int i = 0; i < 4; i++) {
    if (suits[i] >= 5) {
      return true; // At least 5 cards of the same suit found
    }
  }
  return false; // Less than 5 cards of the same suit found
}

// Helper function to check if at least five cards form a straight
bool isStraight(const Card *cards[], int num_cards) {
  // Sort cards by rank
  qsort(cards, num_cards, sizeof(Card *), compareCardRanks);

  int consecutive_count = 1; // Initialize count for consecutive cards
  for (int i = 0; i < num_cards - 1; i++) {
    // Check if the current card's rank is consecutive with the next card
    if (cards[i]->rank == cards[i + 1]->rank + 1) {
      consecutive_count++;
      // If consecutive_count reaches 5, we have a straight
      if (consecutive_count >= 5) {
        return true;
      }
    } else if (cards[i]->rank != cards[i + 1]->rank) {
      // If there's a gap, reset consecutive_count
      consecutive_count = 1;
    }
  }
  // If we reach this point, no straight was found
  return false;
}

// Helper function to count duplicates of a specific rank in the hand
int countDuplicates(const Card *cards[], int num_cards) {
  int count = 0;
  for (int i = 0; i < num_cards - 1; i++) {
    // Reset duplicate count for each new rank
    int rank_count = 1;
    // Check if the current card has duplicates
    for (int j = i + 1; j < num_cards; j++) {
      if (cards[i]->rank == cards[j]->rank) {
        rank_count++;
      }
    }
    // Update the overall count with the maximum count of duplicates for any
    // rank
    if (rank_count > count) {
      count = rank_count;
    }
  }

  // printf("count: %d\n", count);
  return count;
}

// Helper function to count the occurrences of each rank in the hand (THIS DONT
// WORK  ?????? -_-)
void countRanks(const Card *cards[], int num_cards, int rank_counts[]) {
  // Initialize rank_counts array to zeros
  memset(rank_counts, 0, sizeof(int) * 13);

  // Count the occurrences of each rank
  for (int i = 0; i < num_cards; i++) {
    rank_counts[cards[i]->rank]++;
  }
}

// Helper function to compare card ranks and suits (for sorting)
int compareCard(const void *a, const void *b) {
  int rank_diff = (*(Card **)b)->rank - (*(Card **)a)->rank;
  if (rank_diff == 0) {
    return (*(Card **)b)->suit - (*(Card **)a)->suit;
  }
  return rank_diff;

  // positive: b is higher than a
  // negative: a is higher than b
  // equal: someone is cheating
}

// Helper function to get the value of the highest card in the hand
// Helper function to get the value of the highest card in the hand
Card *getHighestCardValue(const Card *cards[], int num_cards) {
  // Sort cards by rank
  qsort(cards, num_cards, sizeof(Card *), compareCard);
  return cards[0]; // Return the pointer to the highest card in the hand
}

/*int main() {
  // Example hand of cards
  Card *hand[] = {
      &(Card){TEN, HEARTS},     // 10 of Hearts
      &(Card){QUEEN, DIAMONDS}, // Q of Diamonds
      &(Card){QUEEN, CLUBS},    // Queen of Clubs
      &(Card){TWO, SPADES},     // 2 of Spades
      &(Card){QUEEN, SPADES}    // 7 of Diamonds
  };
  int num_cards = sizeof(hand) / sizeof(hand[0]);

  // Get the highest card value in the hand
  Card *highest_card = getHighestCardValue(hand, num_cards);

  // Print the highest card value
  printf("Highest Card: Rank %s, Suit %s\n", rankToString(highest_card->rank),
         suitToString(highest_card->suit));

  return 0;
}*/

// Helper function to get the value of the kicker (highest card not part of a
// pair/trip/quad)
int getKickerValue(const Card *cards[], int num_cards, int exclude_value) {
  for (int i = 0; i < num_cards; i++) {
    if (cards[i]->rank != exclude_value) {
      return cards[i]->rank;
    }
  }
  return -1; // No kicker found
}

// Main function to evaluate the strength of the hand
int evaluateHand(const Card *cards[], int num_cards) {
  // Sort cards by rank
  qsort(cards, num_cards, sizeof(Card *), compareCardRanks);

  // Check for flush and straight flush
  bool flush = isFlush(cards, num_cards);
  bool straight = isStraight(cards, num_cards);

  if (flush && straight) {

    // Royal flush
    if (cards[0]->rank == ACE && cards[1]->rank >= KING &&
        cards[2]->rank >= QUEEN && cards[3]->rank >= JACK &&
        cards[4]->rank >= TEN) {
      return ROYAL_FLUSH;
    }

    for (int i = 0; i < num_cards - 1; i++) {
      if (cards[i]->rank != cards[i + 1]->rank + 1) {
        return STRAIGHT_FLUSH;
      }
    }
  }

  int dupeCount = countDuplicates(cards, num_cards);

  // Check for four of a kind
  if (dupeCount == 4) {
    return FOUR_OF_A_KIND;
  }

  // Check for full house
  if (dupeCount == 3) {
    // Loop through all cards to find possible three of a kind and two of a kind
    // combinations
    for (int i = 0; i < num_cards - 2; i++) {
      if (cards[i]->rank == cards[i + 1]->rank &&
          cards[i]->rank == cards[i + 2]->rank) {
        // We found a three of a kind at index i
        for (int j = 0; j < num_cards - 1; j++) {
          if (j != i && j + 1 != i && j + 2 != i &&
              cards[j]->rank == cards[j + 1]->rank) {
            // We found a two of a kind at index j
            return FULL_HOUSE;
          }
        }
      }
    }
  }

  // Check for flush
  if (flush) {
    return FLUSH;
  }

  // Check for straight
  if (straight) {
    return STRAIGHT;
  }

  // Check for three of a kind
  if (dupeCount == 3) {
    return THREE_OF_A_KIND;
  }

  if (dupeCount == 2) {
    // We know there are at least two cards of the same rank.
    // We need to check if there are two distinct pairs.
    int num_pairs = 0;
    for (int i = 0; i < num_cards - 1; i++) {
      if (cards[i]->rank == cards[i + 1]->rank) {
        num_pairs++;
        // Skip the next card to ensure we're counting distinct pairs
        i++;
      }
    }
    if (num_pairs == 2) {
      return TWO_PAIR;
    } else {
      return ONE_PAIR;
    }
  }

  // High card
  return HIGH_CARD;
}
/*
int main() {
 #include "card.h"
#include "community.h"
#include "deck.h"
#include "evaluator.h"
#include "player.h"
#include <stdio.h>

int main() {
  // Example hand
  Card *hand[7];
  hand[0] = createCard(ACE, SPADES);
  hand[1] = createCard(QUEEN, SPADES);
  hand[2] = createCard(FOUR, HEARTS);
  hand[3] = createCard(TEN, SPADES);
  hand[4] = createCard(FOUR, DIAMONDS);
  hand[5] = createCard(KING, SPADES);
  hand[6] = createCard(JACK, SPADES);

  int num_cards = sizeof(hand) / sizeof(hand[0]);

  // Evaluate hand
  int hand_strength = evaluateHand(hand, num_cards);

  printHandRank(hand_strength);

  return 0;
}
}*/

int compareHands(const Card *hand1[], int num_cards_hand1, const Card *hand2[],
                 int num_cards_hand2) {
  printf("\nComparing hands...\n");

  // Find the best card for each hand
  Card *hand1_best_card = getHighestCardValue(hand1, num_cards_hand1);
  Card *hand2_best_card = getHighestCardValue(hand2, num_cards_hand2);

  if (hand1_best_card == NULL || hand2_best_card == NULL) {
    printf("Error: Unable to find the best card for comparison.\n");
    return 0; // Handle the error condition
  }

  // Compare the best cards
  int compareResult = compareCard((const void *)&hand1_best_card,
                                  (const void *)&hand2_best_card);

  if (compareResult < 0) {
    // printf("Player 1 wins with the best card:\n");
    printCard(hand1_best_card);
    return 1;
  } else if (compareResult > 0) {
    // printf("Player 2 wins with the best card:\n");
    printCard(hand2_best_card);
    return -1;
  } else {
    printf("It's a tie.\n");
    return 0;
  }
}

/*
int main() {
  // Example hand 1
  Card *hand1[2];
  hand1[0] = createCard(ACE, HEARTS);
  hand1[1] = createCard(QUEEN, DIAMONDS);
  int num_cards_hand1 = sizeof(hand1) / sizeof(hand1[0]);
  // Example hand 2
  Card *hand2[2];
  hand2[0] = createCard(TEN, HEARTS);
  hand2[1] = createCard(ACE, SPADES);
  int num_cards_hand2 = sizeof(hand2) / sizeof(hand2[0]);

  // Compare hands
  int winner = compareHands(hand1, num_cards_hand1, hand2, num_cards_hand2);

  // Free memory for each card
  for (int i = 0; i < 5; i++) {
    destroyCard(hand1[i]);
    destroyCard(hand2[i]);
  }
  return 0;
}*/

int determineWinner(Player players[], int num_players,
                    CommunityCards *community) {
  int best_rank = HIGH_CARD; // Initialize with the lowest rank
  int best_player = -1;      // Initialize with an invalid player index

  // Evaluate hands of all players
  for (int i = 0; i < num_players; i++) {
    Card *allCards[2 + MAX_COMMUNITY_CARDS];
    int total_cards = 2 + community->num_cards;
    Card **playerCards = getAllCardsInHand(&players[i]);
    for (int j = 0; j < 2; j++) {
      allCards[j] = playerCards[j];
    }
    Card **communityCards = getAllCardsInCommunity(community);
    for (int j = 0; j < community->num_cards; j++) {
      allCards[2 + j] = communityCards[j];
    }
    int rank = evaluateHand(allCards, total_cards);

    // Update best rank and player if necessary
    printf("Player %d's hand rank: ", i + 1);
    printHandRank(rank);

    if (players[i].folded) {
      printf("Player %d has folded, so will not be accounted.\n", i + 1);
      rank = -1;
    }

    if (rank > best_rank) {
      best_rank = rank;
      best_player = i;
    } else if (rank == best_rank) {
      // Handle tie
      printf("\nTwo players have the same rank.\n");
      int result =
          compareHands(players[best_player].hand.cards, MAX_CARDS_PER_HAND,
                       players[i].hand.cards, MAX_CARDS_PER_HAND);
      if (result < 0) {
        // Update best player if the current player has a better comparison hand
        // value
        best_player = i;
      }
    }
  }

  printf("\nBest hand rank: ");
  printHandRank(best_rank);
  printf("Winner: Player %d\n", best_player + 1);
  printf("\n");
  return best_player; // Return the index of the winning
}

/*
#include "card.h"
#include "community.h"
#include "deck.h"
#include "evaluator.h"
#include "player.h"
#include <stdio.h>

int main() {
  // Example players
  Player players[2];
  initializePlayer(&players[0], 0);
  initializePlayer(&players[1], 0);

  // Example community cards
  CommunityCards community;
  initializeCommunityCards(&community);
  addCardToCommunity(&community, createCard(ACE, SPADES));
  addCardToCommunity(&community, createCard(KING, SPADES));
  addCardToCommunity(&community, createCard(QUEEN, SPADES));
  addCardToCommunity(&community, createCard(JACK, SPADES));
  addCardToCommunity(&community, createCard(TEN, SPADES));

  // Player 1's hand
  addCardToHand(&players[0].hand, createCard(TWO, SPADES));
  addCardToHand(&players[0].hand, createCard(THREE, SPADES));

  // Player 2's hand
  addCardToHand(&players[1].hand, createCard(NINE, SPADES));
  addCardToHand(&players[1].hand, createCard(EIGHT, SPADES));

  // Evaluate winner
  int winner = determineWinner(players, 2, &community);

  // Clean up memory
  destroyPlayer(&players[0]);
  destroyPlayer(&players[1]);
  destroyCommunityCards(&community);

  return 0;
}
}
  */

void printHandRank(int hand_rank) {
  // Print the hand rank
  switch (hand_rank) {
  case HIGH_CARD:
    printf("High Card\n");
    break;
  case ONE_PAIR:
    printf("One Pair\n");
    break;
  case TWO_PAIR:
    printf("Two Pair\n");
    break;
  case THREE_OF_A_KIND:
    printf("Three of a Kind\n");
    break;
  case STRAIGHT:
    printf("Straight\n");
    break;
  case FLUSH:
    printf("Flush\n");
    break;
  case FULL_HOUSE:
    printf("Full House\n");
    break;
  case FOUR_OF_A_KIND:
    printf("Four of a Kind\n");
    break;
  case STRAIGHT_FLUSH:
    printf("Straight Flush\n");
    break;
  case ROYAL_FLUSH:
    printf("Royal Flush\n");
    break;
  default:
    printf("Invalid Hand\n");
    break;
  }
}
