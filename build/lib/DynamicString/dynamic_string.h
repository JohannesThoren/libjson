/**
 * @file
 */

#ifndef STRING
#define STRING

#include <stddef.h>

/**
 *Dynamic string type
 * 
 */
typedef struct String {
    char *ptr;
    size_t length;
    size_t capacity;
} String;

/**
 *Allocates memory for a new string object
 *@return The new string
 */
String new_string();

/**
 *Deallocates the string and zeroes it's length and capacity
 *@param[out] str The string to delete
 */
void delete_string(String *str);

/**
 *Clones a strings characters
 *Allocates a new string object with the capacity of the original string length and then copies the characters
 *@param[in] str The string to copy
 *@return The copied string
 */
String string_copy(const String str);

/**
 *Clears a string
 *Sets the strings length to zero and zeroes out the string ptr
 *@param[in] str The string to clear
 */
void string_clear(String *str);

/**
 *Allocates a new string with space for capacity characters
 *@param[in] capacity The amount of capacity to allocate in the string
 *@return The allocated string with capacity capacity and 0 length
 */
String string_with_capacity(size_t capacity);

/**
 *Appends the character at the end of the string and expands if needed
 *@param[out] str The string pointer object
 *@param[in] ch The char to append to the end of the string
 */
void string_push_char(String *str, char ch);

/**
 *Appends another string at the end of the dynamic string and expands if needed
 *@param[out] str The string pointer object
 *@param[in] other The string to append at the end
 */
void string_append(String *str, const char *other);

/**
 *Reserves room for at least additional more characters
 *This function will reserve at least additional characters but might be more to decrease the amount of allocations 
 *@param[out] str The string pointer object
 *@param[in] additional The amount of space to reserve
 */
void string_reserve(String *str, size_t additional);

/**
 *Reserves room for at least additional more characters
 *This function will reserve at least additional characters but might be inefficient if several allocations will take place. In this case please refer to string_reserve 
 *@param[out] str The string pointer object
 *@param[in] additional The amount of space to reserve
 */
void string_reserve_exact(String *str, size_t additional);

/**
 *Prints a formated string to the end of the str
 *This function will work like a normal formated printf but will append all charecters to the end of str
 *@param[out] str The string pointer object
 *@param[in] fornat The string format
 *@return Error code for printing will be the same as defined by vsnprintf
 */
int string_printf(String *str, char const* const _Format, ...);



#endif