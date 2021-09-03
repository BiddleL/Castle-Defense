// Assignment 2 20T2 COMP1511: Castle Defense
//
// This program was written by Liam Biddle (z5311885)
// on 1-8-20
//
// Version 1.0.0 (2020-07-20): Assignment released.
// Version 1.0.1 (2020-07-21): Add return value to main.
// Version 1.1.0 (2020-07-23): Add test_apply_buff to main.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "realm.h"
#include "test_realm.h"
#include "capture.h"

////////////////////////////////////////////////////////////////////
//                        Function Protoypes                      //
////////////////////////////////////////////////////////////////////

// TODO: Add your own prototypes here, if necessary.


////////////////////////////////////////////////////////////////////
//                         Helper Functions                       //
////////////////////////////////////////////////////////////////////

// Find the string 'needle' in 'haystack'
int string_contains(char *haystack, char *needle) {
    return strstr(haystack, needle) != NULL;
}

// NOTE: You should not declare any structs from realm.c here.
// Declaring structs from realm.c in this function is against the
// rules of the assignment. 

// Main function: this function will not be used in testing, you do not
// need to change it.
// If you want to write your own tests, add them in `extra_tests`.
int main(int argc, char *argv[]) {
    printf("\n================== Castle Defense Tests ==================\n");

    test_add_location();
    test_print_realm();
    test_new_enemy();
    test_new_tower();
    test_apply_damage();
    test_apply_buff();
    extra_tests();
    printf("\n");

    return 0;
}

// NOTE: These tests are explained in `test_realm.h`

void test_add_location(void) {
    printf("Test whether this add_location follows the spec: ");
    int pass = 0;
    // Test 1: Does add_location's return value count the Castle & Lair?

    Realm test_realm = new_realm();
    int num_locations = add_location(test_realm, "Location");
    if (num_locations != 3) {
        pass++;
        // since we don't want to print FOLLOWS_SPEC at the end,
        // we just return to the main function here. 
    }
    // Test 2: Does add_location return the correct amount for lots of locations?
    add_location(test_realm,"Marc");
    add_location(test_realm,"Liam");
    add_location(test_realm,"Scout");
    add_location(test_realm,"Dog");
    add_location(test_realm,"Josh");
    num_locations = add_location(test_realm,"COMP1511");
    if (num_locations != 9) {
        pass++;
    }
    // Test 3: Add your own test, and explain it below:
    // testing for possible error in the name inputed,
    Realm test_two = new_realm();
    int count = 0;
    add_location(test_two,"Marc");
    add_location(test_two," "); 
    add_location(test_two,"\0");
    count = add_location(test_two,"\n");
 
    if (count != 6) {
        pass++;
    }
    
    if (pass == 0) {
        printf(FOLLOWS_SPEC);
    } else {
        printf(DOES_NOT_FOLLOW_SPEC);
    } 
}

void test_print_realm(void) {
    printf("Test whether this print_realm follows the spec: ");
    int pass = 0;
    // Test 1: Does print_realm show the Castle?

    Realm test_realm = new_realm();

    char print_text[10000];
    CAPTURE(print_realm(test_realm), print_text, 10000);

    if (!string_contains(print_text, "Castle")) {
        pass++;
        // since we don't want to print FOLLOWS_SPEC at the end,
        // we just return to the main function here.
    }

    // Test 2: Does print_realm show the correct HP of the castle?

    if (!string_contains(print_text, "Castle Defenses: 100")) {
        pass++;
        // since we don't want to print FOLLOWS_SPEC at the end,
        // we just return to the main function here.
    }
    

    // Test 3: Add your own test, and explain it below: 
    // checking if towers and enemies are printed
    Realm test_realm_2 = new_realm();
    char print_text_2[10000];
    
    add_location(test_realm_2,"Marc");
    add_location(test_realm_2,"Liam");
    add_location(test_realm_2,"Scout");
    add_location(test_realm_2,"Dog");
    add_location(test_realm_2,"Josh");
    new_enemy(test_realm_2,"Dog","Enemy",10);
    new_tower(test_realm_2,"Liam","Tower", 5, 5);
    CAPTURE(print_realm(test_realm_2), print_text_2, 10000);
    if (!string_contains(print_text_2, "Tower" ) || 
        !string_contains(print_text_2, "Enemy")) {
        pass++;
        // since we don't want to print FOLLOWS_SPEC at the end,
        // we just return to the main function here.
    }
    if (pass == 0) {
        printf(FOLLOWS_SPEC);
    } else {
        printf(DOES_NOT_FOLLOW_SPEC);
    } 
}

// Stage 2

void test_new_enemy(void) {
    printf("Test whether this new_enemy follows the spec: ");
    int pass = 0;
    // Test 1: Does new_enemy work if you're adding to the Lair?
    Realm test_realm = new_realm();
    add_location(test_realm,"Marc");
    add_location(test_realm,"Liam");
    add_location(test_realm,"Scout");
    add_location(test_realm,"Dog");
    add_location(test_realm,"Josh");
    int number = new_enemy(test_realm,"Lair", "Enemy",5);
    if (number !=0) {
        pass++;
    }
    // Test 2: Add your own test, and explain it below:
    // testing to see if multiple enemies can be added
    // to the same location
    
    new_enemy(test_realm,"Scout", "Enemy_1",5);
    new_enemy(test_realm,"Scout", "Enemy_2",5);
    int test = new_enemy(test_realm,"Scout", "Enemy_3",5);
    if (test !=0) {
        pass++;
    }

    if (pass == 0) {
        printf(FOLLOWS_SPEC);
    } else {
        printf(DOES_NOT_FOLLOW_SPEC);
    }
}

void test_new_tower(void) {
    printf("Test whether this new_tower follows the spec: ");
    int pass = 0;
    // Test 1: Does new_tower work if you're adding after another tower?

    Realm test_realm = new_realm();
    new_tower(test_realm,"Castle","Tower 1",5, 5);
    int test_1 = new_tower(test_realm,"Castle","Tower 1", 5, 5);
    if (test_1 !=0) {
        pass++;
    }
    
    // Test 2: Add your own test, and explain it below:
    // testing if a tower can be added after lair
    int test_2 = new_tower(test_realm,"Lair","After Lair", 5, 5);
    if (test_2 !=0) {
        pass++;
    }

    if (pass == 0) {
        printf(FOLLOWS_SPEC);
    } else {
        printf(DOES_NOT_FOLLOW_SPEC);
    }
}

// Stage 3 (2 marks)

void test_apply_damage(void) {
    printf("Test whether this apply_damage follows the spec: ");
    int pass = 0;
    // Test 1: Does apply_damage actually destroy enemies?

    Realm test_realm = new_realm();
    add_location(test_realm,"Marc");
    add_location(test_realm,"Liam");
    add_location(test_realm,"Scout");
    add_location(test_realm,"Dog");
    add_location(test_realm,"Josh");
    new_tower(test_realm,"Scout","Tower", 5, 5);
    new_enemy(test_realm,"Tower", "Enemy_1",1);
    int damage = apply_damage(test_realm);
    if (damage !=1) {
        pass++;
    }
    // Test 2: Add your own test, and explain it below:
    // does destorying enemies work with advancing enemies?

    Realm test_realm_2 = new_realm();
    add_location(test_realm_2,"Marc");
    add_location(test_realm_2,"Liam");
    add_location(test_realm_2,"Scout");
    add_location(test_realm_2,"Dog");
    add_location(test_realm_2,"Josh");
    new_tower(test_realm_2,"Marc","Tower", 5, 5);
    new_enemy(test_realm_2,"Tower", "Enemy_1",1);
    new_enemy(test_realm_2,"Liam", "Enemy_2",1);
    int damage_1 = apply_damage(test_realm_2 );
    advance_enemies(test_realm_2);
    int damage_2 = apply_damage(test_realm_2);
    if (damage_1 + damage_2 !=2) {
        pass++;
    }
    if (pass == 0) {
        printf(FOLLOWS_SPEC);
    } else {
        printf(DOES_NOT_FOLLOW_SPEC);
    }
}


// Stage 4 (1 marks)

void test_apply_buff(void) {
    printf("Test whether this apply_buff follows the spec: ");
    // Test 1: Add your own test, and explain it below:
    // ........

    // TODO: Change Here

    // Test 2: Add your own test, and explain it below:
    // ........

    // TODO: Change Here

    printf(FOLLOWS_SPEC);
}

// Extra Tests (not worth marks, but fun!)

void extra_tests(void) {
    // TODO: add extra tests, if you'd like to.
}
