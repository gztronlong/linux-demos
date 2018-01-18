#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "cJSON.h"

char *Create_json();
void Parse_json(char *data);

int main(int argc, const char *argv[]){
    printf("\n************************\n");
    printf("******Create json*******");
    printf("\n************************\n");
    char *json = Create_json();

    printf("\n*************************\n");
    printf("*******Parse json********");
    printf("\n*************************\n");
    Parse_json(json);

    return 0;
}

char *Create_json(){
    cJSON *root, *sub1, *sub2;

    root = cJSON_CreateObject();
    sub1 = cJSON_CreateObject();
    sub2 = cJSON_CreateObject();

    assert(root);
    assert(sub1);
    assert(sub2);

    cJSON_AddStringToObject(root,"Country","China");
    cJSON_AddItemToObject(root,"PInfo",sub1);
    cJSON_AddStringToObject(sub1,"Name","XiaoMing");
    cJSON_AddNumberToObject(sub1,"Age",18);
    cJSON_AddFalseToObject(sub1,"Married");
    cJSON_AddItemToObject(sub1,"Body",sub2);
    cJSON_AddNumberToObject(sub2,"Hight(CM)",170);
    cJSON_AddNumberToObject(sub2,"Wight(Kg)",58);

    char *s = cJSON_Print(root);
    assert(s);
    printf("%s\n",s);

    return s;
}


void Parse_json(char *data){
    //parse json
    cJSON *proot = cJSON_Parse(data);
    assert(proot);

    //access element
    cJSON *Country = cJSON_GetObjectItem(proot,"Country");
    if(Country->type == cJSON_String){
        printf("\nCountry_Type: %d\n",Country->type);
        printf("Country: %s\n",Country->valuestring);
    }

    cJSON *PInfo = cJSON_GetObjectItem(proot,"PInfo");
    printf("\nPInfo_Type: %d\n",PInfo->type);

    cJSON *Name = cJSON_GetObjectItem(PInfo,"Name");
    if(Name->type == cJSON_String){
        printf("\nName_Type: %d\n",Name->type);
        printf("Name: %s\n",Name->valuestring);
    }

    cJSON *Age = cJSON_GetObjectItem(PInfo,"Age");
    if(Age->type == cJSON_Number){
        printf("\nAge_Type: %d\n",Age->type);
        printf("Age: %d\n",Age->valueint);
    }

    cJSON *Married = cJSON_GetObjectItem(PInfo,"Married");
    if(Married->type == cJSON_False){
        printf("\nMarried_Type: %d\n",Married->valueint);
        printf("Married: %s\n",Married->valueint?"True":"False");
    }

    cJSON *Body = cJSON_GetObjectItem(PInfo,"Body");
    printf("\nBody_Type: %d\n",Body->type);

    cJSON *Hight = cJSON_GetObjectItem(Body,"Hight(CM)");
    if(Hight->type == cJSON_Number){
        printf("\nHight_Type: %d\n",Hight->type);
        printf("Hight(CM): %d\n",Hight->valueint);
    }

    cJSON *Wight = cJSON_GetObjectItem(Body,"Wight(Kg)");
    if(Wight->type == cJSON_Number){
        printf("\nWight_Type: %d\n",Wight->type);
        printf("Wight(Kg): %d\n",Wight->valueint);
    }

    cJSON_Delete(proot);
}
