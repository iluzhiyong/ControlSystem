/*****************************************************************************
 * HAlloc.h
 ***************************************************************************** 
 *
 * Project:     HALCON/libhalcon
 * Description: Memory allocation
 *
 * (c) 1996-2009 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 * 
 * $Revision: 1.61 $
 * $Date: 2010/06/25 11:35:26 $
 *
 */

#ifndef HALLOC_H
#define HALLOC_H


#if defined(__cplusplus)
extern "C" {
#endif

/*===========================================================================*/
/*                                  Defines                                  */
/*===========================================================================*/
#define H_LOCAL_ALLOC  -111                    /* AllocLevel for local. mem. */
#define H_GLOBAL_ALLOC -112                    /* use global AllocLevel      */

/* factor for memory allocation with HNewSpace():                            
   HInitMem()          -> 2*H_NEW_SPACE_FAC*MAX_SAVE
   else (HXAlloc())    ->   H_NEW_SPACE_FAC*MAX_SAVE                         */
#define H_NEW_SPACE_FAC   (1<<13) /* results in 1 MB FSP blocks */
/* maximal size of elements (in alignment units) to be put into cache:       */
#ifndef MAX_SAVE
#  define MAX_SAVE        128
#endif

/*===========================================================================*/
/*                               Data Structures                             */
/*===========================================================================*/
/* table element for stack management:                                       */
typedef struct 
{ 
  VOIDP          data;      /* pointer to stored data of a stack element   */
  MACHINE_WORD   *next;     /* pointer to next free memory area            */
  VOIDP          last;      /* pointer to last allocated area              */
  MACHINE_WORD   size;      /* size of memory within this stack element    */
  MACHINE_WORD   free;      /* free memory within stack element            */
} MemTElt;

typedef struct MEM_TT_EL_SAVE
{ 
  VOIDP                 *data;      /* memory area to manage               */
  MACHINE_WORD          size;       /* size of memory                      */
  struct MEM_TT_EL_SAVE *next;      /* next element in list                */
} MemTEltSave;
  

/* element of global memory list, needed if data should be collected to 
 * test and check allocated memory*/
typedef struct MEM_GLOBAL_ENTRY
{ 
  MACHINE_WORD            magic;    /* unlikely address: -1 */
  MACHINE_WORD            *block;   /* start address of allocated memory   */
  MACHINE_WORD            *data;    /* data start address                  */
  MACHINE_WORD            size;     /* size of data                        */
  HBOOL                   cached;   /* was memory cached or allocated by 
                                     * malloc?                             */
  char                    *file;    /* file of allocation                  */
  MACHINE_WORD            line;     /* line of code of allocation          */
  INT                     level;    /* level of allocation                 */
  struct MEM_GLOBAL_ENTRY *succ;    /* next entry in list                  */
  struct MEM_GLOBAL_ENTRY *pred;    /* previous entry in list              */
} GlbMemEntry; 



/* memory management handle (with instance/thread-specific content)        */
typedef struct MEM_HANDLE 
{
  /*------------------------------ management of stack memory ---------*/
  MemTElt        *MemTTable;    /* management list for stack memory    */
  INT            MemTTLength;   /* current length of management list   */
  INT            MemTStack;     /* current stack depth                 */
  INT4           MemTmpCount;   /* num. of allocated stack mem. blocks */
  HBOOL          MemTmpInit;    /* must we init. the stack management? */
  /*------------------------------ management of local memory ---------*/
  MACHINE_WORD   *MemLocalList; /* management list for local memory    */
  H_pthread_mutex_t MutexLocalList; /* mutex variable for MemLocalList */
  INT4           MemLocalCount; /* num. of allocated local mem. blocks */
  /*------------------------------ management of global memory --------*/
  VOIDP          *GlbMemCache;  /* holds a cache for global 
                                 * memory locally */
  VOIDP          BlockCache;    /* holds a larger memory blocks */
} MEMHANDLE;
typedef MEMHANDLE *MemHandle;

/*===========================================================================*/
/*                          Prototypes of Functions                          */
/*===========================================================================*/
/*---------------------------------------------------------------------------*/
/*                          Test Memory Allocation                           */
/*---------------------------------------------------------------------------*/
#define ALLOC_ENTRY      (- 2) /* offset of allocation flags: every memory 
                                * block must have this entry at this offset 
                                * position */


#define HMEMBITtemp   (1<<0)
#define HMEMBITlocal  (1<<1)
#define HMEMBITglobal (1<<2)
#define HMEMBITcheck  (1<<3)
#define HMEMBITcached (1<<4)
#define HMEMBITrl     (1<<5)
#define HMEMBITrlnum  (1<<6)

#define HAllocBit(MEM, BIT)                             \
  (((MACHINE_WORD*)(MEM))[ALLOC_ENTRY] & HMEMBIT ## BIT) 

#define HSetAllocBit(MEM, BIT)                      \
{                                                   \
  ((MACHINE_WORD*)(MEM))[ALLOC_ENTRY] |= HMEMBIT ## BIT; \
}

#define HClearAllocBit(MEM, BIT)                    \
{                                                   \
  ((MACHINE_WORD*)(MEM))[ALLOC_ENTRY] &= ~HMEMBIT ## BIT; \
}

#define  HAllocEntry(MEM)                       \
  (((MACHINE_WORD*)(MEM))[ALLOC_ENTRY])

/* HXTestPtrRange - tests memory (address) allocated with HAlloc/HAllocLocal */
extern HLibExport Herror HXTestPtrRange(void *ptr, const char *file, 
                                        INT4_8 line,HBOOL do_print);

/* HXTestPtr - tests memory (address+size) allocated with HAlloc/HAllocLocal */
extern HLibExport Herror HXTestPtr(void *ptr, const char *file, INT4_8 line);

/* HTestMem - tests all the memory currently allocated by HAlloc/HAllocLocal */
extern HLibExport Herror HTestMem (void);



/*---------------------------------------------------------------------------*/
/*                 Auxiliary Functions for Memory Management                 */
/*---------------------------------------------------------------------------*/
extern HLibExport HBOOL  HTraceMemory; /* switch if memory statistics 
                             * should be collected (do not set this directly)*/

/* set HMemoryTrace Flag to TRUE (efficient) / FALSE (debug information 
 * available)
 * NOTE: this function is not thread save and has to be used exclusively     */
extern HLibExport Herror HSetTraceMemory(Hproc_handle proc_handle,
                                         HBOOL trace_memory);
extern HLibExport Herror HGetTraceMemory(Hproc_handle proc_handle,
                                         HBOOL *trace_memory);
/* Switch local memory warnings (for debug purpose) */
extern void  HSetLocalMemWarnings(HBOOL state);
extern HBOOL HGetLocalMemWarnings(void);

/* HTmpStackSize - return stack size for temporary memory blocks             */
extern HLibExport Herror HTmpStackSize(Hproc_handle proc_handle, INT4_8 *size);

/* HSetAllocLevel - set 'level' of memory allocation                         */
extern HLibExport Herror HSetAllocLevel(INT4_8 level);

/* HGetAllocLevel - read 'level' of memory allocation                        */
extern HLibExport Herror HGetAllocLevel(INT4_8 *level);

/* HSetGlbMemCacheMode - sets the running mode of the cache for globally 
 * allocated memory blocks. */
void HSetGlbMemCacheMode(int mode);
/* HGetGlbMemCacheMode - returns the running mode of the cache for globally 
 * allocated memory blocks. */
void HGetGlbMemCacheMode(int *mode);

/* HGetImageCacheSize - current size in bytes of all cached images           */
extern HLibExport Herror HGetImageCacheSize(Hproc_handle proc_handle, 
                                            INT cache_mode, INT4_8 *size);

/* HGetImageCacheCapacity - maximum size in bytes of all cached images       */
extern HLibExport Herror HGetImageCacheCapacity(Hproc_handle proc_handle, 
                                                INT cache_mode, INT4_8 *size);

/* HGetImageCacheCapacity - set maximum size in bytes of all cached images 
 *                      0 switches off image caching                         */
extern HLibExport Herror HSetImageCacheCapacity(Hproc_handle proc_handle, 
                                                INT cache_mode, INT4_8 size);

/* HClearImageCache - free all cached memory blocks                          */
extern HLibExport Herror HClearImageCache(Hproc_handle proc_handle, 
                                          INT cache_mode);

/* HMallocInfo - return the number of calls of clib malloc                   */
extern HLibExport Herror HMallocInfo(INT4_8 *mcalls);

/* HMemInfo - retrieve informationen zaboutc current memory allocation       */
extern HLibExport Herror HMemInfo(Hproc_handle proc_handle,
                                  INT4_8 *alloc_mem, INT4_8 *free_mem,
                                  INT4_8 *used_mem, INT4_8 *alloc_call,
                                  INT4_8 *free_call, double *hit_rate,
                                  double *small_rate, size_t *cache,
                                  size_t *stack);

/* HNumMemElements - get number of currently allocated memory blocks         */
extern HLibExport Herror HNumMemElements(INT4_8  *num);

/* HAllocInfo - get line of call/size of all blocks with level >= AllocLevel */
extern HLibExport Herror HAllocInfo(INT max, char *file[], INT4 line[],
                                    INT4_8 size[], INT4 level[], INT4_8 *num);
extern HLibExport int HfprintfAllocInfo( FILE *stream);

/* HPrepMem - prepares memory (allocates+frees memory) => more efficiency    */
extern HLibExport Herror HPrepMem (Hproc_handle proc_handle, size_t size,
                                   size_t *si_ze);

/* HFreeUnusedTmpMem - free any unused temporary memory                      */
extern HLibExport Herror HFreeUnusedTmpMem(Hproc_handle proc_handle);

/*---------------------------------------------------------------------------*/
/*                   Management of Free Usable Memory                        */
/*---------------------------------------------------------------------------*/
/* HInitMem - initialize global cache-memory and free memory management   */
extern HLibExport Herror HInitMem (Hproc_handle proc_handle, size_t size);

/* HCloseMem - shut down management of cache memory and free memory       */
extern HLibExport Herror HCloseMem(void);

/* HMemInitHandle - initialize client-specific memory management handle      */
extern HLibExport Herror HMemInitHandle(Hproc_handle proc_handle);

/* HMemCloseHandle - give free client-specific memory management handle      */
extern HLibExport Herror HMemCloseHandle(Hproc_handle proc_handle, 
                                         Herror proc_res);

/* HMemResetHandle - give free client-specific memory management handle      */
extern HLibExport Herror HMemResetHandle(Hproc_handle proc_handle, 
                                         Herror proc_res);

/* HCleanUpGlobalMemCache - frees unused cache blocks                        */
extern HLibExport Herror HCleanupGlobalMemCache(void);


/* HXAlloc - allocate free usable memory                                     */
extern HLibExport Herror HXAlloc (Hproc_handle proc_handle, size_t size,
                                  VOIDP void_p);

/* HXAllocMemCheck - allocate free usable memory and collect test info       */
extern HLibExport Herror HXAllocMemCheck (Hproc_handle proc_handle, 
                                          size_t size, const char *file, 
                                          INT4_8 line, INT4 level,
                                          VOIDP void_p);

/* HXFree - give free (free usable) memory (insert it into cache) */
extern HLibExport Herror HXFree(Hproc_handle proc_handle, VOIDP mem);

/* HXFree - give free (free usable) memory (insert it into cache) and test it*/
extern HLibExport Herror HXFreeMemCheck(Hproc_handle proc_handle, VOIDP mem,
                                        const char *file, INT4_8 line);

/* HXRealloc - reallocate memory and test it (memory still available?)       */
extern HLibExport Herror HXRealloc(Hproc_handle proc_handle, VOIDP ptr,
                                   size_t size, VOIDP void_p,
                                   const char *file, INT4_8 line);
/* HXReallocToGlobal - reallocate global/local memory and convert to global
 *  memory   */
extern HLibExport Herror HXReallocToGlobal(Hproc_handle proc_handle, VOIDP ptr,
                                           size_t size, VOIDP void_p,
                                           const char *file, INT4_8 line);

/* HXAllocRLNum - allocate permanent memory for specific region 
 * (pass Hrlregion** to region parameter)                                    */
extern HLibExport Herror HXAllocRLNum(Hproc_handle proc_handle, 
                                      VOIDP region, size_t num,
                                      const char *file, INT4_8 line);

/* HXReallocRLNum - reallocate global memory for specific region         
 * (pass Hrlregion** to reg_new parameter but Hrlregion * to reg_old)        */
extern HLibExport Herror HXReallocRLNum(Hproc_handle proc_handle, 
                                        VOIDP reg_old,size_t num_new,
                                        VOIDP reg_new, 
                                        const char *file, INT4_8 line);

/* HXReallocRLNumToGlobal - reallocate local or global memory for specific 
 * region and convert it to global memory (pass Hrlregion** to reg_new 
 * parameter but Hrlregion * to reg_old) */
extern HLibExport Herror HXReallocRLNumToGlobal(Hproc_handle proc_handle, 
                                                VOIDP reg_old,size_t num_new,
                                                VOIDP reg_new, 
                                                const char *file, INT4_8 line);

/* HXAllocRL - allocate permanent memory for new regions                    
 * (pass Hrlregion** to region parameter)                                    */
extern HLibExport Herror HXAllocRL(Hproc_handle proc_handle, 
                                   VOIDP region,
                                   const char *file, INT4_8 line);

/* HXFreeRL - give free (permanent) memory of a specific region              */
extern HLibExport Herror HXFreeRL(Hproc_handle proc_handle,VOIDP region, 
                                  const char *file, INT4_8 line);


/* HXAllocGeneral - allocate memory according to allocation bit     */
Herror HXAllocGeneral(Hproc_handle ph, MACHINE_WORD allocbit,
                      INT4_8 size, void *mem, 
                      const char *file, INT4_8 line);      
#define HAllocGeneral(ph,allocbit,size,mem)               \
  HXAllocGeneral(ph,allocbit,size,(void*)mem,__FILE__,__LINE__)



/* HXFreeGeneral - give free (free usable) memory (insert it into cache)     */
extern HLibExport Herror HXFreeGeneral(Hproc_handle proc_handle, VOIDP mem);

/* HXReallocGeneral - reallocate global/local memory and test it             */
extern HLibExport Herror HXReallocGeneral(Hproc_handle proc_handle, VOIDP ptr,
                                          size_t size, VOIDP void_p,
                                          const char *file, INT4_8 line);

/* HXReallocRLNumGeneral - reallocate global/local memory for specific region  
 * (pass Hrlregion** to reg_new parameter but Hrlregion * to reg_old)        */
extern HLibExport Herror HXReallocRLNumGeneral(Hproc_handle proc_handle, 
                                        VOIDP reg_old,size_t num_new,
                                        VOIDP reg_new, 
                                        const char *file, INT4_8 line);

/* HXFreeRLGeneral - give free global/local/temp  memory of a  region        */
extern HLibExport Herror HXFreeRLGeneral(Hproc_handle proc_handle,
                                         VOIDP region,    
                                         const char *file, INT4_8 line);


/* HAllocImageCached - fetch image memory from cache                         */
extern HLibExport void* HAllocImageCached(size_t size);

/* HFreeImageCached - cache image memory                                     */
extern HLibExport void HFreeImageCached(VOIDP image);



/*---------------------------------------------------------------------------*/
/*                   Management of Local Free Memory                         */
/*---------------------------------------------------------------------------*/
/* HXAllocLocal - allocate local memory (AllocLevel=H_LOCAL_ALLOC)           */
extern HLibExport Herror HXAllocLocal (Hproc_handle proc_handle, size_t size,
                                       const char *file,
                                       INT4_8 line, VOIDP void_p);

/* HXFreeLocal - give free local memory (insert it in cache) and test it     */
extern HLibExport Herror HXFreeLocal(Hproc_handle proc_handle, 
                                     VOIDP mem, const char *file, 
                                     INT4_8 line);

/* HXReallocLocal - reallocate local memory (AllocLevel=H_LOCAL_ALLOC)       */
extern HLibExport Herror HXReallocLocal(Hproc_handle proc_handle, VOIDP ptr, 
                                        size_t size, VOIDP void_p, 
                                        const char *file, INT4_8 line);

/* HConvertLocalToGlobal - converts a local memory block to a global one     */
extern HLibExport Herror HConvertLocalToGlobal(Hproc_handle proc_handle, 
                                               VOIDP ptr);


/* HXAllocRLNumLocal - allocate local memory for specific region             
 * (pass Hrlregion** to region parameter)                                    */
extern HLibExport Herror HXAllocRLNumLocal(Hproc_handle proc_handle, 
                                           VOIDP region, size_t num, 
                                           const char *file, INT4_8 line);

/* HXReallocRLNumLocal - reallocate local memory for specific region         
 * (pass Hrlregion** to reg_new parameter but Hrlregion * to reg_old)        */
extern HLibExport Herror HXReallocRLNumLocal(Hproc_handle proc_handle, 
                                             VOIDP reg_old,size_t num_new,
                                             VOIDP reg_new, 
                                             const char *file, INT4_8 line);

/* HXAllocRLLocal -  allocate local memory for new regions                  
 * (pass Hrlregion** to region parameter)                                    */
extern HLibExport Herror HXAllocRLLocal(Hproc_handle proc_handle, 
                                        VOIDP region,
                                        const char *file, INT4_8 line);

/* HXFreeRLLocal - give free (local) memory of a specific region             */
extern HLibExport Herror HXFreeRLLocal(Hproc_handle proc_handle,
                                       VOIDP region, 
                                       const char *file, INT4_8 line);

/* HMemClrLocal - give free the whole local memory (alloc. by HXAllocLocal)  */
extern HLibExport Herror HMemClrLocal(Hproc_handle proc_handle);

/* HMemMoveLocal - transfers all local memory from a proc_handle to an other */
extern HLibExport void HMemMoveLocal(Hproc_handle dest, Hproc_handle src);


/* HFreeMemTTable - give free all temporary stack memory (of an instance)    */
extern HLibExport Herror HFreeMemTTable(Hproc_handle proc_handle);

/* HXFreeAllTmp - give free all the allocated temporary memory               */
extern HLibExport Herror HXFreeAllTmp(Hproc_handle proc_handle,
                                      const char *file, INT4_8 line);

/* HMemClrTTable - free (logically) all the allocated stack memory(MemTTable)*/
extern HLibExport Herror HMemClrTTable(Hproc_handle proc_handle);

/* HXAllocTmp - allocate temporary memory within a stack                     */
extern HLibExport Herror HXAllocTmp(Hproc_handle proc_handle, VOIDP ptr,
                                    size_t size, const char *file,
                                    INT4_8 line);

/* HXAllocDomainImageTmp - allocate memory for temporary images regarding 
 * input image width and domain height */
extern HLibExport Herror HXAllocDomainImageTmp(Hproc_handle proc_handle, 
                                               VOIDP mem_ptr, VOIDP img_ptr,
                                               Hrlregion *region,
                                               HIMGDIM width, HIMGDIM height,
                                               INT byte_per_pixel, 
                                               HIMGDIM border_rows, 
                                               HIMGCNT border_bytes,
                                               const char *file, INT4_8 line);

/* HXAllocRLNumTmp - allocate temporary memory for specific region           
 * (pass Hrlregion** to region parameter)                                    */
extern HLibExport Herror HXAllocRLNumTmp(Hproc_handle proc_handle,
                                         VOIDP region, size_t num,
                                         const char *file, INT4_8 line);

/* HXAllocRLTmp - allocate temporary memory for new regions                  
 * (pass Hrlregion** to region parameter)                                    */
extern HLibExport Herror HXAllocRLTmp(Hproc_handle proc_handle, 
                                      VOIDP region,
                                      const char *file, INT4_8 line);

/* HXTestAllTmp - test all stacks for temporary memory of consistency        */
extern HLibExport Herror HXTestAllTmp(Hproc_handle proc_handle,
                                      const char *file, INT4_8 line);

/* HXTestTmp - test the specified temporary memory block of consistency      */
extern HLibExport Herror HXTestTmp(Hproc_handle proc_handle,
                                   VOIDP ptr, const char *file, INT4_8 line, 
                                   HBOOL do_printf);

/* HXFreeTmp - gives free the lastly allocated (!) stack memory              */
extern HLibExport Herror HXFreeTmp(Hproc_handle proc_handle, VOIDP ptr,
                                   const char *file, INT4_8 line);

/* HXFreeRLTmp - gives free temporary memory of a specific region            */
extern HLibExport Herror HXFreeRLTmp(Hproc_handle proc_handle,
                                     VOIDP region,
                                     const char *file, INT4_8 line);

/* HXFreeNTmp - gives free the 'num' lastly allocated stack memory blocks    */
extern HLibExport Herror HXFreeNTmp(Hproc_handle proc_handle, INT num,
                                    const char *file, INT4_8 line);

/* HXFreeUpToTmp - give free stack memory up to a specified memory block,
 *                 including the specified memory block.                     */
extern HLibExport Herror HXFreeUpToTmp(Hproc_handle proc_handle, void *ptr, 
                                       const char *file, INT4_8 line);

/* HXFreeUpToTmpExcl - give free stack memory up to a specified memory block,
 *                     excluding the specified memory block.                 */
extern HLibExport Herror HXFreeUpToTmpExcl(Hproc_handle proc_handle, 
                                           void *ptr, 
                                           const char *file, INT4_8 line);

/* HXGetLastTmp - get the last allocated memory block on the stack memory    */
extern HLibExport Herror HGetLastTmp(Hproc_handle proc_handle, void **last);

/* HFreeUnusedStacks - gives free (physically) all currently unused stacks   */
extern HLibExport Herror HFreeUnusedStacks(Hproc_handle proc_handle);

/* HShrinkRL - gives free the unused memory of a specific region             
 * (pass Hrlregion** to region parameter)                                    */
extern HLibExport Herror HShrinkRL(Hproc_handle proc_handle, 
                                   VOIDP region);


/*---------------------------------------------------------------------------*/
/*              Management of Thread Group Shared Memory                     */
/*---------------------------------------------------------------------------*/
/* HXAllocShared - allocates shared memory                                   */
extern HLibExport Herror HXAllocShared(Hproc_handle proc_id, UINT4_8 size,
                                       const char * file, INT4_8 line,
                                       void *ptr);

/* HXFreeShared - frees shared memory                                        */
extern HLibExport Herror HXFreeShared(Hproc_handle proc_id, void *ptr,
                                      const char * file, INT4_8 line);


/* HlibAlloc
 * HlibFree
 *  workarround for windows: under windows every DLL has its own memory heap
 *  -> the free method must be called in the same DLL as the malloc
 *---------------------------------------------------------------------------*/
#ifdef _WIN32
extern HLibExport void* Hlib_malloc(size_t size);
extern HLibExport void  Hlib_free(void* mem);
#else
#define Hlib_malloc malloc
#define Hlib_free   free
#endif

/* HUpdateMemInfo - updates the do_low_error_level memory check information  */
extern Herror HUpdateMemInfo(void);

#if defined(__cplusplus)
}
#endif


#endif
