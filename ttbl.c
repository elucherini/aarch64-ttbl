#include <stdio.h>
#include <stdbool.h>
#include <ttbl_entry.h>

struct uint48 {
    uint64_t x:48;
} __attribute__((packed));

struct uint48 ttbr_el1_baddr;

ttbl_entry ttbl_l1;
ttbl_entry ttbl_l2;
ttbl_entry ttbl_l3[512];

void ttbl_init()
{
	ttbl_entry *p;
	p = ttbl_l3;
	printf("ttbl_l3 is %p\n", p);
	//for (; p <; p++) {
	//	printf("ttbl_l3 next entry: %p\n", p);
	//}

}

int main()
{
	ttbl_init();
	ttbr_el1_baddr.x = 0x56490967b070;
	printf("TTBR_EL1.BADDR = 0x%012llx\n", ttbr_el1_baddr.x);
	//printf("ttbl_l3 is %p\n", p);
}