h=0.1;
Point(1) = {-0.25,-1,0, h};
Point(2) = {0.25,-1,0, h};
Point(3) = {0.25,0,0, h};
Point(4) = {-0.25,0,0, h};
Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};
Line Loop(1) = {1,2,3,4};
h=0.1;
Point(5) = {-0.125,-0.5,0, h};
Point(6) = {0.125,-0.5,0, h};
Point(7) = {0.125,-0.25,0, h};
Point(8) = {-0.125,-0.25,0, h};
Line(5) = {5,6};
Line(6) = {6,7};
Line(7) = {7,8};
Line(8) = {8,5};
Line Loop(2) = {5,6,7,8};
Plane Surface(1) = {1,-2};
Plane Surface(2) = {2};
Physical Line("ground") = {3};
Physical Surface("soil.0") = {1};
Physical Surface("soil.1") = {2};
