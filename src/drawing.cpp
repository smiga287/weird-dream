#include <program_state.h>
#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <Skybox.h>
#include <Floor.h>
#include <sstream>
#include <array>

void setup_lighting(const Shader& lights_shader, const ProgramState& state) {
    const PointLight& pointLight = state.pointLight;
    auto& camera = state.camera;

    lights_shader.use();
    lights_shader.setVec3("viewPos", camera.Position);
    lights_shader.setFloat("material.shininess", 16.0f); // TODO: try out 32.0f
    lights_shader.setInt("blinn", state.is_blinn_enabled);
    lights_shader.setInt("flashLight", state.is_flashlight_enabled);

    // directional light setup
    lights_shader.setVec3("dirLight.direction", 1.0f, -0.5f, 0.0f);
    lights_shader.setVec3("dirLight.ambient", 0.01f, 0.01f, 0.01f);
    lights_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    lights_shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

    auto pointLightPositions = std::array{
         glm::vec3(state.building_position.x + 8.5f, state.building_position.y + 3.2f, state.building_position.z + 4.5f),
         glm::vec3(state.building_position.x + 6.5f, state.building_position.y + 3.7f, state.building_position.z - 3.0f),
         glm::vec3(state.building_position.x - 5.2f, state.building_position.y + 3.7f, state.building_position.z + 9.0f),
         glm::vec3(state.building_position.x - 10.5f, state.building_position.y + 3.7f, state.building_position.z + 7.0f),
         glm::vec3(state.building_position.x + 0.5f, state.building_position.y + 6.0f, state.building_position.z + 3.0f)
    };

    // point light setup
    const int POINT_LIGHT_NUM = 6; // TODO: refactor
    for (int i = 0; i < POINT_LIGHT_NUM; i++) {
        std::stringstream point_light_stream;
        point_light_stream << "pointLights[" << i << "].";
        const auto& point_light = point_light_stream.str();

        lights_shader.setVec3(point_light + "position", pointLightPositions[i]);
        lights_shader.setVec3(point_light + "ambient", pointLight.ambient);
        lights_shader.setVec3(point_light + "diffuse", pointLight.diffuse);
        lights_shader.setVec3(point_light + "specular", pointLight.specular);
        lights_shader.setFloat(point_light + "constant", pointLight.constant);
        lights_shader.setFloat(point_light + "linear", pointLight.linear);
        lights_shader.setFloat(point_light + "quadratic", pointLight.quadratic);
    }

    // spotlight setup
    lights_shader.setVec3("spotLight.position", camera.Position);
    lights_shader.setVec3("spotLight.direction", camera.Front);
    lights_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    lights_shader.setVec3("spotLight.diffuse", 0.7f, 0.7f, 0.7f);
    lights_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    lights_shader.setFloat("spotLight.constant", 1.0f);
    lights_shader.setFloat("spotLight.linear", 0.05);
    lights_shader.setFloat("spotLight.quadratic", 0.012);
    lights_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.5f)));
    lights_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(13.0f)));
}

void draw_skybox(const Skybox& skybox, const ProgramState& state) {
    const FrameMemo& frame_metadata = state.frame_memo;
    auto skybox_view = glm::mat4(glm::mat3(frame_metadata.view));
    skybox.render(glm::mat4(1.0f), skybox_view, frame_metadata.projection);
}

void draw_floor(const Floor& floor, const ProgramState& state) {
    const FrameMemo& frame_metadata = state.frame_memo;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.51f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(25.0f));
    floor.render(model, frame_metadata.view, frame_metadata.projection);
}

void draw_building(const Model& building_model, const Shader& shader, const ProgramState& state) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, state.building_position); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(state.building_scale));    // it's a bit too big for our scene, so scale it down
    shader.use();
    shader.setMat4("model", model);
    building_model.Draw(shader);
}
