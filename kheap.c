#include "kheap.h"

#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include "memory_manager.h"

//==================================================================//
//==================================================================//
//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)//
//==================================================================//
//==================================================================//

void initialize_dyn_block_system()
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] initialize_dyn_block_system
	// your code is here, remove the panic and write your code
	//kpanic_into_prompt("initialize_dyn_block_system() is not implemented yet...!!");

	//[1] Initialize two lists (AllocMemBlocksList & FreeMemBlocksList) [Hint: use LIST_INIT()]
	LIST_INIT(&AllocMemBlocksList);
	LIST_INIT(&FreeMemBlocksList);
#if STATIC_MEMBLOCK_ALLOC
	//DO NOTHING
#else
	MAX_MEM_BLOCK_CNT = (KERNEL_HEAP_MAX - KERNEL_HEAP_START) / PAGE_SIZE;
	uint32 arraySize = ROUNDUP(sizeof(struct MemBlock) * MAX_MEM_BLOCK_CNT,PAGE_SIZE);
	MemBlockNodes = ((struct MemBlock *)KERNEL_HEAP_START);
	allocate_chunk(ptr_page_directory, KERNEL_HEAP_START, arraySize, PERM_WRITEABLE);
	initialize_MemBlocksList(MAX_MEM_BLOCK_CNT);
	struct MemBlock *blockToInsert = LIST_LAST(&AvailableMemBlocksList);
	blockToInsert->size = (KERNEL_HEAP_MAX - KERNEL_HEAP_START) - arraySize;
	blockToInsert->sva = (KERNEL_HEAP_START + arraySize);
	LIST_REMOVE(&AvailableMemBlocksList, blockToInsert);
	LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);

#endif

	//[3] Initialize AvailableMemBlocksList by filling it with the MemBlockNodes
	//[4] Insert a new MemBlock with the remaining heap size into the FreeMemBlocksList
}

void * kmalloc(unsigned int size)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kmalloc
	// your code is here, remove the panic and write your code
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details
	// use "isKHeapPlacementStrategyFIRSTFIT() ..." functions to check the current strategy

	struct MemBlock * insertedBlock;
	uint32 va = ROUNDUP(size,PAGE_SIZE);
	if(isKHeapPlacementStrategyFIRSTFIT() == 1){
		insertedBlock = alloc_block_FF(va);
	}
	else if(isKHeapPlacementStrategyBESTFIT() == 1)
	{
		insertedBlock = alloc_block_BF(va);
	}
	if(insertedBlock != NULL)
	{
		allocate_chunk(ptr_page_directory,insertedBlock->sva,va,PERM_WRITEABLE);
		insert_sorted_allocList(insertedBlock);
		return (void *) insertedBlock->sva;
	}
	else
	{
		return NULL;
	}
}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kfree
	// Write your code here, remove the panic and write your code
	//panic("kfree() is not implemented yet...!!");
	uint32 va = (uint32) virtual_address;
	struct MemBlock *blockTodelete;
	blockTodelete = find_block(&AllocMemBlocksList ,va);
	if(blockTodelete == NULL)
		return;
	int num_of_pages = blockTodelete->size / PAGE_SIZE;
	for(int i=0;i<num_of_pages;i++)
		unmap_frame(ptr_page_directory,va + (i*PAGE_SIZE));
	LIST_REMOVE(&AllocMemBlocksList,blockTodelete);
	insert_sorted_with_merge_freeList(blockTodelete);
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_virtual_address
	// Write your code here, remove the panic and write your code

	//panic("kheap_virtual_address() is not implemented yet...!!");
	struct FrameInfo *ptr_temp;
	ptr_temp = to_frame_info(physical_address);
	return ptr_temp->va;

	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details
	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_physical_address
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");

	return virtual_to_physical(ptr_page_directory, virtual_address);

	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details
}


void kfreeall()
{
	panic("Not implemented!");

}

void kshrink(uint32 newSize)
{
	panic("Not implemented!");
}

void kexpand(uint32 newSize)
{
	panic("Not implemented!");
}




//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT MS2 - BONUS] [KERNEL HEAP] krealloc
	// Write your code here, remove the panic and write your code
	panic("krealloc() is not implemented yet...!!");

}


