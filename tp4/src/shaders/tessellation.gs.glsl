#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in ATTRIB_TES_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
} attribIn[];

out ATTRIB_GS_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
    vec3 barycentricCoords;
} attribOut;

void main()
{
    for (int i = 0; i < 3; i++) {
        // utilisant la position interpolée et en la conver-
        //tissant dans un intervalle [0, 1] avec la taille du plan complet (prenez note que celui-ci est centré
        //en (0, 0)). De plus, on divisera les coordonnées par 4 pour étirer la texture sur le plan et réduire la
        //fréquence des montagnes
        attribOut.texCoords = attribIn[i].texCoords;
        // Pour connaitre la hauteur du vertex, on convertira la donnée de hauteur [0, 1] en [−32, 32]. L’attribut
        //en sortie aura la valeur normalisée.
        attribOut.height = attribIn[i].height;
        // L’attribut de sortie patchDistance est un vecteur où chaque composante est la distance par rapport à
        //un côté de la patch (coordonnées barycentriques du quadrilatère). On peut facilement l’obtenir avec
        //les coordonnées de gl _T essCoord et 1 − gl_T essCoord.
        attribOut.patchDistance = attribIn[i].patchDistance;
        // Pour l’attribut de coordonnées de texture, on utilise directement gl_T essCoord avec un multiplicateur
        //de 2 pour augmenter la répétition dans la texture.
        attribOut.barycentricCoords = vec3(0);
        EmitVertex();
    }
}
