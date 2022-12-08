#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>


// Initialization of structs

typedef struct {
	char* data;
	size_t len;
	size_t capacity;
} String;

typedef struct {
	String** data;
	size_t len;
	size_t capacity;
} DynArr;

void initDynArr(DynArr* arr) {
	arr->data = NULL;
	arr->len = 0;
	arr->capacity = 1;
}

void initStr(String* arr) {
	arr->data = NULL;
	arr->len = 0;
	arr->capacity = 1;
}

// Print

void printStr(String* line, FILE* file_output) {
	for (size_t i = 0; i < line->len;i++) {
		fprintf(file_output, "%c", line->data[i]);
	}
}

void printDynArr(DynArr* lines, FILE* file_output) {
	for (size_t i = 0; i < lines->len;i++) {
		printStr(lines->data[i], file_output);
	}
}

// Null checks

void nullCheck(void* ptr) {
	if (ptr == NULL) {
		printf("Out of memory!");
		exit(0);
	}
}

void nullCheckFile(FILE* file) {
	if (file == NULL) {
		printf("File can not be opened");
		exit(0);
	}
}

// Check type of symbols 

int isLetter(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));	
}

int isDigit(char c) {
	return (c >= '0' && c <= '9');
}

// Add char to string

void addChar(String* line, char c) {
	size_t index = line->len;

	if (line->capacity == line->len) {
		line->data = (char*)realloc(line->data, (line->capacity * 2) * sizeof(char));
		nullCheck(line->data);
		line->capacity = line->capacity * 2;
	}
	if (line->data == NULL) {
		line->data = (char*)malloc(sizeof(char));
	}
	line->data[index] = c;

	if (c)
		line->len++;
}

// Add line to dynamic array

void addLine(DynArr* lines, String* string) {
	size_t index = lines->len;

	if (lines->capacity == lines->len) {
		lines->data = (String*)realloc(lines->data, (lines->capacity * 2) * sizeof(String));
		nullCheck(lines->data);
		lines->capacity = lines->capacity * 2;
	}
	if (lines->data == NULL) {
		lines->data = (String*)malloc(sizeof(String));
	}
	lines->data[index] = string;

	if (string)
		lines->len++;
}

// String and char* operations

size_t strLen(const char* str) {
	size_t l = 0;
	while (*str++) l++;
	return l;
}

String* newLine() {
	String* str = (String*)malloc(sizeof(String));
	initStr(str);
	nullCheck(str);
	return str;
}

void completeLine(String* string, DynArr* arr) {
	addChar(string, '\0');
	addLine(arr, string);
}

void replaceStr(String* line, const char* src) {
	size_t len = strLen(src);
	if (len + 1 > line->capacity) {
		while (line->capacity <= len) {
			line->data = (char*)realloc(line->data, (line->capacity * 2) * sizeof(char));
			nullCheck(line->data);
			line->capacity = line->capacity * 2;
		}
	}
	char* dst = line->data;
	char* result = dst;
	while (*dst++ = *src++);
	line->data = result;
	line->len = len;
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
		addChar(line, '-');
	}
	else if (number > 0) {
		addChar(line, '+');
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
		addChar(line, num_to_char);
		i--;
	}
	addChar(line, '\0');
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
		}
		if (count == line->len - 3) {
			return 1;
		}
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
	addChar(str, ',');
	addChar(str, rounded_frac_part);
	addChar(str, 't');
	addChar(str, 'C');
	addChar(str, '\0');
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
		if (isLetter(line->data[i])) {
			if (line->data[i] != line->data[len - i - 1]) {
				return 0;
			}
			return 1;
		}
		return 0;
	}
	return 0;
}


void palindrom(DynArr* lines) {
	char* pal = "PALINDROM";
	for (size_t i = 0; i < lines->len;i++) {
		if (isPalindrom(lines->data[i])) {

			replaceStr(lines->data[i], pal);
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
		if (isLetter(line->data[i]) || isDigit(line->data[i])|| line->data[i] == '+' || line->data[i] == '-') {
			count++;
		}
	}
	if (count == size) {
		return 1;
	}
	return 0;
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
				//if (arr->data[k]->data[0] == ' ') {
				//	deleteLine(arr,k);
				//}
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


int math_operations(char c, int buffer, int num, FILE* file) {

	switch (c) {
	case '+':
		return buffer + num;
	case '-':
		return buffer - num;
	case '*':
		return buffer * num;
	case '/':
		if (num == 0) {
			fprintf(file, "%s", "ERROR");
			exit(0);
		}
		else {
			return buffer / num;
		}
	default:

		exit(0);
	}

}

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
		addChar(line, '0');
		addChar(line, '\0');
		return line;
	}

	size_t i = count - 1;
	int buf;
	char num_to_char;

	while (number) {
		buf = number / Pow(10, i);
		number = number % Pow(10, i);
		num_to_char = buf + '0';
		addChar(line, num_to_char);
		i--;
	}
	addChar(line, '\0');
	return line;
}


int isNumber(String* line) {

	size_t size = line->len;
	size_t count = 0;
	for (size_t i = 0; i < size; i++) {
		if (isDigit(line->data[i]))
			count++;
	}
	if (count == size) {
		return 1;
	}
	else return 0;
}

int computability(DynArr* arr) {
	for (size_t i = 0; i < arr->len; i++) {
		char* ch = arr->data[i]->data[0];
		if ((ch == '*' || ch == '/' || ch == '-' || ch == '+') && arr->data[i]->len == 1) {
			return 1;
		}
	}
	return 0;
}

void calc(DynArr* arr, FILE* file) {
	String** lines = arr->data;
	size_t size = arr->len;
	int buff = 0;

	size_t indexNum;
	size_t Lbracket;
	size_t Rbracket;


	while (computability(arr) != 0) {
		Lbracket = NULL;
		Rbracket = NULL;
		indexNum = NULL;

		for (size_t i = 0; i < size; i++) {
			if (arr->data[i]->data[0] == '(')
				Lbracket = i;
			if (arr->data[i]->data[0] == ')') {
				Rbracket = i;
				break;
			}
		}

		if (Lbracket && Rbracket) {

			for (size_t j = Lbracket; j < Rbracket; j++) {
				if (isNumber(arr->data[j])) {

					indexNum = j;
					break;
				}
			}
			if (indexNum) {
				buff = math_operations(arr->data[indexNum + 2]->data[0],
					convertCharToNum(arr->data[indexNum]), convertCharToNum(arr->data[indexNum + 4]), file);

				size_t diff = Rbracket - Lbracket;
				while (diff) {
					deleteLine(arr, Lbracket);
					diff--;
				}
				if (arr->len == 0) {
					String* str = newLine();
					addLine(arr, str);
				}
				else {
					String* k = convertNumToStr(buff);
					replaceStr(arr->data[Lbracket], k->data);
				}
			}
			else break;
		}
		else {
			for (size_t j = 0; j < arr->len; j++) {
				if (isNumber(arr->data[j])) {
					indexNum = j;
					break;
				}
			}

			if (indexNum) {
				buff = math_operations(arr->data[indexNum + 2]->data[0],
					convertCharToNum(arr->data[indexNum]), convertCharToNum(arr->data[indexNum + 4]), file);

				size_t diff = 4;
				while (diff) {
					deleteLine(arr, indexNum);
					diff--;
				}

				if (arr->len == 0) {
					String* str = newLine();
					addLine(arr, str);
				}

				else {
					String* k = convertNumToStr(buff);
					replaceStr(arr->data[indexNum], k->data);
				}
			}
			else break;
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
	nullCheckFile(file_input);
	FILE* file_output = fopen(argv[2], "w");
	nullCheckFile(file_output);

	DynArr* lines = (DynArr*)malloc(sizeof(String*));
	initDynArr(lines);
	nullCheck(lines);

	String* str = (String*)malloc(sizeof(char*));
	initStr(str);
	nullCheck(str);

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
			addChar(str, c);
		}
		else if (c == '+' || c == '-') {
			if (str->len) {
				completeLine(str, lines);
			}
			str = newLine();
			addChar(str, c);
		}
		else {
			if (str->len) {
				completeLine(str, lines);
				str = newLine();
			}
			addChar(str, c);
			completeLine(str, lines);
			str = newLine();
		}
	}

	convertDegree(lines);
	palindrom(lines);
	calc(lines, file_output);
	remover(lines);
	delBrackets(lines);

	printDynArr(lines, file_output);
	fclose(file_input);
	fclose(file_output);
	freeDynArr(lines);
}
