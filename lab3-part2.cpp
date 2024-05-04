#include <linesdiscoverysystem/readfiles.h>

#include <vector>
#include <string>
#include <filesystem>

#include <rendering/window.h>
#include <fmt/format.h>

#include <unordered_map>
#include <set>
#include <fstream>

void plotData(const std::string& name);

/* ************************************* */

struct Point 
{
    double x = 0.0;
    double y = 0.0;
};


/* ****************** MAIN ****************** */

int main() try {    
    std::cout << "Enter the name of input points file: ";
    std::string s;
    std::cin >> s;  // e.g. points1.txt, points200.txt, largeMystery.txt

    plotData(s);
} catch (const std::exception& e) {
    fmt::print("Error: {}", e.what());
    return 1;
}

/* ***************** FUNCTIONS ******************** */

double CalcSlope(Point& start, Point& end) 
{ 
    return ((end.x - start.x)/(end.y-start.y));
}

void plotData(const std::string& name) {
    
    // Reads in all points and store them in a vector called points
    std::filesystem::path points_name = name;
    const auto points = readPoints(data_dir / points_name);


    /********************************************************************************
    *   unordered_map:  Container that contains key - value pairs with unique keys  *
    *                   Search, Insertion and Removal = O(1)                        *
    *                                                                               *
    *   set:            Stores unique elements, automatically sorted (ascending)    *
    *                   Search, Insertion and Removal = O(log n)                    *
    *                                                                               *
    *   pair:           class for points, holds (x,y)                               *
    *********************************************************************************/

    std::unordered_map<float, std::set<std::pair<int, int>>> slopes;

    // ************ CALCULATE SLOPE & INSERT INTO VECTOR ************
    for (int p1 = 0; p1 < std::ssize(points) - 1; p1++)         // Point1 chosen to compare other points with
    {
        for (int p2 = p1 + 1; p2 < std::ssize(points); p2++)    // Loop through rest of points to compare to Point1
        {
            // Point 1 Coordinates (non-normalized)
            float X1 = points[p1].position.x * 32767;
            float Y1 = points[p1].position.y * 32767;

            // Point 2 Coordinates (non-normalized)
            float X2 = points[p2].position.x * 32767;
            float Y2 = points[p2].position.y * 32767;
        
            float k;        // Slope value
            if (X1 != X2)   // If the denominator is not = 0
            {
                k = (Y2 - Y1) / (X2 - X1);
            }
            else
            {
                k = std::numeric_limits<float>::infinity();
            }

            // If we are not comparing the same points with each other.
            if (p1 != p2)
            {
                if (k == -0) { k = 0; }

                // insert points in the index with same k value
                slopes[k].insert({X1, Y1});
                slopes[k].insert({X2, Y2});
            }
        }
    }

    // ************ CREATION OF LINES ************
    
    std::set<std::pair<int, std::set<std::pair<int, int>>>> linesToDraw;  // No need for map since we just need a simple vector holding sets of pairs.
    
    for (auto& k : slopes) 
    {
        // ------------- PRINTS OUT ALL K VALUES AND HOW MANY POINTS ALONG THE LINES (DEBUG PURPOSES) ---------------

        //if (k.second.size() > 3)  // If it has more than 3 points
        //{
        //    // Write out the first and last point into a txt file.
        //    std::cout   << std::left << std::setw(10) << "|| K value = " << std::setw(10) << k.first 
        //                << std::setw(20) << " || Points along the same k value = " << std::setw(3) << k.second.size() << "|| \n";
        //}
        //else
        //{
        //    std::cout   << std::left << std::setw(10) << "|| K value = " << std::setw(10) << k.first 
        //                << std::setw(20) << " || Points along the same k value = " << std::setw(3) << k.second.size() << "|| \n";
        //}

        // Find a K value with < 3 points
        if (k.second.size() > 3) 
        {
            // Insert the first and the last into lines.
            
        }
    }


    /*******************************************/

    std::filesystem::path segments_name = "segments-" + name;
    const auto lines = readLineSegments(data_dir / "output" / segments_name);

    rendering::Window window(850, 850, rendering::Window::UseVSync::Yes);
    while (!window.shouldClose()) {
        window.beginFrame();
        window.clear({0, 0, 0, 1});
        window.drawLines(lines);    // to plot the line segments discovered
        window.drawPoints(points);  // to plot input points
        window.endFrame();
    }
}
/* ************************************************ */
