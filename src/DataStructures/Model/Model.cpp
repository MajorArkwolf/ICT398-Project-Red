#include "Model.hpp"

#include <iostream>
#include <set>
#include <Engine/Engine.hpp>

static inline glm::vec3 vec3_cast(const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); }
static inline glm::vec2 vec2_cast(const aiVector3D &v) { return glm::vec2(v.x, v.y); } // it's aiVector3D because assimp's texture coordinates use that
static inline glm::quat quat_cast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }
static inline glm::mat4 mat4_cast(const aiMatrix4x4 &m) { return glm::transpose(glm::make_mat4(&m.a1)); }

model::Model::Model(char *path, bool gamma = false) : gamma_correction_(gamma) {
    LoadModel(path);
}

model::Model::Model(const std::filesystem::path &path, bool gamma) : gamma_correction_(gamma){
    LoadModel(path);
}

model::Model::Model(const std::string& path, bool gamma = false) : gamma_correction_(gamma) {
    LoadModel(path);
}

void model::Model::Draw(Shader* shader) {
    auto cameraPos = redengine::Engine::get().renderer_.GetActiveCamera()->position_;
    shader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader->SetVec3("lightPos", 1.0f, 400.0f, 1.0f);
    shader->SetVec3("viewPos", cameraPos);
    for (auto &mesh : meshes_) {
        mesh.Draw(shader);
    }
}

static inline bool CheckFileType(const std::filesystem::path& dir) {
    return !(dir == "fbx" || dir == "obj");
}

void model::Model::LoadModel(const std::filesystem::path &path) {
    // read file via ASSIMP
    directory_ = path;
    Assimp::Importer importer;
    auto *scene =
        importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs |
                                    aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights | aiProcess_GenSmoothNormals);
    // check for errors

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) // if is Not Zero
    {
        std::string error = static_cast<std::string>(importer.GetErrorString());
        std::cout << "ERROR::ASSIMP:: " << error << std::endl;
        return;
    }

    // retrieve the directory path of the filepath
    is_animated_ = CheckFileType(path.extension());
    is_animated_ = is_animated_ && scene->HasAnimations();
    global_inverse_transform_ = glm::inverse(mat4_cast(scene->mRootNode->mTransformation));
    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
    for (auto &mesh : meshes_) {
        mesh.MoveToGPU();
    }
}

void model::Model::ProcessNode(aiNode *node, const aiScene *scene) {
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        auto newMesh = ProcessMesh(mesh, scene);
        meshes_.push_back(newMesh);
        if (is_animated_) {
            LoadBones(i, mesh);
            LoadAnimations(scene);
            LoadJoints(mesh, scene->mRootNode);
        }
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh model::Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
    // data to fill
    std::vector<Vertex> vertices = {};
    std::vector<unsigned int> indices = {};
    std::vector<TextureB> textures = {};

    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex = {};
        glm::vec3 vector{
            0, 0,
            0}; // we declare a placeholder vector since assimp uses
                // its own vector class that doesn't directly convert
                // to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        if (mesh->HasPositions()) {
            vector.x        = mesh->mVertices[i].x;
            vector.y        = mesh->mVertices[i].y;
            vector.z        = mesh->mVertices[i].z;
            vertex.position = vector;
        }
        // normals
        if (mesh->HasNormals()) {
            vector.x      = mesh->mNormals[i].x;
            vector.y      = mesh->mNormals[i].y;
            vector.z      = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x            = mesh->mTextureCoords[0][i].x;
            vec.y            = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        } else
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        // tangent
        if (mesh->HasTangentsAndBitangents()) {
            vector.x       = mesh->mTangents[i].x;
            vector.y       = mesh->mTangents[i].y;
            vector.z       = mesh->mTangents[i].z;
            vertex.tangent = vector;
            // bitangent
            vector.x         = mesh->mBitangents[i].x;
            vector.y         = mesh->mBitangents[i].y;
            vector.z         = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse
    // texture should be named as 'texture_diffuseN' where N is a sequential
    // number ranging from 1 to MAX_SAMPLER_NUMBER. Same applies to other
    // texture as the following list summarizes: diffuse: texture_diffuseN
    // specular: texture_specularN normal: texture_normalN
    // 1. diffuse maps
    std::vector<TextureB> diffuseMaps =
            LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<TextureB> specularMaps =
            LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<TextureB> normalMaps =
            LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<TextureB> heightMaps =
            LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<TextureB> model::Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                                         const std::string& type_name) {
    std::vector<TextureB> textures = {};
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str = {};
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (auto & j : textures_loaded_) {
            if (std::strcmp(j.path.data(), str.C_Str()) == 0) {
                textures.push_back(j);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;

            }
        }
        if (!skip) { // if texture hasn't been loaded already, load it
            TextureB texture = {};
            texture.id   = redengine::Engine::get().renderer_.TextureFromFile(str.C_Str(), this->directory_, false);
            texture.type = type_name;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded_.push_back(
                texture); // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

void model::Model::Update(float t, float dt) {

}


static inline void ToUpperString(std::string& string) {
    std::for_each(string.begin(), string.end(), [](char & c){
      c = static_cast<char>(toupper(c));
    });
}

void model::Model::LoadAnimations(const aiScene* scene) {
    for (size_t i = 0; i < scene->mNumAnimations; ++i) {
        std::string anim_name = scene->mAnimations[i]->mName.C_Str();
        ToUpperString(anim_name);
        auto new_animation = Animation(anim_name, scene->mAnimations[i]->mDuration , scene->mAnimations[i]->mTicksPerSecond);
        scene->mAnimations[i]->mName.C_Str();
        for (size_t ii = 0; ii < scene->mAnimations[i]->mNumChannels; ++ii) {
            AnimJointNode anim = {};
            auto channel = scene->mAnimations[i]->mChannels[ii];
            anim.num_pos_keys = channel->mNumPositionKeys;
            anim.num_rot_keys = channel->mNumRotationKeys;
            //anim.numScaleKeys = channel->mNumScalekeys;
            for (size_t x = 0; x < channel->mNumRotationKeys; ++x) {
                anim.rot_key.emplace_back(channel->mRotationKeys[x].mTime, quat_cast(channel->mRotationKeys[x].mValue));
            }
            for (size_t x = 0; x < channel->mNumPositionKeys; ++x) {
                anim.pos_key.emplace_back(channel->mRotationKeys[x].mTime, vec3_cast(channel->mPositionKeys[x].mValue));
            }
//            for (size_t x = 0; x < channel->mNumScalingKeys; ++x) {
//                anim.pos_key.emplace_back(vec3_cast(channel->mScalingKeys[x]));
//            }
            new_animation.anim_map_.emplace(channel->mNodeName.C_Str(), anim);
        }
        animation_.emplace_back(new_animation);
    }
}

void model::Model::LoadBones(unsigned mesh_index, const aiMesh* ai_mesh)
{
    for (unsigned i = 0 ; i < ai_mesh->mNumBones; ++i) {
        unsigned boneIndex = 0;
        std::string boneName(ai_mesh->mBones[i]->mName.data);

        if (bone_mapping_.find(boneName) == bone_mapping_.end()) {
            boneIndex = num_bones_;
            ++num_bones_;
            BoneInfo bi;
            bone_info_.push_back(bi);
        }
        else {
            boneIndex = bone_mapping_[boneName];
        }

        bone_mapping_[boneName] = boneIndex;
        bone_info_[boneIndex].bone_offset = mat4_cast(ai_mesh->mBones[i]->mOffsetMatrix);

        for (unsigned j = 0 ; j < ai_mesh->mBones[i]->mNumWeights; ++j) {
            unsigned VertexID = ai_mesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = ai_mesh->mBones[i]->mWeights[j].mWeight;
            meshes_.at(mesh_index).AddBoneData(VertexID, static_cast<int>(boneIndex), Weight);
        }
    }
}

static inline JointsName RecurseJoints(aiNode* node, aiNode* rootScene) {
    JointsName parent = {};
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        parent.children.push_back(RecurseJoints(rootScene->FindNode(node->mChildren[i]->mName), rootScene));
    }
    parent.name = std::string(node->mName.C_Str());
    parent.transform = mat4_cast(node->mTransformation);
    return parent;
}
static inline aiNode* FindRootJoint(aiMesh* mesh, aiNode* root) {
    std::map<std::string, aiString> boneNames = {};
    for (size_t i = 0; i < mesh->mNumBones; ++i) {
        boneNames.insert({std::string(mesh->mBones[i]->mName.C_Str()), mesh->mBones[i]->mName});
    }
    for (size_t i = 0; i < mesh->mNumBones; ++i) {
        auto *boneNode = root->FindNode(mesh->mBones[i]->mName);
        auto boneNodeParent = boneNode->mParent->mName;
        if (!boneNames.count(std::string(boneNodeParent.C_Str()))) {
            return root->FindNode(boneNodeParent);
        }
    }
    return nullptr;
}

void model::Model::LoadJoints(aiMesh* mesh, aiNode* root) {
    auto root_bone = FindRootJoint(mesh, root);
    if (root_bone == nullptr) {
        std::cerr << "ERROR: ROOT JOINT NOT FOUND";
        EXIT_FAILURE;
    }
    root_joint_ = RecurseJoints(root_bone, root);
}

Animation* model::Model::GetAnimation(const std::string &anim_name) {
    Animation* idle = nullptr;
    for (auto &anim : animation_) {
        std::string animNameStored = anim.GetName();
        size_t index = animNameStored.find(anim_name);
        if (index != std::string::npos) {
            return &anim;
        }
        index = animNameStored.find("IDLE");
        if (index != std::string::npos) {
            idle = &anim;
        }
    }
    if (idle != nullptr) {
        return idle;
    }
    if (!animation_.empty()) {
        return &animation_.at(0);
    }
    return nullptr;
}

model::Material loadMaterial(aiMaterial* mat) {
    model::Material material = {};
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess = 0.0f;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.Diffuse = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.Ambient = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.Specular = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.Shininess = shininess;

    return material;
}