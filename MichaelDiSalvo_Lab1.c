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
	// create our temporary argv array and token string, get starting token physical size, and set initial tokenSize and token number to 0 
	char ** argv = (char **) malloc(sizeof(void *) * physicalSize); 
	argv[0] = NULL;
	int tokenPhysicalSize = 10;
	char* token = (char *) malloc(sizeof(char)*tokenPhysicalSize);
	token[0] = '\0';
	int tokenSize = 0;
	int tokenNumber = 0;

	char ch = (char) getchar();
	while (ch != '\n')
	{
		// if we found a space 
		if (ch == ' ')
		{
			// if token is not empty, we found a valid token, add it to argv and reset/update values
			if (tokenSize > 0)
			{
				// allocate memory in argv
				argv[tokenNumber] = (char *) malloc(sizeof(char)*tokenPhysicalSize);
				// copy contents to newly allocated space 
				strcpy(argv[tokenNumber], token);
				// reset token size and null terminator, incrememnt token number, and  
				tokenSize = 0;
				tokenNumber++;
				token[0] = '\0';
				// update the null terminator for the array of strings  
				argv[tokenNumber] = NULL;
				
				// check if there are less then 2 spots left in argv, if so then re-allocate memory  
				if (tokenNumber + 1 == tokenPhysicalSize)
				{
					ReallocArgv(&argv, &physicalSize);
				}
			}
			// if token size is 0, then we just found an extra white space so skip it
			else
			{
				ch = (char) getchar();
				continue;
			}
		}
		// if we ch is not a newline or space chatacter, add it to token string 
		else
		{
			// add character, update null terminator, and increment size 
			token[tokenSize] = ch;
			token[tokenSize + 1] = '\0';
			tokenSize++;
			// if we have less than 2 spaces left in the string, reallocate memory for it 
			if (tokenSize + 2 == physicalSize)
			{
				ReallocToken(&token, &tokenPhysicalSize);
			}
		}
		// always get next character while we haven't seen a newline 
		ch = (char) getchar();
	}
	// if we have found a newline character, we must process the argv so it can be returned 
	if (ch == '\n')
	{
		// if we currently have a token that has not yet been added to argv 
		if (tokenSize > 0)
		{
			// add token to argv
			argv[tokenNumber] = (char *) malloc(sizeof(char)*tokenPhysicalSize);
			strcpy(argv[tokenNumber], token);
			argv[tokenNumber + 1] = NULL;
			tokenNumber++;
			// then assign to output argv
			*argv2 = argv;
			free(token);
			// case where exit had been typed but not yet registered 
			if (strcmp(argv[0], exitString) == 0 && argv[1] == NULL)
			{
				return 0; 
			}
			return 1;
		}
		// case were only exit was typed and it has been registered already as token
		else if (strcmp(argv[0], exitString) == 0 && tokenNumber == 1 && argv[1] == NULL)
		{
			*argv2 = argv;
			free(token);
			return 0; 
		}
		else 
		{
			*argv2 = argv;
			free(token);
			return 1; 
		}
	}
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



