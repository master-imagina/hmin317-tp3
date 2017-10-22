#ifdef GL_ES

// Définie la précision par défaut à moyen
precision mediump int;
precision mediump float;

#endif

// Matrice de projection
uniform mat4 m_projection;

// Matrice de caméra
uniform mat4 m_view;

// Matrice de l'objet
uniform mat4 m_model;

// Coordonnées du sommet
attribute vec4 a_position;

// Coordonnées du fragment
varying vec3 v_position;

// Vecteur normal du sommet
attribute vec4 a_normal;

// Vecteur normal du sommet
varying vec3 v_normal;

// Coordonnées sur la texture (en entrée)
attribute vec2 a_texcoord;

// Coordonnées sur la texture (en sortie)
varying vec2 v_texcoord;

// Fonction d'entrée
void main() {
	// Calcule la position du sommet projeté
    gl_Position = m_projection * m_view * m_model * a_position;
	
	// Calcule le vecteur normal du sommet
	v_position = a_position.xyz;
	
	// Calcule le vecteur normal du sommet
	v_normal = a_normal.xyz;

	// Transmet les coordonnées de texture au fragment shader
	// La valeur est automatiquement interpolée aux fragments sur la face du polygone
    v_texcoord = a_texcoord;
}
