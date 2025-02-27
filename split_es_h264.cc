#include "split_es_h264.h"

//callback data from sdk
//is split backage
//
//




split_ps::split_ps(int size)
{
    this->h264_start_flag = 0;
    this->es_sum=0;
    this->h264_length_now=0;
    this->h264_complete.reserve(size);
    this->count_Es=0;
    this->count_PS_head=0;
}

split_ps::~split_ps()
{
    //
    
    std::vector <unsigned char>().swap(this->h264_complete);
    printf("clean vector space\n");
}




void split_ps::split_h264formES2(unsigned char *databuffer,int size,void(*callback_fun)(unsigned char * data,int size))
{
//
//printf("1:%x 2:%x 3:%x 4:%x \n",databuffer[0],databuffer[1],databuffer[2],databuffer[3]);



int skip_num = databuffer[2];
int single_h264_len = size-(3+skip_num);
unsigned char * temp_pointer = databuffer+3+skip_num;
//printf("1:%x 2:%x 3:%x 4:%x \n",temp_pointer[0],temp_pointer[1],temp_pointer[2],temp_pointer[3]);

if(temp_pointer[0]==0x00 && temp_pointer[1]==0x00 && temp_pointer[2]==0x00 && temp_pointer[3]==0x01 && h264_start_flag==0)//first start
{
    h264_start_flag=1;
    for(int ti=0;ti<single_h264_len;ti++)
    {
        h264_complete.emplace_back(temp_pointer[ti]);
    }


    
    
    es_sum++;
    h264_length_now+=single_h264_len;
}
else if(temp_pointer[0]!=0x00 || temp_pointer[1]!=0x00 || temp_pointer[2]!=0x00 || temp_pointer[3]!=0x01 && h264_start_flag==1)//previous backage
{   
    for(int ti=0;ti<single_h264_len;ti++)
    {
        h264_complete.emplace_back(temp_pointer[ti]);
    }



    

    
    h264_length_now+=single_h264_len;
    
}
else if(temp_pointer[0]==0x00 && temp_pointer[1]==0x00 && temp_pointer[2]==0x00 && temp_pointer[3]==0x01 && h264_start_flag==1)//new start 
{   


    callback_fun(&h264_complete[0],h264_complete.size());

    h264_complete.clear();
   
    for(int ti=0;ti<single_h264_len;ti++)
    {
        h264_complete.emplace_back(temp_pointer[ti]);
    }

    


    es_sum++;
    h264_length_now+=single_h264_len;
}
//
}









void split_ps::getES(unsigned char * databuffer,int size_)
{
//
unsigned char * es_nake_data = nullptr;
if(databuffer[0]==0x00 && databuffer[1]==0x00 && databuffer[2]==0x01 && databuffer[3]==0xe0)
{   
    
    int es_backage_length = (databuffer[4]<<8) | databuffer[5];
    es_nake_data = new unsigned char[es_backage_length];
    
    if (size_<es_backage_length+6)
    {
        //in other backage
        printf("the length is not right!!!");
    }
    else
    {   
        
        memcpy(es_nake_data,databuffer+6,sizeof(unsigned char)*es_backage_length);
        split_h264formES2(es_nake_data,es_backage_length,callback);

    }
    delete[] es_nake_data;
    count_Es++;


}
else if(databuffer[0]==0x00 && databuffer[1]==0x00 && databuffer[2]==0x01 && databuffer[3]==0xba)
{
    count_PS_head++;
    //get es stream find 00 00 01 e0
    //
    unsigned char * temp_p=databuffer;
    for(int index=0;index<size_-4;index++)
    {   
        if(temp_p[0]==0x00 && temp_p[1]==0x00 && temp_p[2]==0x01 && temp_p[3]==0xe0)
        {
            int es_backage_length=(temp_p[4] <<8 )| temp_p[5];
            split_h264formES2(temp_p+6,es_backage_length,callback);
        }
        temp_p++;
    }
}
//
}

void split_ps::set_callback(void(*callback_)(unsigned char*,int))
{
    callback = callback_;
}