#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


// MASKS: GENERIC
#define TTBL_INVALIDATE_ENTRY_MASK	0xfffffffffffffffe
#define TTBL_VALIDATE_ENTRY_MASK	0x0000000000000001
#define TTBL_IS_VALID_ENTRY_MASK	TTBL_VALIDATE_ENTRY_MASK //(addr & MASK != 0? VALID : NOT)

// LEVEL 0, 1, 2
#define TTBL_BLOCK_ENTRY_MASK		0xfffffffffffffffd
#define TTBL_TABLE_ENTRY_MASK		0x0000000000000002
#define TTBL_IS_TABLE_ENTRY_MASK	TTBL_TABLE_ENTRY_MASK //(addr & MASK != 0? TABLE : BLOCK)

// BLOCK
#define TTBL_LOWER_ATTR_MASK		0x0000000000000ffc // (addr & MASK = isolates [11:2])
#define TTBL_UPPER_ATTR_MASK		0xfff0000000000000 // (addr & MASK = isolates [63:52])
#define TTBL_L1_OA_MASK				0x0000ffffc0000000 // (addr & MASK = isolates [47:30])
#define TTBL_L2_OA_MASK				0x0000ffffffe00000 // (addr & MASK = isolates [47:21])

// BLOCK STAGE 1
#define TTBL_UPPER_PBHA_MASK		0x7800000000000000
#define TTBL_UPPER_UXN_MASK			0x0040000000000000
#define TTBL_UPPER_PXN_MASK			0x0020000000000000
#define TTBL_UPPER_CONT_MASK		0x0010000000000000
#define TTBL_UPPER_DBM_MASK			0x0008000000000000
#define TTBL_LOWER_NG_MASK			0x0000000000000800
#define TTBL_LOWER_AF_MASK			0x0000000000000400
#define TTBL_LOWER_SH_MASK			0x0000000000000300
#define TTBL_LOWER_AP_MASK			0x00000000000000c0
#define TTBL_LOWER_NS_MASK			0x0000000000000020
#define TTBL_LOWER_ATTRIDX_MASK		0x000000000000001c

// TABLE
#define TTBL_TABLE_ADDR_MASK		0x0000fffffffff000 // (addr & MASK = isolates [47:12])
#define TTBL_PXNTABLE_MASK			0x0800000000000000
#define TTBL_XNTABLE_MASK 			0x1000000000000000
#define TTBL_APTABLE_MASK			0x6000000000000000
#define TTBL_NSTABLE_MASK			0x8000000000000000




int64_t ttbl_invalidate_entry (int64_t *addr)
{
	*addr &= TTBL_INVALIDATE_ENTRY_MASK;
}

bool ttbl_is_valid_entry (int64_t addr)
{
	return (addr & TTBL_IS_VALID_ENTRY_MASK);
}

bool ttbl_is_table_entry (int64_t addr)
{
	// TODO: check that entry belongs to level 0, 1, 2
	return addr & TTBL_IS_TABLE_ENTRY_MASK;
}

bool ttbl_is_block_entry (int64_t addr)
{
	// TODO: check that entry belongs to level 0, 1, 2
	return !ttbl_is_table_entry(addr);
}

int main()
{
	/* TEST PROGRAM */
	int64_t a = 0x0000f44400004445LL;
	int64_t b = 0x0000f44400004446LL;

	// TEST VALIDATION/INVALIDATION
	/*
	if (ttbl_is_valid_entry(a))
		printf("a is valid: 0x%016llx\n", a);
	// invalidate i
	printf("Invalidate a...\n");
	ttbl_invalidate_entry(&a);
	//a &= TTBL_INVALIDATE_ENTRY_MASK;
	if (!ttbl_is_valid_entry(a))
		printf("a becomes invalid: 0x%016llx\n", a);
	// validate i
	printf("Validate a again...\n");
	a |= TTBL_VALIDATE_ENTRY_MASK;
	if (ttbl_is_valid_entry(a))
		printf("a becomes valid: 0x%016llx\n", a);
	*/

	// LEVEL 0, 1, 2

	// TEST BLOCK/TABLE
	
	if (ttbl_is_table_entry(b))
		printf("b is a table: 0x%016llx\n", b);
	// convert b into block
	printf("Convert b into a block...\n");
	b &= TTBL_BLOCK_ENTRY_MASK;
	if (ttbl_is_block_entry(b))
		printf("b becomes a block: 0x%016llx\n", b);
	// convert b into table
	printf("Convert b back into a table...\n");
	b |= TTBL_TABLE_ENTRY_MASK;
	if (!ttbl_is_block_entry(b))
		printf("b becomes a table: 0x%016llx\n", b);
	
}
