#include <iostream>
#include "FractalCreator.h"

using namespace std;
using namespace caveofprogramming;

int main() {

	FractalCreator fractalCreator(800, 600);

	fractalCreator.addColorRange(0.0, RGB(0,0,0));
	fractalCreator.addColorRange(0.33, RGB(255,0,0));
	fractalCreator.addColorRange(0.66, RGB(0,255,0));
	fractalCreator.addColorRange(1.0, RGB(0,0,255));


	//cout << fractalCreator.getRange(200);

	//fractalCreator.addZoom(Zoom(295, 202, 0.1));
	//fractalCreator.addZoom(Zoom(400, 290, 0.1));
	//fractalCreator.addZoom(Zoom(400, 290, 0.1));
	//fractalCreator.addZoom(Zoom(140, 70, 0.1));
	//fractalCreator.addZoom(Zoom(400, 300, 0.1));
	//fractalCreator.addZoom(Zoom(400, 300, 0.1));
	//fractalCreator.addZoom(Zoom(150, 200, 0.1));
	//fractalCreator.addZoom(Zoom(400, 300, 0.1));
	//fractalCreator.addZoom(Zoom(400, 300, 0.1));
	//fractalCreator.addZoom(Zoom(400, 300, 0.1));
	//fractalCreator.addZoom(Zoom(100, 100, 0.1));
	//fractalCreator.addZoom(Zoom(370, 290, 0.1));

	//fractalCreator.addZoom(Zoom(300, 300, 0.1));
	fractalCreator.run("test.bmp");

	cout << "Finished." << endl;
	return 0;
}
