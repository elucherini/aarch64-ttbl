#include <stdio.h>
#include <stdbool.h>
#include <ttbl_entry.h>

// Define mock page structure
struct page {
	uint64_t one;
	uint64_t two;
};

// Define TTBR0_EL1.Baddr
unsigned long ttbr_el1_baddr;

// Reserve memory for pages and TTBL
struct page pages[512];

ttbl_entry ttbl_l1;
ttbl_entry ttbl_l2;
ttbl_entry ttbl_l3[512];





void ttbl_init(unsigned long map_start, unsigned long map_end)
{
	ttbl_entry *p, *entry;
	int i;
	unsigned long page_addr = map_start;
	unsigned long addr = (unsigned long) &ttbl_l1;
	p = ttbl_l3;
	ttbl_entry *ttbl = (ttbl_entry *) ttbr_el1_baddr;

	for (i = 0; i < TTBL_L1_ENTRY_CNT; i++) {

	}

	printf("Map start: 0x%012llx. Map end: 0x%012llx\n\n", map_start, map_end);

	while (page_addr < map_end) {
		
		page_addr += TTBL_PAGESIZE;
	}

	ttbl_entry_set_ap(&ttbl_l1, 1);
	//ttbl_entry_set_next_table_addr(&ttbl_l1, addr);
	printf("ttbl_l1 at %p: 0x%016llx\n", &ttbl_l1, ttbl_l1);
	addr = 0x111101110;
	ttbl_entry_set_ap(&ttbl_l2, 1);
	//ttbl_entry_set_next_table_addr(&ttbl_l2, addr);
	printf("ttbl_l2 at %p: 0x%016llx\n", &ttbl_l2, ttbl_l2);
	for (i = 0; p <= &ttbl_l3[511]; p++, i++) {
		entry = p;
		*entry = 0x0ULL;
		//printf("ttbl_l3[i] at %p, entry at %p, p at %p\n", &ttbl_l3[i], entry, p);
		ttbl_entry_set_ap(entry, 1);
		//printf("ttbl_l3 entry #%d at %p: 0x%016llx\n", i, entry, *entry);
	}

}

int main()
{
	struct page *pages_start = &pages[0];
	struct page *pages_end = &pages[511];
	pages_end++;

	ttbr_el1_baddr = 0x56490967b070;
	printf("TTBR_EL1.BADDR = 0x%012llx\n", ttbr_el1_baddr);
	ttbl_init((unsigned long) pages_start, (unsigned long) pages_end);
	//printf("ttbl_l3 is %p\n", p);
}