#include "libft.h"

// ptr must be allocated memory
void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_mem;

	new_mem = malloc(size);
	if (!new_mem)
		return (NULL);
	ft_memcpy(new_mem, ptr, size); // THIS WILL SEGFAULT!! Gotta fix it...
	free(ptr);
	return (new_mem);
}
