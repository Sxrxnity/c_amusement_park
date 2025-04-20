// CS Amusement Park
// cs_amusement_park.h
// Written by <Danny Sun>, <z5691331>
// on <03/04/2025>

////////////////////////////////////////////////////////////////////////////////
// Provided Constants
////////////////////////////////////////////////////////////////////////////////
#define MAX_NAME_SIZE           100

////////////////////////////////////////////////////////////////////////////////
// Your constants
////////////////////////////////////////////////////////////////////////////////
#define MAX_COMMAND_SIZE        120

#define FALSE                   0
#define TRUE                    1
#define SAME                    0

#define MIN_HEIGHT              50
#define MAX_HEIGHT              250
#define MAX_VISITORS            40

#define RC_RIDER_CAPACITY       4
#define RC_QUEUE_CAPACITY       7
#define RC_MIN_HEIGHT           120.0
#define CAROUSEL_RIDER_CAPACITY 6
#define CAROUSEL_QUEUE_CAPACITY 9
#define CAROUSEL_MIN_HEIGHT     60.0
#define FW_RIDER_CAPACITY       8
#define FW_QUEUE_CAPACITY       11
#define FW_MIN_HEIGHT           75.0
#define BC_RIDER_CAPACITY       10
#define BC_QUEUE_CAPACITY       13
#define BC_MIN_HEIGHT           100.0

#define MAX_QUEUE_LENGTH        15
#define CAPACITY_MULTIPLIER     2
#define NEW_NAME_CHARS          2
#define MIN_VALID_INDEX         1
#define MIN_TICKS               1

#define HELP                    '?'
#define APPEND                  'a'
#define RIDE                    'r'
#define VISITOR                 'v'
#define PRINT                   'p'

#define INSERT                  'i'
#define ADD_V_TO_R              'j'
#define REMOVE_V_FROM_R         'd'
#define MOVE_V_TO_R             'm'
#define COUNT_TOTAL_VISITORS    't'
#define COUNT_QUEUE_VISITORS    'c'

#define QUIT                    'q'
#define VISITOR_LEAVE           'l'
#define OPERATE_RIDES           'r'
#define SHUT_DOWN_RIDE          'S'

#define MERGE                   'M'
#define SPLIT                   's'
#define SCHEDULE_COMMAND        'T'
#define ADVANCE_TICKS           '~'

////////////////////////////////////////////////////////////////////////////////
// Provided Enums
////////////////////////////////////////////////////////////////////////////////

enum ride_type { ROLLER_COASTER, CAROUSEL, FERRIS_WHEEL, BUMPER_CARS, INVALID };

////////////////////////////////////////////////////////////////////////////////
// Provided Structs
////////////////////////////////////////////////////////////////////////////////

struct park {
    // The name of the park
    char name[MAX_NAME_SIZE];
    // The total number of visitors in the park
    int total_visitors;
    // The list of rides in the park (this points to the first one)
    struct ride *rides;
    // The list of visitors in the park (this points to the first one)
    struct visitor *visitors;
    // The list of scheduled instructions to be executed
    // (this points to the first one)
    struct scheduled_command *commands;
};

struct ride {
    // The name of the ride
    char name[MAX_NAME_SIZE];
    // The type of the ride i.e. ROLLER_COASTER, CAROUSEL,
    // FERRIS_WHEEL, BUMPER_CARS, INVALID
    enum ride_type type;
    // The number of riders the ride can hold
    int rider_capacity;
    // The number of visitors that can be in the queue
    int queue_capacity;
    // The minimum height required to ride
    double min_height;
    // The queue of visitors waiting to ride
    struct visitor *queue;
    // The next ride in the list
    struct ride *next;
};

struct visitor {
    // The name of the visitor
    char name[MAX_NAME_SIZE];
    // The height of the visitor
    double height;
    // The next visitor in the list
    struct visitor *next;
};

// My structs

struct validate_fields {
    // The pointer to the ride
    struct ride *ride;
    // The pointer to the target ride, only relevant for
    // 'moving visitor to different ride' function
    struct ride *target_ride;
    // The pointer to the visitor
    struct visitor *visitor;
    // The name of the ride
    char r_name[MAX_NAME_SIZE];
    // The name of the visitor
    char v_name[MAX_NAME_SIZE];
};

struct scheduled_command {
    // The string equivalent of the command
    char instruction[MAX_COMMAND_SIZE];
    // Ticks until instruction needs to be executed
    int ticks_until_execution;
    // Pointer to the next instruction
    struct scheduled_command *next;
};

////////////////////////////////////////////////////////////////////////////////
// Provided function prototypes
////////////////////////////////////////////////////////////////////////////////
void print_usage(void);
void scan_name(char name[MAX_NAME_SIZE]);
void print_ride(struct ride *ride);
void print_visitor(struct visitor *visitor);
enum ride_type scan_type(void);
void print_welcome_message(char name[MAX_NAME_SIZE]);

// Additional provided function prototypes
// You won't need to use these functions!
// We use just them to implement some of the provided helper functions.
int scan_token(char *buffer, int buffer_size);
char *type_to_string(enum ride_type type);
enum ride_type string_to_type(char *type_str);

////////////////////////////////////////////////////////////////////////////////
// Your function prototypes
////////////////////////////////////////////////////////////////////////////////


// --- Stage 1.1: Creating the Park ---
struct park *initialise_park(char name[MAX_NAME_SIZE]);
struct visitor *create_visitor(char name[MAX_NAME_SIZE], double height);
struct ride *create_ride(char name[MAX_NAME_SIZE], enum ride_type type);

// --- Stage 1.2: Command Loop and Help ---
void command_loop(struct park *park);
void execute_ticking_instruction(struct park *park,
    char command, char *args);

// --- Stage 1.3: Append Rides and Visitors ---
void add_entity(struct park *park, char *args);
void append_ride(struct park *park, char *args);
void append_visitor(struct park *park, char *args);

// --- Stage 1.4: Printing the Park ---
void print_all_rides(struct ride *first_ride);
void print_all_visitors(struct visitor *first_visitor);
void print_park(struct park *park);

// --- Stage 1.5: Handling Errors ---
int ride_exists(struct ride *first_ride, char name[MAX_NAME_SIZE]);
int visitor_exists(struct visitor *first_visitor, char name[MAX_NAME_SIZE]);
int is_type_invalid(enum ride_type type);
int visitor_height_valid(double height);
int park_is_full(int total_visitors);


// --- Stage 2.1: Inserting New Rides ---
void insert_ride(struct park *park, char *args);
int is_valid_index(int index);

// --- Stage 2.2: Add Visitors to Ride Queues ---
void add_visitor_to_ride(struct park *park, char *args);
void add_visitor(struct visitor **head, struct visitor *visitor);

// --- Stage 2.3: Remove Visitors from Queues ---
void remove_visitor_from_ride(struct park *park, char *args);
void remove_visitor(struct visitor **head, struct visitor *visitor);

// --- Stage 2.4: Move Visitors Between Rides ---
void move_visitor_to_different_ride(struct park *park, char *args);
void transfer_visitor_queue(struct visitor **remove_head,
    struct visitor **add_head, char action, struct validate_fields fields);

// --- Stage 2.5: Count Visitors ---
void count_total_visitors(struct park *park);
void count_queue_visitors(struct park *park, char *args);

// --- Shared validation and retrieval helpers ---
int validate_action(struct park *park, char action,
    struct validate_fields *fields);
int validate_avtr(struct validate_fields *fields);
int validate_mvtdr(struct validate_fields *fields);

struct ride *retrieve_ride(struct ride *head, char name[MAX_NAME_SIZE]);
struct visitor *retrieve_visitor(struct visitor *head, char name[MAX_NAME_SIZE]);
struct ride *find_ride_containing(struct park *park, char v_name[MAX_NAME_SIZE]);
int calculate_list_length(struct visitor *visitor);


// --- Stage 3.1: End of Day Procedure ---
void end_of_day_procedure(struct park *park);
void free_visitors(struct visitor *head);

// --- Stage 3.2: Visitor leaves Park ---
void free_one_visitor(struct park *park, char *args);

// --- Stage 3.3: Operate Rides ---
void operate_all_rides(struct park *park, struct ride *ride);
void operate_ride(struct park *park, struct ride *ride);

// --- Stage 3.4: Shut Down Rides ---
void shut_down_ride(struct park *park, char *args);
int validate_sdr(struct park *park, struct validate_fields *fields);
void free_ride(struct park *park, struct ride *ride);
void send_visitors_to_roaming(struct park *park, struct ride *ride);
int type_vacancy(struct ride *head, enum ride_type type,
    char ride_name[MAX_NAME_SIZE]);


// --- Stage 4.1: Merge Rides ---
void merge_rides(struct park *park, char *args);
int calculate_type_count(struct park *park, enum ride_type type);
void merge_ride_queues(struct ride *first_ride, struct ride *second_ride);
struct ride *find_shortest_queue(struct park *park, enum ride_type type);
struct ride *find_2nd_shortest_queue(struct park *park,
    enum ride_type type, struct ride *ride_to_skip);
int is_closer_to_head(struct ride *head,
    struct ride *ride1, struct ride *ride2);

// --- Stage 4.2: Split Rides ---
void split_ride(struct park *park, char *args);
void make_unique_name(char *ride_name,
    char *original_name, int name_len, struct ride *ride_list);
void distribute_visitors(struct ride *new_ride,
    struct ride *ride_to_split, int *remaining_new_rides);
void insert_split_ride(struct ride **head,
    struct ride *ride_to_insert_after, struct ride *ride_to_insert);

// --- Stage 4.3: Scheduled Commands ---
void schedule_command(struct park *park, char *args);
void move_schedule_forward(struct park *park, char *args);
void tick_forward(struct park *park, int ticks);
void execute_command(struct park *park, struct scheduled_command *cmd);
void free_command(struct park *park, struct scheduled_command *cmd);
void scan_token_by_index(char *src, char *dest, int max_len,
    int start_index, int *next_index);