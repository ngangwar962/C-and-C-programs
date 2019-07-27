#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
unsigned char buffer[512][512];
unsigned char out_buffer[512][512];
unsigned char out_buffer1[512][512][3];
unsigned char buffer_color[512][512][3];
unsigned char out_buffer_color[512][512][3];
pthread_t t[1000],t1[1000][1000],temp_thread;
unsigned char byte[54],colorTable[1024];
char s1[1000];
int character;
int average;
FILE *fin,*fout;
int choice1;
int size,height,width,bitDepth;
int A1[1000][1000][2];
int A[1000];
int ch3;
int flag=0;
float filter[3][3]={{0,-1,0},{1,1,1},{0,-1,0}};
void *colored_image_edge_detection(void *argc)//thread function for detecting the edges in the colored image
{
	int *c=(int*)argc;
	int i,j,k,l,p,q;
	float sum0=0;
	float sum1=0;
	float sum2=0;
	float filter_edge[3][3]={{1,0,-1},{2,0,-2},{1,0,-1}};
	for(i=c[0],p=0;i<c[0]+3;i++,p++)
	{
		for(j=c[1],q=0;j<c[1]+3;j++,q++)
		{
			sum0=sum0+(float)buffer_color[i][j][0]*filter_edge[p][q];
			sum1=sum1+(float)buffer_color[i][j][1]*filter_edge[p][q];
			sum2=sum2+(float)buffer_color[i][j][2]*filter_edge[p][q];
		}
	}
	out_buffer_color[c[0]+1][c[1]+1][0]=sum0;
	out_buffer_color[c[0]+1][c[1]+1][1]=sum1;
	out_buffer_color[c[0]+1][c[1]+1][2]=sum2;
	return NULL;

}
void *rotate_right_color(void *arg) /*thread function to rotate the complete image to right*/
{
int *c=(int*)arg;
int i;
for(i=0;i<512;i++)
{
	out_buffer_color[c[0]][i][0]=buffer_color[i][511-c[0]][0];
	out_buffer_color[c[0]][i][1]=buffer_color[i][511-c[0]][1];
	out_buffer_color[c[0]][i][2]=buffer_color[i][511-c[0]][2];
}
return NULL;
}
void *edge_detection(void *argc)//edge detection 
{
	int *c=(int*)argc;
	int i,j;
	if((buffer[c[0]+1][c[1]+1]>=average)&&(buffer[c[0]+1][c[1]+1]<=220))
	{
		out_buffer[c[0]+1][c[1]+1]=255;
	}
	else
	{
		out_buffer[c[0]+1][c[1]+1]=0;
	}
	return NULL;
}
void *sepia_effect(void *argc)//putting sepia effect into the image
{
	int i,j,ch;
	printf("in which color you want to see the image\npress 1 to see it in green\npress 2 to see it in reddish\npress 3 to see it in blue\npress other any other key to see as it is\nenter your choice\n");
	scanf("%d",&ch);
	switch(ch)
	{
		case 1:
		{
			for(i=0;i<height;i++)
			{
				for(j=0;j<width;j++)
				{
					putc(out_buffer_color[i][j][0],fout);
					putc(out_buffer_color[i][j][2],fout); //green
					putc(out_buffer_color[i][j][1],fout);
		
				}
			}
			break;
		}
		case 2:
		{
			for(i=0;i<height;i++)
			{
				for(j=0;j<width;j++)
				{
					putc(out_buffer_color[i][j][1],fout);
					putc(out_buffer_color[i][j][0],fout);//reddish
					putc(out_buffer_color[i][j][2],fout);
		
				}
			}
			break;
		}
		case 3:
		{
			for(i=0;i<height;i++)
			{
				for(j=0;j<width;j++)
				{
					putc(out_buffer_color[i][j][2],fout);
					putc(out_buffer_color[i][j][0],fout);//bluish
					putc(out_buffer_color[i][j][1],fout);
		
				}
			}
			break;
		}//case 3 ending
		default:
		{
			for(i=0;i<height;i++)
			{
				for(j=0;j<width;j++)
				{
					putc(out_buffer_color[i][j][0],fout);
					putc(out_buffer_color[i][j][1],fout);//normal image
					putc(out_buffer_color[i][j][2],fout);
		
				}
			}
			break;
		}//case 4 ending
	}//switch case closing
return NULL;
}
void *maxpool(void *argc)//maxpooling the value in the matrix
{
int *c=(int*)argc;
int i,j;
int max1=0,max2=0,max3=0;
for(i=c[0];i<c[0]+3;i++)
{
	for(j=c[1];j<c[1]+3;j++)
	{
		if(buffer_color[i][j][0]>max1)
		max1=buffer_color[i][j][0];
		if(buffer_color[i][j][1]>max2)
		max2=buffer_color[i][j][1];
		if(buffer_color[i][j][2]>max3)
		max3=buffer_color[i][j][2];
	}
}
out_buffer_color[c[0]+1][c[1]+1][0]=max1;
out_buffer_color[c[0]+1][c[1]+1][1]=max2;
out_buffer_color[c[0]+1][c[1]+1][2]=max3;
return NULL;
}
void *maxpool_grayscale(void *argc)//maxpooling the value in the matrix
{
int *c=(int*)argc;
int i,j;
int max1=0;
for(i=c[0];i<c[0]+3;i++)
{
	for(j=c[1];j<c[1]+3;j++)
	{
		if(buffer[i][j]>max1)
		max1=buffer[i][j];
	}
}
out_buffer[c[0]+1][c[1]+1]=max1;
return NULL;
}
void *rotate_left_color(void *arg) /*thread function to rotate the complete image to left*/
{
int i;
int *c=(int*)arg;
for(i=0;i<512;i++)
{
	out_buffer_color[c[0]][i][0]=buffer_color[511-i][c[0]][0];
	out_buffer_color[c[0]][i][1]=buffer_color[511-i][c[0]][1];
	out_buffer_color[c[0]][i][2]=buffer_color[511-i][c[0]][2];
}
return NULL;
}
void *rotate_invert_color(void *arg) /*thread function to invert the complete image*/
{
	int i;
	int *c=(int*)arg;
	for(i=0;i<512;i++)
	{
		out_buffer_color[c[0]][i][0]=buffer_color[511-c[0]][i][0];
		out_buffer_color[c[0]][i][1]=buffer_color[511-c[0]][i][1];
		out_buffer_color[c[0]][i][2]=buffer_color[511-c[0]][i][2];
	}
	return NULL;
}
void *part_rotation_color(void *arg) /*thread function to rotate the portions of the images*/
{
	int i,j;
	int *c=(int*)arg;
	int ch;
	printf("press 1 to rotate this part to right\npress 2 to rotate this part to left\npress 3 to invert this part\n");
	scanf("%d",&ch);
	switch(ch)
	{
	case 1: /*rotating the part of the image to the right*/
	{
		for(i=c[0];i<c[3];i++)
		{
			for(j=c[0];j<c[3];j++)
			{
				out_buffer_color[i][j][0]=buffer_color[j][c[3]-i][0];
				out_buffer_color[i][j][1]=buffer_color[j][c[3]-i][1];
				out_buffer_color[i][j][2]=buffer_color[j][c[3]-i][2];
			}
		}
		break;	
	}
	case 2:/*rotating this part of the image to the left*/
	{
		for(i=c[0];i<c[3];i++)
		{
			for(j=c[0];j<c[3];j++)
			{
				out_buffer_color[i][j][0]=buffer_color[c[3]-j][i][0];
				out_buffer_color[i][j][1]=buffer_color[c[3]-j][i][1];
				out_buffer_color[i][j][2]=buffer_color[c[3]-j][i][2];
			}
		}
		break;
	}
	case 3:/*inverting the part of the image*/
	{
		for(i=c[0];i<c[3];i++)
		{
			for(j=c[0];j<c[3];j++)
			{
				out_buffer_color[i][j][0]=buffer_color[c[3]-i][j][0];	
				out_buffer_color[i][j][1]=buffer_color[c[3]-i][j][1];
				out_buffer_color[i][j][2]=buffer_color[c[3]-i][j][2];
			}
		}
		break;
	}
	}
return NULL;
}

void *matrix_multiply_image_blurring(void *arg)//thread function to blur the colored image
{
	int *c=(int*)arg;
	int i,j,k,l,p,q;
	float sum0=0;
	float sum1=0;
	float sum2=0;
	float v=1/9.0;
	float filter_blur[3][3]={{v,v,v},{v,v,v},{v,v,v}};
	for(i=c[0],p=0;i<c[0]+3;i++,p++)
	{
		for(j=c[1],q=0;j<c[1]+3;j++,q++)
		{
			sum0=sum0+(float)buffer_color[i][j][0]*filter_blur[p][q];
			sum1=sum1+(float)buffer_color[i][j][1]*filter_blur[p][q];
			sum2=sum2+(float)buffer_color[i][j][2]*filter_blur[p][q];
		}
	}
	out_buffer_color[c[0]+1][c[1]+1][0]=sum0;
	out_buffer_color[c[0]+1][c[1]+1][1]=sum1;
	out_buffer_color[c[0]+1][c[1]+1][2]=sum2;

	return NULL;
}
void *matrix_multiply_image_sharpening(void *arg)//thread function to sharpen the image
{
	int *c=(int*)arg;
	int i,j,k,l,p,q;
	float sum0=0;
	float sum1=0;
	float sum2=0;
	float v=1/9.0;
	float filter_sharpening[3][3]={{0,-1,0},{-1,5,-1},{0,-1,0}};
	for(i=c[0],p=0;i<c[0]+3;i++,p++)
	{
		for(j=c[1],q=0;j<c[1]+3;j++,q++)
		{
			sum0=sum0+(float)buffer_color[i][j][0]*filter_sharpening[p][q];
			sum1=sum1+(float)buffer_color[i][j][1]*filter_sharpening[p][q];
			sum2=sum2+(float)buffer_color[i][j][2]*filter_sharpening[p][q];
		}
	}
	float quant=2;
	out_buffer_color[c[0]+1][c[1]+1][0]=sum0;
	out_buffer_color[c[0]+1][c[1]+1][1]=sum1;
	out_buffer_color[c[0]+1][c[1]+1][2]=sum2;
	return NULL;
}

void *rotate_right(void *arg)/*thread function to rotate the complete image to right*/
{
int *c=(int*)arg;
int i;
for(i=0;i<512;i++)
{
	out_buffer[c[0]][i]=buffer[i][511-c[0]];
}
return NULL;
}

void *rotate_left(void *arg)/*thread function to rotate the complete image to left*/
{
int i;
int *c=(int*)arg;
for(i=0;i<512;i++)
{
	out_buffer[c[0]][i]=buffer[511-i][c[0]];
}
return NULL;
}
void *rotate_invert(void *arg)/*thread function to invert the complete image*/
{
	int i;
	int *c=(int*)arg;
	for(i=0;i<512;i++)
	{
		out_buffer[c[0]][i]=buffer[511-c[0]][i];
	}
	return NULL;
}

void *matrix_multiplication_edge_detection(void *arg) /*code for blurring the grayscale image*/
{
	int *c=(int*)arg;
	int i,j,p,q;
	long long int sum0=0;
	float filter[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};
	for(i=c[0],p=0;i<c[0]+3;i++,p++)
	{
		for(j=c[1],q=0;j<c[1]+3;j++,q++)
		{
			sum0=sum0+(float)buffer[i][j]*filter[p][q];
		}
	}
	out_buffer[c[0]+1][c[1]+1]=sum0;
	return NULL;
}
void *rgb_to_gray(void *arg)//thread function for converting the colored image back to grayscale
{
	int *c=(int*)arg;
	int i,j;
	float y=0;
	y=(buffer_color[c[0]][c[1]][0]*0.3) + (buffer_color[c[0]][c[1]][1]*0.59)	+ (buffer_color[c[0]][c[1]][2]*0.11);
	out_buffer1[c[0]][c[1]][0]=y;
	out_buffer1[c[0]][c[1]][1]=y;
	out_buffer1[c[0]][c[1]][2]=y;
	return NULL;
}
void *crop_from_side(void *arg)//thread function to crop the image from the side
{
int i,j;
int *c=(int*)arg;
out_buffer[c[0]][c[1]]=buffer[c[0]][c[1]];
return NULL;
}
void *part_rotation(void *arg)   /*thread function to rotate the portions of the grayscale image*/
{
	int i,j;
	int *c=(int*)arg;
	int ch;
	printf("press 1 to rotate this part to right\npress 2 to rotate this part to left\npress 3 to invert this part\n");
	scanf("%d",&ch);
	switch(ch)
	{
	case 1:/*rotating the part of the image to the right*/
	{
		for(i=c[0];i<c[3];i++)
		{
			for(j=c[0];j<c[3];j++)
			{
				out_buffer[i][j]=buffer[j][c[3]-i];
			}
		}
		break;	
	}
	case 2:/*rotating this part of the image to the left*/
	{
		for(i=c[0];i<c[3];i++)
		{
			for(j=c[0];j<c[3];j++)
			{
				out_buffer[i][j]=buffer[c[3]-j][i];
			}
		}
		break;
	}
	case 3:/*inverting the part of the image*/
	{
		for(i=c[0];i<c[3];i++)
		{
			for(j=c[0];j<c[3];j++)
			{
				out_buffer[i][j]=buffer[c[3]-i][j];	
			}
		}
		break;
	}
	}//switch case ending
return NULL;
}
void *grayscale_image_reading(void *argc)//thread function for reading the grayscale file
{
	int i,j,k,l,ch;
	fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena.bmp","r");
	fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_rotate_threads.bmp","w+");
	/*fin=fopen("E:/p3/image_work_project/lena_gray.bmp","r");
	fout=fopen("E:/p3/image_work_project/lena_thread_operation.bmp","+w");*/
	if(fin==NULL)
	{
		printf("file not found at specified position\n");
	}
	for(i=0;i<54;i++)	
	{
		byte[i] = getc(fin);
	}
	fwrite(byte,sizeof(unsigned char),54,fout);	
	height = *(int*)&byte[18];
	width = *(int*)&byte[22];
	bitDepth = *(int*)&byte[28];

	/*printf("width: %d\n",width);
	printf("height: %d\n",height );
	printf("bitdepth: %d\n",bitDepth);*/

	size = height*width;

	if(bitDepth <= 8)			
	{
		fread(colorTable,sizeof(unsigned char),1024,fin);
		fwrite(colorTable,sizeof(unsigned char),1024,fout);
	}

	fread(buffer,sizeof(unsigned char),size,fin);
	for(i=0;i<height;i++)
	{
		A[i]=i;
	}
	for(i=0;i<height-2;i++)
	{
		for(j=0;j<width-2;j++)
		{
			A1[i][j][0]=i;
			A1[i][j][1]=j;
		}
	}
return NULL;
}
void *colored_image_reading(void *argc)//thread function to read the colored image
{
	int i,j,k,l,ch;
	fin=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_color.bmp","r");
	fout=fopen("/media/gangwar/4E2C57A92C578B3B/p3/lena_rotate_threads.bmp","w+");
	/*fin=fopen("E:/p3/image_work_project/lena_color.bmp","r");
	fout=fopen("E:/p3/image_work_project/lena_thread_operation.bmp","+w");*/
	if(fin==NULL)
	{
		printf("file not found at specified position\n");
	}
	for(i=0;i<54;i++)	
	{
		byte[i] = getc(fin);
	}
	fwrite(byte,sizeof(unsigned char),54,fout);

	height = *(int*)&byte[18];
	width = *(int*)&byte[22];
	bitDepth = *(int*)&byte[28];

	size = height*width;

	if(bitDepth <= 8)			
	{
		fread(colorTable,sizeof(unsigned char),1024,fin);
		fwrite(colorTable,sizeof(unsigned char),1024,fout);
	}

	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
			buffer_color[i][j][0]=getc(fin);
			buffer_color[i][j][1]=getc(fin);
			buffer_color[i][j][2]=getc(fin);
		}
	}
	for(i=0;i<height;i++)
	{
		A[i]=i;
	}
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			A1[i][j][0]=i;
			A1[i][j][1]=j;
		}
	}
	out_buffer_color[height][width][3]={0};
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
				out_buffer_color[i][j][0]=buffer_color[i][j][0];
				out_buffer_color[i][j][1]=buffer_color[i][j][1];
				out_buffer_color[i][j][2]=buffer_color[i][j][2];
		}
	}
}
void *part_viewer(void *argc)//thread function to view a particular portion of the image
{
	int *c=(int*)argc;
	int i,j;
	for(i=c[0];i<=c[2];i++)
	{
		for(j=c[1];j<=c[3];j++)
		{
			out_buffer[i][j]=buffer[i][j];	
		}
	}
	fwrite(out_buffer,sizeof(unsigned char),size,fout);
	return NULL;
}
int main(int argc,char *argv[])
{
	int i,j,k,l,ch;
	do
        {
	printf("press 1 for gray scale image\npress 2 for coloured image\n");
	scanf("%d",&choice1);
	switch(choice1)
	{
	case 1:
	{
		pthread_create(&temp_thread,NULL,grayscale_image_reading,NULL);
		pthread_join(temp_thread,NULL);
		printf("press 1 to rotate the image to right\npress 2 to rotate the image to left\npress 3 to rotate the image by 180\npress 4 if wants to rotate the particulat portion of the image\npress 5 to blurr the image\npress 6 to crop image form each side\npress 7 for edge detection\npress 8 for sobel filter\npress 9 to view particular portion\nenter your choice\n");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:/*rotating the complete image to the right*/
			{
				for(i=0;i<height;i++)
				{
					pthread_create(&t[i],NULL,rotate_right,(void*)&A[i]);
					pthread_join(t[i],NULL);
				}
				fwrite(out_buffer,sizeof(unsigned char),size,fout);
				break;
			}
			case 2:/*rotating the complete image to the left*/
			{
				for(i=0;i<height;i++)
				{
					pthread_create(&t[i],NULL,rotate_left,(void*)&A[i]);
					pthread_join(t[i],NULL);
				}
				fwrite(out_buffer,sizeof(unsigned char),size,fout);
				break;
			}
			case 3:/*inverting the complete image*/
			{	
				for(i=0;i<height;i++)
				{
					pthread_create(&t[i],NULL,rotate_invert,(void*)&A[i]);
					pthread_join(t[i],NULL);
				}
				fwrite(out_buffer,sizeof(unsigned char),size,fout);	
				break;
			}
			case 4:/*code for rotating some part of the image*/
			{
				int x1=height+1,x2=height+1,y2=width+1,y1=width+1;
				printf("enter the leftmost coordinates as x and y\n");	
				while(x1>height||y1>width)
				{
					scanf("%d%d",&x1,&y1);
				}
				printf("ok coordinates entered are fine\n");
				do
				{
					printf("enter the rightmost coordinates as x and y such that difference should be equal\n");
					scanf("%d%d",&x2,&y2);
				}while((x2>height||y2>width)||((abs(x2-y2))!=(abs(x1-y1))));
				printf("ok these are also fine\n");
				for(i=0;i<height;i++)
				{
				for(j=0;j<width;j++)
				{
				out_buffer[i][j]=buffer[i][j];
				}
				}
				int arr[1][4]={x1,y1,x2,y2};
				do
				{
				pthread_create(&t[0],NULL,part_rotation,(void*)&arr[0]);
				pthread_join(t[0],NULL);
				fwrite(out_buffer,sizeof(unsigned char),size,fout);
				printf("want to try another way rotation press 7 for that else anything else\n");
				scanf("%d",&character);
				if(character==7)
				{
					pthread_create(&temp_thread,NULL,grayscale_image_reading,NULL);
					pthread_join(temp_thread,NULL);
				}
				if(character!=7)
				break;
				}while(character==7);
				if(character!=7)
				break;
			}
			case 5:
			{
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{
						pthread_create(&t1[i][j],NULL,maxpool_grayscale,(void*)&A1[i][j]);
						pthread_join(t1[i][j],NULL);
					}
				}
				fwrite(out_buffer,sizeof(unsigned char),size,fout);
				break;
			}

			case 6:
			{
				int side,k,l;
				printf("enter the size to be cropped from each sides\n");
				scanf("%d",&side);
				printf("cropped amount will be %d\n",side);
				for(k=side;k<(height-side);k++)
				{
					for(l=side;l<(width-side);l++)
					{
						pthread_create(&t1[k][l],NULL,crop_from_side,(void*)&A1[k][l]);
						pthread_join(t1[k][l],NULL);
					}				
				}
				fwrite(out_buffer,sizeof(unsigned char),size,fout);
				break;
			}
			case 7:
			{
				long long int sum=0;
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{
						sum=sum+buffer[i][j];
					}
				}
				average=sum/size;
				for(i=0;i<height-2;i++)
				{
					for(j=0;j<width-2;j++)
					{	
						pthread_create(&t1[i][j],NULL,edge_detection,(void*)&A1[i][j]);
						pthread_join(t1[i][j],NULL);
					}
				}
				fwrite(out_buffer,sizeof(unsigned char),size,fout);
			}
			case 8:
			{
				for(i=0;i<height-2;i++)
				{
					for(j=0;j<width-2;j++)
					{
						pthread_create(&t1[i][j],NULL,matrix_multiplication_edge_detection,(void*)&A1[i][j]);
						pthread_join(t1[i][j],NULL);						
					}					
				}
				fwrite(out_buffer,sizeof(unsigned char),size,fout);
				break;
			}
			case 9:
			{
				int x1,x2,y1,y2;
				printf("enter the top left most coordinates\n");
				scanf("%d%d",&x1,&y1);
				printf("enter the bottom left y coordinate\n");
				scanf("%d",&x2);
				printf("enter the top right most x coordinates\n");
				scanf("%d",&y2);
				int temp_array[]={x1,y1,x2,y2};
				for(i=x1;i<=x2;i++)
				{
				
					pthread_create(&t[i],NULL,part_viewer,(void*)&temp_array[0]);
					pthread_join(t[i],NULL);
				}
				break;
			}
		}
		break;
	  }
	  case 2:
	  {
		pthread_create(&temp_thread,NULL,colored_image_reading,NULL);
		pthread_join(temp_thread,NULL);
		printf("press 1 to rotate the image to right\npress 2 to rotate the image to left\npress 3 to rotate the image by 180\npress 4 if wants to rotate the particulat portion of the image\npress 5 to blur the colored image\npress 6 for converting the rgb to gray scale\npress 7 to maxpool\npress 8 for image sharpening\npress 9 for edge detection\nenter your choice\n");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:/*rotating the complete image to the right*/
			{
				for(i=0;i<height;i++)
				{
					pthread_create(&t[i],NULL,rotate_right_color,(void*)&A[i]);
					pthread_join(t[i],NULL);
				}
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{
						putc(out_buffer_color[i][j][0],fout);
						putc(out_buffer_color[i][j][1],fout);
						putc(out_buffer_color[i][j][2],fout);
					}
				}
				break;
			}
			case 2:/*rotating the complete image to the left*/
			{
				for(i=0;i<height;i++)
				{
					pthread_create(&t[i],NULL,rotate_left_color,(void*)&A[i]);
					pthread_join(t[i],NULL);
				}
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{
						putc(out_buffer_color[i][j][0],fout);
						putc(out_buffer_color[i][j][1],fout);
						putc(out_buffer_color[i][j][2],fout);
					}
				}
				
				break;
			}
			case 3:/*inverting the complete image*/
			{	
				for(i=0;i<height;i++)
				{
					pthread_create(&t[i],NULL,rotate_invert_color,(void*)&A[i]);
					pthread_join(t[i],NULL);
				}
				for(i=0;i<height;i++)
				{
				for(j=0;j<width;j++)
				{
					putc(out_buffer_color[i][j][0],fout);
					putc(out_buffer_color[i][j][1],fout);
					putc(out_buffer_color[i][j][2],fout);
				}
				}
	
				break;

			}
			case 4:   /*code for rotating some part of the image*/
			{
				flag=0;
				int x1=height+1,x2=height+1,y2=width+1,y1=width+1;
				printf("enter the leftmost coordinates as x and y\n");	
				while(x1>height||y1>width)
				{
					scanf("%d%d",&x1,&y1);
				}
				printf("ok coordinates entered are fine\n");
				do
				{
					printf("enter the rightmost coordinates as x and y such that difference should be equal\n");
					scanf("%d%d",&x2,&y2);
				}while((x2>height||y2>width)||((abs(x2-y2))!=(abs(x1-y1)))||(y2<y1)||(x2<x1));
				printf("ok these are also fine\n");
				int arr[1][4]={x1,y1,x2,y2};
				pthread_create(&t[0],NULL,part_rotation_color,(void*)&arr[0]);
				pthread_join(t[0],NULL);
				do
				{
				printf("want to see sepia effect here with the rotated part of the image\npress 1 to see it in red\npress 2 to see it in the green\npress 3 to see it in blue\npress 4 to see it as it is\npress 5 to exit\nenter your choice\n");
				scanf("%d",&ch3);
				switch(ch3)
				{
					case 1:
					{
						for(i=0;i<height;i++)
						{
							for(j=0;j<width;j++)
							{
								if((i>=x1&&i<=x2)&&(j>=y1&&j<=y2))
								{
									putc(out_buffer_color[i][j][1],fout);
									putc(out_buffer_color[i][j][0],fout);//reddish
									putc(out_buffer_color[i][j][2],fout);
								}
								else
								{
									putc(out_buffer_color[i][j][0],fout);
									putc(out_buffer_color[i][j][1],fout);//normal
									putc(out_buffer_color[i][j][2],fout);	
								}
							}
						}	
					break;
					}
					case 2:
					{
						for(i=0;i<height;i++)
						{
							for(j=0;j<width;j++)
							{
								if((i>=x1&&i<=x2)&&(j>=y1&&j<=y2))
								{
									putc(out_buffer_color[i][j][0],fout);
									putc(out_buffer_color[i][j][2],fout);//greenish
									putc(out_buffer_color[i][j][1],fout);
								}
								else
								{
									putc(out_buffer_color[i][j][0],fout);
									putc(out_buffer_color[i][j][1],fout);//normal
									putc(out_buffer_color[i][j][2],fout);	
								}
							}
						}	
						
					break;
					}
					case 3:
					{
						for(i=0;i<height;i++)
						{
							for(j=0;j<width;j++)
							{
								if((i>=x1&&i<=x2)&&(j>=y1&&j<=y2))
								{
									putc(out_buffer_color[i][j][2],fout);
									putc(out_buffer_color[i][j][0],fout);//bluish
									putc(out_buffer_color[i][j][1],fout);
								}
								else
								{
									putc(out_buffer_color[i][j][0],fout);
									putc(out_buffer_color[i][j][1],fout);//normal
									putc(out_buffer_color[i][j][2],fout);	
								}
							}
						}	
						
					break;

					}
					case 4:
					{
						for(i=0;i<height;i++)
						{
							for(j=0;j<width;j++)
							{
								putc(out_buffer_color[i][j][0],fout);
								putc(out_buffer_color[i][j][1],fout);
								putc(out_buffer_color[i][j][2],fout);
							}
						}
						break;
					}//case 4 ends here
					case 5:
					{					
						flag=1;
						break;
					}
				}//switch case ending here
				if(flag==1)
				break;
				else
				{
					printf("press digit 7 to try once more and for exiting press other digit\n");
					fflush(stdin);
					fflush(stdin);
					scanf("%d",&character);
					if(character!=7)
					{
						break;
					}
					pthread_create(&temp_thread,NULL,colored_image_reading,NULL);
					pthread_join(temp_thread,NULL);
				}
				}while(character==7);//while loop ending here
				break;
			}
			case 5:/* code for blurring the image using thread function matrix_multiply_image_blurring*/
			{
				do
				{
					for(i=0;i<height-2;i++)
					{
						for(j=0;j<width-2;j++)
						{
							pthread_create(&t1[i][j],NULL,matrix_multiply_image_blurring,(void*)&A1[i][j]);
							pthread_join(t1[i][j],NULL);
						}
					}
					pthread_create(&temp_thread,NULL,sepia_effect,NULL);
					pthread_join(temp_thread,NULL);
					printf("press 7 to check another color\n");
					scanf("%d",&character);
					pthread_create(&temp_thread,NULL,colored_image_reading,NULL);
					pthread_join(temp_thread,NULL); 
				}while(character==7);//do-while loop ends here
				break;
			}
			case 8:/* code for shapening the image using thread function matrix_multiply_image_sharpening*/
			{
				for(i=0;i<height-2;i++)
				{
					for(j=0;j<width-2;j++)
					{
						pthread_create(&t1[i][j],NULL,matrix_multiply_image_sharpening,(void*)&A1[i][j]);
						pthread_join(t1[i][j],NULL);
					}
				}
				pthread_create(&temp_thread,NULL,sepia_effect,NULL);
				pthread_join(temp_thread,NULL);
				break;
			}

			case 6:/*converting the rgb image to grayscale image*/
			{
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{
						pthread_create(&t1[i][j],NULL,rgb_to_gray,(void*)&A1[i][j]);
						pthread_join(t1[i][j],NULL);
					}
				}
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{

						putc(out_buffer1[i][j][2],fout);
						putc(out_buffer1[i][j][1],fout);
						putc(out_buffer1[i][j][0],fout);
					}
				}
				fwrite(out_buffer1,sizeof(unsigned char),size,fout);
				break;
			}
			case 7:
			{
				for(i=0;i<height-2;i++)
				{
					for(j=0;j<width-2;j++)
					{
						pthread_create(&t1[i][j],NULL,maxpool,(void*)&A1[i][j]);					
						pthread_join(t1[i][j],NULL);
					}
				}
				pthread_create(&temp_thread,NULL,sepia_effect,NULL);
				pthread_join(temp_thread,NULL);
			        break;//case 7 break
		        }//case 7 closing
			case 9:
			{
				for(i=0;i<height-2;i++)
				{
					for(j=0;j<width-2;j++)
					{
						pthread_create(&t1[i][j],NULL,colored_image_edge_detection,(void*)&A1[i][j]);
						pthread_join(t1[i][j],NULL);
					}
				}
				for(i=0;i<height;i++)
				{
					for(j=0;j<width;j++)
					{
						putc(out_buffer_color[i][j][0],fout);
						putc(out_buffer_color[i][j][1],fout);
						putc(out_buffer_color[i][j][2],fout);
							
					}
				}
				break;
			}
		}//switch case ending in case 2
		break;//case 2 breaks here
    }//case 2 closing 
}//swich case ending
printf("if want to exit enter exit and for continuation anything else\n");
scanf("%s",s1);
}while(strcmp(s1,"exit"));//do while loop ends here
return 0;
}//main function ending
