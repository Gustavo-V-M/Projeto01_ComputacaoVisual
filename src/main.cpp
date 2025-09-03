// Copyright (c) 2025 Andre Kishimoto - https://kishimoto.com.br/
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

using namespace std;

void handle_exit()
{
  SDL_Log("Exiting...\n");
  SDL_Quit();
}

int main(int argc, char *argv[])
{

  atexit(handle_exit);

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_LogError(0, "Unable to initialize SDL: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}