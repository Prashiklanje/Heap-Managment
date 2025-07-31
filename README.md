Heap-Management
This project implements a basic Memory Management System in C using a heap simulation. It provides functions for dynamic memory allocation and deallocation using a custom-defined heap with a metadata structure for tracking memory blocks.

Features Heap Initialization: Initializes a heap of predefined size and tracks memory blocks with metadata. Memory Allocation: Allocates a requested memory size from the heap, splitting memory blocks when necessary. Memory Deallocation: Frees previously allocated memory and merges adjacent free memory blocks. Memory Visualization: Displays the current status of the heap, showing allocated and free memory blocks.

Files allocator.c: The main C file that contains the implementation of the memory allocator. HeapSize: A macro defining the size of the heap (default: 10,000 bytes). MetaBlock: A struct representing the metadata of each memory block, which includes: size: Size of the memory block. status: Status of the block (f for free, a for allocated). next: Pointer to the next metadata block.

Functions

1. Initialize() Initializes the heap and sets up the first block of memory. This is done by assigning the size of the heap minus the size of the metadata block to the first block.
2. void *Allocate(size_t size_to_be_allocated) Allocates a block of memory of the specified size. Splits the block if the memory block is larger than the requested size. Marks the allocated block as "allocated" (a).
3. void Free(void *free_ptr) Frees the specified memory block and marks it as free (f). Calls the Merge() function to merge adjacent free memory blocks.
4. void Merge() Merges adjacent free memory blocks into one larger block to reduce fragmentation.
5. void DisplayHeap() Displays the current state of the heap, including block indices, sizes, and their status (free or allocated).
