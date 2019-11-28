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
// TODO Check if this can distinguish invalid file correctly
int parse_line(char* line, struct linkedlist* list, int name_loc, int isquoted[]) {
	if(line == NULL){
		return -1;
	}
	if(strlen(line) > 1024){ //the length of line shouldn't exceed 1024
		return -1;
	}
	int counter = 0;
	char buf[LINE_MAX];
	char elements[LINE_MAX][LINE_MAX];
	char* p;
	p = strtok(line, ",");
	if(*(p + strlen(p) -1) == '\n'){
		*(p + strlen(p) -1) = '\0';
	}
	if(isquoted[counter] == 0){
		if(*p == '"' || *(p + strlen(p) -1) == '"'){
			return -1;
		}
	}else if(isquoted[counter] == 1){
		if(*p != '"' || *(p + strlen(p) -1) != '"'){
			return -1;
		}
	}else if(isquoted[counter] == -1){
		return -1;
	}
	counter++;

	while(p != NULL){
		p = strtok(NULL, ",");

		if(p != NULL){
			if(*(p + strlen(p) -1) == '\n'){
				*(p + strlen(p) -1) = '\0';
			}
			if(isquoted[counter] == 0){
				if(*p == '"' || *(p + strlen(p) -1) == '"'){
					return -1;
				}
				if(counter == name_loc){
					linkedlist_inc(list,p);
				}
			}else if(isquoted[counter] == 1){
				if(*p != '"' || *(p + strlen(p) -1) != '"'){
					return -1;
				}
				if(counter == name_loc){
					*(p + strlen(p) -1) = '\0';
					linkedlist_inc(list,p+1);
				}
			}else if (isquoted[counter] == -1){
				return -1;
			}
		}
		counter++;
	}
	return 0;
}

void insert_space(char* line){
	for(int i=0;i<strlen(line);i++){
		if(line[i] == ',' && line[i+1] == ','){
			line[i+1] = '\0';
			char* p = &line[i+2];
			char buf[LINE_MAX];
			strcpy(buf,p);
			strcat(line, " ,");
			strcat(line, buf);
			i++;
		}
	}
}

int get_name_location(char* header) {
	// Must have "name" column
	if(header == NULL){
		return -1;
	}
	if(strlen(header) > 1024){ //the length of header shouldn't exceed 1024
		return -1;
	}
	int comma_count = 0;
	for (int i = 0; i < LINE_MAX - NAME_SIZE; i++) {
		if (header[i] == '\0') return -1;
		if (header[i] == ',') comma_count++;
		if (strncmp(header + i, "name", 4) == 0) return comma_count;
		if (strncmp(header + i, "\"name\"", 6) == 0) return comma_count;
	}
	return -1;
}

//check if the header is valid
//TODO Check if this can distinguish invalid header correctly
int header_check(char* header, int isquoted[]){
	if(header == NULL){
		return -1;
	}
	int counter = 0;
	char columns[LINE_MAX][LINE_MAX];
	char* p;
	char headercp[LINE_MAX+100];
	strcpy(headercp,header);
	p = strtok(headercp, ",");
	if(*(p + strlen(p) -1) == '\n'){
		*(p + strlen(p) -1) = '\0';
	}
	if(*p == '"'){
		if(*(p+1) != '\0' && *(p + strlen(p) -1) == '"'){
			isquoted[counter] = 1;
		}/*else if(*(p+strlen(p) -1) == '\n' && *(p+strlen(p) -2) == '"' && strcmp(p,"\"\n") != 0){
					isquoted[counter] = 1;
		}*/else{
			return -1;
		}
	}else{
		isquoted[counter] = 0;
	}
	if(isquoted[counter] == 1 && strcmp(p,"\"\"") == 0){
		strcpy(columns[counter], "");
	}else if(isquoted[counter] == 1){
		char q[LINE_MAX];
		strcpy(q,p);
		*(q + strlen(q) -1) = '\0';
		strcpy(columns[counter],q+1);
	}else{
		strcpy(columns[counter], p);
	}
	counter++;

	while(p != NULL){
		p = strtok(NULL, ",");

		if(p != NULL){
			if(*(p + strlen(p) -1) == '\n'){
				*(p + strlen(p) -1) = '\0';
			}
			if(*p == '"'){
				if(*(p+1) != '\0' && *(p + strlen(p) -1) == '"'){
					isquoted[counter] = 1;
				}/*else if(*(p+strlen(p) -1) == '\n' && *(p+strlen(p) -2) == '"' && strcmp(p,"\"\n") != 0){
					isquoted[counter] = 1;
				}*/
				else{
					return -1;
				}
			}else{
				isquoted[counter] = 0;
			}
			if(isquoted[counter] == 1 && strcmp(p, "\"\"") == 0){
				strcpy(columns[counter], "");
			}
			else if(isquoted[counter] == 1){
				char q[LINE_MAX];
				strcpy(q,p);
				*(q + strlen(q) -1) = '\0';
				strcpy(columns[counter],q+1);
			}else{
				strcpy(columns[counter], p);
			}
		}
		counter++;
	}

	counter--;

	for(int i=0;i < counter; i++){
		char column[LINE_MAX];
		strcpy(column,columns[i]);
		for(int j=i+1; j<counter; j++){
			if(strcmp(column, columns[j]) == 0){
				return -1;
			}
		}
	}

	return 0;
}

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
		char currentname[LINE_MAX];
		strcpy(currentname, current->name);
		current->count = prevmax->count;
		strcpy(current->name, prevmax->name);
		prevmax->count = temp;
		strcpy(prevmax->name, currentname);
		current = current->next;
	}
}

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
	char line[LINE_MAX+100];
	int isquoted[LINE_MAX];
	for(int i=0;i<LINE_MAX;i++){
		isquoted[i] = -1;
	}
	fgets(line, LINE_MAX + 100, csv);
	int name_loc = get_name_location(line);
	if (name_loc == -1) {
		printf("Invalid Input Format\n");
		return -1;
	}

	if(header_check(line, isquoted) == -1){
		printf("Invalid Input Format\n");
		return -1;
	}

	// Store in some data structure (linked list for now)
	struct linkedlist* list = malloc(sizeof(struct linkedlist));
	int linecount = 1;

	while (fgets(line, LINE_MAX + 100, csv) != NULL) {
		insert_space(line);
		if (parse_line(line, list, name_loc, isquoted) == -1) {
			printf("Invalid Input Format\n");
			return -1;
		}
		linecount++;
	}

	if(linecount > 20000){ //length of the file shouldn't exceed 20000 lines
		printf("Invalid Input Format\n");
		return -1;
	}

	//sort the list
	sortlist(list);
	struct node* current = list->head;
	int count = 0;
	
	while(current != NULL && count < 10){
		printf("%s: %d\n" ,current->name, current->count);
		current = current->next;
		count++;
	}

	linkedlist_del(list);
	
	exit(0);
}
