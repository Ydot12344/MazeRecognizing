#include"line_recognizing.h"

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
cv::Vec3b get_side_color(cv::Point start, cv::Mat& grim, cv::Mat& clim) {
	cv::Vec3b res;
	if (start.x - 1 >= 0 && (int)grim.at<uchar>(cv::Point(start.x - 1, start.y)) == 0) {
		res = clim.at<cv::Vec3b>(cv::Point(start.x - 1, start.y));
	}
	else if (start.y - 1 >= 0 && (int)grim.at<uchar>(cv::Point(start.x, start.y - 1)) == 0) {
		res = clim.at<cv::Vec3b>(cv::Point(start.x, start.y - 1));
	}
	else if (start.x + 1 < grim.cols && (int)grim.at<uchar>(cv::Point(start.x + 1, start.y)) == 0) {
		res = clim.at<cv::Vec3b>(cv::Point(start.x + 1, start.y));
	}
	else  {
		res = clim.at<cv::Vec3b>(cv::Point(start.x, start.y + 1));
	}

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
		if (len(cl, fl_cl) > fl_shift && len(cl, s_cl) > cl_shift)
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


cv::Point get_end_of_line(cv::Point start, cv::Mat& clim, int shift) {
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
		if (p.x - 1 >= 0 && d[p.x-1][p.y] ==-1 && len(cur_col(p.x-1, p.y), line_col) < shift){
			q.push({ p.x - 1, p.y });
			d[p.x - 1][p.y] = d[p.x][p.y] + 1;
		}
		if (p.y - 1 >= 0 && d[p.x][p.y - 1] == -1 &&len(cur_col(p.x, p.y - 1), line_col) < shift) {
			q.push({ p.x, p.y - 1});
			d[p.x][p.y - 1] = d[p.x][p.y] + 1;
		}
		if (p.x + 1 < clim.cols && d[p.x + 1][p.y] == -1 && len(cur_col(p.x + 1, p.y), line_col) < shift) {
			q.push({ p.x + 1, p.y });
			d[p.x + 1][p.y] = d[p.x][p.y] + 1;
		}
		if (p.y + 1 < clim.rows && d[p.x][p.y + 1] == -1 && len(cur_col(p.x, p.y + 1), line_col) < shift) {
			q.push({ p.x, p.y + 1});
			d[p.x][p.y + 1] = d[p.x][p.y] + 1;
		}
	}
	return { x_max, y_max };
}

std::vector<cv::Point> GetTrace(cv::Point start, cv::Point end, cv::Mat& clim, cv::Mat& grim, int shift) {
	std::vector<cv::Point> result;
	cv::Vec3d line_color = clim.at<cv::Vec3b>(start);
	for (int i = 0; i < clim.cols;i++) {
		for (int j = 0; j < clim.rows;j++) {
			d[i][j] = -1;
		}
	}
	d[start.x][start.y] = 0;
	std::queue<cv::Point> q;
	q.push(start);

	auto cur_col = [&clim](int x, int y) {return clim.at<cv::Vec3b>({ x,y }); };

	while (!q.empty()) {
		cv::Point p = q.front();
		if (p == end)
			break;
		q.pop();
		if (p.x - 1 >= 0 && d[p.x - 1][p.y] == -1 && ((int)grim.at<uchar>({p.x -1, p.y}) == 255 || len(cur_col(p.x-1,p.y), line_color) < 400)) {
			d[p.x - 1][p.y] = d[p.x][p.y] + 1;
			q.push({ p.x - 1, p.y });
		}
		if (p.y  - 1 >= 0 && d[p.x][p.y - 1] == -1 && ((int)grim.at<uchar>({ p.x, p.y - 1}) == 255 || len(cur_col(p.x, p.y - 1), line_color) < 400)) {
			d[p.x][p.y - 1] = d[p.x][p.y] + 1;
			q.push({ p.x, p.y - 1});
		}
		if (p.y + 1 < clim.rows && d[p.x][p.y + 1] == -1 && ((int)grim.at<uchar>({ p.x, p.y  + 1}) == 255 || len(cur_col(p.x, p.y + 1), line_color) < 400)) {
			d[p.x][p.y + 1] = d[p.x][p.y] + 1;
			q.push({ p.x, p.y + 1 });
		}
		if (p.x + 1 < clim.cols && d[p.x + 1][p.y] == -1 && ((int)grim.at<uchar>({ p.x + 1, p.y }) == 255 || len(cur_col(p.x + 1, p.y), line_color) < 400)) {
			d[p.x + 1][p.y] = d[p.x][p.y] + 1;
			q.push({ p.x + 1, p.y });
		}
	}



	cv::Point cur = end;
	if (d[cur.x][cur.y] == -1)
		throw std::runtime_error("");
	while (d[cur.x][cur.y] != 0) {
		result.push_back(cur);
		if (cur.x - 1 >= 0 && d[cur.x - 1][cur.y] + 1 == d[cur.x][cur.y])
			cur = { cur.x - 1, cur.y };
		else if (cur.y - 1 >= 0 && d[cur.x][cur.y - 1] + 1 == d[cur.x][cur.y])
			cur = { cur.x, cur.y - 1 };
		else if(cur.x + 1 < clim.cols && d[cur.x + 1][cur.y] + 1 == d[cur.x][cur.y])
			cur = { cur.x + 1, cur.y };
		else if (cur.y + 1 < clim.rows && d[cur.x ][cur.y + 1] + 1 == d[cur.x][cur.y])
			cur = { cur.x, cur.y + 1};
	}

	return result;
}