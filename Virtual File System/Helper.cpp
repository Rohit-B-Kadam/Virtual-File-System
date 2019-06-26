//Helper.cpp

#include "Header.h"
 UFDT UFDTArr[MAX_UFDT];		//One Process can open MAX_UFDT
 SUPERBLOCK SUPERBOLCKobj;
 PINODE head = NULL;

void InitialiseSuperBlock()
{
	int i = 0 ;
	SUPERBOLCKobj.TotalInode = MAXINODE;
	SUPERBOLCKobj.FreeInode  = MAXINODE;

	while ( i < MAX_UFDT)
	{
		UFDTArr[i].ptrfiletable = NULL;
		i++;
	}
}

void CreateDILB()
{
	PINODE newInode = NULL;
	PINODE temp = head;
	int i = 1; /* Incode number 0 is not use.
					Because after deleting a file from directory . 
					In that directory beside that file name incode value is keep 0. To indicate that the file is been delete*/

	while( i < MAXINODE )
	{
		newInode = (PINODE)malloc(sizeof(INODE));
		strcpy_s(newInode->FileName,"");
		newInode -> InodeNumber = i;
		newInode -> LinkCount = newInode->ReferenceCount = 0;;
		newInode -> Buffer = NULL;
		newInode -> next =  NULL;
		newInode -> FileSize = 0;
		newInode -> FileType = 0;

		if(temp == NULL)
		{
			head = newInode;
			temp = head;
		}
		else
		{
			temp->next = newInode;
			temp = newInode;
		}
		i++;
	}
}

//open and create are almost similar
int OpenFile( char *name , int mode)
{
	int i = 0;
	PINODE temp = NULL;
	//mode can't be more than 3 or less than 1
	if( name == NULL || mode <= 0 || mode >= 4 )
		return ERR_INCORRECT_PARMETER;
	

	//Checking is file exist or not
	temp = GetInode(name);
	if(temp == NULL)
		return ERR_FILE_NOT_FOUND;

	//Filter - not to open file if its already open

	i = GetFDFromName(name);
	if( i != ERR_FILE_NOT_FOUND )
		return ERR_FILE_ALREADY_OPEN;

	if( temp->Permission < mode )
		return ERR_PERMISSION_DENIED;

	while( i < MAX_UFDT)
	{
		if( UFDTArr[i].ptrfiletable == NULL )
			break;
		i++;
	}
	if( i == MAX_UFDT )
		return ERR_UFDT_FULL;

	/*I Initialise ptablefile
		open Inode at i position*/

	UFDTArr[i].ptrfiletable = (PFILETABLE) malloc (sizeof(FILETABLE));
	if( UFDTArr[i].ptrfiletable == NULL )
		return ERR_MEMORY_ALLOCATION_FAIL;

	UFDTArr[i].ptrfiletable->ptrinode = temp;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

	UFDTArr[i].ptrfiletable->count = 1;							
	UFDTArr[i].ptrfiletable->mode = mode;
	
		UFDTArr[i].ptrfiletable->readOffset = 0;
		UFDTArr[i].ptrfiletable->writeOffset = 0;
	
		UFDTArr[i].ptrfiletable->readOffset = 0;
	
		UFDTArr[i].ptrfiletable->writeOffset = 0;
	return i;
}

void ls_file()
{
	PINODE temp = head;
	if(SUPERBOLCKobj.FreeInode == MAXINODE )
	{
		printf("There is no files\n");
		return;
	}

	printf("\nFile Name\tInode number\tFile size\tLink number\t\n");
	printf("--------------------------------------------------------------------\n");
	
	while( temp != NULL )
	{
		if( temp -> LinkCount != 0 )
		{
			printf("%s\t\t%d\t\t%d\t\t%d\n",temp->FileName,temp->InodeNumber,temp->FileSize,temp->LinkCount);
		}
		temp = temp->next;
	}
	printf("--------------------------------------------------------------------\n");
}

int CreateFile( char *name , int permission )
{
	int i = 0;
	PINODE temp = head;
	if( name == NULL || permission == 0 || permission > 3 )
		return ERR_INCORRECT_PARMETER;

	if( SUPERBOLCKobj.FreeInode == 0 )
		return ERR_INODE_FULL;

	if( GetInode(name) != NULL )
		return ERR_FILE_EXISTS;

	/* new logic */
	while(temp->LinkCount != 0 )
	{
		temp = temp->next;
	}
	//Here we get free Inode Pointer(temp)

	while( i < MAX_UFDT )
	{
		if( UFDTArr[i].ptrfiletable == NULL )
			break;
		i++;
	}
	if( i == MAX_UFDT)
		return ERR_UFDT_FULL;
	//Here we get Free index of Free UFDT

	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if( UFDTArr[i].ptrfiletable == NULL )
		return ERR_MEMORY_ALLOCATION_FAIL;

	UFDTArr[i].ptrfiletable->count = 1;			//for simplicity we give 1;
	UFDTArr[i].ptrfiletable->mode  = permission;
	UFDTArr[i].ptrfiletable->readOffset = 0;
	UFDTArr[i].ptrfiletable->writeOffset = 0;
	
	UFDTArr[i].ptrfiletable->ptrinode = temp;

	strcpy_s( UFDTArr[i].ptrfiletable->ptrinode->FileName , name );
	UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
	UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->Permission = permission;

	UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
	UFDTArr[i].ptrfiletable->ptrinode->FileActualSize = 0;
	UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char*) malloc (MAXFILESIZE);
	memset(UFDTArr[i].ptrfiletable->ptrinode->Buffer,0,MAXFILESIZE);
	
	(SUPERBOLCKobj.FreeInode)--;

	return i;
}

PINODE GetInode( char* name)
{
	PINODE temp = head;
	if( name == NULL )
		return NULL;

	while( temp != NULL )
	{
		if( strcmp(temp->FileName,name) == 0 )
			break;
		temp = temp->next;
	}
	return temp;
}

int CloseFileByName(char * name )
{
	int i = 0;
	i = GetFDFromName(name);
	if( i == ERR_FILE_NOT_FOUND )
		return i;

	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount) --;
	free(UFDTArr[i].ptrfiletable);
	UFDTArr[i].ptrfiletable = NULL;
	return SUCCESS;
}

void CloseAllFile()
{
	int i = 0;
	while( i < MAX_UFDT )
	{
		if( UFDTArr[i].ptrfiletable != NULL )
		{
			(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
			free(UFDTArr[i].ptrfiletable);
			UFDTArr[i].ptrfiletable = NULL;
		}
		i++;
	}
}

int rm_File( char *name)
{
	int fd = 0;
	PINODE temp = head;

	if( name == NULL )
		return ERR_INCORRECT_PARMETER;

	fd = GetFDFromName(name);
	if(fd >= 0)
		return ERR_FILE_IS_NOT_CLOSE;
	// File cannot be close if its is open

	while( temp != NULL  )
	{
		if( strcmp( temp->FileName , name) == 0 )
			break;
		temp = temp->next;
	}
	if( temp == NULL )
		return ERR_FILE_NOT_FOUND;

	(temp->LinkCount)--;

	//Remove Inode Data if linkCount == 0
	if( temp->LinkCount == 0 )
	{
		free(temp->Buffer);
		temp->FileActualSize = 0;
		strcpy_s(temp->FileName,"");
		(SUPERBOLCKobj.FreeInode)++;
	}
	return SUCCESS;
}

int GetFDFromName( char* name)
{
	int i = 0;
	while( i < MAX_UFDT)
	{
		if( UFDTArr[i].ptrfiletable != NULL )
		{
			if( strcmp( UFDTArr[i].ptrfiletable->ptrinode->FileName , name ) == 0 )
				break;
		}
		i++;
	}

	if( i == MAX_UFDT)
		return ERR_FILE_NOT_FOUND;
	return i;
}

int StatFile( char * name )
{
	PINODE temp = head;

	if( name == NULL )
		return ERR_INCORRECT_PARMETER;

	while( temp != NULL )
	{
		if( strcmp(temp->FileName , name ) == 0 )
			break;
		temp = temp-> next;
	}

	if( temp == NULL )
		return ERR_FILE_NOT_FOUND;

	DisplayStatInfoOfInode( temp );
	return SUCCESS;
}

int FstatFile(int fd)
{
	PINODE  temp = NULL;

	if( fd < 0 || fd > MAX_UFDT )
		return ERR_INCORRECT_PARMETER;
	if(UFDTArr[fd].ptrfiletable == NULL)
		return ERR_FILE_NOT_FOUND;


	temp = UFDTArr[fd].ptrfiletable->ptrinode;
	DisplayStatInfoOfInode( temp );
	return SUCCESS;
}

void DisplayStatInfoOfInode(PINODE temp)
{
	printf("\n----------Statistical Information About File--------------\n");
	printf("File Name: %s\n",temp->FileName);
	printf("Inode Number: %d\n",temp->InodeNumber);
	printf("File Size: %d\n",temp->FileSize);
	printf("Actual File Size: %d\n",temp->FileActualSize);
	printf("Link Count :%d\n",temp->LinkCount);
	printf("Reference Count : %d\n",temp->ReferenceCount);
	printf("File Type : Regular file\n");
	if(temp->Permission == 1 )
		printf("File Permission :Read only\n");
	else if(temp->Permission == 2 )
		printf("File Permission : Write\n");
	else
		printf("File Permission :Read and Write\n");

	printf("----------------------------------------------------------\n");
}

int WriteFile( int fd , char *arr , int isize)
{
	//checking permission  given at open file
	if( ( UFDTArr[fd].ptrfiletable->mode ) == READ )
		return ERR_PERMISSION_DENIED;

	if( ( UFDTArr[fd].ptrfiletable->writeOffset + isize ) >= MAXFILESIZE)
		return ERR_IN_SUFFICIENT_MEMORY;
	
	strncpy( (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + ( UFDTArr[fd].ptrfiletable->writeOffset ),arr,isize );
	UFDTArr[fd].ptrfiletable->writeOffset += isize;
	if( UFDTArr[fd].ptrfiletable->writeOffset > UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize )
		UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize += isize;

	return SUCCESS;
}

int ReadFile( int fd , char *arr , int isize)
{
	int read_size;

	if(UFDTArr[fd].ptrfiletable->mode == WRITE )
		return ERR_PERMISSION_DENIED;

	if(UFDTArr[fd].ptrfiletable->readOffset == UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
		return ERR_REACH_AT_END;

	read_size = ( UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) - ( UFDTArr[fd].ptrfiletable->readOffset );
	if(read_size < isize)
	{
		strncpy(arr ,
					( UFDTArr[fd].ptrfiletable->ptrinode->Buffer) +
					(UFDTArr[fd].ptrfiletable->readOffset) , read_size);
		UFDTArr[fd].ptrfiletable->readOffset += read_size;
		return read_size;
	}
	else
	{
		strncpy(arr ,
					( UFDTArr[fd].ptrfiletable->ptrinode->Buffer) +
					(UFDTArr[fd].ptrfiletable->readOffset) , isize);
		
		UFDTArr[fd].ptrfiletable->readOffset += isize; 
	}
	return isize;
}

int LseekFile( int fd , int size , int from)
{
	if( ( fd < 0 ) || ( fd > MAX_UFDT ) )
	{
		return ERR_INCORRECT_PARMETER;
	}

	if( from > 2  || from < 0)
	{
		return ERR_INCORRECT_PARMETER;
	}

	if( UFDTArr[fd].ptrfiletable == NULL )
		return ERR_FILE_NOT_FOUND;

	if( ( UFDTArr[fd].ptrfiletable->mode == READ ) || ( UFDTArr[fd].ptrfiletable->mode == READ + WRITE ))
	{
		if( from == CURRENT )
		{
			if( ( UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize ) < ( (UFDTArr[fd].ptrfiletable->readOffset) + size))
				return ERR_UNABLE_TO_PERFORM_LSEEK;
			if( ((UFDTArr[fd].ptrfiletable->readOffset )+ size ) < 0 )
				return ERR_UNABLE_TO_PERFORM_LSEEK;

			(UFDTArr[fd].ptrfiletable->readOffset) = ( UFDTArr[fd].ptrfiletable->readOffset ) + size;
		}
		else if( from == START )
		{
			if( size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize ) )
				return ERR_UNABLE_TO_PERFORM_LSEEK;
			if( size < 0 )
				return ERR_UNABLE_TO_PERFORM_LSEEK;

			(UFDTArr[fd].ptrfiletable->readOffset) = size;
		}
		else if( from == END)
		{
			if( ( UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize ) < ( (UFDTArr[fd].ptrfiletable->readOffset) + size))
				return ERR_UNABLE_TO_PERFORM_LSEEK;
			if( ((UFDTArr[fd].ptrfiletable->readOffset ) + size ) < 0 )
				return ERR_UNABLE_TO_PERFORM_LSEEK;

			(UFDTArr[fd].ptrfiletable->readOffset) = ( UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize ) + size;
			
		}
	}
	else
	if( ( UFDTArr[fd].ptrfiletable->mode ==  WRITE ))
	{
		if( from == CURRENT )
		{
			if( (  (UFDTArr[fd].ptrfiletable->writeOffset) + size) > ( UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize ) )
				return ERR_UNABLE_TO_PERFORM_LSEEK;
			if( ((UFDTArr[fd].ptrfiletable->writeOffset )+ size ) < 0 )
				return ERR_UNABLE_TO_PERFORM_LSEEK;

			(UFDTArr[fd].ptrfiletable->writeOffset) = ( UFDTArr[fd].ptrfiletable->writeOffset ) + size;
		}
		else if( from == START )
		{
			if( size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize ) )
				return ERR_UNABLE_TO_PERFORM_LSEEK;
			if( size < 0 )
				return ERR_UNABLE_TO_PERFORM_LSEEK;

			(UFDTArr[fd].ptrfiletable->writeOffset) = size;
		}
		else if( from == END)
		{
			if( ( UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize ) < ( (UFDTArr[fd].ptrfiletable->writeOffset) + size))
				return ERR_UNABLE_TO_PERFORM_LSEEK;
			if( ((UFDTArr[fd].ptrfiletable->writeOffset ) + size ) < 0 )
				return ERR_UNABLE_TO_PERFORM_LSEEK;

			(UFDTArr[fd].ptrfiletable->writeOffset) = ( UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize ) + size;
			
		}
	}
	return SUCCESS;
}

int truncateFile( char * name )
{
	int fd = GetFDFromName(name);
	if( fd == ERR_FILE_NOT_FOUND )
		return ERR_FILE_NOT_FOUND;

	memset( UFDTArr[fd].ptrfiletable->ptrinode->Buffer , 0 , 1025 );
	UFDTArr[fd].ptrfiletable -> readOffset = 0;
	UFDTArr[fd].ptrfiletable -> writeOffset = 0;
	UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = 0;
	return SUCCESS;
}