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
/// �������� �������� shape �������� �� � ��������.
/// </summary>
/// <typeparam name="OutputIterator">
/// ������� ��������� ���� Output.
/// </typeparam>
/// <param name="A">
/// Aplha shape.
/// </param>
/// <param name="out">
/// �������� ����������, ������� ����� ���������.
/// </param>
template <class OutputIterator>
void alpha_edges(const Alpha_shape_2& A, OutputIterator out);

/// <summary>
/// ���������� �����������
/// </summary>
/// <param name="picture">�������� �����������</param>
void binarize(cv::Mat& picture);

/// <summary>
/// ������� ������ ���������
/// </summary>
/// <param name="picture">�������������� �����������</param>
/// <returns>������ �������� �������</returns>
std::vector<Segment> get_boundary(const cv::Mat& picture);