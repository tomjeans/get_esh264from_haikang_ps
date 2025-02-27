#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <chrono>



class split_ps
{
    public:
        split_ps(int size);
        ~split_ps();
        void getES(unsigned char * databuffer,int size_);
        void set_callback(void(*callback_)(unsigned char*,int));
        int h264_start_flag;
        int es_sum;
        int h264_length_now;
        std::vector<unsigned char> h264_complete;
        void(*callback)(unsigned char*,int)=nullptr; 
        int count_Es;
        int count_PS_head;
    private:
        void split_h264formES2(unsigned char *databuffer,int size,void(*callback_fun)(unsigned char * data,int size));

};