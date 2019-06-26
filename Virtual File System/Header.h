//Header.h
#include <stdio.h>
#include <string>
#include <io.h>
#include <stdlib.h>

#define MAXINODE 50			//set maximum number of inode
#define MAXFILESIZE 1024	//set maximum size of file ( don't change )
#define MAX_UFDT 50			//set maximum number of file that one process can open

//MODE OF FILE 
#define READ 1
#define WRITE 2

//FILE TYPE
#define REGULAR 1			//only regular is use
#define SPECIAL 2

//FOR LSEEK FUNCTION
#define START 0
#define CURRENT 1
#define END 2

//SUCCESS
#define SUCCESS 0

//ERROR CODE
#define ERR_INCORRECT_PARMETER -1
#define ERR_INODE_FULL -2
#define ERR_FILE_EXISTS -3
#define ERR_MEMORY_ALLOCATION_FAIL -4
#define ERR_UFDT_FULL -5
#define ERR_FILE_NOT_FOUND -6
#define ERR_FILE_IS_NOT_CLOSE -7
#define ERR_PERMISSION_DENIED -8
#define ERR_IN_SUFFICIENT_MEMORY -9
#define ERR_REACH_AT_END -10
#define ERR_FILE_ALREADY_OPEN -11
#define ERR_UNABLE_TO_PERFORM_LSEEK -12

typedef struct superblock
{
	int TotalInode;
	int FreeInode;
}SUPERBLOCK , *PSUPERBLOCK;


/* Here we consider dynamic Inode and static Inode as one because we creating Filesystem on RAM.
	But it will more similar to static incode ie Disk Inode List Block
*/
typedef struct inode
{
	char FileName[50];		/*	In actual inode struct , file name is not store.
								For simplicity we add this field.		*/
	
	int InodeNumber;		/*	This inode struct is on RAM (dynamic inode) so we store Inodenumber value inside struct.
								This field is absent in static inode (Inode in Disk Incode List Table ).	*/

	int FileSize;			/*	Total memory(in byte) allocated for file   ( block addition)	*/
	int FileActualSize;		/*	Total memory(in byte) use by file within allocated memory	(byte additon)	*/
	
	int FileType;			/*	Macro is define ( REGULAR , SPECIAL )	*/

	char *Buffer;			/*	In actual Inode there is Array of 15 pointer which point to bock in data block.  
								For Simplicity we as use char pointer.
								Afterward we will allocated memory dynamically */
	
	int LinkCount;			/*	LinkCount store number of hard link to this Inode (ie. number of name given to inode )
								For Simplicity we consider LinkCount = 1	*/
	int ReferenceCount;		/*	ReferenceCount tells how many process are using this Inode */

	int Permission;			/*	Macro is define:-   READ , WRITE , or both ( READ + WRITE )	*/

	struct inode *next;	

}INODE;
typedef INODE * PINODE;

typedef struct fileTable
{
	int readOffset;			/* In actual sytem there is one offset , but for simplcity we have use two offset*/
	int writeOffset;
	int count;
	int mode;
	PINODE ptrinode;
}FILETABLE , *PFILETABLE;

typedef struct ufdt			//USER FILE DESCRIPTION TABLE
{
	PFILETABLE ptrfiletable;
}UFDT;


//GOBAL VARIABLE DECALARATION
extern UFDT UFDTArr[MAX_UFDT];		//One Process can open MAX_UFDT
extern SUPERBLOCK SUPERBOLCKobj;
extern PINODE head;

//FUNCTION DEFINTION

void DisplayHelp();
void man(char *);
void DisplayError(int );
void InitialiseSuperBlock();
void CreateDILB();
void ls_file();

int CreateFile( char* , int  );
PINODE GetInode(char*);

int CloseFileByName(char *);
void CloseAllFile();
int rm_File( char * );

int GetFDFromName(char *);

int StatFile( char * );
int FstatFile( int );
void DisplayStatInfoOfInode(PINODE);

int OpenFile( char* , int );

int WriteFile(int , char * , int  );
int ReadFile( int , char * , int  );
int LseekFile( int , int , int );

int truncateFile( char * );