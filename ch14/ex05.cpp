#include <iostream>
#include <cerrno>

namespace Exercises
{
    // The errno value is not included in the 
    class CLibraryError { public: int cerrno; CLibraryError() { cerrno = errno; } };
    class IOError : public CLibraryError {};
    class ResourceBusyError : public CLibraryError {};
    class AccessDeniedError : public CLibraryError {};
    class FileExistsError : public CLibraryError {};
    class IsADirectoryError : public CLibraryError {};
    class TooManyFilesOpenError : public CLibraryError {};
    class FileNameTooLongError : public CLibraryError {};
    class FileNotExistsError : public CLibraryError {};
    class BadFileError : public CLibraryError {};
    class FileTooBigError : public CLibraryError {};
    class NoSpaceLeftError : public CLibraryError {};

    FILE* cpp_fopen(const char* filename, const char* mode)
    {
        FILE* f = fopen(filename, mode);
        if(!f)
        {
            switch(errno)
            {
            case EIO: throw IOError();
            case EBUSY: throw ResourceBusyError();
            case EPERM: throw AccessDeniedError();
            case EEXIST: throw FileExistsError();
            case EISDIR: throw IsADirectoryError();
            case ENFILE: case EMFILE: throw TooManyFilesOpenError();
            case ENAMETOOLONG: throw FileNameTooLongError();
            case ENOENT: throw FileNotExistsError();
            case ENOSPC: throw NoSpaceLeftError();
            default: throw CLibraryError();
            }
        }
        return f;
    }

    void cpp_fclose(FILE* stream)
    {
        int r = fclose(stream);
        if(r == EOF)
        {
            switch(errno)
            {
            case EBADF: throw BadFileError();
            default: throw CLibraryError();
            }
        }
    }

    char* cpp_fgets(char* str, int num, FILE* stream)
    {
        char* r = fgets(str, num, stream);
        if(!r)
        {
            // End of file is not an exceptional case, just return null.
            if(feof(stream)) return r;
            switch(errno)
            {
            case EBADF: throw BadFileError();
            case EIO: throw IOError();
            default: throw CLibraryError();
            }
        }
    }

    int cpp_fputs(const char* str, FILE* stream)
    {
        int r = fputs(str, stream);
        if (r == EOF)
        {
            switch(errno)
            {
            case EBADF: throw BadFileError();
            case EIO: throw IOError();
            case EFBIG: throw FileTooBigError();
            case ENOSPC: throw NoSpaceLeftError();
            default: throw CLibraryError();
            }
        }
    }
}

int main()
{
    using namespace std;
    using namespace Exercises;

    try
    {
        FILE* f = cpp_fopen("no_such_file","r");
    }
    catch(FileNotExistsError ex)
    {
        cout << "the file doesn't exist: error code " << ex.cerrno << endl;
    }

    return 0;
}
