#include <iostream>
#include <string>
#include <GL/glew.h>
#include <fstream>
#ifndef NVectorC
#define NVectorC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#include <vector>

using namespace std;
class shader
{
  private:
    string vertex_path, frag_path;
    string vertex_shader, fragment_shader;
    GLuint shader_program;
    vector<unsigned int> texture;

  public:
    shader(string vertex_path, string frag_path)
    {
        this->frag_path = frag_path;
        this->vertex_path = vertex_path;

        ifstream ifs(frag_path);
        fragment_shader.assign((istreambuf_iterator<char>(ifs)),
                               (istreambuf_iterator<char>()));

        ifstream ifs2(vertex_path);
        vertex_shader.assign((istreambuf_iterator<char>(ifs2)),
                             (istreambuf_iterator<char>()));
        compile();
    }

    void compile()
    {
        const char *vertex_shader_tmp = vertex_shader.c_str();
        const char *frag_shader_tmp = fragment_shader.c_str();
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &vertex_shader_tmp, NULL);
        glCompileShader(vert_shader);
        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &frag_shader_tmp, NULL);
        glCompileShader(frag_shader);
        shader_program = glCreateProgram();
        glAttachShader(shader_program, frag_shader);
        glAttachShader(shader_program, vert_shader);
        glLinkProgram(shader_program);
    }

    void printFilePath()
    {
        cout << "vertex shader file path is " << vertex_path << "\n";
        cout << "fragment shader file path is " << frag_path << "\n";
    }

    GLuint get_program()
    {
        return this->shader_program;
    }

    void use()
    {
        glUseProgram(this->shader_program);
    }

    unsigned int getTexture(int index)
    {
        return texture[index];
    }

    void addTexture(string filename)
    {
        unsigned int textureEle;
        glGenTextures(1, &textureEle);
        glBindTexture(GL_TEXTURE_2D, textureEle);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
        }
        stbi_image_free(data);

        texture.push_back(textureEle);
    }

    void bindTexture(string name, int index)
    {
        int textureLocation = glGetUniformLocation(shader_program, name.c_str());
        glUniform1i(textureLocation, index);
        glActiveTexture(GL_TEXTURE0 + index); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, this->getTexture(index));
    }

    void set4f(string identifier, float e1, float e2, float e3, float e4)
    {
        int vertexColorLocation = glGetUniformLocation(this->get_program(), identifier.c_str());
        glUniform4f(vertexColorLocation, e1, e2, e3, e4);
    }
    void setMatrix4f(string identifier, glm::mat4 mat4)
    {
        unsigned int transformLoc = glGetUniformLocation(this->get_program(), identifier.c_str());
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mat4));
    }
};
