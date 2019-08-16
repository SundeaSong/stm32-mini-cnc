							/*****************************************************************************
 *   fatfs_app.h  Header file for NXP LPC1xxx Family Microprocessors
 *
 *   Copyright(C) 2008, wushuilin
 *   All rights reserved.
 *
 *   History
 *   2010.07.25  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __fatfs_app_H 
#define __fatfs_app_H

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "ff.h"				/* FatFs configurations and declarations */
#include "diskio.h"		/* Declarations of low level disk I/O functions */


#define FILE_MAX	2

extern DWORD AccSize;				/* Work register for fs command */
extern WORD AccFiles, AccDirs;		/* 文件目录及文件计数 */
extern FATFS Fatfs[_VOLUMES];		/* File system object for each logical drive */
extern FIL File[FILE_MAX];				/* File object */



//extern FRESULT format_disk(void);
//extern FRESULT CreateFolder(const TCHAR *FolderName);
//extern FRESULT CreateFile(const TCHAR* FileName);

////文件写
//#if _FS_READONLY == 0
//extern FRESULT WriteFile(const TCHAR* FileName, const char *buffer, const uint32_t len);
//extern FRESULT Offset_WriteFile(const TCHAR* path, const uint32_t ofs, char *buff, const uint32_t strl);
//#endif
////文件读
//extern uint32_t ReadFile(const TCHAR *file_path, uint8_t *buffer, const uint32_t len);
//extern int32_t Offset_ReadFile(const TCHAR* file_path, uint32_t ofs, char *buffer, uint32_t len);
////删除文件
//extern FRESULT DeleteFile(const TCHAR* path);
//extern FRESULT DeleteDir(const TCHAR* path);

//extern FRESULT ScanFiles(TCHAR *path);
//extern FRESULT Disk_Detection(void);
//extern FRESULT Get_ReadDir(const TCHAR* path, char (*filename)[13]);


#endif /* end */
/*****************************************************************************
**                            End Of File
******************************************************************************/
