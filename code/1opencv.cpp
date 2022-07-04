#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

int main()
{
	Scalar yellow = Scalar(35, 255, 255);
	Scalar yellowmin = Scalar(23, 100, 150);
	Scalar red = Scalar(184, 255, 255);
	Scalar redmin = Scalar(172, 100, 150);
	Scalar green = Scalar(70, 255, 255);
	Scalar greenmin = Scalar(55, 100, 150);

	Mat target = imread("C:/Users/Volch/OneDrive/Рабочий стол/Alex/1.jpg");

	Mat background;
	target.copyTo(background);

	cvtColor(target, target, COLOR_BGR2HSV);
	vector<vector<Point>> contours;
	Mat mask;
	inRange(target, yellowmin, yellow, mask);
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(background, contours, -1, (0, 255, 0), 1);
	RotatedRect rot_rect;
	Point center;
	rot_rect = minAreaRect(contours[0]);
	float x, y;
	string string_x, string_y, coord;
	center = rot_rect.center;
	putText(background, ".", center, FONT_HERSHEY_DUPLEX, 1, (0, 255, 0));
	x = round(center.x * 10) / 10;
	y = round(center.y * 10) / 10;
	string_x = to_string(x);
	string_y = to_string(y);
	string_x.erase(5);
	string_y.erase(5);
	coord = "(" + string_x + "," + string_y + ")";
	center.x += 10;
	center.y -= 10;
	putText(background, coord, center, FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0));
	

	inRange(target, redmin, red, mask);
	imshow("origi", mask);
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(background, contours, -1, (0, 255, 0), 1);
	rot_rect = minAreaRect(contours[0]);
	center = rot_rect.center;
	putText(background, ".", center, FONT_HERSHEY_DUPLEX, 1, (0, 255, 0));
	x = round(center.x * 10) / 10;
	y = round(center.y * 10) / 10;
	string_x = to_string(x);
	string_y = to_string(y);
	string_x.erase(5);
	string_y.erase(5);
	coord = "(" + string_x + "," + string_y + ")";
	center.x += 10;
	center.y -= 10;
	putText(background, coord, center, FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0));

	inRange(target, greenmin, green, mask);
	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	rot_rect = minAreaRect(contours[0]);
	center = rot_rect.center;
	putText(background, ".", center, FONT_HERSHEY_DUPLEX, 1, (0, 255, 0));
	x = round(center.x * 10) / 10;
	y = round(center.y * 10) / 10;
	string_x = to_string(x);
	string_y = to_string(y);
	string_x.erase(5);
	string_y.erase(5);
	coord = "(" + string_x + "," + string_y + ")";
	center.x += 10;
	center.y -= 10;
	putText(background, coord, center, FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0));

	imshow("original", background);

	waitKey(0);

	return 0;
}