#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <ttbl_entry.h>


void ttbl_entry_set_invalid (ttbl_entry *addr)
{
	*addr &= TTBL_INVALIDATE_ENTRY_MASK;
}

void ttbl_entry_reset (ttbl_entry *addr)
{
	*addr = 0;
	//memset(*addr, 0, sizeof(ttbl_entry));
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