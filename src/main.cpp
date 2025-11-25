#include "../libraries/include/Application/Application.h"

int main(){
    Application Game;
    Game.GenerateWorld();
    Game.Initialize();
    Game.SetWindow();
    Game.SetBuffers();
    Game.SetTexture() ;
    Game.SetShaders();
    Game.SetCamera();
    Game.Run();
    return 0;
}
