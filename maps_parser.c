# include "maps_parser.h"

char	*resolve_path(char *path_pid)
{
	char	*path_prefix = "/proc/";
	char	*path_suffix = "/maps";
	char	*path;

	path = malloc(strlen(path_pid) + strlen(path_prefix) + strlen(path_suffix) + 1);
	strlcpy(path, path_prefix, 12);
	strlcat(path, path_pid, (strlen(path_prefix) + strlen(path_pid) + 1));
	strlcat(path, path_suffix, (strlen(path) + strlen(path_suffix) + 1));
	return (path);
}

int	main(int argc, char **argv)
{
	char	*path;
	char	buffer[1024];
	FILE	*fptr;

	path = resolve_path(argv[1]);

	fptr = fopen(path, "r");
	printf("%s\n", path);
	fgets(buffer, 1024, fptr);
	char	**arr = split_word(buffer, ' ');
//	while (fgets(buffer, 1024, fptr))
	printf("%s\n", buffer);
	
	int	i = 0;
	while (arr[i])
	{
		printf("%s\n", arr[i]);
		i++;
	}

	return 0;
	
}
