#pragma once

#include<opencv2/opencv.hpp>
#include<unordered_set>

struct Segment {
    cv::Point source;
    cv::Point destination;
    bool operator==(const Segment& a) const {
        return source == a.source && destination == a.destination;
    }
};



struct SegmentHash {
    std::hash<int> int_hash;
    std::size_t operator()(const Segment& seg) const {
        std::size_t lhs = int_hash(seg.source.x);
        std::size_t rhs = int_hash(seg.source.y);
        lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
        rhs = int_hash(seg.destination.x);
        lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
        rhs = int_hash(seg.destination.y);
        lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
        return lhs;
    }
};

std::vector<Segment> alpha_shape(std::vector<cv::Point2f> points, float alpha);


/// <summary>
/// Бинаризует изображение
/// </summary>
/// <param name="picture">Исходное изображение</param>
void binarize(cv::Mat& picture);

/// <summary>
/// Находит контур лабиринта
/// </summary>
/// <param name="picture">Бинаризованное изображение</param>
/// <returns>Вектор отрезков границы</returns>
std::vector<Segment> get_boundary(const cv::Mat& picture);

