#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include <stdbool.h>

typedef enum { FOLD, CHECK, BET, CALL, RAISE, ALL_IN, NUM_ACTIONS } Action;

typedef struct {
  Hand hand;
  int money;
  bool folded; // Flag to track whether player has folded or not
  bool available_actions[NUM_ACTIONS]; // Array to store available actions
} Player;

void initializePlayer(Player *player, int initial_money);
void printPlayer(const Player *player);
void printAllPlayerMoney(Player players[], int num_players);
void showHand(const Player *player);
void showAllPlayerHand(Player players[], int num_players);

void addMoney(Player *player, int amount);
void subtractMoney(Player *player, int amount);
void setActionAvailability(Player *player, Action action, bool available);
void resetPlayer(Player *player);

// Functions for performing player actions
bool performFold(Player *player);
bool performCheck(Player *player);
bool performBet(Player *player, int amount);
bool performCall(Player *player, int amount);
bool performRaise(Player *player, int amount);
bool performAllIn(Player *player);

#endif /* PLAYER_H */
