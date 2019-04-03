#version 330 compatibility

uniform float	uDirection;		// direction
uniform float	uPara_2;		// direction

// -(3-(0.2*x*x+0.1*y*y))*0.04 top net
// sin(3.14* x/6) z change 
void
main( )
{	
	
	vec3 vert = gl_Vertex.xyz;
	if (vert.z == - 1.4) {			//left side net
	
	}
	else if (vert.z == 1.4)			//right side net
	{
		
	}
	else {
		if (vert.x > -7.3)			// top net
		{
			vert.y += (0.17-(0.05 * (vert.x + 7.1) * (vert.x + 7.1) + 0.1 * vert.z * vert.z) * (uDirection) / (uPara_2 - 60.));
		}
		else {						//back net
			
		}
	}  
	gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}
