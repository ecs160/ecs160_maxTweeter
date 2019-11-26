#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 1024
#define NAME_SIZE 4

struct linkedlist {
	struct node* head;
	struct node* tail;
};

struct node {
	char* name;
	int count;
	struct node* next;
};

//add a new node to list
void linkedlist_add(struct linkedlist* list, char* str) {
	struct node* new_node = malloc(sizeof(struct node));
	new_node->name = malloc(strlen(str));
	strcpy(new_node->name, str);
	new_node->count = 1;
	new_node->next = NULL;

	if (list->head == NULL) {
		// If no other nodes exist
		list->head = new_node;
		list->tail = new_node;
	} else {
		// If other nodes exist
		list->tail->next = new_node;
		list->tail = new_node;
	}
}

//increment a counter or create new node
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

//delete the list
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
		if (str[i] == '\0') return -1;
		if (str[i] == ',') return i;
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
			buf[len] = '\0';
			linkedlist_inc(list, buf);
			return 0;
		}
		if (line[i] == ',') comma_count++;
	}
	return -1;
}

// TODO Need to ensure header is valid
int get_name_location(char* header) {
	// Must have "name" column
	int comma_count = 0;
	for (int i = 0; i < LINE_MAX - NAME_SIZE; i++) {
		if (header[i] == '\0') return -1;
		if (header[i] == ',') comma_count++;
		if (strncmp(header + i, "name", 4) == 0) return comma_count;
		if (strncmp(header + i, "\"name\"", 6) == 0) return comma_count;
	}
	return -1;
}

//Version4
//sort the list
void sortlist(struct linkedlist* list){
	int maxvalue = 0;
	char* name;
	struct node* current = list->head;

	while(current != NULL){
		struct node* max = current->next;
		struct node* prevmax = current;
		while(max != NULL){
			if(max->count > prevmax->count){
				prevmax = max;
			}
			max = max->next;
		}
		int temp = current->count;
		char* currentname;
		strcpy(currentname, current->name);
		current->count = prevmax->count;
		strcpy(current->name, prevmax->name);
		prevmax->count = temp;
		strcpy(prevmax->name, currentname);
		current = current->next;
	}
}
//Version4

int main(int argc, char** argv) {

	// Check for argument
	if (argc != 2){
		printf("There should be 2 arguments\n");
		printf("Usage: ./maxTweeter <file path>");
		return -1;
	} 

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

	while (fgets(line, LINE_MAX, csv) != NULL) {
		if (parse_line(line, list, name_loc) == -1) {
			printf("Invalid Input Format\n");
			return -1;
		}
	}

	//Version4
	sortlist(list);
	struct node* current = list->head;
	int count = 0;
	
	while(current != NULL && count < 10){
		printf("%s: %d\n" ,current->name, current->count);
		current = current->next;
		count++;
	}
	//Version4

	linkedlist_del(list);
	
	return 0;
}
