h = 0.01;
a=-1;
b=1;
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
Physical Line("Dirichlet") = {1, 2, 3, 4};
Physical Surface("Mat1") = {6};
