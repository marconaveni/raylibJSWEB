

export class Vector2 {
    constructor(x = 0, y = 0) {
        this.x = x;                                             // Vector x component
        this.y = y;                                             // Vector y component
    }
}

export class Camera {
    constructor(offset = new Vector2(), target = new Vector2(), rotation = 0, zoom = 0) {
        this.offset = offset;                                   // Camera offset (displacement from target)
        this.target = target;                                   // Camera target (rotation and zoom origin)
        this.rotation = rotation;                               // Camera rotation in degrees
        this.zoom = zoom;                                       // Camera zoom (scaling), should be 1 by default
    }
}


export class Rectangle {
    constructor(x = 0, y = 0, width = 0, height = 0) {
        this.x = x;                                             // Rectangle x component
        this.y = y;                                             // Rectangle y component
        this.width = width;                                     // Rectangle width component
        this.height = height;                                   // Rectangle height component
    }
}

export class Texture2D {
    constructor(id = 0, width = 0, height = 0, mipmaps = 0, format = 0) {
        this.id = id;
        this.width = width;
        this.height = height;
        this.mipmaps = mipmaps;
        this.format = format;
    }

}

export class Color {

    static lightgray = new Color(200, 200, 200, 255);   // Light Gray
    static gray = new Color(130, 130, 130, 255);        // Gray
    static darkgray = new Color(80, 80, 80, 255);       // Dark Gray
    static yellow = new Color(253, 249, 0, 255);        // Yellow
    static gold = new Color(255, 203, 0, 255);          // Gold
    static orange = new Color(255, 161, 0, 255);        // Orange
    static pink = new Color(255, 109, 194, 255);        // Pink
    static red = new Color(230, 41, 55, 255);           // Red
    static maroon = new Color(190, 33, 55, 255);        // Maroon
    static green = new Color(0, 228, 48, 255);          // Green
    static lime = new Color(0, 158, 47, 255);           // Lime
    static darkgreen = new Color(0, 117, 44, 255);      // Dark Green
    static skyblue = new Color(102, 191, 255, 255);     // Sky Blue
    static blue = new Color(0, 121, 241, 255);          // Blue
    static darkblue = new Color(0, 82, 172, 255);       // Dark Blue
    static purple = new Color(200, 122, 255, 255);      // Purple
    static violet = new Color(135, 60, 190, 255);       // Violet
    static darkpurple = new Color(112, 31, 126, 255);   // Dark Purple
    static beige = new Color(211, 176, 131, 255);       // Beige
    static brown = new Color(127, 106, 79, 255);        // Brown
    static darkbrown = new Color(76, 63, 47, 255);      // Dark Brown
    static white = new Color(255, 255, 255, 255);       // White
    static black = new Color(0, 0, 0, 255);             // Black
    static blank = new Color(0, 0, 0, 0);               // Blank (Transparent)
    static magenta = new Color(255, 0, 255, 255);       // Magenta
    static raywhite = new Color(245, 245, 245, 255);    // White (raylib logo)

    constructor(r, g, b, a) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }
}

export class Layer {
    static renderUi = 2;
    static renderMain = 1;
    static renderBackground = 0;
}