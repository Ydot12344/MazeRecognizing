#pragma once

#include<opencv2/opencv.hpp>
#include<queue>
#include<vector>
#include"boundary.h"

/// <summary>
/// Возвращает квадрат евклидова
/// расстояния между двумя векторами.
/// </summary>
/// <param name="x">
/// Первый вектор.
/// </param>
/// <param name="y">
/// Второй вектор.
/// </param>
/// <returns>
/// Квадрат расстояния.
/// </returns>
int len(cv::Vec3b x, cv::Vec3b y);

/// <summary>
/// Получает цвет границы лабиринта.
/// </summary>
/// <param name="start">
/// Точка принадлежащая boundary лабиринта.
/// </param>
/// <param name="grim">
/// Черно-белое изображение.
/// </param>
/// <param name="clim">
/// Цветное изображение.
/// </param>
/// <returns>
/// Цвет границы лабиринта.
/// </returns>
cv::Vec3b get_side_color(cv::Mat& grim, cv::Mat& clim, std::vector<Segment>& boundary);

/// <summary>
/// Находит начало линии ребенка на лабиринте.
/// </summary>
/// <param name="color_img">
/// Цветное изображение.
/// </param>
/// <param name="gray_img">
/// Черно-белое изображение.>
/// </param>
/// <param name="start">
/// Точка начала лабиринта.
/// </param>
/// <param name="fl_cl">
/// Цвет пола лабиринта.
/// </param>
/// <param name="s_cl">
/// Цвет границы лабиринта.
/// </param>
/// <returns>
/// Точка начала линии ребенка.
/// </returns>
cv::Point find_line(cv::Mat& color_img, cv::Mat& gray_img, cv::Point start, cv::Vec3b fl_cl, cv::Vec3b s_cl, int fl_sh,int cl_sh);

/// <summary>
/// Получает цвет пола лабиринта.
/// </summary>
/// <param name="start">
/// Точка начала лабиринта.
/// </param>
/// <param name="grim">
/// Черно-белое изображение.
/// </param>
/// <param name="clim">
/// Цветное изображение.
/// </param>
/// <returns>
/// Цвет пола лабиринта.
/// </returns>
cv::Vec3b get_floor_color(cv::Point start, cv::Mat& grim, cv::Mat& clim);

/// <summary>
/// Находит точку концы линии пользователя
/// </summary>
/// <param name="start">
/// Точка начала линии пользователя
/// </param>
/// <param name="clim">
/// Цветное изображение
/// </param>
/// <returns>
/// Точка конца линии пользователя
/// </returns>
cv::Point get_end_of_line(cv::Point start, cv::Mat& clim);

/// <summary>
/// Расчитывает путь до конца лабиринта
/// </summary>
/// <param name="start">
/// Точка старта
/// </param>
/// <param name="end">
/// Точка конца лабиринта
/// </param>
/// <param name="clim">
/// Цветная матрица лабиринта
/// </param>
/// <param name="grim">
/// Черно-белая матрица лабиринта
/// </param>
/// <returns>
/// Вектор точек пути от старта до конца
/// лабиринта
/// </returns>
std::vector<cv::Point> GetTrace(cv::Point start, cv::Point end, cv::Mat& clim, cv::Mat& grim);