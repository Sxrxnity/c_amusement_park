// CS Amusement Park
// main.c
// Written by <Danny Sun>, <z5691331>
// on <03/04/2025>
//
// <Managing the CS Amusement Park with the help of linked lists>

#include <stdio.h>
#include <string.h>

#include "cs_amusement_park.h"

int main(void) {
    // Initialise the park
    char park_name[MAX_SIZE];
    strcpy(park_name, "UNSW");

    struct park *my_park = initialise_park(park_name);

    // Print park details
    printf("Park initialised:\n");
    printf("  Name: %s\n", my_park->name);
    printf("  Total Visitors: %d\n", my_park->total_visitors);
    if (my_park->rides == NULL) {
        printf("  Rides: NULL\n");
    } else {
        printf("  Rides not set to NULL\n");
    }
    if (my_park->visitors == NULL) {
        printf("  Visitors: NULL\n\n");
    } else {
        printf("  Visitors not set to NULL\n\n");
    }

    // Create a ride
    char ride_name[MAX_SIZE];
    strcpy(ride_name, "Thunderbolt");
    struct ride *roller_coaster = create_ride(ride_name, ROLLER_COASTER);

    // Print ride details
    printf("Ride created:\n");
    printf("  Name: %s\n", roller_coaster->name);
    printf("  Type: %d\n", roller_coaster->type);
    printf("  Rider Capacity: %d\n", roller_coaster->rider_capacity);
    printf("  Queue Capacity: %d\n", roller_coaster->queue_capacity);
    printf("  Minimum Height: %.2f\n", roller_coaster->min_height);
    if (roller_coaster->queue == NULL) {
        printf("  Queue: NULL\n");
    } else {
        printf("  Queue not set to NULL\n");
    }
    if (roller_coaster->next == NULL) {
        printf("  Next Ride: NULL\n\n");
    } else {
        printf("  Next Ride not set to NULL\n\n");
    }

    
    // Create a visitor
    char visitor_name[MAX_SIZE];
    strcpy(visitor_name, "Sasha");
    struct visitor *visitor = create_visitor(visitor_name, 160.5);

    // Print visitor details
    printf("Visitor created:\n");
    printf("  Name: %s\n", visitor->name);
    printf("  Height: %.2f cm\n", visitor->height);

    if (visitor->next == NULL) {
        printf("  Next Visitor: NULL\n\n");
    } else {
        printf("  Next Visitor not set to NULL\n\n");
    }

    return 0;
}

/*
int main(void) {
    // Welcome message
    printf("Welcome to CS Amusement Park!\n");
    
    // NOTE: The printing of the ascii park will fail `1511 style`. 
    // You will not be marked on this.
    printf(
        "                                           o\n"
        "                                         o |\n"
        "                                         |\n"
        "      .       .           ._._.    _                     .===.\n"
        "      |`      |`        ..'\\ /`.. |H|        .--.      .:'   `:.\n"
        "     //\\-...-/|\\         |- o -|  |H|`.     /||||\\     ||     ||\n"
        " ._.'//////,'|||`._.    '`./|\\.'` |\\\\||:. .'||||||`.   `:.   .:'\n"
        " ||||||||||||[ ]||||      /_T_\\   |:`:.--'||||||||||`--..`=:='...\n\n"
    );

    printf("Enter the name of the park: ");
    char name[MAX_SIZE];
    scan_name(name);
    struct park *park = initialise_park(name);

    // Command loop
    command_loop(park);

    // End message
    printf("\nGoodbye!\n");

    return 0;
}
*/