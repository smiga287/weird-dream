#include <program_state.h>
#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <Skybox.h>
#include <Floor.h>

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

    // point light setup
    lights_shader.setVec3("pointLight.position", pointLight.position);
    lights_shader.setVec3("pointLight.ambient", pointLight.ambient);
    lights_shader.setVec3("pointLight.diffuse", pointLight.diffuse);
    lights_shader.setVec3("pointLight.specular", pointLight.specular);
    lights_shader.setFloat("pointLight.constant", pointLight.constant);
    lights_shader.setFloat("pointLight.linear", pointLight.linear);
    lights_shader.setFloat("pointLight.quadratic", pointLight.quadratic);

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
