#include <iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include"boundary.h"
#include"line_recognizing.h"

#include<array>

using namespace cv;

int main()
{

	std::array<cv::Point, 5>starts = { cv::Point(255,218), cv::Point(776,164), cv::Point(256,94), cv::Point(148,216), cv::Point(412,387) };
	std::array<cv::Point, 5>ends = { cv::Point(853,555), cv::Point(431,721), cv::Point(656,750), cv::Point(146,420), cv::Point(520,680) };

	std::array<Mat, 5> imgs;
	std::string path;
	for (int i = 0; i < 5; i++) {
		if (i < 4)
			path = "D:/sample" + std::to_string(i+1) + ".png";
		else
			path = "D:/sample" + std::to_string(7) + ".png";
		imgs[i] = imread(path);
	}

	int max_col = -1;
	for (int fl_sh = 10000; fl_sh <= 10000; fl_sh+=1000) {
		int col = 0;
		for (int i = 1; i <= 5; i++) {
			try {
				cv::Point start = starts[i - 1];

				
				Mat img;
				imgs[i-1].copyTo(img);
				Mat picture;
				medianBlur(img, img, 5);
				{
					Mat a, b, c;
					std::vector<float> v1 = { -1, -1, -1 };
					std::vector<float> v2 = { -1, 9, -1 };
					a = Mat(Mat(v1).t());
					b = Mat(Mat(v2).t());
					c = Mat(Mat(v1).t());
					Mat kernel;
					kernel.push_back(a);
					kernel.push_back(b);
					kernel.push_back(c);
					filter2D(img, img, -1, kernel);
					cvtColor(img, picture, cv::COLOR_RGB2GRAY);
				}

				binarize(picture);

				std::vector<Segment> boundary = get_boundary(picture);

				long long r = 0, g = 0, b = 0;
				for (auto& c : boundary) {
					Vec3b side_color = get_side_color({ (int)c.source().x(), (int)c.source().y() }, picture, img);
					r += side_color[0];
					g += side_color[1];
					b += side_color[2];
				}

				Vec3b side_color;
				side_color[0] = r * 1.0 / boundary.size();
				side_color[1] = g * 1.0 / boundary.size();
				side_color[2] = b * 1.0 / boundary.size();

				Vec3b floor_color = get_floor_color(start, picture, img);

				imwrite("D:/sample7(transf).png", img);

				cv::Point start_line = find_line(img, picture, start, floor_color, side_color, 40000, 40000);

				cv::Point end_line = get_end_of_line(start_line, img, 5000);

				//cv::circle(picture, start_line, 3, 0, 3);
				//cv::circle(picture, end_line, 3, 0, 3);

				for (const auto& c : boundary) {
					cv::line(picture, { (int)c.source().x(), (int)c.source().y() }, { (int)c.target().x(), (int)c.target().y() }, { 0,255,0 }, 10);
				}

				std::cout << "Floor color: " << (int)floor_color[0] << "/" << (int)floor_color[1] << "/" << (int)floor_color[2] << '\n';
				std::cout << "Side color: " << (int)side_color[0] << "/" << (int)side_color[1] << "/" << (int)side_color[2] << '\n';
				std::cout << "Start_line: " << start_line.x << " " << start_line.y << '\n';
				std::cout << "End_line: " << end_line.x << " " << end_line.y << '\n';
				std::cout << '\n';

				std::vector<cv::Point> trace = GetTrace(end_line, ends[i - 1], img, picture, 5000);

				for (int i = 0; i < trace.size() - 1; i++)
					cv::line(img, trace[i], trace[i + 1], { 0,255,0 }, 2);

				imshow(std::to_string(i), img);
				cv::waitKey(0);

				col++;
			}
			catch (const std::exception& e) {

			}
		}
		if (col >= max_col) {
			max_col = col;
			std::cout << fl_sh << " " << col << '\n';
		}
	}
}
