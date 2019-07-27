#include<stdio.h>
#define BRIGHTNESS 15
#define MAX_COLOR 255
int main()
{
	int i,j,k,l,ch;
	unsigned char byte[54],colorTable[1024];
	FILE *fin,*fout;
	fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena.bmp","r");
	fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_rotate_bright.bmp","w+");
	if(fin==NULL)
	{
		printf("file not found at specified position\n");
	}
	for(i=0;i<54;i++)										//read the 54 byte header from fIn
	{
		byte[i] = getc(fin);
	}
	fwrite(byte,sizeof(unsigned char),54,fout);	
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];
	
	printf("width: %d\n",width);
	printf("height: %d\n",height );
	printf("bitdepth: %d\n",bitDepth);

	int size = height*width;
	
	if(bitDepth <= 8)										//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fin);
		fwrite(colorTable,sizeof(unsigned char),1024,fout);
	}

	unsigned char buffer[width][height];					//to store the image data
	unsigned char out_buffer[width][height];
	unsigned char out_buffer1[width][height];

	fread(buffer,sizeof(unsigned char),size,fin);
	printf("enter your choice\npress 1 to rotate left\npress 2 to rotate right\npress 3 to rotate by 180\n");
        scanf("%d",&ch);
	switch(ch)
	{
	case 1:
	{
		for(i=0;i<width;i++)
		{
			for(j=0;j<height;j++)
			{
				out_buffer[j][height-1-i]=buffer[i][j];
			}
		}
	break;
	}
	case 2:
	{
	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
			out_buffer[i][j]=buffer[j][width-1-i];
		}
	}
	break;
	}
	case 3:
	{
		for(i=0;i<width;i++)
		{
			for(j=0;j<height;j++)
			{
				out_buffer[i][j]=buffer[width-1-i][j];
		
			}
		}
	break;
	}
	}
	for(i=0;i<width;i++)
	{
	for(j=0;j<height;j++)
	{
		int temp=out_buffer[i][j]+BRIGHTNESS;
		out_buffer[i][j]=(temp>MAX_COLOR)?MAX_COLOR:temp;
	}
	}
	fwrite(out_buffer,sizeof(unsigned char),size,fout);
	return 0;
}
