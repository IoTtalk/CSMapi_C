//
//  main.c
//  CSMapi

#include <stdio.h>
#include "CSMapi.h"


int main(int argc, const char * argv[]) {
    
    const char *endPoint = "http://10.211.55.5:9999";
    
    const char *macAddr = "0fafg3";
    
    const char *dmName = "MusicBox";
    
    const char *dName = "MusicBox05";
    
    const char *uNmae = "yb";
    
    BOOL isSim = 0;
    
    const char *(dfList[1]) = {"Luminance"};
    
    int dfListLength = 1;
    
    Response res;
    
    /*Set iottalk_url:9999*/
    setEndPoint(endPoint);
    
    /*Set profile and register*/
    printf("Register:");
    JSONString *profileJSONString = makeProfileJSONString(dmName, dName, uNmae, isSim,dfList, dfListLength);
    res = register_(macAddr,profileJSONString);
    
    /*Push L-O feature to iottalk*/
    printf("Push data:");
    Data pushData;
    pushData.type = json_type_double;
    double luminance[3] = {0.7};
    pushData.dimension = 1;
    pushData.itemArray.doubleArray = luminance;
    JSONString *dataJSONString = makeDataJSONArrayString(pushData);
    printf("%s\n",dataJSONString);
    res = push(macAddr, dfList[0], dataJSONString);
    
    /*Pull L-O feature from iottalk*/
    printf("Pull data:");
    Buffer data;
    res = pull(macAddr, dfList[0], &data);
    //printf("%s",data.buffer);
    Data pullData = getDataItem(data.buffer);
    int i;
    if(pullData.type == json_type_double)
        for(i = 0; i < pullData.dimension; i++)
            printf((i==pullData.dimension-1) ? "%f\n" : "%f ",pullData.itemArray.doubleArray[i]);
    
    /*Response code*/
    printf("HTTP Response code: %ld\n",res.httpCode);
    
    /*Deregister*/
    printf("Deregister\n");
    res = deregister(macAddr);
    
    return 0;
}
