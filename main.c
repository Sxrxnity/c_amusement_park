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
