mSize=0.025;
boxLength = 0.584;
boxWidth = 0.146;
boxHeight = 0.70;

Point(1) = {0.0, 0.0, 0, mSize};
Point(2) = {0.0, 0.0, boxHeight,mSize};
Point(3) = {boxLength, 0.0, boxHeight,mSize};
Point(4) = {boxLength, 0.0, 0,mSize};

Point(5) = {0.0, boxWidth, 0,mSize};
Point(6) = {0.0, boxWidth, boxHeight,mSize};
Point(7) = {boxLength, boxWidth, boxHeight,mSize};
Point(8) = {boxLength, boxWidth, 0,mSize};

Line(11) = {1, 2};
Line(12) = {2, 3};
Line(13) = {3, 4};
Line(14) = {4, 1};

Line(15) = {1, 5};
Line(16) = {2, 6};
Line(17) = {3, 7};
Line(18) = {4, 8};

Line(19) = {5, 6};
Line(20) = {6, 7};
Line(21) = {7, 8};
Line(22) = {8, 5};

Line Loop(31) = {-14, -13, -12, -11}; //front
Line Loop(32) = {13,18,-21,-17}; //right
Line Loop(33) = {20,21,22,19}; //back
Line Loop(34) = {11,16,-19,-15}; //left
Line Loop(35) = {-22,-18,14,15}; // bottom
Line Loop(36) = {12, 17, -20, -16}; // top

Plane Surface(41) = {35}; //bottom
Plane Surface(42) = {31}; //front 
Plane Surface(43) = {32}; //right
Plane Surface(44) = {33}; //back 
Plane Surface(45) = {34}; //left
Plane Surface(46) = {36}; //top

Surface Loop(51) = {41,42,43,44,45,46};

Volume(61) = {51};

//Physical Volume("internal") = {51};

//Physical Surface("front") = {42};
//Physical Surface("back") = {44};
//Physical Surface("bottom") = {41};
//Physical Surface("left") = {45};
//Physical Surface("top") = {46};
//Physical Surface("right") = {43};
