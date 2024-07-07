/*
 * dyn_block_management.c
 *
 *  Created on: Sep 21, 2022
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//===========================
// PRINT MEM BLOCK LISTS:
//===========================

void print_mem_block_lists() {
	cprintf("\n=========================================\n");
	struct MemBlock* blk;
	struct MemBlock* lastBlk = NULL;
	cprintf("\nFreeMemBlocksList:\n");
	uint8 sorted = 1;
	LIST_FOREACH(blk, &FreeMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size);
		lastBlk = blk;
	}
	if (!sorted)
		cprintf("\nFreeMemBlocksList is NOT SORTED!!\n");

	lastBlk = NULL;
	cprintf("\nAllocMemBlocksList:\n");
	sorted = 1;
	LIST_FOREACH(blk, &AllocMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size);
		lastBlk = blk;
	}
	if (!sorted)
		cprintf("\nAllocMemBlocksList is NOT SORTED!!\n");
	cprintf("\n=========================================\n");

}



//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks) {
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList
	// Write your code here, remove the panic and write your code
	//panic("initialize_MemBlocksList() is not implemented yet...!!");
	LIST_INIT(&AvailableMemBlocksList);
	for (int i = 0; i < numOfBlocks; i++) {
		if (i == 0) {
			LIST_INSERT_HEAD(&AvailableMemBlocksList, &(MemBlockNodes[i]));
		} else {
			LIST_INSERT_TAIL(&AvailableMemBlocksList, &(MemBlockNodes[i]));
		}
	}
}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock *find_block(struct MemBlock_List *blockList, uint32 va) {
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block
	// Write your code here, remove the panic and write your code
	//panic("find_block() is not implemented yet...!!");
	int size = LIST_SIZE(blockList);
	struct MemBlock* element = blockList->lh_first;
	for (int i = 0; i < size; i++) {
		if (va == element->sva) {
			return element;
		}
		element = LIST_NEXT(element);
	}
	return NULL;
}

//=========================================
// [3] INSERT BLOCK IN ALLOC LIST [SORTED]:
//=========================================
void insert_sorted_allocList(struct MemBlock *blockToInsert) {
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
	// Write your code here, remove the panic and write your code
	//panic("insert_sorted_allocList() is not implemented yet...!!");
	int listSize = LIST_SIZE(&AllocMemBlocksList);
	struct MemBlock* element = LIST_FIRST(&AllocMemBlocksList);
	if (listSize == 0) {
		LIST_INSERT_HEAD(&AllocMemBlocksList, blockToInsert);
	} else if (blockToInsert->sva <= LIST_FIRST(&AllocMemBlocksList)->sva) {
		LIST_INSERT_HEAD(&AllocMemBlocksList, blockToInsert);
	} else if (blockToInsert->sva >= LIST_LAST(&AllocMemBlocksList)->sva) {
		LIST_INSERT_TAIL(&AllocMemBlocksList, blockToInsert);
	} else {
		element = LIST_FIRST(&AllocMemBlocksList);
		listSize = LIST_SIZE(&AllocMemBlocksList);
		for (int i = 0; i < listSize; i++) {
			if (blockToInsert->sva >= element->sva && blockToInsert->sva <=
			LIST_NEXT(element)->sva) {
				LIST_INSERT_AFTER(&AllocMemBlocksList, element, blockToInsert);
				break;
			}
			element = LIST_NEXT(element);
		}
	}
}

//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
struct MemBlock *alloc_block_FF(uint32 size) {
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_FF() is not implemented yet...!!");
	struct MemBlock* element = NULL;
	LIST_FOREACH(element, &FreeMemBlocksList)
	{
		if (size == element->size) {
			LIST_REMOVE(&FreeMemBlocksList, element);
			return element;
		} else if (element->size > size) {
			struct MemBlock* newBlock = AvailableMemBlocksList.lh_first;
			newBlock->sva = element->sva;
			newBlock->size = size;
			LIST_REMOVE(&AvailableMemBlocksList, newBlock);
			element->sva += size;
			element->size -= size;
			return newBlock;
		}
	}
	return NULL;
}

//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
struct MemBlock *alloc_block_BF(uint32 size) {
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_BF() is not implemented yet...!!");
	struct MemBlock* element = FreeMemBlocksList.lh_first;
	int listSize = LIST_SIZE(&FreeMemBlocksList);
	uint32 minSpace = 4294967295; // minimum space to fit the block
	int k = -1;
	for (int i = 0; i < listSize; i++) {
		if (size == element->size) {
			LIST_REMOVE(&FreeMemBlocksList, element);
			return element;
		} else if (element->size > size) {

			if (minSpace > element->size) {
				minSpace = element->size;
				k = i;
			}
		}
		element = LIST_NEXT(element);
	}
	element = FreeMemBlocksList.lh_first;
	for (int j = 0; j < listSize; j++) {
		if (j == k) {
			struct MemBlock* newBlock = AvailableMemBlocksList.lh_first;
			newBlock->sva = element->sva;
			newBlock->size = size;
			LIST_REMOVE(&AvailableMemBlocksList, newBlock);
			element->sva += size;
			element->size -= size;
			return newBlock;
		}
		element = LIST_NEXT(element);
	}
	return NULL;
}
int i = 0;
//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
struct MemBlock *alloc_block_NF(uint32 size) {
	//TODO: [PROJECT MS1 - BONUS] [DYNAMIC ALLOCATOR] alloc_block_NF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_NF() is not implemented yet...!!");
	struct MemBlock* element = FreeMemBlocksList.lh_first;
	int listSize = LIST_SIZE(&FreeMemBlocksList);
	int rst = 0;
	if (i != 0) {
		int k = 0;
		while (k != i) {
			element = LIST_NEXT(element);
			k++;
		}
	}
	for (int j = i; j < listSize; j++) {
		if (size == element->size) {
			i = j;
			LIST_REMOVE(&FreeMemBlocksList, element);
			return element;
		} else if (element->size > size) {
			struct MemBlock* newBlock = AvailableMemBlocksList.lh_first;
			newBlock->sva = element->sva;
			newBlock->size = size;
			LIST_REMOVE(&AvailableMemBlocksList, newBlock);
			element->sva += size;
			element->size -= size;
			i = j;
			return newBlock;
		}
		element = LIST_NEXT(element);
		if (j == listSize - 1 && rst == 0) {
			j = 0;
			element = FreeMemBlocksList.lh_first;
			rst = 1;
		}
	}
	return NULL;
}

//===================================================
// [8] INSERT BLOCK (SORTED WITH MERGE) IN FREE LIST:
//===================================================
void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert) {
//	cprintf(
//			"BEFORE INSERT with MERGE: insert [%x, %x)\n=====================\n",
//			blockToInsert->sva, blockToInsert->sva + blockToInsert->size);
//	print_mem_block_lists();

	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_with_merge_freeList
	// Write your code here, remove the panic and write your code
	//panic("insert_sorted_with_merge_freeList() is not implemented yet...!!");
	int listSize = LIST_SIZE(&FreeMemBlocksList);
	struct MemBlock* element = LIST_FIRST(&FreeMemBlocksList);
	if (listSize == 0) //Insert in empty list
			{
		LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
	} else if (blockToInsert->sva <= LIST_FIRST(&FreeMemBlocksList)->sva) //Insert in head
			{
		if ((blockToInsert->size + blockToInsert->sva)
				== LIST_FIRST(&FreeMemBlocksList)->sva) //merge
				{
			LIST_FIRST(&FreeMemBlocksList)->sva = blockToInsert->sva;
			LIST_FIRST(&FreeMemBlocksList)->size += blockToInsert->size;
			blockToInsert->size = 0;
			blockToInsert->sva = 0;
			LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
		} else {
			LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
		}

	} else if (blockToInsert->sva >= LIST_LAST(&FreeMemBlocksList)->sva) //Insert in tail
			{
		if ((LIST_LAST(&FreeMemBlocksList)->sva
				+ LIST_LAST(&FreeMemBlocksList)->size) == blockToInsert->sva) {
			LIST_LAST(&FreeMemBlocksList)->size += blockToInsert->size;
			blockToInsert->size = 0;
			blockToInsert->sva = 0;
			LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
		} else {
			LIST_INSERT_TAIL(&FreeMemBlocksList, blockToInsert);
		}
	} else {
		element = LIST_FIRST(&FreeMemBlocksList);
		for (int i = 0; i < listSize; i++) {
			if (blockToInsert->sva > element->sva
					&& blockToInsert->sva < LIST_NEXT(element)->sva) {
				// merge 3 blocks
				if ((element->size + element->sva) == blockToInsert->sva
						&& (blockToInsert->sva + blockToInsert->size)
								== LIST_NEXT(element)->sva) {
					element->size += (blockToInsert->size
							+ LIST_NEXT(element)->size);
					blockToInsert->size = 0;
					blockToInsert->sva = 0;
					LIST_NEXT(element)->size = 0;
					LIST_NEXT(element)->sva = 0;
					struct MemBlock* mem = LIST_NEXT(element);
					LIST_REMOVE(&FreeMemBlocksList, mem);
					LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
					LIST_INSERT_TAIL(&AvailableMemBlocksList, mem);

				}
				// merge with PREVIOUS
				else if ((element->sva + element->size) == blockToInsert->sva) {
					element->size += blockToInsert->size;
					blockToInsert->size = 0;
					blockToInsert->sva = 0;
					LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
				}
				// merge with next
				else if ((blockToInsert->sva + blockToInsert->size)
						== LIST_NEXT(element)->sva) {
					LIST_NEXT(element)->sva = blockToInsert->sva;
					LIST_NEXT(element)->size += blockToInsert->size;
					blockToInsert->size = 0;
					blockToInsert->sva = 0;
					LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
				}
				// No merge
				else {
					LIST_INSERT_AFTER(&FreeMemBlocksList, element,
							blockToInsert);
				}
				break;
			}
			element = LIST_NEXT(element);
		}
	}

//	cprintf("\nAFTER INSERT with MERGE:\n=====================\n");
//	print_mem_block_lists();

}

