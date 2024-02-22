#ifndef CONTACTS_H
#define CONTACTS_H

#define MAX_NAME_LEN 32
#define NUM_BUCKETS 1787

// Data type for elements in each hash bucket
typedef struct node {
    char name[MAX_NAME_LEN]; // Contact's Name
    unsigned long phone_number;   // Contact's phone number
    unsigned zip_code;       // Contact's zip code
    struct node *next;       // Next node in list, or NULL if no next node
} node_t;

// Contacts log data type
typedef struct {
    char log_name[MAX_NAME_LEN];    // Name of contacts log
    node_t *buckets[NUM_BUCKETS];   // Hash table buckets (linked list heads)
    unsigned size;                  // Total number of entries in contacts log
} contacts_log_t;

// Create a new contacts log instance
// log_name: The name of the contacts log
// Returns: Pointer to a contacts_log_t representing an empty log
//          or NULL if an error occurs
contacts_log_t *create_contacts_log(const char *log_name);

// Returns a pointer to the contacts log name
// log: A pointer to a log to get the name of
// Returns: Pointer to log's name (not to be modified)
const char *get_contacts_log_name(const contacts_log_t *log);

// Add a new contact to the log (insert into hash table)
// log: A pointer to a contacts log to add the contact to
// name: Contact's name
// phone_number: Contact's phone number
// zip_code: Contact's zip code
// Returns: 0 if the contact was successfully added
//          or -1 if the contact could not be added
int add_contact(contacts_log_t *log, const char *name, unsigned long phone_number, unsigned zip_code);

// Search for a specific contact's phone number in the contacts log
// log: A pointer to a log to search for the phone number in
// name: Contact's name to search for
// Returns: The contact's phone number if found
//          or -1 if no matching contact name is found
long find_phone_number(const contacts_log_t *log, const char *name);

// Print out all contacts in the contacts log
// log: A pointer to the log containing the contacts to print
void print_contacts_log(const contacts_log_t *log);

// Frees all memory used to store the contents of the contacts log
// log: A pointer to the contacts log to free
void free_contacts_log(contacts_log_t *log);

// Write out all contacts in the log to a text file
// log: A pointer to the log containing the contacts to write out
// Returns: 0 on success or -1  if an error occurs opening the destination file
int write_contacts_log_to_text(const contacts_log_t *log);

// Read in all contacts from a text file and add to a new contacts_log
// file_name: The name of the text file to read
// Returns: A pointer to a new contacts log containing all contacts as recorded
//          in the file or NULL if the read operation fails
contacts_log_t *read_contacts_log_from_text(const char *file_name);

// Write all contacts in a contacts log to a binary file
// log: A pointer to a contacts log containing the contacts to write out
// Returns: 0 on success or -1 if an error occurs opening the destination file
int write_contacts_log_to_binary(const contacts_log_t *log);

// Read in all contacts from a binary file and add to a new contacts log
// file_name: The name of the binary file to read
// Returns: A pointer to a new contacts log containing all contacts as recorded
//          in the file or NULL if the read operation fails
contacts_log_t *read_contacts_log_from_binary(const char *file_name);

#endif // CONTACTS_H
