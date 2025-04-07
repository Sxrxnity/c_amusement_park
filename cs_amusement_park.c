// CS Amusement Park
// cs_amusement_park.c
// Written by <Danny Sun>, <z5691331>
// on <03/04/2025>

////////////////////////////////////////////////////////////////////////////////
// Provided Libraries
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>

#include "cs_amusement_park.h"

////////////////////////////////////////////////////////////////////////////////
// Your libraries
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// Function Definitions
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// BEGIN STAGE 1
////////////////////////////////////////////////////////////////////////////////

// Stage 1.1
// Function to initialise the park
// Params:
//      name - the name of the park
// Returns: a pointer to the park
struct park *initialise_park(char name[MAX_SIZE]) {
    struct park *new_park = malloc(sizeof(struct park));
    strcpy(new_park->name, name);
    new_park->total_visitors = 0;
    new_park->rides = NULL;
    new_park->visitors = NULL;
    return new_park;
}

// Stage 1.1
// Function to create a visitor
// Params:
//      name - the name of the visitor
//      height - the height of the visitor
// Returns: a pointer to the visitor
struct visitor *create_visitor(char name[MAX_SIZE], double height) {
    struct visitor *new_visitor = malloc(sizeof(struct visitor));
    strcpy(new_visitor->name, name);
    new_visitor->height = height;
    new_visitor->next = NULL;
    return new_visitor;
}

// Stage 1.1
// Function to create a ride
// Params:
//      name - the name of the ride
//      type - the type of the ride
// Returns: a pointer to the ride
struct ride *create_ride(char name[MAX_SIZE], enum ride_type type) {
    struct ride *new_ride = malloc(sizeof(struct ride));
    strcpy(new_ride->name, name);
    new_ride->type = type;
    if (type == ROLLER_COASTER) {
        new_ride->rider_capacity = 4;
        new_ride->queue_capacity = 7;
        new_ride->min_height = 120.0;
    } else if (type == CAROUSEL) {
        new_ride->rider_capacity = 6;
        new_ride->queue_capacity = 9;
        new_ride->min_height = 60.0;
    } else if (type == FERRIS_WHEEL) {
        new_ride->rider_capacity = 8;
        new_ride->queue_capacity = 11;
        new_ride->min_height = 75.0;
    } else if (type == BUMPER_CARS) {
        new_ride->rider_capacity = 10;
        new_ride->queue_capacity = 13;
        new_ride->min_height = 100.0;
    }
    new_ride->queue = NULL;
    new_ride->next = NULL;
    return new_ride;
}

// Stage 1.2
// Function to run the command loop
//
// Params:
//     park - a pointer to the park
// Returns: None
void command_loop(struct park *park) {
    char command;
    printf("Enter command: ");
    while (scanf(" %c", &command) == 1) {
        if (command == HELP) {
            print_usage();
        } else if (command == APPEND) {
            add_entity(park);
        } else if (command == INSERT) {
            insert_ride(park);
        } else if (command == ADD_V_TO_R) {
            add_visitor_to_ride(park);
        } else if (command == REMOVE_V_FROM_R) {
            remove_visitor_from_ride(park);
        } else if (command == PRINT) {
            print_park(park);
        }
        printf("Enter command: ");
    }
}

// Adds either a ride or visitor to the park
void add_entity(struct park *park) {
    char second_command;
    scanf(" %c", &second_command);
    if (second_command == RIDE) {
        append_ride(park);
    } else if (second_command == VISITOR) {
        append_visitor(park);
    }
}

// Stage 1.3
// Function to add a ride to the park
// Params:
//      park - a pointer to the park
// Returns: None
void append_ride(struct park *park) {
    char name[MAX_SIZE];
    scan_name(name);
    enum ride_type type = scan_type();

    if (is_type_invalid(type) == TRUE) {
        return;
    }

    if (park->rides == NULL) {
        park->rides = create_ride(name, type);
    } else {
        if (is_existing_ride(park->rides, name) == TRUE) {
            printf("ERROR: '%s' already exists.\n", name);
            return;
        }
        struct ride *current = park->rides;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = create_ride(name, type);
    }
    printf("Ride: '%s' added!\n", name);

}

// Stage 1.3
// Function to add a visitor to the park
// Params:
//      park - a pointer to the park
// Returns: None
void append_visitor(struct park *park) {
    char name[MAX_SIZE];
    scan_name(name);
    double height;
    scanf(" %lf", &height);

    if (visitor_height_valid(height) == FALSE ||
        park_is_full(park->total_visitors) == TRUE) {
        return;
    }

    if (park->visitors == NULL) {
        park->visitors = create_visitor(name, height);
    } else {
        if (is_existing_visitor(park->visitors, name) == TRUE) {
            printf("ERROR: '%s' already exists.\n", name);
            return;
        }
        struct visitor *current = park->visitors;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = create_visitor(name, height);
    }
    printf("Visitor: '%s' has entered the amusement park!\n", name);
    park->total_visitors++;
}

// Removes the bulky code for printing rides from the print_park function
void print_all_rides(struct ride *first_ride) {
    struct ride *current_ride = first_ride;
    printf("Rides:\n");
    while (current_ride != NULL) {
        print_ride(current_ride);
        current_ride = current_ride->next;
    }
}

// Removes the bulky code for printing visitors from the print_park function
void print_all_visitors(struct visitor *first_visitor) {
    struct visitor *current_visitor = first_visitor;
    printf("Visitors:\n");
    while (current_visitor != NULL) {
        print_visitor(current_visitor);
        current_visitor = current_visitor->next;
    }
}

// Stage 1.4
// Function to print the park
// Params:
//      park - a pointer to the park
// Returns: None
void print_park(struct park *park) {
    print_welcome_message(park->name);
    if (park->rides != NULL && park->visitors != NULL) {
        print_all_rides(park->rides);
        print_all_visitors(park->visitors);
    } else if (park->rides == NULL && park->visitors == NULL) {
        printf("The amusement park is empty!\n");
    } else {
        if (park->rides == NULL) {
            printf("Rides:\n  No rides!\n");
            print_all_visitors(park->visitors);
        } else if (park->visitors == NULL) {
            print_all_rides(park->rides);
            printf("Visitors:\n  No visitors!\n");
        }
    }
    printf("\n");
}

////////////////////////////////////////////////////////////////////////////////
// END STAGE 1
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// BEGIN STAGE 2
////////////////////////////////////////////////////////////////////////////////

// Inserts a ride at a specific position in the park's ride list
void insert_ride(struct park *park) {
    int index;
    scanf(" %d", &index);
    char name[MAX_SIZE];
    scan_name(name);
    enum ride_type type = scan_type();

    if (is_valid_index(index) == FALSE ||
        is_type_invalid(type) == TRUE) {
        return;
    } else if (is_existing_ride(park->rides, name) == TRUE) {
        printf("ERROR: a ride with name: ");
        printf("'%s' already exists in this park.\n", name);
        return;
    } else {
        struct ride *new_ride = create_ride(name, type);
        if (park->rides == NULL) {
            park->rides = new_ride;
        } else if (index == 1) {
            new_ride->next = park->rides;
            park->rides = new_ride;
        } else {
            struct ride *current = park->rides;
            int i = 1;
            while (current->next != NULL && i < index - 1) {
                current = current->next;
                i++;
            }
            new_ride->next = current->next;
            current->next = new_ride;
        }
        printf("Ride: '%s' inserted!\n", name);
    }
}

// Adds a visitor to the queue of a specific ride
void add_visitor_to_ride(struct park *park) {
    char ride_name[MAX_SIZE];
    char visitor_name[MAX_SIZE];
    scan_name(ride_name);
    scan_name(visitor_name);

    struct ride *ride = retrieve_ride(park->rides, ride_name);
    struct visitor *visitor = retrieve_visitor(park->visitors, visitor_name);

    if (valid_ride_and_visitor(ride, visitor,
        ride_name, visitor_name) == TRUE) {
        remove_visitor_from_roaming(park, visitor_name);
        struct visitor *current = ride->queue;
        if (current == NULL) {
            ride->queue = visitor;
        } else {
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = visitor;
        }
        printf("Visitor: '%s' has entered the queue for '%s'.\n",
            visitor_name, ride_name);
    }
}

// Removes a visitor from the queue of a specific ride and
// puts them back in roaming
void remove_visitor_from_ride(struct park *park) {
    char visitor_name[MAX_SIZE];
    scan_name(visitor_name);

    struct ride *ride = find_ride_containing(park, visitor_name);
    if (ride == NULL) {
        printf("ERROR: Visitor '%s' not found in any queue.\n", visitor_name);
        return;
    }

    struct visitor *visitor = retrieve_visitor(ride->queue, visitor_name);
    int list_length = calculate_list_length(ride->queue);
    if (visitor == NULL) {
        printf("ERROR: Visitor '%s' not found in any queue.\n", visitor_name);
        return;
    }

    struct visitor *previous = NULL;
    struct visitor *current = ride->queue;
    if (list_length == 1) {
        ride->queue = NULL;
        reappend_visitor(park, visitor);
    } else if (strcmp(current->name, visitor_name) == 0) {
        ride->queue = current->next;
        current->next = NULL;
        reappend_visitor(park, visitor);
    } else {
        while (current != NULL) {
            if (strcmp(current->name, visitor_name) == 0) {
                previous->next = current->next;
                current->next = NULL;
                reappend_visitor(park, visitor);
                break;
            }
            previous = current;
            current = current->next;
        }
    }
}

struct ride *find_ride_containing(struct park *park,
    char visitor_name[MAX_SIZE]) {
    struct ride *current_ride = park->rides;
    while (current_ride != NULL) {
        struct visitor *current_visitor = current_ride->queue;
        while (current_visitor != NULL) {
            if (strcmp(current_visitor->name, visitor_name) == 0) {
                return current_ride;
            }
            current_visitor = current_visitor->next;
        }
        current_ride = current_ride->next;
    }
    return NULL;
}

// Retrieves the correct ride to add a visitor to
struct ride *retrieve_ride(struct ride *ride, char name[MAX_SIZE]) {
    struct ride *current = ride;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Retrieves the correct visitor to add to a ride queue
// and removes them from roaming
struct visitor *retrieve_visitor(struct visitor *visitor, char name[MAX_SIZE]) {
    struct visitor *current = visitor;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Checks if the ride and visitor are valid
int valid_ride_and_visitor(struct ride *ride, struct visitor *visitor,
    char ride_name[MAX_SIZE], char visitor_name[MAX_SIZE]) {

    if (ride == NULL) {
        printf("ERROR: No ride exists with name '%s'.\n", ride_name);
        return FALSE;
    } else if (visitor == NULL) {
        printf("ERROR: No visitor exists with name '%s'.\n", visitor_name);
        return FALSE;
    } else if (visitor->height < ride->min_height) {
        printf("ERROR: '%s' is not tall enough to ride '%s'.\n",
            visitor_name, ride_name);
        return FALSE;
    }
    int list_length = calculate_list_length(ride->queue);
    if (list_length > ride->queue_capacity) {
        printf("ERROR: The queue for '%s' is full. ", ride_name);
        printf("'%s' cannot join the queue.\n", visitor_name);
        return FALSE;
    }
    return TRUE;
}

// Removes a visitor from roaming the park
void remove_visitor_from_roaming(struct park *park, char name[MAX_SIZE]) {
    struct visitor *previous = NULL;
    struct visitor *current = park->visitors;
    int list_length = calculate_list_length(park->visitors);

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (list_length == 1) {
                park->visitors = NULL;
            } else if (previous == NULL && list_length > 1) {
                park->visitors = current->next;
            } else {
                previous->next = current->next;
            }
            current->next = NULL;
            break;
        }
        previous = current;
        current = current->next;
    }
}

// Reappends a visitor to the park after they have been removed from a ride
void reappend_visitor(struct park *park, struct visitor *visitor) {
    if (park->visitors == NULL) {
        park->visitors = visitor;
    } else {
        struct visitor *current = park->visitors;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = visitor;
    }
    printf("Visitor: '%s' has been removed ", visitor->name);
    printf("from their ride queue and is now roaming the park.\n");
}

////////////////////////////////////////////////////////////////////////////////
// END STAGE 2
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// BEGIN STAGE 3
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// END STAGE 3
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// BEGIN STAGE 4
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// END STAGE 4
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// BEGIN STAGE 5
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// END STAGE 5
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// BEGIN HELPERS
////////////////////////////////////////////////////////////////////////////////

// Checks a given ride name against the list of rides in the park
int is_existing_ride(struct ride *first_ride, char name[MAX_SIZE]) {
    struct ride *current = first_ride;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}

// Checks a given visitor name against the list of visitors in the park
int is_existing_visitor(struct visitor *first_visitor, char name[MAX_SIZE]) {
    struct visitor *current = first_visitor;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}

// Checks if the given ride type is invalid
int is_type_invalid(enum ride_type type) {
    if (type == INVALID) {
        printf("ERROR: Invalid ride type.\n");
        return TRUE;
    } else {
        return FALSE;
    }
}

// Checks if the given visitor height is valid
int visitor_height_valid(double height) {
    if (height < MIN_VISITOR_HEIGHT || height > MAX_VISITOR_HEIGHT) {
        printf("ERROR: Height must be between 50 and 250.\n");
        return FALSE;
    } else {
        return TRUE;
    }
}

// Returns a flag indicating if the park is full
int park_is_full(int total_visitors) {
    if (total_visitors == MAX_VISITORS) {
        printf("ERROR: Cannot add another visitor to the park. ");
        printf("The park is at capacity.\n");
        return TRUE;
    } else {
        return FALSE;
    }
}

// Checks if the given insertion index is valid
int is_valid_index(int index) {
    if (index < 1) {
        printf("ERROR: n must be at least 1.\n");
        return FALSE;
    } else {
        return TRUE;
    }
}

// Calculates the length of a queue
int calculate_list_length(struct visitor *visitor) {
    int length = 0;
    struct visitor *current = visitor;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}


////////////////////////////////////////////////////////////////////////////////
// END HELPERS
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Providing function definitions
////////////////////////////////////////////////////////////////////////////////

// Function to print the usage of the program
// '?' command
// Params: None
// Returns: None
// Usage:
// ```
//      print_usage();
// ```
void print_usage(void) {
    printf(
        "======================[ CS Amusement Park ]======================\n"
        "      ===============[     Usage Info     ]===============       \n"
        "  a r [ride_name] [ride_type]                                    \n"
        "    Add a ride to the park.                                      \n"
        "  a v [visitor_name] [visitor_height]                            \n"
        "    Add a visitor to the park.                                   \n"
        "  i [index] [ride_name] [ride_type]                              \n"
        "    Insert a ride at a specific position in the park's ride list.\n"
        "  j [ride_name] [visitor_name]                                   \n"
        "    Add a visitor to the queue of a specific ride.               \n"
        "  m [visitor_name] [ride_name]                                   \n"
        "    Move a visitor from roaming to a ride's queue.               \n"
        "  d [visitor_name]                                               \n"
        "    Remove a visitor from any ride queue and return to roaming.  \n"
        "  p                                                              \n"
        "    Print the current state of the park, including rides and     \n"
        "    visitors.                                                    \n"
        "  t                                                              \n"
        "    Display the total number of visitors in the park, including  \n"
        "    those roaming and in ride queues.                            \n"
        "  c [start_ride] [end_ride]                                      \n"
        "    Count and display the number of visitors in queues between   \n"
        "    the specified start and end rides, inclusive.                \n"
        "  l [visitor_name]                                               \n"
        "    Remove a visitor entirely from the park.                     \n"
        "  r                                                              \n"
        "    Operate all rides, allowing visitors to enjoy the rides      \n"
        "    and moving them to roaming after their ride.                 \n"
        "  M [ride_type]                                                  \n"
        "    Merge the two smallest rides of the specified type.          \n"
        "  s [n] [ride_name]                                              \n"
        "    Split an existing ride into `n` smaller rides.               \n"
        "  q                                                              \n"
        "    Quit the program and free all allocated resources.           \n"
        "  T [n] [command]                                                \n"
        "    Scheduled the [command] to take place `n` ticks              \n"
        "    in the future.                                               \n"
        "  ~ [n]                                                          \n"
        "    Progress the schedule for `n` ticks.                         \n"
        "  ?                                                              \n"
        "    Show this help information.                                  \n"
        "=================================================================\n");
}

// Function to print a welcome message
// Params:
//      name - the name of the park
// Returns: None
// Usage:
// ```
//      print_welcome_message(name);
// ```
void print_welcome_message(char name[MAX_SIZE]) {
    printf("===================[ %s ]===================\n", name);
}

// Function to print a ride
// Params:
//      ride - the ride to print
// Returns: None
// Usage:
// ```
//      print_ride(ride);
// ```
void print_ride(struct ride *ride) {
    printf("  %s (%s)\n", ride->name, type_to_string(ride->type));
    printf("    Rider Capacity: %d\n", ride->rider_capacity);
    printf("    Queue Capacity: %d\n", ride->queue_capacity);
    printf("    Minimum Height: %.2lfcm\n", ride->min_height);
    printf("    Queue:\n");
    struct visitor *curr_visitor = ride->queue;
    if (curr_visitor == NULL) {
        printf("      No visitors\n");
    } else {
        while (curr_visitor != NULL) {
            printf(
                "      %s (%.2lfcm)\n", curr_visitor->name,
                curr_visitor->height);
            curr_visitor = curr_visitor->next;
        }
    }
}

void print_visitor(struct visitor *visitor) {
    printf("  %s (%.2lfcm)\n", visitor->name, visitor->height);
}

// Scan in the a name string into the provided buffer, placing
// '\0' at the end.
//
// Params:
//      name - a char array of length MAX_SIZE, which will be used
//                  to store the name.
// Returns: None
// Usage:
// ```
//      char name[MAX_SIZE];
//      scan_name(name);
// ```
void scan_name(char name[MAX_SIZE]) {
    scan_token(name, MAX_SIZE);
}

// Scans a string and converts it to a ride_type.
//
// Params: None
// Returns:
//      The corresponding ride_type, if the string was valid,
//      Otherwise, returns INVALID.
//
// Usage:
// ```
//      enum ride_type type = scan_type();
// ```
//
enum ride_type scan_type(void) {
    char type[MAX_SIZE];
    scan_token(type, MAX_SIZE);
    return string_to_type(type);
}

////////////////////////////////////////////////////////////////////////////////
// Additional provided functions
////////////////////////////////////////////////////////////////////////////////

// You don't need to use any of these, or understand how they work!
// We use them to implement some of the provided helper functions.

enum ride_type string_to_type(char *type_str) {
    int len = strlen(type_str);

    if (strncasecmp(type_str, "roller_coaster", len) == 0) {
        return ROLLER_COASTER;
    }
    if (strncasecmp(type_str, "CAROUSEL", len) == 0) {
        return CAROUSEL;
    }
    if (strncasecmp(type_str, "FERRIS_WHEEL", len) == 0) {
        return FERRIS_WHEEL;
    }
    if (strncasecmp(type_str, "BUMPER_CARS", len) == 0) {
        return BUMPER_CARS;
    }

    return INVALID;
}

char *type_to_string(enum ride_type type) {
    if (type == ROLLER_COASTER) {
        return "ROLLER_COASTER";
    }
    if (type == CAROUSEL) {
        return "CAROUSEL";
    }
    if (type == FERRIS_WHEEL) {
        return "FERRIS_WHEEL";
    }
    if (type == BUMPER_CARS) {
        return "BUMPER_CARS";
    }
    return "INVALID";
}

int scan_token(char *buffer, int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    scanf(" ");

    while (i < buffer_size - 1 && (num_scanned = scanf("%c", &c)) == 1 &&
           !isspace(c)) {
        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}
