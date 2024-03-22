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

// Helper function to check if all cards are of the same suit
bool isFlush(const Card *cards[], int num_cards) {
  Suit suit = cards[0]->suit;
  for (int i = 1; i < num_cards; i++) {
    if (cards[i]->suit != suit) {
      return false;
    }
  }
  return true;
}

// Helper function to check if the cards form a straight
bool isStraight(const Card *cards[], int num_cards) {
  // Sort cards by rank
  qsort(cards, num_cards, sizeof(Card *), compareCardRanks);

  // Check for Ace-low straight
  if (cards[0]->rank == ACE && cards[num_cards - 1]->rank == TWO) {
    // If Ace is the lowest card and there's a two, check if the other cards are
    // consecutive
    for (int i = 1; i < num_cards - 1; i++) {
      if (cards[i]->rank != cards[i + 1]->rank + 1) {
        return false;
      }
    }
    return true;
  }

  // Check if the cards are consecutive
  for (int i = 0; i < num_cards - 1; i++) {
    if (cards[i]->rank != cards[i + 1]->rank + 1) {
      return false;
    }
  }
  return true;
}

// Helper function to count duplicates of a specific rank in the hand
int countDuplicates(const Card *cards[], int num_cards) {
  int count = 0;
  for (int i = 0; i < num_cards - 1; i++) {
    if (cards[i]->rank == cards[i + 1]->rank) {
      count++;
    }
  }
  return count;
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
    if (cards[0]->rank == ACE && cards[num_cards - 1]->rank == TEN) {
      return ROYAL_FLUSH;
    }
    // Straight flush
    return STRAIGHT_FLUSH;
  }

  // Check for four of a kind
  if (countDuplicates(cards, num_cards) == 3) {
    return FOUR_OF_A_KIND;
  }

  // Check for full house
  if (countDuplicates(cards, num_cards) == 2) {
    // Check if the first three cards are the same rank (full house)
    if (cards[0]->rank == cards[2]->rank) {
      return FULL_HOUSE;
    }
    // Check if the last three cards are the same rank (full house)
    if (cards[num_cards - 1]->rank == cards[num_cards - 3]->rank) {
      return FULL_HOUSE;
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
  if (countDuplicates(cards, num_cards) == 1) {
    return THREE_OF_A_KIND;
  }

  // Check for two pair
  if (countDuplicates(cards, num_cards) == 0 && num_cards >= 4) {
    return TWO_PAIR;
  }

  // Check for one pair
  if (countDuplicates(cards, num_cards) == 0) {
    return ONE_PAIR;
  }

  // High card
  return HIGH_CARD;
}
/*int main() {
  // Example hand
  Card *hand[5];
  hand[0] = createCard(ACE, HEARTS);
  hand[1] = createCard(KING, DIAMONDS);
  hand[2] = createCard(QUEEN, SPADES);
  hand[3] = createCard(JACK, CLUBS);
  hand[4] = createCard(TEN, HEARTS);
  int num_cards = sizeof(hand) / sizeof(hand[0]);

  // Evaluate hand
  int hand_strength = evaluateHand(hand, num_cards);

  printHandRank(hand_strength);

  return 0;
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
    printf("Player 1 wins with the best card:\n");
    printCard(hand1_best_card);
    return 1;
  } else if (compareResult > 0) {
    printf("Player 2 wins with the best card:\n");
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

// Function to determine the winner of the game
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

    if (rank > best_rank) {
      best_rank = rank;
      best_player = i;
    } else if (rank == best_rank) {
      // Handle tie
      printf("\nTwo of the same ranks, comparing raw card value...");

      const Card *hand1[2 + MAX_COMMUNITY_CARDS];
      const Card *hand2[2 + MAX_COMMUNITY_CARDS];
      for (int j = 0; j < 2; j++) {
        hand1[j] = playerCards[j];
        hand2[j] = playerCards[j]; // Use the same player's cards for comparison
      }
      for (int j = 0; j < community->num_cards; j++) {
        hand1[2 + j] = communityCards[j];
        hand2[2 + j] = communityCards[j];
      }
      int result = compareHands(hand1, total_cards, hand2, total_cards);
      if (result > 0) {
        best_player = i;
      }

      printf("\nPlayer %d has the higest rank\n", best_player + 1);
    }
  }

  printf("\n");
  return best_player; // Return the index of the winning player
}

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
