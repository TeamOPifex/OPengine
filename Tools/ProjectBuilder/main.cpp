#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
 #include <sys/types.h>

#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"

#include <iostream>
#include <fstream>
using namespace std;

void write(ofstream* stream, void* data, int size) {
	stream->write((char*)data, size);
}

char* OPstringCreateMerged(const char* str, const char* add) {
	int lenA = strlen(str);
	int lenB = strlen(add);
	char* result = (char*)malloc(lenA + lenB + 1);
	strcpy(result, str);
	strcat(result, add);
	return result;
}

char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = (char*)malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

int main(int argc, char **argv) {

	// Request Project Name
	char projectName[256];

	if(argc > 1) {
		memcpy(&projectName[0], argv[1], strlen(argv[1]));
		printf("Project Name: %s\n", projectName);
	} else {
		printf("Enter Project Name: ");
		fgets (projectName, 256, stdin);
	}

	// Request Project Directory	
	char* directory = (char*)malloc(sizeof(char)* 512);

	if(argc > 2) {
		memcpy(&directory[0], argv[2], strlen(argv[2]));
		printf("Directory Target: %s\n", directory);
	} else {
		printf("Enter directory to create at: ");
		fgets (directory, 512, stdin);
	}

	int dirLength = strlen(directory);
	if(directory[dirLength - 1] != '/') {
		directory = OPstringCreateMerged(directory, "/");
	}

	if (0 != access(directory, F_OK)) {
	  if (ENOENT == errno) {
	    // does not exist
	  	printf("Directory OK\n");
 		int result = mkdir(directory, 0777);
	  }
	  if (ENOTDIR == errno) {
	    // not a directory
	  	printf("Not a directory: %s", directory);
		return EXIT_FAILURE;
	  }
	} else {

	}
	
	// Configure files

	static const char *s_Test_archive_filename = "__opifex_project.zip";

	char data[2048];
	char archive_filename[64];
	printf("miniz.c version: %s\n", MZ_VERSION);
	int status;

	mz_zip_archive zip_archive;
	memset(&zip_archive, 0, sizeof(zip_archive));

	status = mz_zip_reader_init_file(&zip_archive, s_Test_archive_filename, 0);
	if (!status)
	{
		printf("Failed to read the archive\n");
		return EXIT_FAILURE;
	}

	// Make sure all directories are created
	for (int i = 0; i < (int)mz_zip_reader_get_num_files(&zip_archive); i++)
	{
		mz_zip_archive_file_stat file_stat;
		if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
		{
			printf("The zip file is corrupt!\n");
			mz_zip_reader_end(&zip_archive);
			return EXIT_FAILURE;
		}		

		const char* filename = file_stat.m_filename;
		if (mz_zip_reader_is_file_a_directory(&zip_archive, i))
		{
			printf("Directory found: %s\n", filename);
			if (!mz_zip_reader_is_file_a_directory(&zip_archive, i))
			{
				printf("mz_zip_reader_is_file_a_directory() didn't return the expected results!\n");
				mz_zip_reader_end(&zip_archive);
				return EXIT_FAILURE;
			}

			char* result = OPstringCreateMerged(directory, filename);
			printf("Creating dir: %s\n", result);
 			mkdir(result, 0777);
		} 
	}

	// Get and print information about each file in the archive.
	for (int i = 0; i < (int)mz_zip_reader_get_num_files(&zip_archive); i++)
	{
		mz_zip_archive_file_stat file_stat;
		if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
		{
			printf("The zip file is corrupt!\n");
			mz_zip_reader_end(&zip_archive);
			return EXIT_FAILURE;
		}

		printf("Filename: \"%s\", Comment: \"%s\", Uncompressed size: %u, Compressed size: %u, Is Dir: %u\n", 
			file_stat.m_filename, 
			file_stat.m_comment, 
			(unsigned int)file_stat.m_uncomp_size, 
			(unsigned int)file_stat.m_comp_size, 
			mz_zip_reader_is_file_a_directory(&zip_archive, i));

		if (!mz_zip_reader_is_file_a_directory(&zip_archive, i))
		{
			const char* filename = file_stat.m_filename;
			size_t uncompressed_size = file_stat.m_uncomp_size;
			void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, filename, &uncompressed_size, 0);
			if(!p) {
				printf("Failed to get file contents");
			} else {
				char* contents = (char*)p;
				char* replaced = contents;
				int size = uncompressed_size;
				//char* replaced = str_replace(contents, "<%OPIFEX%>", projectName);
				//printf("File Contents: \n%s\n", replaced);
				char* fileLocation = OPstringCreateMerged(directory, filename);
				char* cleansed = str_replace(fileLocation, ".out", "");
				if(strlen(cleansed) != strlen(fileLocation)) {
					printf("Found an out file: %s - %s\n", cleansed, fileLocation);
					replaced = str_replace(contents, "<%OPIFEX_PROJECT_NAME%>", projectName);
					size = strlen(replaced);
				}
				ofstream myFile(cleansed, ios::binary);
				write(&myFile, replaced, size);
				//free(replaced);
				free(fileLocation);
			}
		}
	}

	// Close the archive, freeing any resources it was using
	mz_zip_reader_end(&zip_archive);

	// Output files

	return 0;
}