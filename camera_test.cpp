#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <unistd.h>
 
#include "opencv2/video/tracking.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <pthread.h>
#include <iostream>
#include "v4l_wrapper.h"

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <sys/loctl.h>
#include <termios.h>
#include <signal.h>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;

#define CAM_WIDTH 1280
#define CAM_HEIGHT 720

/** Function Headers */

int detectAndDisplay(IplImage *frame);

/** Global variables */
String window_name = "Face detection";

void detectFaces(IplImage *img);
int Kbhit (void);

//Todo: making Thread for picture
/*
void *CannyimageSaveThread(void *vargp)
{
    blur(src_gray, detected_edges, Size(3,3));
    Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

    dst = Scalar::all(0);

    src.copyTo(dst, detected_edges);

    sleep(1);
}
*/
/** @function detectAndSisplay */


unsigned char quit = 0;

void user_signal1(int sig){
    quit=1;
}

int digitAccepter(){
    //accept 4digit number
    int dev_push_switch, dev_fnd;
    unsigned char push_sw_buff[9];
    int buff_size;
    int flag_push[9] = {0,};
    dev_push_switch = open("/dev/fpga_push_switch", O_RDWR);

    if(dev_push_switch<0){
        close(dev_push_switch);
        printf("Device Open Error: dev number: %d \n", dev_push_switch);
        return -1;
    }
    else
    {
        printf("Switch Device Number %d \n", dev_push_switch );
    }
    
    printf("Please enter your phone number\n");
    (void)signal(SIGINT, user_signal1);
    buff_size=sizeof(push_sw_buff);
    
    int fourdigit[4];
    int resultnumber=0;
    int n=0;

    while(1){
        usleep(100000);

        if(n > 3){ break;}
        
        read(dev_push_switch,&push_sw_buff, buff_size);
        for(int i=0; i<9; i++){
            if(push_sw_buff[i]==1){
                fourdigit[n]=i;
                printf("%d\n",i);
                n++;
            }
                    
        }
        
    }
    printf("4 digit accepted!!\n");
    resultnumber= 1000*fourdigit[0] + 100*fourdigit[1] +
                  10*fourdigit[2] + fourdigit[3];
    char message[30];
    sprintf(message, "./fpga_test_fnd %d",resultnumber);
    system(message);
    printf("your number is %d\n", resultnumber);

    printf("exit in 2 seconds\n");
    usleep(1000*1000*2);

    system("./fpga_test_fnd 0");
    //--------------------
    close(dev_push_switch);
    close(dev_fnd);
    return resultnumber;
}

int main (int argc, char **argv)
{
    //for file stream
    ofstream outFile("output.txt");

    //
    
    fsl_v4l_cap mycamera;
    fsl_v4l_out mydisplay;
    int ret;
    char *buffer;

    int width = 640;
    int height = 480;

    //Todo
    //pthread_t camera_capture;
    IplImage *image;

    printf ("\n\nInitialzing Camera Device: Video0 (640x480)...");

    //Camera Device init
    ret = V4LWrapper_CreateCameraCapture (&mycamera, "/dev/video0", width, height);
    if (ret == V4LWrapper_SUCCESS)
    {
        printf ("OK\n");
    }
    else
    {
        printf ("\nDevice not found, make sure the driver are properly installed:");
        printf ("\nov5642_camera.ko, ov5640_camera_mipi.ko and mxc_v4l2_capture.ko\n");
        exit (0);
    }

    //init Device init
    printf ("\nInitialzing Display Device: video17, fb0 (640x480)...");
    ret = V4LWrapper_CreateOutputDisplay (&mydisplay, "/dev/fb0", NULL, width, height);
    if (ret == V4LWrapper_SUCCESS)
        printf ("OK\n");
    else
    {
        V4LWrapper_CloseCameraCapture (&mycamera);
        exit (0);
    }

    // allocating data buffer
    printf ("\nAllocating data buffer...");
    buffer = (char *) malloc (mycamera.g_frame_size);
    if (buffer)
        printf ("OK\n");
    else
    {
        V4LWrapper_CloseCameraCapture (&mycamera);
        V4LWrapper_CloseOutputDisplay (&mydisplay);

        exit (0);
    }

    printf ("\ncv create image...");
    image = cvCreateImage (cvSize (width, height), IPL_DEPTH_8U,3 );
    
    printf ("\nbefore whImage *gray = cile");
    //Todo 
    int value=0;
    while (!Kbhit())
    {
        value=0;    
        /*
        Mat img =  cvarrToMat(gray);
        cv::Mat img = cv::cvarrToMat(gray);
        if (img.empty())
        {
           printf(" --(!) No camd frame -- Break!");
            break;
        }*/
       // IplImage *gray = cvCreateImage (cvSize (width, height), IPL_DEPTH_8U, 1);
         
        // cv::Mat img = cv::Mat(cvSize(width,height),CV_8UC1);
        V4LWrapper_QueryFrame (&mycamera, buffer);	
        V4LWrapper_CvtColor (buffer, image->imageData, width, height, YUV422toRGB888);
        
        time_t timer;
        struct tm *t;
        char currentTime[50];
        timer = time(NULL);
        t = localtime(&timer);
        

        sprintf(currentTime, "%04d-%02d-%02d-%02d-%02d-%02d.bmp"
               , t->tm_year+1900, t->tm_mon+1, t->tm_mday,
                t-> tm_hour,t->tm_min,t->tm_sec);

        value = detectAndDisplay(image);
        if(value>0){ outFile << value << endl;}
        V4LWrapper_OutputDisplay (&mydisplay, buffer);
    }

    V4LWrapper_CloseCameraCapture (&mycamera);
    V4LWrapper_CloseOutputDisplay (&mydisplay);
    free (buffer);
    cvReleaseImage (&image);
    outFile.close();
    return 0;
}
/*
  void detectFaces(IplImage *img)
  {
  int i;
  int test = 1;
  CvSeq *faces;
Mat frame
//CvSeq *faces = cvHaarDetectObjects(img, cascade, storage, 1.1, 3,0, cvSize(50,50));
//CvSeq *faces = cvHaarDetectObjects(img, test, storage , 3,0, cvSize(50,50), cvSize(50,50));

for(i = 0 ; i < (faces ? faces->total : 0) ; i++) {
faces = cvHaarDetectObjects(img, cascade, storage, 1.1, 3,0, cvSize(50,50), cvSize(50,50));
CvRect *r = (CvRect*)cvGetSeqElem(faces, i);
cvRectangle( img, cvPoint(r->x, r->y), cvPoint(r->x + r->width, r->y + r->height), CV_RGB(0, 230, 0), 1, 8, 0);

CvFont a;Image *gray = c
cvInitFont(&a, CV_FONT_HERSHEY_TRIPLEX, 1.0f, 1.0f, 1.0f, 4, CV_AA);

char face[30];
sprintf(face, "Face %d", i + 1);

CvSize textSize;
int ymin = 0;
cvGetTextSize(face, &a, &textSize, &ymin);

cvPutText( img, face, cvPoint(r->x + ((r->width / 2) - textSize.width / 2), r ->y + r->height + 25), &a, CV_RGB(0, 230, 0));
}

cvShowImage("Source", img);
}
*/

int Kbhit (void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int/*Mat*/ detectAndDisplay(IplImage *frame)
{
    vector<Rect> faces;
    IplImage *grayimg = cvCreateImage (cvSize (640,480), IPL_DEPTH_8U, 1);
    
    CascadeClassifier face_cascade;
     
    char* face_cascade_name = "haarcascade_frontalface_alt.xml";
    int k;
    // load face_cascase xml file!
    if (!face_cascade.load(face_cascade_name))
    { printf("--(!)Error loading face cascade\n"); };
   
    // todo
    Mat origin_frame = cvarrToMat(frame);
    
    if (origin_frame.empty())
    {
        printf(" --(!) No camd frame -- Break!");
        exit(0);
    }
    
    cvCvtColor(frame, grayimg, CV_RGB2GRAY);

    Mat frame_gray =  cvarrToMat(grayimg);
    //equalizeHist(frame_gray, frame_gray);

    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    for (size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(origin_frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(0, 0, 255), 4, 8, 0);
    }

    if(faces.size() > 0){
        k= digitAccepter();
        imwrite("test.bmp",origin_frame );
       // printf("2.your number is %d",k);
       return k;
    }

    return 0;
   // imshow(window_name, origin_frame);
  // return origin_frame;
}

