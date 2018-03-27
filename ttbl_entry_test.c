#include <stdio.h>
#include <stdbool.h>
#include <ttbl_entry.h>

int main()
{
	/* TEST PROGRAM */
	ttbl_entry a = 0x0000f44400004445LL;
	ttbl_entry b = 0x0000f44400004446LL;

	// TEST VALIDATION/INVALIDATION
	/*
	if (ttbl_entry_is_valid(a))
		printf("a is valid: 0x%016llx\n", a);
	// invalidate i
	printf("Invalidate a...\n");
	ttbl_entry_set_invalid(&a);
	//a &= TTBL_INVALIDATE_ENTRY_MASK;
	if (!ttbl_entry_is_valid(a))
		printf("a becomes invalid: 0x%016llx\n", a);
	// validate i
	printf("Validate a again...\n");
	a |= TTBL_VALIDATE_ENTRY_MASK;
	if (ttbl_entry_is_valid(a))
		printf("a becomes valid: 0x%016llx\n", a);
	*/

	// TEST BLOCK/TABLE
	
	if (ttbl_entry_is_table(b))
		printf("b is a table: 0x%016llx\n", b);
	// convert b into block
	printf("Convert b into a block...\n");
	b &= TTBL_BLOCK_ENTRY_MASK;
	if (ttbl_entry_is_block(b))
		printf("b becomes a block: 0x%016llx\n", b);
	// convert b into table
	printf("Convert b back into a table...\n");
	b |= TTBL_TABLE_ENTRY_MASK;
	if (!ttbl_entry_is_block(b))
		printf("b becomes a table: 0x%016llx\n", b);
	// reset entry
	ttbl_entry_reset(&b);
	printf("Resetting b...\n");
	printf("b is now: 0x%016llx\n", b);
}
