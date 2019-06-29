//Main.cpp

#include "Header.h"
extern UFDT UFDTArr[MAX_UFDT];		//One Process can open MAX_UFDT
extern SUPERBLOCK SUPERBOLCKobj;
extern PINODE head;

int main()
{
	char command[4][80];
	char str[80];
	int count;
	int ret;
	int fd ;
	char arr[1024];
	char *ptr;

	// Initialise Super Block and DILB
	InitialiseSuperBlock();
	CreateDILB();

	// Create shell process
	SetUareaForProcess();
	
	while(1)
	{
		fflush(stdin);
		strcpy_s(str,"");

		printf("\nUnix File System: > ");
		fgets(str,80,stdin);

		count = sscanf(str , "%s%s%s%s",command[0],command[1],command[2],command[3]);

		if(count == 1)
		{
			if(_stricmp( command[0] , "help" ) == 0 )
			{
				DisplayHelp();
				continue;
			}
			else if( _stricmp( command[0] , "clear") == 0 )
			{
				system("cls");
				continue;
			}
			else if( _stricmp( command[0] , "exit") == 0 )
			{
				printf("Terminating the Marvallous Virtual File System\n");
				break;
			}
			else if( _stricmp( command[0] , "ls") == 0 )
			{
				ls_file();
				continue;
			}
			else if( _stricmp( command[0] , "closeall") == 0 )
			{
				CloseAllFile();
				continue;
			}
			else
			{
				printf("ERROR: Command not found !!! \n");
				continue;
			}
		}
		else if( count == 2 )
		{
			if( _stricmp( command[0] , "man" ) == 0 )
			{
				man(command[1]);
				continue;
			}
			else if( _stricmp( command[0] , "close") == 0 )
			{
				ret = CloseFileByName( command[1] );
				if( ret == SUCCESS )
					printf("Files is successfully Close\n");
				else
					DisplayError(ret);
				continue;
			}
			else if( _stricmp( command[0] , "rm") == 0 )
			{
				ret = rm_File( command[1] );
				if( ret == SUCCESS )
					printf("Files is successfully Remove\n");
				else
					DisplayError(ret);
				continue;
			}
			else if( _stricmp( command[0] , "stat") == 0 )
			{
				ret = StatFile( command[1] );
				if( ret != SUCCESS )
					DisplayError(ret);
				continue;
			}
			else if( _stricmp( command[0] , "fstat") == 0 )
			{
				ret = FstatFile( atoi(command[1]) );
				if( ret != SUCCESS )
					DisplayError(ret);
				continue;
			}
			else if( _stricmp( command[0] , "write") == 0 )
			{
				fd = GetFDFromName(command[1]);
				if( fd == ERR_FILE_NOT_FOUND )
				{
					DisplayError(fd);
					continue;
				}
				printf("Enter the data: \n");
				scanf("%[^\n]",arr);

				ret = strlen(arr);
				if( ret == 0 )
				{
					continue;
				}

				ret = WriteFile( fd , arr , ret );

				if( ret != SUCCESS )
					DisplayError(ret);
				else
					printf("Successfully write\n");
				continue;
			}
			else if( _stricmp( command[0] , "truncate") == 0 )
			{
				ret = truncateFile( command[1] );
				if( ret != SUCCESS )
					DisplayError(ret);
				continue;
			}
			else
			{
				printf("ERROR: Command not found !!! \n");
				continue;
			}
		}
		else if(count == 3)
		{
			if( _stricmp( command[0] , "create") == 0 )
			{
				ret = CreateFile(command[1] , atoi(command[2]) ); 
				if( ret >= 0 )
				{
					printf("FILE is successfully created with file descriptor: %d\n",ret);
				}
				else
					DisplayError(ret);
				continue;
			}else if( _stricmp( command[0] , "open") == 0 )
			{
				ret = OpenFile( command[1] , atoi( command[2] ) );
				if( ret >= 0 )
				{
					printf("FILE is successfully created with file descriptor: %d\n",ret);
				}
				else
					DisplayError(ret);
				continue;
			}
			else if( _stricmp( command[0] , "read") == 0 )
			{
				fd = GetFDFromName(command[1]);
				if( fd == ERR_FILE_NOT_FOUND )
				{
					DisplayError(fd);
					continue;
				}
				
				ptr = (char * )malloc( atoi(command[2]) + 1 );
				if( ptr == NULL )
				{
					printf("Error\n");
					continue;
				}

				ret = ReadFile( fd , ptr , atoi(command[2]));

				if( ret > 0 )
					_write(2,ptr,ret);
				else
					DisplayError(ret);
				continue;
			}
			else
			{
				printf("ERROR: Command not found !!! \n");
				continue;
			}
		}else if( count == 4)
		{
			if( _stricmp( command[0] , "lseek") == 0 )
			{
				fd = GetFDFromName(command[1]);
				if( fd == ERR_FILE_NOT_FOUND )
				{
					DisplayError(fd);
					continue;
				}
				
				ret = LseekFile( fd , atoi(command[2]) , atoi(command[3]) );

				if( ret == SUCCESS )
				{
					printf("Successfullt seek\n");
				}	
				else
					DisplayError(ret);
				continue;
			}
			else
			{
				printf("ERROR: Command not found !!! \n");
				continue;
			}

		}else
		{
			printf("ERROR: Command not found !!! \n");
			continue;
		}

	
	}

}