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

    std::set<std::pair<int, std::set<std::pair<int, int>>>> linesToDraw;  // No need for map since we just need a simple vector holding sets of pairs.

    // ************ CALCULATE SLOPE & INSERT INTO VECTOR ************
    for (int p1 = 0; p1 < std::ssize(points) - 1; p1++)                     // Point1 chosen to compare other points with
    {
        std::unordered_map<float, std::set<std::pair<int, int>>> slopes;    // Calculate slopes independently for every iteration

        for (int p2 = p1 + 1; p2 < std::ssize(points); p2++)                // Loop through rest of points to compare to Point1
        {
                                                                            // Point 1 Coordinates (non-normalized)
            float X1 = points[p1].position.x * 32767;
            float Y1 = points[p1].position.y * 32767;

                                                                            // Point 2 Coordinates (non-normalized)
            float X2 = points[p2].position.x * 32767;
            float Y2 = points[p2].position.y * 32767;

            float k;                                                        // Slope value
            if (X1 != X2)                                                   // If the denominator is not = 0
            {
                k = (Y2 - Y1) / (X2 - X1);                                  // Slope calculations
            } 
            else 
            {
                k = std::numeric_limits<float>::infinity();                 // Divided by 0 --> infinity
            }

            // If we are not comparing the same points with each other.
            if (p1 != p2) 
            {                                                 
                if (k == -0) 
                {                                              // Keep zeros positive.
                    k = 0;
                }

                // insert points in the index with same k value
                slopes[k].insert({X1, Y1});                                 // Insert first point
                slopes[k].insert({X2, Y2});                                 // Insert second point
            }
        }

        // Sort line segments depending on first point's y value
        for (auto& e : slopes) 
        {
            if (e.second.size() > 3) 
            {
                linesToDraw.insert({(*(e.second.begin())).second, std::move(e.second)});    // Inserting the y values into lineToDraw
            }
        }
    }

    // ************ CREATION OF LINES ************

    // Remove short segments
    for (auto i = linesToDraw.begin(); i != linesToDraw.end(); ++i)                         
    {
        for (auto j = linesToDraw.begin(); j != linesToDraw.end();) 
        {
            if (i != j)
            {   
                std::pair<int, int> start1 = *((*i).second.begin());                                        // Get the coordinates 
                std::pair<int, int> end1 = *(--(*i).second.end());
                std::pair<int, int> start2 = *((*j).second.begin());
                std::pair<int, int> end2 = *(--(*j).second.end());

                int slope1, slope2;
                if (start1.first != end1.first)
                {
                    slope1 = (end1.second - start1.second) / (end1.first - start1.first);  // Calculating slope 1
                }
                else
                {
                    slope1 = std::numeric_limits<float>::infinity();
                }

                if (start2.first != end2.first)
                {
                    slope2 = (end2.second - start2.second) / (end2.first - start2.first);  // Calculating slope 2
                }
                else
                {
                    slope2 = std::numeric_limits<float>::infinity();
                }

                if (slope1 == slope2 && start1.first >= start2.first && start1.second >= start2.second &&   // If the slopes are the same AND start1 and end1 is within start2 and end2
                    end1.first <= end2.first && end1.second <= end2.second) 
                {                                   
                    i = linesToDraw.erase(i);                                                               // Erase the points inbetween

                    if (i == linesToDraw.end())
                    {
                        j = linesToDraw.end();                                                              
                    }
                    else
                    {
                        j = linesToDraw.begin();
                    }
                } 
                else
                {
                    ++j;
                }
            } 
            else
            {
                ++j;
            }
        }
    }

    // Open output file for writing
    std::ofstream outputFile("../detectionsystem/data/output/segments-" + name);
    if (!outputFile) 
    {
        std::cerr << std::endl << "Unable to open file for writing: " << name << std::endl;
        return;
    }

    // Write out the coordinates to the file
    for (auto i : linesToDraw) 
    {
        std::pair<int, int> p1 = *(i.second.begin());
        std::pair<int, int> p2 = *(--i.second.end());
        outputFile << p1.first << " " << p1.second << " " << p2.first << " " << p2.second << "\n";
        
        // Write out to the console
        for (auto j : i.second) 
        {
            if (j != *(--i.second.end()))
                std::cout << "(" << j.first << "," << j.second << ")->";
            else
                std::cout << "(" << j.first << "," << j.second << ")\n";
        }
    }

    outputFile.close();

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
