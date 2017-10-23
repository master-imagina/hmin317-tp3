#ifdef GL_ES

// D�finie la pr�cision par d�faut � moyen
precision mediump int;
precision mediump float;

#endif

// Matrice de projection
uniform mat4 m_projection;

// Matrice de cam�ra
uniform mat4 m_view;

// Matrice de l'objet
uniform mat4 m_model;

// Coordonn�es du sommet
attribute vec4 a_position;

// Coordonn�es du fragment
varying vec3 v_position;

// Vecteur normal du sommet
attribute vec4 a_normal;

// Vecteur normal du sommet
varying vec3 v_normal;

// Coordonn�es sur la texture (en entr�e)
attribute vec2 a_texcoord;

// Coordonn�es sur la texture (en sortie)
varying vec2 v_texcoord;

// Fonction d'entr�e
void main() {
	// Calcule la position du sommet projet�
    gl_Position = m_projection * m_view * m_model * a_position;
	
	// Calcule le vecteur normal du sommet
	v_position = a_position.xyz;
	
	// Calcule le vecteur normal du sommet
	v_normal = a_normal.xyz;

	// Transmet les coordonn�es de texture au fragment shader
	// La valeur est automatiquement interpol�e aux fragments sur la face du polygone
    v_texcoord = a_texcoord;
}
