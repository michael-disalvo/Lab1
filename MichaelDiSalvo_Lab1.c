#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int ProcessTokens(char*** argv2, int physicalSize);
void ReallocToken(char **array, int *physicalSize);
void ReallocArgv(char ***array, int *physicalSize);

char *exitString = "exit", *prompt = "[%]: ";
int main()
{
	char** argv;
	int physicalSize = 10;
	
	int running = 1;
	while(running)
	{
		printf("%s", prompt);
		running = ProcessTokens(&argv, physicalSize);

		char* ptr = argv[0];
		int i = 0;
		while(ptr != NULL)
		{
			
			printf("argv[%d]: %s\n", i, ptr);
			i++;
			ptr = argv[i];
		}
	}

	printf("Have a nice day!\n");
	free(argv);
	exit(0);
}	

int ProcessTokens(char*** argv2, int physicalSize)
{
	char ** argv = (char **) malloc(sizeof(void *) * physicalSize); 
	int tokenPhysicalSize = 10;
	char* token = (char *) malloc(sizeof(char)*tokenPhysicalSize);
	token[0] = '\0';
	int tokenSize = 0;
	int tokenNumber = 0;

	char ch = (char) getchar();
	while (ch != '\n')
	{
		//printf("found %c with token size: %d, token Number: %d \n", ch, tokenSize, tokenNumber);
		if (ch == ' ' && tokenSize > 0)
		{
			argv[tokenNumber] = (char *) malloc(sizeof(char)*tokenPhysicalSize);
			strcpy(argv[tokenNumber], token);
			tokenSize = 0;
			tokenNumber += 1;
			token[0] = '\0';

			if (tokenNumber + 1 == tokenPhysicalSize)
			{
				ReallocArgv(&argv, &physicalSize);
			}

		}
		else if (ch == ' ' && tokenSize == 0)
		{
			ch = (char) getchar();
			continue; 	
		}
		else
		{
			token[tokenSize] = ch;
			token[tokenSize + 1] = '\0';
			tokenSize++;

			if (tokenSize + 2 == physicalSize)
			{
				printf("reached max length of token\n");
				ReallocToken(&token, &tokenPhysicalSize);
			}
		}
		ch = (char) getchar();
	}

	if (ch == '\n')
	{
		if (tokenSize > 0)
		{
			argv[tokenNumber] = (char *) malloc(sizeof(char)*10);
			strcpy(argv[tokenNumber], token);
			tokenSize = 0;
			tokenNumber++;
			argv[tokenNumber] = NULL;
			token[0] = '\0';
		}
		// check if the exit string was the only token entered
		if (tokenNumber == 1 && strcmp(argv[0], exitString) == 0)
		{
			// return false
			printf("%d\n", tokenNumber);
			argv[tokenNumber] = NULL;
			free(token);
			*argv2 = argv;
			return 0;
		}

	}
	argv[tokenNumber + 1] = NULL;
	free(token);
	*argv2 = argv;
	return 1; 
}

void ReallocToken(char **array, int *physicalSize)
{
	int size = *physicalSize;
    char *newArray = (char *) malloc(sizeof(char)*(2*size));
    /*if the array is null then do not copy data over and do not free */
    if (*array != NULL){
        memcpy(newArray, *array, sizeof(char)*(size));
        free(*array);
    }

	*physicalSize = 2*size;
    *array = newArray;
}

void ReallocArgv(char ***array, int *physicalSize)
{
	int size = *physicalSize;
    char **newArray = (char **) malloc(sizeof(void *)*(2*size));
    /*if the array is null then do not copy data over and do not free */
    if (*array != NULL){
        memcpy(newArray, *array, sizeof(void *)*(size));
        free(*array);
    }

	*physicalSize = 2*size;
    *array = newArray;
}



