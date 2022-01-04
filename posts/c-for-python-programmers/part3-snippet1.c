#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int health;
  int strength;
  int agility;
  int intelligence;
  // ...
} Character;

bool is_character_dead(Character* character) {
  // Same as:
  // return (*character).health <= 0;
  return character->health <= 0;
}

void fight(Character* characterA, Character* characterB) {
  if (characterA->strength > characterB->strength) {
    characterB->health = 0;  // Kill B if less strong
  } else if (characterA->strength < characterB->strength) {
    characterA->health = 0;  // Kill A if less strong
  }
  // do nothing if same strength
}

int main() {
  Character hero = {
      .health = 30, .strength = 10, .agility = 2, .intelligence = 20};

  printf("%ld\n", sizeof(hero));

  if (is_character_dead(&hero))
    printf("Dead :(\n");
  else
    printf("Alive!\n");
}