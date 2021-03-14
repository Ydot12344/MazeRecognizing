#include"boundary.h"

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
void alpha_edges(const Alpha_shape_2& A, OutputIterator out)
{
	Alpha_shape_edges_iterator it = A.alpha_shape_edges_begin(),
		end = A.alpha_shape_edges_end();
	for (; it != end; ++it)
		*out++ = A.segment(*it);
}

/// <summary>
/// ���������� �����������
/// </summary>
/// <param name="picture">�������� �����������</param>
void binarize(cv::Mat& picture) {
	//medianBlur(picture, picture, 5);
	threshold(picture, picture, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
}

/// <summary>
/// ������� ������ ���������
/// </summary>
/// <param name="picture">�������������� �����������</param>
/// <returns>������ �������� �������</returns>
std::vector<Segment> get_boundary(const cv::Mat& picture) {

	std::vector<std::vector<cv::Point>> cntr;
	findContours(picture, cntr, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
	std::vector<K::Point_2> points;

	for (auto c : cntr) {
		for (auto p : c) {
			if (p.x == 0 || p.y == 0 || p.x == picture.cols - 1 || p.y == picture.rows - 1) {
				continue;
			}
			points.push_back({ p.x, p.y });
		}
	}

	Alpha_shape_2 A(points.begin(), points.end(),
		FT(10000),
		Alpha_shape_2::GENERAL);
	A.set_alpha(15000);

	std::vector<Segment> segments;
	alpha_edges(A, std::back_inserter(segments));
	return segments;
}