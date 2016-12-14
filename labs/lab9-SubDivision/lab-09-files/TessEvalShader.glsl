#version 430 

layout(triangles, equal_spacing, ccw) in;

void main()
{   
	//find barimetric coordinate
	//area of trianangle 	
	
    gl_Position =
        gl_in[0].gl_Position * gl_TessCoord.x +
        gl_in[1].gl_Position * gl_TessCoord.y +
        gl_in[2].gl_Position * gl_TessCoord.z  ;
    float temp = gl_Position[3];
    gl_Position[3] = 0;
    gl_Position = normalize(gl_Position);
    gl_Position[3] = temp;
    
}
//gl_TessCoord is barycentric coordinet of the new gl_position 
