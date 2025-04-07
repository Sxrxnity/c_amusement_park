// CS Amusement Park
// cs_amusement_park.h
// Written by <Danny Sun>, <z5691331>
// on <03/04/2025>

////////////////////////////////////////////////////////////////////////////////
// Provided Constants
////////////////////////////////////////////////////////////////////////////////
#define MAX_SIZE 100

////////////////////////////////////////////////////////////////////////////////
// Your constants
////////////////////////////////////////////////////////////////////////////////
#define FALSE 0
#define TRUE 1

#define HELP               '?'
#define APPEND             'a'
#define INSERT             'i'
#define RIDE               'r'
#define VISITOR            'v'
#define ADD_V_TO_R         'j'
#define REMOVE_V_FROM_R   'd'
#define PRINT              'p'

#define MIN_VISITOR_HEIGHT 50
#define MAX_VISITOR_HEIGHT 250

#define MAX_VISITORS       40

////////////////////////////////////////////////////////////////////////////////
// Provided Enums
////////////////////////////////////////////////////////////////////////////////

enum ride_type { ROLLER_COASTER, CAROUSEL, FERRIS_WHEEL, BUMPER_CARS, INVALID };

////////////////////////////////////////////////////////////////////////////////
// Provided Structs
////////////////////////////////////////////////////////////////////////////////

struct park {
    // The name of the park
    char name[MAX_SIZE];
    // The total number of visitors in the park
    int total_visitors;
    // The list of rides in the park (this points othe first one)
    struct ride *rides;
    // The list of visitors in the park (this points othe first one)
    struct visitor *visitors;
};

struct ride {
    // The name of the ride
    char name[MAX_SIZE];
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
    char name[MAX_SIZE];
    // The height of the visitor
    double height;
    // The next visitor in the list
    struct visitor *next;
};

////////////////////////////////////////////////////////////////////////////////
// Provided function prototypes
////////////////////////////////////////////////////////////////////////////////
void print_usage(void);
void scan_name(char name[MAX_SIZE]);
void print_ride(struct ride *ride);
void print_visitor(struct visitor *visitor);
enum ride_type scan_type(void);
void print_welcome_message(char name[MAX_SIZE]);

// Additional provided function prototypes
// You won't need to use these functions!
// We use just them to implement some of the provided helper functions.
int scan_token(char *buffer, int buffer_size);
char *type_to_string(enum ride_type type);
enum ride_type string_to_type(char *type_str);

////////////////////////////////////////////////////////////////////////////////
// Your function prototypes
////////////////////////////////////////////////////////////////////////////////

// Stage 1
struct park *initialise_park(char name[MAX_SIZE]);
struct visitor *create_visitor(char name[MAX_SIZE], double height);
struct ride *create_ride(char name[MAX_SIZE], enum ride_type type);
void command_loop(struct park *park);
void add_entity(struct park *park);
void append_ride(struct park *park);
void append_visitor(struct park *park);

void print_all_rides(struct ride *first_ride);
void print_all_visitors(struct visitor *first_visitor);
void print_park(struct park *park);

// Stage 2
void insert_ride(struct park *park);
void add_visitor_to_ride(struct park *park);
void remove_visitor_from_ride(struct park *park);
struct ride *find_ride_containing(struct park *park,
    char visitor_name[MAX_SIZE]);

struct ride *retrieve_ride(struct ride *ride, char name[MAX_SIZE]);
struct visitor *retrieve_visitor(struct visitor *visitor, char name[MAX_SIZE]);
int valid_ride_and_visitor(struct ride *ride, struct visitor *visitor,
    char ride_name[MAX_SIZE], char visitor_name[MAX_SIZE]);
void remove_visitor_from_roaming(struct park *park, char visitor_name[MAX_SIZE]);
void reappend_visitor(struct park *park, struct visitor *visitor);

// Stage 3

// Stage 4

// Stage 5

// Helper functions
int is_existing_ride(struct ride *first_ride, char name[MAX_SIZE]);
int is_existing_visitor(struct visitor *first_visitor, char name[MAX_SIZE]);

int is_type_invalid(enum ride_type type);
int visitor_height_valid(double height);
int park_is_full(int total_visitors);
int is_valid_index(int index);

int calculate_list_length(struct visitor *visitor);
