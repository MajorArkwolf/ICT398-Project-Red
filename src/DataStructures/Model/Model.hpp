#pragma once
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "DataStructures/Model/Animation.hpp"
#include "DataStructures/Model/DataTypes.hpp"
#include "DataStructures/Model/Mesh.hpp"

class Shader;

namespace model {
    class Model {
      public:
        Model() = default;
        /// Textures IDs that have been loaded.
        std::vector<TextureB> textures_loaded_ = {}; // Stores all textures, needs to be global
        /// Meshes of models.
        std::vector<Mesh> meshes_ = {};
        /// The directory of the model.
        std::filesystem::path directory_ = {};
        /// Does the model need gamma correction.
        bool gamma_correction_ = {};

        /**
         * Constructor for the model.
         * @param path path to the model
         * @param gamma correction if required.
         */
        Model(char *path, bool gamma);
        /**
         * Constructor for the model.
         * @param path path to the model
         * @param gamma correction if required.
         */
        Model(const std::string& path, bool gamma);

        /**
         * Constructor for a filepath for the model.
         * @param path file path
         * @param gamma is there gamma
         */
        Model(const std::filesystem::path& path, bool gamma);

        /**
         * Draw call for the model
         * @param shader used to draw the model.
         */
        void Draw(Shader* shader);

        void Update(float t, float dt);

        /*----------------*/
        unsigned num_bones_ = 0;
        std::vector<BoneInfo> bone_info_ = {};
        std::vector<Animation> animation_ = {};

        /**
         * Gets an animation by name.
         * @param anim_name searches for the string animation with given name.
         * @return either the animation that matches the string or the idle animation
         */
        Animation* GetAnimation(const std::string& anim_name);

      public:
        /**
         * Loads a model from file.
         * @param path to the model.
         */
        void LoadModel(const std::filesystem::path &path);
        /**
         * Process Nodes from ASSIMP into the models.
         * @param node of the model.
         * @param scene the model loaded in.
         */
        void ProcessNode(aiNode *node, const aiScene *scene);

        /**
         * Process the mesh of a model.
         * @param mesh of the model.
         * @param scene scene the model loaded in.
         * @return returns the next mesh.
         */
        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
        /**
         * Load a material into the GPU
         * @param mat the material type.
         * @param type texture type
         * @param type_name name
         * @return the ID of it in memory.
         */
        std::vector<TextureB> LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                                   const std::string& type_name);

        /// Bone map, takes a string and gives its ID.
        std::map<std::string, unsigned int> bone_mapping_ = {};
        /// global inverse transform matrix
        glm::mat4 global_inverse_transform_ = {};

        /**
         * Load bones
         * @param mesh_index mesh index
         * @param ai_mesh object that contains the mesh.
         */
        void LoadBones(unsigned int mesh_index, const aiMesh *ai_mesh);

        /**
         * Load animations
         * @param scene scene that contains the animation
         */
        void LoadAnimations(const aiScene *scene);

        /**
         * Load joints
         * @param mesh mesh ID
         * @param root node of the joint
         */
        void LoadJoints(aiMesh* mesh, aiNode* root);

        /**
         * Loads the material information into the mesh.
         * @param mat the assimp material information.
         * @return material to be passed to the renderer.
         */
        Material LoadMaterial(aiMaterial* mat);

        /// root joint.
        JointsName root_joint_;
        /// Is the model animation
        bool is_animated_ = false;
    };
}

