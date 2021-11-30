/**
// @file: jWrite.h
//
// @author: TonyWilk, Mar 2015 (original work)
// @author: Sergio Sáez, Dec 2019 (FILE support)
//
// A *really* simple JSON writer in C  (C89)
// - a collection of functions to generate JSON semi-automatically
//
// The idea is to simplify writing native C values into a JSON string and
// to provide some error trapping to ensure that the result is valid JSON.
//
// Example:
//		jwOpen( buffer, buflen, JW_OBJECT, JW_PRETTY );		// open root node as object
//		jwObj_string( "key", "value" );
//		jwObj_int( "int", 1 );
//		jwObj_array( "anArray");
//			jwArr_int( 0 );
//			jwArr_int( 1 );
//			jwArr_int( 2 );
//		jwEnd();
//		err= jwClose();								// close root object
//
// results in:
//
//		{
//		    "key": "value",
//		    "int": 1,
//		    "anArray": [
//		        0,
//		        1,
//		        2
//		    ]
//		}
//
// Note that jWrite handles string quoting and getting commas in the right place.
// If the sequence of calls is incorrect
// e.g.
//		jwOpen( buffer, buflen, JW_OBJECT, 1 );
//		jwObj_string( "key", "value" );
//			jwArr_int( 0 );
//      ...
//
// then the error code returned from jwClose() would indicate that you attempted to
// put an array element into an object (instead of a key:value pair)
// To locate the error, the supplied buffer has the JSON created upto the error point
// and a call to jwErrorPos() would return the function call at which the error occurred
// - in this case 3, the 3rd function call "jwArr_int(0)" is not correct at this point.
//
// The root JSON type can be JW_OBJECT or JW_ARRAY.
//
// For more information on each function, see the prototypes below.
//
//
// GLOBAL vs. Application-Supplied Control Structure
// -------------------------------------------------
// jWrite requires a jWriteControl structure to save the internal state.
// For many applications it is much simpler for this to be a global variable as
// used by the above examples.
//
// To use multiple instances of jWrite, an application has to supply unique instances
// of jWriteControl structures. 
//
// This feature is enabled by commenting out the definition of JW_GLOBAL_CONTROL_STRUCT
//
// All the jWrite functions then take an additional parameter: a ptr to the structure
// e.g.
//		struct jWriteControl jwc;
//
//		jwOpen( &jwc, buffer, buflen, JW_OBJECT, 1 );
//		jwObj_string( &jwc, "key", "value" );
//		jwObj_int( &jwc, "int", 1 );
//		jwObj_array( &jwc, "anArray");
//			jwArr_int( &jwc, 0 );
//			jwArr_int( &jwc, 1 );
//			jwArr_int( &jwc, 2 );
//		jwEnd( &jwc );
//		err= jwClose( &jwc );
//
// - which is more flexible, but a pain to type in !
//
// FILE support:
//  Example:
//		jwOpen( f_out, JW_OBJECT, JW_PRETTY );		// open root node as object
//
 */

#ifndef JWRITE_H
#define JWRITE_H

#include <stdint.h>

#define JW_GLOBAL_CONTROL_STRUCT    // <--- comment this out to use applic-supplied jWriteControl
#define JW_FILE_STREAM              // <--- comment this out to use a buffer instead of direct writting on a stream

#define JWRITE_STACK_DEPTH 32            // max nesting depth of objects/arrays

#define JW_COMPACT    0                    // output string control for jwOpen()
#define JW_PRETTY    1                    // pretty adds \n and indentation

#ifdef JW_FILE_STREAM
#include <stdio.h>
#endif

enum jwNodeType {
    JW_OBJECT = 1,
    JW_ARRAY
};

struct jwNodeStack {
    enum jwNodeType nodeType;
    int elementNo;
};

struct jWriteControl {
#ifdef JW_FILE_STREAM
    FILE *stream;
#else
    char *buffer;                        // pointer to application's buffer
    unsigned int buflen;                // length of buffer
    char *bufp;                            // current write position in buffer
#endif
    char tmpbuf[32];                    // local buffer for int/double convertions
    int error;                            // error code
    int callNo;                            // API call on which error occurred
    struct jwNodeStack nodeStack[JWRITE_STACK_DEPTH];    // stack of array/object nodes
    int stackpos;
    int isPretty;                        // 1= pretty output (inserts \n and spaces)
};

// Error Codes
// -----------
#define JWRITE_OK           0
#define JWRITE_BUF_FULL     1        // output buffer full
#define JWRITE_NOT_ARRAY    2        // tried to write Array value into Object
#define JWRITE_NOT_OBJECT    3        // tried to write Object key/value into Array
#define JWRITE_STACK_FULL    4        // array/object nesting > JWRITE_STACK_DEPTH
#define JWRITE_STACK_EMPTY    5        // stack underflow error (too many 'end's)
#define JWRITE_NEST_ERROR    6        // nesting error, not all objects closed when jwClose() called

#ifndef CPROTO

#include "jwrite_api.h"

#endif

#endif /* end of jWrite.h */
