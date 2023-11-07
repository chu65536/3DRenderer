#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>

#include "Object.hpp"
#include "OBJ_Loader.h"
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>


Object::Object(const std::string& path):
    m_model(glm::mat4(1.0f)),
    m_position(glm::vec3(0.0f, 0.0f, 0.0f)), 
    m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
{
    loadModel(path);
    initBuffers();
    // dont know how to properly load complex textures yet :(
    // loadTexture(texturePath); 
}

void Object::Draw()
{   
    for (size_t i = 0; i < m_meshes.size(); ++i) 
    {      
        glBindVertexArray(m_VAO[i]);
        glDrawElements(GL_TRIANGLES, m_meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
    }
}

void Object::loadModel(const std::string& path)
{   
	objl::Loader Loader;
	bool loadout = Loader.LoadFile(path);

	if (loadout)
	{
		std::ofstream file("e1Out.txt");

        float lX = 0.f, rX = 0.f;
        float lY = 0.f, rY = 0.f;
        float lZ = 0.f, rZ = 0.f;
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			objl::Mesh curMesh = Loader.LoadedMeshes[i];
			file << "Mesh " << i << ": " << curMesh.MeshName << "\n";
			file << "Vertices:\n";

            std::vector<float> positions;
            std::vector<float> normals;

			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{   
                float posX = curMesh.Vertices[j].Position.X;
                float posY = curMesh.Vertices[j].Position.Y;
                float posZ = curMesh.Vertices[j].Position.Z;
				file << "V" << j << ": " <<
					"P(" << posX << ", " << posY << ", " << posZ << ") " <<
					"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
					"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
                positions.emplace_back(posX);
                positions.emplace_back(posY);
                positions.emplace_back(posZ);
                lX = std::min(lX, posX); rX = std::max(rX, posX);
                lY = std::min(lY, posY); rY = std::max(rY, posY);
                lZ = std::min(lZ, posZ); rZ = std::max(rZ, posZ);
                positions.emplace_back(curMesh.Vertices[j].Normal.X); 
                positions.emplace_back(curMesh.Vertices[j].Normal.Y); 
                positions.emplace_back(curMesh.Vertices[j].Normal.Z); 
			}
            
			file << "Indices:\n";

            std::vector<unsigned int> indices;
			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				file << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
                indices.emplace_back(curMesh.Indices[j]);
                indices.emplace_back(curMesh.Indices[j + 1]);
                indices.emplace_back(curMesh.Indices[j + 2]);
			}

			file << "Material: " << curMesh.MeshMaterial.name << "\n";
			file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
			file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
			file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
			file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
			file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
			file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
			file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
			file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
			file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
			file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
			file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
			file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";

			file << "\n";

            Mesh mesh {positions, normals, indices};
            m_meshes.emplace_back(mesh);
		}

		file.close();
        scaleNormalize(lX, rX, lY, rY, lZ, rZ);
	}
	else
	{
		std::ofstream file("e1Out.txt");
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
        std::cout << "Failed to load object: " << path << std::endl;
		file.close();
	}
}

void Object::initBuffers()
{   
    for (size_t i = 0; i < m_meshes.size(); ++i)
    {
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_meshes[i].positions.size(), m_meshes[i].positions.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * m_meshes[i].indices.size(), m_meshes[i].indices.data(), GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        m_VAO.emplace_back(VAO);

    }
    glBindVertexArray(0);
}

void Object::SetRotation(float x, float y, float z)
{
    m_rotation = glm::vec3(x, y, z);
    updateModel();
}

void Object::SetPosition(float x, float y, float z)
{
    m_position = glm::vec3(x, y, z);
    updateModel();
}

void Object::SetScale(float x, float y, float z)
{
    m_scale = glm::vec3(x, y, z);
    updateModel();
}

void Object::SetColor(float r, float g, float b)
{
    m_color = glm::vec3(r, g, b);
}

void Object::updateModel()
{
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_position);
    m_model = glm::rotate(m_model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_model = glm::rotate(m_model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_model = glm::rotate(m_model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    m_model = glm::scale(m_model, m_scale);
}

glm::mat4 Object::GetModel() const
{
    return m_model;
}

glm::vec3 Object::GetColor() const
{
    return m_color;
}

void Object::Debug()
{
    std::cout << m_meshes.size() << ' ' << m_VAO.size() << std::endl;
}

void Object::scaleNormalize(float lX, float rX, float lY, float rY, float lZ, float rZ)
{
    float xDiff = rX - lX;
    float yDiff = rY - lY;
    float zDiff = rZ - lZ;
    
    float mxDiff = std::max(xDiff, std::max(yDiff, zDiff));
    float k = 1.f / mxDiff;

    float midX = (lX + rX) * 0.5f * k;
    float midY = (lY + rY) * 0.5f * k;
    float midZ = (lZ + rZ) * 0.5f * k;

    SetPosition(-midX, -midY, -midZ);
    SetScale(k, k, k);
}

std::array<float, 3> Object::GetPosition()
{
    return {m_position.x, m_position.y, m_position.z};
}

std::array<float, 3> Object::GetRotation()
{
    return {m_rotation.x, m_rotation.y, m_rotation.z};
}

std::array<float, 3> Object::GetScale()
{
    return {m_scale.x, m_scale.y, m_scale.z};
}


// Uncompleted
void Object::loadTexture(const std::string& path)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {   
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << nrChannels << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    std::cout << "Texture loaded successfully" << std::endl;
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, texture);
}