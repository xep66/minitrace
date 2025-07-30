# include "maps_parser.h"

static int	wordcount(const char *str, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != c && in_word == 0)
		{
			count++;
			in_word = 1;
		}
		if (*str != c && in_word == 1)
			;
		else if (*str == c)
			in_word = 0;
		str++;
	}
	return (count);
}

static char	*get_word(const char *str, int start, int end)
{
	char	*word;
	int		i;

	word = malloc((end - start + 1) * sizeof(char));
	if (word == NULL)
		return (NULL);
	i = 0;
	while (start < end)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	**split_word(char const *str, char c)
{
	char	**split;
	int		i;
	int		j;
	int		word_start;
	int		word_count;

	word_count = wordcount(str, c);
	split = malloc((word_count + 1) * sizeof(char *));
	if (!split || !str)
		return (NULL);
	i = 0;
	j = -1;
	while (++j < word_count)
	{
		while (str[i] == c)
			i++;
		word_start = i;
		while (str[i] && str[i] != c)
			i++;
		split[j] = get_word(str, word_start, i);
		if (!split[j])
			return (NULL);
	}
	split[j] = NULL;
	return (split);
}
