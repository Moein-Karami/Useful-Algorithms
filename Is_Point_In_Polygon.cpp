#include <iostream>
using namespace std;
// Define Infinite (Using INT_MAX caused overflow problems)
#define INF 10000
struct Point
{
	int x;
	int y;
};
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
			q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;
	return false;
}
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
			(q.x - p.x) * (r.y - q.y);
	if (val == 0) return 0; // colinear
	return (val > 0)? 1: 2; // clock or counterclock wise
}
// The function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);
	if (o1 != o2 && o3 != o4)
		return true;
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;
}
bool isInside(Point polygon[], int n, Point p)
{
	if (n < 3) return false;
	Point extreme = {INF, p.y};
	int count = 0, i = 0;
	do
	{
		int next = (i+1)%n;
		if (doIntersect(polygon[i], polygon[next], p, extreme))
		{
			if (orientation(polygon[i], p, polygon[next]) == 0)
			return onSegment(polygon[i], p, polygon[next]);
			count++;
		}
		i = next;
	} while (i != 0);
	return count&1; // Same as (count%2 == 1)
}
// Driver program to test above functions
int main()
{
	Point polygon1[] = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
	int n = sizeof(polygon1)/sizeof(polygon1[0]);
	Point p = {20, 20};
	isInside(polygon1, n, p)? cout << "Yes \n": cout << "No \n";
	return 0;
}

