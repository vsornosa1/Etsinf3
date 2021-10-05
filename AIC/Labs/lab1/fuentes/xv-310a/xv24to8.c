/*
 * xv24to8.c  -  contains the 24-to-8-bit Conv24to8() procedure
 *               and the 8-to-24-bit Conv8to24() procedure
 *
 * The Conv24to8 procedure takes a pointer to a 24-bit image (loaded
 * previously).  The image will be a w * h * 3 byte array of
 * bytes.  The image will be arranged with 3 bytes per pixel (in order
 * R, G, and B), pixel 0 at the top left corner.  (As normal.)
 * The procedure also takes a maximum number of colors to use (numcols)
 * and pointers to three 256-long arrays of bytes (to hold the returned
 * colormap)
 *
 * Note that Conv24to8() does NOT free the pic24 image under any circumstances
 *
 * The Conv24to8 procedure will set up the following:  it will allocate, make
 * & return 'pic8', a 'w' by 'h' (passed in values) 8-bit picture.
 * it will load up the rmap, gmap and bmap colormap arrays.  it will NOT 
 * calculate numcols, since the cmap sort procedure has to be called anyway
 *
 * Conv24to8 returns 'pic8' if successful, 'NULL' on failure (presumably on a 
 * malloc())
 *
 * The 'slow' code, while still based on Heckbert's Median Cut algorithm, 
 * has been shamelessly lifted from the Independent JPEG Group's software
 * (jquant2.c), as (for a variety of reasons) theirs was far better than 
 * the version I was previously using.  Thanks guys!
 *
 * Also, as is my way, I've stripped out most of the IJG's well-written
 * comments regarding their algorithm.  Folks interested in learning how it
 * works are encouraged to look at the original source.  (jpeg/jquant2.c)
 *
 * contains:
 *   Cont24to8()
 *   Init24to8()
 */

#include "copyright.h"

/*
 * Portions Copyright (C) 1989, 1991 by Jef Poskanzer.  See copyright notice
 * below, at the beginning of the relevant code.
 */

#include "xv.h"

static int    quick_check PARM((byte*, int,int, byte*, byte*,byte*,byte*,int));
static int    quick_quant PARM((byte*, int,int, byte*, byte*,byte*,byte*,int));
static int    ppm_quant   PARM((byte *,int,int, byte*, byte*,byte*,byte*,int));

static int    slow_quant  PARM((byte*, int,int, byte*, byte*,byte*,byte*,int));

/****************************/
void Init24to8()
{
  /* doesn't do anything anymore... */
}



/****************************/
byte *Conv24to8(pic24,w,h,nc,rm,gm,bm)
     byte *pic24;
     byte *rm, *gm, *bm;
     int   w,h,nc;
{
  /* returns pointer to new 8-bit-per-pixel image (w*h) if successful, or
     NULL if unsuccessful */
  
  int   i;
  byte *pic8;
  
  if (!pic24) return NULL;

  pic8 = (byte *) malloc((size_t) (w * h));
  if (!pic8) {
    fprintf(stderr,"%s: Conv24to8() - failed to allocate 'pic8'\n",cmd);
    return pic8;
  }

  if (nc<=0) nc = 255;  /* 'nc == 0' breaks code */

  if (!noqcheck && quick_check(pic24, w,h, pic8, rm,gm,bm, nc)) { 
    SetISTR(ISTR_INFO,"No color compression was necessary.\n");
    return pic8;   
  }

  switch (conv24) {
  case CONV24_FAST:
    SetISTR(ISTR_INFO,"Doing 'quick' 24-bit to 8-bit conversion.");
    i = quick_quant(pic24, w, h, pic8, rm, gm, bm, nc);
    break;
    
  case CONV24_BEST:
    SetISTR(ISTR_INFO,"Doing 'best' 24-bit to 8-bit conversion.");
    i = ppm_quant(pic24, w, h, pic8, rm, gm, bm, nc);
    break;
    
  case CONV24_SLOW:
  default:
    SetISTR(ISTR_INFO,"Doing 'slow' 24-bit to 8-bit conversion.");
    i = slow_quant(pic24, w, h, pic8, rm, gm, bm, nc);
    break;
  }
  
  if (i) { free(pic8);  pic8 = NULL; }
  return pic8;
}


/***************************************************************/
byte *Conv8to24(pic8, w, h, rmap,gmap,bmap)
     byte *pic8, *rmap, *gmap, *bmap;
     int   w, h;
{
  /* converts an w*h 8-bit image (with colormap rmap,gmap,bmap) into a
   * 24-bit image.  Note, 'pic8' could be NULL
   *
   * returns pointer to new 24-bits-per-pixel image (w*h) if successful,
   * or NULL if unsuccessful
   */

  int   i;
  byte *pic24, *sp, *dp;

  pic24 = (byte *) malloc((size_t) (w * h * 3));
  if (!pic24) return pic24;

  for (i=w*h, sp=pic8, dp=pic24; i; i--, sp++) {
    if ((i&0x1ffff)==0) WaitCursor();
    *dp++ = rmap[*sp];
    *dp++ = gmap[*sp];
    *dp++ = bmap[*sp];
  }

  return pic24;
}
  

/****************************/
static int quick_check(pic24, w,h, pic8, rmap,gmap,bmap, maxcol)
     byte *pic24, *pic8, *rmap, *gmap, *bmap;
     int   w,h,maxcol;
{
  /* scans picture until it finds more than 'maxcol' different colors.  If it
     finds more than 'maxcol' colors, it returns '0'.  If it DOESN'T, it does
     the 24-to-8 conversion by simply sticking the colors it found into
     a colormap, and changing instances of a color in pic24 into colormap
     indicies (in pic8) */

  unsigned long colors[256],col;
  int           i, nc, low, high, mid;
  byte         *p, *pix;

  if (maxcol>256) maxcol = 256;

  /* put the first color in the table by hand */
  nc = 0;  mid = 0;  

  for (i=w*h,p=pic24; i; i--) {
    col  = (((u_long) *p++) << 16);  
    col += (((u_long) *p++) << 8);
    col +=  *p++;

    /* binary search the 'colors' array to see if it's in there */
    low = 0;  high = nc-1;
    while (low <= high) {
      mid = (low+high)/2;
      if      (col < colors[mid]) high = mid - 1;
      else if (col > colors[mid]) low  = mid + 1;
      else break;
    }

    if (high < low) { /* didn't find color in list, add it. */
      if (nc>=maxcol) return 0;
      xvbcopy((char *) &colors[low], (char *) &colors[low+1],
	      (nc - low) * sizeof(u_long));
      colors[low] = col;
      nc++;
    }
  }


  /* run through the data a second time, this time mapping pixel values in
     pic24 into colormap offsets into 'colors' */

  for (i=w*h,p=pic24, pix=pic8; i; i--,pix++) {
    col  = (((u_long) *p++) << 16);  
    col += (((u_long) *p++) << 8);
    col +=  *p++;

    /* binary search the 'colors' array.  It *IS* in there */
    low = 0;  high = nc-1;
    while (low <= high) {
      mid = (low+high)/2;
      if      (col < colors[mid]) high = mid - 1;
      else if (col > colors[mid]) low  = mid + 1;
      else break;
    }

    if (high < low) {
      fprintf(stderr,"quick_check:  impossible situation!\n");
      exit(1);
    }
    *pix = mid;
  }

  /* and load up the 'desired colormap' */
  for (i=0; i<nc; i++) {
    rmap[i] =  colors[i]>>16;  
    gmap[i] = (colors[i]>>8) & 0xff;
    bmap[i] =  colors[i]     & 0xff;
  }

  return 1;
}




/************************************/
static int quick_quant(p24,w,h, p8, rmap,gmap,bmap, nc)
     byte *p24, *p8, *rmap, *gmap, *bmap;
     int   w,h,nc;
{
  /* called after 'pic8' has been alloced, pWIDE,pHIGH set up, mono/1-bit
     checked already */
  
/* up to 256 colors:     3 bits R, 3 bits G, 2 bits B  (RRRGGGBB) */
#define RMASK      0xe0
#define RSHIFT        0
#define GMASK      0xe0
#define GSHIFT        3
#define BMASK      0xc0
#define BSHIFT        6

  byte *pp;
  int  r1, g1, b1;
  int  *thisline, *nextline, *thisptr, *nextptr, *tmpptr;
  int  i, j, val, pwide3;
  int  imax, jmax;

  pp = p8;  pwide3 = w * 3;  imax = h-1;  jmax = w-1;


  /* load up colormap:
   *   note that 0 and 255 of each color are always in the map;
   *   intermediate values are evenly spaced.
   */

  for (i=0; i<256; i++) {
    rmap[i] = (((i<<RSHIFT) & RMASK) * 255 + RMASK/2) / RMASK;
    gmap[i] = (((i<<GSHIFT) & GMASK) * 255 + GMASK/2) / GMASK;
    bmap[i] = (((i<<BSHIFT) & BMASK) * 255 + BMASK/2) / BMASK;
  }
  

  thisline = (int *) malloc(pwide3 * sizeof(int));
  nextline = (int *) malloc(pwide3 * sizeof(int));
  if (!thisline || !nextline) {
    if (thisline) free(thisline);
    if (nextline) free(nextline);
    fprintf(stderr,"%s: unable to allocate memory in quick_quant()\n", cmd);
    return(1);
  }
  
  /* get first line of picture */
  for (j=pwide3, tmpptr=nextline; j; j--) *tmpptr++ = (int) *p24++;
  
  for (i=0; i<h; i++) {
    tmpptr = thisline;  thisline = nextline;  nextline = tmpptr;   /* swap */
    
    if ((i&0x3f) == 0) WaitCursor();

    if (i!=imax)   /* get next line */
      for (j=pwide3, tmpptr=nextline; j; j--)
	*tmpptr++ = (int) *p24++;
    
    for (j=0, thisptr=thisline, nextptr=nextline; j<w; j++,pp++) {
      r1 = *thisptr++;  g1 = *thisptr++;  b1 = *thisptr++;
      RANGE(r1,0,255);  RANGE(g1,0,255);  RANGE(b1,0,255);  
      
      /* choose actual pixel value */
      val = (((r1&RMASK)>>RSHIFT) | ((g1&GMASK)>>GSHIFT) | 
	     ((b1&BMASK)>>BSHIFT));
      *pp = val;
      
      /* compute color errors */
      r1 -= rmap[val];
      g1 -= gmap[val];
      b1 -= bmap[val];
      
      /* Add fractions of errors to adjacent pixels */
      if (j!=jmax) {  /* adjust RIGHT pixel */
	thisptr[0] += (r1*7) / 16;
	thisptr[1] += (g1*7) / 16;
	thisptr[2] += (b1*7) / 16;
      }
      
      if (i!=imax) {	/* do BOTTOM pixel */
	nextptr[0] += (r1*5) / 16;
	nextptr[1] += (g1*5) / 16;
	nextptr[2] += (b1*5) / 16;

	if (j>0) {  /* do BOTTOM LEFT pixel */
	  nextptr[-3] += (r1*3) / 16;
	  nextptr[-2] += (g1*3) / 16;
	  nextptr[-1] += (b1*3) / 16;
	}

	if (j!=jmax) {  /* do BOTTOM RIGHT pixel */
	  nextptr[3] += (r1)/16;
	  nextptr[4] += (g1)/16;
	  nextptr[5] += (b1)/16;
	}
	nextptr += 3;
      }
    }
  }
  
  free(thisline);
  free(nextline);
  return 0;


#undef RMASK
#undef RSHIFT
#undef GMASK
#undef GSHIFT
#undef BMASK
#undef BSHIFT
}
      






/***************************************************************/
/* The following code based on code from the 'pbmplus' package */
/* written by Jef Poskanzer                                    */
/***************************************************************/


/* ppmquant.c - quantize the colors in a pixmap down to a specified number
**
** Copyright (C) 1989, 1991 by Jef Poskanzer.
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/


typedef unsigned char pixval;

#define PPM_MAXMAXVAL 255
typedef struct { pixval r, g, b; } pixel;

#define PPM_GETR(p) ((p).r)
#define PPM_GETG(p) ((p).g)
#define PPM_GETB(p) ((p).b)

#define PPM_ASSIGN(p,red,grn,blu) \
  { (p).r = (red); (p).g = (grn); (p).b = (blu); }

#define PPM_EQUAL(p,q) ( (p).r == (q).r && (p).g == (q).g && (p).b == (q).b )


/* Color scaling macro -- to make writing ppmtowhatever easier. */

#define PPM_DEPTH(newp,p,oldmaxval,newmaxval) \
    PPM_ASSIGN( (newp), \
	        ((int) PPM_GETR(p)) * ((int)newmaxval) / ((int)oldmaxval), \
	        ((int) PPM_GETG(p)) * ((int)newmaxval) / ((int)oldmaxval), \
	        ((int) PPM_GETB(p)) * ((int)newmaxval) / ((int)oldmaxval) )


/* Luminance macro. */

/* 
 * #define PPM_LUMIN(p) \
 *   ( 0.299 * PPM_GETR(p) + 0.587 * PPM_GETG(p) + 0.114 * PPM_GETB(p) )
 */

/* Luminance macro, using only integer ops.  Returns an int (*256)  JHB */
#define PPM_LUMIN(p) \
  ( 77 * PPM_GETR(p) + 150 * PPM_GETG(p) + 29 * PPM_GETB(p) )

/* Color histogram stuff. */

typedef struct chist_item* chist_vec;
struct chist_item { pixel color;
			int value;
		      };

typedef struct chist_list_item* chist_list;
struct chist_list_item { struct chist_item ch;
			     chist_list next;
			   };

typedef chist_list* chash_table;

typedef struct box* box_vector;
struct box {
  int index;
  int colors;
  int sum;
};


#define MAXCOLORS 32767
#define CLUSTER_MAXVAL 63

#define LARGE_LUM
#define REP_AVERAGE_PIXELS

#define FS_SCALE 1024

#define HASH_SIZE 6553

#define ppm_hashpixel(p) ((((int) PPM_GETR(p) * 33023 +    \
			    (int) PPM_GETG(p) * 30013 +    \
			    (int) PPM_GETB(p) * 27011) & 0x7fffffff)   \
			  % HASH_SIZE)



/*** function defs ***/

static chist_vec   mediancut        PARM((chist_vec, int, int, int, int));
static int         redcompare       PARM((const void *, const void *));
static int         greencompare     PARM((const void *, const void *));
static int         bluecompare      PARM((const void *, const void *));
static int         sumcompare       PARM((const void *, const void *));
static chist_vec   ppm_computechist PARM((pixel **, int,int,int,int *));
static chash_table ppm_computechash PARM((pixel **, int,int,int,int *));
static chist_vec   ppm_chashtochist PARM((chash_table, int));
static chash_table ppm_allocchash   PARM((void));
static void        ppm_freechist    PARM((chist_vec));
static void        ppm_freechash    PARM((chash_table));


/****************************************************************************/
static int ppm_quant(pic24, cols, rows, pic8, rmap, gmap, bmap, newcolors)
     byte *pic24, *pic8, *rmap, *gmap, *bmap;
     int  cols, rows, newcolors;
{
  pixel**          pixels;
  register pixel*  pP;
  int              row;
  register int     col, limitcol;
  pixval           maxval, newmaxval;
  int              colors;
  register int     index;
  chist_vec chv, colormap;
  chash_table  cht;
  int              i;
  unsigned char    *picptr;
  static char      *fn = "ppmquant()";

  index = 0;
  maxval = 255;

  /*
   *  reformat 24-bit pic24 image (3 bytes per pixel) into 2-dimensional
   *  array of pixel structures
   */

  if (DEBUG) fprintf(stderr,"%s: remapping to ppm-style internal fmt\n", fn);
  WaitCursor();
  
  pixels = (pixel **) malloc(rows * sizeof(pixel *));
  if (!pixels) FatalError("couldn't allocate 'pixels' array");
  for (row=0; row<rows; row++) {
    pixels[row] = (pixel *) malloc(cols * sizeof(pixel));
    if (!pixels[row]) FatalError("couldn't allocate a row of pixels array");

    for (col=0, pP=pixels[row]; col<cols; col++, pP++) {
      pP->r = *pic24++;
      pP->g = *pic24++;
      pP->b = *pic24++;
    }
  }
  if (DEBUG) fprintf(stderr,"%s: done format remapping\n", fn);


    

  /*
   *  attempt to make a histogram of the colors, unclustered.
   *  If at first we don't succeed, lower maxval to increase color
   *  coherence and try again.  This will eventually terminate, with
   *  maxval at worst 15, since 32^3 is approximately MAXCOLORS.
   */

  WaitCursor();
  for ( ; ; ) {
    if (DEBUG) fprintf(stderr, "%s:  making histogram\n", fn);

    chv = ppm_computechist(pixels, cols, rows, MAXCOLORS, &colors);
    if (chv != (chist_vec) 0) break;
    
    if (DEBUG) fprintf(stderr, "%s: too many colors!\n", fn);
    newmaxval = maxval / 2;
    if (DEBUG) fprintf(stderr, "%s: rescaling colors (maxval=%d) %s\n",
		       fn, newmaxval, "to improve clustering");

    for (row=0; row<rows; ++row)
      for (col=0, pP=pixels[row]; col<cols; ++col, ++pP)
	PPM_DEPTH( *pP, *pP, maxval, newmaxval );
    maxval = newmaxval;
  }

  if (DEBUG) fprintf(stderr,"%s: %d colors found\n", fn, colors);



  /*
   * Step 3: apply median-cut to histogram, making the new colormap.
   */

  WaitCursor();
  if (DEBUG) fprintf(stderr, "%s: choosing %d colors\n", fn, newcolors);
  colormap = mediancut(chv, colors, rows * cols, maxval, newcolors);
  ppm_freechist(chv);



  /*
   *  Step 4: map the colors in the image to their closest match in the
   *  new colormap, and write 'em out.
   */

  if (DEBUG) fprintf(stderr,"%s: mapping image to new colors\n", fn);
  cht = ppm_allocchash();

  picptr = pic8;
  for (row = 0;  row < rows;  ++row) {
    col = 0;  limitcol = cols;  pP = pixels[row];

    ProgressMeter(0, rows-1, row, "24 -> 8");

    if ((row & 0x1f) == 0) WaitCursor();
    do {
      int hash;
      chist_list chl;

      /* Check hash table to see if we have already matched this color. */

      hash = ppm_hashpixel(*pP);
      for (chl = cht[hash];  chl;  chl = chl->next)
	if (PPM_EQUAL(chl->ch.color, *pP)) {index = chl->ch.value; break;}

      if (!chl /*index = -1*/) {/* No; search colormap for closest match. */
	register int i, r1, g1, b1, r2, g2, b2;
	register long dist, newdist;

	r1 = PPM_GETR( *pP );
	g1 = PPM_GETG( *pP );
	b1 = PPM_GETB( *pP );
	dist = 2000000000;

	for (i=0; i<newcolors; i++) {
	  r2 = PPM_GETR( colormap[i].color );
	  g2 = PPM_GETG( colormap[i].color );
	  b2 = PPM_GETB( colormap[i].color );

	  newdist = ( r1 - r2 ) * ( r1 - r2 ) +
	            ( g1 - g2 ) * ( g1 - g2 ) +
	            ( b1 - b2 ) * ( b1 - b2 );

	  if (newdist<dist) { index = i;  dist = newdist; }
	}

	hash = ppm_hashpixel(*pP);
	chl = (chist_list) malloc(sizeof(struct chist_list_item));
	if (!chl) FatalError("ran out of memory adding to hash table");

	chl->ch.color = *pP;
	chl->ch.value = index;
	chl->next = cht[hash];
	cht[hash] = chl;
      }

      *picptr++ = index;

      ++col;
      ++pP;
    }
    while (col != limitcol);
  }

  /* rescale the colormap and load the XV colormap */
  for (i=0; i<newcolors; i++) {
    PPM_DEPTH(colormap[i].color, colormap[i].color, maxval, 255);
    rmap[i] = PPM_GETR( colormap[i].color );
    gmap[i] = PPM_GETG( colormap[i].color );
    bmap[i] = PPM_GETB( colormap[i].color );
  }

  /* free the pixels array */
  for (i=0; i<rows; i++) free(pixels[i]);
  free(pixels);

  /* free cht and colormap */
  ppm_freechist(colormap);
  ppm_freechash(cht);

  return 0;
}



/*
** Here is the fun part, the median-cut colormap generator.  This is based
** on Paul Heckbert's paper "Color Image Quantization for Frame Buffer
** Display", SIGGRAPH '82 Proceedings, page 297.
*/



/****************************************************************************/
static chist_vec mediancut( chv, colors, sum, maxval, newcolors )
     chist_vec chv;
     int colors, sum, newcolors;
     int maxval;
{
  chist_vec colormap;
  box_vector bv;
  register int bi, i;
  int boxes;

  bv = (box_vector) malloc(sizeof(struct box) * newcolors);
  colormap = (chist_vec) 
             malloc(sizeof(struct chist_item) * newcolors );

  if (!bv || !colormap) FatalError("unable to malloc in mediancut()");

  for (i=0; i<newcolors; i++)
    PPM_ASSIGN(colormap[i].color, 0, 0, 0);

  /*
   *  Set up the initial box.
   */
  bv[0].index = 0;
  bv[0].colors = colors;
  bv[0].sum = sum;
  boxes = 1;


  /*
   ** Main loop: split boxes until we have enough.
   */

  while ( boxes < newcolors ) {
    register int indx, clrs;
    int sm;
    register int minr, maxr, ming, maxg, minb, maxb, v;
    int halfsum, lowersum;

    /*
     ** Find the first splittable box.
     */
    for (bi=0; bv[bi].colors<2 && bi<boxes; bi++) ;
    if (bi == boxes) break;	/* ran out of colors! */

    indx = bv[bi].index;
    clrs = bv[bi].colors;
    sm = bv[bi].sum;

    /*
     ** Go through the box finding the minimum and maximum of each
     ** component - the boundaries of the box.
     */
    minr = maxr = PPM_GETR( chv[indx].color );
    ming = maxg = PPM_GETG( chv[indx].color );
    minb = maxb = PPM_GETB( chv[indx].color );

    for (i=1; i<clrs; i++) {
      v = PPM_GETR( chv[indx + i].color );
      if (v < minr) minr = v;
      if (v > maxr) maxr = v;

      v = PPM_GETG( chv[indx + i].color );
      if (v < ming) ming = v;
      if (v > maxg) maxg = v;

      v = PPM_GETB( chv[indx + i].color );
      if (v < minb) minb = v;
      if (v > maxb) maxb = v;
    }

    /*
     ** Find the largest dimension, and sort by that component.  I have
     ** included two methods for determining the "largest" dimension;
     ** first by simply comparing the range in RGB space, and second
     ** by transforming into luminosities before the comparison.  You
     ** can switch which method is used by switching the commenting on
     ** the LARGE_ defines at the beginning of this source file.
     */
    {
      /* LARGE_LUM version */

      pixel p;
      int rl, gl, bl;

      PPM_ASSIGN(p, maxr - minr, 0, 0);
      rl = PPM_LUMIN(p);

      PPM_ASSIGN(p, 0, maxg - ming, 0);
      gl = PPM_LUMIN(p);

      PPM_ASSIGN(p, 0, 0, maxb - minb);
      bl = PPM_LUMIN(p);

      if (rl >= gl && rl >= bl)
	qsort((char*) &(chv[indx]), (size_t) clrs, sizeof(struct chist_item),
	      redcompare );
      else if (gl >= bl)
	qsort((char*) &(chv[indx]), (size_t) clrs, sizeof(struct chist_item),
	      greencompare );
      else 
	qsort((char*) &(chv[indx]), (size_t) clrs, sizeof(struct chist_item),
	      bluecompare );
    }

    /*
     ** Now find the median based on the counts, so that about half the
     ** pixels (not colors, pixels) are in each subdivision.
     */
    lowersum = chv[indx].value;
    halfsum = sm / 2;
    for (i=1; i<clrs-1; i++) {
      if (lowersum >= halfsum) break;
      lowersum += chv[indx + i].value;
    }

    /*
     ** Split the box, and sort to bring the biggest boxes to the top.
     */
    bv[bi].colors = i;
    bv[bi].sum = lowersum;
    bv[boxes].index = indx + i;
    bv[boxes].colors = clrs - i;
    bv[boxes].sum = sm - lowersum;
    ++boxes;
    qsort((char*) bv, (size_t) boxes, sizeof(struct box), sumcompare);
  }  /* while (boxes ... */
  
  /*
   ** Ok, we've got enough boxes.  Now choose a representative color for
   ** each box.  There are a number of possible ways to make this choice.
   ** One would be to choose the center of the box; this ignores any structure
   ** within the boxes.  Another method would be to average all the colors in
   ** the box - this is the method specified in Heckbert's paper.  A third
   ** method is to average all the pixels in the box.  You can switch which
   ** method is used by switching the commenting on the REP_ defines at
   ** the beginning of this source file.
   */
  
  for (bi=0; bi<boxes; bi++) {
    /* REP_AVERAGE_PIXELS version */
    register int indx = bv[bi].index;
    register int clrs = bv[bi].colors;
    register long r = 0, g = 0, b = 0, sum = 0;

    for (i=0; i<clrs; i++) {
      r += PPM_GETR( chv[indx + i].color ) * chv[indx + i].value;
      g += PPM_GETG( chv[indx + i].color ) * chv[indx + i].value;
      b += PPM_GETB( chv[indx + i].color ) * chv[indx + i].value;
      sum += chv[indx + i].value;
    }

    r = r / sum;  if (r>maxval) r = maxval;	/* avoid math errors */
    g = g / sum;  if (g>maxval) g = maxval;
    b = b / sum;  if (b>maxval) b = maxval;

    PPM_ASSIGN( colormap[bi].color, r, g, b );
  }

  free(bv);
  return colormap;
}


/**********************************/
static int redcompare(p1, p2)
     const void *p1, *p2;
{
  return (int) PPM_GETR( ((chist_vec)p1)->color ) - 
         (int) PPM_GETR( ((chist_vec)p2)->color );
}

/**********************************/
static int greencompare(p1, p2)
     const void *p1, *p2;
{
  return (int) PPM_GETG( ((chist_vec)p1)->color ) - 
         (int) PPM_GETG( ((chist_vec)p2)->color );
}

/**********************************/
static int bluecompare(p1, p2)
     const void *p1, *p2;
{
  return (int) PPM_GETB( ((chist_vec)p1)->color ) - 
         (int) PPM_GETB( ((chist_vec)p2)->color );
}

/**********************************/
static int sumcompare(p1, p2)
     const void *p1, *p2;
{
  return ((box_vector) p2)->sum - ((box_vector) p1)->sum;
}



/****************************************************************************/
static chist_vec 
  ppm_computechist(pixels, cols, rows, maxcolors, colorsP)
     pixel** pixels;
     int cols, rows, maxcolors;
     int* colorsP;
{
  chash_table cht;
  chist_vec chv;

  cht = ppm_computechash(pixels, cols, rows, maxcolors, colorsP);
  if (!cht) return (chist_vec) 0;

  chv = ppm_chashtochist(cht, maxcolors);
  ppm_freechash(cht);
  return chv;
}


/****************************************************************************/
static chash_table ppm_computechash(pixels, cols, rows, 
					    maxcolors, colorsP )
     pixel** pixels;
     int cols, rows, maxcolors;
     int* colorsP;
{
  chash_table cht;
  register pixel* pP;
  chist_list chl;
  int col, row, hash;

  cht = ppm_allocchash( );
  *colorsP = 0;

  /* Go through the entire image, building a hash table of colors. */
  for (row=0; row<rows; row++)
    for (col=0, pP=pixels[row];  col<cols;  col++, pP++) {
      hash = ppm_hashpixel(*pP);

      for (chl = cht[hash]; chl != (chist_list) 0; chl = chl->next)
	if (PPM_EQUAL(chl->ch.color, *pP)) break;
      
      if (chl != (chist_list) 0) ++(chl->ch.value);
      else {
	if ((*colorsP)++ > maxcolors) {
	  ppm_freechash(cht);
	  return (chash_table) 0;
	}
	
	chl = (chist_list) malloc(sizeof(struct chist_list_item));
	if (!chl) FatalError("ran out of memory computing hash table");

	chl->ch.color = *pP;
	chl->ch.value = 1;
	chl->next = cht[hash];
	cht[hash] = chl;
      }
    }
  
  return cht;
}


/****************************************************************************/
static chash_table ppm_allocchash()
{
  chash_table cht;
  int i;

  cht = (chash_table) malloc( HASH_SIZE * sizeof(chist_list) );
  if (!cht) FatalError("ran out of memory allocating hash table");

  for (i=0; i<HASH_SIZE; i++ )
    cht[i] = (chist_list) 0;

  return cht;
}


/****************************************************************************/
static chist_vec ppm_chashtochist( cht, maxcolors )
     chash_table cht;
     int maxcolors;
{
  chist_vec chv;
  chist_list chl;
  int i, j;

  /* Now collate the hash table into a simple chist array. */
  chv = (chist_vec) malloc( maxcolors * sizeof(struct chist_item) );

  /* (Leave room for expansion by caller.) */
  if (!chv) FatalError("ran out of memory generating histogram");

  /* Loop through the hash table. */
  j = 0;
  for (i=0; i<HASH_SIZE; i++)
    for (chl = cht[i];  chl != (chist_list) 0;  chl = chl->next) {
      /* Add the new entry. */
      chv[j] = chl->ch;
      ++j;
    }

  return chv;
}


/****************************************************************************/
static void ppm_freechist( chv )
     chist_vec chv;
{
  free( (char*) chv );
}


/****************************************************************************/
static void ppm_freechash( cht )
     chash_table cht;
{
  int i;
  chist_list chl, chlnext;

  for (i=0; i<HASH_SIZE; i++)
    for (chl = cht[i];  chl != (chist_list) 0; chl = chlnext) {
      chlnext = chl->next;
      free( (char*) chl );
    }

  free( (char*) cht );
}





/***************************************************************/
/* The following is based on jquant2.c from version 5          */
/* of the IJG JPEG software, which is                          */
/* Copyright (C) 1991-1994, Thomas G. Lane.                    */
/***************************************************************/

#define MAXNUMCOLORS  256	/* maximum size of colormap */

#define C0_SCALE 2		/* scale R distances by this much */
#define C1_SCALE 3		/* scale G distances by this much */
#define C2_SCALE 1		/* and B by this much */

#define HIST_C0_BITS  5		/* bits of precision in R histogram */
#define HIST_C1_BITS  6		/* bits of precision in G histogram */
#define HIST_C2_BITS  5		/* bits of precision in B histogram */

/* Number of elements along histogram axes. */
#define HIST_C0_ELEMS  (1<<HIST_C0_BITS)
#define HIST_C1_ELEMS  (1<<HIST_C1_BITS)
#define HIST_C2_ELEMS  (1<<HIST_C2_BITS)

/* These are the amounts to shift an input value to get a histogram index. */
#define C0_SHIFT  (8-HIST_C0_BITS)
#define C1_SHIFT  (8-HIST_C1_BITS)
#define C2_SHIFT  (8-HIST_C2_BITS)


typedef unsigned char JSAMPLE;
typedef JSAMPLE * JSAMPROW;

typedef u_short histcell;	/* histogram cell; prefer an unsigned type */

typedef histcell * histptr;	/* for pointers to histogram cells */

typedef histcell hist1d[HIST_C2_ELEMS]; /* typedefs for the histogram array */
typedef hist1d hist2d[HIST_C1_ELEMS];
typedef hist2d hist3d[HIST_C0_ELEMS];

typedef short FSERROR;		/* 16 bits should be enough */
typedef int LOCFSERROR;		/* use 'int' for calculation temps */

typedef FSERROR *FSERRPTR;	/* pointer to error array */

typedef struct {
  /* The bounds of the box (inclusive); expressed as histogram indexes */
  int c0min, c0max;
  int c1min, c1max;
  int c2min, c2max;
  /* The volume (actually 2-norm) of the box */
  INT32 volume;
  /* The number of nonzero histogram cells within this box */
  long colorcount;
} box;
typedef box * boxptr;

/* Local state for the IJG quantizer */

static hist2d * sl_histogram;	/* pointer to the 3D histogram array */
static FSERRPTR sl_fserrors;	/* accumulated-errors array */
static int * sl_error_limiter;	/* table for clamping the applied error */
static int sl_on_odd_row;	/* flag to remember which row we are on */
static JSAMPROW sl_colormap[3];	/* selected colormap */
static int sl_num_colors;	/* number of selected colors */


static void   slow_fill_histogram PARM((byte*, int));
static boxptr find_biggest_color_pop PARM((boxptr, int));
static boxptr find_biggest_volume PARM((boxptr, int));
static void   update_box PARM((boxptr));
static int    median_cut PARM((boxptr, int, int));
static void   compute_color PARM((boxptr, int));
static void   slow_select_colors PARM((int));
static int    find_nearby_colors PARM((int, int, int, JSAMPLE []));
static void   find_best_colors PARM((int,int,int,int, JSAMPLE [], JSAMPLE []));
static void   fill_inverse_cmap PARM((int, int, int));
static void   slow_map_pixels PARM((byte*, int, int, byte*));
static void   init_error_limit PARM((void));


/* Master control for slow quantizer. */
static int slow_quant(pic24, w, h, pic8, rm,gm,bm, descols)
     byte *pic24, *pic8, *rm, *gm, *bm;
     int   w, h, descols;
{
  size_t fs_arraysize = (w + 2) * (3 * sizeof(FSERROR));

  /* Allocate all the temporary storage needed */
  if (sl_error_limiter == NULL)
    init_error_limit();
  sl_histogram = (hist2d *) malloc(sizeof(hist3d));
  sl_fserrors = (FSERRPTR) malloc(fs_arraysize);

  if (! sl_error_limiter || ! sl_histogram || ! sl_fserrors) {
    /* we never free sl_error_limiter once acquired */
    if (sl_histogram) free(sl_histogram);
    if (sl_fserrors) free(sl_fserrors);
    fprintf(stderr,"%s: slow_quant() - failed to allocate workspace\n",cmd);
    return 1;
  }

  sl_colormap[0] = (JSAMPROW) rm;
  sl_colormap[1] = (JSAMPROW) gm;
  sl_colormap[2] = (JSAMPROW) bm;

  /* Compute the color histogram */
  slow_fill_histogram(pic24, w*h);

  /* Select the colormap */
  slow_select_colors(descols);

  /* Zero the histogram: now to be used as inverse color map */
  xvbzero((char *) sl_histogram, sizeof(hist3d));

  /* Initialize the propagated errors to zero. */
  xvbzero((char *) sl_fserrors, fs_arraysize);
  sl_on_odd_row = FALSE;

  /* Map the image. */
  slow_map_pixels(pic24, w, h, pic8);

  /* Release working memory. */
  /* we never free sl_error_limiter once acquired */
  free(sl_histogram);
  free(sl_fserrors);

  return 0;
}


static void slow_fill_histogram (pic24, numpixels)
     register byte *pic24;
     register int   numpixels;
{
  register histptr histp;
  register hist2d * histogram = sl_histogram;

  xvbzero((char *) histogram, sizeof(hist3d));

  while (numpixels-- > 0) {
    /* get pixel value and index into the histogram */
    histp = & histogram[pic24[0] >> C0_SHIFT]
		       [pic24[1] >> C1_SHIFT]
		       [pic24[2] >> C2_SHIFT];
    /* increment, check for overflow and undo increment if so. */
    if (++(*histp) <= 0)
      (*histp)--;
    pic24 += 3;
  }
}


static boxptr find_biggest_color_pop (boxlist, numboxes)
     boxptr boxlist;
     int numboxes;
{
  register boxptr boxp;
  register int i;
  register long maxc = 0;
  boxptr which = NULL;
  
  for (i = 0, boxp = boxlist; i < numboxes; i++, boxp++) {
    if (boxp->colorcount > maxc && boxp->volume > 0) {
      which = boxp;
      maxc = boxp->colorcount;
    }
  }
  return which;
}


static boxptr find_biggest_volume (boxlist, numboxes)
     boxptr boxlist;
     int numboxes;
{
  register boxptr boxp;
  register int i;
  register INT32 maxv = 0;
  boxptr which = NULL;
  
  for (i = 0, boxp = boxlist; i < numboxes; i++, boxp++) {
    if (boxp->volume > maxv) {
      which = boxp;
      maxv = boxp->volume;
    }
  }
  return which;
}


static void update_box (boxp)
     boxptr boxp;
{
  hist2d * histogram = sl_histogram;
  histptr histp;
  int c0,c1,c2;
  int c0min,c0max,c1min,c1max,c2min,c2max;
  INT32 dist0,dist1,dist2;
  long ccount;
  
  c0min = boxp->c0min;  c0max = boxp->c0max;
  c1min = boxp->c1min;  c1max = boxp->c1max;
  c2min = boxp->c2min;  c2max = boxp->c2max;
  
  if (c0max > c0min)
    for (c0 = c0min; c0 <= c0max; c0++)
      for (c1 = c1min; c1 <= c1max; c1++) {
	histp = & histogram[c0][c1][c2min];
	for (c2 = c2min; c2 <= c2max; c2++)
	  if (*histp++ != 0) {
	    boxp->c0min = c0min = c0;
	    goto have_c0min;
	  }
      }
 have_c0min:
  if (c0max > c0min)
    for (c0 = c0max; c0 >= c0min; c0--)
      for (c1 = c1min; c1 <= c1max; c1++) {
	histp = & histogram[c0][c1][c2min];
	for (c2 = c2min; c2 <= c2max; c2++)
	  if (*histp++ != 0) {
	    boxp->c0max = c0max = c0;
	    goto have_c0max;
	  }
      }
 have_c0max:
  if (c1max > c1min)
    for (c1 = c1min; c1 <= c1max; c1++)
      for (c0 = c0min; c0 <= c0max; c0++) {
	histp = & histogram[c0][c1][c2min];
	for (c2 = c2min; c2 <= c2max; c2++)
	  if (*histp++ != 0) {
	    boxp->c1min = c1min = c1;
	    goto have_c1min;
	  }
      }
 have_c1min:
  if (c1max > c1min)
    for (c1 = c1max; c1 >= c1min; c1--)
      for (c0 = c0min; c0 <= c0max; c0++) {
	histp = & histogram[c0][c1][c2min];
	for (c2 = c2min; c2 <= c2max; c2++)
	  if (*histp++ != 0) {
	    boxp->c1max = c1max = c1;
	    goto have_c1max;
	  }
      }
 have_c1max:
  if (c2max > c2min)
    for (c2 = c2min; c2 <= c2max; c2++)
      for (c0 = c0min; c0 <= c0max; c0++) {
	histp = & histogram[c0][c1min][c2];
	for (c1 = c1min; c1 <= c1max; c1++, histp += HIST_C2_ELEMS)
	  if (*histp != 0) {
	    boxp->c2min = c2min = c2;
	    goto have_c2min;
	  }
      }
 have_c2min:
  if (c2max > c2min)
    for (c2 = c2max; c2 >= c2min; c2--)
      for (c0 = c0min; c0 <= c0max; c0++) {
	histp = & histogram[c0][c1min][c2];
	for (c1 = c1min; c1 <= c1max; c1++, histp += HIST_C2_ELEMS)
	  if (*histp != 0) {
	    boxp->c2max = c2max = c2;
	    goto have_c2max;
	  }
      }
 have_c2max:

  dist0 = ((c0max - c0min) << C0_SHIFT) * C0_SCALE;
  dist1 = ((c1max - c1min) << C1_SHIFT) * C1_SCALE;
  dist2 = ((c2max - c2min) << C2_SHIFT) * C2_SCALE;
  boxp->volume = dist0*dist0 + dist1*dist1 + dist2*dist2;
  
  ccount = 0;
  for (c0 = c0min; c0 <= c0max; c0++)
    for (c1 = c1min; c1 <= c1max; c1++) {
      histp = & histogram[c0][c1][c2min];
      for (c2 = c2min; c2 <= c2max; c2++, histp++)
	if (*histp != 0) {
	  ccount++;
	}
    }
  boxp->colorcount = ccount;
}


static int median_cut (boxlist, numboxes, desired_colors)
     boxptr boxlist;
     int numboxes, desired_colors;
{
  int n,lb;
  int c0,c1,c2,cmax;
  register boxptr b1,b2;

  while (numboxes < desired_colors) {
    /* Select box to split.
     * Current algorithm: by population for first half, then by volume.
     */
    if (numboxes*2 <= desired_colors) {
      b1 = find_biggest_color_pop(boxlist, numboxes);
    } else {
      b1 = find_biggest_volume(boxlist, numboxes);
    }
    if (b1 == NULL)		/* no splittable boxes left! */
      break;
    b2 = &boxlist[numboxes];	/* where new box will go */
    /* Copy the color bounds to the new box. */
    b2->c0max = b1->c0max; b2->c1max = b1->c1max; b2->c2max = b1->c2max;
    b2->c0min = b1->c0min; b2->c1min = b1->c1min; b2->c2min = b1->c2min;
    /* Choose which axis to split the box on.
     */
    c0 = ((b1->c0max - b1->c0min) << C0_SHIFT) * C0_SCALE;
    c1 = ((b1->c1max - b1->c1min) << C1_SHIFT) * C1_SCALE;
    c2 = ((b1->c2max - b1->c2min) << C2_SHIFT) * C2_SCALE;
    cmax = c1; n = 1;
    if (c0 > cmax) { cmax = c0; n = 0; }
    if (c2 > cmax) { n = 2; }
    switch (n) {
    case 0:
      lb = (b1->c0max + b1->c0min) / 2;
      b1->c0max = lb;
      b2->c0min = lb+1;
      break;
    case 1:
      lb = (b1->c1max + b1->c1min) / 2;
      b1->c1max = lb;
      b2->c1min = lb+1;
      break;
    case 2:
      lb = (b1->c2max + b1->c2min) / 2;
      b1->c2max = lb;
      b2->c2min = lb+1;
      break;
    }
    /* Update stats for boxes */
    update_box(b1);
    update_box(b2);
    numboxes++;
  }
  return numboxes;
}


static void compute_color (boxp, icolor)
     boxptr boxp;
     int icolor;
{
  /* Current algorithm: mean weighted by pixels (not colors) */
  /* Note it is important to get the rounding correct! */
  hist2d * histogram = sl_histogram;
  histptr histp;
  int c0,c1,c2;
  int c0min,c0max,c1min,c1max,c2min,c2max;
  long count;
  long total = 0;
  long c0total = 0;
  long c1total = 0;
  long c2total = 0;
  
  c0min = boxp->c0min;  c0max = boxp->c0max;
  c1min = boxp->c1min;  c1max = boxp->c1max;
  c2min = boxp->c2min;  c2max = boxp->c2max;
  
  for (c0 = c0min; c0 <= c0max; c0++)
    for (c1 = c1min; c1 <= c1max; c1++) {
      histp = & histogram[c0][c1][c2min];
      for (c2 = c2min; c2 <= c2max; c2++) {
	if ((count = *histp++) != 0) {
	  total += count;
	  c0total += ((c0 << C0_SHIFT) + ((1<<C0_SHIFT)>>1)) * count;
	  c1total += ((c1 << C1_SHIFT) + ((1<<C1_SHIFT)>>1)) * count;
	  c2total += ((c2 << C2_SHIFT) + ((1<<C2_SHIFT)>>1)) * count;
	}
      }
    }
  
  sl_colormap[0][icolor] = (JSAMPLE) ((c0total + (total>>1)) / total);
  sl_colormap[1][icolor] = (JSAMPLE) ((c1total + (total>>1)) / total);
  sl_colormap[2][icolor] = (JSAMPLE) ((c2total + (total>>1)) / total);
}


static void slow_select_colors (descolors)
     int descolors;
/* Master routine for color selection */
{
  box boxlist[MAXNUMCOLORS];
  int numboxes;
  int i;

  /* Initialize one box containing whole space */
  numboxes = 1;
  boxlist[0].c0min = 0;
  boxlist[0].c0max = 255 >> C0_SHIFT;
  boxlist[0].c1min = 0;
  boxlist[0].c1max = 255 >> C1_SHIFT;
  boxlist[0].c2min = 0;
  boxlist[0].c2max = 255 >> C2_SHIFT;
  /* Shrink it to actually-used volume and set its statistics */
  update_box(& boxlist[0]);
  /* Perform median-cut to produce final box list */
  numboxes = median_cut(boxlist, numboxes, descolors);
  /* Compute the representative color for each box, fill colormap */
  for (i = 0; i < numboxes; i++)
    compute_color(& boxlist[i], i);
  sl_num_colors = numboxes;
}


/* log2(histogram cells in update box) for each axis; this can be adjusted */
#define BOX_C0_LOG  (HIST_C0_BITS-3)
#define BOX_C1_LOG  (HIST_C1_BITS-3)
#define BOX_C2_LOG  (HIST_C2_BITS-3)

#define BOX_C0_ELEMS  (1<<BOX_C0_LOG) /* # of hist cells in update box */
#define BOX_C1_ELEMS  (1<<BOX_C1_LOG)
#define BOX_C2_ELEMS  (1<<BOX_C2_LOG)

#define BOX_C0_SHIFT  (C0_SHIFT + BOX_C0_LOG)
#define BOX_C1_SHIFT  (C1_SHIFT + BOX_C1_LOG)
#define BOX_C2_SHIFT  (C2_SHIFT + BOX_C2_LOG)


static int find_nearby_colors (minc0, minc1, minc2, colorlist)
     int minc0, minc1, minc2;
     JSAMPLE colorlist[];
{
  int numcolors = sl_num_colors;
  int maxc0, maxc1, maxc2;
  int centerc0, centerc1, centerc2;
  int i, x, ncolors;
  INT32 minmaxdist, min_dist, max_dist, tdist;
  INT32 mindist[MAXNUMCOLORS];	/* min distance to colormap entry i */

  maxc0 = minc0 + ((1 << BOX_C0_SHIFT) - (1 << C0_SHIFT));
  centerc0 = (minc0 + maxc0) >> 1;
  maxc1 = minc1 + ((1 << BOX_C1_SHIFT) - (1 << C1_SHIFT));
  centerc1 = (minc1 + maxc1) >> 1;
  maxc2 = minc2 + ((1 << BOX_C2_SHIFT) - (1 << C2_SHIFT));
  centerc2 = (minc2 + maxc2) >> 1;

  minmaxdist = 0x7FFFFFFFL;

  for (i = 0; i < numcolors; i++) {
    /* We compute the squared-c0-distance term, then add in the other two. */
    x = sl_colormap[0][i];
    if (x < minc0) {
      tdist = (x - minc0) * C0_SCALE;
      min_dist = tdist*tdist;
      tdist = (x - maxc0) * C0_SCALE;
      max_dist = tdist*tdist;
    } else if (x > maxc0) {
      tdist = (x - maxc0) * C0_SCALE;
      min_dist = tdist*tdist;
      tdist = (x - minc0) * C0_SCALE;
      max_dist = tdist*tdist;
    } else {
      /* within cell range so no contribution to min_dist */
      min_dist = 0;
      if (x <= centerc0) {
	tdist = (x - maxc0) * C0_SCALE;
	max_dist = tdist*tdist;
      } else {
	tdist = (x - minc0) * C0_SCALE;
	max_dist = tdist*tdist;
      }
    }

    x = sl_colormap[1][i];
    if (x < minc1) {
      tdist = (x - minc1) * C1_SCALE;
      min_dist += tdist*tdist;
      tdist = (x - maxc1) * C1_SCALE;
      max_dist += tdist*tdist;
    } else if (x > maxc1) {
      tdist = (x - maxc1) * C1_SCALE;
      min_dist += tdist*tdist;
      tdist = (x - minc1) * C1_SCALE;
      max_dist += tdist*tdist;
    } else {
      /* within cell range so no contribution to min_dist */
      if (x <= centerc1) {
	tdist = (x - maxc1) * C1_SCALE;
	max_dist += tdist*tdist;
      } else {
	tdist = (x - minc1) * C1_SCALE;
	max_dist += tdist*tdist;
      }
    }

    x = sl_colormap[2][i];
    if (x < minc2) {
      tdist = (x - minc2) * C2_SCALE;
      min_dist += tdist*tdist;
      tdist = (x - maxc2) * C2_SCALE;
      max_dist += tdist*tdist;
    } else if (x > maxc2) {
      tdist = (x - maxc2) * C2_SCALE;
      min_dist += tdist*tdist;
      tdist = (x - minc2) * C2_SCALE;
      max_dist += tdist*tdist;
    } else {
      /* within cell range so no contribution to min_dist */
      if (x <= centerc2) {
	tdist = (x - maxc2) * C2_SCALE;
	max_dist += tdist*tdist;
      } else {
	tdist = (x - minc2) * C2_SCALE;
	max_dist += tdist*tdist;
      }
    }

    mindist[i] = min_dist;	/* save away the results */
    if (max_dist < minmaxdist)
      minmaxdist = max_dist;
  }

  ncolors = 0;
  for (i = 0; i < numcolors; i++) {
    if (mindist[i] <= minmaxdist)
      colorlist[ncolors++] = (JSAMPLE) i;
  }
  return ncolors;
}


static void find_best_colors (minc0, minc1, minc2, numcolors,
			      colorlist, bestcolor)
     int minc0, minc1, minc2, numcolors;
     JSAMPLE colorlist[];
     JSAMPLE bestcolor[];
{
  int ic0, ic1, ic2;
  int i, icolor;
  register INT32 * bptr;	/* pointer into bestdist[] array */
  JSAMPLE * cptr;		/* pointer into bestcolor[] array */
  INT32 dist0, dist1;		/* initial distance values */
  register INT32 dist2;		/* current distance in inner loop */
  INT32 xx0, xx1;		/* distance increments */
  register INT32 xx2;
  INT32 inc0, inc1, inc2;	/* initial values for increments */
  /* This array holds the distance to the nearest-so-far color for each cell */
  INT32 bestdist[BOX_C0_ELEMS * BOX_C1_ELEMS * BOX_C2_ELEMS];

  /* Initialize best-distance for each cell of the update box */
  bptr = bestdist;
  for (i = BOX_C0_ELEMS*BOX_C1_ELEMS*BOX_C2_ELEMS-1; i >= 0; i--)
    *bptr++ = 0x7FFFFFFFL;
  
  /* Nominal steps between cell centers ("x" in Thomas article) */
#define STEP_C0  ((1 << C0_SHIFT) * C0_SCALE)
#define STEP_C1  ((1 << C1_SHIFT) * C1_SCALE)
#define STEP_C2  ((1 << C2_SHIFT) * C2_SCALE)
  
  for (i = 0; i < numcolors; i++) {
    icolor = colorlist[i];
    /* Compute (square of) distance from minc0/c1/c2 to this color */
    inc0 = (minc0 - (int) sl_colormap[0][icolor]) * C0_SCALE;
    dist0 = inc0*inc0;
    inc1 = (minc1 - (int) sl_colormap[1][icolor]) * C1_SCALE;
    dist0 += inc1*inc1;
    inc2 = (minc2 - (int) sl_colormap[2][icolor]) * C2_SCALE;
    dist0 += inc2*inc2;
    /* Form the initial difference increments */
    inc0 = inc0 * (2 * STEP_C0) + STEP_C0 * STEP_C0;
    inc1 = inc1 * (2 * STEP_C1) + STEP_C1 * STEP_C1;
    inc2 = inc2 * (2 * STEP_C2) + STEP_C2 * STEP_C2;
    /* Now loop over all cells in box, updating distance per Thomas method */
    bptr = bestdist;
    cptr = bestcolor;
    xx0 = inc0;
    for (ic0 = BOX_C0_ELEMS-1; ic0 >= 0; ic0--) {
      dist1 = dist0;
      xx1 = inc1;
      for (ic1 = BOX_C1_ELEMS-1; ic1 >= 0; ic1--) {
	dist2 = dist1;
	xx2 = inc2;
	for (ic2 = BOX_C2_ELEMS-1; ic2 >= 0; ic2--) {
	  if (dist2 < *bptr) {
	    *bptr = dist2;
	    *cptr = (JSAMPLE) icolor;
	  }
	  dist2 += xx2;
	  xx2 += 2 * STEP_C2 * STEP_C2;
	  bptr++;
	  cptr++;
	}
	dist1 += xx1;
	xx1 += 2 * STEP_C1 * STEP_C1;
      }
      dist0 += xx0;
      xx0 += 2 * STEP_C0 * STEP_C0;
    }
  }
}


static void fill_inverse_cmap (c0, c1, c2)
     int c0, c1, c2;
{
  hist2d * histogram = sl_histogram;
  int minc0, minc1, minc2;	/* lower left corner of update box */
  int ic0, ic1, ic2;
  register JSAMPLE * cptr;	/* pointer into bestcolor[] array */
  register histptr cachep;	/* pointer into main cache array */
  /* This array lists the candidate colormap indexes. */
  JSAMPLE colorlist[MAXNUMCOLORS];
  int numcolors;		/* number of candidate colors */
  /* This array holds the actually closest colormap index for each cell. */
  JSAMPLE bestcolor[BOX_C0_ELEMS * BOX_C1_ELEMS * BOX_C2_ELEMS];

  /* Convert cell coordinates to update box ID */
  c0 >>= BOX_C0_LOG;
  c1 >>= BOX_C1_LOG;
  c2 >>= BOX_C2_LOG;

  minc0 = (c0 << BOX_C0_SHIFT) + ((1 << C0_SHIFT) >> 1);
  minc1 = (c1 << BOX_C1_SHIFT) + ((1 << C1_SHIFT) >> 1);
  minc2 = (c2 << BOX_C2_SHIFT) + ((1 << C2_SHIFT) >> 1);
  
  numcolors = find_nearby_colors(minc0, minc1, minc2, colorlist);

  /* Determine the actually nearest colors. */
  find_best_colors(minc0, minc1, minc2, numcolors, colorlist, bestcolor);

  /* Save the best color numbers (plus 1) in the main cache array */
  c0 <<= BOX_C0_LOG;		/* convert ID back to base cell indexes */
  c1 <<= BOX_C1_LOG;
  c2 <<= BOX_C2_LOG;
  cptr = bestcolor;
  for (ic0 = 0; ic0 < BOX_C0_ELEMS; ic0++) {
    for (ic1 = 0; ic1 < BOX_C1_ELEMS; ic1++) {
      cachep = & histogram[c0+ic0][c1+ic1][c2];
      for (ic2 = 0; ic2 < BOX_C2_ELEMS; ic2++) {
	*cachep++ = (histcell) (*cptr++ + 1);
      }
    }
  }
}


static void slow_map_pixels (pic24, width, height, pic8)
     byte *pic24, *pic8;
     int   width, height;
{
  register LOCFSERROR cur0, cur1, cur2;	/* current error or pixel value */
  LOCFSERROR belowerr0, belowerr1, belowerr2; /* error for pixel below cur */
  LOCFSERROR bpreverr0, bpreverr1, bpreverr2; /* error for below/prev col */
  register FSERRPTR errorptr;	/* => fserrors[] at column before current */
  JSAMPROW inptr;		/* => current input pixel */
  JSAMPROW outptr;		/* => current output pixel */
  histptr cachep;
  int dir;			/* +1 or -1 depending on direction */
  int dir3;			/* 3*dir, for advancing inptr & errorptr */
  int row, col;
  int *error_limit = sl_error_limiter;
  JSAMPROW colormap0 = sl_colormap[0];
  JSAMPROW colormap1 = sl_colormap[1];
  JSAMPROW colormap2 = sl_colormap[2];
  hist2d * histogram = sl_histogram;

  for (row = 0; row < height; row++) {

    if ((row&0x3f) == 0) WaitCursor();
    ProgressMeter(0, height-1, row, "Dither");

    inptr = & pic24[row * width * 3];
    outptr = & pic8[row * width];
    if (sl_on_odd_row) {
      /* work right to left in this row */
      inptr += (width-1) * 3;	/* so point to rightmost pixel */
      outptr += width-1;
      dir = -1;
      dir3 = -3;
      errorptr = sl_fserrors + (width+1)*3; /* => entry after last column */
      sl_on_odd_row = FALSE;	/* flip for next time */
    } else {
      /* work left to right in this row */
      dir = 1;
      dir3 = 3;
      errorptr = sl_fserrors;	/* => entry before first real column */
      sl_on_odd_row = TRUE;	/* flip for next time */
    }
    /* Preset error values: no error propagated to first pixel from left */
    cur0 = cur1 = cur2 = 0;
    /* and no error propagated to row below yet */
    belowerr0 = belowerr1 = belowerr2 = 0;
    bpreverr0 = bpreverr1 = bpreverr2 = 0;

    for (col = width; col > 0; col--) {
      cur0 = (cur0 + errorptr[dir3+0] + 8) >> 4;
      cur1 = (cur1 + errorptr[dir3+1] + 8) >> 4;
      cur2 = (cur2 + errorptr[dir3+2] + 8) >> 4;
      cur0 = error_limit[cur0];
      cur1 = error_limit[cur1];
      cur2 = error_limit[cur2];
      cur0 += inptr[0];
      cur1 += inptr[1];
      cur2 += inptr[2];
      RANGE(cur0, 0, 255);
      RANGE(cur1, 0, 255);
      RANGE(cur2, 0, 255);
      /* Index into the cache with adjusted pixel value */
      cachep = & histogram[cur0>>C0_SHIFT][cur1>>C1_SHIFT][cur2>>C2_SHIFT];
      /* If we have not seen this color before, find nearest colormap */
      /* entry and update the cache */
      if (*cachep == 0)
	fill_inverse_cmap(cur0>>C0_SHIFT, cur1>>C1_SHIFT, cur2>>C2_SHIFT);
      /* Now emit the colormap index for this cell */
      { register int pixcode = *cachep - 1;
	*outptr = (JSAMPLE) pixcode;
	/* Compute representation error for this pixel */
	cur0 -= (int) colormap0[pixcode];
	cur1 -= (int) colormap1[pixcode];
	cur2 -= (int) colormap2[pixcode];
      }
      /* Compute error fractions to be propagated to adjacent pixels.
       * Add these into the running sums, and simultaneously shift the
       * next-line error sums left by 1 column.
       */
      { register LOCFSERROR bnexterr, delta;

	bnexterr = cur0;	/* Process component 0 */
	delta = cur0 * 2;
	cur0 += delta;		/* form error * 3 */
	errorptr[0] = (FSERROR) (bpreverr0 + cur0);
	cur0 += delta;		/* form error * 5 */
	bpreverr0 = belowerr0 + cur0;
	belowerr0 = bnexterr;
	cur0 += delta;		/* form error * 7 */
	bnexterr = cur1;	/* Process component 1 */
	delta = cur1 * 2;
	cur1 += delta;		/* form error * 3 */
	errorptr[1] = (FSERROR) (bpreverr1 + cur1);
	cur1 += delta;		/* form error * 5 */
	bpreverr1 = belowerr1 + cur1;
	belowerr1 = bnexterr;
	cur1 += delta;		/* form error * 7 */
	bnexterr = cur2;	/* Process component 2 */
	delta = cur2 * 2;
	cur2 += delta;		/* form error * 3 */
	errorptr[2] = (FSERROR) (bpreverr2 + cur2);
	cur2 += delta;		/* form error * 5 */
	bpreverr2 = belowerr2 + cur2;
	belowerr2 = bnexterr;
	cur2 += delta;		/* form error * 7 */
      }
      /* At this point curN contains the 7/16 error value to be propagated
       * to the next pixel on the current line, and all the errors for the
       * next line have been shifted over.  We are therefore ready to move on.
       */
      inptr += dir3;		/* Advance pixel pointers to next column */
      outptr += dir;
      errorptr += dir3;		/* advance errorptr to current column */
    }
    /* Post-loop cleanup: we must unload the final error values into the
     * final fserrors[] entry.  Note we need not unload belowerrN because
     * it is for the dummy column before or after the actual array.
     */
    errorptr[0] = (FSERROR) bpreverr0; /* unload prev errs into array */
    errorptr[1] = (FSERROR) bpreverr1;
    errorptr[2] = (FSERROR) bpreverr2;
  }
}


static void init_error_limit ()
/* Allocate and fill in the error_limiter table */
/* Note this should be done only once. */
{
  int * table;
  int in, out;

  table = (int *) malloc((size_t) ((255*2+1) * sizeof(int)));
  if (! table) return;

  table += 255;		/* so can index -255 .. +255 */
  sl_error_limiter = table;

#define STEPSIZE ((255+1)/16)
  /* Map errors 1:1 up to +- 255/16 */
  out = 0;
  for (in = 0; in < STEPSIZE; in++, out++) {
    table[in] = out; table[-in] = -out;
  }
  /* Map errors 1:2 up to +- 3*255/16 */
  for (; in < STEPSIZE*3; in++, out += (in&1) ? 0 : 1) {
    table[in] = out; table[-in] = -out;
  }
  /* Clamp the rest to final out value (which is (255+1)/8) */
  for (; in <= 255; in++) {
    table[in] = out; table[-in] = -out;
  }
#undef STEPSIZE
}
