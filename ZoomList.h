#ifndef ZOOMLIST_H_
#define ZOOMLIST_H_

#include <vector>
#include <utility>
#include "Zoom.h"

using namespace std;

namespace caveofprogramming {

class ZoomList {
private:
	double m_xCenter{0};
	double m_yCenter{0};
	double m_scale{1.0};

	int m_width{0};
	int m_height{0};
	vector<Zoom> zooms;

public:
	ZoomList(int width, int height);
	void add(const Zoom& zoom);
	pair<double, double> doZoom(int x, int y);
};

ZoomList::ZoomList(int width, int height) :
		m_width(width), m_height(height) {
	// TODO Auto-generated constructor stub

}

void ZoomList::add(const Zoom& zoom) {
	zooms.push_back(zoom);

	m_xCenter += (zoom.x - m_width / 2) * m_scale;
	m_yCenter += -(zoom.y - m_height / 2) * m_scale;

	m_scale *= zoom.scale;
}

pair<double, double> ZoomList::doZoom(int x, int y) {
	double xFractal = (x - m_width / 2) * m_scale + m_xCenter;
	double yFractal = (y - m_height / 2) * m_scale + m_yCenter;

	return pair<double, double>(xFractal, yFractal);
}

} /* namespace caveofprogramming */

#endif /* ZOOMLIST_H_ */
