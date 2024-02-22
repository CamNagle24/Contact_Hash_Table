#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contacts.h"

// This is the (somewhat famous) djb2 hash
unsigned hash(const char *str) {
    unsigned hash_val = 5381;
    int i = 0;
    while (str[i] != '\0') {
        hash_val = ((hash_val << 5) + hash_val) + str[i];
        i++;
    }
    return hash_val % NUM_BUCKETS;
}

contacts_log_t *create_contacts_log(const char *log_name) {
    contacts_log_t *new_log = malloc(sizeof(contacts_log_t));
    if (new_log == NULL) {
        return NULL;
    }

    strcpy(new_log->log_name, log_name);
    for (int i = 0; i < NUM_BUCKETS; i++) {
        new_log->buckets[i] = NULL;
    }
    new_log->size = 0;
    return new_log;
}

const char *get_contacts_log_name(const contacts_log_t *log) {
    return log->log_name;
}

int add_contact(contacts_log_t *log, const char *name, unsigned long phone_number, unsigned zip_code) {
    //Makes the memory for the node, creates it, and puts in in the right linked list/hash table function spot
    node_t *new_node = malloc(sizeof(node_t));
    strcpy(new_node->name, name);
    new_node->phone_number = phone_number;
    new_node->zip_code = zip_code;
    new_node->next = NULL;
    log->size = log->size + 1;
    if (log->buckets[hash(name)] == NULL) {
        log->buckets[hash(name)] = new_node;
        return 0;
    } else {
        node_t *curr_Node = log->buckets[hash(name)];
        while (curr_Node->next != NULL) {
            curr_Node = curr_Node->next;
        }
        curr_Node->next = new_node;
        return 0;
    }
        return -1;
    
}

long find_phone_number(const contacts_log_t *log, const char *name) {
    //Goes to the bucket where the name is and goes through the linked list to find the name
    node_t *curr_Node = log->buckets[hash(name)];
    while (curr_Node != NULL) {
        if (strcmp(curr_Node->name, name) == 0) {
            return curr_Node->phone_number;
        }
        curr_Node = curr_Node->next;
    }
    return -1;
}

void print_contacts_log(const contacts_log_t *log) {
    //Goes through all the buckets and prints out the contact information
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        node_t *curr_Node = log->buckets[i];
        while (curr_Node != NULL) {
            printf("Name: %s", curr_Node->name);
            printf(", Phone Number: %lu", curr_Node->phone_number);
            printf(", Zip Code: %u\n", curr_Node->zip_code);
            curr_Node = curr_Node->next;
        }
    }
}

void free_contacts_log(contacts_log_t *log) {
    //Frees all the data
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        node_t *curr_Node = log->buckets[i];
        while (curr_Node != NULL) {
            node_t *past_Node = curr_Node;
            curr_Node = curr_Node->next;
            free(past_Node);
        }
        log->buckets[i] = NULL;
    }
    free(log);
}

int write_contacts_log_to_text(const contacts_log_t *log) {
    //Writes the contacts in the log to a txt file
    char file_name[MAX_NAME_LEN + strlen(".txt")];
    strcpy(file_name, log->log_name);
    strcat(file_name, ".txt");
    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        return -1;
    }

    fprintf(f, "%u\n", log->size);
    for (int i = 0; i < NUM_BUCKETS; i++) {
        node_t *current = log->buckets[i];
        while (current != NULL) {
            fprintf(f, "%s %lu %u\n", current->name, current->phone_number, current->zip_code);
            current = current->next;
        }
    }
    fclose(f);
    return 0;
}

contacts_log_t *read_contacts_log_from_text(const char *file_name) {
    //Reads the contacts from a txt file and makes a log with the contacts
    FILE *file_handle = fopen(file_name, "r");
    if (file_handle == NULL) {
        return NULL;
    }
    unsigned int num_contacts;
    fscanf(file_handle, "%u", &num_contacts);
    
    unsigned long phone_number;
    unsigned zip_code;

    char log_name[MAX_NAME_LEN];
    strncpy(log_name, file_name, strlen(file_name) - 4); 
    log_name[strlen(file_name) - 4] = '\0';
    contacts_log_t *log = create_contacts_log(log_name);
    for (int i = 0; i < num_contacts; ++i) {
        char contact_name[MAX_NAME_LEN] = {};
        fscanf(file_handle, "%s %lu %u", contact_name, &phone_number, &zip_code);
        add_contact(log, contact_name, phone_number, zip_code);
    }
    fclose(file_handle);
    return log;
}

int write_contacts_log_to_binary(const contacts_log_t *log) {
    //Writes the contacts in the log to a bin file
    char file_name[MAX_NAME_LEN + strlen(".bin")];
    strcpy(file_name, log->log_name);
    strcat(file_name, ".bin");
    FILE *f = fopen(file_name, "wb");
    if (f == NULL) {
        return -1;
    }

    fwrite(&log->size, sizeof(unsigned), 1, f);
    for (int i = 0; i < NUM_BUCKETS; i++) {
        node_t *current = log->buckets[i];
        while (current != NULL) {
            int length = strlen(current->name);
            fwrite(&length, sizeof(int), 1, f);
            fwrite(&current->name, sizeof(char), length, f);
            fwrite(&current->phone_number, sizeof(unsigned long), 1, f);
            fwrite(&current->zip_code, sizeof(unsigned), 1, f);
            current = current->next;
        }

    }
    fclose(f);
    return 0;
}

contacts_log_t *read_contacts_log_from_binary(const char *file_name) {
    //Reads the contacts from a bin file and makes a log with the contacts
    FILE *file_handle = fopen(file_name, "rb");

    if (file_handle == NULL) {
        return NULL;
    }
    char log_name[MAX_NAME_LEN];
    strncpy(log_name, file_name, strlen(file_name) - 4);
    log_name[strlen(file_name) - 4] = '\0';
    contacts_log_t *log = create_contacts_log(log_name);
    if (log == NULL) {
        fclose(file_handle);
        return NULL;
    }
    unsigned int num_contacts;
    fread(&num_contacts, sizeof(unsigned int), 1, file_handle);
    unsigned length;
    unsigned long phone_number;
    unsigned zip_code;
    for (int i = 0; i < num_contacts; ++i) {
        char contact_name[MAX_NAME_LEN] = {};
        fread(&length, sizeof(unsigned), 1, file_handle);
        fread(contact_name, sizeof(char), length, file_handle);
        fread(&phone_number, sizeof(unsigned long), 1, file_handle);
        fread(&zip_code, sizeof(unsigned), 1, file_handle);
        add_contact(log, contact_name, phone_number, zip_code);
    }
    fclose(file_handle);
    return log;
}
