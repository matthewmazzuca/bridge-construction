h = 0.1;
a=0;
b=5;
c=-1;
d=1;
Point(1) = {a,c,0.0,h};
Point(2) = {b,c+0,0.0,h};
Point(3) = {b+0,d,0.0,h};
Point(4) = {a+0,d+0,0.0,h};
Line(1) = {4,1};
Line(2) = {1,2};
Line(3) = {2,3};
Line(4) = {3,4};
Line Loop(5) = {1,2,3,4};
Plane Surface(6) = {5};
Physical Line("inlet") = {1};
Physical Line("bottomwall") = {2};
Physical Line("topwall") = {4};
Physical Line("wall") = {2,4};
Physical Line("outlet") = {3};
Physical Surface(6) = {6};
