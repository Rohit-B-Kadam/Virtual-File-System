//InfoDisplay.cpp

#include "Header.h"
extern UFDT UFDTArr[MAX_UFDT];		//One Process can open MAX_UFDT
extern SUPERBLOCK SUPERBOLCKobj;
extern PINODE head;

void DisplayHelp()
{
	printf("ls         : To List out all Files\n");
	printf("clear      : To Clear console\n");
	printf("create     : To Create the new file\n");
	printf("rm         : To Delete the file\n");
	printf("open       : To Open the file\n");
	printf("close      : To Close the file\n");
	printf("closeall   : To Close all opened file\n");
	printf("read       : To Read the contents from file\n");
	printf("write      : To Write contents into file\n");
	printf("stat       : To Display information of file using name\n");
	printf("fstat      : To Display information of file using file descriptor\n");
	printf("truncate   : To Remove all data from file\n");
	printf("exit       : To Terminate file system\n");

}

void man( char* name)
{
	if(name == NULL )	return;
	if( _stricmp( name , "create" ) == 0 )
	{
		printf("Description : Used to create new regular file\n");
		printf("Usage       : create File_name Permission\n");
		printf("                Permission can be  1 2 3");
	}
	else if( _stricmp( name , "read" ) == 0 )
	{
		printf("Description : Used to read data from regular file\n");
		printf("Usage       : read File_name No_of_byte_to_Read\n");
	}
	else if( _stricmp( name , "write" ) == 0 )
	{
		printf("Description : Used to write  into regular file\n");
		printf("Usage       : write File_name\n");
		printf("               After this enter the data that we want to write\n");
	}
	else if( _stricmp( name , "ls" ) == 0 )
	{
		printf("Description : Used to list all information of files\n");
		printf("Usage       : ls\n");
	}
	else if( _stricmp( name , "stat" ) == 0 )
	{
		printf("Description : Used to display information of file\n");
		printf("Usage       : stat File_name\n");
	}
	else if( _stricmp( name , "fstat" ) == 0 )
	{
		printf("Description : Used to display information of file\n");
		printf("Usage       : fstat File_descriptor\n");
	}
	else if( _stricmp( name , "truncate" ) == 0 )
	{
		printf("Description : Used to remove data from file\n");
		printf("Usage       : truncate File_name\n");
	}
	else if( _stricmp( name , "open" ) == 0 )
	{
		printf("Description : Used to open existing file\n");
		printf("Usage       : open File_name mode\n");
	}
	else if( _stricmp( name , "close" ) == 0 )
	{
		printf("Description : Used to close opened file\n");
		printf("Usage       : close File_name\n");
	}
	else if( _stricmp( name , "closeall" ) == 0 )
	{
		printf("Description : Used to close all opened file\n");
		printf("Usage       : closeall\n");
	}
	else if( _stricmp( name , "lseek" ) == 0 )
	{
		printf("Description : Used to change file offset\n");
		printf("Usage       : lseek File_name Change_In_Offset StarPoint\n");
		printf("                StarPoint can be START 0 CURRENT 1 END 2\n");
	}
	else if( _stricmp( name , "rm" ) == 0 )
	{
		printf("Description : Used to delete the file\n");
		printf("Usage       : rm File_name\n");
	}
	else
	{
		printf("ERROR: No manual entry available.");
	}

}

void DisplayError( int errorCode )
{
	switch(errorCode)
	{
	case  ERR_INCORRECT_PARMETER :	
		printf("ERROR: InCorrect Paramter\n");
		break;

	case ERR_INODE_FULL :
		printf("ERROR: InNode is Full\n");
		break;

	case  ERR_FILE_EXISTS :
		printf("ERROR: File is Already exist\n");
		break;

	case ERR_MEMORY_ALLOCATION_FAIL:
		printf("ERROR: Memory allocation fail\n");
		break;

	case ERR_UFDT_FULL :
		printf("ERROR: Process UFDT is Full\n");
		break;	

	case ERR_FILE_NOT_FOUND :
		printf("ERROR: File no found\n");
		break;

	case ERR_FILE_IS_NOT_CLOSE :
		printf("ERROR: File is not close\n");
		break;

	case ERR_PERMISSION_DENIED :
		printf("ERROR: Permission denied\n");
		break;

	case ERR_IN_SUFFICIENT_MEMORY :
		printf("ERROR: Insufficients Memory\n");
		break;

	case ERR_REACH_AT_END :
		printf("ERROR: file is reach at end\n");
		break;

	case ERR_FILE_ALREADY_OPEN :
		printf("ERROR: File is already open\n");
		break;

	case ERR_UNABLE_TO_PERFORM_LSEEK :
		printf("ERROR: Unacle to perform lseef\n");
		break;

	}
}