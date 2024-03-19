#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
size_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
struct String{
	char* text;
	int length;
};
struct arg_struct{
	struct String** data;
	int* datalength;
};
void string_print(struct String string)
{
	int i = 0;
	for(i = 0; i < string.length; i++)
	{
		printf("%c", string.text[i]);
	}
}
void *threadfunction(void *arguments)
{
	struct arg_struct* args = (struct arg_struct*)arguments;
	int i = 0;
	while(1)
	{
		printf("%d of %d : ", (i + 1), *(args->datalength));	
		string_print(*(*(args->data) + i));
		if (i == (*(args->datalength)) - 1)
			i = 0;
		else
			i++;
		Sleep(1000);
	}
}
int main()
{
	int datalength = 1;	
	//Double pointer to make sure the pointer address given to the thread doesn't change!
	struct String** data;
	(*data) = (struct String*)malloc(sizeof(struct String) * datalength);
	if ((*data) == NULL)
	{
		printf("ERROR : Couldn't malloc()\n");
		return 1;
	}
	(*data)->text = malloc(sizeof(char) * 5);
	(*data)->length = 5;
	strncpy((*data)->text, "test\n", 5); 

	struct arg_struct args;
	args.data = data;
	args.datalength = &datalength;

	//Print list of strings in a seperate loop
	pthread_t thread;
	pthread_create(&thread, NULL, threadfunction, (void*)&args);

	char *lineBuffer = NULL;
	size_t lineLength = 0;
	ssize_t charCount;
	while(1)
	{
		//Wait for new input
		charCount = getline(&lineBuffer, &lineLength, stdin); 
		//Add new input to list of strings
		datalength++;	
		(*data) = realloc((*data), sizeof(struct String) * datalength);
		if ((*data) == NULL)
		{
			printf("ERROR : Couldn't realloc()\n");
			exit(1);
		}
		((*data) + datalength - 1)->text = malloc(sizeof(char) * (size_t)charCount);
		((*data) + datalength - 1)->length = (int)charCount;
		strncpy(((*data) + datalength - 1)->text, lineBuffer, (size_t)charCount); 
	}
	return 0;
}
