//---------------------------------------------------------
// 概要      : 追記プログラム（情報有り）
// File Name : jyouhoutuiki.cpp
// Library   : OpenCV 2.0
//---------------------------------------------------------

#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#pragma comment (lib,"cv200d.lib")
#pragma comment (lib,"cxcore200d.lib")
#pragma comment (lib,"highgui200d.lib")



#define WIDTH	720	//	キャプチャ画像の横幅
#define HEIGHT	480	//	キャプチャ画像の縦幅

#define THRESH_BOTTOM	200			//	明度下限の閾値
//#define THRESH_TOP	80			//	明度値の上限の閾値
#define THRESHOLD_MAX_VALUE	255		//	2値化の際に使用する最大値

#define CIRCLE_RADIUS	2		//	円の半径
#define LINE_THICKNESS	3		//	線の太さ
#define LINE_TYPE		8		//	線の種類
#define SHIFT			0		//	座標の小数点以下の桁を表すビット数


int main( int argc, char **argv ){ 
	int key;							//	キー入力用の変数
	CvCapture *capture = NULL;			//	カメラキャプチャ用の構造体
	IplImage *frameImage;				//	キャプチャ画像用IplImage
	IplImage *frameImage2;				//	キャプチャ画像用IplImage2

	//	画像を生成する
	IplImage *backgroundImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//背景画像用IplImage
	IplImage *grayImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );				//グレースケール画像用IplImage
	IplImage *differenceImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//差分画像用IplImage

	IplImage *hsvImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3 );				//HSV画像用IplImage
	IplImage *hueImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );				//色相(H)情報用IplImage
	IplImage *saturationImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//彩度(S)情報用IplImage
	IplImage *valueImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );				//明度(V)情報用IplImage
	IplImage *thresholdImage1 = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//明度がTHRES_BOTTOMより大きい領域用IplImage
//	IplImage *thresholdImage2 = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//明度がTHRES_TOP以下の領域用IplImage
//	IplImage *thresholdImage3 = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//thresholdImage1とthresholdImage2のAND演算結果用IplImage
	IplImage *lightImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );				//光っている部分の領域の抽出結果用IplImage
	
	char windowNameCapture[] = "Capture"; 			//キャプチャした画像を表示するウィンドウの名前
	char windowNameLight[] = "Light";				//光っている部分の領域を表示するウィンドウの名前
	char windowNameCapture2[] = "Capture2"; 		//キャプチャした画像を表示するウィンドウの名前
	char windowNameThreshold[] = "Threshold";		//thresholdImage1を表示するウィンドウの名前

	CvMoments moment;
	double m_00;
	double m_10;
	double m_01;
	int gravityX;
	int gravityY;
	unsigned char r,g,b;
    
	
	
	int x, y;
	int m,d,ss;
	uchar r0,g0,b0,r1,g1,b1,r2,g2,b2,r3,g3,b3,s;
	int rr,gg,bb;




	//	カメラを初期化する
	if ( ( capture = cvCreateCameraCapture( 0 ) ) == NULL ) {
		//	カメラが見つからなかった場合
		printf( "カメラが見つかりません\n" );
		return -1;
	}

	//	ウィンドウを生成する
	cvNamedWindow( windowNameCapture, CV_WINDOW_AUTOSIZE );
	cvNamedWindow( windowNameLight, CV_WINDOW_AUTOSIZE );
  	cvNamedWindow( windowNameCapture2, CV_WINDOW_AUTOSIZE );
	cvNamedWindow( windowNameThreshold, CV_WINDOW_AUTOSIZE );

  	//	初期背景を設定するためにカメラから画像を取得
	frameImage = cvQueryFrame( capture );
	//	frameImageをグレースケール化し、背景画像とする
	cvCvtColor( frameImage, backgroundImage, CV_BGR2GRAY );
	frameImage2 = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3 );
	cvSet (frameImage2, CV_RGB(0,0,0));  //黒色で塗りつぶす
	
	rr=0;
	gg=150;
	bb=0;
	m=0;
	d=0;
	ss=0;
	//	メインループ
	while( 1 ) {
		frameImage = cvQueryFrame( capture );

      /* 画素値を直接操作する一例*/   
		x = 160;
		y = 120;

		b0 = frameImage ->imageData[frameImage ->widthStep * y + x * 3];        // B
		g0 = frameImage ->imageData[frameImage ->widthStep * y + x * 3 + 1];    // G      
		r0 = frameImage ->imageData[frameImage ->widthStep * y + x * 3 + 2];    // R

    	     frameImage->imageData[frameImage->widthStep * y+ x * 3] = 200;
	    frameImage->imageData[frameImage->widthStep * y + x * 3 + 1] = 200;
	    frameImage->imageData[frameImage->widthStep * y + x * 3 + 2] = 200;

		x = 161;
		y = 120;
		b1 = frameImage ->imageData[frameImage ->widthStep * y + x * 3];        // B
		g1 = frameImage ->imageData[frameImage ->widthStep * y + x * 3 + 1];    // G      
		r1 = frameImage ->imageData[frameImage ->widthStep * y + x * 3 + 2];    // R

        	    frameImage->imageData[frameImage->widthStep * y+ x * 3] = 200;
	    frameImage->imageData[frameImage->widthStep * y + x * 3 + 1] = 200;
	    frameImage->imageData[frameImage->widthStep * y + x * 3 + 2] = 200;

		x = 160;
		y = 121;

		b2 = frameImage ->imageData[frameImage ->widthStep * y + x * 3];        // B
		g2= frameImage ->imageData[frameImage ->widthStep * y + x * 3 + 1];     // G      
		r2 = frameImage ->imageData[frameImage ->widthStep * y + x * 3 + 2];    // R

       	    frameImage->imageData[frameImage->widthStep * y+ x * 3] = 200;
	    frameImage->imageData[frameImage->widthStep * y + x * 3 + 1] = 200;
	    frameImage->imageData[frameImage->widthStep * y + x * 3 + 2] = 200;


		x = 161;
		y = 121;
		b3 = frameImage ->imageData[frameImage ->widthStep * y + x * 3];        // B
		g3 = frameImage ->imageData[frameImage ->widthStep * y + x * 3 + 1];    // G      
		r3 = frameImage ->imageData[frameImage ->widthStep * y + x * 3 + 2];    // R

        	    frameImage->imageData[frameImage->widthStep * y+ x * 3] = 200;
     	    frameImage->imageData[frameImage->widthStep * y + x * 3 + 1] = 200;
	    frameImage->imageData[frameImage->widthStep * y + x * 3 + 2] = 200;

		s=(r0+g0+b0+r1+g1+b1+r2+g2+b2+r3+g3+b3)/12;

		if (s<200) {
			
			if(m==0)
				ss=1;
		
			if(ss)
				m=m+1;	
			printf("0 m= %d,d=%02X  \n",m,d);
		}
		else {
//			printf("%d \n",m);			
	
			if(ss){
				d=d+(1<<(m-1));
				m=m+1;
			}
			printf("1 m= %d,d=%02X  \n",m,d);
		
		}

		if(m>8){
		printf("コード　d= %c   \n",d);
		
			if(d==97){
				rr=0;
				gg=0;
				bb=150;
			}

			if(d==98){
				rr=150;
				gg=0;
				bb=0;
			}

		m=0;
		d=0;
		ss=0;
		}

		//	captureの入力画像フレームをframeImageに格納する
//		frameImage = cvQueryFrame( capture );
		//	frameImageをグレースケール化したものを、grayImageに格納する
		cvCvtColor( frameImage, grayImage, CV_BGR2GRAY );
		//	grayImageと背景画像との差分をとる
		cvAbsDiff( grayImage, backgroundImage, differenceImage );
		
		//	frameImageをBGRからHSVに変換する
		cvCvtColor( frameImage, hsvImage, CV_BGR2HSV );
		//	HSV画像をH、S、V画像に分ける
		cvSplit( hsvImage, hueImage, saturationImage, valueImage, NULL );
		//	明度が明るい部分を抽出、その部分のみ出力する
		cvThreshold( valueImage, thresholdImage1, THRESH_BOTTOM, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );
//		cvThreshold( hueImage, thresholdImage2, THRESH_TOP, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY_INV );
//		cvAnd( thresholdImage1, thresholdImage2, thresholdImage3, NULL );
		
		//	背景差分画像と明るい領域とのANDをとる
		cvAnd( differenceImage, thresholdImage1, lightImage, NULL );
		
		//	光っている領域の重心を算出する
		cvMoments( lightImage, &moment, 0 );
		m_00 = cvGetSpatialMoment( &moment, 0, 0 );
		m_10 = cvGetSpatialMoment( &moment, 1, 0 );
		m_01 = cvGetSpatialMoment( &moment, 0, 1 );
		gravityX = m_10 / m_00;
		gravityY = m_01 / m_00;

		if (0<gravityX){
			b = frameImage ->imageData[frameImage ->widthStep * (gravityY) + gravityX * 3 ];     // B
			g = frameImage ->imageData[frameImage ->widthStep * gravityY + gravityX * 3 + 1];    // G
			r = frameImage ->imageData[frameImage ->widthStep * gravityY + gravityX * 3 + 2];    // R

//			printf ("x= %d ,y= %d v= %d,s= %d,h= %d  \n" ,gravityX,gravityY,v,s,h);

		//	画像上に円を描画する
			if (g>200){
			cvCircle( frameImage2, cvPoint( gravityX, gravityY ), CIRCLE_RADIUS,
			 CV_RGB( rr, gg, bb ), LINE_THICKNESS, LINE_TYPE, 0 );


			}
		}

		//	画像を表示する
		cvShowImage( windowNameCapture, frameImage );
		cvShowImage( windowNameLight, lightImage );
		cvShowImage( windowNameCapture2,   frameImage2);
		cvShowImage( windowNameThreshold, thresholdImage1);

		//	キー入力判定
		key = cvWaitKey( 10 );
		if( key == 'q' ) 
			//	'q'キーが押されたらループを抜ける
			break;
		else if( key == 'b' ) {
			//	'b'キーが押されたら、その時点での画像を背景画像とする
		   frameImage = cvQueryFrame( capture );
		    cvCvtColor( frameImage, backgroundImage, CV_BGR2GRAY );
		}
		else if(key == 'c') {
			//	'c'キーが押されたら画像を保存
			cvSaveImage( "image/frame.bmp", frameImage );
			cvSaveImage( "image/light.bmp", lightImage );
		}
	}


	//	キャプチャを解放する
	cvReleaseCapture( &capture );
	//	メモリを解放する
	cvReleaseImage( &backgroundImage );
	cvReleaseImage( &grayImage );
	cvReleaseImage( &differenceImage );
	cvReleaseImage( &hsvImage );
	cvReleaseImage( &hueImage );
	cvReleaseImage( &saturationImage );
	cvReleaseImage( &valueImage );
	cvReleaseImage( &thresholdImage1 );
//	cvReleaseImage( &thresholdImage2 );
//	cvReleaseImage( &thresholdImage3 );
	cvReleaseImage( &lightImage );
	//	ウィンドウを破棄する
	cvDestroyWindow( windowNameCapture );
	cvDestroyWindow( windowNameLight );
	cvDestroyWindow( windowNameThreshold );
	cvDestroyWindow( windowNameCapture2 );

	return 0;
} 
