/*
 * chunk_operations.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include <kern/trap/fault_handler.h>
#include <kern/disk/pagefile_manager.h>
#include "kheap.h"
#include "memory_manager.h"


/******************************/
/*[1] RAM CHUNKS MANIPULATION */
/******************************/

//===============================
// 1) CUT-PASTE PAGES IN RAM:
//===============================
//This function should cut-paste the given number of pages from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int cut_paste_pages(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 num_of_pages)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] cut_paste_pages
	// Write your code here, remove the panic and write your code
	//panic("cut_paste_pages() is not implemented yet...!!");
	uint32 temp = dest_va;
	uint32* ptr_dest_page_table = NULL;
	for(int i = 0;i <= num_of_pages;i++){
		get_page_table(page_directory, temp, &ptr_dest_page_table);
		uint32 ptx = PTX(temp);
		if(ptr_dest_page_table == NULL){
			temp += PAGE_SIZE;
			continue;
		}

		if(ptr_dest_page_table[ptx] != 0)
		{
			return -1;
		}
		temp += PAGE_SIZE;
	}
	uint32 tempSrc = source_va;
	uint32 tempDest = dest_va;
	uint32* ptr_temp_src_page_table = NULL;
	uint32* ptr_temp_dest_page_table = NULL;
	for(int i = 0;i <= num_of_pages;i++){
		get_page_table(page_directory, tempSrc, &ptr_temp_src_page_table);
		get_page_table(page_directory, tempDest, &ptr_temp_dest_page_table);
		if(ptr_temp_dest_page_table == NULL)
		{
			ptr_temp_dest_page_table = create_page_table(page_directory, tempDest);
		}
		ptr_temp_dest_page_table[PTX(tempDest)] = ptr_temp_src_page_table[PTX(tempSrc)];
		ptr_temp_src_page_table[PTX(tempSrc)] = 0;
		tempSrc += PAGE_SIZE;
		tempDest += PAGE_SIZE;
	}
	return 0;
}

//===============================
// 2) COPY-PASTE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int copy_paste_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] copy_paste_chunk
	// Write your code here, remove the panic and write your code
	//panic("copy_paste_chunk() is not implemented yet...!!");
	uint32 temp = dest_va;
	uint32 num_of_pages = ROUNDUP(size,PAGE_SIZE)/PAGE_SIZE;
	if(source_va % PAGE_SIZE != 0)
	{
		num_of_pages++;
	}
	uint32* ptr_dest_page_table = NULL;
	for(int i = 0;i < num_of_pages;i++){
		get_page_table(page_directory, temp, &ptr_dest_page_table);
		uint32 ptx = PTX(temp);
		if(ptr_dest_page_table == NULL)
		{
			temp += PAGE_SIZE;
			continue;
		}

		if(ptr_dest_page_table[ptx] != 0)
		{
			if((ptr_dest_page_table[ptx] & PERM_WRITEABLE) == 0)
			{
				return -1;
			}
		}
		temp += PAGE_SIZE;
		}
		uint32 tempSrc = source_va;
		uint32 tempDest = dest_va;
		uint32* ptr_temp_src_page_table = NULL;
		uint32* ptr_temp_dest_page_table = NULL;

		for(int i = 0;i < num_of_pages;i++)
		{
		get_page_table(page_directory, tempSrc, &ptr_temp_src_page_table);
		get_page_table(page_directory, tempDest, &ptr_temp_dest_page_table);
		if(ptr_temp_dest_page_table == NULL)
		{
			ptr_temp_dest_page_table = create_page_table(page_directory, tempDest);
		}
		if(ptr_temp_dest_page_table[PTX(tempDest)] == 0)
		{
			struct FrameInfo *ptr_dest_frame_info;
			allocate_frame(&ptr_dest_frame_info);
			uint32 entry = ptr_temp_src_page_table[PTX(tempSrc)];
			if((entry & PERM_USER) != 0)
			{
				map_frame(page_directory,ptr_dest_frame_info,tempDest, PERM_WRITEABLE | PERM_USER);
			}
			else
			{
				map_frame(page_directory,ptr_dest_frame_info,tempDest, PERM_WRITEABLE);
			}
		}
		tempSrc += PAGE_SIZE;
		tempDest += PAGE_SIZE;
		}
		for(int j=0;j < size;j++)
		{
			*(char*)(dest_va + j) = *(char*)(source_va + j);
		}
		return 0;
}

//===============================
// 3) SHARE RANGE IN RAM:
//===============================
//This function should share the given size from dest_va with the source_va
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int share_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] share_chunk
	// Write your code here, remove the panic and write your code
	//panic("share_chunk() is not implemented yet...!!");
	uint32 num_of_pages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;
	uint32 tempSrc = source_va;
	if (source_va % PAGE_SIZE != 0)
	{
	        num_of_pages++;
	}
	uint32 temp = dest_va;
	uint32* ptr_dest_page_table = NULL;
	    for (int i = 0; i <= num_of_pages; i++)
	    {
	        get_page_table(page_directory, temp, &ptr_dest_page_table);
	        uint32 ptx = PTX(temp);
	        if (ptr_dest_page_table == NULL)
	        {
	            temp += PAGE_SIZE;
	            continue;
	        }

	        if (ptr_dest_page_table[ptx] != 0)
	        {
	            return -1;
	        }
	        temp += PAGE_SIZE;
	    }
	tempSrc = source_va;
	uint32 tempDest = dest_va;
	uint32* ptr_temp_src_page_table = NULL;
	uint32* ptr_temp_dest_page_table = NULL;
	for(int i = 0;i < num_of_pages;i++)
	{
		get_page_table(page_directory, tempSrc, &ptr_temp_src_page_table);
		get_page_table(page_directory, tempDest, &ptr_temp_dest_page_table);
		if(ptr_temp_dest_page_table == NULL)
		{
			ptr_temp_dest_page_table = create_page_table(page_directory, tempDest);
		}
		struct FrameInfo* ptr_frame_info = NULL;
		ptr_frame_info = get_frame_info(page_directory,tempSrc,&ptr_temp_src_page_table);
		map_frame(page_directory,ptr_frame_info,tempDest,perms);
		tempSrc += PAGE_SIZE;
		tempDest += PAGE_SIZE;
	}
	return 0;
}

//===============================
// 4) ALLOCATE CHUNK IN RAM:
//===============================
//This function should allocate in RAM the given range [va, va+size)
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int allocate_chunk(uint32* page_directory, uint32 va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] allocate_chunk
	// Write your code here, remove the panic and write your code
	//panic("allocate_chunk() is not implemented yet...!!");

	uint32 num_of_pages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;
	uint32 tempSrc = va;

	if(va % PAGE_SIZE != 0)
	{
		num_of_pages++;
	}

	for(int i = 0;i < num_of_pages;i++)
	{
		uint32* ptr_src_page_table = NULL;
		get_page_table(page_directory, tempSrc, &ptr_src_page_table);
		uint32 ptx = PTX(tempSrc);
		if(ptr_src_page_table == NULL)
		{
			tempSrc += PAGE_SIZE;
			continue;
		}
		if(ptr_src_page_table[ptx] != 0)
		{
			return -1;
		}
		tempSrc += PAGE_SIZE;
	}

	tempSrc = va;

	for(int i = 0;i < num_of_pages;i++)
	{
		uint32* ptr_temp_src_page_table = NULL;
		get_page_table(page_directory, tempSrc, &ptr_temp_src_page_table);
		if(ptr_temp_src_page_table == NULL)
		{
			ptr_temp_src_page_table = create_page_table(page_directory, tempSrc);
		}
		struct FrameInfo *ptr_frame_info = NULL;
		ptr_frame_info = get_frame_info(page_directory, tempSrc, &ptr_temp_src_page_table);
		allocate_frame(&ptr_frame_info);
		map_frame(page_directory, ptr_frame_info, tempSrc, perms);
		ptr_frame_info->va = tempSrc;
		tempSrc += PAGE_SIZE;
	}
	return 0;

}

/*BONUS*/
//=====================================
// 5) CALCULATE ALLOCATED SPACE IN RAM:
//=====================================
void calculate_allocated_space(uint32* page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_allocated_space
	// Write your code here, remove the panic and write your code
	//panic("calculate_allocated_space() is not implemented yet...!!");
	sva = ROUNDDOWN(sva,PAGE_SIZE);
	eva = ROUNDUP(eva,PAGE_SIZE);
	struct FrameInfo *ptr;
	uint32* ptrpage;
	uint32* ptrpage2;
	uint32* ptrpage3;

	for(uint32 i=sva;i<eva;i+=PAGE_SIZE){
		ptr = get_frame_info(page_directory,i,&ptrpage2);
		if(ptr != NULL){
			(*num_pages)++;
		}
	}

	for(uint32 i=sva;i<eva;i+=PAGE_SIZE){
		int ret = get_page_table(page_directory,i,&ptrpage);
		if(ret != TABLE_NOT_EXIST && ptrpage != ptrpage3){
			(*num_tables)++;
		}
		ptrpage3 = ptrpage;

	}

}

/*BONUS*/
//=====================================
// 6) CALCULATE REQUIRED FRAMES IN RAM:
//=====================================
// calculate_required_frames:
// calculates the new allocation size required for given address+size,
// we are not interested in knowing if pages or tables actually exist in memory or the page file,
// we are interested in knowing whether they are allocated or not.
uint32 calculate_required_frames(uint32* page_directory, uint32 sva, uint32 size)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_required_frames
	// Write your code here, remove the panic and write your code
	//panic("calculate_required_frames() is not implemented yet...!!");
	uint32 pageofsva = sva >> 12;
	uint32 eva = sva + size;
	uint32 pageofeva = eva >> 12;

	uint32 numberOfpagestotal = 0;

	if(eva % PAGE_SIZE == 0){
		numberOfpagestotal = pageofeva - pageofsva;
	}
	else {
		numberOfpagestotal = pageofeva - pageofsva + 1;

	}
	sva = ROUNDDOWN(sva,PAGE_SIZE);
	struct FrameInfo *ptr;
	uint32* ptrpage;
	uint32* ptrpage2;
	uint32* ptrpage3;
	uint32 numPagesallocated = 0;
	uint32 numTablesallocated = 0;
	uint32 numberOftablesTotal = 0;
	for(uint32 i=sva;i<eva;i+=PAGE_SIZE){
			ptr = get_frame_info(page_directory,i,&ptrpage2);
			if(ptr != NULL){
				numPagesallocated++;

			}
		}
			for(uint32 i=sva;i<eva;i+=PAGE_SIZE){
				int ret = get_page_table(page_directory,i,&ptrpage);
				if(ret != TABLE_NOT_EXIST && ptrpage != ptrpage3){
					numTablesallocated++;

				}
				ptrpage3 = ptrpage;

	}

	uint32 numberOfpagesRequired = numberOfpagestotal - numPagesallocated;
	numberOftablesTotal = (pageofeva / 1024) - (pageofsva / 1024) + 1;
	if(pageofeva % 1024 == 0 && eva % PAGE_SIZE == 0){
		numberOftablesTotal--;
	}
	uint32 numberRequiredtables = numberOftablesTotal - numTablesallocated;
	return (numberOfpagesRequired + numberRequiredtables);


}

//=================================================================================//
//===========================END RAM CHUNKS MANIPULATION ==========================//
//=================================================================================//

/*******************************/
/*[2] USER CHUNKS MANIPULATION */
/*******************************/

//======================================================
/// functions used for USER HEAP (malloc, free, ...)
//======================================================

//=====================================
// 1) ALLOCATE USER MEMORY:
//=====================================
void allocate_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	// Write your code here, remove the panic and write your code
	panic("allocate_user_mem() is not implemented yet...!!");
}

//=====================================
// 2) FREE USER MEMORY:
//=====================================
void free_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3] [USER HEAP - KERNEL SIDE] free_user_mem
	// Write your code here, remove the panic and write your code
	//panic("free_user_mem() is not implemented yet...!!");

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
	uint32 sizeofWs = env_page_ws_get_size(e);
	uint32 endOfRange = virtual_address + size;
	uint32 va = virtual_address ;
	for(int j=0; j<e->page_WS_max_size; j++){
		pf_remove_env_page(e,va);
		va+=PAGE_SIZE;
		if(env_page_ws_is_entry_empty(e, j) == 1){
			continue;
		}
		else{
			uint32 envVa = env_page_ws_get_virtual_address(e, j);
			if(envVa >= virtual_address && envVa < endOfRange){
				env_page_ws_clear_entry(e, j);
			}
		}
	}
	for(int i=0; i < size/PAGE_SIZE; i++){
		bool empty = 1;
		uint32* ptr_src_page_table = NULL;
		get_page_table(e->env_page_directory, virtual_address + PAGE_SIZE*i, &ptr_src_page_table);
		unmap_frame(e->env_page_directory,virtual_address + (i*PAGE_SIZE));
		if(ptr_src_page_table == NULL){
			continue;
		}
		for(int k=0;k<1024;k++){
			if(ptr_src_page_table[k] != 0){
				empty = 0;
			}
		}
		if(empty == 1){
			unmap_frame(e->env_page_directory, (uint32)ptr_src_page_table);
			kfree (ptr_src_page_table);
			pd_clear_page_dir_entry(e->env_page_directory,virtual_address + (i * PAGE_SIZE));
		}
	}

}


//=====================================
// 2) FREE USER MEMORY (BUFFERING):
//=====================================
void __free_user_mem_with_buffering(struct Env* e, uint32 virtual_address, uint32 size)
{
	// your code is here, remove the panic and write your code
	panic("__free_user_mem_with_buffering() is not implemented yet...!!");

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Free any BUFFERED pages in the given range
	//4. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)


}

//=====================================
// 3) MOVE USER MEMORY:
//=====================================
void move_user_mem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3 - BONUS] [USER HEAP - KERNEL SIDE] move_user_mem
	//your code is here, remove the panic and write your code
	panic("move_user_mem() is not implemented yet...!!");

	// This function should move all pages from "src_virtual_address" to "dst_virtual_address"
	// with the given size
	// After finished, the src_virtual_address must no longer be accessed/exist in either page file
	// or main memory

	/**/
}

//=================================================================================//
//========================== END USER CHUNKS MANIPULATION =========================//
//=================================================================================//

