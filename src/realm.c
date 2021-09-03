// Assignment 2 20T2 COMP1511: Castle Defense
//
// This program was written by Liam Biddle (z5311885)
// on 1-8-20
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "realm.h"
///////////////////////////////////////////////////////////////////////

// `struct realm` represents a realm, which represents the state of the
// entire program. It is mainly used to point to a linked list of 
// locations, though you may want to add other fields to it.
//
// You may need to add fields to struct realm.
struct realm {
    struct location *castle;
    struct location *lair;
};

// `struct location` represents a location, which could be a land,
// a tower, or a castle. Each of those places has a different type
// of information that this struct will need to store.
//
// You will need to add fields to struct location.
struct location {
    char name[MAX_NAME_LENGTH];
    struct enemy *enemies;
    struct location *next;
    int castleDefense;
    int type;
    int uses;
    int power;

};

// `struct enemy` represents an enemy, which will move through the
// realm (to each location). Towers can deal damage to it, and
// it can deal damage to the Castle.
//
// You will need to add fields to struct enemy.
struct enemy {
    struct enemy *next;
    int enemyHpMax;
    int enemyHpCurrent;
    char name[MAX_NAME_LENGTH];
};

// Add any other structs you define here.

///////////////////////////////////////////////////////////////////////

// Function prototypes for helper functions
static Location new_location(char *name);
static void print_tower(char *name, int power, int uses, Effect effect);
static void print_land(char *name);
static void print_castle(char *name, int defense);
static void print_enemy(char *name, int cur_hp, int max_hp);
static void freeEnemies (struct enemy *enemy);
static void deleteEnemy(struct enemy *delete, struct enemy *prev, struct location *location);

// Add prototypes for any extra functions you create here.

///////////////////////////////////////////////////////////////////////

// You need to implement the following 9 functions.
// In other words, write code to make the function work as described 
// in realm.h.

// Create a new realm, and return a pointer to it.
// You may need to change this function in later stages.
Realm new_realm(void) {
    struct realm *realm = malloc(sizeof(struct realm));
    realm->castle = new_location("Castle");
    realm->castle->castleDefense = STARTING_CASTLE_HP;
    realm->castle->type = TYPE_CASTLE;
    realm->lair = new_location("Lair");

    if (realm->castle != NULL && realm->lair != NULL) {
        realm->castle->next = realm->lair;
    }

    return realm;
}


// Return a new location created with malloc.
static Location new_location(char *name) {
    struct location *head = malloc(sizeof(struct location));
    strcpy(head->name, name);
    head->type = TYPE_LAND;
    head->enemies = NULL;
    head->next = NULL;
    head->castleDefense = 0;
    return head;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////

// Add a new location to the realm, and return the total number of
// locations in the realm.
int add_location(Realm realm, char *name) {
    struct location *current = realm->castle;
    struct location *prev = NULL;
    int count = 0;
    while (current !=NULL && strcmp(current->name,"Lair") !=0) {
        prev = current;
        current = current->next;    
        count++;
    }
    count++;
    struct location *new = new_location(name);
    count++;
    prev->next = new;
    new->next = current;
    return count; 
}

// Print out the realm.
void print_realm(Realm realm) {
    struct location *current = realm->castle;
    while (current !=NULL) {
        if (current->type == TYPE_CASTLE) {
            print_castle(current->name,current->castleDefense);
        }
        else if (current->type == TYPE_TOWER) {
            print_tower(current->name, current->power, current->uses 
            , EFFECT_NONE);
        }
        else if (current->type == TYPE_LAND) {
            print_land(current->name);
        }
        
        if (current->enemies !=NULL) {
            struct enemy *head = current->enemies;
            while (head !=NULL) {
                print_enemy(head->name,head->enemyHpCurrent,head->enemyHpMax);
                head = head->next;
            }
        }
        current = current->next;
    }
}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

// Add an enemy to the realm.
int new_enemy(Realm realm, char *location_name, char *name, int hp) {
    struct location *head = realm->castle;
    if (hp < 1) {
        return ERROR_INVALID_STAT;
    }
    while (strcmp(head->name,location_name) !=0 && head->next !=NULL) {
        head = head->next;
    }
    if (strcmp(head->name,location_name) !=0) {
        return ERROR_NO_LOCATION;
    }
    
    struct enemy *enemyHead = head->enemies;
    struct enemy *new = malloc(sizeof(struct enemy));
    if (head->enemies == NULL) {
        head->enemies = new;
    } else {
        enemyHead = head->enemies;
        while (enemyHead->next !=NULL) {
            enemyHead = enemyHead->next;
        }
        enemyHead->next = new;
    }
    strcpy(new->name,name);
    new->enemyHpMax = hp;
    new->enemyHpCurrent = hp;
    new->next = NULL;
        
    if(strcmp(new->name,name) == 0) {
        return SUCCESS;
    } else {
        return ERROR_NOT_IMPLEMENTED;
    }
}

// Add a new tower to the realm.
int new_tower(Realm realm, char *prev_name, char *name, int power, int uses) {
    struct location *head = realm->castle->next;
    struct location *prev = realm->castle;
    struct location *newTower = malloc(sizeof(struct location));
    newTower->type = TYPE_TOWER;
    newTower->power = power;
    newTower->uses = uses;
    strcpy(newTower->name,name);
    newTower->enemies = NULL;
    
    while (head !=NULL && strcmp(prev->name,prev_name) !=0) {
        prev = head;
        head = head->next;
    }
    if (strcmp(prev->name,prev_name) !=0) {
        return ERROR_NO_LOCATION;
    }
    if (power < 1 || uses < 1) {
        return ERROR_INVALID_STAT;
    }
    if (prev == NULL) {
        newTower->next = head->next;
        head->next = newTower;
    }
    if (head !=NULL && prev !=NULL) {
        newTower->next = prev->next;
        prev->next = newTower;
    }
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

// Destroy the realm, and free any associated memory.
void destroy_realm(Realm realm) {
    struct location *head = realm->castle;
    struct location *prev = NULL;
    while (head !=NULL) {
        if (head->enemies !=NULL) {
                freeEnemies(head->enemies);
            }
        prev = head;
        head = head->next;
        free(prev);
        }
    free(realm);
    }


// Advance enemies towards the castle.
int advance_enemies(Realm realm) {
    struct location *prev = realm->castle;
    struct location *head = prev->next;
    int count = 0;
    if (prev->enemies !=NULL) {
        struct enemy *eCount = prev->enemies;
        while (eCount !=NULL) {
            count++;
            eCount = eCount->next;
        }
        freeEnemies(prev->enemies);
    }
    while (head !=NULL) {
        prev->enemies = head->enemies;
        head->enemies = NULL;
        prev = prev->next;
        head = head->next;
    }
    return count;
}

// Apply damage to the enemies at each tower, and reduce the castle's
// defense.
int apply_damage(Realm realm) {
    struct location *head = realm->castle;
    int count = 0;
    while (head !=NULL) {
        // if the land type is a TOWER and enemies do exist
        if (head->enemies !=NULL && head->type == TYPE_TOWER) {
            struct enemy *eHead = head->enemies;
            struct enemy *ePrev = NULL;
            // looping for enemies
            while (eHead !=NULL && head->uses > 0) {
                eHead->enemyHpCurrent = eHead->enemyHpCurrent - head->power;
                head->uses--;
                count++;
                if (eHead->enemyHpCurrent <= 0) {
                    if (head->enemies == eHead && ePrev == NULL) {
                        head->enemies = eHead->next;
                    } else {
                        ePrev->next = eHead->next;
                    }
                    struct enemy *delete = eHead;
                    eHead = eHead->next;
                    free(delete);
                }
                if (head->uses == 0) {
                    head->type = TYPE_LAND;
                }
                if (eHead !=NULL) {
                ePrev = eHead;
                eHead = eHead->next;
                }
            }
        } 
        // if the type is a CASTLE, then applying the HP damage to the castle
        else if (head->type == TYPE_CASTLE) {
            int hpCount = 0;
            struct enemy *eHead = head->enemies;
            while (eHead !=NULL) {
                hpCount = hpCount + eHead->enemyHpCurrent;
                eHead = eHead->next;
            }
            head->castleDefense = head->castleDefense - hpCount;
        }
        head = head->next;
    }
    return count;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////

// Apply the specified buff to the relevant towers or enemies.
int apply_buff(Realm realm, char *search_term, Buff buff, int amount){
    return 0;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

// Apply the specified effect to each tower whose name matches the
// specified search term.
int apply_effect(Realm realm, char *search_term, Effect effect) {
    return 0;
}

////////////////////////////////////////////////////////////////////////

// Add definitions for your own functions below.
// Make them static to limit their scope to this file.

// This function frees the memory for each enemies part of the location 
static void freeEnemies (struct enemy *enemy) {
    struct enemy *head = enemy;
    struct enemy *prev = NULL;
    while (head !=NULL) {
        prev = head;
        head = head->next;
        free(prev);
    }
}
static void deleteEnemy(struct enemy *delete, struct enemy *prev, struct location *location) {
        if (location->enemies == delete && prev == NULL) {
            location->enemies = delete->next;
        } else {
            prev->next = delete->next;
        }
        free(delete);
}



////////////////////////////////////////////////////////////////////////
//                    Provided print functions                        //
//         NOTE: YOU SHOULD NOT MODIFY THE FOLLOWING FUNCTIONS        //
////////////////////////////////////////////////////////////////////////

void print_tower(char *name, int power, int uses, int effect) {
    printf(" ^ %32s [pow: %3d | uses: %3d]", name, power, uses);
    if (effect == EFFECT_NONE) printf(" {%c}", EFFECT_NONE_CHAR);
    if (effect == EFFECT_PORTAL) printf(" {%c}", EFFECT_PORTAL_CHAR);
    if (effect == EFFECT_ICE) printf(" {%c}", EFFECT_ICE_CHAR);
    printf("\n");
}

void print_land(char *name) {
    printf(" ^ %32s [____________________]\n", name);
}

void print_castle(char *name, int defense) {
    printf(" ^ %32s [Castle Defenses: %3d]\n", name, defense);
}

void print_enemy(char *name, int cur_hp, int max_hp) {
    printf(" ^ %40s [hp: %d/%d]\n", name, cur_hp, max_hp);
}

////////////////////////////////////////////////////////////////////////
//               End of provided print functions                      //
////////////////////////////////////////////////////////////////////////




