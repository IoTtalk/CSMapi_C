//
//  CSMapi.h
//  CSMapi


#ifndef CSMapi_h
#define CSMapi_h

#include <stdio.h>
#include <json-c/json.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#endif /* CSMapi_h */

typedef const char JSONString;
typedef unsigned int BOOL;

typedef struct
{
    long httpCode;
    int success;//boolean
}Response;

typedef struct
{
    char *buffer;
    size_t size;
}Buffer;


typedef union{
    double *doubleArray;
    int *intArray;
    char **stringArray;
}ItemArray;

typedef struct
{
    ItemArray itemArray;
    json_type type;
    int dimension;
}Data;


extern void setEndPoint(const char *ep);
extern Response register_ (const char *d_id, const char* profile);
extern Response push (const char *d_id, const char *df_name, const  char* data);
extern Response pull (const char *d_id, const char *df_name, Buffer *data);
extern Response deregister (const char *d_id);

extern JSONString *makeProfileJSONString(const char *dm_name, const char *d_name,
                                    const char *u_name,BOOL is_sim,const char *(df_list[]),int df_list_length);
extern JSONString *makeDataJSONArrayString(Data dataItem);
extern Data getDataItem(const char *JSONString);



