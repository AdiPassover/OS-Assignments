#include "ConvexHull.hpp"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT "9034"
#define BUFFER_SIZE 50

using std::cout;
using std::cin;
using std::endl;

void inputPoints(vector<Point> &p) {
    cout << "Enter the x and y coordinates of the points (separated by comma): " << endl;
    float x, y;
    for (size_t i = 0; i < p.size(); i++) {
        cout << ">> ";
        if (fscanf(stdin, "%f,%f", &x, &y) != 2)
            throw std::runtime_error("Invalid input");
        p[i] = Point(x, y);
    }
}

void processCommand(vector<Point>& points, char* line) {
    if (strncmp(line,"Newgraph", 8) == 0) {
        size_t n;
        if (sscanf(line, "Newgraph %zu", &n) != 1)
            throw std::runtime_error("Invalid Newgraph command format");
        if (n == 0)
            throw std::runtime_error("You must have at least one point");

        points.clear();
        points.resize(n);
        inputPoints(points);
        cout << "New graph created" << endl;
    }
    else if (strcmp(line,"CH") == 0) {
        cout << ConvexHull::convexHullArea(points) << endl;
    }
    else if (strncmp(line,"Newpoint", 8) == 0) {
        float x, y;
        if (sscanf(line, "Newpoint %f,%f", &x, &y) != 2)
            throw std::runtime_error("Invalid coordinates format");
        points.emplace_back(x, y);
    }
    else if (strncmp(line,"Removepoint", 11) == 0) {
        float x, y;
        if (sscanf(line, "Removepoint %f,%f", &x, &y) != 2)
            throw std::runtime_error("Invalid coordinates format");

        for (unsigned int i = 0; i < points.size(); i++) {
            if (points[i].getX() == x && points[i].getY() == y) {
                points[i] = points.back();
                points.pop_back();
                break;
            }
        }
    }
    else throw std::runtime_error("Invalid command");
}

int main() {
    vector<Point> points(1);

    while (1) {
        char buffer[BUFFER_SIZE];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL && buffer[0] != '\n') {
            buffer[strcspn(buffer, "\n")] = 0;

            processCommand(points, buffer);
        }
    }

}