#ifndef ELF_H
#define ELF_H

#include <stdint.h>

#define MAGIC_NUMBER 0x7f454c46 // 0x7f followed by E L F

#pragma pack(push, 1)
/* Type for a 16-bit quantity.  */
typedef uint16_t Elf32_Half;
typedef uint16_t Elf64_Half;
/* Types for signed and unsigned 32-bit quantities.  */
typedef uint32_t Elf32_Word;
typedef        int32_t  Elf32_Sword;
typedef uint32_t Elf64_Word;
typedef        int32_t  Elf64_Sword;
/* Types for signed and unsigned 64-bit quantities.  */
typedef uint64_t Elf32_Xword;
typedef        int64_t  Elf32_Sxword;
typedef uint64_t Elf64_Xword;
typedef        int64_t  Elf64_Sxword;
/* Type of addresses.  */
typedef uint32_t Elf32_Addr;
typedef uint64_t Elf64_Addr;
/* Type of file offsets.  */
typedef uint32_t Elf32_Off;
typedef uint64_t Elf64_Off;
/* Type for section indices, which are 16-bit quantities.  */
typedef uint16_t Elf32_Section;
typedef uint16_t Elf64_Section;
/* Type for version symbol information.  */
typedef Elf32_Half Elf32_Versym;
typedef Elf64_Half Elf64_Versym;

enum {
	EI_NIDENT = 16
};

typedef struct
{
  unsigned char     e_ident[EI_NIDENT];        /* Magic number and other info */
  Elf32_Half        e_type;                        /* Object file type */
  Elf32_Half        e_machine;                /* Architecture */
  Elf32_Word        e_version;                /* Object file version */
  Elf32_Addr        e_entry;                /* Entry point virtual address */
  Elf32_Off         e_phoff;                /* Program header table file offset */
  Elf32_Off         e_shoff;                /* Section header table file offset */
  Elf32_Word        e_flags;                /* Processor-specific flags */
  Elf32_Half        e_ehsize;                /* ELF header size in bytes */
  Elf32_Half        e_phentsize;                /* Program header table entry size */
  Elf32_Half        e_phnum;                /* Program header table entry count */
  Elf32_Half        e_shentsize;                /* Section header table entry size */
  Elf32_Half        e_shnum;                /* Section header table entry count */
  Elf32_Half        e_shstrndx;                /* Section header string table index */
} Elf32_Ehdr;

typedef struct
{
  unsigned char     e_ident[EI_NIDENT];        /* Magic number and other info */
  Elf64_Half        e_type;                        /* Object file type */
  Elf64_Half        e_machine;                /* Architecture */
  Elf64_Word        e_version;                /* Object file version */
  Elf64_Addr        e_entry;                /* Entry point virtual address */
  Elf64_Off         e_phoff;                /* Program header table file offset */
  Elf64_Off         e_shoff;                /* Section header table file offset */
  Elf64_Word        e_flags;                /* Processor-specific flags */
  Elf64_Half        e_ehsize;                /* ELF header size in bytes */
  Elf64_Half        e_phentsize;                /* Program header table entry size */
  Elf64_Half        e_phnum;                /* Program header table entry count */
  Elf64_Half        e_shentsize;                /* Section header table entry size */
  Elf64_Half        e_shnum;                /* Section header table entry count */
  Elf64_Half        e_shstrndx;                /* Section header string table index */
} Elf64_Ehdr;

#define SHT_SYMTAB	2
#define SHT_STRTAB	3

typedef struct elf32_shdr {
  Elf32_Word	sh_name;
  Elf32_Word	sh_type;
  Elf32_Word	sh_flags;
  Elf32_Addr	sh_addr;
  Elf32_Off	sh_offset;
  Elf32_Word	sh_size;
  Elf32_Word	sh_link;
  Elf32_Word	sh_info;
  Elf32_Word	sh_addralign;
  Elf32_Word	sh_entsize;
} Elf32_Shdr;

typedef struct elf64_shdr {
  Elf64_Word sh_name;		/* Section name, index in string tbl */
  Elf64_Word sh_type;		/* Type of section */
  Elf64_Xword sh_flags;		/* Miscellaneous section attributes */
  Elf64_Addr sh_addr;		/* Section virtual addr at execution */
  Elf64_Off sh_offset;		/* Section file offset */
  Elf64_Xword sh_size;		/* Size of section in bytes */
  Elf64_Word sh_link;		/* Index of another section */
  Elf64_Word sh_info;		/* Additional section information */
  Elf64_Xword sh_addralign;	/* Section alignment */
  Elf64_Xword sh_entsize;	/* Entry size if section holds table */
} Elf64_Shdr;

typedef struct elf32_sym{
  Elf32_Word	st_name;
  Elf32_Addr	st_value;
  Elf32_Word	st_size;
  unsigned char	st_info;
  unsigned char	st_other;
  Elf32_Half	st_shndx;
} Elf32_Sym;

typedef struct elf64_sym {
  Elf64_Word st_name;		/* Symbol name, index in string tbl */
  unsigned char	st_info;	/* Type and binding attributes */
  unsigned char	st_other;	/* No defined meaning, 0 */
  Elf64_Half st_shndx;		/* Associated section index */
  Elf64_Addr st_value;		/* Value of the symbol */
  Elf64_Xword st_size;		/* Associated symbol size */
} Elf64_Sym;


// This values are to analyse the type of the symbol, through st_info and st_other
#define STT_NOTYPE 0
#define STT_OBJECT 1
#define STT_FUNC 2
#define STT_SECTION 3
#define STT_FILE 4
#define STT_COMMON 5
#define STT_TLS 6
#define STT_LOOS 10
#define STT_HIOS 12
#define STT_LOPROC 13
#define STT_SPARC_REGISTER 13
#define STT_HIPROC 15

#define STV_DEFAULT 0
#define STV_INTERNAL 1
#define STV_HIDDEN 2
#define STV_PROTECTED 3
#define STV_EXPORTED 4
#define STV_SINGLETON 5
#define STV_ELIMINATE 6

#define STB_LOCAL 0
#define STB_GLOBAL 1
#define STB_WEAK 2
#define STB_LOOS 10
#define STB_HIOS 12
#define STB_LOPROC 13
#define STB_HIPROC 15

#pragma pack(pop)
#endif
