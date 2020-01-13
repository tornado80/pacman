#define STR_(X) #X // stringify of numerical symbolic constants defined in other header files
#define STR(X) STR_(X) // makes sure for expansion
#define MAP_MAX_ROWS 20 // you can set a limit for maximum rows number
#define MAP_MAX_COLS 20 // you can set a limit for maximum column number
#define MAP_GRID_LIMIT 0 // you can disable grid limiting which ignores above settings by setting 0 or enable it by setting 1