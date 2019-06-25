#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>


using namespace cv;
using namespace std;

Scalar olive(128, 128, 0), violet(221, 160, 221), brown(42, 42, 165);


int pixelMean(const Mat & input);
void brightnessControl(Mat & input, Mat & output, float gamma);
float data_transform(float x, float in_min, float in_max, float out_min, float out_max);


int main()
{
	Mat frame;

	//비디오 캡쳐 초기화
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cerr << "에러 - 카메라를 열 수 없습니다.\n";
		return -1;
	}


	while(1)
	{
		cap.read(frame);

		int height = frame.rows;
		int width = frame.cols;
		int size = height * width;

		Mat input = frame.clone();
		Mat output(height, width, CV_8UC3);


    Mat gray;
    cvtColor(frame, gray, CV_BGR2GRAY);

    // imshow("gray", gray);

    int Mean = pixelMean(gray);

    cout << "Mean : " << Mean << endl;


    float sign = Mean / 255.0 * 100;

		string pg;
		stringstream s;
		s << (int) sign;
		pg = s.str();
		pg += "% brightness";


		Point myPoint;
		myPoint.x = 10;
		myPoint.y = 40;
		putText(frame, pg, myPoint, 2, 1.2, olive);

		Mat C;
		// imshow("frame", frame);

		float gamma;
		if(Mean >= 80) gamma = 0.8;
		else if(Mean < 80) gamma = 1.2;

		brightnessControl(input, output, gamma);
		putText(output, "OUTPUT", myPoint, 2, 1.2, olive);

		hconcat(frame, output, C);

		imshow("output", C);

		if (waitKey(25) >= 0)
			break;
	}


	return 0;
}

int pixelMean(const Mat & input){

  int height = input.rows;
  int width = input.cols;
  int size = height * width;

  int Mean = 0;

  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
        Mean += input.at<uchar>(i, j);

    }
  }


  return Mean / size;
}

void brightnessControl(Mat & input, Mat & output, float gamma){

	const int C = 1;

// 1) RGB -> HSV 변환
// 2) V 값 조절
// 3) HSB -> RGB 변환
// 4) 표현

	int height = input.rows;
	int width = input.cols;

	Mat hsv;
	cvtColor(input, hsv, CV_BGR2HSV);
	cvtColor(input, output, CV_BGR2HSV);

	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			float num = C * pow((float) input.at<Vec3b>(y, x)[2] / 255.f, gamma) * 255 + 0.5;
			if(num > 255.0){
				num = 255;
			}
			else if(num < 0.0){
				num = 0;
			}
			output.at<Vec3b>(y, x)[2] = (int) num;

		}
	}

	cvtColor(output, output, CV_HSV2BGR);

}

float data_transform(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
