#ifdef GL_ES

// D�finie la pr�cision par d�faut � moyen
precision mediump int;
precision mediump float;

#endif

// Texture 2D
uniform sampler2D texture;

// Position de la cam�ra
uniform vec3 v_camerapos;

// Position de la lumi�re
uniform vec3 v_lightpos;

// Couleur de la lumi�re
uniform vec3 v_lightcolor;

// Coordonn�es du fragment
varying vec3 v_position;

// Vecteur normal du sommet
varying vec3 v_normal;

// Coordonn�es sur la texture (en entr�e)
varying vec2 v_texcoord;

// Constante de l'ambiant
float Ka = 0.3;

// Constante du diffuse
float Kd = 0.8;

// Constante du sp�culaire
float Ks = 0.8;

// Fonction d'entr�e
void main() {
	// Ambient
    vec3 ambient = Ka * v_lightcolor;
	
	// Diffuse
	vec3 light_dir = normalize(v_lightpos - v_position);
    vec3 diffuse = Kd * max(dot(v_normal, light_dir), 0.0) * v_lightcolor;
    
    // Specular
	vec3 view_dir = normalize(v_camerapos - v_position);
    vec3 reflect_dir = reflect(-light_dir, v_normal); 
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
    vec3 specular = Ks * spec * v_lightcolor;

	// Assigne la couleur du fragment par rapport � la texture
    gl_FragColor = vec4(ambient + diffuse + specular, 1.0) * texture2D(texture, v_texcoord);
}
