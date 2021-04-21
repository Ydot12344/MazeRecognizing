#include"line_recognizing.h"
#include<algorithm>
#include"boundary.h"

bool comp(cv::Vec3b a, cv::Vec3b b) {
    int black_v_max = 95;
    int white_s_max = 20;
    int white_v_min = 240;

    int h_a, s_a, v_a, h_b, s_b, v_b;
    h_a = a[0];
    s_a = a[1];
    v_a = a[2];

    h_b = b[0];
    s_b = b[1];
    v_b = b[2];

    if (s_a < white_s_max && v_a > white_v_min && s_b < white_s_max && v_b > white_v_min)
        return true;
    else if (v_a < black_v_max && v_b < black_v_max)
        return true;
    else if (!((s_a < white_s_max && v_a > white_v_min) || (s_b < white_s_max && v_b > white_v_min) || (v_a < black_v_max) || (v_b < black_v_max)))
        return std::min(std::abs(h_a - h_b), 180 - std::abs(h_a - h_b)) < 40;
    else
        return false;
}

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
int len(cv::Vec3b x, cv::Vec3b y) {
    return ((int)x[0] - (int)y[0]) * ((int)x[0] - (int)y[0]) + ((int)x[1] - (int)y[1]) * ((int)x[1] - (int)y[1]) +
           ((int)x[2] - (int)y[2]) * ((int)x[2] - (int)y[2]);
}

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
cv::Vec3b get_side_color(cv::Mat& grim, cv::Mat& clim, std::vector<Segment>& boundary) {
    long long h = 0, s = 0, v = 0;
    cv::Vec3b res;
    long long cnt = 0;
    for (const auto& c : boundary) {
        cv::Point start = c.source;
        if (start.x - 1 >= 0 && (int)grim.at<uchar>(cv::Point(start.x - 1, start.y)) == 0) {
            cv::Vec3b tmp = clim.at<cv::Vec3b>(cv::Point(start.x - 1, start.y));
            h += tmp[0];
            s += tmp[1];
            v += tmp[2];
            cnt++;
        }
        else if (start.y - 1 >= 0 && (int)grim.at<uchar>(cv::Point(start.x, start.y - 1)) == 0) {
            cv::Vec3b tmp = clim.at<cv::Vec3b>(cv::Point(start.x, start.y - 1));
            h += tmp[0];
            s += tmp[1];
            v += tmp[2];
            cnt++;
        }
        else if (start.x + 1 < grim.cols && (int)grim.at<uchar>(cv::Point(start.x + 1, start.y)) == 0) {
            cv::Vec3b tmp = clim.at<cv::Vec3b>(cv::Point(start.x + 1, start.y));
            h += tmp[0];
            s += tmp[1];
            v += tmp[2];
            cnt++;
        }
        else if (start.y + 1 < grim.rows && (int)grim.at<uchar>(cv::Point(start.x, start.y + 1)) == 0) {
            cv::Vec3b tmp = clim.at<cv::Vec3b>(cv::Point(start.x, start.y + 1));
            h += tmp[0];
            s += tmp[1];
            v += tmp[2];
            cnt++;
        }
    }

    res[0] = h / cnt;
    res[1] = s / cnt;
    res[2] = v / cnt;

    return res;
}

// Массив расстояний
short d[2000][2000];

/// <summary>
/// Находит начало линии ребенка на лабиринте.
/// </summary>
/// <param name="color_img">
/// Цветное изображение.
/// </param>
/// <param name="gray_img">
/// Черно-белое изображение.
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
cv::Point find_line(cv::Mat& color_img, cv::Mat& gray_img, cv::Point start, cv::Vec3b fl_cl, cv::Vec3b s_cl, int fl_shift, int cl_shift) {
    int n = gray_img.cols;
    int m = gray_img.rows;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            d[i][j] = -1;
    d[start.x][start.y] = 0;
    std::queue<cv::Point> points;

    points.push(start);

    while (!points.empty()) {
        cv::Point tmp = points.front();
        points.pop();
        uchar p = gray_img.at<uchar>({ tmp.x, tmp.y });

        cv::Vec3b cl = color_img.at<cv::Vec3b>(cv::Point(tmp.x, tmp.y));
        if (!comp(cl, fl_cl) && !comp(cl, s_cl))
            return tmp;

        if ((int)p == 0) {
            continue;
        }
        if (tmp.x - 1 >= 0 && d[tmp.x - 1][tmp.y] == -1) {
            d[tmp.x - 1][tmp.y] = d[tmp.x][tmp.y] + 1;
            points.push({ tmp.x - 1, tmp.y });
        }
        if (tmp.y - 1 >= 0 && d[tmp.x][tmp.y - 1] == -1) {
            d[tmp.x][tmp.y - 1] = d[tmp.x][tmp.y] + 1;
            points.push({ tmp.x, tmp.y - 1 });
        }
        if (tmp.x + 1 < gray_img.cols && d[tmp.x + 1][tmp.y] == -1) {
            d[tmp.x + 1][tmp.y] = d[tmp.x][tmp.y] + 1;
            points.push({ tmp.x + 1, tmp.y });
        }
        if (tmp.y + 1 < gray_img.rows && d[tmp.x][tmp.y + 1] == -1) {
            d[tmp.x][tmp.y + 1] = d[tmp.x][tmp.y] + 1;
            points.push({ tmp.x, tmp.y + 1 });
        }
    }
    return { 0,0 };
}



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
cv::Vec3b get_floor_color(cv::Point start, cv::Mat& grim, cv::Mat& clim) {
    std::queue<cv::Point> q;
    q.push(start);

    for (int i = 0; i < grim.cols; i++)
        for (int j = 0; j < grim.rows; j++)
            d[i][j] = -1;

    while (!q.empty()) {
        cv::Point tmp = q.front();
        q.pop();

        d[tmp.x][tmp.y] = 0;
        if ((int)grim.at<uchar>(tmp) == 255)
            return clim.at<cv::Vec3b>(tmp);

        if (tmp.x > 0 && d[tmp.x - 1][tmp.y] == -1)
            q.push({ tmp.x - 1, tmp.y });
        if (tmp.y > 0 && d[tmp.x][tmp.y - 1] == -1)
            q.push({ tmp.x, tmp.y - 1 });
        if (tmp.x < grim.cols - 1 && d[tmp.x + 1][tmp.y] == -1)
            q.push({ tmp.x + 1, tmp.y });
        if (tmp.y < grim.rows - 1 && d[tmp.x][tmp.y + 1] == -1)
            q.push({ tmp.x, tmp.y + 1 });
    }
}

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
cv::Point get_end_of_line(cv::Point start, cv::Mat& clim) {
    int x_max = start.x, y_max = start.y;

    for (int i = 0; i < clim.cols; i++)
        for (int j = 0; j < clim.rows; j++)
            d[i][j] = -1;
    d[start.x][start.y] = 0;
    std::queue<cv::Point> q;
    q.push(start);
    auto line_col = clim.at<cv::Vec3b>(start);

    while (!q.empty()) {
        cv::Point p = q.front();
        q.pop();
        if (d[p.x][p.y] > d[x_max][y_max]) {
            x_max = p.x;
            y_max = p.y;
        }
        auto cur_col = [&clim](int x, int y) {return clim.at<cv::Vec3b>({ x,y }); };
        if (p.x - 1 >= 0 && d[p.x - 1][p.y] == -1 && comp(cur_col(p.x - 1, p.y), line_col)) {
            q.push({ p.x - 1, p.y });
            d[p.x - 1][p.y] = d[p.x][p.y] + 1;
        }
        if (p.y - 1 >= 0 && d[p.x][p.y - 1] == -1 && comp(cur_col(p.x, p.y - 1), line_col)) {
            q.push({ p.x, p.y - 1 });
            d[p.x][p.y - 1] = d[p.x][p.y] + 1;
        }
        if (p.x + 1 < clim.cols && d[p.x + 1][p.y] == -1 && comp(cur_col(p.x + 1, p.y), line_col)) {
            q.push({ p.x + 1, p.y });
            d[p.x + 1][p.y] = d[p.x][p.y] + 1;
        }
        if (p.y + 1 < clim.rows && d[p.x][p.y + 1] == -1 && comp(cur_col(p.x, p.y + 1), line_col)) {
            q.push({ p.x, p.y + 1 });
            d[p.x][p.y + 1] = d[p.x][p.y] + 1;
        }
    }
    return { x_max, y_max };
}

/// <summary>
/// Рачитывает среднюю величину прохода в
/// лабиринте.
/// </summary>
/// <param name="line_color">
/// Цвет линии пользователя
/// </param>
/// <param name="clim">
/// Цветная матрица лабиринта
/// </param>
/// <param name="grim">
/// Черно-белая матрица лабиринта
/// </param>
/// <returns>
/// Средняя величина прохода
/// </returns>
int get_gap(cv::Vec3b line_color, cv::Mat& clim, cv::Mat& grim) {
    long long cnt = 0;
    long long sum = 0;

    int x = 0, y = 0;
    bool fl = false;
    while (y < clim.rows) {
        x = 0;
        while (x < clim.cols) {
            while (x < clim.cols && (int)grim.at<uchar>({ x,y }) == 0 && !comp(clim.at<cv::Vec3b>({ x,y }), line_color)) {
                fl = true;
                x++;
            }
            if (fl && x != clim.cols) {
                int tmp = 0;
                while (x < clim.cols && ((int)grim.at<uchar>({ x,y }) == 255 || comp(clim.at<cv::Vec3b>({ x,y }), line_color))) {
                    tmp++;
                    x++;
                }
                if (x != clim.cols) {
                    cnt++;
                    sum += tmp;
                }
                fl = false;
            }
            else {
                while (x < clim.cols && ((int)grim.at<uchar>({ x,y }) == 255 || comp(clim.at<cv::Vec3b>({ x,y }), line_color)))
                    x++;
            }
        }
        y++;
    }
    return sum / cnt;
}

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
std::vector<cv::Point> GetTrace(cv::Point start, cv::Point end, cv::Mat& clim, cv::Mat& grim) {
    std::vector<cv::Point> result;
    cv::Vec3d line_color = clim.at<cv::Vec3b>(start);
    for (int i = 0; i < clim.cols; i++) {
        for (int j = 0; j < clim.rows; j++) {
            d[i][j] = -1;
        }
    }
    d[start.x][start.y] = 0;
    std::queue<cv::Point> q;
    q.push(start);

    auto cur_col = [&clim](int x, int y) {return clim.at<cv::Vec3b>({ x,y }); };

    int gap = std::max(get_gap(line_color, clim, grim) / 5, 2);

    while (!q.empty()) {
        cv::Point p = q.front();
        if (p == end)
            break;
        q.pop();
        if (d[p.x][p.y] < 0)
            continue;



        int i = 1;
        while (i < gap) {
            if (p.x - i >= 0 && cv::Point{ p.x - i, p.y } == end
                || p.y - i >= 0 && cv::Point(p.x, p.y - i) == end
                || p.y + i <= clim.rows && cv::Point(p.x, p.y + i) == end
                || p.x + i <= clim.cols && cv::Point(p.x + i, p.y) == end) {
                i = gap;
                break;
            }

            if (!(p.x - i >= 0 && ((int)grim.at<uchar>({ p.x - i, p.y }) == 255 || comp(cur_col(p.x - i, p.y), line_color))))
                break;
            if (!(p.y - i >= 0 && ((int)grim.at<uchar>({ p.x, p.y - i }) == 255 || comp(cur_col(p.x, p.y - i), line_color))))
                break;
            if (!(p.y + i < clim.rows && ((int)grim.at<uchar>({ p.x, p.y + i }) == 255 || comp(cur_col(p.x, p.y + i), line_color))))
                break;
            if (!(p.x + i < clim.cols && ((int)grim.at<uchar>({ p.x + i, p.y }) == 255 || comp(cur_col(p.x + i, p.y), line_color))))
                break;
            i++;
        }
        if (i != gap)
            continue;


        if (p.x - 1 >= 0 && d[p.x - 1][p.y] == -1 && ((int)grim.at<uchar>({ p.x - 1, p.y }) == 255 || comp(cur_col(p.x - 1, p.y), line_color))) {
            d[p.x - 1][p.y] = d[p.x][p.y] + 1;
            q.push({ p.x - 1, p.y });
        }
        if (p.y - 1 >= 0 && d[p.x][p.y - 1] == -1 && ((int)grim.at<uchar>({ p.x, p.y - 1 }) == 255 || comp(cur_col(p.x, p.y - 1), line_color))) {
            d[p.x][p.y - 1] = d[p.x][p.y] + 1;
            q.push({ p.x, p.y - 1 });
        }
        if (p.y + 1 < clim.rows && d[p.x][p.y + 1] == -1 && ((int)grim.at<uchar>({ p.x, p.y + 1 }) == 255 || comp(cur_col(p.x, p.y + 1), line_color))) {
            d[p.x][p.y + 1] = d[p.x][p.y] + 1;
            q.push({ p.x, p.y + 1 });
        }
        if (p.x + 1 < clim.cols && d[p.x + 1][p.y] == -1 && ((int)grim.at<uchar>({ p.x + 1, p.y }) == 255 || comp(cur_col(p.x + 1, p.y), line_color))) {
            d[p.x + 1][p.y] = d[p.x][p.y] + 1;
            q.push({ p.x + 1, p.y });
        }
    }

    cv::Point cur = end;
    if (d[cur.x][cur.y] == -1)
        throw std::runtime_error("");
    int dir = -1;

    auto left = [&cur,&dir] {
        if (cur.x - 1 >= 0 && d[cur.x - 1][cur.y] + 1 == d[cur.x][cur.y]) {
            cur = { cur.x - 1, cur.y };
            dir = 0;
            return true;
        }
        return false;
    };
    auto up = [&cur,&dir] {
        if (cur.y - 1 >= 0 && d[cur.x][cur.y - 1] + 1 == d[cur.x][cur.y]) {
            cur = { cur.x, cur.y - 1 };
            dir = 1;
            return true;
        }
        return false;
    };
    auto right = [&dir ,&cur,&clim] {
        if (cur.x + 1 < clim.cols && d[cur.x + 1][cur.y] + 1 == d[cur.x][cur.y]) {
            cur = { cur.x + 1, cur.y };
            dir = 2;
            return true;
        }
        return false;
    };
    auto down = [&dir, &cur, &clim] {
        if (cur.y + 1 < clim.rows && d[cur.x][cur.y + 1] + 1 == d[cur.x][cur.y]) {
            cur = { cur.x, cur.y + 1 };
            dir = 3;
            return true;
        }
        return false;
    };
    std::vector<std::function<bool()>> v = { left, up, right, down };

    while (d[cur.x][cur.y] != 0) {
        result.push_back(cur);

        if (dir == -1) {
            int i = 0;
            while (i < 4 && !(v[i]()))i++;
        }
        else {
            int i = 0;
            bool fl = false;
            while (i < 4) {
                if (dir != i) {
                    if (v[i]()) {
                        fl = true;
                        break;
                    }
                }
                i++;
            }
            if (!fl)
                v[dir]();
        }

    }

    return result;
}