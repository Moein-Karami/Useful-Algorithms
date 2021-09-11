#include <bits/stdc++.h>
using namespace std;

// A point in 2D plane
struct Point
{
	int x, y;
};

// A line segment with left as Point
// with smaller x value and right with
// larger x value.
struct Segment
{
	Point left, right;
};


// An event for sweep line algorithm
// An event has a point, the position
// of point (whether left or right) and
// index of point in the original input
// array of segments.
struct Event {
	int x, y;
	bool isLeft;
	int index;
	Event(int x, int y, bool l, int i) : x(x), y(y), isLeft(l), index(i) {}

	// This is for maintaining the order in set.
	bool operator<(const Event& e) const {
			if(y==e.y)return x<e.x;
			return y < e.y;
	}
};


// Given three collinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r)
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
	return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (q.y - p.y) * (r.x - q.x) -
			(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0; // collinear

	return (val > 0)? 1: 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(Segment s1, Segment s2)
{
	Point p1 = s1.left, q1 = s1.right, p2 = s2.left, q2 = s2.right;

	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are collinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are collinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}


// Find predecessor of iterator in s.
set<Event>::iterator pred(set<Event> &s, set<Event>::iterator it) {
	return it == s.begin() ? s.end() : --it;
}

// Find successor of iterator in s.
set<Event>::iterator succ(set<Event> &s, set<Event>::iterator it) {
	return ++it;
}

// Returns true if any two lines intersect.
int isIntersect(Segment arr[], int n)
{
	unordered_map<string,int> mp; // to note the pair for which intersection is checked already
	// Pushing all points to a vector of events
	vector<Event> e;
	for (int i = 0; i < n; ++i) {
		e.push_back(Event(arr[i].left.x, arr[i].left.y, true, i));
		e.push_back(Event(arr[i].right.x, arr[i].right.y, false, i));
	}

	// Sorting all events according to x coordinate.
	sort(e.begin(), e.end(), [](Event &e1, Event &e2) {return e1.x < e2.x;});

	// For storing active segments.
	set<Event> s;
	int ans=0;
	// Traversing through sorted points
	for (int i=0; i<2*n; i++)
	{
		Event curr = e[i];
		int index = curr.index;

		// If current point is left of its segment
		if (curr.isLeft)
		{
			// Get above and below points
			auto next = s.lower_bound(curr);
			auto prev = pred(s, next);
			// Check if current point intersects with
			// any of its adjacent
			bool flag=false;
			if (next != s.end() && doIntersect(arr[next->index], arr[index])){
				string s=to_string(next->index+1)+" "+to_string(index+1);
				if(mp.count(s)==0){mp[s]++;ans++;} //if not already checked we can increase count in map
			}
			if (prev != s.end() && doIntersect(arr[prev->index], arr[index])){
					string s=to_string(prev->index+1)+" "+to_string(index+1);
				if(mp.count(s)==0){mp[s]++;ans++;} //if not already checked we can increase count in map
			}
			// if same line segment is there then decrease answer as it got increased twice
			if(prev != s.end() && next != s.end() && next->index==prev->index)ans--;


			// Insert current point (or event)
			s.insert(curr);
		}

		// If current point is right of its segment
		else
		{
			// Find the iterator
			auto it=s.find(Event(arr[index].left.x, arr[index].left.y, true, index));
			// Find above and below points
			auto next = succ(s, it);
			auto prev = pred(s, it);

			// If above and below point intersect
			if (next != s.end() && prev != s.end())
			{ string s=to_string(next->index+1)+" "+to_string(prev->index+1);
					string s1=to_string(prev->index+1)+" "+to_string(next->index+1);
				if (mp.count(s)==0&&mp.count(s1)==0&&doIntersect(arr[prev->index], arr[next->index]))
					ans++;
					mp[s]++;
				}

			// Remove current segment
			s.erase(it);

		}
	}
	//print pair of lines having intersection

	for(auto &pr:mp){
		cout<<pr.first<<"\n";
	}
	return ans;
}

// Driver code
int main() {
	Segment arr[] = { {{1, 5}, {4, 5}}, {{2, 5}, {10, 1}},{{3, 2}, {10, 3}},{{6, 4}, {9, 4}},{{7, 1}, {8, 1}}};
	int n = sizeof(arr)/sizeof(arr[0]);
	cout<<isIntersect(arr, n);
	return 0;
}

