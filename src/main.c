#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libft.h>

#include "nm.h"
#include "elf.h"

// TODO match everything to unix style norm for practice
int check_magic_number(void* ptr)
{
	Elf32_Ehdr *header = ptr;
	if (header->e_ident[EI_MAG0] != ELFMAG0 ||
		header->e_ident[EI_MAG1] != ELFMAG1 ||
		header->e_ident[EI_MAG2] != ELFMAG2 ||
		header->e_ident[EI_MAG3] != ELFMAG3)
	{
		return 1;
	}
	return 0;
}

int display_file(char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("Error: %s\n", strerror(errno)); // TODO remove strerror ? TODO move to stderr 
		return 1;
	}
	
	// file stats
	struct stat file_stats;	
	if (fstat(fd, &file_stats))
	{
		ft_printf("Error: %s\n", strerror(errno)); // TODO remove strerror ? TODO move to stderr 
		return 1;
	}


	void* file_ptr = mmap(NULL, file_stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file_ptr == MAP_FAILED)
	{
		ft_printf("Error: %s\n", strerror(errno)); // TODO remove strerror ? TODO move to stderr  move all the error message into one function
		return 1;
	}

	if (check_magic_number(file_ptr))
	{
		ft_printf("Error: not an ELF file\n"); // TODO move to stderr  change this message
		return 1;
	}

	// set correct read function depending on endianness
	if (((Elf32_Ehdr*)file_ptr)->e_ident[EI_DATA] == ELFDATA2LSB)
	{
		read8 = read8_little;
		read16 = read16_little;
		read32 = read32_little;
		read64 = read64_little;
	}
	else if (((Elf32_Ehdr*)file_ptr)->e_ident[EI_DATA] == ELFDATA2MSB)
	{
		read8 = read8_big;
		read16 = read16_big;
		read32 = read32_big;
		read64 = read64_big;
	}
	else
	{
		ft_printf("Error: not little or big endian\n"); // TODO move to stderr TODO change message
		return 1;
	}
	

	int version = ((Elf32_Ehdr*)file_ptr)->e_ident[EI_CLASS] == ELFCLASS32 ? 0 : 1;
	int r = 0;
	if (version == 0)
		/* retval = parse_elf_32(fd); */
		;
	else if (version == 1)
		r = parse_elf_64(file_ptr);
	else
	{
		r = 1;
		ft_printf("Error: not x86 or x86_64 ABI\n"); // TODO move to stderr 
	}
	return r;
}


int main(int argc, char **argv)
{
	int retval = 0;

	char *filename;

	if (argc == 1)
	{
		ft_printf("Usage: %s <elf_file>\n", argv[0]);
		return 1;
	}

	// no options managed because I'm lazy
	for (int i = 1; i < argc; i++)
	{
		filename = argv[i];
		if (display_file(filename)) // default behaviour of nm is to count errors as return value
			retval++;
	}

	return retval;
}
