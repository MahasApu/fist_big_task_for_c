#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


// Null checks

#define NULL_CHECK(type, message)                               \
void nullCheck_##type(type* ptr) {                              \
  if (ptr == NULL) {                                            \
    printf(message);                                            \
    exit(11);                                                   \
  }                                                             \
}

NULL_CHECK(void, "Out of memory!");
NULL_CHECK(FILE, "File cannot be opened");



// Initialization of structs

#define DYN_ARR(type, dynArrForStruct)                          \
typedef struct {                                                \
   type* data;                                                  \
   size_t len;                                                  \
   size_t capacity;                                             \
} dynArrForStruct;                                              \
                                                                \
void init_##dynArrForStruct(dynArrForStruct* arr) {             \
   arr->data = NULL;                                            \
   arr->len = 0;                                                \
   arr->capacity = 1;                                           \
}                                                               \


DYN_ARR(char, String);
DYN_ARR(String*, DynArr);






// Add char to string and add string to dynArr


#define ADD_TO_DYNARR(type_arr, type_data, name_of)                                               \
                                                                                                  \
void add_##name_of(type_arr arr, type_data elem) {                                                \
	size_t index = arr->len;                                                                      \
                                                                                                  \
	if (arr->capacity == arr->len) {                                                              \
		arr->data = (type_data*)realloc(arr->data, (arr->capacity * 2) * sizeof(type_data));      \
		nullCheck_void(arr->data);                                                                \
		arr->capacity = arr->capacity * 2;                                                        \
	}                                                                                             \
	if (arr->data == NULL) {                                                                      \
		arr->data = (type_data*)malloc(sizeof(type_data));                                        \
		nullCheck_void(arr->data);                                                                \
		arr->capacity = 1;                                                                        \
	}                                                                                             \
	arr->data[index] = elem;                                                                      \
                                                                                                  \
	if (elem)                                                                                     \
		arr->len++;                                                                               \
}

ADD_TO_DYNARR(DynArr*, String*, Line);
ADD_TO_DYNARR(String*, char, Char);


// Print

void printDynArr(DynArr* lines, FILE* file_output) {
	for (size_t i = 0; i < lines->len;i++)
		fprintf(file_output, "%s", lines->data[i]->data);
}

// Check type of symbols 

int isLetter(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int isDigit(char c) {
	return (c >= '0' && c <= '9');
}


// String and char* operations

void replaceStr(String* line, const char* src) {
	size_t len = strlen(src);
	if (len + 1 > line->capacity) {
		while (line->capacity <= len) {
			line->data = (char*)realloc(line->data, (line->capacity * 2) * sizeof(char));
			nullCheck_void(line->data);
			line->capacity = line->capacity * 2;
		}
	}

	strcpy(line->data, src);
	line->len = len;
}


String* newLine() {
	String* str = (String*)malloc(sizeof(String));
	nullCheck_void(str);
	init_String(str);
	str->capacity = 1;
	return str;
}

void completeLine(String* string, DynArr* arr) {
	add_Char(string, '\0');
	add_Line(arr, string);
}


// Converting Fahrenheit to Celsius


int Pow(int num, size_t degree) {
	int result = 1;
	for (size_t i = 0; i < degree; i++) {
		result = num * result;
	}
	return result;
}

String* convertNumToChar(int number) {
	String* line = newLine();
	size_t count = 0;
	int num = number;
	if (number < 0) {
		number = number * (-1);
		add_Char(line, '-');
	}
	else if (number > 0) {
		add_Char(line, '+');
	}
	while (num) {
		num = num / 10;
		count++;
	}

	size_t i = count - 1;
	int buf;
	char num_to_char;

	while (number) {
		buf = number / Pow(10, i);
		number = number % Pow(10, i);
		num_to_char = buf + '0';
		add_Char(line, num_to_char);
		i--;
	}
	add_Char(line, '\0');
	return line;
}


char fractionalPart(float number) {
	if (number < 0) {
		number = number * (-1);
	}
	number = 10 * number;
	return (number + '0');
}

int isTempFahr(String* line) {
	if ((line->len <= 10 && line->len >= 4) &&
		(line->data[0] == '+' || line->data[0] == '-') &&
		(line->data[line->len - 2] == 't' && line->data[line->len - 1] == 'F')) {
		size_t count = 0;

		for (size_t i = 1; i <= line->len - 3;i++) {
			if (isDigit(line->data[i])) {
				count++;
			}
			else return 0;
		}
		return (count == line->len - 3);

	}
	return 0;
}

void convertFahrToCels(String* line) {
	int fahr_deg = 0;
	size_t st = line->len;
	size_t j = st - 4;
	for (size_t i = 1; i <= st - 3;i++) {
		line->data[i] = line->data[i] - '0';
		fahr_deg = fahr_deg + line->data[i] * Pow(10, j--);
	}
	if (line->data[0] == '-') {
		fahr_deg = fahr_deg * (-1);
	}
	float cels_float_deg = (float)(fahr_deg - 32.0) / 1.8; // full value of Cels
	int cels_whole_part = (fahr_deg - 32) / 1.8;           // whole part of Cels
	float cels_frac_part = cels_float_deg - cels_whole_part; // fractional part of Cels
	char rounded_frac_part = fractionalPart(cels_frac_part); // number after comma

	String* str = newLine();
	str = convertNumToChar(cels_whole_part);
	add_Char(str, ',');
	add_Char(str, rounded_frac_part);
	add_Char(str, 't');
	add_Char(str, 'C');
	add_Char(str, '\0');
	replaceStr(line, str->data);
}

void convertDegree(DynArr* lines) {
	for (size_t i = 0; i < lines->len;i++) {
		if (isTempFahr(lines->data[i]))
			convertFahrToCels(lines->data[i]);
	}
}


///// Palindrom

int isPalindrom(String* line) {

	size_t middle = line->len / 2;
	size_t len = line->len;

	for (size_t i = 0; i < line->len; i++) {
		if (isLetter(line->data[i]) == 1) {
			if (line->data[i] != line->data[len - i - 1]) {
				return 0;
			}
			return 1;
		}
	}
	return 0;
}


void palindrom(DynArr* lines) {
	for (size_t i = 0; i < lines->len;i++) {
		if (isPalindrom(lines->data[i]) == 1) {
			replaceStr(lines->data[i], "PALINDROM");
		}
	}
}

// Remove trailing whitespace and \n


void deleteLine(DynArr* arr, size_t index) {
	String** lines = arr->data;
	size_t size = arr->len;
	for (size_t i = index; i < size - 1; i++) {
		lines[i] = lines[i + 1];
	}
	arr->len--;
	arr->data = lines;
}

void remover(DynArr* arr) {
	size_t i = 0;
	size_t size = arr->len;

	while (i < size - 1) {
		if (arr->data[0]->data[0] == ' ' || arr->data[0]->data[0] == '\n') {
			deleteLine(arr, 0);
			size--;
		}
		else if (arr->data[i]->data[0] == '\n' && arr->data[i + 1]->data[0] == '\n'
			&& arr->data[i + 2]->data[0] == '\n') {
			deleteLine(arr, i + 2);
			size--;
		}

		else if ((arr->data[i]->data[0] == ' ' && arr->data[i + 1]->data[0] == ' ') ||
			(arr->data[i]->data[0] == '\n' && arr->data[i + 1]->data[0] == ' ')) {
			deleteLine(arr, i + 1);
			size--;
		}
		else i++;
	}

}


// Remove extra brackets

int isWord(String* line) {
	size_t size = line->len;
	size_t count = 0;
	for (size_t i = 0; i < size;i++) {
		if (isLetter(line->data[i]) == 1 || isDigit(line->data[i]) == 1 || line->data[i] == '+' || line->data[i] == '-') {
			count++;
		}
	}
	return (count == size);
}

void delBrackets(DynArr* arr) {
	size_t size = arr->len;


	for (size_t i = 0; i < size; i++) {
		size_t count = 0;
		size_t k = 0;
		size_t buffer1 = 0;
		size_t buffer2 = 0;
		if (arr->data[i]->data[0] == '(') {

			buffer1 = i;
			k = i;
			while (arr->data[k]->data[0] != ')' && k != size - 2) {
				if (isWord(arr->data[k])) {
					count++;
				}
				k++;
			}
			buffer2 = k - 1;
		}
		if (count == 1) {
			deleteLine(arr, buffer1);
			deleteLine(arr, buffer2);
		}
	}
	remover(arr);
}

//////// math operations


int convertCharToNum(String* line) {

	size_t size = line->len - 1;
	int number = 0;
	for (size_t i = 0; i < line->len; i++) {
		number = (line->data[i] - '0') * Pow(10, size) + number;
		size--;
	}
	return number;
}


String* convertNumToStr(int number) {
	String* line = newLine();
	int num = number;

	size_t count = 0;
	while (num) {
		num = num / 10;
		count++;
	}
	if (num == 0 && count == 0) {
		add_Char(line, '0');
		add_Char(line, '\0');
		return line;
	}

	size_t i = count - 1;
	int buf;
	char num_to_char;

	while (number) {
		buf = number / Pow(10, i);
		number = number % Pow(10, i);
		num_to_char = buf + '0';
		add_Char(line, num_to_char);
		i--;
	}
	add_Char(line, '\0');
	return line;
}


int isNumber(String* line) {

	size_t size = line->len;
	size_t count = 0;
	for (size_t i = 0; i < size; i++) {
		if (isDigit(line->data[i]) == 1)
			count++;
	}
	return (count == size);
}

int computability(DynArr* arr) {
	for (size_t i = 0; i < arr->len; i++) {
		char ch = arr->data[i]->data[0];
		if ((ch == '*' || ch == '/' || ch == '-' || ch == '+') && arr->data[i]->len == 1) {
			return 1;
		}
	}
	return 0;
}

int math_operations(char c, int buffer, int num, bool* zeroDivision) {

	switch (c) {
	case '+':
		return buffer + num;
	case '-':
		return buffer - num;
	case '*':
		return buffer * num;
	case '/':
		if (num == 0) {
			*zeroDivision = true;
			break;
		}
		else {
			return buffer / num;
		}
	default:
		exit(17);
	}
}

void findNumIndex(DynArr* arr, size_t left, size_t right, \
	size_t* indexNum, bool* idxChanged) {

	for (size_t j = left; j < right; j++) {
		if (isNumber(arr->data[j]) == 1) {
			*indexNum = j;
			*idxChanged = true;
			break;
		}
	}
}

void delUsedMathExpr(DynArr* arr, size_t indexNum, size_t differ) {
	size_t diff = differ;
	while (diff) {
		deleteLine(arr, indexNum);
		diff--;
	}
	if (arr->len == 0) {
		String* str = newLine();
		add_Line(arr, str);
	}
}

void updateMathExpr(DynArr* arr, size_t differ, bool zero_div, \
	int buff, size_t ind_to_del, size_t indexNum) {

	if (zero_div == false) {

		delUsedMathExpr(arr, indexNum, differ);
		String* k = convertNumToStr(buff);
		replaceStr(arr->data[ind_to_del], k->data);

	}
	else {
		delUsedMathExpr(arr, indexNum, differ);
		replaceStr(arr->data[indexNum], "ERROR");
	}
}

void calcOperation(DynArr* arr, size_t indexNum, size_t differ, size_t ind_to_del, int buff, \
	bool idxChanged, bool zeroDivision, size_t left, size_t right, bool* flag) {

	findNumIndex(arr, left, right, &indexNum, &idxChanged);

	if (indexNum || idxChanged) {
		*flag = true;

		buff = math_operations(arr->data[indexNum + 2]->data[0],
			convertCharToNum(arr->data[indexNum]), convertCharToNum(arr->data[indexNum + 4]),
			&zeroDivision);

		updateMathExpr(arr, differ, zeroDivision, \
			buff, ind_to_del, indexNum);
	}
	else *flag = false;

}


void calc(DynArr* arr) {
	size_t size = arr->len;
	int buff = 0;
	size_t indexNum;
	size_t Lbracket;
	size_t Rbracket;
	bool zeroDivision = false;

	while (computability(arr) != 0) {
		Lbracket = 0;
		Rbracket = 0;
		indexNum = 0;

		bool idxChanged = false;
		bool LbrChanged = false;
		bool RbrChanged = false;
		bool operationDone = false;

		for (size_t i = 0; i < size; i++) {
			if (arr->data[i]->data[0] == '(') {
				Lbracket = i;
				LbrChanged = true;
			}
			if (arr->data[i]->data[0] == ')') {
				Rbracket = i;
				RbrChanged = true;
				break;
			}
		}

		if (LbrChanged && RbrChanged) {

			calcOperation(arr, indexNum, Rbracket - Lbracket, Lbracket, buff, \
				idxChanged, zeroDivision, Lbracket, Rbracket, &operationDone);

			if (operationDone == false) break;

		}
		else {
			calcOperation(arr, indexNum, 4, indexNum, buff, \
				idxChanged, zeroDivision, 0, arr->len, &operationDone);

			if (operationDone == false) break;

		}
	}
}


// free structs

void freeDynArr(DynArr* arr) {
	for (size_t i = 0; i < arr->len; i++) {
		free(arr->data[i]->data);
		free(arr->data[i]);
	}
	free(arr->data);
	free(arr);
}

//////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
	FILE* file_input = fopen(argv[1], "r");
	nullCheck_FILE(file_input);
	FILE* file_output = fopen(argv[2], "w");
	nullCheck_FILE(file_output);

	DynArr* lines = (DynArr*)malloc(sizeof(DynArr));
	init_DynArr(lines);
	nullCheck_void(lines);

	String* str = (String*)malloc(sizeof(String));
	init_String(str);
	nullCheck_void(str);

	char c = 0;

	// Read file
	while (c != EOF) {
		c = fgetc(file_input);
		if (c == EOF) {
			if (str->len) {
				completeLine(str, lines);
			}
		}
		else if (isLetter(c) || isDigit(c)) {
			add_Char(str, c);
		}
		else if (c == '+' || c == '-') {
			if (str->len) {
				completeLine(str, lines);
			}
			str = newLine();
			add_Char(str, c);
		}
		else {
			if (str->len) {
				completeLine(str, lines);
				str = newLine();
			}
			add_Char(str, c);
			completeLine(str, lines);
			str = newLine();
		}
	}

	convertDegree(lines);
	palindrom(lines);
	remover(lines);
	delBrackets(lines);
	calc(lines);
	printDynArr(lines, file_output);
	freeDynArr(lines);

	fclose(file_input);
	fclose(file_output);
}
