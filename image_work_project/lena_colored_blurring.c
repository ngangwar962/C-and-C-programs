#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
int main()
{
int i,j,k,l;
FILE *fin,*fout;
fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena.bmp","r");
fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_bw_blur","w+");
unsigned char byte[54];
if(fin==NULL)
{
printf("file not found\n");
}
for(i=0;i<54;i++)
{
byte[i]=getc(fin);
}
fwrite(byte,sizeof(unsigned char),54,fout);

int height=*(int*)&byte[18];
int width=*(int*)&byte[22];
int bitdepth=*(int*)&byte[28];

unsigned char colortable[1024];

int size=height*width;

unsigned char buffer[size];
unsigned char out[size];


if(bitdepth<=8)
{
fread(colortable,sizeof(unsigned char),1024,fin);
fwrite(colortable,sizeof(unsigned char),1024,fout);
}

fread(buffer,sizeof(unsigned char),size,fin);

float a =1.0/9;

for(i=0;i<size;i++)
{
out[i]=buffer[i];
}
float filter[3][3]={{a,a,a},{a,a,a},{a,a,a}};
for(i=1;i<height-1;i++)
{
for(j=1;j<width-1;j++)
   {
	float sum=0.0;
	for(k=-1;k<=1;k++)
	{
		for(l=-1;l<=1;l++)
		{
			sum=sum+(float)filter[k+1][l+1]*buffer[(i+k)*width+(j+l)];	
		}
		
	}
	out[(i)*width+(j)]=sum;
    }
}
int count=0;
float sumf=0;
for(i=0;i<size;i++)
{
if(buffer[i]!=out[i])
{
count++;
sumf=sumf+out[i];
//out[i]=buffer[i];
}
}
int average=(float)sumf/count;
printf("average=%d\n",average);
/*for(i=0;i<size;i++)
{
if(buffer[i]!=out[i])
{
	out[i]=average;
}
}*/
fwrite(out,sizeof(unsigned char),size,fout);
fclose(fin);
fclose(fout);
return 0;
}
