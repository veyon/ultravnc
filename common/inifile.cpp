// This file is part of UltraVNC
// https://github.com/ultravnc/UltraVNC
// https://uvnc.com/
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// SPDX-FileCopyrightText: Copyright (C) 2002-2025 UltraVNC Team Members. All Rights Reserved.
// SPDX-FileCopyrightText: Copyright (C) 1999-2002 Vdacc-VNC & eSVNC Projects. All Rights Reserved.
//


//#include "stdhdrs.h"
#include "inifile.h"
#pragma warning( disable : 4100 )

#ifdef ULTRAVNC_VEYON_SUPPORT
extern BOOL ultravnc_veyon_load_int(LPCSTR valname, int *out);
extern void ultravnc_veyon_load_password(char* out, int size);
#endif

IniFile::IniFile()
{
}

void IniFile::setIniFile(char* iniFile)
{
#ifndef ULTRAVNC_VEYON_SUPPORT
	strcpy_s(strIniFile, iniFile);
#endif
}

IniFile::~IniFile()
{
}

bool
IniFile::WriteString(const char*key1, const char*key2,char *value)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	return true;
#else
	//vnclog.Print(LL_INTERR, VNCLOG("%s \n"),strIniFile);
	return (FALSE != WritePrivateProfileString(key1,key2, value,strIniFile));
#endif
}

bool
IniFile::WriteInt(const char*key1, const char*key2,int value)
{
#ifndef ULTRAVNC_VEYON_SUPPORT
	char       buf[32];
	wsprintf(buf, "%d", value);
	//vnclog.Print(LL_INTERR, VNCLOG("%s \n"),strIniFile);
	int result=WritePrivateProfileString(key1,key2, buf,strIniFile);
	if (result==0) return false;
#endif
	return true;
}

int
IniFile::ReadInt(const char*key1, const char*key2,int Defaultvalue)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	int out;
	if (ultravnc_veyon_load_int(key2, &out))
	{
        return out;
    }
    return Defaultvalue;
#else
	//vnclog.Print(LL_INTERR, VNCLOG("%s \n"),strIniFile);
	return GetPrivateProfileInt(key1, key2, Defaultvalue, strIniFile);
#endif
}

void
IniFile::ReadString(const char*key1, const char*key2,char *value,int valuesize)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	memset(value, 0, valuesize);
#else
	//vnclog.Print(LL_INTERR, VNCLOG("%s \n"),strIniFile);
	GetPrivateProfileString(key1,key2, "",value,valuesize,strIniFile);
#endif
}

void
IniFile::ReadPassword(char *value,int valuesize)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	ultravnc_veyon_load_password(value, valuesize);
#else
	GetPrivateProfileStruct("UltraVNC","passwd",value,8,strIniFile);
#endif
}

void
IniFile::ReadPasswordViewOnly(char *value,int valuesize) //PGM
{ 
#ifdef ULTRAVNC_VEYON_SUPPORT
	ultravnc_veyon_load_password(value, valuesize);
#else
	GetPrivateProfileStruct("UltraVNC","passwd2",value,8,strIniFile); //PGM
#endif
}

bool
IniFile::WritePassword(char *value)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	return true;
#else
	if (strlen(value) == 0)
		return (FALSE != WritePrivateProfileStruct("UltraVNC", "passwd", NULL, 8, strIniFile));
	return (FALSE != WritePrivateProfileStruct("UltraVNC","passwd", value,8,strIniFile));
#endif
}

bool 
IniFile::WritePasswordViewOnly(char*value)
{ 
#ifdef ULTRAVNC_VEYON_SUPPORT
	return true;
#else
	if (strlen(value) == 0)
		return (FALSE != WritePrivateProfileStruct("UltraVNC", "passwd2", NULL, 8, strIniFile));
	return (FALSE != WritePrivateProfileStruct("UltraVNC","passwd2", value,8,strIniFile));
#endif
} 

bool IniFile::IsWritable()
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	return false;
#else
    bool writable = WriteInt("Permissions", "isWritable",1);
    if (writable)
        WritePrivateProfileSection("Permissions", "", strIniFile);

    return writable;
#endif
}

void
IniFile::ReadHash(char* value, int valuesize)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	memset(value, 0, valuesize);
#else
	GetPrivateProfileStruct("UltraVNC", "hash", value, valuesize, strIniFile);
#endif
}

bool
IniFile::WriteHash(char* value, int valuesize)
{
#ifdef ULTRAVNC_VEYON_SUPPORT
	return false;
#else
	return (FALSE != WritePrivateProfileStruct("UltraVNC", "hash", value, valuesize, strIniFile));
#endif
}
