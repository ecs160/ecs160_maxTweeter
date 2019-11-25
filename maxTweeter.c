#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 1024

struct linkedlist {
	struct node* head;
	struct node* tail;
}

struct node {
	char* name;
	int count;
	struct node* next;
}

void linkedlist_add(struct linkedlist* list, char* str) {
	struct node* new_node = malloc(sizeof(struct node));
	new_node->name = malloc(strlen(str));
	strcpy(new_node->name, str);
	new_node->count = 1;
	new_node->next = NULL;

	if (linkedlist->head == NULL) {
		// If no other nodes exist
		linkedlist->head = new_node;
		linkedlist->tail = new_node;
	} else {
		// If other nodes exist
		linkedlist->tail->next = new_node;
		linkedlist->tail = new_node;
	}
}

void linkedlist_inc(struct linkedlist* list, char* str) {
	struct node* current = list->head;
	while (current != NULL) {
		if (strcmp(current->name, str) == 0) {
			current->count++;
			return;
		}

		current = current->next;
	}

	// If arrives here, need to add to linkedlist
	linkedlist_add(list, str);
}

void linkedlist_del(struct linkedlist* list) {
	struct node* current = list->head;
	while (current != NULL) {
		struct node* temp = current->next;
		free(current->name);
		free(current);
		current = temp;
	}


	free(list);
}

int find_next_comma(char* str) {
	for (int i = 0; i < LINE_MAX; i++) {
		if (line[i] == '\0') return -1;
		if (line[i] == ',') return i;
	}
	return -1;
}

// Returns -1 if invalid line
// Otherwise, counts the name
// TODO Check if this works
// TODO Have to account for quotations around the word
int parse_line(char* line, struct linkedlist* list, int name_loc) {
	char buf[LINE_MAX];
	int comma_count = 0;
	for (int i = 0; i < LINE_MAX; i++) {
		if (line[i] == '\0') return -1;
		if (comma_count == name_loc) {
			int len = find_next_comma(line + i);
			if (len == -1) return -1;
			strncpy(buf, line + i, len);
			linkedlist_inc(list, buf);
		}
		if (line[i] == ',') comma_count++;
	}
}

// TODO Need to ensure header is valid
int get_name_location(char* header) {
	// Must have "name" column
	int comma_count = 0;
	for (int i = 0; i < LINE_MAX - NAME_SIZE; i++) {
		if (header[i] == '\0') return -1;
		if (header[i] == ',') comma_count++;
		if (strncmp(header + i, "name", 4)) return comma_count;
		if (strncmp(header + i, "\"name\"", 6)) return comma_count;
	}
	return -1;
}

int main(int argc, char** argv) {
	// Check for argument
	if (argc != 1) return -1;

	FILE * csv = fopen(argv[1], "r");

	// Check that file was opened
	if (!csv) { 
		printf("Invalid Input Format\n");
		return -1;
	}

	// Parse header
	char line[LINE_MAX];
	fgets(line, LINE_MAX, csv);
	int name_loc = get_name_location(line);
	if (name_loc == -1) {
		printf("Invalid Input Format\n");
		return -1;
	}

	// Store in some data structure (linked list for now)
	struct linkedlist* list = malloc(sizeof(struct linkedlist));

	while (getc(csv) != EOF) {
		fgets(line, LINE_MAX, csv);

		if (parse_line(line, name_loc, list) == -1) {
			printf("Invalid Input Format\n");
			return -1;
		}
	}

	linkedlist_del(list);

	return 0;
}
