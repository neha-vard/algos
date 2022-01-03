//============================================================================
// Name        : csv.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#define FILE_NAME "C:\\Users\\Neha\\ws\\csv\\data1.csv"

void error() {
	printf(
			"error- please enter: csv.exe \"csv file address\" \"field name\" 0/1/2 A/D\n");
	printf("sort data type:\n0 - string\n1 - int\n2 - date");
	exit(EXIT_FAILURE);
}

int compare_chars(const void *x, const void *y);

int compare_strings(const void *x, const void *y);

int compare_ints(const void *x, const void *y);

int compare_dates(const void *x, const void *y);

void quicksort(void * first_element, int num_elements, int size,
		int (*compare)(const void *, const void *));

struct cell_info {
	char *cell;
	char *row;
};

int main(int argc, char* argv[]) {

	if (argc < 5) {
		error();
	}

	char data_type = *argv[3];
	if ((data_type - '0') < 0 || (data_type - '0') > 2) {
		error();
	}

	if (strcmp(argv[4], "A") != 0 && strcmp(argv[4], "D") != 0) {
		error();
	}

	FILE *fp;
	fp = fopen(argv[1], "r+");
	if (fp == NULL) {
		error();
	}
	fseek(fp, 0L, SEEK_END);
	int file_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char *buffer = (char *) malloc(file_size + 2);
	memset(buffer, 0, file_size + 2);
	char *s_buffer = buffer;

	//read data row by row to p using fgets, count # lines
	int num_rows = -1;
	for (; (s_buffer - buffer) < file_size; num_rows++) {
		fgets(s_buffer, file_size, fp);
		s_buffer += strlen(s_buffer) + 1;
	}
	if (*(s_buffer - 2) != '\n') {
		*(s_buffer - 1) = '\n';
		s_buffer = 0;
	}

	//fill array
	cell_info *sort_data = (cell_info *) malloc((num_rows) * sizeof(cell_info));

	int sort_col;
	char *field_name = argv[2];
	char *pfield = strstr(buffer, field_name);
	if (pfield == 0) {
		printf("column not found");
		error();
	}
	s_buffer = buffer;
	for (sort_col = 0;
			(s_buffer = (char*) (memchr(s_buffer, ',', (pfield - s_buffer))))
					!= NULL; sort_col++)
		s_buffer++;
	;

	s_buffer = buffer;
	cell_info *s_data = sort_data;
	for (int i = 0; i < num_rows; i++) {
		s_buffer = (char*) memchr(s_buffer, 0, file_size) + 1;
		s_data->row = s_buffer;
		for (int i = 0; i < sort_col; i++) {
			s_buffer = (char*) memchr(s_buffer, ',', file_size) + 1;
		}
		s_data->cell = s_buffer;
		s_data++;
	}

	//quicksort

	int (*pf[3])(const void *,
			const void *) = {compare_strings, compare_ints, compare_dates};
	quicksort(sort_data, num_rows, sizeof(cell_info), pf[data_type - '0']);

	//write data back out to file
	fseek(fp, 0L, SEEK_SET);

	fprintf(fp, "%s", buffer);
	if (strcmp(argv[4], "D") == 0) {
		for (int i = num_rows - 1; i >= 0; i--) {
			if (*sort_data[i].row == '\n')
				i--;
			fprintf(fp, "%s", sort_data[i].row);
		}
	} else {
		for (int i = 0; i < num_rows; i++) {
			if (*sort_data[i].row == '\n')
				i++;
			fprintf(fp, "%s", sort_data[i].row);
		}
	}

	fclose(fp);

	return 0;
}

void quicksort(void * first_element, int num_elements, int size,
		int (*compare)(const void *, const void *)) {

	char *low = (char *) first_element;
	char *high = ((char *) first_element) + (size * (num_elements - 1));

	if (low >= high) {
		return;
	}

	void *last_element = high;

	void *partition = malloc(size);
	memcpy(partition, low, size);

	void *hole = low;
	memset(hole, 0, size);

	while (low != high) {

		while (high != low && compare((void *) high, partition) >= 0) {
			high -= size;
		}
		if (high != low) {
			memcpy(hole, high, size);
			hole = high;
			memset(hole, 0, size);
			low += size;
		}
		if (high == low) {
			memcpy(hole, partition, size);
			break;
		}

		while (low != high && compare((void *) low, partition) <= 0) {
			low += size;
		}
		if (low != high) {
			memcpy(hole, low, size);
			hole = low;
			memset(hole, 0, size);
			high -= size;
		}
		if (low == high) {
			memcpy(hole, partition, size);
			break;
		}
	}

	num_elements = (int) (((char*) hole - (char*) first_element) / size);
	quicksort(first_element, num_elements, size, compare);

	num_elements = (int) (((char*) last_element - (char*) hole) / size);
	quicksort((char*) hole + size, num_elements, size, compare);

}

int compare_chars(const void *x, const void *y) {
	char *ch1 = (char*) x;
	char *ch2 = (char*) y;
	char chx = tolower(*ch1);
	char chy = tolower(*ch2);
	if ((chx >= 'a' && chx <= 'z') || (chx >= '0' && chx <= '9')
			|| chx == ' ') {
		if ((chy >= 'a' && chy <= 'z') || (chx >= '0' && chx <= '9')
				|| chx == ' ') {
			return chx - chy;
		} else
			return 1;
	} else if ((chy >= 'a' && chy <= 'z') || (chx >= '0' && chx <= '9')
			|| chx == ' ') {
		return -1;
	} else
		return 0;
}

int compare_strings(const void *x, const void *y) {
	cell_info *s = (cell_info*) x;
	char *x2 = s->cell;
	cell_info *s2 = (cell_info*) y;
	char *y2 = s2->cell;
	char *word1 = (char*) malloc(257);
	char *word2 = (char*) malloc(257);
	int i;
	for (i = 0; *x2 != ',' && *x2 != '\n'; i++, x2++) {
		*(word1 + i) = *x2;
	}
	*(word1 + i) = *x2;
	for (i = 0; *y2 != ',' && *y2 != '\n'; i++, y2++) {
		*(word2 + i) = *y2;
	}
	*(word2 + i) = *x2;
	for (int n = 0; n + 1; n++) {
		if (word1[n] == ',' || word1[n] == '\n') {
			if (word2[n] == ',' || word2[n] == '\n') {
				return 0;
			} else
				return -1;
		} else if (word2[n] == ',' || word2[n] == '\n') {
			if (word1[n] == ',' || word1[n] == '\n') {
				return 0;
			} else
				return 1;
		} else if (compare_chars(&word1[n], &word2[n]) != 0) {
			return compare_chars(&word1[n], &word2[n]);
		}
	}
	return 2;
}

int compare_ints(const void *x, const void *y) {

	cell_info *s = (cell_info*) x;
	char *x2 = s->cell;
	cell_info *s2 = (cell_info*) y;
	char *y2 = s2->cell;
	int int1 = 0;
	for (; *x2 >= '0' && *x2 <= '9'; x2++) {
		int1 = (int1 * 10) + *x2 - '0';
	}
	int int2 = 0;
	for (; *y2 >= '0' && *y2 <= '9'; y2++) {
		int2 = (int2 * 10) + *y2 - '0';
	}
	return int1 - int2;
}

int compare_dmy(char* x, char* y) {
	int int1 = 0;
	for (; *x >= '0' && *x <= '9'; x++) {
		int1 = (int1 * 10) + *x - '0';
	}
	int int2 = 0;
	for (; *y >= '0' && *y <= '9'; y++) {
		int2 = (int2 * 10) + *y - '0';
	}
	return int1 - int2;
}

int compare_dates(const void *x, const void *y) {
	cell_info *s = (cell_info*) x;
	char *x2 = s->cell;
	cell_info *s2 = (cell_info*) y;
	char *y2 = s2->cell;

	//month
	int c1 = 0;
	for (; *x2 >= '0' && *x2 <= '9'; x2++) {
		c1 = (c1 * 10) + *x2 - '0';
	}
	x2++;
	int c2 = 0;
	for (; *y2 >= '0' && *y2 <= '9'; y2++) {
		c2 = (c2 * 10) + *y2 - '0';
	}
	y2++;
	int cmonth = c1 - c2;

	//day
	c1 = 0;
	for (; *x2 >= '0' && *x2 <= '9'; x2++) {
		c1 = (c1 * 10) + *x2 - '0';
	}
	x2++;
	c2 = 0;
	for (; *y2 >= '0' && *y2 <= '9'; y2++) {
		c2 = (c2 * 10) + *y2 - '0';
	}
	y2++;
	int cday = c1 - c2;

	//year
	c1 = 0;
	for (; *x2 >= '0' && *x2 <= '9'; x2++) {
		c1 = (c1 * 10) + *x2 - '0';
	}
	c2 = 0;
	for (; *y2 >= '0' && *y2 <= '9'; y2++) {
		c2 = (c2 * 10) + *y2 - '0';
	}
	int cyear = c1 - c2;

	if (cyear != 0)
		return cyear;
	else if (cmonth != 0)
		return cmonth;
	else
		return cday;
}
