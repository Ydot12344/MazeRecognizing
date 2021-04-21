#include"boundary.h"

/// <summary>
/// Бинаризует изображение
/// </summary>
/// <param name="picture">Исходное изображение</param>
void binarize(cv::Mat& picture) {
    //medianBlur(picture, picture, 5);
    threshold(picture, picture, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
}

void add_edge(std::unordered_set<Segment, SegmentHash>& edges, const cv::Point& source, const cv::Point& destination) {
    if (edges.count({ destination, source }) != 0) {
        edges.erase({ destination, source });
        return;
    }
    edges.insert({ source, destination });
}

using namespace std;
using namespace cv;
std::vector<std::vector<cv::Point2f>> Triangulate(std::vector<cv::Point2f>& pts)

{

    std::vector<std::vector<cv::Point2f>> result;

    std::vector<cv::Scalar>  val;

    std::vector<int> hull;

    vector<Point2f> hull_pts;

    // Найдем выпуклую оболочку вокруг наших точек

    convexHull(pts, hull, false);


    // Перекинем точки оболочки в отдельный массив и найдем габарит этой оболочки

    // (нужно для инициализации триангулятора)

    Point2f top_left(FLT_MAX, FLT_MAX);			   // min_x min_y

    Point2f bottom_right(FLT_MIN, FLT_MIN);		   // max_x max_y


    for (int i = 0; i < hull.size(); i++)

    {

        Point2f p = pts[hull[i]];

        hull_pts.push_back(p);

        if (p.x < top_left.x) { top_left.x = p.x; }

        if (p.y < top_left.y) { top_left.y = p.y; }

        if (p.x > bottom_right.x) { bottom_right.x = p.x; }

        if (p.y > bottom_right.y) { bottom_right.y = p.y; }

    }

    // Немного раздвинем область

    top_left.x--;

    top_left.y--;

    bottom_right.x++;

    bottom_right.y++;

    // Габаритный прямоугольник наших точек, расширенный на 1 в каждую сторону

    Rect region(top_left, bottom_right);

    // Инициализируем триангулятор

    Subdiv2D subdiv(region);

    // Загружаем в него точки

    subdiv.insert(pts);


    vector<Vec6f> triangleList;

    subdiv.getTriangleList(triangleList);

    vector<Point2f> pt(3);

    for (size_t i = 0; i < triangleList.size(); i++)

    {

        Vec6f t = triangleList[i];

        //

        pt[0] = Point(t[0], t[1]);

        pt[1] = Point(t[2], t[3]);

        pt[2] = Point(t[4], t[5]);

        // нам нужна триангуляция выпуклого многогранника, а не всей области

        int p1 = pointPolygonTest(hull_pts, pt[0], false);

        int p2 = pointPolygonTest(hull_pts, pt[1], false);

        int p3 = pointPolygonTest(hull_pts, pt[2], false);

        // если точки треугольника внутри выпуклой оболчки, то запоминаем треугольник

        if (p1 > -1 && p2 > -1 && p3 > -1)

        {

            result.push_back(pt);

        }

    }


    return result;

}

std::vector<Segment> alpha_shape(std::vector<cv::Point2f> points, float alpha) {
    std::unordered_set<Segment,SegmentHash> edges;
    std::vector<vector<Point2f>> tri = Triangulate(points);
    for (auto& tr : tri) {
        cv::Point point_a = tr[0];
        cv::Point point_b = tr[1];
        cv::Point point_c = tr[2];

        float len_a = std::sqrt((point_a.x - point_b.x)* (point_a.x - point_b.x) + (point_a.y - point_b.y)* (point_a.y - point_b.y));
        float len_b = std::sqrt((point_b.x - point_c.x) * (point_b.x - point_c.x) + (point_b.y - point_c.y) * (point_b.y - point_c.y));
        float len_c = std::sqrt((point_c.x - point_a.x) * (point_c.x - point_a.x) + (point_c.y - point_a.y) * (point_c.y - point_a.y));
        float p = (len_a + len_b + len_c) / 2.f;
        float s = std::sqrt(p * (p - len_a) * (p - len_b) * (p - len_c));
        float r = len_a * len_b * len_c / (4.f * s);
        if (r < alpha) {
            add_edge(edges, point_a, point_b);
            add_edge(edges, point_b, point_c);
            add_edge(edges, point_c, point_a);
        }
    }
    std::vector<Segment> res;
    for (auto& c : edges) {
        res.push_back(c);
    }
    return res;
}

/// <summary>
/// Находит контур лабиринта
/// </summary>
/// <param name="picture">Бинаризованное изображение</param>
/// <returns>Вектор отрезков границы</returns>
std::vector<Segment> get_boundary(const cv::Mat& picture) {

    std::vector<std::vector<cv::Point>> cntr;
    findContours(picture, cntr, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    std::vector<cv::Point2f> points;

    for (auto c : cntr) {
        for (auto p : c) {
            if (p.x == 0 || p.y == 0 || p.x == picture.cols - 1 || p.y == picture.rows - 1) {
                continue;
            }
            points.push_back({ (float)p.x,(float)p.y });
        }
    }

    std::vector<Segment> segments = alpha_shape(points, 100);
    return segments;
}