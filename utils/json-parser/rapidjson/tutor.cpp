#include <iostream>
#include <cassert>
#include <string>
#include <cstdio>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;

const char *json = "{"
    "\"Str_val\":\"Hello World\","
    "\"Bool_val\":true,"
    "\"Null_val\":null,"
    "\"Int_val\":1024,"
    "\"Float_val\":3.1416,"
    "\"Array_val\":[1,2,3,4]"
"}";

void basic_usage(const char *data);
void write_json();
void read_json(const char *data);

int main(){
    cout<<"\n********************************"<<endl;
    cout<<"*******Basic Usage Of DOM*******"<<endl;
    cout<<"********************************\n"<<endl;
    basic_usage(json);

    cout<<"\n********************************"<<endl;
    cout<<"***********Write JSON***********"<<endl;
    cout<<"********************************\n"<<endl;
    write_json();

    cout<<"\n********************************"<<endl;
    cout<<"************Read JSON***********"<<endl;
    cout<<"********************************\n"<<endl;
    read_json(json);

    return 0;
}

/***************/
/***Basic Use***/
/***************/
void basic_usage(const char *data){
    Document doc;

    /* parser json */
    doc.Parse(data);

    cout<<"###-Query DOM-###\n"<<endl;

    /* query object */
    assert(doc.IsObject());

    /* query string */
    assert(doc.HasMember("Str_val"));
    assert(doc["Str_val"].IsString());
    cout<<"Str_val = "<<doc["Str_val"].GetString()<<endl;
    cout<<"The length of Str_val = "<<doc["Str_val"].GetStringLength()<<endl;

    /* query bool */
    assert(doc.HasMember("Bool_val"));
    assert(doc["Bool_val"].IsBool());
    assert(doc["Bool_val"].IsTrue());
    cout<<"Bool_val = ";
    doc["Bool_val"].GetBool()?(puts("True")):(puts("False"));

    /* query null */
    cout<<"Null_val = ";
    doc["Null_val"].IsNull()?(puts("null")):(puts("?"));

    /* query int */
    assert(doc.HasMember("Int_val"));
    assert(doc["Int_val"].IsNumber());
    assert(doc["Int_val"].IsInt());
    cout<<"Int_val = "<<doc["Int_val"].GetInt()<<endl;

    /* query float */
    assert(doc.HasMember("Float_val"));
    assert(doc["Float_val"].IsNumber());
    assert(doc["Float_val"].IsFloat());
    cout<<"Float_val = "<<doc["Float_val"].GetFloat()<<endl;

    /* query array */
    assert(doc.HasMember("Array_val"));
    assert(doc["Array_val"].IsArray());
    const Value &arr = doc["Array_val"];
    for(SizeType i = 0;i < arr.Size();i++)
        cout<<"Array_val["<<i+1<<"] = "<<arr[i].GetInt()<<endl;

    /* Query object */
    static const char* kTypeNames[] =
            { "Null", "False", "True", "Object", "Array", "String", "Number" };
    Value::ConstMemberIterator iter;
    for(iter = doc.MemberBegin();iter < doc.MemberEnd();iter++)
        cout<<"Type of member "<<iter->name.GetString()<<" is "
            <<kTypeNames[iter->value.GetType()]<<endl;

    cout<<"\n###-Modify DOM-###\n"<<endl;

    /* modify Str_val */
    doc["Str_val"] = "Modify Str_val"; 
    assert(doc.HasMember("Str_val"));
    assert(doc["Str_val"].IsString());
    cout<<"Str_val = "<<doc["Str_val"].GetString()<<endl;
    cout<<"The length of Str_val = "<<doc["Str_val"].GetStringLength()<<endl;

    /* add value to Array_val */
    Value &a = doc["Array_val"];
    Document::AllocatorType &allocator = doc.GetAllocator();
    for(int i = 5;i <= 7;i++)
        a.PushBack(i,allocator);
    assert(doc.HasMember("Array_val"));
    assert(doc["Array_val"].IsArray());
    for(SizeType i = 0;i < a.Size();i++)
        cout<<"Array_val["<<i+1<<"] = "<<a[i].GetInt()<<endl;

    /* add a string to DOM */
    Value Name;
    char buf[10];
    int len = sprintf(buf,"%s","Allen");
    Name.SetString(buf,static_cast<SizeType>(len),doc.GetAllocator());
    doc.AddMember("Author",Name,doc.GetAllocator());

    cout<<"\n###-Reformat JSON-###\n"<<endl;
    StringBuffer sb;
    PrettyWriter<StringBuffer> Writer(sb);
    doc.Accept(Writer);
    puts(sb.GetString());
}

/****************/
/***Write JSON***/
/****************/
void write_json(){
    StringBuffer strbuf;
    PrettyWriter<StringBuffer> Writer(strbuf);

    /* write int32_t data */
    Writer.StartObject();
    Writer.Key("Date");
    Writer.Int(20170416);

    /* write string data */
    Writer.Key("Weather");
    Writer.String("cloudy day");
    Writer.Key("Temp");
    Writer.String("29/22°C");

    /* write bool data */
    Writer.Key("Suitability to play?");
    Writer.Bool(true);

    /* write array data */
    Writer.Key("9~12am AQI");
    Writer.StartArray();
    Writer.Uint(65);
    Writer.Uint(65);
    Writer.Uint(65);
    Writer.Uint(63);
    Writer.EndArray();
    Writer.EndObject();

    cout<<strbuf.GetString()<<endl;
}


/***************/
/***Read JSON***/
/***************/
struct MyHandler{
    bool Null(){ cout << "Null()" << endl; return true; }

    bool Bool(bool b){ cout << "Bool(" << boolalpha << b << ")" << endl; return true;}

    bool Int(int i){ cout << "Int(" << i << ")" << endl; return true;}

    bool Int64(int64_t i){ cout << "Int64(" << i << ")" << endl; return true;}

    bool Uint(unsigned u){ cout << "Uint(" << u << ")" << endl; return true;}

    bool Uint64(uint64_t u){ cout << "Uint64(" << u <<")" << endl; return true;}

    bool Double(double d){ cout << "Double(" << d << ")" << endl; return true;}

    bool RawNumber(const char *str, SizeType length, bool copy){
        cout<<"Number("<< str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }

    bool String(const char *str, SizeType length, bool copy){
        cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }

    bool StartObject(){ cout << "StartObject()" << endl; return true;}

    bool Key(const char *str, SizeType length, bool copy){
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }

    bool EndObject(SizeType memberCount){ cout << "EndObject(" << memberCount << ")" <<endl;
        return true;
    }

    bool StartArray(){ cout << "StartArray()" << endl; return true; }

    bool EndArray(SizeType elementCount){ cout << "EndArray(" << elementCount << ")" << endl;
        return true;
    }
};

void read_json(const char *data)
{
    MyHandler handler;
    Reader reader;
    StringStream ss(data);
    reader.Parse(ss, handler);
}
