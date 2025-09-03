# Projeto 01 de computação visual

## Desenvolvimento e Compilação

### Linux
A compilação desse programa em linux só ira funcionar em uma máquina linux com X11 ou Wayland (Todas as distros gráficas já vem com um desses dois). **WSL não vai funcionar, a não ser que você instale um Desktop Environment/Window Manager nele**
1. Instale as depêndencias necessárias (https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md)
2. Instale o cmake 
3. No diretorio root do projeto, crie uma pasta *lib/* e clone o repositório do SDL3 para a pasta SDL3
```
mkdir lib && cd lib
git clone https://github.com/libsdl-org/SDL SDL3
```
4. no diretorio root do projeto, rode os seguintes comandos para compilar:
```
cmake -S . -B build
cmake --build build
```
5. O executavel estará em `build/main`

### Windows
*PS. não seja masoquista e use o Visual Studio para desenvolvimento de C/C++ em windows, puta cambiarra fazer funcionar dessa forma*
1. Siga os passos descritos pelo professor no video "Configuração da máquina e do vscode para compilar código C que usa a biblioteca SDL3." da Semana 2 disponivel no moddle, ou siga os passos descritos em https://www.msys2.org/
2. Adicione o seguinte caminho ao seu PATH do windows `C:\msys64\ucrt64\bin` (caso tenha instalado o msys64 em outro caminho, modifique).
3. No diretorio do projeto, crie uma pasta *lib*
4. Baixe a release "SDL3-devel-3.2.22-mingw.zip" do repositorio do SDL3, descompacte e renomeie o arquivo "x86_64-w64-mingw32" para "SDL3"
5. Coloque essa pasta dentro da pasta *lib*
6. instale o Gnu Make da forma que preferir (scoop, chocolatey, etc)
7. Na pasta root do projeto, rode o seguinte comando:
```
make
```
8. O Executavel estara na pasta root do projeto como `main.exe`

## TODOs

1. Passar o CMakeLists.txt e Makefile para c++
2. Tentar fazer o Windows usar CMake também ao invez do Makefile
3. Fazer o Projeto