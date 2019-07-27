#include <stdio.h>
#include <time.h>

int main()
{
	clock_t start, stop;

	start = clock();											
	FILE *fIn,*fOut;
	fIn=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_color.bmp","r");
	fOut=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_color_rgb_to_gray.bmp","w+");
		    		

	int i,j,y;
	unsigned char byte[54];
	
	if(fIn==NULL)												
	{											
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)										
	{									
		byte[i] = getc(fIn);								
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);					//write the header back

	// extract image height, width and bitDepth from imageHeader 
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;									//calculate image size

	unsigned char buffer[height][width][3];								//to store the image data
	unsigned char out_buffer[height][width][3];
			
	for(i=0;i<height;i++)											//RGB to gray
	{
		for(j=0;j<width;j++)
		{
			y=0;
			buffer[i][j][2]=getc(fIn);									//blue
			buffer[i][j][1]=getc(fIn);									//green
			buffer[i][j][0]=getc(fIn);									//red
			y=(buffer[i][j][0]*0.3)+(buffer[i][j][1]*0.59)+(buffer[i][j][2]*0.11);
			/*out_buffer[i][j][0]=y;
			out_buffer[i][j][1]=y;
			out_buffer[i][j][2]=y;*/
			putc(y,fOut);
			putc(y,fOut);
			putc(y,fOut);
		}
	}
	fwrite(out_buffer,sizeof(unsigned char ),size,fOut);
	fclose(fOut);
	fclose(fIn);

stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start); 
printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
	return 0;
}
