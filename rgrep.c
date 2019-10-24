#include <stdio.h>
#define MAXSIZE 4096

/**
 * You can use this recommended helper function 
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */

int matches_leading(char *partial_line, char *pattern)
{
	int matches = 400;
	//if both null return 1. Reached end of line and pattern.
	if (*partial_line == '\0' && *pattern == '\0')
	{

		return 1;
	}
	//if pattern reaches end, means pattern found!
	if (*pattern == '\0')
	{

		return 1;
	}

	//If reached end of line or pattern, pattern not found.
	if ((*partial_line == '\0' && *pattern != '\0') || (*partial_line != '\0' && *pattern == '\0'))
	{

		return 0;
	}
	//if the pattern = .? or '\\', return 0
	if (*partial_line != *pattern && *pattern != '.' && *(pattern + 1) != '?' && *pattern != '\\')
	{
		return 0;
	}

	//if char and pattern match
	if (*partial_line == *pattern && *(pattern + 1) != '+' && *(pattern + 1) != '?')
	{

		//Checks if not end of line before entering
		if (*(partial_line + 1) != '\0' && *(pattern + 1) != '\0')
		{

			//Increment up
			matches = matches_leading(partial_line + 1, pattern + 1);

			if (matches == 0)
			{
				return 0;
			}
		}
		return 1;
	}

	//Checking for the .+ wildcard check!
	if (*pattern == '.' && *(pattern + 1) != '+' && *(pattern + 1) != '?')
	{
		matches = matches_leading(partial_line + 1, pattern + 1);
		if (matches == 0)
		{
			return 0;
		}
	}

	//Checking \\ pattern.

	if (*pattern == '\\')
	{
		if (*partial_line == *(pattern + 1))
		{
			matches = matches_leading(partial_line + 1, pattern + 2);
			if (matches == 1)
				return 1;
			return 0;
		}
		else
			return 0;
	}

	//Checking for char+ card.
	if (*pattern != '\\' && *(pattern + 1) == '+')
	{
		char patt[MAXSIZE];
		char *p = patt;
		int i = 0;
		while (matches != 1 && (partial_line + i) != '\0')
		{
			/* for(int k = 0; k < MAXSIZE; k++){
				*(p+k) = 0;
			} */
			int j = 0;
			int k = 0;
			for (j = 0; j < i; j++)
			{
				*(p + j) = *pattern;
			}
			while (*(pattern + k) != '\0')
			{
				//Skipping the + char if found and continue to next pattern.
				if (*(pattern + k) == '+')
				{
					k++;
					//continue;
				}
				//String copy without +
				*(p + j) = *(pattern + k);

				j++;
				k++;
			}
			*(p + j) = '\0';
			//Pass in the line and the pattern without the + char
			matches = matches_leading(partial_line, p);
			i++;
		}
		//Result of match check.
		if (matches == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	// ? check
	if (*pattern != '\\' && *(pattern + 1) == '?' && *(pattern + 1) != '+' && *(pattern + 1) != '\0')
	{
		char patt[MAXSIZE];
		char *p = patt;
		int j = 0;
		int match2 = 1;
		if (matches != 0)
		{
			//Without ?
			matches = matches_leading(partial_line, pattern + 2);
		}
		if (match2 != 0)
		{
			for (int k = 0; k < MAXSIZE; k++)
			{
				*(p + k) = 0;
			}
			*p = *pattern;
			j = 1;
			// copy char before ? then skip it.
			while (pattern[j + 1] != '\0')
			{
				*(p + j) = pattern[j + 1];
				j++;
			}
			match2 = matches_leading(partial_line, p);
		}
		if (matches == 0 && match2 == 0)
		{
			return 0;
		}
	}

	return 1;
}

/**
 * You may assume that all strings are properly null terminated 
 * and will not overrun the buffer set by MAXSIZE 
 *
 * Implementation of the rgrep matcher function
 */

int rgrep_matches(char *line, char *pattern)
{

	if (*pattern == '\0')
		return 1;

	//looking for partial pattern
	int i = 0;
	int match;
	char p[MAXSIZE];
	char *partial = p;

	while (*(line + i + 1) != '\0')
	{
		//Checks if \\ is only pattern.
		if (*pattern == '\\' && *(pattern + 2) == '\0')
		{
			if (*(line + i) == *(pattern + 1))
			{
				return 1;
			}
			else
			{
				i++;
				continue;
			}
		}
		if (*(line + i) == *pattern || *pattern == '.')
		{

			int k = 0;

			while (*(line + i + 1) != '\0')
			{
				*(partial + k) = *(line + i);
				i++;
				k++;
			}
			*(partial + k + 1) = '\0';
			match = matches_leading(partial, pattern);
			if (match == 1)
			{
				return 1;
			}
		}
		i++;
	}
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <PATTERN>\n", argv[0]);
		return 2;
	}

	/* we're not going to worry about long lines */
	char buf[MAXSIZE];

	while (!feof(stdin) && !ferror(stdin))
	{
		if (!fgets(buf, sizeof(buf), stdin))
		{
			break;
		}
		if (rgrep_matches(buf, argv[1]))
		{
			fputs(buf, stdout);
			fflush(stdout);
		}
	}

	if (ferror(stdin))
	{
		perror(argv[0]);
		return 1;
	}

	return 0;
}
