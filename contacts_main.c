#include <stdio.h>
#include <string.h>

#include "contacts.h"

#define MAX_CMD_LEN 128

/*
 * This is in general *very* similar to the list_main file seen in lab 2
 * One big difference is the notion of switching between contact logs in one
 * run of the program.
 * You have to create or load a contact log from a file before you can do things
 * like add, lookup, or write.
 * The code below has to check if contact log is NULL before the operations
 * occur. Also, the user has to explicitly clear the current contact log before
 * they can create or load in a new one.
 */
int main(int argc, char **argv) {
    contacts_log_t *log = NULL;
    if (argc == 2) {
        //Get the final 3 characters of the file and see if to load from text binary or error
        char final[4];
        final[0] = argv[1][strlen(argv[1]) - 3];
        final[1] = argv[1][strlen(argv[1]) - 2];
        final[2] = argv[1][strlen(argv[1]) - 1];
        final[3] = '\0';
        if (strcmp(final, "txt") == 0) {
            log = read_contacts_log_from_text(argv[1]);
            if (log != NULL) {
                    printf("Contacts log loaded from text file\n");
            } else {
                    printf("Failed to read contacts log from text file\n");
            }
        } else if (strcmp(final, "bin") == 0) {
            log = read_contacts_log_from_binary(argv[1]);
            if (log != NULL) {
                    printf("Contacts log loaded from binary file\n");
            } else {
                    printf("Failed to read contacts log from binary file\n");
            }
        } else {
            printf("Error: Unknown contacts log file extension\n");
        }
    }

    printf("CSCI 2021 Contact Log System\n");
    printf("Commands:\n");
    printf("  create <name>:            creates a new log with specified name\n");
    printf("  log:                      shows the name of the active contacts log\n");
    printf("  add <name> <phone> <zip>: adds a new contact\n");
    printf("  lookup <name>:            searches for a phone number by contact name\n");
    printf("  clear:                    resets current contacts log\n");
    printf("  print:                    shows all contacts in active log\n");
    printf("  write_text:               saves all contacts to text file\n");
    printf("  read_text <file_name>:    loads contacts from text file\n");
    printf("  write_bin:                saves all contacts to binary file\n");
    printf("  read_bin <file_name>:     loads contacts from binary file\n");
    printf("  exit:                     exits the program\n");

    char cmd[MAX_CMD_LEN];
    while (1) {
        printf("contacts> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            break;
        }

        else if (strcmp("create", cmd) == 0) {
            scanf("%s", cmd); // Read in new log name
            if (log != NULL) {
                printf("Error: You already have an contacts log.\n");
                printf("You can remove it with the \'clear\' command\n");
            } else {
                log = create_contacts_log(cmd);
                if (log == NULL) {
                    printf("Contacts log creation failed\n");
                }
            }
        }
        else if (strcmp("log", cmd) == 0) {
            //shows the name of the active contacts log
            if(log != NULL) {
                printf("%s\n", get_contacts_log_name(log));
            } else {
                printf("Error: You must create or load a contacts log first\n");
            }
        }

        else if (strcmp("add", cmd) == 0) {
            //adds a new contact
            char name[MAX_NAME_LEN];
            unsigned long phone_number;
            unsigned zip_code; 
            scanf("%s", name);
            scanf("%lu", &phone_number);
            scanf("%u", &zip_code);
            if (log != NULL) {
                if (phone_number < 1000000000 || phone_number > 9999999999) {
                    printf("Error: Invalid phone number and/or zip code\n");
                } else if (zip_code < 10000 || zip_code > 99999) {
                    printf("Error: Invalid phone number and/or zip code\n");
                } else {
                    if (add_contact(log, name, phone_number, zip_code) == -1) {
                        printf("Error: Could not add contact\n");
                    }
                }
            } else {
                printf("Error: You must create or load a contacts log first\n");
            }
        }

        else if (strcmp("lookup", cmd) == 0) {
            //searches for a phone number by contact name
            char name[MAX_NAME_LEN];
            scanf("%s", name);
            if (log != NULL) {
                if (find_phone_number(log, name) == -1) {
                    printf("No phone number for '%s' found\n", name);
                } else {
                    printf("%s: %lu\n", name, find_phone_number(log, name));
                }
            } else {
                printf("Error: You must create or load a contacts log first\n");
            }
        }

        else if (strcmp("clear", cmd) == 0) {
            //resets current contacts log
            if (log != NULL) {
                free_contacts_log(log);
                log = NULL;
            } else {
                printf("Error: No contacts log to clear\n");
            }
        }

        else if (strcmp("print", cmd) == 0) {
            //shows all contacts in active log
            if (log != NULL) {
                printf("All contacts in %s:\n", get_contacts_log_name(log));
                print_contacts_log(log);
            } else {
                printf("Error: You must create or load a contacts log first\n");
            }
        }

        else if (strcmp("write_text", cmd) == 0) {
            //saves all contacts to text file
            if (log != NULL) {
                if (write_contacts_log_to_text(log) == -1) {
                    printf("Failed to write contacts log to text file\n");
                } else {
                    printf("Contacts log successfully written to %s.txt\n", log->log_name);
                }
            } else {
                printf("Failed to write contacts log to text file\n");
            }
        }

        else if (strcmp("read_text", cmd) == 0) {
            //loads contacts from text file
            char name[MAX_NAME_LEN];
            scanf("%s", name);
            if (log == NULL) {
                log = read_contacts_log_from_text(name);
                if (log != NULL) {
                    printf("Contacts log loaded from text file\n");
                } else {
                    printf("Failed to read contacts log from text file\n");
                }
            } else {
                printf("Error: You must clear current contacts log first\n");
            }
        }

        else if (strcmp("write_bin", cmd) == 0) {
            //saves all contacts to binary file
            if (log != NULL) {
                if (write_contacts_log_to_binary(log) == -1) {
                    printf("Failed to write contacts log to binary file\n");
                } else {
                    printf("Contacts log successfully written to %s.bin\n", log->log_name);
                }
            } else {
                printf("Failed to write contacts log to binary file\n");
            }
        }

        else if (strcmp("read_bin", cmd) == 0) {
            //loads contacts from binary file
            char name[MAX_NAME_LEN];
            scanf("%s", name);
            if (log == NULL) {
                log = read_contacts_log_from_binary(name);
                if (log != NULL) {
                    printf("Contacts log loaded from binary file\n");
                } else {
                    printf("Failed to read contacts log from binary file\n");
                }
            } else {
                printf("Error: You must clear current contacts log first\n");
            }
        }

        else {
            printf("%s", cmd);
            printf("Unknown command __\n");
        }
    }

    if (log != NULL) {
        free_contacts_log(log);
    }
    return 0;
}
