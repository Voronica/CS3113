//
//  Menu.hpp
//  SDLProject
//
//  Created by Shucong Zhou on 2020/7/16.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Scene.h"


class Menu : public Scene {
public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
