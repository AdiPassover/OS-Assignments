both tests where on random 10000 points on seed 42:

  0.00      0.88     0.00        1     0.00   269.35  MonotoneChain::convexHull(std::vector<Point, std::allocator<Point> >&)

  2.27      0.79     0.02        1    20.00   563.98  GrahamScan::convexHull(std::vector<Point, std::allocator<Point> >&)

our implementation for monotone chain is faster than the one for graham scan.