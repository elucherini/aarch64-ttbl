#include <stdbool.h>
#include <ttbl_entry.h>
#include <stdio.h>


void ttbl_entry_set_invalid (ttbl_entry *entry)
{
	*entry &= TTBL_INVALIDATE_ENTRY_MASK;
}

void ttbl_entry_reset (ttbl_entry *entry)
{
	*entry = 0;
	//memset(*entry, 0, sizeof(ttbl_entry));
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
void ttbl_entry_set_ap (ttbl_entry *entry, short unsigned int ap)
{
	if (ap < 0 || ap > 3) {
		printf("ERROR: Invalid value for AP: %d\n", ap);
		return;
	}
	// clear AP bits
	*entry &= ~TTBL_ST1_LOWER_AP_MASK;
	// set AP bits
	*entry |= (ap << TTBL_ST1_LOWER_AP_SHIFT) & TTBL_ST1_LOWER_AP_MASK;
}

void ttbl_entry_set_next_table_addr(ttbl_entry *entry, unsigned long int addr)
{
	if (addr < 0 || addr > (TTBL_TABLE_NEXTTBL_MASK >> TTBL_TABLE_NEXTTBL_SHIFT)) {
		printf("ERROR: Invalid value for ADDR: %d\n", addr);
		return;
	}
	// clear next table bits
	*entry &= ~TTBL_TABLE_NEXTTBL_MASK;
	// set next table bits
	*entry |= (addr << TTBL_TABLE_NEXTTBL_SHIFT) & TTBL_TABLE_NEXTTBL_MASK;
}

bool ttbl_entry_is_valid (ttbl_entry entry)
{
	return (entry & TTBL_IS_VALID_ENTRY_MASK);
}

bool ttbl_entry_is_table (ttbl_entry entry)
{
	// TODO: check that entry belongs to level 0, 1, 2
	return entry & TTBL_IS_TABLE_ENTRY_MASK;
}

bool ttbl_entry_is_block (ttbl_entry entry)
{
	// TODO: check that entry belongs to level 0, 1, 2
	return !ttbl_entry_is_table(entry);
}