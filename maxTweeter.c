#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 1025
#define MAP_SIZE 2048
#define ERROR -1

struct node {
	char* name;
	int count;
	struct node* next;
}

// Returns -1 if invalid line
// Otherwise, counts the name
int parse_line(char* line) {
	// TODO Complete this
}

int get_name_location(char* header) {
	// Must have "name" column
	int comma_count = 0;
	const int NAME_SIZE = 6;
	for (int i = 0; i < LINE_MAX - NAME_SIZE; i++) {
		if (header[i] == '\0') return -1;
		if (header[i] == ',') comma_count++;
		if (strncmp(header + i, "\"name\"", NAME_SIZE)) return comma_count;
	}
	return -1;
}

int main(int argc, char** argv) {
	// Check for argument
	if (argc != 1) return ERROR;

	FILE * csv = fopen(argv[1], "r");

	// Check that file was opened
	if (!csv) return ERROR;

	// Parse header
	char line[LINE_MAX];
	fgets(line, LINE_MAX, csv);
	int name_location = get_name_location(line);
	if (name_location == -1) return ERROR;

	// TODO Store in some data structure (linked list for now)
	struct node* head = malloc(sizeof(struct node));

	// TODO Save here
	while (getc(csv) != EOF) {
		fgets(line, LINE_MAX, csv);
		if (parse_line(line) == -1) return ERROR;
	}

	// TODO Free memory

	return 0;
}
