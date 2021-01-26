#include <iostream>
#include <cmath>
#include <string>

using namespace std;

/**
 *  High precision PI number
 */
const float PI = 3.141592653589793238462643383279502884;

/*
 * Shape names (en-EN)
 */
const string ShapeNames[] = { "Triangle", "Quadrilateral",  "Pentagon", "Hexagon", "Heptagon", "Octagon", "Nonagon" };

/**
 * Converts radian value to degree value.
 * @param radians  Radian value to convert
 * @return Degree value of 'radians'
 */
float RadianToDegree(float radians)
{
    return radians * (180 / PI);
}

/**
 *
 */
struct Point2D
{
    float x;
    float y;
};

// Vector 2D structure
struct Vector2D
{
    Point2D Start;
    Point2D End;
};

// Vectoral multiplication
float DotProduct(Point2D *a, Point2D *b)
{
    return (a->x * b->x) + (a->y * b->y);
}

/*
 * Calculates the length of the given *Point2D* (by origin)
 */
float LengthOfPoint(Point2D *src)
{
    return sqrtf(src->x * src->x + src->y * src->y);
}

/**
 * Calculates the angle between 2 vectors
 * @param a The first vector
 * @param b The second vector
 * @returns Angle between given 2 vectors
 */
float AngleBetweenVectors(Vector2D a, Vector2D b)
{
    Point2D pa;
    pa.x = a.End.x - a.Start.x;
    pa.y = a.End.y - a.Start.y;

    Point2D pb{};
    pb.x = b.Start.x - b.End.x;
    pb.y = b.Start.y - b.End.y;

    return RadianToDegree(acosf(DotProduct(&pa, &pb) / (LengthOfPoint(&pa) * LengthOfPoint(&pb))));
}

/**
 * Calculates the length of the given vector
 * @param v The vector to calculate the length
 * @returns The length of the given vector
 */
float LengthOfEdge(Vector2D *v)
{
    return sqrtf((v->End.x - v->Start.x)*(v->End.x - v->Start.x) + (v->End.y - v->Start.y)*(v->End.y - v->Start.y));
}

int main() {
    // Clear the console
    system("clear");
    // Initialize the pointCount
    int pointCount = 0;

    cout << "Enter point count (3-9): ";
    cin >> pointCount;
    // Only working for shapes that has 3-9 points
    if(pointCount > 9 || pointCount < 3) return -1;

    // Clear the console again
    system("clear");
    // Inform the user about the processes that will be done
    cout << "You will be prompted to enter coordinates of points. Each point will be connected with-\nthe next point you entered and the last point will be connected to the first point.\n" << endl;

    // Initialize a new array of Point2D's
    auto *points = new Point2D[pointCount]();
    // Initialize a new array of Vector2D's (same count as points)
    auto *edges = new Vector2D[pointCount]();

    // Get all the points from the user input and generate edges from them
    for (int i = 0; i < pointCount; ++i) {
        // Get the x coordinate (aka. apse)
        cout << "Enter the 'x' coordinate of point " << (char)(0x41+i) << ": "; // 0x41: A
        cin >> points[i].x;
        // Get the y coordinate (aka. ordinate)
        cout << "Enter the 'y' coordinate of point " << (char)(0x41+i) << ": "; // 0x41: A
        cin >> points[i].y;
        // Inform the user about the point that has been made
        cout << "Coordinates of " << (char)(0x41+i) << " are initialized as (" << points[i].x << ',' << points[i].y << ')' << endl; // 0x41: A

        // If it is the last point, connect it to the first point
        if(i == pointCount - 1) edges[i].End = points[0];
        // If it is not the first point, set previous edge's end to current point
        if(i != 0) edges[i-1].End = points[i];
        // Set the current edge's start point this
        edges[i].Start = points[i];
    }

    // Create array of floats to store the angles
    auto *angles = new float[pointCount]();
    // Create array of floats to store the lengths
    auto *lengths = new float[pointCount]();

    // Clear the console again
    system("clear");

    // Calculate the angles for each point
    for (int i = 0; i < pointCount; ++i) {
        // If its the first point, calculate the angle between the first and the last edges
        if(i == 0) angles[i] = AngleBetweenVectors(edges[0], edges[pointCount - 1]);
        // Else, calculate the angle between this edge and the previous edge
        else angles[i] = AngleBetweenVectors(edges[i], edges[i-1]);

        // Print the calculated angle to screen
        cout << "Angle of point " << (char)(0x41+i) << " is " << angles[i] << endl;

        int k = 0; // Just for showing edges as AB, BC, CD, DA and etc.
        if(i == pointCount -1) k = i + 1; // If it is the last point, set the second point to the first point

        // Calculate the length of the current edge
        lengths[i] = LengthOfEdge(&edges[i]);
        // Print the calculated length to screen
        cout << "The length of " << (char)(0x41+i) << (char)(0x41+i+1-k) << " is " << lengths[i] << endl << endl;
    }

    // Is the shape equilateral?
    bool equilateral = true;
    // Is the shape an isosceles triangle?
    bool isosceles = false;
    // Is the shape a rectangle?
    bool rectangle = false;

    // Check if the shape is an isosceles
    if(pointCount == 3) isosceles = (angles[0] == angles[1] || angles[0] == angles[2] || angles[1] == angles[2]);

    // Check if the shape is a rectangle
    if(pointCount == 4) rectangle = (lengths[0] == lengths[2] || lengths[1] == lengths[3]);

    // The angle that makes the shape equilateral
    float eqangleVal = (float) ((pointCount - 2) * 180) / (float) pointCount;

    // Loop every point of the shape
    for (int i = 0; i < pointCount; ++i) {
        if(angles[i] != eqangleVal) equilateral = false;

        // Exclude the last point
        if(i < pointCount - 1)
            if(lengths[i] != lengths[i+1]) equilateral = false; // The edge length that makes the shape equilateral
    }

    // Check for the shape is equilateral or not
    cout << "\nThe shape you entered is called ";
    if(equilateral)
    {
        if(pointCount == 4) cout << "square" << endl;
        else cout << "equilateral ";
    }
    else if(isosceles) cout << "isosceles triangle" << endl;
    else if(rectangle) cout << "rectangle" << endl;

    // If the shape is a square or isosceles triangle, don't write the shape name (already print square or isosceles triangle)
    if(pointCount != 4 && !isosceles) cout << ShapeNames[pointCount-3] << endl;

    return 1;
}