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




DWORD AccSize = 0;				/* 文件中的字节数 */
WORD AccFiles=0, AccDirs=0;		/* AccFiles 个file 及AccDirs 个folders */

//第一步必须在逻辑盘上注册一个工作区(文件系统),与逻辑驱动器号对应
FATFS Fatfs[_VOLUMES];			/* File system object for each logical drive 工作区*/
//第二步就可以到文件对象进行操作
FIL File[FILE_MAX];					/* File object */

//#if _USE_LFN
//char Lfname[_MAX_LFN+1];
//#endif






#if 0






//为文件系统分配工作空间
FATFS fs;         	/* Work area (file system object) for logical drives */
FIL file;      		/* file objects */

static FRESULT scan_files (TCHAR *path);

//测试函数执行结果分析
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

//磁盘格式化
FRESULT format_disk(void)
{
	//FATFS fs;
	FRESULT res;
	res = f_mount(0,&fs);
	if (res != FR_OK)
	{
	//	printf("\r\n挂载文件系统失败,错误代码: %u",res);
		return res;
	}	
//	printf("\r\n正在格式化磁盘,请稍候...");

	res = f_mkfs(0,1,4096);
	if (res == FR_OK)
	{
	//	printf("\r\n格式化成功...");
	}
	else
	{
	//	printf("\r\n格式化失败...");
	//	printf("\r\n错误代码: %u\r\n",res);
	}
	f_mount(0,NULL);
	return res;
}


/*
Available when _FS_READONLY == 0 and _FS_MINIMIZE == 0.
//目录创建,目录要一级级的建才会成功
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
	//FR_NO_PATH 路径不对
	//FR_EXIST 已存在
	//FR_OK
	return res;
}

/*
//删除一个目录,输入目录的路径;目录里的内容是空的才能删除, 与创建文件夹相反
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
	res = f_unlink(path);	//删除一个目录
	f_mount(0, NULL);
	//FR_NO_PATH		无此目录
	//FR_INVALID_NAME 	没有指定目录名及没有此目录
	//FR_DENIED       	目录不为空
	//FR_OK				OK
	return res;
}


/*----------------------------------------------------------------------
创建文件
文件名：如 "readme.txt"
如果该文件已存在，就创建失败。
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
	//创建一个新文件。如果文件已存在，则创建失败
    res = f_open(&file, FileName, FA_CREATE_NEW);  	//FA_CREATE_NEW		创建一个新文件。如果文件已存在，则创建失败。
                                                   	//FA_CREATE_ALWAYS 	创建一个新文件。如果文件已存在，则它将被截断并覆盖。
                                    				//FA_OPEN_ALWAYS 	如果文件存在，则打开；否则，创建一个新文件。

    /* Close open files */
    f_close(&file);

    /* Unregister work area prior to discard it */
    f_mount(0,NULL);
	//FR_EXIST 文件存在
	//FR_OK		创建成功
	return res;
}

/*----------------------------------------------------------------------
写文件
文件名：如有就写，没有就创建, 如果文件中有数据，那么就是文件最后写入数据
写数据   
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
	// Move to end of the file to append data 移动到文件的末尾追加数据
	//res=f_lseek(&file, file->fsize);   //出错
	//res = f_lseek(&file,(&file)->fsize); //正确  file->fsize file是指向结构的指针 
	res = f_lseek(&file, file.fsize);
	res = f_write(&file, buffer, len, &bw);
	//这也是写
	//n = f_puts("abcdefg123456\r\n", &file) ;  //、、file.fsize  file是结构体变量 
    /* Close open files */
    res = f_close(&file);

    /* Unregister work area prior to discard it */
    res = f_mount(0, NULL);

	return res;
}

/*
//写数据到文件，如果没有此文件则创建文件;输入文件的路径名，写开始偏移指针，内容缓存，写的字节数
Offset_WriteFile("readme.txt", 0, &temp, 1);   //将文件中的第一个字节修改成temp中的数据
*/
FRESULT Offset_WriteFile(const TCHAR* path, const uint32_t ofs, char *buff, const uint32_t strl)
{                                                                                                            
//	FATFS fs;            // Work area (file system object) for logical drive
//	FIL file;
    FRESULT res;         // FatFs function common result code
        
	UINT bw;             //文件写入的字节计数

    // Register a work area for logical drive 0
    f_mount(0, &fs);

    res = f_open(&file, path,  FA_OPEN_ALWAYS | FA_WRITE); 		//可写方式打开 没有文件则创建 

    res = f_lseek(&file, ofs); 									//指针移到文件ofs处；输入file.fsize指针移到文件最后 
    res = f_write(&file, buff, strl, &bw); 						//每次需要写入的数据字节数;如果btr为strlen(buff)则写入所有的字符

    f_close(&file);//关闭文件
    // Unregister a work area before discard it
    f_mount(0, NULL);
	return res;
}

#endif



/////////////////////////////////////////////////////注意是递归调用，小心爆RAM//////////////////////////////////////////////////////////////////////////////////

/*

将目录生成临时文件放在SD卡里面。然后每次关机之前删除这个文件，下次开机的时候在重新生成一下。
不用删...先判断sd卡有没有保存文件名的文件, 没有就创建一个, 最好创建一个适当大小的文件(防止文件碎片), 
扫描sd卡, 保存歌曲到文件中, 下一次开机, 直接覆盖保存. 要不然你老是删来删去造成文件碎片导致访问效率降低. 
*/

/*******************************************************************************
* Function Name  : scan_files
* Description    : 搜索文件目录下所有文件
* Input          : - path: 根目录
* Output         : None
* Return         : FRESULT
* Attention		 : 不支持长文件名
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
            if (res != FR_OK || fno.fname[0] == 0) //读取目录不成功或 (fno.fname[0] == 0)
				break;

            if (fno.fname[0] == '.') 
				continue;
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;	   //如果是，就用长文件名变量，如果不是就还是用短文件名变量。
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) 	  //判断目录项的形式，文件夹是AM_DIR、文件是AM_ARC
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
//获取目录中的文件,输入目录的路径;输出文件名和文件夹名
char name[10][13];
Get_ReadDir("0:/",name);   //读根目录所有文件及文件夹名称(只读"0:/"下面的内容)
*/
FRESULT Get_ReadDir(const TCHAR* path, 
					char (*filename)[13]) 	//文件名 8+.+3+'\0'
											//文件夹名 8字节    
{
    //FATFS fs;            // Work area (file system object) for logical drive
	DIR dir;
	FILINFO finf;
    FRESULT res;         	// FatFs function common result code

    f_mount(0, &fs);
    f_opendir (&dir, path);

    while( ((res = f_readdir(&dir, &finf))==FR_OK) && (finf.fname[0]) )	//获取目录中的文件        
    {																	//finf.fname[0] 不为空
		strcpy(*(filename++), finf.fname);
    	printf("%s",finf.fname);
    }
    f_mount(0, NULL);
    return res;
}


/////////////////////////////////////////////////////注意是递归调用，小心爆RAM//////////////////////////////////////////////////////////////////////////////////







/*******************************************************************************
* Function Name  : ReadFile
* Description    : 读文件内数据(只从文件开始的地方读一次数据，并不代表读完整个文件)
* Input          : - file_path: "0:/" 表示根目前,

*				   - buffer:	存储读到的数据
*				   - len		想要读取的文件长度
* Output         : None
* Return         : 返回实际读到的文件长度
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
	//打开存在的文件，如果没有则返回错误
	res = f_open(&file, file_path, FA_OPEN_EXISTING | FA_READ);	   	//FR_NO_FILE	文件不存在
																	//FR_OK			打开文件OK
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
	//返回-1表示文件不存在
	//返回0表示文件为空
	//返回>0表示读文件成功,并返回读到的文件长度
	return getFlieLen;			
}

/*
//读文件的数据，如果没有此文件则返回错误;输入文件的路径名，读取开始偏移指针，内容缓存，读取的字节数
   ReadFile("0:/readme.txt", 3, str, 1024);
*/
int32_t Offset_ReadFile(const TCHAR* file_path, uint32_t ofs, char *buffer, uint32_t len)
{                                                                                                 
//    FATFS fs;            // Work area (file system object) for logical drive
//    FIL file;            // file objects
    FRESULT res;         // FatFs function common result code
    UINT br;             // File R count 文件读回的字节计数
	int32_t getFlieLen = 0;	

    f_mount(0, &fs);

	//打开存在的文件，如果没有则返回错误
	res = f_open(&file, file_path, FA_OPEN_EXISTING | FA_READ);	   	//FR_NO_FILE	文件不存在
																	//FR_OK			打开文件OK
	if (FR_OK == res)
	{
		res = f_lseek(&file,ofs); 									//指针移到文件ofs个字节处
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
	//返回-1表示文件不存在或文件系统错误
	//返回0表示文件为空
	//返回>0表示读文件成功,并返回读到的文件长度
	return getFlieLen;	
}

/*
 //删除一个文件,输入文件的路径;
 DeleteFile("0:/readme.txt");
*/
FRESULT DeleteFile(const TCHAR* path)        
{
    //FATFS fs;            		// Work area (file system object) for logical drive
    DIR dir;
    FRESULT res;        	 	// FatFs function common result code

    f_mount(0, &fs);
    res = f_opendir(&dir, path);
    res = f_unlink(path);		//删除一个文件
    f_mount(0, NULL);
    return res;
}


/*******************************************************************************
* Function Name  : 磁盘侦测
* Description    : 文件空间占用情况及磁盘状态(获取卡的总容量及剩余容量)
* Input          : None
* Output         : None
* Return         : 返回磁盘状态
* Attention		 : None
*******************************************************************************/
FRESULT Disk_Detection(void)
{
	//FATFS fs;
	FATFS *pfs; 		
	FRESULT res; 		// FatFs function common result code
    DWORD fre_clust;   
	char str[128]; 

	pfs = &fs;			//指向
	f_mount(0, &fs);  	//安装FATFS，就是给FATFS分配空间
    res = f_getfree("0:", &fre_clust, &pfs);  /* 必须是根目录，选择磁盘0; "/"或者"0:/" */
	f_mount(0, NULL); 	//卸载FATFS，就是释放FATFS结构体所占空间

    if ( res==FR_OK ) 
    {
	  /* Print free space in unit of MB (assuming 4096 bytes/sector) */
      sprintf(str, "\r\n%d KB total drive space.\r\n"
           "%d KB available.\r\n",
           ( (pfs->n_fatent - 2) * pfs->csize )*4, 	//总空间
		   (fre_clust * pfs->csize)*4  ); 			//空闲磁盘空间
	}
	//FR_NO_FILESYSTEM  没有文件系统,需要格式化磁盘
	//FR_DISK_ERR		没有磁盘或磁盘损坏
	//FR_OK				磁盘OK
	return res;   
}


/*

    
* 功    能：获得指定路径下,指定格式的文件和文件夹的数量
*---------------------------------------------------------------------------*
* 入口参数：path  : 要检索的文件路径 ;
*             name : 要检索的文件扩展名
*             result : 储存结果的数据区指针;
*                       其中:result[0]:文件数量;result[1]:文件夹数量
* 出口参数：NULL
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
                         
                        i=strlen(fno.fname);        //得到当前查询的文件名长度
            
                   if(i>4)//文件名的长度必须大于4(例如*.txt)
                   {                        
                                   //从文件名的倒数第4个的位置开始比较字符串:比较文件的后缀
                                          //不区分大小写比较,格式:例如.MP3         
                            if(my_memicmp(&fno.fname[i-4],name,4)==0)   
                            {
                                     result[0]++;
                            }                              
                   }           
                 }                
         }       
         

*/
#endif
