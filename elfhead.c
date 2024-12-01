#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <elf.h>

#define ELF_MAGIC_LEN  16
#define E_CLASS         4
#define E_ENDIAN        5
#define E_VER           6
#define E_OSABI         7
#define E_ABIVER        8

typedef struct Elf_Header_Inf {
    uint16_t type;
    uint16_t architecture;
    uint32_t ver;
} ehead_t;

void print_abi(unsigned char e_osabi);
void usage();
FILE* open_file(char *argv[]);
unsigned char get_magic(FILE *fp, unsigned char *magic);
unsigned char is_elf(unsigned char *magic);
void print_magic(const char *format, unsigned char *magic);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        return -1;
    }
    FILE *fp = open_file(argv);
    unsigned char magic[ELF_MAGIC_LEN];
    ehead_t head; 
    if (!get_magic(fp, magic)) return -2;

    print_magic("%.2x ", magic);

    fclose(fp);
}

void usage() {
    puts("Spill: View ELF header data");
    puts("usage: sp <filename>\n");
}

FILE* open_file(char *argv[]) {
    FILE *fp = fopen(argv[1], "rb");
    if (!fp) {
        if (errno == ENOENT) {
            printf("No such file: %s\n", argv[1]);
            exit(-2);
        }
        else {
            perror("Fopen:");
            exit(errno);
        }
    }
    return fp;
}

unsigned char get_magic(FILE *fp, unsigned char *magic) {
    if (!fp) return 0;
    fread(magic, sizeof(char), ELF_MAGIC_LEN, fp);
    if (!is_elf(magic)) {
        puts("Specified file is not an ELF file.");
        fclose(fp);
        exit(-3);
    }
    return 1;
}

unsigned char is_elf(unsigned char *magic) {
    char elf[4] = {0x7f, 'E', 'L', 'F'};
    for (int i = 0; i < 4; i++)
        if (magic[i] != elf[i]) return 0;
    return 1;
}

void print_magic(const char *format, unsigned char *magic) {
    if (!format) return;
    printf("ELF Identifier: ");
    for (int i = 0; i < ELF_MAGIC_LEN; i++) {
        printf(format, magic[i]);
    }
    puts("\n");

    printf("Bit Class:\t\t");
    if (magic[E_CLASS] == ELFCLASS32)
        printf("32 bit\n");
    else if (magic[E_CLASS] == ELFCLASS64)
        printf("64 bit\n");
    else printf("Invalid\n");

    printf("Endianness:\t\t");
    if (magic[E_ENDIAN] == ELFDATA2LSB)
        printf("Little Endian, 2's Complement\n");
    else if (magic[E_ENDIAN == ELFDATA2MSB])
        printf("Big Endian, 2's Complement\n");
    else printf("Unknown\n");

    printf("ELF Version:\t\t%s\n", (magic[E_VER] == EV_CURRENT) ? "Current" : "None (Invalid)" );
    print_abi(magic[E_OSABI]);
    printf("ABI Version:\t\t%hhd\n", magic[E_ABIVER]);
}

void print_abi(unsigned char e_osabi) {
    printf("OS ABI:\t\t\t");
    switch (e_osabi) {
        case ELFOSABI_NONE || ELFOSABI_SYSV:
            puts("Unix SysV");
            break;
        case ELFOSABI_HPUX:
            puts("HP-UX");
            break;
        case ELFOSABI_NETBSD:
            puts("NetBSD");
            break;
        case ELFOSABI_LINUX:
            puts("GNU ELF Extension"); 
            break;
        case ELFOSABI_SOLARIS:
            puts("Solaris");
            break;
        case ELFOSABI_IRIX:
            puts("Irix");
            break;
        case ELFOSABI_FREEBSD:
            puts("FreeBSD");
            break;
        case ELFOSABI_TRU64:
            puts("Compaq Tru64");
            break;
        case ELFOSABI_ARM:
            puts("ARM");
            break;
        case ELFOSABI_AIX:
            puts("IBM AIX");
            break;
        case ELFOSABI_ARM_AEABI:
            puts("ARM EABI");
            break;
        case ELFOSABI_OPENBSD:
            puts("OpenBSD");
            break;
        case ELFOSABI_MODESTO:
            puts("Novell Modesto");
            break;
        case ELFOSABI_STANDALONE:
            puts("Embedded/Standalone");
            break;
        default: puts("Other (unknown)");
    }
}