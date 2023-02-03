/////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2002-2013 UltraVNC Team Members. All Rights Reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
//  USA.
//
// If the source code for the program is not available from the place from
// which you received this file, check 
// http://www.uvnc.com/
//
////////////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "inifile.h"

char *g_szIniFile = 0;

#ifdef ULTRAVNC_VEYON_SUPPORT
extern BOOL ultravnc_veyon_load_int(LPCSTR valname, int *out);
extern void ultravnc_veyon_load_password(char* out, int size);
#endif

IniFile::IniFile()
{
#ifndef ULTRAVNC_VEYON_SUPPORT
	if(g_szIniFile)
	{
		strcpy_s(myInifile,g_szIniFile);
	}
	else
	{
     char WORKDIR[MAX_PATH];
	if (GetModuleFileName(NULL, WORKDIR, MAX_PATH))
		{
		char* p = strrchr(WORKDIR, '\\');
		if (p == NULL) return;
		*p = '\0';
		}
	strcpy_s(myInifile,"");
	strcat_s(myInifile,WORKDIR);//set the directory
	strcat_s(myInifile,"\\");
	strcat_s(myInifile,INIFILE_NAME);
}
#endif
}

void
IniFile::IniFileSetSecure()
{
#ifndef ULTRAVNC_VEYON_SUPPORT
	if(g_szIniFile)
	{
		strcpy_s(myInifile,g_szIniFile);
	}
	else
	{
char WORKDIR[MAX_PATH];
	if (GetModuleFileName(NULL, WORKDIR, MAX_PATH))
		{
		char* p = strrchr(WORKDIR, '\\');
		if (p == NULL) return;
		*p = '\0';
		}
	strcpy_s(myInifile,"");
	strcat_s(myInifile,WORKDIR);//set the directory
	strcat_s(myInifile,"\\");
	strcat_s(myInifile,INIFILE_NAME);
}
#endif
}

void
IniFile::IniFileSetTemp(char *lpCmdLine)
{
#ifndef ULTRAVNC_VEYON_SUPPORT
	strcpy_s(myInifile,260,lpCmdLine);
#endif
}

void
IniFile::copy_to_secure()
{
#ifndef ULTRAVNC_VEYON_SUPPORT
	{
		char dir[MAX_PATH];

		char exe_file_name[MAX_PATH];
		GetModuleFileName(0, exe_file_name, MAX_PATH);

		strcpy_s(dir, exe_file_name);
		strcat_s(dir, " -settingshelper");
		strcat_s(dir, ":");
		strcat_s(dir, myInifile);

		STARTUPINFO          StartUPInfo;
		PROCESS_INFORMATION  ProcessInfo;
		HANDLE Token=NULL;
		HANDLE process=NULL;
		ZeroMemory(&StartUPInfo,sizeof(STARTUPINFO));
		ZeroMemory(&ProcessInfo,sizeof(PROCESS_INFORMATION));
		StartUPInfo.wShowWindow = SW_SHOW;
		StartUPInfo.lpDesktop = "Winsta0\\Default";
		StartUPInfo.cb = sizeof(STARTUPINFO);
		HWND tray = FindWindow(("Shell_TrayWnd"), 0);
		if (!tray)
			goto error;

		DWORD processId = 0;
			GetWindowThreadProcessId(tray, &processId);
		if (!processId)
			goto error;
		process = OpenProcess(MAXIMUM_ALLOWED, FALSE, processId);
		if (!process)
			goto error;
		OpenProcessToken(process, MAXIMUM_ALLOWED, &Token);
		CreateProcessAsUser(Token,NULL,dir,NULL,NULL,FALSE,DETACHED_PROCESS,NULL,NULL,&StartUPInfo,&ProcessInfo);
		DWORD error=GetLastError();
		if (process) CloseHandle(process);
		if (Token) CloseHandle(Token);
		if (ProcessInfo.hThread) CloseHandle (ProcessInfo.hThread);
		if (ProcessInfo.hProcess) CloseHandle (ProcessInfo.hProcess);
		if (error == 1314) goto error;
		return;
	}
		error:
		settingsHelpers::Set_settings_as_admin(myInifile);
#endif
}

IniFile::~IniFile()
{
}

bool
IniFile::WriteString(char *key1, char *key2,char *value)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	return true;
#else
	//vnclog.Print(LL_INTERR, VNCLOG("%s \n"),myInifile);
	return (FALSE != WritePrivateProfileString(key1,key2, value,myInifile));
#endif
}

bool
IniFile::WriteInt(char *key1, char *key2,int value)
{
#ifndef ULTRAVNC_VEYON_SUPPORT
	char       buf[32];
	wsprintf(buf, "%d", value);
	//vnclog.Print(LL_INTERR, VNCLOG("%s \n"),myInifile);
	int result=WritePrivateProfileString(key1,key2, buf,myInifile);
	if (result==0) return false;
#endif
	return true;
}

int
IniFile::ReadInt(char *key1, char *key2,int Defaultvalue)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	int out;
	if (ultravnc_veyon_load_int(key2, &out))
	{
        return out;
    }
    return Defaultvalue;
#else
	//vnclog.Print(LL_INTERR, VNCLOG("%s \n"),myInifile);
	return GetPrivateProfileInt(key1, key2, Defaultvalue, myInifile);
#endif
}

void
IniFile::ReadString(char *key1, char *key2,char *value,int valuesize)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	memset(value, 0, valuesize);
#else
	//vnclog.Print(LL_INTERR, VNCLOG("%s \n"),myInifile);
	GetPrivateProfileString(key1,key2, "",value,valuesize,myInifile);
#endif
}

void
IniFile::ReadPassword(char *value,int valuesize)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	ultravnc_veyon_load_password(value, valuesize);
#else
	//int size=ReadInt("UltraVNC", "passwdsize",0);
	//vnclog.Print(LL_INTERR, VNCLOG("%s \n"),myInifilePasswd);
	GetPrivateProfileStruct("UltraVNC","passwd",value,8,myInifile);
#endif
}

void //PGM
IniFile::ReadPassword2(char *value,int valuesize) //PGM
{ //PGM
#ifdef ULTRAVNC_VEYON_SUPPORT
	ultravnc_veyon_load_password(value, valuesize);
#else
	GetPrivateProfileStruct("UltraVNC","passwd2",value,8,myInifile); //PGM
#endif
} //PGM

bool
IniFile::WritePassword(char *value)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	return true;
#else
		//WriteInt("UltraVNC", "passwdsize",sizeof(value));
		//vnclog.Print(LL_INTERR, VNCLOG("%s \n"),myInifile);
		return (FALSE != WritePrivateProfileStruct("UltraVNC","passwd", value,8,myInifile));
#endif
}

bool //PGM
IniFile::WritePassword2(char *value) //PGM
{ //PGM
#ifdef ULTRAVNC_VEYON_SUPPORT
	return true;
#else
		return (FALSE != WritePrivateProfileStruct("UltraVNC","passwd2", value,8,myInifile)); //PGM
#endif
} //PGM

bool IniFile::IsWritable()
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	return false;
#else
    bool writable = WriteInt("Permissions", "isWritable",1);
    if (writable)
        WritePrivateProfileSection("Permissions", "", myInifile);

    return writable;
#endif
}
