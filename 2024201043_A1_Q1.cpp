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

void print_progress(off_t written,off_t ip_size)
{
	double percentage=(double)written/ip_size*100;
	fflush(stdout);
	printf("\rProgress: %.0lf %%",percentage);
}

void reverse_full(int ip_fd,int op_fd)
{
	off_t ip_size=lseek(ip_fd,0,SEEK_END);
	if(ip_size<=0)
	{
		cerr<<"\nInput file is empty\n";
		exit(EXIT_FAILURE);
	}
	char *buff=new char[CHUNK_SIZE];
	off_t offset=ip_size-1,written=0;
	char last_char;
    lseek(ip_fd, offset, SEEK_SET);
    read(ip_fd, &last_char, 1);
    if (last_char == '\n')
        offset--;	//done to avoid copying new line characters at end of text files
	ssize_t size=0;
	while(offset>=0)
	{
		size=(offset<CHUNK_SIZE)?offset+1:CHUNK_SIZE;	//for edge cases
		lseek(ip_fd,offset-size+1,SEEK_SET);
		read(ip_fd,buff,size);
		int begin=0,end=size-1;
		while(begin<end)
		{
			swap(buff[begin],buff[end]);
			begin++;
			end--;
		}
		write(op_fd,buff,size);
		written+=size;
		offset-=size;
		print_progress(written,ip_size);
	}
	print_progress(ip_size,ip_size);	//because newline character at end of input file was removed while reversing
}

void reverse_part(int ip_fd,int op_fd,off_t st,off_t en)
{
	off_t ip_size=lseek(ip_fd,0,SEEK_END);
	if(ip_size<=0 || st<0 || en<0 || st>ip_size-1 || en>ip_size-1)
	{
		cerr<<"\nInput file is empty\n";
		exit(EXIT_FAILURE);
	}
	char *buff=new char[CHUNK_SIZE];
	char last_char;
	ssize_t size=0;
	off_t offset1=st-1,offset2=en-st+1,offset3=ip_size-en-1,written=0;
	if(st==ip_size-1)
	{
		lseek(ip_fd,offset1,SEEK_SET);
		read(ip_fd, &last_char, 1);
		if (last_char == '\n')
			offset1--;		//done to avoid copying new line characters at end of text files
	}
	while(offset1>=0)	//code portion for reversing from index 0 to st-1 in text file
	{
		size=(offset1<CHUNK_SIZE)?offset1+1:CHUNK_SIZE;		//for edge cases
		lseek(ip_fd,offset1-size+1,SEEK_SET);
		read(ip_fd,buff,size);
		int begin=0,end=size-1;
		while(begin<end)
		{
			swap(buff[begin],buff[end]);
			begin++;
			end--;
		}
		write(op_fd,buff,size);
		offset1-=size;
		written+=size;
		print_progress(written,ip_size);
	}
	off_t pos=st;
	if(en==ip_size-1)
	{
		lseek(ip_fd,en-1,SEEK_SET);
		read(ip_fd, &last_char, 1);
		if (last_char == '\n')
			offset2--;		//done to avoid copying new line characters at end of text files
	}
	while(offset2>0)	//code portion for copying the part from index st to en
	{
		size=(offset2<CHUNK_SIZE)?offset2:CHUNK_SIZE;	//for edge cases
		lseek(ip_fd,pos,SEEK_SET);
		read(ip_fd,buff,size);
		write(op_fd,buff,size);
		offset2-=size;
		pos+=size;
		written+=size;
		print_progress(written,ip_size);
	}
	pos=en+1;
	lseek(ip_fd,ip_size-1,SEEK_SET);
	read(ip_fd, &last_char, 1);
	if (last_char == '\n')
		--offset3;		//done to avoid copying new line characters at end of text files
	while(offset3>0)	//code portion for reversing from index en+1 to the last byte
	{
		size=(offset3<CHUNK_SIZE)?offset3:CHUNK_SIZE;	//for edge cases
		lseek(ip_fd,pos+offset3-size,SEEK_SET);
		read(ip_fd,buff,size);
		int begin=0,end=size-1;
		while(begin<end)
		{
			swap(buff[begin],buff[end]);
			begin++;
			end--;
		}
		write(op_fd,buff,size);
		offset3-=size;
		written+=size;
		print_progress(written,ip_size);	
	}
	print_progress(ip_size,ip_size);	//because newline character at end of input file was removed while reversing
}

int main(int argc, char** argv)
{
	if(argc<3)
	{
		cerr<<"\nToo few arguments!!!\n";
		exit(EXIT_FAILURE);
	}
	char *ip_file=argv[1];
	if(ip_file==NULL)
	{
		cerr<<"\nInvalid argument\n";
		exit(EXIT_FAILURE);
	}
	char *file_name=strrchr(ip_file,'/');
	if(file_name)
		file_name++;
	else
		file_name=ip_file;
	const char *op_base="Assignment1/";
	int ip_fd=open(ip_file,O_RDONLY);
	if(ip_fd<0)
	{
		cerr<<"\nInput file does not exist\n";
		exit(EXIT_FAILURE);
	}
	struct stat st;
	fstat(ip_fd,&st);
	off_t ip_size=st.st_size;
	char* flag=argv[2];
	int size=strlen(argv[2]);
	if(size>1 || size==0)	//since the flag can be of one digit only
	{
		cerr<<"\nInvalid Flag\n";
		exit(EXIT_FAILURE);
	}
	if(!isdigit(*flag))
	{
		cerr<<"\nInvalid flag\n";
		exit(EXIT_FAILURE);
	}
	int fl=atoi(flag);
	if(fl>1)
	{
		cerr<<"\nInvalid mode\n";
		exit(EXIT_FAILURE);
	}
	if(fl==1 && argc!=5)
	{
		cerr<<"\nInvalid mode\n";
        exit(EXIT_FAILURE);
	}
	if(fl==0 && argc!=3)
	{
		cerr<<"\nInvalid mode\n";
        exit(EXIT_FAILURE);
	}
	const char *num=(fl==0)? "0_":"1_";
	size=strlen(op_base)+strlen(num)+strlen(ip_file);
	char* op_file=new char[size+1];
	strcat(op_file,op_base);
	strcat(op_file,num);
	strcat(op_file,file_name);		//concatenating char arrays to get the desired output file name
	if(mkdir("Assignment1",0700)<0 && errno!=EEXIST)
	{
		cerr<<"\nFile directory already exist\n";
		exit(EXIT_FAILURE);
	}
	int op_fd=open(op_file,O_RDWR | O_CREAT | O_TRUNC,0600);
	if(op_fd<0)
	{
		cerr<<"\nFail to create output file\n";
		exit(EXIT_FAILURE);
	}
	if(fl==0)
	{
		reverse_full(ip_fd,op_fd);
	}
	else 
	{
		char* st=argv[3];
		char* en=argv[4];
		if(!isdigit(*st))
		{
			cerr<<"\nWrong argument\n";
			exit(EXIT_FAILURE);
		}
		while(*st)
		{
			if(!isdigit(*st))
			{
				cerr<<"\nInvalid argument\n";
				exit(EXIT_FAILURE);
			}
			st++;
		}
		off_t s=atoll(argv[3]);
		if(!isdigit(*en))
        {
            cerr<<"\nInvalid argument\n";
            exit(EXIT_FAILURE);
        }
		while(*en)
        {
			if(!isdigit(*en))
			{
				cerr<<"\nInvalid argument\n";
				exit(EXIT_FAILURE);
			}
			en++;
		}
		off_t e=atoll(argv[4]);
		if(s>e)
		{
			cerr<<"\nInvalid argument\n";
            exit(EXIT_FAILURE);
		}
		else
		{
			reverse_part(ip_fd,op_fd,s,e);
		}
	}
	if(close(ip_fd)<0)
	{
		cerr<<"\nFile closing error\n";
		exit(EXIT_FAILURE);
	}
	if(close(op_fd)<0)
	{
		cerr<<"\nFile closing error\n";
                exit(EXIT_FAILURE);
	}
	cout<<"\nFile is reversed\n"<<endl;	
	return 0;
}