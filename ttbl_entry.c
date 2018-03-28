#include <stdbool.h>
#include <ttbl_entry.h>
#include <stdio.h>


void ttbl_entry_set_invalid (ttbl_entry *addr)
{
	*addr &= TTBL_INVALIDATE_ENTRY_MASK;
}

void ttbl_entry_reset (ttbl_entry *addr)
{
	*addr = 0;
	//memset(*addr, 0, sizeof(ttbl_entry));
}


/* SET ACCESS PERMISSIONS
 * For EL0 (unprivileged):
 *	0, 2 = NO ACCESS
 *	1 = R + W
 *	3 = R only
 *
 * For EL1/2/3 (privileged):
 *	0, 1 = R + W
 *	2, 3 = R only
 */
void ttbl_entry_set_ap (ttbl_entry *addr, short unsigned int ap)
{
	if (ap < 0 || ap > 3) {
		printf("ERROR: Invalid value for AP: %d", ap);
		return;
	}
	// clear AP bits
	*addr &= ~TTBL_ST1_LOWER_AP_MASK;
	// set AP bits
	*addr |= (ap << TTBL_ST1_LOWER_AP_SHIFT) & TTBL_ST1_LOWER_AP_MASK;
}

void ttbl_entry_set_table (ttbl_entry *addr)
{
}

bool ttbl_entry_is_valid (ttbl_entry addr)
{
	return (addr & TTBL_IS_VALID_ENTRY_MASK);
}

bool ttbl_entry_is_table (ttbl_entry addr)
{
	// TODO: check that entry belongs to level 0, 1, 2
	return addr & TTBL_IS_TABLE_ENTRY_MASK;
}

bool ttbl_entry_is_block (ttbl_entry addr)
{
	// TODO: check that entry belongs to level 0, 1, 2
	return !ttbl_entry_is_table(addr);
}