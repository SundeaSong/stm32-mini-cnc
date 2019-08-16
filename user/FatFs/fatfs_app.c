							   /*****************************************************************************
 *   fatfs_app.c:  * C file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2010, wushuilin
 *   All rights reserved.
 *
 *   History
 *   2010.07.29  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "fatfs_app.h"




DWORD AccSize = 0;				/* �ļ��е��ֽ��� */
WORD AccFiles=0, AccDirs=0;		/* AccFiles ��file ��AccDirs ��folders */

//��һ���������߼�����ע��һ��������(�ļ�ϵͳ),���߼��������Ŷ�Ӧ
FATFS Fatfs[_VOLUMES];			/* File system object for each logical drive ������*/
//�ڶ����Ϳ��Ե��ļ�������в���
FIL File[FILE_MAX];					/* File object */

//#if _USE_LFN
//char Lfname[_MAX_LFN+1];
//#endif






#if 0






//Ϊ�ļ�ϵͳ���乤���ռ�
FATFS fs;         	/* Work area (file system object) for logical drives */
FIL file;      		/* file objects */

static FRESULT scan_files (TCHAR *path);

//���Ժ���ִ�н������
void die(FRESULT res)
{
	uint8_t i;
    switch(res)
	{
	    case FR_OK:					/* (0) Succeeded */
			i++;
		break;

		case FR_DISK_ERR:			/* (1) A hard error occured in the low level disk I/O layer */
			i++;
		break;

		case FR_INT_ERR:			/* (2) Assertion failed */
			i++;
		break;

		case FR_NOT_READY:			/* (3) The physical drive cannot work */
			i++;
		break;	

		case FR_NO_FILE:			/* (4) Could not find the file */
			i++;
		break;	
	
		case FR_NO_PATH:			/* (5) Could not find the path */
			i++;
		break;	

		case FR_INVALID_NAME:		/* (6) The path name format is invalid */
			i++;
		break;		
	
		case FR_DENIED:				/* (7) Acces denied due to prohibited access or directory full */
			i++;
		break;	

		case FR_EXIST:				/* (8) Acces denied due to prohibited access */
			i++;
		break;		
			
		case FR_INVALID_OBJECT:		/* (9) The file/directory object is invalid */
			i++;
		break;		

		case FR_WRITE_PROTECTED:	/* (10) The physical drive is write protected */
			i++;
		break;
		
		case FR_INVALID_DRIVE:		/* (11) The logical drive number is invalid */
			i++;
		break;		
			
		case FR_NOT_ENABLED:		/* (12) The volume has no work area */
			i++;
		break;	
	
		case FR_NO_FILESYSTEM:		/* (13) There is no valid FAT volume on the physical drive */
			i++;
		break;
		
		case FR_MKFS_ABORTED:		/* (14) The f_mkfs() aborted due to any parameter error */
			i++;
		break;		
		
		case FR_TIMEOUT:			/* (15) Could not get a grant to access the volume within defined period */
			i++;
		break;			
	
		case FR_LOCKED:				/* (16) The operation is rejected according to the file shareing policy */
			i++;
		break;	
	
		case FR_NOT_ENOUGH_CORE:	/* (17) LFN working buffer could not be allocated */
			i++;
		break;

		case FR_TOO_MANY_OPEN_FILES:/* (18) Number of open files > _FS_SHARE */
			i++;
		break;	
	
		default:	
			i++;
		break;	
	}
}

//���̸�ʽ��
FRESULT format_disk(void)
{
	//FATFS fs;
	FRESULT res;
	res = f_mount(0,&fs);
	if (res != FR_OK)
	{
	//	printf("\r\n�����ļ�ϵͳʧ��,�������: %u",res);
		return res;
	}	
//	printf("\r\n���ڸ�ʽ������,���Ժ�...");

	res = f_mkfs(0,1,4096);
	if (res == FR_OK)
	{
	//	printf("\r\n��ʽ���ɹ�...");
	}
	else
	{
	//	printf("\r\n��ʽ��ʧ��...");
	//	printf("\r\n�������: %u\r\n",res);
	}
	f_mount(0,NULL);
	return res;
}


/*
Available when _FS_READONLY == 0 and _FS_MINIMIZE == 0.
//Ŀ¼����,Ŀ¼Ҫһ�����Ľ��Ż�ɹ�
Example
    res = f_mkdir("0:/sub1");
    res = f_mkdir("0:/sub1/sub2");
    res = f_mkdir("0:/sub1/sub2/sub3");

	CreateFolder("0:/readme");
	CreateFolder("0:/readme/123");
*/
FRESULT CreateFolder(const TCHAR *FolderName)
{
	//FATFS fs;
	FRESULT res;	
	f_mount(0,&fs);
	res = f_mkdir(FolderName);
	f_mount(0,NULL);
	//FR_NO_PATH ·������
	//FR_EXIST �Ѵ���
	//FR_OK
	return res;
}

/*
//ɾ��һ��Ŀ¼,����Ŀ¼��·��;Ŀ¼��������ǿյĲ���ɾ��, �봴���ļ����෴
	DeleteDir("0:/readme/123");
	DeleteDir("0:/readme");
*/

FRESULT DeleteDir(const TCHAR* path)        
{
    //FATFS fs;            	// Work area (file system object) for logical drive
    DIR dir;
    FRESULT res;         	// FatFs function common result code

	f_mount(0, &fs);
	res = f_opendir (&dir, path);
	res = f_unlink(path);	//ɾ��һ��Ŀ¼
	f_mount(0, NULL);
	//FR_NO_PATH		�޴�Ŀ¼
	//FR_INVALID_NAME 	û��ָ��Ŀ¼����û�д�Ŀ¼
	//FR_DENIED       	Ŀ¼��Ϊ��
	//FR_OK				OK
	return res;
}


/*----------------------------------------------------------------------
�����ļ�
�ļ������� "readme.txt"
������ļ��Ѵ��ڣ��ʹ���ʧ�ܡ�
CreateFile("0:/readme.txt");                                      
-----------------------------------------------------------------------*/
FRESULT CreateFile(const TCHAR* FileName)
{
    //FATFS fs;         	/* Work area (file system object) for logical drives */
    //FIL file;      		/* file objects */
    FRESULT res;         /* FatFs function common result code */

    /* Register work area for logical drives */
    f_mount(0, &fs);

    /* Open source file on the drive 0 
	FA_READ 			Specifies read access to the object. Data can be read from the file.
						Combine with FA_WRITE for read-write access. 
	FA_WRITE 			Specifies write access to the object. Data can be written to the file.
						Combine with FA_READ for read-write access. 
	FA_OPEN_EXISTING 	Opens the file. The function fails if the file is not existing. (Default) 
	FA_OPEN_ALWAYS 		Opens the file, if it is existing. If not, a new file is created. 
	FA_CREATE_NEW 		Creates a new file. The function fails if the file is already existing. 
	FA_CREATE_ALWAYS 	Creates a new file. If the file is existing, it is truncated and overwritten. 
*/
	//����һ�����ļ�������ļ��Ѵ��ڣ��򴴽�ʧ��
    res = f_open(&file, FileName, FA_CREATE_NEW);  	//FA_CREATE_NEW		����һ�����ļ�������ļ��Ѵ��ڣ��򴴽�ʧ�ܡ�
                                                   	//FA_CREATE_ALWAYS 	����һ�����ļ�������ļ��Ѵ��ڣ����������ضϲ����ǡ�
                                    				//FA_OPEN_ALWAYS 	����ļ����ڣ���򿪣����򣬴���һ�����ļ���

    /* Close open files */
    f_close(&file);

    /* Unregister work area prior to discard it */
    f_mount(0,NULL);
	//FR_EXIST �ļ�����
	//FR_OK		�����ɹ�
	return res;
}

/*----------------------------------------------------------------------
д�ļ�
�ļ��������о�д��û�оʹ���, ����ļ��������ݣ���ô�����ļ����д������
д����   
    WriteFile("readme.txt", "123456789", strlen("123456789"));                               
-----------------------------------------------------------------------*/
#if _FS_READONLY == 0
FRESULT WriteFile(const TCHAR* FileName, const char *buffer, const uint32_t len)
{
    //FATFS fs;         	/* Work area (file system object) for logical drives */
    //FIL file;      		/* file objects */
    FRESULT res;        /* FatFs function common result code */
    UINT bw;         	/* File R/W count */

    /* Register work area for logical drives */
    res = f_mount(0, &fs);

    /* Open source file on the drive 0 */
    res = f_open(&file, FileName, FA_OPEN_ALWAYS | FA_WRITE);  
	// Move to end of the file to append data �ƶ����ļ���ĩβ׷������
	//res=f_lseek(&file, file->fsize);   //����
	//res = f_lseek(&file,(&file)->fsize); //��ȷ  file->fsize file��ָ��ṹ��ָ�� 
	res = f_lseek(&file, file.fsize);
	res = f_write(&file, buffer, len, &bw);
	//��Ҳ��д
	//n = f_puts("abcdefg123456\r\n", &file) ;  //����file.fsize  file�ǽṹ����� 
    /* Close open files */
    res = f_close(&file);

    /* Unregister work area prior to discard it */
    res = f_mount(0, NULL);

	return res;
}

/*
//д���ݵ��ļ������û�д��ļ��򴴽��ļ�;�����ļ���·������д��ʼƫ��ָ�룬���ݻ��棬д���ֽ���
Offset_WriteFile("readme.txt", 0, &temp, 1);   //���ļ��еĵ�һ���ֽ��޸ĳ�temp�е�����
*/
FRESULT Offset_WriteFile(const TCHAR* path, const uint32_t ofs, char *buff, const uint32_t strl)
{                                                                                                            
//	FATFS fs;            // Work area (file system object) for logical drive
//	FIL file;
    FRESULT res;         // FatFs function common result code
        
	UINT bw;             //�ļ�д����ֽڼ���

    // Register a work area for logical drive 0
    f_mount(0, &fs);

    res = f_open(&file, path,  FA_OPEN_ALWAYS | FA_WRITE); 		//��д��ʽ�� û���ļ��򴴽� 

    res = f_lseek(&file, ofs); 									//ָ���Ƶ��ļ�ofs��������file.fsizeָ���Ƶ��ļ���� 
    res = f_write(&file, buff, strl, &bw); 						//ÿ����Ҫд��������ֽ���;���btrΪstrlen(buff)��д�����е��ַ�

    f_close(&file);//�ر��ļ�
    // Unregister a work area before discard it
    f_mount(0, NULL);
	return res;
}

#endif



/////////////////////////////////////////////////////ע���ǵݹ���ã�С�ı�RAM//////////////////////////////////////////////////////////////////////////////////

/*

��Ŀ¼������ʱ�ļ�����SD�����档Ȼ��ÿ�ιػ�֮ǰɾ������ļ����´ο�����ʱ������������һ�¡�
����ɾ...���ж�sd����û�б����ļ������ļ�, û�оʹ���һ��, ��ô���һ���ʵ���С���ļ�(��ֹ�ļ���Ƭ), 
ɨ��sd��, ����������ļ���, ��һ�ο���, ֱ�Ӹ��Ǳ���. Ҫ��Ȼ������ɾ��ɾȥ����ļ���Ƭ���·���Ч�ʽ���. 
*/

/*******************************************************************************
* Function Name  : scan_files
* Description    : �����ļ�Ŀ¼�������ļ�
* Input          : - path: ��Ŀ¼
* Output         : None
* Return         : FRESULT
* Attention		 : ��֧�ֳ��ļ���
*******************************************************************************/
char filename[128];		//8+.+3+'\0'
FRESULT scan_files (TCHAR *path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;

#if _USE_LFN   //_USE_LFN > 0
	#define _DF1S	0x81
	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
	fno.lfname = lfn;
	fno.lfsize = sizeof(lfn);
#endif
 
    res = f_opendir(&dir, path);
    if (res == FR_OK) 
	{
        i = strlen(path);
        for (;;) 
		{
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) //��ȡĿ¼���ɹ��� (fno.fname[0] == 0)
				break;

            if (fno.fname[0] == '.') 
				continue;
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;	   //����ǣ����ó��ļ���������������Ǿͻ����ö��ļ���������
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) 	  //�ж�Ŀ¼�����ʽ���ļ�����AM_DIR���ļ���AM_ARC
			{
                sprintf(&path[i], "/%s", fn);
//				WriteFile("FilesLog.txt", path, strlen(path));
//				WriteFile("FilesLog.txt", "\r\n", 1);
                res = scan_files(path);
                if (res != FR_OK) 
					break;
                path[i] = 0;
            } 
			else 
			{
                sprintf(filename, "%s/%s\r\n", path, fn);
				//WriteFile("FileLog.txt", filename, strlen(filename));
            }
        }
    }
    return res;
}

///////////////


/////////////////


/*
char temp[128]="0:";
ScanFiles(temp);
*/
FRESULT ScanFiles (TCHAR *path)
{
	//FATFS fs;
	FRESULT res;
	f_mount(0, &fs);
	res = scan_files(path);
	f_mount(0, NULL);
	return res;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

/*
//��ȡĿ¼�е��ļ�,����Ŀ¼��·��;����ļ������ļ�����
char name[10][13];
Get_ReadDir("0:/",name);   //����Ŀ¼�����ļ����ļ�������(ֻ��"0:/"���������)
*/
FRESULT Get_ReadDir(const TCHAR* path, 
					char (*filename)[13]) 	//�ļ��� 8+.+3+'\0'
											//�ļ����� 8�ֽ�    
{
    //FATFS fs;            // Work area (file system object) for logical drive
	DIR dir;
	FILINFO finf;
    FRESULT res;         	// FatFs function common result code

    f_mount(0, &fs);
    f_opendir (&dir, path);

    while( ((res = f_readdir(&dir, &finf))==FR_OK) && (finf.fname[0]) )	//��ȡĿ¼�е��ļ�        
    {																	//finf.fname[0] ��Ϊ��
		strcpy(*(filename++), finf.fname);
    	printf("%s",finf.fname);
    }
    f_mount(0, NULL);
    return res;
}


/////////////////////////////////////////////////////ע���ǵݹ���ã�С�ı�RAM//////////////////////////////////////////////////////////////////////////////////







/*******************************************************************************
* Function Name  : ReadFile
* Description    : ���ļ�������(ֻ���ļ���ʼ�ĵط���һ�����ݣ�����������������ļ�)
* Input          : - file_path: "0:/" ��ʾ��Ŀǰ,

*				   - buffer:	�洢����������
*				   - len		��Ҫ��ȡ���ļ�����
* Output         : None
* Return         : ����ʵ�ʶ������ļ�����
* Attention		 : 	ReadFile("0:/readme.txt", str, 1024);
*******************************************************************************/
uint32_t ReadFile(const TCHAR *file_path, uint8_t *buffer, const uint32_t len)
{
//  FATFS fs;         	/* Work area (file system object) for logical drives */
//  FIL file;      		/* file objects */
    FRESULT res;        /* FatFs function common result code */
    UINT br;         	/* File R/W count */
	int32_t getFlieLen = 0;	

	f_mount(0, &fs);
	//�򿪴��ڵ��ļ������û���򷵻ش���
	res = f_open(&file, file_path, FA_OPEN_EXISTING | FA_READ);	   	//FR_NO_FILE	�ļ�������
																	//FR_OK			���ļ�OK
	if (FR_OK == res)
	{
        res = f_read(&file, buffer, len, &br);
        if (FR_OK == res)
		{		
			getFlieLen += br;
		}
					  
		f_close(&file);
	}
	else
	{
		getFlieLen = -1;	
	}
	f_mount(0, NULL);
	//����-1��ʾ�ļ�������
	//����0��ʾ�ļ�Ϊ��
	//����>0��ʾ���ļ��ɹ�,�����ض������ļ�����
	return getFlieLen;			
}

/*
//���ļ������ݣ����û�д��ļ��򷵻ش���;�����ļ���·��������ȡ��ʼƫ��ָ�룬���ݻ��棬��ȡ���ֽ���
   ReadFile("0:/readme.txt", 3, str, 1024);
*/
int32_t Offset_ReadFile(const TCHAR* file_path, uint32_t ofs, char *buffer, uint32_t len)
{                                                                                                 
//    FATFS fs;            // Work area (file system object) for logical drive
//    FIL file;            // file objects
    FRESULT res;         // FatFs function common result code
    UINT br;             // File R count �ļ����ص��ֽڼ���
	int32_t getFlieLen = 0;	

    f_mount(0, &fs);

	//�򿪴��ڵ��ļ������û���򷵻ش���
	res = f_open(&file, file_path, FA_OPEN_EXISTING | FA_READ);	   	//FR_NO_FILE	�ļ�������
																	//FR_OK			���ļ�OK
	if (FR_OK == res)
	{
		res = f_lseek(&file,ofs); 									//ָ���Ƶ��ļ�ofs���ֽڴ�
        res = f_read(&file, buffer, len, &br);
        if (FR_OK == res)
		{		
			getFlieLen += br;
		}
					  
		f_close(&file);
	}
	else
	{
		getFlieLen = -1;	
	}
	f_mount(0, NULL);
	//����-1��ʾ�ļ������ڻ��ļ�ϵͳ����
	//����0��ʾ�ļ�Ϊ��
	//����>0��ʾ���ļ��ɹ�,�����ض������ļ�����
	return getFlieLen;	
}

/*
 //ɾ��һ���ļ�,�����ļ���·��;
 DeleteFile("0:/readme.txt");
*/
FRESULT DeleteFile(const TCHAR* path)        
{
    //FATFS fs;            		// Work area (file system object) for logical drive
    DIR dir;
    FRESULT res;        	 	// FatFs function common result code

    f_mount(0, &fs);
    res = f_opendir(&dir, path);
    res = f_unlink(path);		//ɾ��һ���ļ�
    f_mount(0, NULL);
    return res;
}


/*******************************************************************************
* Function Name  : �������
* Description    : �ļ��ռ�ռ�����������״̬(��ȡ������������ʣ������)
* Input          : None
* Output         : None
* Return         : ���ش���״̬
* Attention		 : None
*******************************************************************************/
FRESULT Disk_Detection(void)
{
	//FATFS fs;
	FATFS *pfs; 		
	FRESULT res; 		// FatFs function common result code
    DWORD fre_clust;   
	char str[128]; 

	pfs = &fs;			//ָ��
	f_mount(0, &fs);  	//��װFATFS�����Ǹ�FATFS����ռ�
    res = f_getfree("0:", &fre_clust, &pfs);  /* �����Ǹ�Ŀ¼��ѡ�����0; "/"����"0:/" */
	f_mount(0, NULL); 	//ж��FATFS�������ͷ�FATFS�ṹ����ռ�ռ�

    if ( res==FR_OK ) 
    {
	  /* Print free space in unit of MB (assuming 4096 bytes/sector) */
      sprintf(str, "\r\n%d KB total drive space.\r\n"
           "%d KB available.\r\n",
           ( (pfs->n_fatent - 2) * pfs->csize )*4, 	//�ܿռ�
		   (fre_clust * pfs->csize)*4  ); 			//���д��̿ռ�
	}
	//FR_NO_FILESYSTEM  û���ļ�ϵͳ,��Ҫ��ʽ������
	//FR_DISK_ERR		û�д��̻������
	//FR_OK				����OK
	return res;   
}


/*

    
* ��    �ܣ����ָ��·����,ָ����ʽ���ļ����ļ��е�����
*---------------------------------------------------------------------------*
* ��ڲ�����path  : Ҫ�������ļ�·�� ;
*             name : Ҫ�������ļ���չ��
*             result : ��������������ָ��;
*                       ����:result[0]:�ļ�����;result[1]:�ļ�������
* ���ڲ�����NULL
****************************************************************************

void Ex_GetFileAndDir_Number(char *path,char *name,INT16U *result)
{                
         FILINFO fno;        
         BYTE res,i;        
         
         
         result[0] = result[1] = 0;        
         
         res = f_opendir(&search_dir, path);        
         
         if (res) return;                    
         
         for(;;) {                
                 
                 res = f_readdir(&search_dir, &fno);
                 
                 if ((res != FR_OK) || !fno.fname[0]) 
                   break;
                   
                 if (fno.fattrib & AM_DIR) {
                         result[1]++;
                 } else {                        
                         
                        i=strlen(fno.fname);        //�õ���ǰ��ѯ���ļ�������
            
                   if(i>4)//�ļ����ĳ��ȱ������4(����*.txt)
                   {                        
                                   //���ļ����ĵ�����4����λ�ÿ�ʼ�Ƚ��ַ���:�Ƚ��ļ��ĺ�׺
                                          //�����ִ�Сд�Ƚ�,��ʽ:����.MP3         
                            if(my_memicmp(&fno.fname[i-4],name,4)==0)   
                            {
                                     result[0]++;
                            }                              
                   }           
                 }                
         }       
         

*/
#endif
