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
struct park *initialise_park(char name[MAX_NAME_SIZE]) {

    struct park *park = malloc(sizeof(struct park));
    strcpy(park->name, name);
    park->total_visitors = 0;
    park->rides = NULL;
    park->visitors = NULL;
    park->commands = NULL;
    return park;
}

// Stage 1.1
// Function to create a visitor
// Params:
//      name - the name of the visitor
//      height - the height of the visitor
// Returns: a pointer to the visitor
struct visitor *create_visitor(char name[MAX_NAME_SIZE], double height) {

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
struct ride *create_ride(char name[MAX_NAME_SIZE], enum ride_type type) {

    struct ride *new_ride = malloc(sizeof(struct ride));
    strcpy(new_ride->name, name);
    new_ride->type = type;
    if (type == ROLLER_COASTER) {
        new_ride->rider_capacity = RC_RIDER_CAPACITY;
        new_ride->queue_capacity = RC_QUEUE_CAPACITY;
        new_ride->min_height = RC_MIN_HEIGHT;
    } else if (type == CAROUSEL) {
        new_ride->rider_capacity = CAROUSEL_RIDER_CAPACITY;
        new_ride->queue_capacity = CAROUSEL_QUEUE_CAPACITY;
        new_ride->min_height = CAROUSEL_MIN_HEIGHT;
    } else if (type == FERRIS_WHEEL) {
        new_ride->rider_capacity = FW_RIDER_CAPACITY;
        new_ride->queue_capacity = FW_QUEUE_CAPACITY;
        new_ride->min_height = FW_MIN_HEIGHT;
    } else if (type == BUMPER_CARS) {
        new_ride->rider_capacity = BC_RIDER_CAPACITY;
        new_ride->queue_capacity = BC_QUEUE_CAPACITY;
        new_ride->min_height = BC_MIN_HEIGHT;
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
    char line[MAX_COMMAND_SIZE];
    char command;
    char args[MAX_COMMAND_SIZE];

    printf("Enter command: ");
    while (fgets(line, MAX_COMMAND_SIZE, stdin) != NULL) {

        // Clear out previous data
        command = '\0';
        args[0] = '\0';

        int parsed = sscanf(line, " %c %[^\n]", &command, args);
        if (parsed >= 1) {
            if (command == SCHEDULE_COMMAND) {
                schedule_command(park, args);
            } else if (command == ADVANCE_TICKS) {
                move_schedule_forward(park, args);
            } else {
                tick_forward(park, 1);
                execute_ticking_instruction(park, command, args);
            }
        }
        printf("Enter command: ");
    }
    end_of_day_procedure(park);
}

// Executes an instruction that moves the tick forward
void execute_ticking_instruction(struct park *park,
    char command, char *args) {

    if (command == HELP) {
        print_usage();
    } else if (command == APPEND) {
        add_entity(park, args);
    } else if (command == PRINT) {
        print_park(park);
    } else if (command == INSERT) {
        insert_ride(park, args);
    } else if (command == ADD_V_TO_R) {
        add_visitor_to_ride(park, args);
    } else if (command == REMOVE_V_FROM_R) {
        remove_visitor_from_ride(park, args);
    } else if (command == MOVE_V_TO_R) {
        move_visitor_to_different_ride(park, args);
    } else if (command == COUNT_TOTAL_VISITORS) {
        count_total_visitors(park);
    } else if (command == COUNT_QUEUE_VISITORS) {
        count_queue_visitors(park, args);
    } else if (command == QUIT) {
        end_of_day_procedure(park);
    } else if (command == VISITOR_LEAVE) {
        free_one_visitor(park, args);
    } else if (command == OPERATE_RIDES) {
        operate_all_rides(park, park->rides);
    } else if (command == SHUT_DOWN_RIDE) {
        shut_down_ride(park, args);
    } else if (command == MERGE) {
        merge_rides(park, args);
    } else if (command == SPLIT) {
        split_ride(park, args);
    }
}

// Adds either a ride or visitor to the park
void add_entity(struct park *park, char *args) {

    char second_command;
    sscanf(args, " %c", &second_command);

    if (second_command == RIDE) {
        append_ride(park, args + 2);
    } else if (second_command == VISITOR) {
        append_visitor(park, args + 2);
    }
}

// Stage 1.3
// Function to add a ride to the park
// Params:
//      park - a pointer to the park
// Returns: None
void append_ride(struct park *park, char *args) {

    char name[MAX_NAME_SIZE];
    char type_str[MAX_NAME_SIZE];

    sscanf(args, " %s %s", name, type_str);
    enum ride_type type = string_to_type(type_str);

    if (is_type_invalid(type) == TRUE) {
        return;
    }

    if (park->rides == NULL) {
        park->rides = create_ride(name, type);
    } else {
        if (ride_exists(park->rides, name) == TRUE) {
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
void append_visitor(struct park *park, char *args) {

    char name[MAX_NAME_SIZE];
    double height;

    sscanf(args, " %s %lf", name, &height);

    if (visitor_height_valid(height) == FALSE ||
        park_is_full(park->total_visitors) == TRUE) {
        return;
    }

    if (park->visitors == NULL) {
        park->visitors = create_visitor(name, height);
    } else {
        if (visitor_exists(park->visitors, name) == TRUE) {
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

// Prints every ride in the park
void print_all_rides(struct ride *first_ride) {

    printf("Rides:\n");

    struct ride *current_ride = first_ride;
    while (current_ride != NULL) {
        print_ride(current_ride);
        current_ride = current_ride->next;
    }
}

// Prints every visitor in a queue, whether that's roaming or a ride
void print_all_visitors(struct visitor *first_visitor) {

    printf("Visitors:\n");

    struct visitor *current_visitor = first_visitor;
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

// Checks if a given ride name exists in the park
int ride_exists(struct ride *first_ride, char name[MAX_NAME_SIZE]) {
    struct ride *current = first_ride;

    while (current != NULL) {
        if (strcmp(current->name, name) == SAME) {
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}

// Checks a given visitor name against the list of visitors in the park
int visitor_exists(struct visitor *first_visitor, char name[MAX_NAME_SIZE]) {

    struct visitor *current = first_visitor;
    while (current != NULL) {
        if (strcmp(current->name, name) == SAME) {
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

    if (height < MIN_HEIGHT || height > MAX_HEIGHT) {
        printf("ERROR: Height must be between %d and %d.\n",
            MIN_HEIGHT, MAX_HEIGHT);
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

////////////////////////////////////////////////////////////////////////////////
// END STAGE 1
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// BEGIN STAGE 2
////////////////////////////////////////////////////////////////////////////////

// Inserts a ride at a specific index in the park's ride list
void insert_ride(struct park *park, char *args) {
    int index;
    char name[MAX_NAME_SIZE];
    char type_str[MAX_NAME_SIZE];

    sscanf(args, " %d %s %s", &index, name, type_str);
    enum ride_type type = string_to_type(type_str);

    if (is_valid_index(index) == FALSE || is_type_invalid(type) == TRUE) {
        return;
    } else if (ride_exists(park->rides, name) == TRUE) {
        printf("ERROR: a ride with name: '%s' ", name);
        printf("already exists in this park.\n");
        return;
    }

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

// Checks if the given insertion index is valid
int is_valid_index(int index) {

    if (index < MIN_VALID_INDEX) {
        printf("ERROR: n must be at least %d.\n", MIN_VALID_INDEX);
        return FALSE;
    } else {
        return TRUE;
    }
}

// Adds a visitor to the queue of a specific ride
void add_visitor_to_ride(struct park *park, char *args) {

    struct validate_fields fields;
    sscanf(args, " %s %s", fields.r_name, fields.v_name);

    fields.ride = retrieve_ride(park->rides, fields.r_name);
    fields.visitor = retrieve_visitor(park->visitors, fields.v_name);

    int valid = validate_action(park, ADD_V_TO_R, &fields);
    if (!valid) {
        return;
    }

    transfer_visitor_queue(&(park->visitors),
        &(fields.ride->queue), ADD_V_TO_R, fields);
}

// Adds a visitor to a specific queue
void add_visitor(struct visitor **head, struct visitor *visitor) {

    if (*head == NULL) {
        *head = visitor;
    } else {
        struct visitor *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = visitor;
    }
}

// Removes a visitor from the queue of a specific ride and
// puts them back in roaming
void remove_visitor_from_ride(struct park *park, char *args) {

    struct validate_fields fields;
    sscanf(args, " %s", fields.v_name);

    fields.ride = find_ride_containing(park, fields.v_name);
    if (fields.ride == NULL) {
        printf("ERROR: Visitor '%s' not found in any queue.\n", fields.v_name);
        return;
    }

    fields.visitor = retrieve_visitor(fields.ride->queue, fields.v_name);
    if (fields.visitor == NULL) {
        printf("ERROR: Visitor '%s' not found in any queue.\n", fields.v_name);
        return;
    }

    transfer_visitor_queue(&(fields.ride->queue),
        &(park->visitors), REMOVE_V_FROM_R, fields);
}

// Removes a visitor from a queue
void remove_visitor(struct visitor **head, struct visitor *visitor) {

    struct visitor *previous = NULL;
    struct visitor *current = *head;
    int list_length = calculate_list_length(*head);

    while (current != NULL) {
        if (current == visitor) {
            if (list_length == 1) {
                *head = NULL;
            } else if (previous == NULL && list_length > 1) {
                *head = current->next;
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

// Moves a visitor to a different ride's queue
void move_visitor_to_different_ride(struct park *park, char *args) {

    struct validate_fields fields;
    sscanf(args, " %s %s", fields.v_name, fields.r_name);

    fields.target_ride = retrieve_ride(park->rides, fields.r_name);
    if (fields.target_ride == NULL) {
        printf("ERROR: No ride exists with name '%s'.\n", fields.r_name);
        return;
    }

    fields.ride = find_ride_containing(park, fields.v_name);
    fields.visitor = NULL;

    // Case 1: Visitor is roaming
    if (fields.ride == NULL) {
        fields.visitor = retrieve_visitor(park->visitors, fields.v_name);
        if (fields.visitor != NULL) {
            transfer_visitor_queue(&(park->visitors),
                &(fields.target_ride->queue), MOVE_V_TO_R, fields);
        } else {
            printf("ERROR: No ride exists with name '%s'.\n", fields.r_name);
        }
        return;
    }

    // Case 2: Visitor is in another queue
    fields.visitor = retrieve_visitor(fields.ride->queue, fields.v_name);
    int valid = validate_action(park, MOVE_V_TO_R, &fields);
    if (!valid) {
        return;
    }

    transfer_visitor_queue(&(fields.ride->queue),
        &(fields.target_ride->queue), MOVE_V_TO_R, fields);
}

// Handles the transfer of visitors from one queue to another
void transfer_visitor_queue(struct visitor **remove_head,
    struct visitor **add_head, char action, struct validate_fields fields) {

    remove_visitor(remove_head, fields.visitor);
    add_visitor(add_head, fields.visitor);

    if (action == ADD_V_TO_R) {
        printf("Visitor: '%s' has entered the queue for '%s'.\n",
            fields.v_name, fields.r_name);
    } else if (action == REMOVE_V_FROM_R) {
        printf("Visitor: '%s' has been removed ", fields.v_name);
        printf("from their ride queue and is now roaming the park.\n");
    } else if (action == MOVE_V_TO_R) {
        printf("Visitor: '%s' has been moved to the queue for '%s'.\n",
            fields.v_name, fields.target_ride->name);
    }
}

// Counts the number of visitors in the park
void count_total_visitors(struct park *park) {

    int roaming_visitors;
    roaming_visitors = calculate_list_length(park->visitors);

    int queueing_visitors = 0;
    struct ride *current_ride = park->rides;
    while (current_ride != NULL) {
        queueing_visitors += calculate_list_length(current_ride->queue);
        current_ride = current_ride->next;
    }

    int total_visitors = roaming_visitors + queueing_visitors;
    printf("Total visitors: %d\n", total_visitors);
    printf("Visitors walking around: %d\n", roaming_visitors);
    printf("Visitors in queues: %d\n", queueing_visitors);
}

// Counts the number of visitors in the queue between two rides
void count_queue_visitors(struct park *park, char *args) {

    char first_ride[MAX_NAME_SIZE];
    char second_ride[MAX_NAME_SIZE];
    char direction;
    sscanf(args, " %s %s %c", first_ride, second_ride, &direction);

    int count = 0;
    struct ride *first = retrieve_ride(park->rides, first_ride);
    struct ride *second = retrieve_ride(park->rides, second_ride);

    if (first == NULL || second == NULL) {
        printf("ERROR: One or both rides do not exist ");
        printf("('%s' or '%s').\n", first_ride, second_ride);
        return;
    }

    struct ride *start = NULL;
    struct ride *end = NULL;
    if (direction == '>') {
        start = first;
        end = second;
    } else if (direction == '<') {
        start = second;
        end = first;
    }

    struct ride *current_ride = start;
    while (TRUE) {
        count += calculate_list_length(current_ride->queue);
        if (current_ride == end) {
            break;
        } else if (current_ride->next == NULL) {
            current_ride = park->rides;
        } else {
            current_ride = current_ride->next;
        }
    }

    printf("Total visitors from '%s' to '%s': %d.\n",
        start->name, end->name, count);
}

// Handles validation of certain actions
int validate_action(struct park *park, char action,
    struct validate_fields *fields) {

    int valid = TRUE;

    if (action == ADD_V_TO_R) {
        valid = validate_avtr(fields);
    } else if (action == MOVE_V_TO_R) {
        valid = validate_mvtdr(fields);
    } else if (action == SHUT_DOWN_RIDE) {
        valid = validate_sdr(park, fields);
    }
    return valid;
}

// Validates the add visitor to ride function
int validate_avtr(struct validate_fields *fields) {

    if (fields->ride == NULL) {
        printf("ERROR: No ride exists with name '%s'.\n", fields->r_name);
        return FALSE;
    } else if (fields->visitor == NULL) {
        printf("ERROR: No visitor exists with name '%s'.\n", fields->v_name);
        return FALSE;
    } else if (fields->visitor->height < fields->ride->min_height) {
        printf("ERROR: '%s' is not tall enough to ride '%s'.\n",
            fields->v_name, fields->r_name);
        return FALSE;
    }

    int list_length = calculate_list_length(fields->ride->queue);
    if (list_length > fields->ride->queue_capacity) {
        printf("ERROR: The queue for '%s' is full. ", fields->r_name);
        printf("'%s' cannot join the queue.\n", fields->v_name);
        return FALSE;
    }
    return TRUE;
}

// Validates the move visitor to different ride function
int validate_mvtdr(struct validate_fields *fields) {

    if (fields->ride == fields->target_ride) {
        printf("ERROR: '%s' is already in the queue for '%s'.\n",
            fields->v_name, fields->ride->name);
        return FALSE;
    } else if (fields->visitor == NULL) {
        printf("ERROR: No visitor with name: '%s' exists.\n", fields->v_name);
        return FALSE;
    } else if (calculate_list_length(fields->target_ride->queue)
        >= fields->target_ride->queue_capacity) {
        printf("ERROR: The queue for '%s' is", fields->target_ride->name);
        printf(" full. '%s' cannot join the queue.\n", fields->v_name);
        return FALSE;
    } else if (fields->visitor->height < fields->target_ride->min_height) {
        printf("ERROR: '%s' is not tall enough to ride '%s'.\n",
            fields->v_name, fields->target_ride->name);
        return FALSE;
    }
    return TRUE;
}

// Retrieves the correct ride to add a visitor to
struct ride *retrieve_ride(struct ride *head, char name[MAX_NAME_SIZE]) {

    struct ride *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == SAME) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Retrieves the correct visitor to add to a ride queue
struct visitor *retrieve_visitor(struct visitor *head,
    char name[MAX_NAME_SIZE]) {

    struct visitor *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == SAME) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Finds the ride that contains a specific visitor
struct ride *find_ride_containing(struct park *park,
    char v_name[MAX_NAME_SIZE]) {

    struct ride *current_ride = park->rides;
    while (current_ride != NULL) {
        struct visitor *current_visitor = current_ride->queue;
        while (current_visitor != NULL) {
            if (strcmp(current_visitor->name, v_name) == SAME) {
                return current_ride;
            }
            current_visitor = current_visitor->next;
        }
        current_ride = current_ride->next;
    }
    return NULL;
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
// END STAGE 2
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// BEGIN STAGE 3
////////////////////////////////////////////////////////////////////////////////

// Frees all rides and visitors
void end_of_day_procedure(struct park *park) {

    free_visitors(park->visitors);
    struct ride *current_ride = park->rides;

    while (current_ride != NULL) {
        struct ride *next_ride = current_ride->next;
        free_visitors(current_ride->queue);
        free(current_ride);
        current_ride = next_ride;
    }

    free(park);
    printf("\nGoodbye!\n");
    exit(0);
}

// Frees all visitors from a queue
void free_visitors(struct visitor *head) {

    struct visitor *current = head;
    while (current != NULL) {
        struct visitor *next = current->next;
        free(current);
        current = next;
    }
}

// Frees a single visitor from a specific queue
void free_one_visitor(struct park *park, char *args) {

    char name[MAX_NAME_SIZE];
    sscanf(args, " %s", name);

    struct visitor *visitor = NULL;
    struct ride *ride = find_ride_containing(park, name);

    if (ride == NULL) {
        if (retrieve_visitor(park->visitors, name) == NULL) {
            printf("ERROR: Visitor '%s' not found in the park.\n", name);
            return;
        } else {
            visitor = retrieve_visitor(park->visitors, name);
            remove_visitor(&(park->visitors), visitor);
        }
    } else {
        visitor = retrieve_visitor(ride->queue, name);
        remove_visitor(&(ride->queue), visitor);
    }

    free(visitor);
    park->total_visitors--;
    printf("Visitor: '%s' has left the park.\n", name);
}

// Handles operation of all rides in the park
// Uses recursion to traverse the linked list backwards
void operate_all_rides(struct park *park, struct ride *ride) {

    if (ride == NULL) {
        return;
    }

    operate_all_rides(park, ride->next);
    operate_ride(park, ride);
}

// Operates a single ride
void operate_ride(struct park *park, struct ride *ride) {

    for (int i = 0; i < ride->rider_capacity; i++) {
        if (ride->queue == NULL) {
            return;
        }
        add_visitor(&(park->visitors), ride->queue);
        remove_visitor(&(ride->queue), ride->queue);
    }
}

// Shuts down a ride
void shut_down_ride(struct park *park, char *args) {

    struct validate_fields fields;
    sscanf(args, " %s", fields.r_name);

    fields.ride = retrieve_ride(park->rides, fields.r_name);
    int valid = validate_action(park, SHUT_DOWN_RIDE, &fields);
    if (!valid) {
        return;
    }
    // Handles movement of visitors
    struct ride *current = park->rides;
    while (current != NULL) {
        if (current == fields.ride) {
        } else if (current->type == fields.ride->type) {
            while (current->queue_capacity > calculate_list_length(
                current->queue) && fields.ride->queue != NULL) {
                fields.visitor = fields.ride->queue;
                transfer_visitor_queue(&(fields.ride->queue),
                    &(current->queue), SHUT_DOWN_RIDE, fields);
            }
        }
        current = current->next;
    }

    free_ride(park, fields.ride);
    printf("Ride: '%s' shut down.\n", fields.r_name);
}

// Validates the shutdown of a ride
int validate_sdr(struct park *park, struct validate_fields *fields) {

    if (fields->ride == NULL) {
        printf("ERROR: No ride exists with name: '%s'.\n", fields->r_name);
        return FALSE;
    } else if (fields->ride->queue == NULL) {
        free_ride(park, fields->ride);
        printf("Ride: '%s' shut down.\n", fields->r_name);
        return FALSE;
    } else {
        int stranded_visitors = calculate_list_length(fields->ride->queue);
        int space_in_ride_type = type_vacancy(
            park->rides, fields->ride->type, fields->r_name);
        if (stranded_visitors > space_in_ride_type) {
            printf("ERROR: Not enough capacity to redistribute ");
            printf("all visitors from '%s'.\n", fields->r_name);
            free_ride(park, fields->ride);
            printf("Ride: '%s' shut down.\n", fields->r_name);
            return FALSE;
        }
    }
    return TRUE;
}

// Frees a ride and sends the visitors in the ride back to roaming
void free_ride(struct park *park, struct ride *ride) {

    struct ride *previous = NULL;
    struct ride *current = park->rides;

    while (current != NULL) {
        if (current == ride) {
            send_visitors_to_roaming(park, ride);
            if (previous == NULL) {
                park->rides = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }
}

// Sends all visitors from a certain ride back to roaming
void send_visitors_to_roaming(struct park *park, struct ride *ride) {

    while (ride->queue != NULL) {
        add_visitor(&(park->visitors), ride->queue);
        remove_visitor(&(ride->queue), ride->queue);
    }
}

// Calculates the total number of free spots in a type of ride
int type_vacancy(struct ride *head,
    enum ride_type type, char ride_name[MAX_NAME_SIZE]) {

    int vacancy = 0;

    struct ride *current = head;
    while (current != NULL) {
        if (strcmp(current->name, ride_name) == SAME) {
        } else if (current->type == type) {
            vacancy += current->queue_capacity -
                calculate_list_length(current->queue);
        }
        current = current->next;
    }
    return vacancy;
}

////////////////////////////////////////////////////////////////////////////////
// END STAGE 3
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// BEGIN STAGE 4
////////////////////////////////////////////////////////////////////////////////

// Merges 2 rides of the same type
void merge_rides(struct park *park, char *args) {

    char type_str[MAX_NAME_SIZE];
    sscanf(args, " %s", type_str);
    enum ride_type type = string_to_type(type_str);

    if (calculate_type_count(park, type) < 2) {
        printf("ERROR: Not enough rides of the specified type to merge.\n");
        return;
    }

    struct ride *first_ride = find_shortest_queue(park, type);
    struct ride *second_ride = find_2nd_shortest_queue(park, type, first_ride);

    // Checks which ride is closer to the head of the list and swaps accordingly
    if (!is_closer_to_head(park->rides, first_ride, second_ride)) {
        struct ride *temp = first_ride;
        first_ride = second_ride;
        second_ride = temp;
    }

    first_ride->rider_capacity *= CAPACITY_MULTIPLIER;
    first_ride->queue_capacity *= CAPACITY_MULTIPLIER;
    merge_ride_queues(first_ride, second_ride);

    free_ride(park, second_ride);
    printf("Merged the two smallest rides of type '%s'.\n",
        type_to_string(type));
}

// Calculates the amount of rides of a certain type
int calculate_type_count(struct park *park, enum ride_type type) {

    int type_count = 0;

    struct ride *current = park->rides;
    while (current != NULL) {
        if (current->type == type) {
            type_count++;
        }
        current = current->next;
    }
    return type_count;
}

// Merges the queues of two rides
void merge_ride_queues(struct ride *first_ride, struct ride *second_ride) {

    struct visitor *queue1 = first_ride->queue;
    struct visitor *queue2 = second_ride->queue;
    struct visitor *merged_queue = NULL;
    struct visitor **tail = &merged_queue;

    // Takes alternate visitors from each queue
    while (queue1 != NULL && queue2 != NULL) {
        *tail = queue1;
        queue1 = queue1->next;
        tail = &((*tail)->next);

        *tail = queue2;
        queue2 = queue2->next;
        tail = &((*tail)->next);
    }

    // Adds any remaining visitors to the queue
    if (queue1 != NULL) {
        *tail = queue1;
    } else if (queue2 != NULL) {
        *tail = queue2;
    }

    first_ride->queue = merged_queue;
    second_ride->queue = NULL;
}

// Finds the ride with the shortest queue of a certain type
struct ride *find_shortest_queue(struct park *park, enum ride_type type) {

    struct ride *current = park->rides;
    struct ride *shortest_ride = NULL;
    int shortest_length = MAX_QUEUE_LENGTH;

    while (current != NULL) {
        if (current->type == type) {
            int queue_length = calculate_list_length(current->queue);
            if (queue_length < shortest_length) {
                shortest_length = queue_length;
                shortest_ride = current;
            }
        }
        current = current->next;
    }
    return shortest_ride;
}

// Finds the second shortest queue of a certain type
struct ride *find_2nd_shortest_queue(struct park *park,
    enum ride_type type, struct ride *ride_to_skip) {

    struct ride *current = park->rides;
    struct ride *second_shortest_ride = NULL;
    int shortest_length = MAX_QUEUE_LENGTH;

    while (current != NULL) {
        if (current->type == type && current != ride_to_skip) {
            int queue_length = calculate_list_length(current->queue);
            if (queue_length < shortest_length) {
                shortest_length = queue_length;
                second_shortest_ride = current;
            }
        }
        current = current->next;
    }
    return second_shortest_ride;
}

// Determines if ride1 is closer to the head of the list than ride2
int is_closer_to_head(struct ride *head,
    struct ride *ride1, struct ride *ride2) {

    int ride1_is_closer = FALSE;
    struct ride *current = head;

    while (current != NULL) {
        if (current == ride1) {
            ride1_is_closer = TRUE;
            break;
        } else if (current == ride2) {
            ride1_is_closer = FALSE;
            break;
        }
        current = current->next;
    }
    return ride1_is_closer;
}

// Splits a ride into n different smaller rides
void split_ride(struct park *park, char *args) {

    int n_rides;
    char ride_name[MAX_NAME_SIZE];
    char base_name[MAX_NAME_SIZE];

    sscanf(args, " %d %s", &n_rides, ride_name);
    strcpy(base_name, ride_name);

    struct ride *ride_to_split = retrieve_ride(park->rides, ride_name);
    if (ride_to_split == NULL) {
        printf("ERROR: No ride exists with name: '%s'.\n", ride_name);
        return;
    } else if (n_rides <= 1) {
        printf("ERROR: Cannot split '%s' into %d rides. n must be > 1.\n",
            ride_name, n_rides);
        return;
    }

    int name_len = strlen(ride_name);
    enum ride_type type = ride_to_split->type;
    int remaining_new_rides = n_rides;
    struct ride *previous_new_ride = NULL;
    struct ride *new_ride = NULL;

    // Creates n rides with n unique names, inserts them into the linked list
    for (int i = 1; i <= n_rides; i++) {
        make_unique_name(ride_name, base_name, name_len, park->rides);
        new_ride = create_ride(ride_name, type);
        distribute_visitors(new_ride, ride_to_split, &remaining_new_rides);

        if (i == 1) {
            insert_split_ride(&(park->rides), ride_to_split, new_ride);
        } else {
            insert_split_ride(&(park->rides), previous_new_ride, new_ride);
        }
        previous_new_ride = new_ride;
    }

    free_ride(park, ride_to_split);
    printf("Ride '%s' split into %d new rides.\n", base_name, n_rides);
}

// Generates a unique name
void make_unique_name(char *ride_name,
    char *original_name, int name_len, struct ride *ride_list) {

    int suffix = 1;
    strcpy(ride_name, original_name);

    while (ride_exists(ride_list, ride_name)) {
        if (name_len + NEW_NAME_CHARS >= MAX_NAME_SIZE) {
            ride_name[name_len - 2] = '_';
            ride_name[name_len - 1] = '0' + suffix;
        } else {
            ride_name[name_len] = '_';
            ride_name[name_len + 1] = '0' + suffix;
            ride_name[name_len + 2] = '\0';
        }
        suffix++;
    }
}

// Calculate how many visitors in each ride and add them there
void distribute_visitors(struct ride *new_ride,
    struct ride *ride_to_split, int *remaining_new_rides) {

    int visitors_in_queue = calculate_list_length(ride_to_split->queue);
    int base_visitors_per_ride = visitors_in_queue / *remaining_new_rides;
    int extra_visitors = visitors_in_queue % *remaining_new_rides;

    // Determine the number of visitors for this ride
    int visitors_for_this_ride = base_visitors_per_ride;
    if (extra_visitors > 0) {
        visitors_for_this_ride++;
    }

    for (int i = 0; i < visitors_for_this_ride; i++) {
        add_visitor(&(new_ride->queue), ride_to_split->queue);
        remove_visitor(&(ride_to_split->queue), ride_to_split->queue);
    }
    (*remaining_new_rides)--;
}

// Inserts the split ride into the park's rides
void insert_split_ride(struct ride **head,
    struct ride *ride_to_insert_after, struct ride *ride_to_insert) {

    struct ride *current = *head;
    while (current != NULL) {
        if (current == ride_to_insert_after) {
            ride_to_insert->next = current->next;
            current->next = ride_to_insert;
        }
        current = current->next;
    }
}

// Schedule an event for the future
void schedule_command(struct park *park, char *args) {

    int ticks;
    char command_str[MAX_COMMAND_SIZE];

    if (sscanf(args, " %d %[^\n]", &ticks, command_str) != 2 || ticks < 1) {
        printf("ERROR: Invalid tick delay: %d. Must be > 0.\n", ticks);
        return;
    }

    struct scheduled_command *cmd = malloc(sizeof(struct scheduled_command));
    strcpy(cmd->instruction, command_str);
    cmd->ticks_until_execution = ticks;
    cmd->next = NULL;

    if (park->commands == NULL) {
        park->commands = cmd;
    } else {
        struct scheduled_command *current = park->commands;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = cmd;
    }

    printf("Command scheduled to run in %d ticks.\n", ticks);
}

// Moves the schedule forward by a certain amount of ticks
void move_schedule_forward(struct park *park, char *args) {

    int ticks;
    if (sscanf(args, " %d", &ticks) != 1 || ticks < MIN_TICKS) {
        printf("ERROR: Invalid tick delay: %d. Must be > 0.\n", ticks);
        return;
    }

    tick_forward(park, ticks);
}

// Ticks all scheduled commands forward by x ticks
// Executes commands them and frees them from the list as required
void tick_forward(struct park *park, int ticks) {

    for (int i = 1; i <= ticks; i++) {
        struct scheduled_command *current = park->commands;
        while (current != NULL) {
            current->ticks_until_execution--;
            if (current->ticks_until_execution == 0) {
                execute_command(park, current);
                struct scheduled_command *next = current->next;
                free_command(park, current);
                current = next;
            } else {
                current = current->next;
            }
        }
    }
}

// Acquires command from command struct and executes it
void execute_command(struct park *park, struct scheduled_command *cmd) {

    char command;
    char args[MAX_COMMAND_SIZE] = "";
    sscanf(cmd->instruction, " %c %[^\n]", &command, args);
    // quit scheduled case
    if (cmd->instruction[0] == QUIT) {
        printf("ERROR: Cannot schedule quit command.\n");
        return;
    }

    execute_ticking_instruction(park, command, args);
}

// Frees the current command from the command list
void free_command(struct park *park, struct scheduled_command *cmd) {

    if (park->commands == cmd && cmd->next == NULL) {
        park->commands = NULL;
    } else if (park->commands == cmd && cmd->next != NULL) {
        park->commands = cmd->next;
    } else {
        struct scheduled_command *current = park->commands;
        while (current->next != NULL) {
            if (current->next == cmd) {
                current->next = cmd->next;
                break;
            }
            current = current->next;
        }
    }
    free(cmd);
}

////////////////////////////////////////////////////////////////////////////////
// END STAGE 4
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
void print_welcome_message(char name[MAX_NAME_SIZE]) {
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
//      name - a char array of length MAX_NAME_SIZE, which will be used
//                  to store the name.
// Returns: None
// Usage:
// ```
//      char name[MAX_NAME_SIZE];
//      scan_name(name);
// ```
void scan_name(char name[MAX_NAME_SIZE]) {
    scan_token(name, MAX_NAME_SIZE);
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
    char type[MAX_NAME_SIZE];
    scan_token(type, MAX_NAME_SIZE);
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