#ifndef FUTURAMA_DRAWING_H
#define FUTURAMA_DRAWING_H

struct ProgramState;
class Shader;
class Floor;

void setup_lighting(const Shader& lights_shader, const ProgramState& state);
void draw_skybox(const Skybox& skybox, const ProgramState& state);
void draw_floor(const Floor& floor, const ProgramState& state);
void draw_building(const Model& building_model, const Shader& shader, const ProgramState& state);

#endif //FUTURAMA_DRAWING_H
