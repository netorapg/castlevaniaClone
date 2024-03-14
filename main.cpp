#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>



//Constantes
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_WIDTH = 32;
const int PLAYER_HEIGHT = 64;
const int GRAVITY = 1;

//Estrutura do jogador
struct Player {
    SDL_Rect rect;
    float xVel;
    float yVel;
    bool isJumping;
};

//Função que inicializa o SDL e a janela
bool init(SDL_Window*& window, SDL_Renderer*& renderer){
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }
    window = SDL_CreateWindow("Castlevania Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == nullptr) {
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        return false;
    }
    return true;
}

// Função para carregar recursos do jogo
bool loadMedia(SDL_Texture*& playerTexture, SDL_Renderer* renderer) {
    //Carregar a textura do jogador
    SDL_Surface* playerSurface = SDL_LoadBMP("Vikings_walkabout_000.bmp");
    if (playerSurface == nullptr) {
        return false;
    }
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);
    return true;
}

// Função para processar eventos
void handleEvents(Player& player, bool& quit) {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            quit = true;
        }
        else if(e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_SPACE && !player.isJumping) {
                player.isJumping = true;
                player.yVel = -10; //Velocidade inicial do pulo
            }
        }
    }   
}

// Função para atualizar a lógica do jogo
void update(Player & player) {
    // Aplicar a gravidade
    player.yVel += GRAVITY;
    player.rect.y += player.yVel;

    // Verificar e ajustar colisões com o chão
    if (player.rect.y >= SCREEN_HEIGHT - PLAYER_HEIGHT) {
        player.rect.y = SCREEN_HEIGHT - PLAYER_HEIGHT;
        player.yVel = 0;
        player.isJumping = false;
    }
}

// Funçãoi para renderizar o jogo
void render(SDL_Renderer* renderer, SDL_Texture* playerTexture, Player& player) {
    //Limpar a tela
    SDL_RenderClear(renderer);
    //Renderizar o jogador
    SDL_RenderCopy(renderer, playerTexture, nullptr, &player.rect);
    //Atualizar a tela
    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]){
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* playerTexture = nullptr;
    Player player;

    if (!init(window, renderer)){
        printf("Failed to initialize!\n");
        return -1;
    }

    if(!loadMedia(playerTexture, renderer)) {
        printf("Failed to load media!\n");
        return -1;
    }

    bool quit = false;
    while (!quit) {
        handleEvents(player, quit);
        update(player);
        render(renderer, playerTexture, player);
        SDL_Delay(16); // Delay para atingir aproximadamente 60 quadros por segundo
    }
    //Limpar recursos e sair
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;


}