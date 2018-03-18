
uniform vec2 res;
uniform vec2 tick_lengths;
uniform vec2 tick_intervals;

void main() {

    float x = floor(gl_FragCoord.x);
    float y = floor(gl_FragCoord.y);
    
    float y_axis = floor(res.x / 2.0);
    float x_axis = floor(res.y / 2.0);
    
    // axes
    if (y_axis == x || x_axis == y) {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        return;
    }
    
    // grid lines
    
    // minor X axis
    if (mod(abs(y - x_axis), tick_intervals[0]) < 1.0) {
        if (abs(x - y_axis) < tick_lengths[0] / 2.0) {
        
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
            return;
        }
        gl_FragColor = vec4(1.0, 1.0, 1.0, 0.1);
    }
    
    // major X axis
    if (mod(abs(x - y_axis), tick_intervals[0]) < 1.0) {
        if (abs(y - x_axis) < tick_lengths[0] / 2.0) {
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
            return;
        }
        gl_FragColor = vec4(1.0, 1.0, 1.0, 0.1);
    }
    
    // minor Y axis
    if (mod(abs(y - x_axis), tick_intervals[1]) < 1.0) {
        if (abs(x - y_axis) < tick_lengths[1] / 2.0) {
        
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
            return;
        }
        gl_FragColor = vec4(1.0, 1.0, 1.0, 0.3);
    }
    
    // major Y axis
    if (mod(abs(x - y_axis), tick_intervals[1]) < 1.0) {
        if (abs(y - x_axis) < tick_lengths[1] / 2.0) {
        
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
            return;
        }
        gl_FragColor = vec4(1.0, 1.0, 1.0, 0.3);
    }
}