#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

IplImage* image = 0;
IplImage* src = 0;

int main(int argc, char* argv[])
{
        // имя картинки задаётся первым параметром
        char filename[] = "rect.jpg";
        // получаем картинку
        image = cvLoadImage(filename,1);
        // клонируем картинку 
        src = cvCloneImage(image);

        printf("[i] image: %s\n", filename);
        assert( src != 0 );
	int xt[image->width*image->height];
	int yt[image->width*image->height];
	int t = 0;

	for( int y=0; y<image->height; y++ ) {
                uchar* ptr = (uchar*) (image->imageData + y * image->widthStep);
                for( int x = 6; x < image->width; x = x+5 ) {
                        // 3 канала 
			if(x>0){
				if(abs(ptr[3*x]-ptr[3*(x-1)])>30 || abs(ptr[3*x+1]-ptr[3*(x-1)+1])>30 || abs(ptr[3*x+2]-ptr[3*(x-1)+2])>30){
					
					xt[t] = x;
					yt[t] = y;
					t++;
				}
			}		
                }
        }
	

	int maxX = xt[0]; 
	int minX = xt[0]; 
	int maxY = yt[0]; 
	int minY = yt[0]; 
	int i; 
	for (i = 0; i < t; i++) { 
		if (maxX < xt[i]) maxX = xt[i]; 
		if (minX > xt[i]) minX = xt[i]; 
		if (maxX < xt[i]) maxY = yt[i]; 
		if (minY < xt[i]) minY = yt[i]; 
	}
	
	cvSetImageROI(image, cvRect(minX, maxY, abs(maxX - minX), abs(maxY - minY)));
	IplImage *sub_img = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	cvCopy(image, sub_img, NULL);
	

	cvSaveImage("test.png",sub_img);
	cvResetImageROI(image);
	
 	// окно для отображения картинки
        cvNamedWindow("original",CV_WINDOW_AUTOSIZE);

        // показываем картинку
        cvShowImage("original",image);

	// окно для отображения найденного прямоугольника
        cvNamedWindow("rect",CV_WINDOW_AUTOSIZE);

        // показываем найденный прямоугольник
        cvShowImage("rect",sub_img);
	





        // ждём нажатия клавиши
        cvWaitKey(0);

        // освобождаем ресурсы
        cvReleaseImage(& image);
        cvReleaseImage(&src);
	cvReleaseImage(&sub_img);
        // удаляем окно
        cvDestroyWindow("original");
	cvDestroyWindow("rect");
        return 0;

	
}
