#include "./../include/OPfile.h"

//-----------------------------------------------------------------------------
OPint OPwriteFile(const char* path, OPstream* stream){
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	OPint fd = 0;
	
	// be sure that the file could be opened successfully
	if((fd = open(path, O_CREAT|O_WRONLY|O_TRUNC)) > 0){
		// write the entire stream in one go.
		write(fd, stream->Data, stream->_pointer);
		// finally close the file, we are done writing
		close(fd); 
	}
	else{
		return -1;
	}
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// windows implementation
#endif
}
//-----------------------------------------------------------------------------
OPstream* OPreadFile(const char* path){
	// check to see if the file exists
	if(OPfileExists(path) >= 0){
		printf("%s exists\n", path);
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
		OPint fd = 0, i;
 
		// be sure that the file could be opened successfully
	 	if(fd = open(path, O_RDONLY)){
			ui8 byte = 0;
			OPstream* str = OPstreamCreate(4);

			printf("File opened successfully\n");

			// write the entire file into a stream
			while(read(fd, &byte, sizeof(ui8))){
				//char ch = (char)byte;
				//printf("%c", ch);
				OPwrite(str, &byte, sizeof(ui8));
			}
			close(fd); 
			OPseek(str, 0);

			// finally return the stream
			return str;
		}
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// windows implementation
#endif
	}
	else{
		printf("%s does not exist\n", path);
		return NULL;
	}	
}
//-----------------------------------------------------------------------------
OPint OPfileExists(const char* path){
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	return access(path, F_OK);
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)

#endif
}
//-----------------------------------------------------------------------------
OPint OPdeleteFile(const char* path){
	if(OPfileExists(path)){
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
		return unlink(path);
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)

#endif
	}
	else{
		return -1;
	}
}
//-----------------------------------------------------------------------------
//- C++ Definitions -----------------------------------------------------------
#ifdef __cplusplus // use the C++ file object
	using namespace OPEngine::Data;

	OPStream* OPFile::Read(const char* path){
		// check to see if the file exists
		if(OPFile::Exists(path) >= 0){
			printf("%s exists\n", path);
#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
			OPint fd = 0, i;
	 
			// be sure that the file could be opened successfully
		 	if(fd = open(path, O_RDONLY)){
				ui8 byte = 0;
				OPStream* str = new OPStream(sizeof(OPint) * 4);

				printf("File opened successfully\n");

				// write the entire file into a stream
				while(read(fd, &byte, sizeof(ui8)))
					str->Write(&byte, sizeof(ui8));

				close(fd); 
				str->Seek(0);

				// finally return the stream
				return str;
			}
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// windows implementation
#endif
		}
		else{
			printf("%s does not exist\n", path);
			return NULL;
		}
	}

	OPint OPFile::Write(const char* path, OPStream* stream){
	#if defined(OPIFEX_ANDROID) || defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
		OPint fd = 0;
		OPstream* s = stream->GetStream();	

		// be sure that the file could be opened successfully
		if((fd = open(path, O_CREAT|O_WRONLY|O_TRUNC, 0666)) >= 0){
			stream->Seek(0);
			// write the entire stream in one go.
			printf("Writing to %d @ %d with %d bytes\n", fd, s->_pointer, s->Length);
			write(fd, s->Data, s->Length);
			// finally close the file, we are done writing
			close(fd); 
			return 1;
		}
		else{
			return -1;
		}
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
		// windows implementation
	#endif
	}

	OPint OPFile::Exists(const char* path){
		return OPfileExists(path);
	}

	OPint OPFile::Delete(const char* path){
		return OPdeleteFile(path);
	}
#endif
