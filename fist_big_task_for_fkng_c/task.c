#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef struct {
	char* data;
	size_t len;
	size_t cap;
}String;

typedef struct {
	String** data;
	size_t len;
	size_t cap;
} DynArr;


size_t strlen(const char* str) {
	size_t l = 0;
	while (*str++) l++;
	return l;
}

char* strcpy(char* dst, const char* src) {
	char* result = dst;
	while (*dst++ = *src++);
	return result;
}

char* strchr(const char* str, int c) {
	while (*str != '\0') {
		if (*str == c) return (char*) str;
		str++;
	}
	return NULL;
}

char* strcat(char* dst, const char* src) {
	char* ptr = dst + strlen(dst);
	while (*src != '\0') {
		*ptr++ = *src++;
	}
	*ptr = '\0';
	return dst;
}

int strcmp(const char* s1, const char* s2) {
	for (size_t i = 0; ; i++) {
		if (s1[i] == '\0' && s2[i] == '\0') {
			return NULL;
		}
		if (s1[i] != s2[i]) {
			return -1;
		}
		//int elem1 = s1[i];
		//int elem2 = s2[i];
		//if (elem1 > elem2) return 1;
		//if (elem1 < elem2) return -1;
	}
}

void printStr(String* line) {
	for (size_t i = 0; i < line->len;i++) {
		printf("%c", line->data[i]);
	}
}

void printDynArr(DynArr* lines) {
	for (size_t i = 0; i < lines->len;i++) {
		printStr(lines->data[i]);
		printf('\n');
	}
}

void nullCheck(void* ptr) {
	if (ptr == NULL) {
		printf("Out of memotry!");
		exit(0);
	}
}

void nullCheckFile(FILE* file) {
	if (file == NULL) {
		printf("File opening error\n");
	}
}

#define DYN_ARR(T)	\
typedef struct {	\
	T* data;		\
	size_t len;		\
	size_t cap;		\
}dyn_arr_##T;		\
void init_dyn_arr(){\
}


void addStrToArr(String* strToAdd, DynArr* dynarr) {

}


void createArr(DynArr** retArr) {
	DynArr* newArr = NULL;
	newArr->len = 0;
	newArr->cap = 1;

	do {
		if (newArr->cap == newArr->len) {
			newArr->cap += newArr->len;
		}
		newArr->data = (int*)realloc(newArr->data, newArr->cap * sizeof(int));
		nullCheck(newArr);
		scanf("%d", &newArr[newArr->len++]);

	} while (newArr->data[newArr->len - 1] != '\0');
	*retArr = newArr;

}

void add_to_end(String** retStr) {

}

void nullCheckForFile(FILE* f) {
	if (f == NULL) {
		printf("File is empty!");
		exit(0);
	}
}


int IsLetter(char let) {
	return (let >= 'a' && let <= 'z') || (let >= 'A' && let <= 'Z');
}

int IsNumber(char num) {
	return (num >= '1' && num <= '9');
}


String* madeNew() {
	return (String*)malloc(sizeof(String));
}

int main() {

	FILE* input = fopen("test.txt", "r");
	nullCheckForFile(input);
	FILE* output = fopen("output.txt", "w");
	DynArr* newArr = (DynArr*)malloc(sizeof(DynArr));
	String* onestr = (String*)malloc(sizeof(String));

	newArr->len = 0;
	newArr->cap = 1;
	char sym;



	char ch;
	int flag_1 = 1;
	char buff = 0;
	while (flag_1) {
		ch = fgetc(sym);
		if (ch == EOF) {
			flag_1 = 0;
			if (onestr->len) {
				close(onestr, newArr);
				if (buff) {
					//AAaa+EOF
					onestr = madeNew();
					strcat(onestr, buff);
					close(onestr, newArr);
					buff = 0;
				}
			}
		}
		else if (ch == '+' || ch == '-') {
			buff = ch;
		}
		else if (IsNumber(ch) || IsLetter(ch)) {
			if (buff) {
				strcat(onestr, buff);
				buff = 0;
			}
			addTailToString(onestr, ch);
		}
		else {
			//Для знаков препинания и разделителей вместе
			if (onestr->len) {
				close(onestr, newArr);
				onestr = madeNew();
				if (buff) {
					//AAaa+
					strcat(onestr, buff);
					close(onestr, newArr);
					buff = 0;
					onestr = madeNew();
				}
			}
			else if (buff) {
				// разделитель+
				strcat(onestr, buff);
				close(onestr, newArr);
				buff = 0;
				onestr = madeNew();
			}
			strcat(onestr, ch);
			close(onestr, newArr);
			onestr = madeNew();
		}
	}



	/*while (fgetc(input) != "\0") {
		if ()
		
	}*/

	//createArr(&array, &length, &cap);
	/*printf("length = %zu\n", length);
	printf("capacity = %zu", cap);
	free(array);
	*/
	fclose(input);
	fclose(output);
}