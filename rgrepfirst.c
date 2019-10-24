#include <stdio.h>
#define MAXSIZE 4096

/**
 * You can use this recommended helper function 
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */

int strlength(char *p)
{
	int len = 0;
	while (*p++)
	{
		len++;
	}

	return len;
}
int matchChar(char *partial_line, char *pattern)
{

	int sizeL = 0, sizeP = 0;

	sizeL = strlength(partial_line);
	sizeP = strlength(pattern);

	if (*pattern + 1 == '.' || *pattern + 1 == '\0')
	{
		if (sizeL - 1 >= sizeP)
			return 1;
		else
			return 0;
	}

	for (int i = 0; i <= sizeL - sizeP; i++)
	{
		int j;

		//index i, check for pattern match
		for (j = 0; j < sizeP; j++)
		{
			if (pattern[j + 1] == '?')
			{
				char pat[MAXSIZE];
				int i = 0;
				while (pattern[i] != 0)
				{
					//set pat = 0.
					for (int k = 0; k < MAXSIZE; k++)
					{
						pat[k] = 0;
					}

					//set same char to pat array before ?
					int g = 0;
					int f = 0;

					// copy pattern to pat and if catches ?, increment f to skip it.
					while (pattern[f] != 0)
					{
						if (pattern[f] == '?')
						{
							f++;
							continue;
						}
						pat[g] = pattern[f];
						g++;
						f++;
					}
					pat[g + 1] = '\0';
					return (matchChar(partial_line, pat));
				}
			}
			if (partial_line[i + j] != pattern[j])
				break;
		}
		if (j == sizeP)
			return 1;
	}

	return 0;
}

/**
 * You may assume that all strings are properly null terminated 
 * and will not overrun the buffer set by MAXSIZE 
 *
 * Implementation of the rgrep matcher function
 */
int rgrep_matches(char *line, char *pattern)
{
	int match = 1;

	if ((*pattern == '.' || (*pattern + 1 == '.' || *pattern + 1 == '\0')) || *pattern == '+')
	{
		if (strlength(line) - 1 >= strlength(pattern))
			return 1;
		return 0;
	}
	//Basic Char Check
	if ((*pattern <= 'z' && *pattern >= 'A' && *pattern + 1 != '+'))
	{
		match = matchChar(line, pattern);

		if (match == 1)
			return 1;
	}
	//.+ check
	if (pattern[0] == '.' && pattern[1] == '+')
	{
		if (strlength(line) > 0)
			return 1;
		else
			return 0;
	}
	//char? check
	if ((pattern[1] == '?' || pattern[0] == '?') && pattern[0] != '\\')
	{
		match = rgrep_matches(line, pattern + 2);
		if (match == 1)
			return 1;
	}
	//.char check
	if (pattern[0] == '.' && (pattern[1] != '\0' || pattern[2] == '.'))
	{
		if (pattern[2] == '.')
			pattern[2] = '\0';
		match = rgrep_matches(line, pattern + 1);
		if (match == 1)
			return 1;
	}
	// \? wildcard
	if (*pattern == '\?' || *pattern == '\\')
	{
		if (*pattern == '\?')
			*pattern = '?';
		*pattern = *pattern + 1;
		return matchChar(line, pattern);
	}
	// + wild card, *pattern+1 doesn't work?
	if (*pattern != '\\' || *pattern + 1 == '+')
	{
		char pat[MAXSIZE];
		int i = 0;
		while (pattern[i] != 0)
		{
			//set pat = 0
			for (int k = 0; k < MAXSIZE; k++)
			{
				pat[k] = 0;
			}

			//set same char to pat array before +
			int g = 0;
			int f = 0;
			for (g = 0; g < i; g++)
			{
				pat[g] = pattern[0];
			}
			// copy pattern to pat and if catches +, increment f to skip it.
			while (pattern[f] != 0)
			{
				if (pattern[f] == '+')
				{
					f++;
					continue;
				}
				pat[g] = pattern[f];
				g++;
				f++;
			}
			pat[g + 1] = '\0';
			match = matchChar(line, pat);
			if (match == 1)
				return 1;
			i++;
		}

		return 0;
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
