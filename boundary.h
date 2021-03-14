#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Alpha_shape_2.h>
#include <CGAL/Alpha_shape_vertex_base_2.h>
#include <CGAL/Alpha_shape_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/algorithm.h>
#include <CGAL/assertions.h>
#include<opencv2/opencv.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef K::FT                                                FT;
typedef K::Point_2                                           Point;
typedef K::Segment_2                                         Segment;
typedef CGAL::Alpha_shape_vertex_base_2<K>                   Vb;
typedef CGAL::Alpha_shape_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb>          Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                Triangulation_2;
typedef CGAL::Alpha_shape_2<Triangulation_2>                 Alpha_shape_2;
typedef Alpha_shape_2::Alpha_shape_edges_iterator            Alpha_shape_edges_iterator;

/// <summary>
/// Проходит элементы shape добавляя их в конейнер.
/// </summary>
/// <typeparam name="OutputIterator">
/// Шдаблон итератора типа Output.
/// </typeparam>
/// <param name="A">
/// Aplha shape.
/// </param>
/// <param name="out">
/// Итератор контейнера, который нужно заполнить.
/// </param>
template <class OutputIterator>
void alpha_edges(const Alpha_shape_2& A, OutputIterator out);

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