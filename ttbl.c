#include <stdio.h>
#include <stdbool.h>
#include <ttbl_entry.h>
#include <assert.h>

/* Define mock page structure */
struct page {
	uint64_t one;
	uint64_t two;
};

/* Define structure to manage ttbl's */
struct ttbl_ctrl {
	ttbl_entry *base;
	ttbl_entry *next_ttbl;
	int count;
};

/* Define TTBR0_EL1.Baddr */
unsigned long ttbr_el1_baddr;

/* Reserve memory for pages and TTBL */
struct page pages[TTBL_L3_ENTRY_CNT];

/* TTBL bigger than one for test purposes */
ttbl_entry ttbl_array[TTBL_L1_ENTRY_CNT + TTBL_L2_ENTRY_CNT + TTBL_L3_ENTRY_CNT + 1];
//ttbl_entry ttbl_l2[TTBL_L2_ENTRY_CNT];
//ttbl_entry ttbl_l3[TTBL_L3_ENTRY_CNT];


void ttbl_init(unsigned long map_start, unsigned long map_end, bool writeable)
{
	int i, page_idx, count_pages = 0;
	unsigned long page_addr = map_start;
	struct ttbl_ctrl ttbl_c = {NULL, NULL, 0};
	ttbl_entry *ttbl;
	/* Test pointers */
	ttbl_entry *ttbl_l1, *ttbl_l2, *ttbl_l3, *ttbl_next;
	ttbl_l1 = &ttbl_array[0];
	ttbl_l2 = &ttbl_array[TTBL_L1_ENTRY_CNT];
	ttbl_l3 = &ttbl_array[TTBL_L1_ENTRY_CNT + TTBL_L2_ENTRY_CNT];
	ttbl_next = &ttbl_array[TTBL_L1_ENTRY_CNT + TTBL_L2_ENTRY_CNT + TTBL_L3_ENTRY_CNT];

	ttbl_c.base = (ttbl_entry *) ttbr_el1_baddr;
	ttbl_c.next_ttbl = ttbl_c.base + TTBL_L1_ENTRY_CNT;
	assert(ttbl_c.base == ttbl_l1);
	assert(ttbl_c.next_ttbl == ttbl_l2);

	/* Init ttbl_l1 entries to 0x0 */
	for (i = 0; i < TTBL_L1_ENTRY_CNT; i++) {
		ttbl_c.base[i] = 0x0ULL;
	}

	printf("Map start: 0x%012llx. Map end: 0x%012llx\n\n", map_start, map_end);

	while (page_addr < map_end) {
		ttbl = ttbl_c.base;

		assert(ttbl == ttbl_l1);

		/* Setup level-1 ttbl */
		/* what I would normally do is find the level-1 index of the page.
		 * I'll just default page_idx to 0 because I'm only testing on a single ttbl_l1 entry:
		 */
		page_idx = 0;
		// page_idx = (page_addr & TTBL_LEVEL1_MASK) >> TTBL_LEVEL1_SHIFT;

		if (ttbl_entry_is_valid(ttbl[page_idx])) {
			/* Find level-2 ttbl */
			//ttbl = (ttbl_entry *)(ttbl[page_idx] & TTBL_TABLE_NEXTTBL_MASK);
			ttbl = ttbl_l2;
			//printf("Level-2 ttbl at %012llx\n", ttbl);
		} else {
			/* Create new level-2 ttbl */
			for (i = 0; i < TTBL_L2_ENTRY_CNT; i++) {
				ttbl_c.next_ttbl[i] = 0x0ULL;
			}
			ttbl[page_idx] |= ((unsigned long) ttbl_l2) & TTBL_TABLE_NEXTTBL_MASK;
			ttbl[page_idx] |= (TTBL_VALIDATE_ENTRY_MASK | TTBL_TABLE_ENTRY_MASK);
			ttbl = ttbl_c.next_ttbl;
			assert(ttbl == ttbl_l2);
			ttbl_c.next_ttbl += TTBL_L2_ENTRY_CNT;
			assert(ttbl_c.next_ttbl == ttbl_l3);
			assert(ttbl_l3[1] == 0);
		}
		/* Setup level-2 ttbl */
		page_idx = 0;
		// page_idx = (page_addr & TTBL_LEVEL2_MASK) >> TTBL_LEVEL2_SHIFT;

		if (ttbl_entry_is_valid(ttbl[page_idx])) {
			/* Find level-3 ttbl */
			//ttbl = (ttbl_entry *)(ttbl[page_idx] & TTBL_TABLE_NEXTTBL_MASK);
			ttbl = ttbl_l3;
			//printf("Level-3 ttbl at %012llx\n", ttbl);
		} else {
			/* Create new level-3 ttbl */
			for (i = 0; i < TTBL_L3_ENTRY_CNT; i++) {
				ttbl_c.next_ttbl[i] = 0x0ULL;
			}
			ttbl[page_idx] |= ((unsigned long) ttbl_l3) & TTBL_TABLE_NEXTTBL_MASK;
			ttbl[page_idx] |= (TTBL_VALIDATE_ENTRY_MASK | TTBL_TABLE_ENTRY_MASK);
			ttbl = ttbl_c.next_ttbl;
			assert(ttbl == ttbl_l3);
			ttbl_c.next_ttbl += TTBL_L3_ENTRY_CNT;
			assert(ttbl_c.next_ttbl == ttbl_next);
			assert(ttbl_l3[1] == 0);
		}
		/* Setup level-3 ttbl */
		page_idx = count_pages;
		// page_idx = (page_addr & TTBL_LEVEL3_MASK) >> TTBL_LEVEL3_SHIFT;
		if (!ttbl_entry_is_valid(ttbl[page_idx])) {
			/* Setup level-3 ttbl entry */
			/* normally this would be the physical address of the page */
			// ttbl[page_idx] = va_to_pa(page_addr) & TTBL_PAGE_OA_MASK;
			ttbl[page_idx] = (page_addr << TTBL_PAGE_OA_SHIFT) & TTBL_PAGE_OA_MASK;
			ttbl[page_idx] |= TTBL_LOWER_AF_MASK;
			if (writeable)
				ttbl_entry_set_ap(&ttbl[page_idx], 0);
			else
				ttbl_entry_set_ap(&ttbl[page_idx], 2);
			ttbl[page_idx] |= (5 << TTBL_ST1_LOWER_ATTRIDX_SHIFT) & TTBL_ST1_LOWER_ATTRIDX_MASK;
			ttbl[page_idx] |= TTBL_ST1_LOWER_NS_MASK;
			ttbl[page_idx] |= (0x3 << TTBL_LOWER_SH_SHIFT);
			ttbl[page_idx] |= (TTBL_VALIDATE_ENTRY_MASK | TTBL_TABLE_ENTRY_MASK);

		}
		printf("Entry #%d maps page 0x%012llx: 0x%016llx\n", count_pages, page_addr, ttbl[page_idx]);
		count_pages++;
		page_addr += TTBL_PAGESIZE;
	}

}

int main()
{
	struct page *pages_start = &pages[0];
	struct page *pages_end = &pages[TTBL_L3_ENTRY_CNT - 1];
	pages_end++;

	ttbr_el1_baddr = (unsigned long) ttbl_array;
	printf("TTBR0_EL1.BADDR points to ttbl_l1: 0x%012llx\n", ttbr_el1_baddr);
	ttbl_init((unsigned long) pages_start, (unsigned long) pages_end, true);
}