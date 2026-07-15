// This file is part of UltraVNC
// https://github.com/ultravnc/UltraVNC
// https://uvnc.com/
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// SPDX-FileCopyrightText: Copyright (C) 2002-2025 UltraVNC Team Members. All Rights Reserved.
// SPDX-FileCopyrightText: Copyright (C) 1999-2002 Vdacc-VNC & eSVNC Projects. All Rights Reserved.
//


class vncEncodeUltra2;

#if !defined(_WINVNC_EncodeULTRA2)
#define _WINVNC_EncodeULTRA2
#pragma once
#include "vncencoder.h"
#include <csetjmp>
#ifdef ULTRAVNC_VEYON_SUPPORT
#include <lzo/lzo1x.h>
#include <jpeglib.h>
#else
#include "lzo/minilzo.h"
#ifdef _VCPKG
#include <jpeglib.h>
#else
#include "libjpeg-turbo-win/jpeglib.h"
#endif
#endif


// Class definition

// libjpeg error handler with setjmp/longjmp fallback: By default, libjpeg calls
// error_exit -> exit() on any error (unsupported color space, version/ABI mismatch, etc.),
// which KILLS the entire veyon-server. We use longjmp instead to abort only the current rectangle.
struct UltraJpegErrorMgr
{
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

class vncEncodeUltra2 : public vncEncoder
{
// Fields
public:

// Methods
public:
	// Create/Destroy methods
	vncEncodeUltra2();
	~vncEncodeUltra2();

	virtual void Init();
	virtual const char* GetEncodingName() { return "Ultra"; }

	virtual UINT RequiredBuffSize(UINT width, UINT height);
	virtual UINT NumCodedRects(const rfb::Rect &rect);

	virtual UINT EncodeRect(BYTE *source,VSocket *outConn, BYTE *dest, const rfb::Rect &rect);

// Implementation
private:
	BYTE		      *m_buffer;
	int			       m_bufflen;
	int SendJpegRect(BYTE *src,BYTE *dst, int dst_size, int w, int h, int quality,rfbPixelFormat m_remoteformat);
	bool				lzo;
	lzo_uint out_len;
	unsigned char *destbuffer;
	JSAMPROW *m_rowPointer;
	int m_rowPointerSize;
	void checkRowPointer(int h);
	int m_quality;
	struct jpeg_compress_struct cinfo;
	UltraJpegErrorMgr jerr;
};

#endif // _WINVNC_EncodeUltra

