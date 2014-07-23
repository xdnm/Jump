#include "FileHelper.h"

FileHelper::FileHelper()
{

}

FileHelper::~FileHelper()
{

}

FileHelper* FileHelper::Instance()
{
    static FileHelper* instance = new FileHelper();

    return instance;
}

bool FileHelper::isExist(const char *filename)
{
    if( !filename ) return false;

    //strFilePathName is :/data/data/ + package name
    std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    filePath += filename;

    FILE *fp = fopen(filePath.c_str(),"r");
    if(fp)
    {
        fclose(fp);
        return true;
    }
    return false;
}

void FileHelper::copyFile(const char *filename)
{
    std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
    unsigned long len = 0;
    unsigned char *data = NULL;

    if(isExist(filename) == false)
    {
        CCLog("File %s does not exsit.", filename);
    }

    data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(),"r",&len);
    if(len == 0)
    {
        CCLog("File %s does not exsit!", filename);
    }

    std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    destPath += filename;
    CCLog("copy file to %s", destPath.c_str());
    FILE *fp = fopen(destPath.c_str(),"w+");
    fwrite(data,sizeof(char),len,fp);
    fclose(fp);
    delete []data;
    data = NULL;
}

int FileHelper::getRecord(char *out_str)
{
    string fileName = "Record.xml";

    string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();

    string fullFileName = filePath.append(fileName);

    if(!isExist(fileName.c_str()))
    {
        xml_document<> doc;    
        xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));  
        doc.append_node(rot);  
        xml_node<>* node =   doc.allocate_node(node_element,"record","0");    
        doc.append_node(node);

        std::ofstream out(fullFileName.c_str());  
        out << doc;
    }

    file<> fdoc(fullFileName.c_str());  
    std::cout<<fdoc.data()<<std::endl;  
    xml_document<>   doc;  
    doc.parse<0>(fdoc.data());  

    //std::cout<<doc.name()<<std::endl;  

    //! 获取根节点  
    xml_node<>* root = doc.first_node("record");  
    char *ch_record = root->value();
    
    strcpy(out_str, ch_record);
    
    std::string text;  
    rapidxml::print(std::back_inserter(text),doc,0);  
    CCLog("%s", text.c_str());

    return 0;
}

bool FileHelper::writeRecord(int score)
{
    string fileName = "Record.xml";

    string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();

    string fullFileName = filePath.append(fileName);

    if(!isExist(fileName.c_str()))
    {
        xml_document<> doc;    
        xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));  
        doc.append_node(rot);  
        xml_node<>* node =   doc.allocate_node(node_element,"record","0");    
        doc.append_node(node);

        std::ofstream out(fullFileName.c_str());  
        out << doc;
    }

    file<> fdoc(fullFileName.c_str());  
    std::cout<<fdoc.data()<<std::endl;  
    xml_document<>   doc;  
    doc.parse<0>(fdoc.data());  

    xml_node<> *scoreNode = doc.first_node("record");
    if(scoreNode)
    {
        doc.remove_node(scoreNode);
    }

    char tempStr[20];
    sprintf(tempStr, "%d", score);
    xml_node<> *newScoreNode = doc.allocate_node(node_element,"record",tempStr);  

    doc.append_node(newScoreNode);

    std::string text;  
    rapidxml::print(std::back_inserter(text),doc,0);  
    CCLog("%s", text.c_str());

    std::ofstream out(fullFileName.c_str());  
    out << doc;  
    
    return true;
}
