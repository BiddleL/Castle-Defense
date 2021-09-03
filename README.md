# Castle-Defense

This as my second project for my CS degree  
Topics - **linked lists** 
Based on Castle Defense games

# Given Files
* _main.c_
* _realm.h_
* _test_realm.h_
* _capture.h_
* _capture.c_
# Content Files
* **realm.c** - Contains all the functions implimented by myself for the assignment
* **test_realm.c** - All test cases for above functions

# Stage 1
## Adding to the Realm
Implementing functions that add locations to the realm
```c
static Location new_location(char *name) {
    return NULL
}
int add_location(Realm realm, char *name) {
  return 42;
}
```
## Printing Out Realm
Implement a printing function for the whole realm
```c
void print_realm(Realm realm) {}
```
Using fully implemented functions of 
```c
void print_tower(char *name, int power, int uses, Effect effect);
void print_land(char *name);
void print_castle(char *name, int defense);
void print_enemy(char *name, int cur_hp, int max_hp);
```
# Stage 2
## Adding Enemies
Implement the below function that add a enemy to the realm
```c
int new_enemy(Realm realm, char *location_name, char *name, int hp) {}
```
## New Towers
Implement a function that adds new towers
```c
int new_tower(Realm realm, char *prev_name, char *name, int power, int uses) {}
```
# Stage 3
## Freeing Memory and Destroying Realm
Implementing a function to quit the program and destroying all allocated memory
```c
void destroy_realm(Realm realm);
```
## Advancing Enemies
Implement a function to advance enemies at the tick of a clock
```c
int advance_enemies(Realm realm) {}
```
## Damage
Implementing a function that applies damage to enemies and the castle
```c
int apply_damage(Realm realm) {}
```

  


