/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   debug.h
 * Author: plopez
 *
 * Created on 28 de octubre de 2017, 22:46
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

//#define DEBUG 


#define fnError(formato, args...)  { \
    fprintf(stderr,"Error: %s; función:%s; línea:%d\n",__FILE__, __func__, __LINE__);    \
    fprintf(stderr,formato, ##args);  \
    exit(1);}


#define fnWarning(formato, args...)  { \
    printf("Atención: %s; función:%s; línea:%d\n",__FILE__, __func__, __LINE__); \
    printf(formato, ##args);}

#ifdef DEBUG
#define fnDebug(enable, formato, args...) {\
    if (enable) {\
        printf("Debug: %s; función:%s; línea:%d\n",__FILE__, __func__, __LINE__); \
        printf(formato, ##args);}\
    }

#else
#define fnDebug(formato, args...)
#endif


#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H */

