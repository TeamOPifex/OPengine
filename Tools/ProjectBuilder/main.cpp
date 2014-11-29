#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
 #include <sys/types.h>

//#define OPIFEX_WINDOWS

#ifdef OPIFEX_WINDOWS
	#include <direct.h>
#else
	#include <unistd.h>
#endif

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

void str_removeNewLine(char* orig) {
	int size = strlen(orig);
	for(int i = 0; i < size; i++) {
		if(orig[i] == '\n') {
			orig[i] = NULL;
			return;
		}
	}
}

char *replace_str2(const char *str, const char *old, const char *n)
{
	char *ret, *r;
	const char *p, *q;
	size_t oldlen = strlen(old);
	size_t count, retlen, newlen = strlen(n);
	int samesize = (oldlen == newlen);

	if (!samesize) {
		for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
			count++;
		/* This is undefined if p - str > PTRDIFF_MAX */
		retlen = p - str + strlen(p) + count * (newlen - oldlen);
	} else
		retlen = strlen(str);

	if ((ret = (char*)malloc(retlen + 1)) == NULL)
		return NULL;

	r = ret, p = str;
	while (1) {
		/* If the old and new strings are different lengths - in other
		 * words we have already iterated through with strstr above,
		 * and thus we know how many times we need to call it - then we
		 * can avoid the final (potentially lengthy) call to strstr,
		 * which we already know is going to return NULL, by
		 * decrementing and checking count.
		 */
		if (!samesize && !count--)
			break;
		/* Otherwise i.e. when the old and new strings are the same
		 * length, and we don't know how many times to call strstr,
		 * we must check for a NULL return here (we check it in any
		 * event, to avoid further conditions, and because there's
		 * no harm done with the check even when the old and new
		 * strings are different lengths).
		 */
		if ((q = strstr(p, old)) == NULL)
			break;
		/* This is undefined if q - p > PTRDIFF_MAX */
		ptrdiff_t l = q - p;
		memcpy(r, p, l);
		r += l;
		memcpy(r, n, newlen);
		r += newlen;
		p = q + oldlen;
	}
	strcpy(r, p);

	return ret;
}

char* str_replace(char *orig, char *rep, char *with, int verbose) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if(verbose) {
	    printf("rep: %s\n", rep);
	    printf("with: %s\n", with);
	}

    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    if(verbose) printf("Find count...\n");

    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    if(verbose) printf("Count: %d\n", count);

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = (char*)malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    if(verbose) printf("Begin looping\n");

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }

    if(verbose) printf("Now copy it\n");

    strcpy(tmp, orig);

    return result;
}

int main(int argc, char **argv) {

	// Request Project Name
	char projectName[256];

	if(argc > 1) {
		memcpy(&projectName[0], argv[1], strlen(argv[1]) + 1);
		printf("Project Name: %s\n", projectName);
	} else {
		printf("Enter Project Name: ");
		fgets (projectName, 256, stdin);
		printf("Project Name: %s\n", projectName);
		str_removeNewLine(projectName);
		printf("Project Name: %s\n", projectName);
	}

	// Request Project Directory	
	char* directory = (char*)malloc(sizeof(char)* 512);

	if(argc > 2) {
		memcpy(&directory[0], argv[2], strlen(argv[2]) + 1);
		printf("Directory Target: %s\n", directory);
	} else {
		printf("Enter directory to create at: ");
		fgets (directory, 512, stdin);
		printf("Directory Target: %s\n", directory);
		str_removeNewLine(directory);
		printf("Directory Target: %s\n", directory);
	}

	int dirLength = strlen(directory);
	if(directory[dirLength - 1] != '/') {
		directory = OPstringCreateMerged(directory, "/");
	}


#ifdef OPIFEX_WINDOWS
	mkdir(directory);
#else
	if (0 != access(directory, F_OK)) {
	  if (ENOENT == errno) {
	    // does not exist
	  	//printf("Directory OK\n");
 		int result = mkdir(directory, 0777);
	  }
	  if (ENOTDIR == errno) {
	    // not a directory
	  	printf("Not a directory: %s", directory);
		return EXIT_FAILURE;
	  }
	}
#endif

	// Configure files

	static const char *s_Test_archive_filename = "../AppTemplate.zip";

	char data[2048];
	char archive_filename[64];
	//printf("miniz.c version: %s\n", MZ_VERSION);
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
			//printf("Directory found: %s\n", filename);
			if (!mz_zip_reader_is_file_a_directory(&zip_archive, i))
			{
				printf("mz_zip_reader_is_file_a_directory() didn't return the expected results!\n");
				mz_zip_reader_end(&zip_archive);
				return EXIT_FAILURE;
			}

			char* result = OPstringCreateMerged(directory, filename);
			//printf("Creating dir: %s\n", result);
#ifdef OPIFEX_WINDOWS
			mkdir(result);
#else
			mkdir(result, 0777);
#endif
		} 
	}

	char* TEXT_TO_REPLACE = "<%OPIFEX_PROJECT_NAME%>";

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

		//printf("Filename: \"%s\", Comment: \"%s\", Uncompressed size: %u, Compressed size: %u, Is Dir: %u\n", 
			// file_stat.m_filename, 
			// file_stat.m_comment, 
			// (unsigned int)file_stat.m_uncomp_size, 
			// (unsigned int)file_stat.m_comp_size, 
			// mz_zip_reader_is_file_a_directory(&zip_archive, i));

		if (!mz_zip_reader_is_file_a_directory(&zip_archive, i))
		{
			const char* filename = file_stat.m_filename;
			size_t uncompressed_size = file_stat.m_uncomp_size;
			void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, filename, &uncompressed_size, 0);
			if(!p) {
				printf("Failed to get file contents");
			} else {

				char* contentsPtr = (char*)p;
				char* contents = (char*)malloc(uncompressed_size + 1);
				memcpy(contents, contentsPtr, uncompressed_size);
				contents[uncompressed_size] = NULL;

				char* replaced = contents;
				int size = uncompressed_size;

				char* fileLocation = OPstringCreateMerged(directory, filename);
				char* cleansed = replace_str2(fileLocation, ".out", "");
				
				if(strlen(cleansed) != strlen(fileLocation)) {
					//printf("Found an out file: %s - %s\n", cleansed, fileLocation);
					replaced = replace_str2(contents, TEXT_TO_REPLACE, projectName);
					//printf("Replaced.");
					//printf("Size: %d was %d", strlen(replaced), size);
					size = strlen(replaced);
				}

				ofstream myFile(cleansed, ios::binary);
				write(&myFile, replaced, size);
				//free(replaced);
				if(replaced != contents){
					free(replaced);
				}
				free(fileLocation);
				free(cleansed);

			}
		}
	}

	// Close the archive, freeing any resources it was using
	mz_zip_reader_end(&zip_archive);

	// Output files

	printf("Project Created\n");

	return 0;
}