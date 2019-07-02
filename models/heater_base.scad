$fn=200;

size = 75;
safety_zone_coef = 1.05;
base_height = 3;
leaf_height = 10;
leaf_thick = 2;
r = size / 2;
steps = 3;
angle = 360 / 3;
heater_d = 6;
heater_len = 20;
heater_space = 10;
center_space = 6;

module screw_hole(height) {
    screw_size = 3 * safety_zone_coef;
    cylinder(h=height, d=screw_size);
}

module screw_holes(height) {
    shift = 5;        
    for(i = [0:angle:360]) {
        x = sin(i) * (r - shift);
        y = cos(i) * (r - shift);
        translate([x, y, 0]) screw_hole(height);
    }
}

module base() {
    height = base_height;
    
    module ScrewHeadHole() {
        screw_size = 3 * safety_zone_coef;
        screw_head_size = 6 * safety_zone_coef;
        screw_head_height = 2 * safety_zone_coef;
        cylinder(h=height, d=screw_size);
        cylinder(h=2, d1=screw_head_size, d2=screw_size);
    }

    difference() {
        shift = 5;
        cylinder(h=height, d=size);
        for(i = [0:angle:360]) {
            x = sin(i) * (r - shift);
            y = cos(i) * (r - shift);
            translate([x, y, 0]) ScrewHeadHole();
        }
        linear_extrude(.5) rotate([180, 0, 0]) text("Eshisha", valign="center",halign="center");
    }
}

module leaf() {
    height = leaf_height;
    
    difference() {
        cylinder(h=height, d=size);
        cylinder(h=height, d=(size - (2 * leaf_thick)));
        for(i = [0:angle:360]) {
            // airflow spacing
            rotate(i) rotate(90 + (angle / 2)) translate([0,-heater_space / 2,0]) cube([r, heater_space, height]);
        }
    }
    
    // screw mounts
    intersection() {
        difference() {
            union() {
                for(i = [0:angle:360]) {
                    rotate(i) rotate(90) translate([0,-heater_space / 2,0]) cube([r, 10, leaf_thick]);
                }
            }
            // make them short
            union() {
                for(i = [0:angle:360]) {
                    rotate(i) rotate(90) translate([0,-heater_space / 2 - .5,0]) cube([r - 10, 11, leaf_thick]);
                }
            }
            screw_holes(height);
        }
        // correction
        cylinder(d=size,h=height);
    }
    
    // inner walls (WTF?)
    module sides() {
        %rotate(90) translate([0,-leaf_thick / 2,0]) cube([r,leaf_thick, height]);
        translate([0, center_space, 0]) rotate(59) rotate(90) translate([0,-leaf_thick / 2,0]) cube([r,leaf_thick, height]);
        translate([0, center_space, 0]) rotate(-59) rotate(90) translate([0,-leaf_thick / 2,0]) cube([r,leaf_thick, height]);
    }
    intersection() {
        difference() {
            union() {
                for(i = [0:angle:360]) {
                    rotate(i) sides();
                }
                
            }
            cylinder(d=22, h=height);
        }
        // correction
        cylinder(d=size,h=height);
    }
}

module heater() {
    rotate(90 + (angle / 2)) translate([r - heater_len - 4, 0, 0]) rotate(90, [0, 1, 0]) cylinder(d=heater_d, h=heater_len);
}

module top() {
    height = 2;
    
    difference() {
        cylinder(h=height, d=size);
        screw_holes(height);
    }
}

base();
translate([0,0, base_height]) leaf();

for(i = [0:angle:360]) {
    translate([0, 0, base_height + heater_d / 2 + 1]) {
        for(i = [0:angle:360]) {
            rotate(i) heater();
        }
    }
}

//translate([0,0, base_height + leaf_height]) top();