#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;

struct Centers
{
	Centers(){};
	int x, y;
};

class Markers
{
private:
	Scalar min, max;
	Mat mask;
	RotatedRect rot_rect;
	string string_x, string_y, coord;
	Moments oMoments;
	double dM01;
	double dM10;
	double dArea;
public:
	float min_dist = 1000;
	string name;
	int x, y;
	Point center;
	vector<vector<Point>> contours;
	Markers(Scalar mincolor, Scalar maxcolor, string name_color)
	{
		name = name_color;
		min = mincolor;
		max = maxcolor;
	}
	void do_mask(Mat target)
	{
		inRange(target, min, max, mask);
	}
	void find_contours()
	{
		findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	}
	void do_contours(Mat background)
	{
		vector<vector<Point> >::iterator itc = contours.begin();
		while (itc != contours.end())
		{
			if (itc->size() < 50)
			{
				itc = contours.erase(itc);
			}
			else
			{
				++itc;
			}
		}
		drawContours(background, contours, -1, (0, 255, 0), 1);
	}
	void moment(Mat background)
	{

		imshow("mask", mask);
		oMoments = moments(mask);

		dM01 = oMoments.m01;
		dM10 = oMoments.m10;
		dArea = oMoments.m00;
	}
	void find_center(Centers color)
	{
		center.x = dM10 / dArea;
		center.y = dM01 / dArea;
	}
	void put_center(Mat background)
	{
		circle(background, center, 1, (0, 255, 0), 2);
	}
	void put_text(Mat background)
		{
		string_x = to_string(center.x);
		string_y = to_string(center.y);
		coord = "(" + string_x + "," + string_y + ")";
		center.x += 10;
		center.y -= 10;
		putText(background, coord, center, FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0));
		center.x -= 10;
		center.y += 10;
	}
};



int main()
{
	Scalar yellowmax = Scalar(32, 173, 255);
	Scalar yellowmin = Scalar(0, 101, 213);
	Scalar redmax = Scalar(179, 158, 208);
	Scalar redmin = Scalar(153, 111, 134);
	Scalar greenmax = Scalar(82, 146, 255);
	Scalar greenmin = Scalar(61, 54, 132);
	Scalar bluemax = Scalar(179, 255, 255);
	Scalar bluemin = Scalar(95, 161, 165);
	Scalar purplemax = Scalar(175, 83, 186);
	Scalar purplemin = Scalar(125, 59, 151);
	Scalar con_greenmax = Scalar(51, 108, 213);
	Scalar con_greenmin = Scalar(37, 35, 160);

	VideoCapture cap(0);

	Centers yellow_center, red_center, green_center, blue_center, purple_center;
	vector<Centers> centers = { red_center, yellow_center, green_center, blue_center, purple_center };

	vector<int> yellow_min, green_min, blue_min, purple_min;
	int tmp = 0;
	Markers yellow(yellowmin, yellowmax, "yellow"), red(redmin, redmax, "red"), green(greenmin, greenmax, "green"), blue(bluemin, bluemax, "blue"), purple(purplemin, purplemax, "purple"), con_green(con_greenmin, con_greenmax, "con_green");
	vector<Markers> mas = { red, yellow, green, blue, purple };
	
	int k = 0;
	int flag = 0;
	float min_distance(1500.0);
	while (1)
	{
		flag++;
		Mat video;
		cap >> video;
		imshow("Video", video);
		Mat background;
		float dist;
		video.copyTo(background);
		cvtColor(video, video, COLOR_BGR2HSV);
		for (int i = 0; i < size(mas); i++)
		{
		
			mas[i].do_mask(video);
			mas[i].find_contours();
			mas[i].do_contours(background);
			mas[i].moment(background);
			mas[i].find_center(centers[i]);
			mas[i].put_center(background);
			mas[i].put_text(background);
		
			if (i != 0)
			{
				dist = sqrt(pow(red.center.x - mas[i].center.x, 2) + (pow(red.center.y - mas[i].center.y, 2)));
				if (dist < mas[i].min_dist)
					mas[i].min_dist = dist;
				if (dist < min_distance)
					min_distance = dist;
			}
			
		}
		imshow("background", background);
		char c = waitKey(33);
		if (c == 27)
		{
			destroyWindow("Video");
			break;
		}

	}
	for (int i = 1; i < size(mas); i++)
	{
		cout << "minimal distance by " << mas[i].name << ": " << mas[i].min_dist << endl;
	}
	waitKey(0);
	return 0;
}