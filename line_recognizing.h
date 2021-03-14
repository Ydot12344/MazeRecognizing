#pragma once

#include<opencv2/opencv.hpp>
#include<queue>
#include<vector>
#include"boundary.h"

/// <summary>
/// ���������� ������� ���������
/// ���������� ����� ����� ���������.
/// </summary>
/// <param name="x">
/// ������ ������.
/// </param>
/// <param name="y">
/// ������ ������.
/// </param>
/// <returns>
/// ������� ����������.
/// </returns>
int len(cv::Vec3b x, cv::Vec3b y);

/// <summary>
/// �������� ���� ������� ���������.
/// </summary>
/// <param name="start">
/// ����� ������������� boundary ���������.
/// </param>
/// <param name="grim">
/// �����-����� �����������.
/// </param>
/// <param name="clim">
/// ������� �����������.
/// </param>
/// <returns>
/// ���� ������� ���������.
/// </returns>
cv::Vec3b get_side_color(cv::Mat& grim, cv::Mat& clim, std::vector<Segment>& boundary);

/// <summary>
/// ������� ������ ����� ������� �� ���������.
/// </summary>
/// <param name="color_img">
/// ������� �����������.
/// </param>
/// <param name="gray_img">
/// �����-����� �����������.>
/// </param>
/// <param name="start">
/// ����� ������ ���������.
/// </param>
/// <param name="fl_cl">
/// ���� ���� ���������.
/// </param>
/// <param name="s_cl">
/// ���� ������� ���������.
/// </param>
/// <returns>
/// ����� ������ ����� �������.
/// </returns>
cv::Point find_line(cv::Mat& color_img, cv::Mat& gray_img, cv::Point start, cv::Vec3b fl_cl, cv::Vec3b s_cl, int fl_sh,int cl_sh);

/// <summary>
/// �������� ���� ���� ���������.
/// </summary>
/// <param name="start">
/// ����� ������ ���������.
/// </param>
/// <param name="grim">
/// �����-����� �����������.
/// </param>
/// <param name="clim">
/// ������� �����������.
/// </param>
/// <returns>
/// ���� ���� ���������.
/// </returns>
cv::Vec3b get_floor_color(cv::Point start, cv::Mat& grim, cv::Mat& clim);

cv::Point get_end_of_line(cv::Point start, cv::Mat& clim, int shift);

std::vector<cv::Point> GetTrace(cv::Point start, cv::Point end, cv::Mat& clim, cv::Mat& grim, int shift);