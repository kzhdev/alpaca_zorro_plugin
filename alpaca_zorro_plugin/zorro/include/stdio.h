///////////////////////////////////////////////////////////////////////
// lite-C header file for standard file / directory functions
//////////////////////////////////////////////////////////////////////
#ifndef stdio_h
#define stdio_h
#include <litec.h>

// stdio.h //////////////////////////

typedef char* va_list;
typedef long fpos_t;
typedef long size_t;

struct _iobuf {
    char *_ptr;
    int   _cnt;
    char *_base;
    int   _flag;
    int   _file;
    int   _charbuf;
    int   _bufsiz;
    char *_tmpfname;
} _iobuf;

typedef struct _iobuf FILE;

/* Seek method constants */

#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0

/* Declare _iob[] array */
FILE _iob[3];

#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

#define _IOREAD         0x0001
#define _IOWRT          0x0002

#define _IOFBF          0x0000
#define _IOLBF          0x0040
#define _IONBF          0x0004

#define _IOMYBUF        0x0008
#define _IOEOF          0x0010
#define _IOERR          0x0020
#define _IOSTRG         0x0040
#define _IORW           0x0080
#define _IOAPPEND       0x0200


/* Function prototypes */

FILE * __cdecl _fsopen(const char *, const char *, int);
void __cdecl clearerr(FILE *);

int __cdecl fclose(FILE *);
int __cdecl _fcloseall(void);

FILE * __cdecl _fdopen(int, const char *);

int __cdecl feof(FILE *);
int __cdecl ferror(FILE *);
int __cdecl fflush(FILE *);
int __cdecl fgetc(FILE *);
int __cdecl _fgetchar(void);
int __cdecl fgetpos(FILE *, fpos_t *);
char * __cdecl fgets(char *, int, FILE *);

int __cdecl _fileno(FILE *);
int __cdecl _flushall(void);

FILE * __cdecl fopen(const char *, const char *);
int __cdecl fprintf(FILE *, const char *, ...);
int __cdecl fputc(int, FILE *);
int __cdecl _fputchar(int);
int __cdecl fputs(const char *, FILE *);
size_t __cdecl fread(void *, size_t, size_t, FILE *);
FILE * __cdecl freopen(const char *, const char *, FILE *);
int __cdecl fscanf(FILE *, const char *, ...);
int __cdecl fsetpos(FILE *, const fpos_t *);
int __cdecl fseek(FILE *, long, int);
long __cdecl ftell(FILE *);
size_t __cdecl fwrite(const void *, size_t, size_t, FILE *);
int __cdecl getc(FILE *);
int __cdecl getchar(void);
int __cdecl _getmaxstdio(void);
char * __cdecl gets(char *);
int __cdecl _getw(FILE *);
void __cdecl perror(const char *);
int __cdecl _pclose(FILE *);
FILE * __cdecl _popen(const char *, const char *);

//int __cdecl printf(const char *, ...);
int __cdecl putc(int, FILE *);
int __cdecl putchar(int);
int __cdecl puts(const char *);
int __cdecl _putw(int, FILE *);
int __cdecl remove(const char *);
int __cdecl rename(const char *, const char *);
void __cdecl rewind(FILE *);
int __cdecl _rmtmp(void);
int __cdecl scanf(const char *, ...);
void __cdecl setbuf(FILE *, char *);
int __cdecl _setmaxstdio(int);
int __cdecl setvbuf(FILE *, char *, int, size_t);
int __cdecl _snprintf(char *, size_t, const char *, ...);
//int __cdecl sprintf(char *, const char *, ...);
int __cdecl _scprintf(const char *, ...);
int __cdecl sscanf(const char *, const char *, ...);
int __cdecl _snscanf(const char *, size_t, const char *, ...);
char * __cdecl _tempnam(const char *, const char *);
FILE * __cdecl tmpfile(void);
char * __cdecl tmpnam(char *);
int __cdecl ungetc(int, FILE *);
int __cdecl _unlink(const char *);
int __cdecl vfprintf(FILE *, const char *, va_list);
int __cdecl vprintf(const char *, va_list);
int __cdecl _vsnprintf(char *, size_t, const char *, va_list);
int __cdecl vsprintf(char *, const char *, va_list);
int __cdecl _vscprintf(const char *, va_list);


/* Macro definitions */

#define feof(_stream)     ((_stream)->_flag & _IOEOF)
#define ferror(_stream)   ((_stream)->_flag & _IOERR)
#define _fileno(_stream)  ((_stream)->_file)

// io.h /////////////////////////////////////////

typedef long time_t;       /* time value */
typedef long intptr_t;
typedef long _fsize_t;

typedef struct _finddata_t {
     long    attrib;
     time_t  time_create;    /* -1 for FAT file systems */
     time_t  time_access;    /* -1 for FAT file systems */
     time_t  time_write;
     _fsize_t size;
     char    name[260];
} _finddata_t;

/* File attribute constants for _findfirst() */

#define _A_NORMAL       0x00    /* Normal file - No read/write restrictions */
#define _A_RDONLY       0x01    /* Read only file */
#define _A_HIDDEN       0x02    /* Hidden file */
#define _A_SYSTEM       0x04    /* System file */
#define _A_SUBDIR       0x10    /* Subdirectory */
#define _A_ARCH         0x20    /* Archive file */

/* function prototypes */

int __cdecl _access(const char *, int);
int __cdecl _chmod(const char *, int);
int __cdecl _chsize(int, long);
int __cdecl _close(int);
int __cdecl _commit(int);
int __cdecl _creat(const char *, int);
int __cdecl _dup(int);
int __cdecl _dup2(int, int);
int __cdecl _eof(int);
long __cdecl _filelength(int);
intptr_t __cdecl _findfirst(const char *, _finddata_t *);
int __cdecl _findnext(intptr_t, _finddata_t *);
int __cdecl _findclose(intptr_t);
int __cdecl _isatty(int);
int __cdecl _locking(int, int, long);
long __cdecl _lseek(int, long, int);
char * __cdecl _mktemp(char *);
int __cdecl _open(const char *, int, ...);
int __cdecl _pipe(int *, unsigned int, int);
int __cdecl _read(int, void *, unsigned int);
int __cdecl remove(const char *);
int __cdecl rename(const char *, const char *);
int __cdecl _setmode(int, int);
int __cdecl _sopen(const char *, int, int, ...);
long __cdecl _tell(int);
int __cdecl _umask(int);
int __cdecl _unlink(const char *);
int __cdecl _write(int, const void *, unsigned int);

// direct.h //////////////////////////////////////////////

typedef struct _diskfree_t {
    long total_clusters;
    long avail_clusters;
    long sectors_per_cluster;
    long bytes_per_sector;
} _diskfree_t;

/* function prototypes */

int __cdecl _chdir(const char *);
char * __cdecl _getcwd(char *, int);
int __cdecl _mkdir(const char *);
int __cdecl _rmdir(const char *);

int __cdecl _chdrive(int);
char * __cdecl _getdcwd(int, char *, int);
int __cdecl _getdrive(void);
long __cdecl _getdrives(void);
long __cdecl _getdiskfree(long, _diskfree_t *);

// stdlib.h //////////////////////////////////////////////

void qsort(
   void *base, // array to be sorted
   int num,	   // number of elements
   int width,  // element size
   void* compare //int (__cdecl *compare)(void* element1,void* element2) 
);

void *bsearch( 
   void *element, // element to be searched for
   void *base, // sorted array
   int num,	   // number of elements
   int width,  // element size
   void* compare //int (__cdecl *compare)(void* element1,void* element2) 
);

// string.h //////////////////////////////////////////////

int __cdecl strcpy_s( char *dest, size_t dest_size, const char *src);
int __cdecl sprintf_s(char *buffer,  size_t sizeOfBuffer,  const char *format,  ...);
int __cdecl strcat_s(char *strDestination, size_t numberOfElements, const char *strSource);
int __cdecl memcpy_s(void *dest,   size_t destSize,   const void *src,   size_t count);

API(strcpy_s,msvcrt)
API(sprintf_s,msvcrt)
API(strcat_s,msvcrt)
API(memcpy_s,msvcrt)

// Selected API functions ////////////////////////////////

API(_fsopen,msvcrt)
API(clearerr,msvcrt)
API(fclose,msvcrt)
API(_fcloseall,msvcrt)
API(_fdopen,msvcrt)
API(feof,msvcrt)
API(ferror,msvcrt)
API(fflush,msvcrt)
API(fgetc,msvcrt)
API(_fgetchar,msvcrt)
API(fgetpos,msvcrt)
API(fgets,msvcrt)
API(_fileno,msvcrt)
API(_flushall,msvcrt)
API(fopen,msvcrt)
API(fprintf,msvcrt)
API(fputc,msvcrt)
API(_fputchar,msvcrt)
API(fputs,msvcrt)
API(fread,msvcrt)
API(freopen,msvcrt)
API(fscanf,msvcrt)
API(fsetpos,msvcrt)
API(fseek,msvcrt)
API(ftell,msvcrt)
API(fwrite,msvcrt)
API(getc,msvcrt)
API(getchar,msvcrt)
API(_getmaxstdio,msvcrt)
API(gets,msvcrt)
API(_getw,msvcrt)
API(perror,msvcrt)
API(_pclose,msvcrt)
API(_popen,msvcrt)
//API(printf,msvcrt)
API(putc,msvcrt)
API(putchar,msvcrt)
API(puts,msvcrt)
API(_putw,msvcrt)
API(remove,msvcrt)
API(rename,msvcrt)
API(rewind,msvcrt)
API(_rmtmp,msvcrt)
API(scanf,msvcrt)
API(setbuf,msvcrt)
API(_setmaxstdio,msvcrt)
API(setvbuf,msvcrt)
API(_snprintf,msvcrt)
//API(sprintf,msvcrt)
API(_scprintf,msvcrt)
API(sscanf,msvcrt)
API(_snscanf,msvcrt)
API(_tempnam,msvcrt)
API(tmpfile,msvcrt)
API(tmpnam,msvcrt)
API(ungetc,msvcrt)
API(_unlink,msvcrt)
API(vfprintf,msvcrt)
API(vprintf,msvcrt)
API(_vsnprintf,msvcrt)
API(vsprintf,msvcrt)
API(_vscprintf,msvcrt)

API(_access,msvcrt)
API(_chmod,msvcrt)
API(_chsize,msvcrt)
API(_close,msvcrt)
API(_commit,msvcrt)
API(_creat,msvcrt)
API(_dup,msvcrt)
API(_dup2,msvcrt)
API(_eof,msvcrt)
API(_filelength,msvcrt)
API(_findfirst,msvcrt)
API(_findnext,msvcrt)
API(_findclose,msvcrt)
API(_isatty,msvcrt)
API(_locking,msvcrt)
API(_lseek,msvcrt)
API(_mktemp,msvcrt)
API(_open,msvcrt)
API(_pipe,msvcrt)
API(_read,msvcrt)
API(remove,msvcrt)
API(rename,msvcrt)
API(_setmode,msvcrt)
API(_sopen,msvcrt)
API(_tell,msvcrt)
API(_umask,msvcrt)
API(_unlink,msvcrt)
API(_write,msvcrt)

API(_chdir,msvcrt)
API(_getcwd,msvcrt)
API(_mkdir,msvcrt)
API(_rmdir,msvcrt)
API(_chdrive,msvcrt)
API(_getdcwd,msvcrt)
API(_getdrive,msvcrt)
API(_getdrives,msvcrt)
API(_getdiskfree,msvcrt)

API(qsort,msvcrt)
API(bsearch,msvcrt)

#endif  /* STDIO_H */
