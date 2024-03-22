#include <stdio.h>

#include "card.h"
#include "community.h"
#include "deck.h"
#include "evaluator.h"
#include "player.h"

#define NUM_PLAYERS 3
#define NUM_FLOP_CARDS 3
#define NUM_TURN_CARDS 1
#define NUM_RIVER_CARDS 1
#define BIG_BLIND_AMOUNT 10
#define SMALL_BLIND_AMOUNT 5
#define INITIAL_PLAYER_MONEY 100

// Function to prompt player for action
Action getPlayerAction(Player *player) {
  Action action;
  printf("Player action (fold/check/bet/call/raise/all-in): ");
  char input[10];
  scanf("%s", input);

  if (strcmp(input, "fold") == 0) {
    action = FOLD;
  } else if (strcmp(input, "check") == 0) {
    action = CHECK;
  } else if (strcmp(input, "bet") == 0) {
    action = BET;
  } else if (strcmp(input, "call") == 0) {
    action = CALL;
  } else if (strcmp(input, "raise") == 0) {
    action = RAISE;
  } else if (strcmp(input, "all-in") == 0) {
    action = ALL_IN;
  } else {
    printf("Invalid action. Please try again.\n");
    action = getPlayerAction(player);
  }
  return action;
}

// Function to check if the game should end early
bool checkGameEnd(Player players[], int num_players) {
  // printf("checking gem end sldkfjlkjsdkljf");
  int active_players = 0;

  // Count active players and calculate total money
  for (int i = 0; i < num_players; i++) {
    if (!players[i].folded) {
      active_players++;
    }
  }

  if (active_players == 1) return true;
  return false;
}

// idk why this doens't work if i put it in the main and pass it as a
// pointer???????? im too dumb for this
int current_bet;

// Function to handle the betting round
void bettingRound(Player players[], int num_players, int *pot) {
  // Initialize current_bet only once at the start of the round
  // int current_bet = 0; // The current bet amount
  int bet_amount = 0;

  // Loop through players and handle their actions
  for (int i = 0; i < num_players; i++) {
    if (!players[i].folded) {
      printf("Player %d's turn.\n", i + 1);
      Action action = getPlayerAction(&players[i]);
      switch (action) {
        case FOLD:
          players[i].folded = true;
          break;
        case BET:
          printf("Enter bet amount (minimum: %d): ", current_bet);
          scanf("%d", &bet_amount);
          if (bet_amount >= current_bet) {
            if (bet_amount <= players[i].money) {
              *pot += bet_amount;
              current_bet = bet_amount;
              takeMoney(&players[i], bet_amount);
            } else {
              printf("Bet amount must not exceed the money you have\n");
              i--;
            }
          } else {
            printf("Bet amount must be at least %d.\n", current_bet);
            i--;
          }
          break;
        case RAISE:
          printf("Enter raise amount (minimum: %d): ", current_bet * 2);
          scanf("%d", &bet_amount);
          if (bet_amount >= current_bet * 2) {
            if (bet_amount <= players[i].money) {
              *pot += bet_amount;
              current_bet = bet_amount;
              takeMoney(&players[i], bet_amount);
            } else {
              printf("Bet amount must not exceed the money you have\n");
              i--;
            }
          } else {
            printf("Raise amount must be at least %d.\n", current_bet * 2);
            i--;
          }
          break;

          printf("Does this spot ever get reached?");

          // Adjust pot and current_bet
          *pot += bet_amount;
          current_bet = bet_amount;

          // Take money from the player
          takeMoney(&players[i], bet_amount);
          break;

        case CALL:
          *pot += current_bet;
          takeMoney(&players[i], current_bet);
          break;
        case CHECK:
          // Do nothing, move to the next player
          break;
        case ALL_IN:
          if (players[i].money > 0) {
            *pot += players[i].money;
            takeMoney(&players[i], players[i].money);
          } else {
            printf("You have no money left");
            i--;
          }

          break;
        default:
          printf("Invalid action\n");
          i--;
      }
      // printf("\nPot: %d\n", *pot);
    }
    if (checkGameEnd(players, num_players)) break;
  }
}

int main() {
  Deck deck;
  initializeDeck(&deck);
  shuffleDeck(&deck);

  // displayDeck(&deck);

  // Initialize players
  Player players[NUM_PLAYERS];
  for (int i = 0; i < NUM_PLAYERS; i++) {
    initializePlayer(&players[i], INITIAL_PLAYER_MONEY);
  }

  // Initialize community cards
  CommunityCards community;
  initializeCommunityCards(&community);

  int pot;
  // int current_bet
  // this doens't work????????

  while (true) {
    // Deduct blinds from players' money

    bool gameEnd = false;

    for (int i = 0; i < NUM_PLAYERS; i++) {
      initializeHand(&players[i].hand);
    }

    takeMoney(&players[0], SMALL_BLIND_AMOUNT);
    takeMoney(&players[1], BIG_BLIND_AMOUNT);

    // reset pot
    pot = SMALL_BLIND_AMOUNT + BIG_BLIND_AMOUNT;
    current_bet = BIG_BLIND_AMOUNT;

    // Gameplay loop

    // Deal hole cards to players
    for (int i = 0; i < NUM_PLAYERS; i++) {
      for (int j = 0; j < 2; j++) {
        addCardToHand(&players[i].hand, drawTopCardFromDeck(&deck));
      }
    }

    // we are playing as player 1
    printAllPlayerMoney(players, NUM_PLAYERS);
    printf("Pot: %d\n\n", pot);

    showHand(&players[0]);

    // Preflop betting round
    printf("Preflop betting round:\n");
    bettingRound(players, NUM_PLAYERS, &pot);
    printf("Pot: %d\n", pot);

    if (checkGameEnd(players, NUM_PLAYERS)) {
      gameEnd = true;
      goto gameEnd;
    }

    // Deal flop
    printf("\nFlop:\n");
    for (int i = 0; i < NUM_FLOP_CARDS; i++) {
      addCardToCommunity(&community, drawTopCardFromDeck(&deck));
    }
    printCommunityCards(&community);

    printAllPlayerMoney(players, NUM_PLAYERS);
    printf("Pot: %d\n\n", pot);

    // Flop betting round
    printf("\nFlop betting round:\n");
    showHand(&players[0]);
    bettingRound(players, NUM_PLAYERS, &pot);

    printf("Pot: %d\n", pot);

    if (checkGameEnd(players, NUM_PLAYERS)) {
      gameEnd = true;
      goto gameEnd;
    }

    // Deal turn
    printf("\nTurn:\n");
    addCardToCommunity(&community, drawTopCardFromDeck(&deck));
    printCommunityCards(&community);

    printAllPlayerMoney(players, NUM_PLAYERS);
    printf("Pot: %d\n\n", pot);

    // Turn betting round
    printf("\nTurn betting round:\n");
    showHand(&players[0]);
    bettingRound(players, NUM_PLAYERS, &pot);
    printAllPlayerMoney(players, NUM_PLAYERS);
    printf("Pot: %d\n\n", pot);

    if (checkGameEnd(players, NUM_PLAYERS)) {
      gameEnd = true;
      goto gameEnd;
    }

    // Deal river
    printf("\nRiver:\n");
    addCardToCommunity(&community, drawTopCardFromDeck(&deck));
    printCommunityCards(&community);

    printAllPlayerMoney(players, NUM_PLAYERS);

    // River betting round
    printf("\nRiver betting round:\n");
    showHand(&players[0]);
    bettingRound(players, NUM_PLAYERS, &pot);

    if (checkGameEnd(players, NUM_PLAYERS)) {
      gameEnd = true;
      goto gameEnd;
    }

  // jump destination
  gameEnd:
    printf("GAME OVER!!!!!!!!\n");

    printAllPlayerMoney(players, NUM_PLAYERS);
    printf("Pot: %d\n\n", pot);

    showAllPlayerHand(players, NUM_PLAYERS);
    printCommunityCards(&community);

    int winner_index;
    if (gameEnd) {
      // game has ended prematurely by elimination

      int notFolded = 0;
      for (int i = 0; i < NUM_PLAYERS; i++) {
        if (players[i].folded == false) {
          winner_index = i;
          notFolded++;
        }
      }

      // game didn't end by last man standing

      // assume it's cuz only 1 player has money left
      if (notFolded > 1) {
        for (int i = 0; i < NUM_PLAYERS; i++) {
          if (players[i].money > 0) {
            winner_index = i;
          }
        }
        printf("\nVictory by loss of money\n");
      } else {
        printf("\nVictory by folding\n");
      }

    } else {
      // Evaluate hands and determine winner
      printf("\nEvaluating hands...\n");
      // Collect all cards
      int total_cards = NUM_PLAYERS * 2 + community.num_cards;
      Card *all_cards[total_cards];
      int cardIndex = 0;
      for (int i = 0; i < NUM_PLAYERS; i++) {
        Card **playerCards = getAllCardsInHand(&players[i]);
        for (int j = 0; j < 2; j++) {
          all_cards[cardIndex++] = playerCards[j];
        }
      }
      for (int i = 0; i < community.num_cards; i++) {
        all_cards[cardIndex++] = &community.cards[i];
      }

      // Evaluate hands
      int ranks[NUM_PLAYERS];
      for (int i = 0; i < NUM_PLAYERS; i++) {
        Card *allCards[2 + MAX_COMMUNITY_CARDS];
        int total_cards = 2 + community.num_cards;
        Card **playerCards = getAllCardsInHand(&players[i]);
        for (int j = 0; j < 2; j++) {
          allCards[j] = playerCards[j];
        }
        Card **communityCards = getAllCardsInCommunity(&community);
        for (int j = 0; j < community.num_cards; j++) {
          allCards[2 + j] = communityCards[j];
        }
        ranks[i] = evaluateHand(allCards, total_cards);
      }

      printf("\nHand Ranks:\n");
      for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("Player %d: ", i + 1);
        printHandRank(ranks[i]);
      }

      // Above this is kinda redundant
      winner_index = determineWinner(players, NUM_PLAYERS, &community);
    }

    printf("Player %d wins!\n", winner_index + 1);
    giveMoney(&players[winner_index], pot);
    printf("Player %d received %d from the pot.\n", winner_index + 1, pot);

    // reset cards
    for (int i = 0; i < NUM_PLAYERS; i++) {
      resetPlayer(&players[i]);
    }
    resetCommunityCards(&community);

    displayDeck(&deck);

    // while loop

    // this results in a seg fault for some reason???

    bool playerBroke = false;
    for (int i = 0; i < NUM_PLAYERS; i++) {
      if (players[i].money == 0) {
        printf("\nPlayer %d is broke beyond repair!\n", i + 1);
        playerBroke = true;
      }
    }

    if (playerBroke) {
      printf("Rip no money :(((((\n");
      printf("\n\nEnding the game...\n");
      break;
    }
  }

  // Cleanup
  for (int i = 0; i < NUM_PLAYERS; i++) {
    printf("\nKilling Player %d\n", i + 1);
    destroyPlayer(&players[i]);
  }
  printf("\nBreaking up communities\n");
  destroyCommunityCards(&community);

  printf("\nBurning decks\n");
  destroyDeck(&deck);

  printf("\nGGWP");

  return 0;
}

/*
int main() {

  Deck deck;

  // Initialize and shuffle the deck
  initializeDeck(&deck);
  shuffleDeck(&deck);

  // Initialize players
  Player players[NUM_PLAYERS];
  for (int i = 0; i < NUM_PLAYERS; i++) {
    initializePlayer(&players[i]);
  }

  // Initialize community cards
  CommunityCards community;
  initializeCommunityCards(&community);

  // INITIALIZATION
  // -----------------------------------------------------------------

  // Display the deck before drawing
  printf("Initial deck:\n");
  displayDeck(&deck);
  printf("\n");

  // Draw two cards for each player and add them to their hands
  for (int i = 0; i < NUM_PLAYERS; i++) {
    for (int j = 0; j < 2; j++) {
      addCardToHand(&players[i].hand, drawTopCardFromDeck(&deck));
    }
  }

  // Print players' hands and community cards
  for (int i = 0; i < NUM_PLAYERS; i++) {
    printf("\nPlayer %d's Hand:", i + 1);
    showHand(&players[i]);
  }

  // Deal the flop (three community cards)
  for (int i = 0; i < 3; i++) {
    addCardToCommunity(&community, drawTopCardFromDeck(&deck));
  }

  printCommunityCards(&community);

  // Deal the turn (one more community card)
  addCardToCommunity(&community, drawTopCardFromDeck(&deck));

  printCommunityCards(&community);

  // Deal the river (final community card)
  addCardToCommunity(&community, drawTopCardFromDeck(&deck));

  // check if you can get all the cards (this is a function in evaluator.c))
  // REDUNDANT
  // Collect all cards into a single array of pointers
  int total_cards = NUM_PLAYERS * MAX_CARDS_PER_HAND + community.num_cards;
  Card *allCards[total_cards];
  int cardIndex = 0;

  // Collect player cards
  for (int i = 0; i < NUM_PLAYERS; i++) {
    Card **playerCards = getAllCardsInHand(&players[i]);
    for (int j = 0; j < MAX_CARDS_PER_HAND; j++) {
      allCards[cardIndex++] = playerCards[j];
    }
  }

  // Collect community cards
  Card **communityCards = getAllCardsInCommunity(&community);
  for (int i = 0; i < community.num_cards; i++) {
    allCards[cardIndex++] = communityCards[i];
  }

  printf("\n");

  // Print all cards using the printCardArray function
  printCardArray(allCards, total_cards);

  printf("\n");

  // end

  printCommunityCards(&community);

  // Display the updated deck
  printf("\nUpdated deck:\n");
  displayDeck(&deck);

  // Cleanup: free memory for players and community cards
  for (int i = 0; i < NUM_PLAYERS; i++) {
    destroyPlayer(&players[i]);
  }
  destroyCommunityCards(&community);

  return 0;
}
*/

// Evaluator: It doens't work yet lmaoooo
/*int main() {
  // Example player hand
  Card hand[] = {{SEVEN, SPADES}, {SEVEN, HEARTS}};
  int num_hand_cards = sizeof(hand) / sizeof(hand[0]);

  // Example community cards
  Card community[] = {{ACE, CLUBS}, {SEVEN, DIAMONDS}, {TWO, SPADES}};
  int num_community_cards = sizeof(community) / sizeof(community[0]);

  // Combine player hand and community cards into a single array
  const Card *all_cards[num_hand_cards + num_community_cards];
  for (int i = 0; i < num_hand_cards; i++) {
    all_cards[i] = &hand[i];
  }
  for (int i = 0; i < num_community_cards; i++) {
    all_cards[num_hand_cards + i] = &community[i];
  }

  // Evaluate the hand
  int hand_rank = evaluateHand(all_cards, num_hand_cards + num_community_cards);

  printHandRank(hand_rank);

  return 0;
}
*/