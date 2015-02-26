Mesh.MshFileVersion = 2.2;
Mesh.CharacteristicLengthExtendFromBoundary=1;
Mesh.CharacteristicLengthFromPoints=1;
Mesh.ElementOrder=1;
Mesh.SecondOrderIncomplete = 0;
Mesh.Algorithm = 6;
Mesh.Algorithm3D = 4;
//Mesh.OptimizeNetgen=1;
// partitioning data
Mesh.Partitioner=1;
Mesh.NbPartitions=1;
Mesh.MshFilePartitioned=0;
h = 0.1;
Mesh.RecombinationAlgorithm=0;
a=-0.5;
b=1;
c=-0.5;
d=1.5;
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
Physical Line("wall1") = {2};
Physical Line("wall2") = {4};
Physical Line("wall") = {2,4};
Physical Line("outlet") = {3};
Physical Surface(6) = {6};

