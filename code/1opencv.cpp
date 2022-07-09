#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>

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
	float x, y;
	string string_x, string_y, coord;
public:
	Point center;
	vector<vector<Point>> contours;
	Markers(Scalar mincolor, Scalar maxcolor)
	{
		min = mincolor;
		max = maxcolor;
	}
	void do_mask(Mat target)
	{
		inRange(target, min, max, mask);
	}
	void find_contours()
	{
		return findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	}
	void do_contours(Mat background)
	{
		drawContours(background, contours, -1, (0, 255, 0), 1);
	}
	void min_area()
	{
		rot_rect = minAreaRect(contours[0]);
	}
	void find_center()
	{
		center = rot_rect.center;
	}
	void put_center(Mat background)
	{
		return putText(background, ".", center, FONT_HERSHEY_DUPLEX, 1, (0, 255, 0));
	}
	void put_text(Mat background)
	{
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
		center.x -= 10;
		center.y += 10;
	}
};



int main()
{
	Scalar yellowmax = Scalar(35, 255, 255);
	Scalar yellowmin = Scalar(23, 100, 150);
	Scalar redmax = Scalar(184, 255, 255);
	Scalar redmin = Scalar(170, 100, 150);
	Scalar greenmax = Scalar(70, 255, 255);
	Scalar greenmin = Scalar(55, 100, 150);
	Scalar bluemax = Scalar(125, 255, 255);
	Scalar bluemin = Scalar(95, 100, 150);

	Mat target = imread("C:/Users/Volch/OneDrive/Рабочий стол/Alex/1.jpg");
	Mat background;
	target.copyTo(background);
	cvtColor(target, target, COLOR_BGR2HSV);

	Centers yellow_center, red_center, green_center, blue_center;
	vector<Centers> centers = { yellow_center, red_center, green_center, blue_center };

	Markers yellow(yellowmin, yellowmax), red(redmin,redmax), green(greenmin,greenmax), blue(bluemin, bluemax);
	vector<Markers> mas = { yellow, red, green };
	for (int i = 0; i < size(mas); i++)
	{
		mas[i].do_mask(target);
		mas[i].find_contours();
		mas[i].do_contours(background);
		mas[i].min_area();
		mas[i].find_center();
		mas[i].put_center(background);
		mas[i].put_text(background);
		centers[i].x = mas[i].center.x;
		cout << centers[i].x << endl;
		centers[i].y = mas[i].center.y;
		cout << centers[i].y << endl;
	}
	
	
	imshow("original", background);

	waitKey(0);

	return 0;
}