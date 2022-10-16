#define _CRT_SECURE_NO_WARNINGS
#include <QCoreApplication>
#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QString>
#include <ctype.h>
#include <stddef.h>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <list>
#include <utility>
#include <iterator>
#include <fstream>
#include <QtNetwork>
#include <QFile>
#include "net2.h"
using namespace std;
void HL7_V2_Message_generator();//產生HL7 V2 Message檔案
void fetch_origin_document(char HL7[]);//取得原始HL7 V2 Message
void main_menu(char HL7[], char data[]);//主選單
void sub_menu(int opt);//子選單(產生Json)
void analysis(char HL7[],char data[],int opt);//解析HL7 V2 Message的PID,AL1,DG1,MSH
void generate_XML_format(char HL7[], char data[], int opt);//產生PID XML,AL1 XML,DG1 XML
void generate_JSON_format(int opt);//產生PID JSON,AL1 JSON,DG1 JSON
void export_full_json(char HL7[],char data[]);//產生AllergyIntolerance的JSON檔，並Put、Post到server上
void read_csv_file_and_export_to_json();
struct file {
    string title;
    string detail;
};
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    char HL7[1000] = { "" };
    char data[50] = { "" };
    HL7_V2_Message_generator();
    fetch_origin_document(HL7);
    main_menu(HL7, data);
    return a.exec();
}
void HL7_V2_Message_generator() {
    FILE* fptr;
    int month, day, hour, min, sec;
    //struct tm* localtime(const time_t * timep);
    time_t timep;
    struct tm* p;
    time(&timep);
    p = gmtime(&timep);
    month = 1 + p->tm_mon;
    day = p->tm_mday;
    hour = 8 + p->tm_hour;
    min = p->tm_min;
    sec = p->tm_sec;
    if (((fptr = fopen("HL7.hl7", "w+")) != NULL)) {
        fprintf(fptr, "MSH|^~\&|Microsoft Visual Studio|TestSendingSystem|||");
        fprintf(fptr, "%d", 1900 + p->tm_year);
        if (month < 10) fprintf(fptr, "0%d", month);
        else fprintf(fptr, "%d", month);
        if (day < 10) fprintf(fptr, "0%d", day);
        else fprintf(fptr, "%d", day);
        if (hour < 10) fprintf(fptr, "0%d", hour);
        else fprintf(fptr, "%d", hour);
        if (min < 10) fprintf(fptr, "0%d", min);
        else fprintf(fptr, "%d", min);
        if (min < 10) fprintf(fptr, "0%d", sec);
        else fprintf(fptr, "%d", sec);
        fprintf(fptr, "||ADT^A01||P|2.3\nPID|A123456789||||JONES^WILLIAM||1900/01/01|M\nAL1|||^PENICILLIN||PRODUCES HIVES\nDG1||I9|1550|DiagnosisDescription|19880501103005|F\nPV1|I|SICU^0001^01^GENHOSP|||6N^1234^A^GENHOSP|0200^JONES,GEORGE0148^ADDISON,JAMES||ICU||||||||0148^ANDERSON,CARL|S|1400|A|||||||||||||||||GENJOSP|||||199501102300\n");
        fclose(fptr);
    }
}
void fetch_origin_document(char HL7[]) {
    FILE* fptr;
    int i = 0;
    if ((fptr = fopen("HL7.hl7", "r")) != NULL)
    {
        while (!feof(fptr))
        {
            fscanf(fptr, "%c", &HL7[i]);
            i++;
        }
        printf("%s\n", HL7);
        fclose(fptr);
    }
    else
        printf("開檔失敗!\n");
}
void main_menu(char HL7[], char data[]) {
    int opt = 0;

    while (opt != -1) {
        printf("\n1)Patient PID Code 2)Allergy Code3)Diagnosis Discripttion 4)Export Full FHIR And Post to Server5)Read_CSv_File0)XML to Json-1)exit:");
        scanf("%d", &opt);
        switch (opt)
        {
        case 1:
            generate_XML_format(HL7, data, opt);
            break;
        case 2:
            generate_XML_format(HL7, data, opt);
            break;
        case 3:
            generate_XML_format(HL7, data, opt);
            break;
        case 4:
            export_full_json(HL7, data);
            break;
        case 5:
            read_csv_file_and_export_to_json();
            break;
        case 0:
            sub_menu(opt);
            break;
        case -1:
            break;
        default:
            printf("error , pls enter again!\n");
            break;
        }
    }
}
void sub_menu(int opt) {
    printf("\n1)PID to JSON\n2)AL1 to JSON\n3)DG1 to JSON\n0)Back to main menu\n option:");
    scanf("%d", &opt);
    switch (opt)
    {
    case 1:
        generate_JSON_format(opt);
        break;
    case 2:
        generate_JSON_format(opt);
        break;
    case 3:
        generate_JSON_format(opt);
        break;
    case 0:
        break;
    default:
        printf("error , pls enter again!\n");
        break;
    }
}
void analysis(char HL7[], char data[], int opt) {
    /*HL7[]儲存的是HL7 V2 Message,data[]是紀錄分析後的訊息,opt是看傳進來的值是需要轉換PID、AL1、DG1或是MSH*/
    int i;
    char* location;
    int segment;
    if (opt == 1) {
        if ((location = strstr(HL7, "PID")) != NULL) {
            i = 0;
            segment = 0;
            while (segment != 1) {
                while (*location == '|') {
                    location++;
                    segment++;
                }
                location++;
            }
            location -= 1;
            if (segment == 1) {
                while (*location != '|') {
                    data[i] = *location;
                    i++;
                    location++;
                }
                data[i] = '\0';
            }
            printf("\n%s", data);
        }
    }
    else if (opt == 2) {
        if ((location = strstr(HL7, "AL1")) != NULL) {
            i = 0;
            segment = 0;
            while (segment != 3) {
                while (*location == '|') {
                    location++;
                    segment++;
                }
                location++;
            }
            location -= 1;
            if (segment == 3) {
                while (*location != '|') {
                    while (!isalpha(*location) && !isdigit(*location))
                        location++;
                    data[i] = *location;
                    i++;
                    location++;
                }
                data[i] = '\0';
            }
            printf("\n%s", data);
        }
    }
    else if (opt == 3) {
        if ((location = strstr(HL7, "DG1")) != NULL) {
            i = 0;
            segment = 0;
            while (segment != 4) {
                while (*location == '|') {
                    segment++;
                    location++;
                }
                location++;
            }
            location -= 1;
            if (segment == 4) {
                while (*location != '|') {
                    data[i] = *location;
                    i++;
                    location++;
                }
                data[i] = '\0';
            }
            printf("\n%s", data);
        }
    }
    else if (opt == 4) {
        if ((location = strstr(HL7, "MSH")) != NULL) {
            i = 0;
            segment = 0;
            while (segment != 6) {
                while (*location == '|') {
                    segment++;
                    location++;
                }
                location++;
            }
            location -= 1;
            if (segment == 6) {
                while (i < 10) {
                    if (i == 4 || i == 7) {
                        data[i] = '-';
                        i += 1;
                    }
                    data[i] = *location;
                    i++;
                    location++;
                }
                data[i] = '\0';
            }
            printf("\n%s", data);
        }
    }

}
void generate_XML_format(char HL7[], char data[], int opt) {
    /*HL7[]儲存的是HL7 V2 Message,data[]是紀錄轉換成XML後的訊息,opt是看傳進來的值是需要轉換PID、AL1、DG1或是MSH*/
    FILE* fptr;
    if (opt == 1) {
        analysis(HL7, data, opt);
        if ((fptr = fopen("D:\\Qt project\\sample\\PID", "w+")) != NULL)
        {
            fprintf(fptr, "<HL7Message><%s><%s.0>%s</%s.0></%s></HL7Message>", data, data, data, data, data);
            fclose(fptr);
        }
    }
    else if (opt == 2) {
        analysis(HL7, data, opt);
        if ((fptr = fopen("D:\\Qt project\\sample\\AL1", "w+")) != NULL)
        {
            fprintf(fptr, "<HL7Message><%s><%s.0>%s</%s.0></%s></HL7Message>", data, data, data, data, data);
            fclose(fptr);
        }
    }
    else if (opt == 3) {
        analysis(HL7, data, opt);
        if ((fptr = fopen("D:\\Qt project\\sample\\DG1", "w+")) != NULL)
        {
            fprintf(fptr, "<HL7Message><%s><%s.0>%s</%s.0></%s></HL7Message>", data, data, data, data, data);
            fclose(fptr);
        }
    }
}
void generate_JSON_format(int opt) {
    /*HL7[]儲存的是HL7 V2 Message,data[]是紀錄轉換成JSON後的訊息,opt是看傳進來的值是需要轉換PID、AL1、DG1或是MSH*/
    FILE* fptr = NULL;
    int i = 0;
    char json[1000] = { "" };
    bool flag = false;
    if (opt == 1)
        fptr = fopen("D:\\Qt project\\sample\\PID", "r");
    else if (opt == 2)
        fptr = fopen("D:\\Qt project\\sample\\AL1", "r");
    else if (opt == 3)
        fptr = fopen("D:\\Qt project\\sample\\DG1", "r");
    if (fptr != NULL)
    {
        i = 0;
        while (!feof(fptr))
        {
            fscanf(fptr, "%c", &json[i]);
            i++;
        }
        json[i] = '\0';
        fclose(fptr);
    }
    else
        printf("查無此檔案，請先建XML檔在進來轉檔!\n");
    printf("%s", json);
    if (opt == 1)
        fptr = fopen("D:\\Qt project\\sample\\PID.json", "w+");
    else if (opt == 2)
        fptr = fopen("D:\\Qt project\\sample\\AL1.json", "w+");
    else if (opt == 3)
        fptr = fopen("D:\\Qt project\\sample\\DG1.json", "w+");
    if (fptr != NULL)
    {
        i = 0;
        while (json[i] != '\0') {

            if (isdigit(json[i]) || isalpha(json[i]) || json[i] == '.') {
                fprintf(fptr, "%c", json[i]);
                i++;
            }
            else if (json[i] == '<' && json[i + 1] != '/') {
                fprintf(fptr, "\n{\n\"", json[i]);
                i++;
            }
            else if (json[i] == '>') {
                fprintf(fptr, "\":", json[i]);
                if (json[i + 1] != '<') {
                    fprintf(fptr, "\"", json[i]);
                    while (json[i + 1] != '<') {
                        i++;
                        fprintf(fptr, "%c", json[i]);
                        flag = true;
                    }
                    if (flag == true)
                        fprintf(fptr, "\"", json[i]);
                }
                i++;
            }
            else if ((isdigit(json[i] || isalpha(json[i]))) && json[i + 1] == '<')
                fprintf(fptr, "\"", json[i]);
            else if (json[i] == '<' && json[i + 1] == '/') {
                while (json[i] != '>')
                    i++;
                fprintf(fptr, "\n}\n", json[i]);
                i++;
            }
            else
                i++;
        }
        fclose(fptr);
    }
}
void export_full_json(char HL7[], char data[])
{
    FILE* fptr;
    char json[10000] = { "" };
    string Json;
    QByteArray Data;
    static QString url="https://oauth.dicom.org.tw/fhir/AllergyIntolerance";
    //static QString url="http://192.168.50.5:10050/api/AllergyIntolerance";
    static QString urlput="https://oauth.dicom.org.tw/fhir/Patient/A123456789";
    //static QString urlput="http://192.168.50.5:10058/api/Patient";

    int i = 0;
    char* currentptr, * startptr;
    /***********先讀取範例檔案當作模板，之後再插入欲轉換的資料***********/
    if ((fptr = fopen("fullAL1info.json", "r+")) != NULL)
    {
        printf("\nGot it!\n");
        while (!feof(fptr))
        {
            fscanf(fptr, "%c", &json[i]);
            i++;
        }
        printf("%s",json);
        printf("\nEnd of Reading Loop");
        json[i] = '\0';
        fclose(fptr);
    }
    else
    {
        printf("Not found\n");
    }


    if ((fptr = fopen("allergyintolerance-medication.json", "w+")) != NULL)
    {
        printf("\nBuilding Json file");
        i = 0;
        currentptr = strstr(json, "http://www.nlm.nih.gov/research/umls/rxnorm");
        currentptr = strstr(currentptr, "code");
        currentptr += 8;
        while (&json[i] < currentptr) {
            fprintf(fptr, "%c", json[i]);
            i++;
        }
        fclose(fptr);
    }
    if ((fptr = fopen("allergyintolerance-medication.json", "a+")) != NULL)
    {
        i = 0;
        /**********snomed code**********/
        currentptr = strstr(json, "http://www.nlm.nih.gov/research/umls/rxnorm");
        currentptr = strstr(currentptr, "code");
        currentptr += 8;
        fprintf(fptr, "%s", "6369005\",\n\t\t\t\t");
        currentptr += sizeof("6369005\",\n\t\t\t\t");
        while (*currentptr != '"')
            currentptr++;
        startptr = currentptr;
        /**********插入藥物名稱**********/
        analysis(HL7, data, 2);
        currentptr = strstr(currentptr, "display");
        currentptr += 10;
        while (startptr <= currentptr) {
            fprintf(fptr, "%c", *startptr);
            startptr++;
        }
        fprintf(fptr, "%s", data);
        currentptr += sizeof(data);
        while (*currentptr != '"')
            currentptr++;
        startptr = currentptr;
        /**********插入PID**********/
        analysis(HL7, data, 1);
        currentptr = strstr(currentptr, "patient");
        currentptr = strstr(currentptr, "reference");
        currentptr += 12;
        fprintf(fptr,"Patient/");
        currentptr+=8;
        while (startptr <= currentptr) {
            fprintf(fptr, "%c", *startptr);
            startptr++;
        }
        fprintf(fptr, "%s", data);
        currentptr += sizeof(data);
        while (*currentptr != '"')
            currentptr++;
        startptr = currentptr;
        /**********插入日期**********/
        analysis(HL7, data, 4);
        currentptr = strstr(currentptr, "recordedDate");
        //currentptr = strstr(currentptr, "reference");
        currentptr += 15;
        while (startptr <= currentptr) {
            fprintf(fptr, "%c", *startptr);
            startptr++;
        }
        fprintf(fptr, "%s", data);
        currentptr += sizeof(data);
        while (*currentptr != '"')
            currentptr++;
        startptr = currentptr;
        /**********插入DG1**********/
        /*analysis(HL7, data, 3);
        currentptr = strstr(currentptr, "recorder");
        currentptr = strstr(currentptr, "reference");
        currentptr += 12;
        while (startptr <= currentptr) {
            fprintf(fptr, "%c", *startptr);
            startptr++;
        }
        fprintf(fptr, "%s", data);
        currentptr += sizeof(data);
        while (*currentptr != '"')
            currentptr++;
        startptr = currentptr;*/
        /*********************************/
        fprintf(fptr, "%s", currentptr);
        fclose(fptr);
        printf("\n\n\n");
        /*
        if ((fptr = fopen("D:\\Qt project\\sample\\allergyintolerance-medication.json", "r")) != NULL)
        {
            printf("\nTransfering");
            Net2 transfer;
            fread(&Data,sizeof(QString),5000,fptr);
            transfer.PostData(url,Data);
            fclose(fptr);
        }*/


        QFile putfile("Patient.json");//Put patient的資料
        if(putfile.open(QFile::ReadOnly)){
            QByteArray data=putfile.readAll();
            Net2 transfer;
            transfer.PutData(urlput,data);
        }
        QFile putpratitioner("Practitioner.json");//Put Practioner
        if(putpratitioner.open(QFile::ReadOnly)){
            QByteArray data=putfile.readAll();
            Net2 transfer;
            transfer.PutData(urlput,data);
        }
        QFile doc("allergyintolerance-medication.json");
        if(doc.open(QFile::ReadOnly)){
            QByteArray data=doc.readAll();
            Net2 transfer;
            transfer.PostData(url,data);
        }
    }
}
void read_csv_file_and_export_to_json() {
    FILE* fptr;
    struct file data[25] = { "","" };
    char* token = NULL;
    int i, j;
    char line[300] = { " " };
    list<pair<int, string>>c;
    list<string> f;
    if ((fptr = fopen("D:\\Qt project\\sample\\allergycsv.csv", "r")) != NULL) {
        i = 0;
        while (fgets(line, sizeof(line), fptr)) {
            token = strtok(line, ",");
            while (token != NULL) {
                c.push_back(make_pair(i, token));
                token = strtok(NULL, ",");
                i++;
            }
            printf("\n");
        }
        fclose(fptr);
    }
    list<pair<int, string>>::iterator it = c.begin();
    list<string>::iterator skr = f.begin();
    i = 0;
    j = 0;
    for (it = c.begin(); it != c.end(); it++) {
        printf("%d %s\n", it->first, it->second.c_str());
        if (it->first < 23) {

            //big5->unicode
            char stupidcrap[50] = { "" };
            char* szData = NULL, * sendbuf_utf8 = NULL;
            wchar_t* sendbuf_Unicode = NULL;
            strcpy(stupidcrap, it->second.c_str());
            int nDataLen = MultiByteToWideChar(CP_ACP, 0, szData, -1, NULL, 0);
            sendbuf_Unicode = new wchar_t[nDataLen + 1];
            MultiByteToWideChar(CP_ACP, 0, szData, -1, sendbuf_Unicode, nDataLen);
            //delete new buffer
            delete[]sendbuf_utf8; sendbuf_utf8 = NULL;
            delete[]sendbuf_Unicode; sendbuf_Unicode = NULL;

            //data[i].title = it->second.c_str();
            data[i].title = stupidcrap;
            printf("transformed:%s\n", stupidcrap);
            i++;
        }
        if (it->first >= 23) {
            //big5->unicode
            char stupidcrap[50] = { "" };
            char* szData = NULL, * sendbuf_utf8 = NULL;
            wchar_t* sendbuf_Unicode = NULL;
            strcpy(stupidcrap, it->second.c_str());
            szData = stupidcrap;
            int nDataLen = MultiByteToWideChar(CP_ACP, 0, szData, -1, NULL, 0);
            sendbuf_Unicode = new wchar_t[nDataLen + 1];
            MultiByteToWideChar(CP_ACP, 0, szData, -1, sendbuf_Unicode, nDataLen);

            //delete new buffer
            delete[]sendbuf_utf8; sendbuf_utf8 = NULL;
            delete[]sendbuf_Unicode; sendbuf_Unicode = NULL;
            data[j].detail = stupidcrap;
            printf("transformed:%s\n", stupidcrap);
            j++;


        }

    }
    for (i = 0; i < 23; i++) {
        cout << data[i].title << "," << data[i].detail << endl;
    }
    i = 0;
    fstream myFile;
    myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::out | ios::trunc);
    cout << "{\n\"resourceType\": \"AllergyIntolerance\",\n" << endl;
    myFile << "{\n\"resourceType\": \"AllergyIntolerance\",\n" << endl;
    myFile.close();
    for (i = 0; i < 23; i++) {
        if (data[i].detail == "NULL") {
            i++;
        }

        if (data[i].title.compare("identifier") == 0) {
            myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::app);
            // Write to the file
            //printf("\n\ntitle:%sdetail:%S\n\n", data[i].title, data[i].detail);
            cout << "\"" + data[i].title + "\":\"" + data[i].detail + "\"," << endl;
            myFile << "\"" + data[i].title + "\":\"" + data[i].detail + "\"," << endl;
            // Close file
            myFile.close();
            i++;
        }


        if (data[i].title.compare("clinicalStatus") == 0) {
            myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::app);
            // Write to the file
            cout << "\"" + data[i].title + "\": {\n\"coding\": [\n{\n\"system\": \"http://terminology.hl7.org/CodeSystem/allergyintolerance-clinical\",\n\"code\" : \"" + data[i].detail + "\",\n\"display\" : \"" + data[i].detail + "\"\n}\n]\n}, " << endl;
            myFile << "\"" + data[i].title + "\": {\n\"coding\": [\n{\n\"system\": \"http://terminology.hl7.org/CodeSystem/allergyintolerance-clinical\",\n\"code\" : \"" + data[i].detail + "\",\n\"display\" : \"" + data[i].detail + "\"\n}\n]\n}, " << endl;
            // Close file
            i++;
            myFile.close();
        }
        if (data[i].title.compare("verificationStatus") == 0) {
            myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::app);
            // Write to the file
            cout << "\"" + data[i].title + "\": {\n\"coding\": [\n{\n\"system\": \"http://terminology.hl7.org/CodeSystem/allergyintolerance-clinical\",\n\"code\" : \"" + data[i].detail + "\",\n\"display\" : \"" + data[i].detail + "\"\n}\n]\n}, " << endl;
            myFile << "\"" + data[i].title + "\": {\n\"coding\": [\n{\n\"system\": \"http://terminology.hl7.org/CodeSystem/allergyintolerance-verification\",\n\"code\" : \"" + data[i].detail + "\",\n\"display\" : \"" + data[i].detail + "\"\n}\n]\n}," << endl;
            // Close file
            myFile.close();
            i++;
        }
        if (data[i].title.compare("category") == 0) {
            myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::app);
            // Write to the file
            cout << "\"" + data[i].title + "\": [\n\"" + data[i].detail + "\"\n] ," << endl;
            myFile << "\"" + data[i].title + "\": [\n\"" + data[i].detail + "\"\n], " << endl;
            // Close file
            i++;
            myFile.close();
        }

        if (data[i].title.compare("criticality") == 0) {
            myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::app);
            // Write to the file
            cout << "\"" + data[i].title + "\": \"" + data[i].detail + "\"," << endl;
            myFile << "\"" + data[i].title + "\": \"" + data[i].detail + "\"," << endl;
            // Close file
            i++;
            myFile.close();
        }

        if (data[i].title.compare("code") == 0) {
            myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::app);
            // Write to the file
            cout << "\"code\": {\n\"coding\": [\n{\n\"system\": \"http://www.nlm.nih.gov/research/umls/rxnorm\",\n\"" + data[i].title + "\" : \"" + data[i].detail + "\",\n\"display\" : \"PENICILLIN\"\n}\n]\n}, " << endl;
            myFile << "\"code\": {\n\"coding\": [\n{\n\"system\": \"http://www.nlm.nih.gov/research/umls/rxnorm\",\n\"" + data[i].title + "\" : \"" + data[i].detail + "\",\n\"display\" : \"PENICILLIN\"\n}\n]\n}, " << endl;
            // Close file
            i++;
            myFile.close();
        }

        if (data[i].title.compare("patient") == 0) {
            myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::app);
            // Write to the file
            cout << "\"" + data[i].title + "\": {\n\"reference\": \"" + data[i].detail + "\"\n}, " << endl;
            myFile << "\"" + data[i].title + "\": {\n\"reference\": \"Patient/" + data[i].detail + "\"\n}, " << endl;
            // Close file
            i++;
            myFile.close();
        }

        if (data[i].title.compare("recordedDate") == 0) {
            myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::app);
            // Write to the file
            //cout << "\"" + data[i].title + "\": \"" + data[i].detail + "\", " << endl;
            //myFile << "\"" + data[i].title + "\": \"" + data[i].detail + "\", " << endl;
            // Close file
            i++;
            myFile.close();
        }

        if (data[i].title.compare("recorder") == 0) {
            myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::app);
            // Write to the file
            cout << "\"" + data[i].title + "\": {\n\"reference\": \"" + data[i].detail + "\"\n}," << endl;
            myFile << "\"" + data[i].title + "\": {\n\"reference\": \"" + data[i].detail + "\"\n}," << endl;
            // Close file
            i++;
            myFile.close();
        }

        if (data[i].title.compare("manifestation") == 0) {
            myFile.open("D:\\Qt project\\sample\\csv_to_json.json", ios::app);
            // Write to the file
            cout << "\"reaction\": [\n{\n\"" + data[i].title + "\": [\n{\n\"coding\": [\n{\n\"system\": \"http://snomed.info/sct\",\n\"code\" : \"247472004\",\n\"display\" : \"" + data[i].detail + "\"\n}\n]\n}\n]\n}\n]\n}" << endl;
            myFile << "\"reaction\": [{\n\"" + data[i].title + "\": [\n{\n\"coding\": [\n{\n\"system\": \"http://snomed.info/sct\",\n\"code\" : \"247472004\",\n\"display\" : \"" + data[i].detail + "\"\n}\n]\n}\n]\n}\n]\n}" << endl;
            // Close file
            i++;
            myFile.close();
        }
        i++;
    }
}
