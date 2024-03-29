/* This FileX test concentrates on the local path operations.  */

#ifndef FX_STANDALONE_ENABLE
#include   "tx_api.h"
#endif

#include   "fx_api.h"
#include   <stdio.h>
#include   "fx_ram_driver_test.h"

#ifdef FX_ENABLE_EXFAT

#ifndef FX_STANDALONE_ENABLE

#define     DEMO_STACK_SIZE         8192
#define     CACHE_SIZE              FX_EXFAT_SECTOR_SIZE


/* Define the ThreadX and FileX object control blocks...  */

static TX_THREAD                ftest_0;
static FX_MEDIA                 ram_disk;
static UINT                     error_counter =  0;


/* Define the counters used in the test application...  */

static UCHAR                    *ram_disk_memory;
static UCHAR                    *cache_buffer;

static UINT                     expected_index =  0;
static CHAR *                   expected_name[] =  {

                        "RootFile01",
                        "RootFile02",
                        "RootDir03",
                        "RootDir03_SubDir01",
                        "RootDir03_File02",
                        "RootDir03_SubDir03",
                        "RootDir03_File04",
                        "RootDir03_SubDir05",
                        "RootDir03_File06",
                        "RootDir03_SubDir07",
                        "RootDir03_File08",
                        "RootDir04",
                        "RootDir04_File01",
                        "RootDir04_File02",
                        "RootDir04_SubDir03",
                        "RootDir04_SubDir03_SubSubDir01",
                        "RootDir04_SubDir03_SubSubDir02",
                        "RootDir04_SubDir03_SubSubDir03",
                        "RootDir04_SubDir03_SubSubFile04",
                        "RootDir04_SubDir04",
                        "RootDir04_SubDir04_SubSubDir01",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubDir01",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubDir02",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubDir03",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubFile04",
                        "RootDir04_SubDir04_SubSubDir02",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubDir01",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubDir02",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubDir03",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubFile04",
                        "RootDir04_SubDir04_SubSubDir03",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubDir01",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubDir02",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubDir03",
                        "RootDir04_SubDir04_SubSubDir01_SubSubSubFile04",
                        "RootDir04_SubDir04_SubSubFile04",
                        "RootDir04_SubDir05",
                        "RootDir04_SubDir05_SubSubDir01",
                        "RootDir04_SubDir05_SubSubDir02",
                        "RootDir04_SubDir05_SubSubDir03",
                        "RootDir04_SubDir05_SubSubFile04",
                        "RootDir04_SubDir08_SubSubFile04",
                        "RootDir04_File06",
                        "RootDir04_File07",
                        "RootDir04_SubDir08",
                        "RootDir04_SubDir08_SubSubDir01",
                        "RootDir04_SubDir08_SubSubDir02",
                        "RootDir04_SubDir08_SubSubDir03",
                        "RootDir05",
                        "RootDir05_File02",
                        "RootDir05_File03",
                        "RootDir05_File04",
                        "RootDir05_SubDir05",
                        "RootDir05_SubDir06",
                        "RootDir05_SubDir07",
                        "RootDir05_File08",
                        "RootDir05_SubDir09",
                        "RootDir06",
                        "RootDir06_SubDir01",
                        "RootDir06_SubDir02",
                        "RootDir06_SubDir03",
                        "RootDir06_File04",
                        "RootDir06_File05",
                        "RootDir06_File06",
                        "RootDir06_File07",
                        "RootDir06_SubDir08",
                        "RootDir07",
                        "RootDir07_SubDir01",
                        "RootDir07_SubDir02",
                        "RootDir07_SubDir03",
                        "RootDir07_SubDir04",
                        "RootDir07_File05",
                        "RootDir07_File06",
                        "RootDir07_File07",
                        "RootDir07_File08",
                        "RootDir08",
                        "RootDir08_File01",
                        "RootDir08_File02",
                        "RootDir08_File03",
                        "RootDir08_File04",
                        "RootDir08_SubDir05",
                        "RootDir08_SubDir06",
                        "RootDir08_SubDir07",
                        "RootDir08_SubDir08",
                        "RootFile09",
                        "RootFile10",
                        "RootDir11",
                        "RootDir11_File01",
                        "RootDir11_File02",
                        "RootDir11_SubDir03",
                        "RootDir11_SubDir04",
                        "RootDir11_File05",
                        "RootDir11_File06",
                        "RootDir11_SubDir07",
                        "RootDir11_SubDir08",
                        "RootDir12",
                        "RootDir12_File01",
                        "RootDir12_File02",
                        "RootDir12_SubDir03",
                        "RootDir12_SubDir04",
                        "RootDir12_File05",
                        "RootDir12_File06",
                        "RootDir12_SubDir07",
                        "RootDir12_SubDir08",
                        "RootDir13",
                        "RootDir13_File01",
                        "RootDir13_File02",
                        "RootDir13_SubDir03",
                        "RootDir13_SubDir04",
                        "RootDir13_File05",
                        "RootDir13_File06",
                        "RootDir13_SubDir07",
                        "RootDir13_SubDir08",
                        "RootDir14",
                        "RootFile15",
                        "END"};



/* Define thread prototypes.  */

void    filex_directory_local_path_exfat_application_define(void *first_unused_memory);
static void    ftest_0_entry(ULONG thread_input);
static void    traverse_directory(CHAR *directory_name);

VOID  _fx_ram_driver(FX_MEDIA *media_ptr);
void  test_control_return(UINT status);



/* Define what the initial system looks like.  */

#ifdef CTEST
void test_application_define(void *first_unused_memory)
#else
void    filex_directory_local_path_exfat_application_define(void *first_unused_memory)
#endif
{

UCHAR    *pointer;

    
    /* Setup the working pointer.  */
    pointer =  (UCHAR *) first_unused_memory;

    /* Create the main thread.  */
    tx_thread_create(&ftest_0, "thread 0", ftest_0_entry, 0,  
            pointer, DEMO_STACK_SIZE, 
            4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;

    /* Setup memory for the RAM disk and the sector cache.  */
    cache_buffer =  pointer;
    pointer =  pointer + CACHE_SIZE;
    ram_disk_memory =  pointer;

memset(ram_disk_memory, 0xbf, 512*128);

    /* Initialize the FileX system.  */
    fx_system_initialize();
}



/* Define the test threads.  */

static void    ftest_0_entry(ULONG thread_input)
{

UINT        status;

    FX_PARAMETER_NOT_USED(thread_input);

    /* Print out some test information banners.  */
    printf("FileX Test:   exFAT Directory local path test........................");

    /* Format the media.  This needs to be done before opening it!  */
    status =  fx_media_exFAT_format(&ram_disk, 
                            _fx_ram_driver,         // Driver entry
                            ram_disk_memory,        // RAM disk memory pointer
                            cache_buffer,           // Media buffer pointer
                            CACHE_SIZE,             // Media buffer size 
                            "MY_RAM_DISK",          // Volume Name
                            1,                      // Number of FATs
                            0,                      // Hidden sectors
                            1024,                   // Total sectors 
                            FX_EXFAT_SECTOR_SIZE,   // Sector size   
                            1,                      // exFAT Sectors per cluster
                            12345,                  // Volume ID
                            0);                     // Boundary unit

    /* Determine if the format had an error.  */
    if (status)
    {

        printf("ERROR!\n");
        test_control_return(2);
    }

    /* Open the ram_disk.  */
    status =  fx_media_open(&ram_disk, "RAM DISK", _fx_ram_driver, ram_disk_memory, cache_buffer, CACHE_SIZE);

    /* Check the status.  */
    if (status != FX_SUCCESS)
    {

        /* Error, return error code.  */
        printf("ERROR!\n");
        test_control_return(3);
    }

    /* Create a hiearchy of files and sub-directories.   */
    status =   fx_file_create(&ram_disk,      "RootFile01");
    status +=  fx_file_create(&ram_disk,      "RootFile02");
    status +=  fx_directory_create(&ram_disk, "RootDir03");
    status +=  fx_directory_create(&ram_disk, "RootDir04");
    status +=  fx_directory_create(&ram_disk, "RootDir05");
    status +=  fx_directory_create(&ram_disk, "RootDir06");
    status +=  fx_directory_create(&ram_disk, "RootDir07");
    status +=  fx_directory_create(&ram_disk, "RootDir08");
    status +=  fx_file_create(&ram_disk,      "RootFile09");
    status +=  fx_file_create(&ram_disk,      "RootFile10");
    status +=  fx_directory_create(&ram_disk, "RootDir11");
    status +=  fx_directory_create(&ram_disk, "RootDir12");
    status +=  fx_directory_create(&ram_disk, "RootDir13");
    status +=  fx_directory_create(&ram_disk, "RootDir14");
    status +=  fx_file_create(&ram_disk,      "RootFile15");
    
    /* Create the sub-directories for RootDir03.  */
    status +=  fx_directory_create(&ram_disk, "/RootDir03/RootDir03_SubDir01");
    status +=  fx_file_create(&ram_disk,      "/RootDir03/RootDir03_File02");
    status +=  fx_directory_create(&ram_disk, "/RootDir03/RootDir03_SubDir03");
    status +=  fx_file_create(&ram_disk,      "/RootDir03/RootDir03_File04");
    status +=  fx_directory_create(&ram_disk, "/RootDir03/RootDir03_SubDir05");
    status +=  fx_file_create(&ram_disk,      "/RootDir03/RootDir03_File06");
    status +=  fx_directory_create(&ram_disk, "/RootDir03/RootDir03_SubDir07");
    status +=  fx_file_create(&ram_disk,      "/RootDir03/RootDir03_File08");

    /* Create the sub-directories for RootDir04.  */
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_File01");
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_File02");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir03");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir05");
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_File06");
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_File07");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir08");

    /* Create the sub-directories for RootDir05.  */
    status +=  fx_file_create(&ram_disk,      "/RootDir05/RootDir05_File02");
    status +=  fx_file_create(&ram_disk,      "/RootDir05/RootDir05_File03");
    status +=  fx_file_create(&ram_disk,      "/RootDir05/RootDir05_File04");
    status +=  fx_directory_create(&ram_disk, "/RootDir05/RootDir05_SubDir05");
    status +=  fx_directory_create(&ram_disk, "/RootDir05/RootDir05_SubDir06");
    status +=  fx_directory_create(&ram_disk, "/RootDir05/RootDir05_SubDir07");
    status +=  fx_file_create(&ram_disk,      "/RootDir05/RootDir05_File08");
    status +=  fx_directory_create(&ram_disk, "/RootDir05/RootDir05_SubDir09");


    /* Create the sub-directories for RootDir06.  */
    status +=  fx_directory_create(&ram_disk, "/RootDir06/RootDir06_SubDir01");
    status +=  fx_directory_create(&ram_disk, "/RootDir06/RootDir06_SubDir02");
    status +=  fx_directory_create(&ram_disk, "/RootDir06/RootDir06_SubDir03");
    status +=  fx_file_create(&ram_disk,      "/RootDir06/RootDir06_File04");
    status +=  fx_file_create(&ram_disk,      "/RootDir06/RootDir06_File05");
    status +=  fx_file_create(&ram_disk,      "/RootDir06/RootDir06_File06");
    status +=  fx_file_create(&ram_disk,      "/RootDir06/RootDir06_File07");
    status +=  fx_directory_create(&ram_disk, "/RootDir06/RootDir06_SubDir08");


    /* Create the sub-directories for RootDir07.  */
    status +=  fx_directory_create(&ram_disk, "/RootDir07/RootDir07_SubDir01");
    status +=  fx_directory_create(&ram_disk, "/RootDir07/RootDir07_SubDir02");
    status +=  fx_directory_create(&ram_disk, "/RootDir07/RootDir07_SubDir03");
    status +=  fx_directory_create(&ram_disk, "/RootDir07/RootDir07_SubDir04");
    status +=  fx_file_create(&ram_disk,      "/RootDir07/RootDir07_File05");
    status +=  fx_file_create(&ram_disk,      "/RootDir07/RootDir07_File06");
    status +=  fx_file_create(&ram_disk,      "/RootDir07/RootDir07_File07");
    status +=  fx_file_create(&ram_disk,      "/RootDir07/RootDir07_File08");

    /* Create the sub-directories for RootDir08.  */
    status +=  fx_file_create(&ram_disk,      "/RootDir08/RootDir08_File01");
    status +=  fx_file_create(&ram_disk,      "/RootDir08/RootDir08_File02");
    status +=  fx_file_create(&ram_disk,      "/RootDir08/RootDir08_File03");
    status +=  fx_file_create(&ram_disk,      "/RootDir08/RootDir08_File04");
    status +=  fx_directory_create(&ram_disk, "/RootDir08/RootDir08_SubDir05");
    status +=  fx_directory_create(&ram_disk, "/RootDir08/RootDir08_SubDir06");
    status +=  fx_directory_create(&ram_disk, "/RootDir08/RootDir08_SubDir07");
    status +=  fx_directory_create(&ram_disk, "/RootDir08/RootDir08_SubDir08");

    /* Create the sub-directories for RootDir11.  */
    status +=  fx_file_create(&ram_disk,      "/RootDir11/RootDir11_File01");
    status +=  fx_file_create(&ram_disk,      "/RootDir11/RootDir11_File02");
    status +=  fx_directory_create(&ram_disk, "/RootDir11/RootDir11_SubDir03");
    status +=  fx_directory_create(&ram_disk, "/RootDir11/RootDir11_SubDir04");
    status +=  fx_file_create(&ram_disk,      "/RootDir11/RootDir11_File05");
    status +=  fx_file_create(&ram_disk,      "/RootDir11/RootDir11_File06");
    status +=  fx_directory_create(&ram_disk, "/RootDir11/RootDir11_SubDir07");
    status +=  fx_directory_create(&ram_disk, "/RootDir11/RootDir11_SubDir08");


    /* Create the sub-directories for RootDir12.  */
    status +=  fx_file_create(&ram_disk,      "/RootDir12/RootDir12_File01");
    status +=  fx_file_create(&ram_disk,      "/RootDir12/RootDir12_File02");
    status +=  fx_directory_create(&ram_disk, "/RootDir12/RootDir12_SubDir03");
    status +=  fx_directory_create(&ram_disk, "/RootDir12/RootDir12_SubDir04");
    status +=  fx_file_create(&ram_disk,      "/RootDir12/RootDir12_File05");
    status +=  fx_file_create(&ram_disk,      "/RootDir12/RootDir12_File06");
    status +=  fx_directory_create(&ram_disk, "/RootDir12/RootDir12_SubDir07");
    status +=  fx_directory_create(&ram_disk, "/RootDir12/RootDir12_SubDir08");

    /* Create the sub-directories for RootDir13.  */
    status +=  fx_file_create(&ram_disk,      "/RootDir13/RootDir13_File01");
    status +=  fx_file_create(&ram_disk,      "/RootDir13/RootDir13_File02");
    status +=  fx_directory_create(&ram_disk, "/RootDir13/RootDir13_SubDir03");
    status +=  fx_directory_create(&ram_disk, "/RootDir13/RootDir13_SubDir04");
    status +=  fx_file_create(&ram_disk,      "/RootDir13/RootDir13_File05");
    status +=  fx_file_create(&ram_disk,      "/RootDir13/RootDir13_File06");
    status +=  fx_directory_create(&ram_disk, "/RootDir13/RootDir13_SubDir07");
    status +=  fx_directory_create(&ram_disk, "/RootDir13/RootDir13_SubDir08");

    /* Create the Sub-sub directories under RootDir04.  */
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir03/RootDir04_SubDir03_SubSubDir01");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir03/RootDir04_SubDir03_SubSubDir02");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir03/RootDir04_SubDir03_SubSubDir03");
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_SubDir03/RootDir04_SubDir03_SubSubFile04");

    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir01");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir02");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir03");
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubFile04");

    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir05/RootDir04_SubDir05_SubSubDir01");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir05/RootDir04_SubDir05_SubSubDir02");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir05/RootDir04_SubDir05_SubSubDir03");
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_SubDir05/RootDir04_SubDir05_SubSubFile04");

    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir08/RootDir04_SubDir08_SubSubDir01");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir08/RootDir04_SubDir08_SubSubDir02");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir08/RootDir04_SubDir08_SubSubDir03");
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_SubDir05/RootDir04_SubDir08_SubSubFile04");
    
    /* Create the sub-sub-sub directories under RootDir04.  */
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir01/RootDir04_SubDir04_SubSubDir01_SubSubSubDir01");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir01/RootDir04_SubDir04_SubSubDir01_SubSubSubDir02");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir01/RootDir04_SubDir04_SubSubDir01_SubSubSubDir03");
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir01/RootDir04_SubDir04_SubSubDir01_SubSubSubFile04");

    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir02/RootDir04_SubDir04_SubSubDir01_SubSubSubDir01");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir02/RootDir04_SubDir04_SubSubDir01_SubSubSubDir02");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir02/RootDir04_SubDir04_SubSubDir01_SubSubSubDir03");
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir02/RootDir04_SubDir04_SubSubDir01_SubSubSubFile04");

    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir03/RootDir04_SubDir04_SubSubDir01_SubSubSubDir01");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir03/RootDir04_SubDir04_SubSubDir01_SubSubSubDir02");
    status +=  fx_directory_create(&ram_disk, "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir03/RootDir04_SubDir04_SubSubDir01_SubSubSubDir03");
    status +=  fx_file_create(&ram_disk,      "/RootDir04/RootDir04_SubDir04/RootDir04_SubDir04_SubSubDir03/RootDir04_SubDir04_SubSubDir01_SubSubSubFile04");


    /* Check the status.  */
    if (status != FX_SUCCESS)
    {

        /* Error creating directory structure.  Return to caller.  */
        printf("ERROR!\n");
        test_control_return(4);
    }

    /* Traverse the directory structure recursively.  */
    traverse_directory(FX_NULL);

    /* Determine if the traversal matched what was expected.  */
    if (error_counter)
    {
    
        /* Nope, error traversing the directory structure!  */
        printf("ERROR!\n");
        test_control_return(5);
    }
    
    /* Close the media.  */
    status =  fx_media_close(&ram_disk);

    /* Determine if the test was successful.  */
    if (status != FX_SUCCESS) 
    {

        printf("ERROR!\n");
        test_control_return(6);
    }
    else
    {

        printf("SUCCESS!\n");
        test_control_return(0);
    }
}


/* Define the directory traversal routine for the local path test.  */
void  traverse_directory(CHAR *directory_name)
{

FX_LOCAL_PATH   local_path;
FX_FILE         file;
UINT            status;
CHAR            name[300];


    /* Determine if we are at the start.  */
    if (directory_name == FX_NULL)
    {
        status =  fx_directory_local_path_set(&ram_disk, &local_path, "\\");
    }
    else
    {
        status =  fx_directory_local_path_set(&ram_disk, &local_path, directory_name);
    }

    /* Get the first directory entry in the root path.  */
    status =  fx_directory_first_entry_find(&ram_disk, name);

    /* Loop through the directory entries for this path.  */
    while (status == FX_SUCCESS)
    {

        /* Compare with what is expected.  */
        if (strcmp(name, expected_name[expected_index++]))
            error_counter++;
       
        /* Determine if this name is a directory or a file.  */
        status =  fx_directory_name_test(&ram_disk, name);

        /* What is it?  */
        if (status == FX_NOT_DIRECTORY)
        {

            /* This is a file, open it to test its integrity.  */
            status =  fx_file_open(&ram_disk, &file, name, FX_OPEN_FOR_READ);
            if (status != FX_SUCCESS)
                error_counter++;
            status =  fx_file_close(&file);
            if (status != FX_SUCCESS)
                error_counter++;
        
            /* Pickup the next directory entry.  */
            status =  fx_directory_next_entry_find(&ram_disk, name);
        }
        else if (status == FX_SUCCESS)
        {

            /* Recursive call to traverse directory.  */
            traverse_directory(name);
        
            /* Restore path.  */
            status =  fx_directory_local_path_restore(&ram_disk, &local_path);        

            status =  fx_directory_next_entry_find(&ram_disk, name);
        } 
    } 

    fx_directory_local_path_clear(&ram_disk);
}


#else

#include "fx_api.h"
#include   "fx_ram_driver_test.h"
#include   <stdio.h>


void  test_control_return(UINT status);

/* Define what the initial system looks like.  */

#ifdef CTEST
void test_application_define(void *first_unused_memory)
#else
void    filex_directory_local_path_exfat_application_define(void *first_unused_memory)
#endif
{

    FX_PARAMETER_NOT_USED(first_unused_memory);

    /* Print out some test information banners.  */
    printf("FileX Test:   exFAT Directory local path test........................N/A\n");

    test_control_return(255);
}

#endif /* FX_STANDALONE_ENABLE */
#endif /* FX_ENABLE_EXFAT */

