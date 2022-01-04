#include <elf-parser.h>


void read_sections(FILE* fp,Elf32_Ehdr* elfhead, Elf32_Shdr* elf_shdr){


    // stand the FP pointer upon the beggining of the section header
    // and copy into the buffer  

    fseek(fp, elfhead->e_shoff,SEEK_SET);
    fread(elf_shdr,  sizeof(Elf32_Shdr), elfhead->e_shnum ,fp);

    // allocate the string table to cache the section names

    char* string_table;
    string_table = malloc( elf_shdr[elfhead->e_shstrndx].sh_size);

    fseek(fp,elf_shdr[elfhead->e_shstrndx].sh_offset,SEEK_SET);         // set the fp pointer upon the string table section
    fread(string_table,elf_shdr[elfhead->e_shstrndx].sh_size,1,fp);     // bring the table to memory 

    printf("========================\n");
    printf("************************\n");
    printf("READING SECTIONS\n" );
    printf("************************\n");
    printf("========================\n");

    unsigned int* instructions = NULL;

    for ( int i=0 ; i < elfhead->e_shnum ; i++){

        instructions = malloc (elf_shdr[i].sh_size );

        printf(" section name : %s\n",string_table + elf_shdr[i].sh_name);
        printf(" section type : %d\n", elf_shdr[i].sh_type );
        printf(" section size : %d\n",elf_shdr[i].sh_size );

        // read data from section 
        fseek(fp,elf_shdr[i].sh_offset,SEEK_SET);
        fread(instructions,elf_shdr[i].sh_size,1,fp);

        // find name 
        printf(" Dissasembly of section %s\n",string_table + elf_shdr[i].sh_name );

		printf(" Dissasembly of test size  %d\n",elf_shdr[i].sh_size);

		if(elf_shdr[i].sh_size > 20 ){

			for (unsigned int i = 0 ; i < 20; i++){
				//for (unsigned int i = 0 ; i < elf_shdr[i].sh_size; i++){
				//printf(" Dissasembly cc %d\n",elf_shdr[i].sh_size);

				//printf("==%d\n", instructions[i]);
			}

		}

        printf("\n");
        free(instructions);
    }

    free(string_table);
}



void disassemble(int32_t fd, Elf32_Ehdr eh, Elf32_Shdr* sh_table)
{
	int32_t i;
	char* sh_str;   /* section-header string-table is also a section. */
	char* buf;      /* buffer to hold contents of the .text section */

	/* Read section-header string-table */
	debug("eh.e_shstrndx = 0x%x\n", eh.e_shstrndx);
	sh_str = read_section(fd, sh_table[eh.e_shstrndx]);

	for(i=0; i<eh.e_shnum; i++) {
		if(!strcmp(".text", (sh_str + sh_table[i].sh_name))) {
			printf("Found section\t\".text\"\n");
			printf("at offset\t0x%08x\n", sh_table[i].sh_offset);
			printf("of size\t\t0x%08x\n", sh_table[i].sh_size);
			break;
		}
	}

	assert(lseek(fd, sh_table[i].sh_offset, SEEK_SET)==sh_table[i].sh_offset);
	buf = malloc(sh_table[i].sh_size);
	if(!buf) {
		printf("Failed to allocate %dbytes!!\n", sh_table[i].sh_size);
		printf("Failed to disassemble!!\n");
		return;
	}

	for (size_t i = 0; i < 20; i++)
	{
		printf("content0: %d,%x, %s", i, buf[i], buf[i]);
	}
	
	assert(read(fd, buf, sh_table[i].sh_size)==sh_table[i].sh_size);

	/* Now buf contains the instructions (4bytes each) */

}


void disassemble64(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr* sh_table)
{
	int32_t i;
	char* sh_str;   /* section-header string-table is also a section. */
	char* buf;      /* buffer to hold contents of the .text section */

	/* Read section-header string-table */
	debug("eh.e_shstrndx = 0x%x\n", eh.e_shstrndx);
	sh_str = read_section64(fd, sh_table[eh.e_shstrndx]);

	for(i=0; i<eh.e_shnum; i++) {
		if(!strcmp(".text", (sh_str + sh_table[i].sh_name))) {
			printf("Found section\t\".text\"\n");
			printf("at offset\t0x%08lx\n", sh_table[i].sh_offset);
			printf("of size\t\t0x%08lx\n", sh_table[i].sh_size);
			break;
		}
	}

	assert(lseek(fd, sh_table[i].sh_offset, SEEK_SET)==sh_table[i].sh_offset);
	buf = malloc(sh_table[i].sh_size);
	if(!buf) {
		printf("Failed to allocate %ldbytes!!\n", sh_table[i].sh_size);
		printf("Failed to disassemble!!\n");
		return;
	}
	for (size_t i = 0; i < 20; i++)
	{
		printf("content0: %d,%x, %s", i, buf[i], buf[i]);
	}
	
	assert(read(fd, buf, sh_table[i].sh_size)==sh_table[i].sh_size);


	/* Now buf contains the instructions (4bytes each) */

}
