cl__1 = 2;
Point(1) = {-1, -1, 0, 2};
Point(2) = {1, -1, 0, 2};
Point(3) = {-1, 1, 0, 2};
Point(4) = {1, 1, 0, 2};
Line(1) = {1, 2};
Line(2) = {2, 4};
Line(3) = {4, 3};
Line(4) = {3, 1};
Line(5) = {2, 3};
Transfinite Line "*" = 1;
Line Loop(7) = {1,5,4};
Line Loop(8) = {3,-5,2};
Plane Surface(7) = {7};
Plane Surface(8) = {8};
