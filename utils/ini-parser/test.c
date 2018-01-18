
/* Thanks to TI */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include "dictionary.h"
#include "iniparser.h"

#define INIT  "sample.ini"

int main() {
    int i;
    int sec_count;
    dictionary * dic = NULL ;

    /******************/
    /*iniparser.h demo*/
    /******************/

    //1,解析dictionary对象并返回(分配内存)dictionary对象
    dic = iniparser_load(INIT);

    //2,保存dictionary对象到file 
    iniparser_dump_ini(dic, stdout);

    //3,获取dictionary对象的section个数
    sec_count = iniparser_getnsec(dic);

    //4,获取dictionary对象的 第n个section的名字 
    for (i = 0; i < sec_count; i++) {
        printf("%s\n", iniparser_getsecname(dic, i)); 
    }

    //5,保存dictionary对象一个section到file 
    iniparser_dumpsection_ini(dic, "file00", stdout);

    //6,保存dictionary对象到file
    iniparser_dump(dic, stdout);

    //7,获取dictionary对象某个section下的key个数
    int num = iniparser_getsecnkeys(dic, "file01");
    printf("\n\nfile01下的key个数:%d\n", num);

    //8,获取dictionary对象某个section下的key个数
    iniparser_getseckeys(dic, "file02");


    //9,返回dictionary对象的section:key对应的字串值  
    char *def;
    char *np = iniparser_getstring(dic, "file02:speed", def);
    printf("(string)file02:speed = %s\n", np);

    //10,返回idictionary对象的section:key对应的整形值
    int x, y;
    x = iniparser_getint(dic, "file02:speed", y);
    printf("(int)   file02:speed = %d\n", x);

    //11,返回dictionary对象的section:key对应的双浮点值
    double d1, d2;
    d1 = iniparser_getdouble(dic, "file02:speed", d2);
    printf("(double)file02:speed = %.1f\n", d1);

    //12,返回dictionary对象的section:key对应的布尔值 
    bool bl1, bl2;
    bl1 = iniparser_getboolean(dic, "file01:speed", bl2);
    printf("(bool)  file02:speed = %d\n", bl1);

    //13,设置dictionary对象的某个section:key的值
    int z = iniparser_set(dic, "file02:speed", "4800");
    if(!z) {
        printf("Succeed to reset!\n");
        x = iniparser_getint(dic, "file02:speed", y);
        printf("(int)   file02:speed = %d\n", x);
    }
    else
        printf("Failed to reset!\n");

    //14,删除dictionary对象中某个section:key
    iniparser_unset(dic, "file02:speed");
    iniparser_dumpsection_ini(dic, "file02", stdout);

    //15,判断dictionary对象中是否存在某个section:key 
    bool bl = iniparser_find_entry(dic, "file02:speed");
    if(bl)
        printf("Have found the key!\n");
    else
        printf("Have't found the key!\n");

    //16,释放dictionary对象(内存) 
    //iniparser_freedict(dic);


    /*******************/
    /*dictionary.h demo*/
    /*******************/

    //1,计算关键词的hash值
    unsigned hs = dictionary_hash("file01:ip");
    printf("Key_Hash_Val = %d\n", hs);

    //2,创建dictionary对象
    dictionary *New_Dic = dictionary_new(512);

    //3,设置dictionary对象的key值 
    int judge = dictionary_set(dic, "file00:width", "1234");
    if(!judge)
        printf("Seucceed to reset!\n");
    else
        printf("Failed to reset!\n");

    //4,获取dictionary对象的key值 
    char * Def;
    char * key_val = dictionary_get(dic, "file00:width", Def);
    printf("file00:width = %s\n", key_val);

    //5,删除dictionary对象的key值  
    dictionary_unset(dic, "file00:width");

    //6,保存dictionary对象  
    dictionary_dump(dic, stdout);

    //7,删除dictionary对象                                                 
    dictionary_del(New_Dic);

    //释放dictionary对象(内存) 
    iniparser_freedict(dic);


    return 0;
}
