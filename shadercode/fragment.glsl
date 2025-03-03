#version 320 es
precision highp float;

struct Light_Struct {
    vec4 color;
    vec3 position;
    int use;
};

uniform Light_Struct lights[10];
uniform vec4 object_color;
uniform vec3 CameraView;

// Texture Uniform 
uniform bool use_texture;
uniform sampler2D texture1;

out vec4 outColor;

in vec2 FragTexCoords;
in vec3 Fragnormal;
in mat4 model_matrix;
in vec4 FragVertices;

void main(void) {
    if( use_texture )
        outColor = texture(texture1, FragTexCoords);
    else
        outColor = vec4(object_color.rgb, 1.0f);

    vec4 sum_light = vec4(0);
    for(int i = 0; i < 10; i++)
    {
        if( lights[i].use == 1 )
        {

            float amb_strength = 0.1;
            vec3 ambient = amb_strength * lights[i].color.rgb;

            vec3 normal = vec3( Fragnormal );
            vec3 lightDir = normalize( lights[i].position );

            float diff = max(dot(normal, lightDir), 0.1);
            vec3 diffuse = vec3(1) * diff;

            // Specular
            vec3 viewDir = normalize(CameraView - FragVertices.xyz);
            vec3 ref = reflect(-lightDir, normal);
            float specular = pow( max(dot(viewDir, ref), 0.0), 256.0);

            sum_light.rgb += ambient + diffuse;
        }
    }

    outColor.rgb *= sum_light.rgb;
}