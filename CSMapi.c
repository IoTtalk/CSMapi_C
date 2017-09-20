//
//  CSMapi.c
//  CSMapi


#include "CSMapi.h"


char *endPoint = NULL;
CURL *curlHandle;

// This is the function we pass to Libcurl, which writes the output to a Buffer
static size_t writeMemoryCallback
(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    Buffer * mem = (Buffer *) data;
    mem->buffer = realloc(mem->buffer, mem->size+realsize+1);
    if (mem->buffer)
    {
        memcpy(&(mem->buffer[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->buffer[mem->size] = 0;
    }
    return realsize;
}

Response get(const char *url,Buffer *output)
{
    Response response;
    if(url != NULL){
 
        CURLcode result;
        
        output->buffer = NULL;
        output->size = 0;
        
        curlHandle = curl_easy_init();
        curl_easy_setopt(curlHandle, CURLOPT_URL, url);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)output);
        curl_easy_setopt(curlHandle, CURLOPT_URL, url);
        result = curl_easy_perform(curlHandle);
        curl_easy_getinfo (curlHandle, CURLINFO_RESPONSE_CODE, &response.httpCode);
        if (response.httpCode == 200 && result != CURLE_ABORTED_BY_CALLBACK){//success
            response.success = 1;
        }
        else{//failed
            response.success = 0;
        }
        curl_easy_cleanup(curlHandle);
    }
    else{
        fprintf(stderr,"Url can not be null!\n");
        response.httpCode = -1;
        response.success = 0;
    }
    return response;
}

Response post(const char *url, const char *body)
{
    curlHandle = curl_easy_init();
    Response response;

    if(url != NULL){
        CURLcode result;
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        curlHandle = curl_easy_init();
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curlHandle, CURLOPT_URL, url);
        curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, body);
        result = curl_easy_perform(curlHandle);
        curl_easy_getinfo (curlHandle, CURLINFO_RESPONSE_CODE, &response.httpCode);
        if (response.httpCode == 200 && result != CURLE_ABORTED_BY_CALLBACK){//success
            response.success = 1;
        }
        else{//failed
            response.success = 0;
        }
        curl_easy_cleanup(curlHandle);

    }
    else{
        fprintf(stderr,"Url can not be null!\n");
        response.httpCode = -1;
        response.success = 0;
    }
    return response;
}

Response put(const char *url, const char *body)
{
    curlHandle = curl_easy_init();
    Response response;
    
    if(url != NULL){
        CURLcode result;
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        curlHandle = curl_easy_init();
        curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curlHandle, CURLOPT_URL, url);
        curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, body);
        result = curl_easy_perform(curlHandle);
        curl_easy_getinfo (curlHandle, CURLINFO_RESPONSE_CODE, &response.httpCode);
        if (response.httpCode == 200 && result != CURLE_ABORTED_BY_CALLBACK){//success
            response.success = 1;
        }
        else{//failed
            response.success = 0;
        }
        curl_easy_cleanup(curlHandle);
        
    }
    else{
        fprintf(stderr,"Url can not be null!\n");
        response.httpCode = -1;
        response.success = 0;
    }
    return response;
}

Response delete(const char *url)
{
    curlHandle = curl_easy_init();
    Response response;
    if(url != NULL){
        CURLcode result;
        curlHandle = curl_easy_init();
        curl_easy_setopt(curlHandle, CURLOPT_URL, url);
        curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "DELETE");
        result = curl_easy_perform(curlHandle);
        curl_easy_getinfo (curlHandle, CURLINFO_RESPONSE_CODE, &response.httpCode);
        if (response.httpCode == 200 && result != CURLE_ABORTED_BY_CALLBACK){//success
            response.success = 1;
        }
        else{//failed
            response.success = 0;
        }
        curl_easy_cleanup(curlHandle);
        
    }
    else{
        fprintf(stderr,"Url can not be null!\n");
        response.httpCode = -1;
        response.success = 0;
    }
    return response;
}

void setEndPoint(const char *ep)
{
    endPoint = realloc(endPoint,strlen(ep)+1);
    strcpy(endPoint,ep);
    printf("%s\n",endPoint);
}

Response register_(const char *d_id, const char* profile)
{
    char url[strlen(d_id)+strlen(endPoint)+2];
    strcpy(url,endPoint);
    strcat(url,"/");
    strcat(url,d_id);
    return post(url,profile);
}

Response push(const char *d_id, const char *df_name, const char* data)
{
    char url[strlen(d_id)+strlen(endPoint)+strlen(df_name)+3];
    strcpy(url,endPoint);
    strcat(url,"/");
    strcat(url,d_id);
    strcat(url,"/");
    strcat(url,df_name);
    return put(url,data);
}
Response pull(const char *d_id, const char *df_name, Buffer *data)
{
    char url[strlen(d_id)+strlen(endPoint)+strlen(df_name)+3];
    strcpy(url,endPoint);
    strcat(url,"/");
    strcat(url,d_id);
    strcat(url,"/");
    strcat(url,df_name);
    return get(url,data);
}

Response deregister (const char *d_id)
{
    char url[strlen(d_id)+strlen(endPoint)+2];
    strcpy(url,endPoint);
    strcat(url,"/");
    strcat(url,d_id);
    return delete(url);
}

JSONString *makeProfileJSONString(const char *dm_name, const char *d_name,
                              const char *u_name, BOOL is_sim, const char *(df_list[]),int df_list_length){
    int i;
    
    json_object *object, *profile, *dfList;
    dfList = json_object_new_array();
    profile = json_object_new_object();
    object = json_object_new_object();
    
    if(df_list == NULL){
        fprintf(stderr,"df_list can not be NULL\n");
        EXIT_FAILURE;
    }
    else{
        for(i = 0; i < df_list_length; i++)
            json_object_array_add(dfList, json_object_new_string(df_list[i]));
    }
    if(dm_name != NULL)
        json_object_object_add(profile, "dm_name", json_object_new_string(dm_name));
    else
        json_object_object_add(profile, "dm_name", json_object_new_string(""));
    if(d_name != NULL)
        json_object_object_add(profile, "d_name", json_object_new_string(d_name));
    else
        json_object_object_add(profile, "d_name", json_object_new_string(""));

    if(u_name != NULL)
        json_object_object_add(profile, "u_name", json_object_new_string(u_name));
    else
        json_object_object_add(profile, "d_name", json_object_new_string(""));

    if(is_sim == 0)
        json_object_object_add(profile, "is_sim", json_object_new_boolean(FALSE));
    else
        json_object_object_add(profile, "is_sim", json_object_new_boolean(TRUE));

    json_object_object_add(profile, "df_list", dfList);
    
    json_object_object_add(object, "profile", profile);
    
    // Output to string
    //    printf("%s\n", json_object_to_json_string(object));
    return json_object_to_json_string(object);
}

JSONString *makeDataJSONArrayString(Data dataItem)
{
    int i;
    json_object *data, *jsonDataArray;
    jsonDataArray = json_object_new_array();
    data = json_object_new_object();
    
    if(dataItem.itemArray.doubleArray == NULL && dataItem.itemArray.intArray == NULL && dataItem.itemArray.stringArray == NULL){
        fprintf(stderr,"data can not be NULL\n");
        EXIT_FAILURE;
    }
    
    if(dataItem.type == json_type_string){
        for(i = 0; i < dataItem.dimension; i++)
            json_object_array_add(jsonDataArray, json_object_new_string(dataItem.itemArray.stringArray[i]));
    }
    else if(dataItem.type == json_type_int){
        for(i = 0; i < dataItem.dimension; i++)
            json_object_array_add(jsonDataArray, json_object_new_int(dataItem.itemArray.intArray[i]));
    }
    else if(dataItem.type == json_type_double){
        //if no fraction, ex:1.0 add 0.0000001 become 1.0000001, to make json type parse correct.
        if (dataItem.itemArray.doubleArray[0] == (int)(dataItem.itemArray.doubleArray[0])){
            dataItem.itemArray.doubleArray[0]+=0.0000001;
        }
        for(i = 0; i < dataItem.dimension; i++){
            json_object_array_add(jsonDataArray, json_object_new_double(dataItem.itemArray.doubleArray[i]));
        }
    }
        
    
    json_object_object_add(data, "data", jsonDataArray);
    return json_object_to_json_string(data);

}

Data getDataItem(const char *JSONString)
{
    Data returnItem;
    json_type type;
    json_object *jobj = json_tokener_parse(JSONString);
    
    json_object_object_foreach(jobj, key, val){
        type = json_object_get_type(val);
        returnItem.type = type;
        switch (type) {
            case json_type_int:
            {
                int i[1];
                i[0] = json_object_get_int(val);
                returnItem.itemArray.intArray = i;
                break;
            }
            case json_type_double:
            {
                double d[1];
                d[0] = json_object_get_double(val);
                returnItem.itemArray.doubleArray = d;
                break;
            }
            case json_type_boolean:
            {
                int i[1];
                i[0] = json_object_get_boolean(val);
                returnItem.itemArray.intArray = i;
                break;
            }
            case json_type_string:
            {
                char *(s[1]);
                s[0] = (char *)json_object_get_string(val);
                returnItem.itemArray.stringArray = s;
                break;
            }
            case json_type_array:
            {
                int i = 0;
                int arrayLen = json_object_array_length(val);
                json_type t = json_object_get_type(json_object_array_get_idx(val, 0));
                json_type t0 = json_object_get_type(json_object_array_get_idx(val, 0));
                if(arrayLen == 2){
                    json_type t1 = json_object_get_type(json_object_array_get_idx(val, 1));
                    i = (t0 == json_type_string && t1 == json_type_array) ? 1 : 0;
                    if(i == 1)
                        t = t1;
                }
                switch (t) {
                    case json_type_array:
                    {
                        json_object *data = json_object_new_object();
                        json_object_object_add(data, "data", json_object_array_get_idx(val, i));
                        return getDataItem(json_object_to_json_string(data));
                        break;
                    }
                    case json_type_int:
                    {
                        int intArray[arrayLen];
                        int j;
                        for(j = 0; j < arrayLen; j++)
                            intArray[j] = json_object_get_int(json_object_array_get_idx(val, j));
                        returnItem.itemArray.intArray = intArray;
                        returnItem.type = json_type_int;
                        returnItem.dimension = arrayLen;
                        break;
                    }
                    case json_type_double:
                    {
                        double doubleArray[arrayLen];
                        int j;
                        for(j = 0; j < arrayLen; j++)
                            doubleArray[j] = json_object_get_double(json_object_array_get_idx(val, j));
                        returnItem.itemArray.doubleArray = doubleArray;
                        returnItem.type = json_type_double;
                        returnItem.dimension = arrayLen;
                        break;
                    }
                    case json_type_string:
                    {
                        char *(stringArray[arrayLen]);
                        int j;
                        for(j = 0; j < arrayLen; j++)
                            stringArray[j] = (char *)json_object_get_string(json_object_array_get_idx(val, j));
                        returnItem.itemArray.stringArray = stringArray;
                        returnItem.type = json_type_string;
                        returnItem.dimension = arrayLen;
                        break;
                    }
                    default:
                    {
                        fprintf(stderr,"can only parse json array with int, double and string type\n");
                        returnItem.type = json_type_null;
                        break;
                    }
                }
                break;
            }
            case json_type_object:
            {
                fprintf(stderr,"not support yet!\n");
            }
            case json_type_null:
            {
                fprintf(stderr,"json string format might be wrong!\n");
                break;
            }

        }
    }
    return returnItem;
}
