#include "player.h"
#include <stdio.h>

// Function to initialize a player
void initializePlayer(Player *player, int initial_money) {
  initializeHand(&player->hand);
  player->money = initial_money;
  player->moneySpent = 0;

  // Enable all actions by default
  for (int i = 0; i < NUM_ACTIONS; i++) {
    player->available_actions[i] = true;
  }
  // Initialize other fields as needed
}

// Function to destroy a player and free memory
void destroyPlayer(Player *player) {
  destroyHand(&player->hand);
  // Free any other resources if necessary
}

// Function to print the amount of money a player has
void printMoney(const Player *player) {
  printf("Player's money: %d\n", player->money);
}

void printAllPlayerMoney(Player players[], int num_players) {
  for (int i = 0; i < num_players; i++) {
    printf("\nPlayer %d's Money Left: %d", i + 1, players[i].money);
  }
  printf("\n");
}

// Function to show a player's hand
void showHand(const Player *player) {
  printf("\n");
  printHand(&player->hand);
  // printf("\n");
}

void showAllPlayerHand(Player players[], int num_players) {
  for (int i = 0; i < num_players; i++) {
    printf("Player %d:", i + 1);
    printHand(&players[i].hand);
    printf("\n");
  }
}

void resetPlayer(Player *player) {
  destroyHand(&player->hand);
  player->folded = false;
  player->moneySpent = 0;
  // Enable all actions by default
  for (int i = 0; i < NUM_ACTIONS; i++) {
    player->available_actions[i] = true;
  }
}

// Function to give money to a player
void giveMoney(Player *player, int amount) { player->money += amount; }

// Function to take money from a player
void takeMoney(Player *player, int amount) {
  player->money -= amount;
  player->moneySpent += amount;
}

// Function to get pointers to all the cards in a player's hand
Card **getAllCardsInHand(const Player *player) { return player->hand.cards; }

void setActionAvailability(Player *player, Action action, bool available) {
  player->available_actions[action] = available;
}

// Action functions

bool performFold(Player *player) {
  if (player->available_actions[FOLD]) {
    // Implement fold action here
    printf("Player folds.\n");
    return true;
  }
  return false; // Action not available
}

bool performCheck(Player *player) {
  if (player->available_actions[CHECK]) {
    // Implement check action here
    printf("Player checks.\n");
    return true;
  }
  return false; // Action not available
}

bool performBet(Player *player, int amount) {
  if (player->available_actions[BET]) {
    // Implement bet action here
    printf("Player bets $%d.\n", amount);
    takeMoney(player, amount);
    return true;
  }
  return false; // Action not available
}

bool performCall(Player *player, int amount) {
  if (player->available_actions[CALL]) {
    // Implement call action here
    printf("Player calls $%d.\n", amount);
    takeMoney(player, amount);
    return true;
  }
  return false; // Action not available
}

bool performRaise(Player *player, int amount) {
  if (player->available_actions[RAISE]) {
    // Implement raise action here
    printf("Player raises to $%d.\n", amount);
    takeMoney(player, amount);
    return true;
  }
  return false; // Action not available
}

bool performAllIn(Player *player) {
  if (player->available_actions[ALL_IN]) {
    // Implement all-in action here
    printf("Player goes all-in!\n");
    takeMoney(player, player->money);
    return true;
  }
  return false; // Action not available
}