$fn=200;

height = 3;
size = 75;

module ScrewHole() {
    screw_size = 3;
    screw_head_size = 6;
    screw_head_height = 2;
    cylinder(h=size, d=screw_size);
    #cylinder(h=2, d1=screw_head_size, d2=screw_size);
}


difference() {
    shift = 4;
    cylinder(h=height, d=size);
    translate([(sin(45) * size / 2) - shift, (cos(45) * size / 2) - shift, 0]) ScrewHole();
    translate([-(sin(45) * size / 2) + shift, (sin(45) * size / 2) - shift, 0]) ScrewHole();
    translate([(sin(45) * size / 2) - shift, -(sin(45) * size / 2) + shift, 0]) ScrewHole();
    translate([-(sin(45) * size / 2) + shift, -(sin(45) * size / 2) + shift, 0]) ScrewHole();
}