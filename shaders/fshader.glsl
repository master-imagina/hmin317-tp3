#ifdef GL_ES

// Définie la précision par défaut à moyen
precision mediump int;
precision mediump float;

#endif

// Texture 2D
uniform sampler2D texture;

// Position de la caméra
uniform vec3 v_camerapos;

// Position de la lumière
uniform vec3 v_lightpos;

// Couleur de la lumière
uniform vec3 v_lightcolor;

// Coordonnées du fragment
varying vec3 v_position;

// Vecteur normal du sommet
varying vec3 v_normal;

// Coordonnées sur la texture (en entrée)
varying vec2 v_texcoord;

// Constante de l'ambiant
float Ka = 0.3;

// Constante du diffuse
float Kd = 0.8;

// Constante du spéculaire
float Ks = 0.8;

// Fonction d'entrée
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

	// Assigne la couleur du fragment par rapport à la texture
    gl_FragColor = vec4(ambient + diffuse + specular, 1.0) * texture2D(texture, v_texcoord);
}
