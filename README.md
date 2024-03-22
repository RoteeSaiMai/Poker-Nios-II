# Poker-Nios-II
A project that implements the game of Texas hold'em no-limit poker using the NIOS II processor in the DE-1-SOC FPGA board. The game will be one player against a AI player, using mouse and keyboard as the controls.


Input/Output:
- PS2 Mouse
- PS2 Keyboard
- VGA Display

Task delegations:
- Game implementation: Cartoon
  -  Base struct: Card, Deck, Community, Hand and Player
- Game Logic (how the game plays out): Cartoon
  -  Player actions
  -  Money system
  -  Hand evaluating system
- I\O: Alex
  - Mouse input
  - Keyboard input
  - VGA display
- Enemy AI: Alex
