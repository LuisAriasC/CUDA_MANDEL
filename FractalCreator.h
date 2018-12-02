#ifndef FRACTALCREATOR_H_
#define FRACTALCREATOR_H_

#include <string>
#include <cstdint>
#include <memory>
#include <math.h>
#include <vector>
#include <iostream>
#include <assert.h>
#include <omp.h>
#include "Bitmap.h"
#include "Mandelbrot.h"
#include "Zoom.h"
#include "RGB.h"
#include "lines.h"

using namespace std;

namespace caveofprogramming {

class FractalCreator {
private:
	int m_width;
	int m_height;
	unique_ptr<int[]> m_histogram;
	unique_ptr<int[]> m_fractal;
	Zoom m_zoomList;
	int m_total{0};

	vector<int> m_ranges;
	vector<RGB> m_colors;
	vector<int> m_rangeTotals;

	bool m_bGotFirstRange{false};
	void calculateIteration();
	void calculateIterationOMP();
	void calculateTotalIterations();
	void calculateTotalIterationsOMP();
	void calculateRangeTotals();
	void calculateRangeTotalsOMP();
	int getRange(int iterations) const;

public:
	FractalCreator(int width, int height);
	virtual ~FractalCreator();
	void addColorRange(double rangeEnd, const RGB& rgb);
	void addZoom(int x, int y, double scale);
	void run();
	void drawFractal();

};

void FractalCreator::run() {
	calculateIterationOMP();
	printf("Iteraciones Calculadas\n");
	calculateTotalIterations();
	printf("Totales it Calculadas\n");
	calculateRangeTotals();
	printf("Totales Rango Calculadas\n");
	//drawFractal();
}

FractalCreator::FractalCreator(int width, int height) :
		m_width(width), m_height(height), m_histogram(
				new int[Mandelbrot::MAX_ITERATIONS] { 0 }), m_fractal(
				new int[m_width * m_height] { 0 }), m_zoomList(
				m_width, m_height) {

	m_zoomList.add(m_width / 2, m_height / 2, 4.0 / m_width);
}

FractalCreator::~FractalCreator() {
}

void FractalCreator::calculateIteration() {
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			pair<double, double> coords = m_zoomList.doZoom(x, y);

			int iterations = Mandelbrot::getIterations(coords.first,
					coords.second);

			m_fractal[y * m_width + x] = iterations;

			if (iterations != Mandelbrot::MAX_ITERATIONS) {
				m_histogram[iterations]++;
			}

		}
	}
}

void FractalCreator::calculateIterationOMP() {

	int y;

	#pragma omp parallel for private(y) shared(m_fractal,m_histogram)
	for (y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			pair<double, double> coords = m_zoomList.doZoom(x, y);

			int iterations = Mandelbrot::getIterations(coords.first,
					coords.second);

			m_fractal[y * m_width + x] = iterations;

			if (iterations != Mandelbrot::MAX_ITERATIONS) {
				#pragma omp atomic
				m_histogram[iterations]++;
			}

		}
	}
}

void FractalCreator::calculateRangeTotals(){
	int rangeIndex = 0;

	for (int i = 0; i < Mandelbrot::MAX_ITERATIONS; i++) {
		int pixels = m_histogram[i];

		if (i >= m_ranges[rangeIndex + 1])
			rangeIndex++;

		m_rangeTotals[rangeIndex] += pixels;
	}
}

void FractalCreator::calculateRangeTotalsOMP(){
	cout << "Entra" << endl;
	int rangeIndex = 0;
	int i = 0;

	#pragma omp parallel for private(i,rangeIndex) shared(m_ranges,m_rangeTotals, m_histogram)
	for (i = 0; i < Mandelbrot::MAX_ITERATIONS; i++) {
		int pixels = m_histogram[i];

		if (i >= m_ranges[rangeIndex + 1])
			rangeIndex++;

		m_rangeTotals[rangeIndex] += pixels;
	}
}

void FractalCreator::calculateTotalIterations() {
	for (int i = 0; i < Mandelbrot::MAX_ITERATIONS; i++)
		m_total += m_histogram[i];
}

void FractalCreator::drawFractal() {

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {

			int iterations = m_fractal[y * m_width + x];

			int range = getRange(iterations);
			int rangeTotal = m_rangeTotals[range];
			int rangeStart = m_ranges[range];

			RGB& startColor = m_colors[range];
			RGB& endColor = m_colors[range + 1];
			RGB colorDiff = endColor - startColor;

			float red = 0;
			float green = 0;
			float blue = 0;

			if (iterations != Mandelbrot::MAX_ITERATIONS) {

				double hue = 0.0;
				int totalPixels = 0;

				for (int i = rangeStart; i <= iterations; i++)
					totalPixels += m_histogram[i];

				red = (float)(startColor.r + colorDiff.r * (double)totalPixels/rangeTotal)/255;
				green = (float)(startColor.g + colorDiff.g * (double)totalPixels/rangeTotal)/255;
				blue = (float)(startColor.b + colorDiff.b * (double)totalPixels/rangeTotal)/255;
				//printf("%f %f %f\n",red/255, green/255, blue/255 );
			}
			plot(x,y,red,green,blue);
		}
	}
	cout << "FP" << endl;
}

void FractalCreator::addColorRange(double rangeEnd, const RGB& rgb){
	m_ranges.push_back(rangeEnd * Mandelbrot::MAX_ITERATIONS);
	m_colors.push_back(rgb);

	if (m_bGotFirstRange) {
		m_rangeTotals.push_back(0);
	}

	m_bGotFirstRange = true;
}

int FractalCreator::getRange(int iterations) const {
	int range = 0;

	for (int i = 1; i < m_ranges.size(); i++) {
		range = i;
		if (m_ranges[i] > iterations)
			break;
	}

	range --;
	assert(range > -1);
	assert(range < m_ranges.size());

	return range;
}


void FractalCreator::addZoom(int x, int y, double scale) {
	m_zoomList.add(x,y,scale);
}

} /* namespace caveofprogramming */

#endif /* FRACTALCREATOR_H_ */
