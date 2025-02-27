#include "split_es_h264.h"

int main()
{
std::string path_folder = "./ps_packets/"; 
std::string file_last = ".ps";
int nums = 17378;

h264_complete.reserve(1024*1024*4);

auto start_time = std::chrono::high_resolution_clock::now();

split_ps te(1024*1024*4);
te.set_callback(callback_save);


for(int i=0;i<nums;i++)
{
    std::string all_file_path = path_folder+std::to_string(i)+file_last;
    //std::cout<<"all_file_path :"<<all_file_path<<std::endl;

    FILE * fp = fopen(all_file_path.c_str(), "rb");
    

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);//get size files
    fseek(fp, 0, SEEK_SET);
    //there is read from local files

    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * fileSize); //

    size_t readSize = fread(buffer, fileSize, 1, fp);//read all in 

    if (readSize != 1)
    {
        printf("%s(%d): Error: read_size=%d != 1\n", __FUNCTION__, __LINE__, readSize);
        fclose(fp);
        free(buffer);
    }
    

    //std::cout<<" "<<int(buffer[0])<<" "<<int(buffer[1])<<" "<<int(buffer[2])<<" "<<int(buffer[3])<<std::endl;
    //printf("1:%x 2:%x 3:%x 4:%x\n",buffer[0],buffer[1],buffer[2],buffer[3]);

  
    
    te.getES(buffer,fileSize);

    free(buffer);

    fclose(fp);

}
auto end_time = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> duration = end_time - start_time;
printf("execution time: %f seconds\n", duration.count());

}
