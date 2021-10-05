/* $Header: /usr/people/sam/tiff/libtiff/RCS/tif_apple.c,v 1.17 1994/09/29 17:29:57 sam Exp $ */

/*
 * Copyright (c) 1988, 1989, 1990, 1991, 1992, 1993, 1994 Sam Leffler
 * Copyright (c) 1991, 1992, 1993, 1994 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * TIFF Library Macintosh-specific routines.
 *
 * These routines use only Toolbox and high-level File Manager traps.
 * They make no calls to the THINK C "unix" compatibility library.  Also,
 * malloc is not used directly but it is still referenced internally by
 * the ANSI library in rare cases.  Heap fragmentation by the malloc ring
 * buffer is therefore minimized.
 *
 * O_RDONLY and O_RDWR are treated identically here.  The tif_mode flag is
 * checked in TIFFWriteCheck().
 *
 * Create below fills in a blank creator signature and sets the file type
 * to 'TIFF'.  It is much better for the application to do this by Create'ing
 * the file first and TIFFOpen'ing it later.
 */

#include "tiffiop.h"
#include <Errors.h>
#include <Files.h>
#include <Memory.h>

#ifdef applec
#define	CtoPstr	c2pstr
#endif

static tsize_t
_tiffReadProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	return (FSRead((short) fd, (long*) &size, (char*) buf) == noErr ?
	    size : (tsize_t) -1);
}

static tsize_t
_tiffWriteProc(thandle_t fd, tdata_t buf, tsize_t size)
{
	return (FSWrite((short) fd, (long*) &size, (char*) buf) == noErr ?
	    size : (tsize_t) -1);
}

static toff_t
_tiffSeekProc(thandle_t fd, toff_t off, int whence)
{
	long fpos, size;

	if (GetEOF((short) fd, &size) != noErr)
		return EOF;
	(void) GetFPos((short) fd, &fpos);

	switch (whence) {
	case SEEK_CUR:
		if (off + fpos > size)
			SetEOF((short) fd, off + fpos);
		if (SetFPos((short) fd, fsFromMark, off) != noErr)
			return EOF;
		break;
	case SEEK_END:
		if (off > 0)
			SetEOF((short) fd, off + size);
		if (SetFPos((short) fd, fsFromStart, off + size) != noErr)
			return EOF;
		break;
	case SEEK_SET:
		if (off > size)
			SetEOF((short) fd, off);
		if (SetFPos((short) fd, fsFromStart, off) != noErr)
			return EOF;
		break;
	}

	return (toff_t)(GetFPos((short) fd, &fpos) == noErr ? fpos : EOF);
}

static int
_tiffMapProc(thandle_t fd, tdata_t* pbase, toff_t* psize)
{
	return (0);
}

static void
_tiffUnmapProc(thandle_t fd, tdata_t base, toff_t size)
{
}

static int
_tiffCloseProc(thandle_t fd)
{
	return (FSClose((short) fd));
}

static toff_t
_tiffSizeProc(thandle_t fd)
{
	long size;

	if (GetEOF((short) fd, &size) != noErr) {
		TIFFError("_tiffSizeProc", "%s: Cannot get file size");
		return (-1L);
	}
	return ((toff_t) size);
}

/*
 * Open a TIFF file descriptor for read/writing.
 */
TIFF*
TIFFFdOpen(int fd, const char* name, const char* mode)
{
	TIFF* tif;

	tif = TIFFClientOpen(name, mode, (thandle_t) fd,
	    _tiffReadProc, _tiffWriteProc, _tiffSeekProc, _tiffCloseProc,
	    _tiffSizeProc, _tiffMapProc, _tiffUnmapProc);
	if (tif)
		tif->tif_fd = fd;
	return (tif);
}

/*
 * Open a TIFF file for read/writing.
 */
TIFF*
TIFFOpen(const char* name, const char* mode)
{
	static const char module[] = "TIFFOpen";
	Str255 pname;
	FInfo finfo;
	short fref;
	OSErr err;

	strcpy((char*) pname, name);
	CtoPstr((char*) pname);

	switch (_TIFFgetMode(mode, module)) {
	default:
		return ((TIFF*) 0);
	case O_RDWR | O_CREAT | O_TRUNC:
		if (GetFInfo(pname, 0, &finfo) == noErr)
			FSDelete(pname, 0);
		/* fall through */
	case O_RDWR | O_CREAT:
		if ((err = GetFInfo(pname, 0, &finfo)) == fnfErr) {
			if (Create(pname, 0, '    ', 'TIFF') != noErr)
				goto badCreate;
			if (FSOpen(pname, 0, &fref) != noErr)
				goto badOpen;
		} else if (err == noErr) {
			if (FSOpen(pname, 0, &fref) != noErr)
				goto badOpen;
		} else
			goto badOpen;
		break;
	case O_RDONLY:
	case O_RDWR:
		if (FSOpen(pname, 0, &fref) != noErr)
			goto badOpen;
		break;
	}
	return (TIFFFdOpen((int) fref, name, mode));
badCreate:
	TIFFError(module, "%s: Cannot create", name);
	return ((TIFF*) 0);
badOpen:
	TIFFError(module, "%s: Cannot open", name);
	return ((TIFF*) 0);
}

void
_TIFFmemset(void* p, int v, size_t c)
{
	memset(p, v, c);
}

void
_TIFFmemcpy(void* d, const void* s, size_t c)
{
	memcpy(d, s, c);
}

int
_TIFFmemcmp(const void* p1, const void* p2, size_t c)
{
	return (memcmp(p1, p2, c));
}

void *
_TIFFmalloc(size_t s)
{
	return (NewPtr(s));
}

void
_TIFFfree(void* p)
{
	DisposePtr(p);
}

void *
_TIFFrealloc(void* p, size_t s)
{
	Ptr n = p;

	SetPtrSize(p, s);
	if (MemError() && (n = NewPtr(s)) != NULL) {
		BlockMove(p, n, GetPtrSize(p));
		DisposePtr(p);
	}
	return (n);
}

static void
appleWarningHandler(const char* module, const char* fmt, va_list ap)
{
	if (module != NULL)
		fprintf(stderr, "%s: ", module);
	fprintf(stderr, "Warning, ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, ".\n");
}
TIFFErrorHandler _TIFFwarningHandler = appleWarningHandler;

static void
appleErrorHandler(const char* module, const char* fmt, va_list ap)
{
	if (module != NULL)
		fprintf(stderr, "%s: ", module);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, ".\n");
}
TIFFErrorHandler _TIFFerrorHandler = appleErrorHandler;
