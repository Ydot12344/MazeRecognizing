#include <iostream>
#include "main.h"
#include<vector>
#include"boundary.h"
#include"line_recognizing.h"
#include<array>

using namespace cv;

cv::Mat solve(cv::Point start, cv::Point end, cv::Mat& img) {

    Mat picture;
    medianBlur(img, img, 3);
    cvtColor(img, picture, cv::COLOR_RGB2GRAY);
    Mat img_hsv;
    cvtColor(img, img_hsv, COLOR_BGR2HSV);

    binarize(picture);

    std::vector<Segment> boundary = get_boundary(picture);

    Vec3b side_color = get_side_color(picture, img_hsv, boundary);


    Vec3b floor_color = get_floor_color(start, picture, img_hsv);

    cv::Point start_line = find_line(img_hsv, picture, start, floor_color, side_color, 15000, 15000);

    cv::Point end_line = get_end_of_line(start_line, img_hsv);

    for (const auto& c : boundary) {
        cv::line(picture, c.source, c.destination, { 0,255,0 }, 3);
    }


    std::vector<cv::Point> trace = GetTrace(end_line, end, img_hsv, picture);

    for (int i = 0; i < trace.size() - 1; i++)
        cv::line(img, trace[i], trace[i + 1], { 255,0,0 ,100}, 3);

    return img;
}


void myFlip(cv::Mat& input, cv::Mat& output){
    cv::flip(input, output, 0);
}