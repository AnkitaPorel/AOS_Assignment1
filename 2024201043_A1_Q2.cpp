#include<iostream>
#include<cstdio>
#include<cstring>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<cerrno>
#include<string>

#define CHUNK_SIZE 4096

using namespace std;

void checkPermission(char *file_path,const char *type)
{
    struct stat st;
    if(stat(file_path,&st)<0)
    {
        cerr<<"\nInvalid file\n";
        exit(EXIT_FAILURE);
    }
    if(st.st_mode & S_IRUSR)
        printf("\nUser has read permissions on %s: Yes\n",type);
    else
        printf("\nUser has read permissions on %s: No\n",type);
    if(st.st_mode & S_IWUSR)
        printf("\nUser has write permissions on %s: Yes\n",type);
    else
        printf("\nUser has write permissions on %s: No\n",type);
    if(st.st_mode & S_IXUSR)
        printf("\nUser has execute permissions on %s: Yes\n",type);
    else
        printf("\nUser has execute permissions on %s: No\n",type);
    if(st.st_mode & S_IRGRP)
        printf("\nGroup has read permissions on %s: Yes\n",type);
    else
        printf("\nGroup has read permissions on %s: No\n",type);
    if(st.st_mode & S_IWGRP)
        printf("\nGroup has write permissions on %s: Yes\n",type);
    else
        printf("\nGroup has write permissions on %s: No\n",type);
    if(st.st_mode & S_IXGRP)
        printf("\nGroup has execute permissions on %s: Yes\n",type);
    else
        printf("\nGroup has execute permissions on %s: No\n",type);
    if(st.st_mode & S_IROTH)
        printf("\nOthers have read permissions on %s: Yes\n",type);
    else
        printf("\nOthers have read permissions on %s: No\n",type);
    if(st.st_mode & S_IWOTH)
        printf("\nOthers have write permissions on %s: Yes\n",type);
    else
        printf("\nOthers have write permissions on %s: No\n",type);
    if(st.st_mode & S_IXOTH)
        printf("\nOthers have execute permissions on %s: Yes\n",type);
    else
        printf("\nOthers have execute permissions on %s: No\n",type);
}

bool isReverse(int new_fd,int old_fd,off_t len)
{
    char *buff1=new char[CHUNK_SIZE];
    char *buff2=new char[CHUNK_SIZE];
    off_t offset=len-1,pos1=0;
    char last_char;
    lseek(old_fd, offset, SEEK_SET);
    read(old_fd, &last_char, 1);
    if (last_char == '\n')
        offset--;       //since I removed the newline character in input file while reversing
	ssize_t size=0;
	while(offset>=0)
	{
        size=(offset<CHUNK_SIZE)?offset+1:CHUNK_SIZE;   //for edge cases
        lseek(new_fd,pos1,SEEK_SET);
        lseek(old_fd,offset-size+1,SEEK_SET);
        read(new_fd,buff1,size);
        read(old_fd,buff2,size);
        int begin=0,end=size-1;
		while(begin<end)
		{
			swap(buff1[begin],buff1[end]);
			begin++;
			end--;
		}
        if(strcmp(buff1,buff2)!=0)
            return false;
        pos1+=size;
        offset-=size;
    }
    return true;
}

int main(int argc,char** argv)
{
    if(argc!=4)
    {
        cerr<<"\nInvalid number of arguments!!!\n";
		exit(EXIT_FAILURE);
    }
    char *newfile_path=argv[1],*oldfile_path=argv[2],*directory_path=argv[3];
    struct stat dir_st;
    int new_fd,old_fd;
    off_t file1_size,file2_size;
    if(newfile_path==NULL || oldfile_path==NULL || newfile_path==NULL)
    {
        cerr<<"\nInvalid arguments\n";
        exit(EXIT_FAILURE);
    }
    if(stat(directory_path,&dir_st)==0 && S_ISDIR(dir_st.st_mode))
    {
        printf("\nDirectory is created: Yes\n");
    }
    else
        printf("\nDirectory is not created\n");
    new_fd=open(newfile_path,O_RDONLY);
    if(new_fd<0)
    {
        cerr<<"\nFailed to open new file\n";
        exit(EXIT_FAILURE);
    }
    old_fd=open(oldfile_path,O_RDONLY);
    if(old_fd<0)
    {
        cerr<<"\nFailed to open old file\n";
        exit(EXIT_FAILURE);
    }
    char ch;
    file1_size=lseek(new_fd,0,SEEK_END);
    file2_size=lseek(old_fd,0,SEEK_END);
    lseek(old_fd,file2_size-1,SEEK_SET);
    read(old_fd,&ch,1);
    if (ch == '\n')
        file2_size-=1;      //because I removed the newline character in input file while reversing
    if(file1_size==file2_size)
        printf("\nBoth file's sizes are same: Yes\n");
    else
        printf("\nBoth file's sizes are same: No\n");
    if(file1_size==file2_size && isReverse(new_fd,old_fd,file1_size))
        printf("\nWhether file contents are reversed in new file: Yes\n");
    else
        printf("\nWhether file contents are reversed in new file: No\n");
    
    checkPermission(newfile_path,"New File");
    checkPermission(oldfile_path,"Old file");
    checkPermission(directory_path,"Directory");
    if(close(new_fd)<0)
	{
		cerr<<"\nFile closing error\n";
		exit(EXIT_FAILURE);
	}
	if(close(old_fd)<0)
	{
		cerr<<"\nFile closing error\n";
                exit(EXIT_FAILURE);
	}
    return 0;
}