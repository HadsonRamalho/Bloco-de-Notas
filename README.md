# Bloco-de-Notas
Este é um programa em C++ para criar, excluir, editar e listar notas de arquivo. O programa utiliza arquivos binários para armazenar as notas e organiza os arquivos no Desktop do usuário.

##Funcionalidades
Criar Nova Nota: Permite ao usuário criar uma nova nota. O título da nota é solicitado, e um arquivo binário com esse título é criado no Desktop do usuário.

Excluir Nota: Permite ao usuário excluir uma nota existente. O usuário precisa digitar o nome da nota (sem a extensão) e confirmar a exclusão.

Editar Nota: Permite ao usuário editar o conteúdo de uma nota existente. O usuário digita o título da nota, e o programa abre o arquivo para edição.

Listar Todas as Notas: Lista todas as notas presentes no Desktop do usuário.

##Estrutura do Projeto
main.cpp: Contém todo o código do programa, incluindo o menu e a chamada das funções correspondentes a cada opção.

##Como Compilar e Executar
Certifique-se de ter um compilador C++ instalado em seu sistema. Você pode compilar e executar o programa usando um terminal:

```
g++ main.cpp -o main.exe
./main.exe
```
Certifique-se de ajustar os comandos conforme o sistema operacional que você está utilizando.

##Observações
O programa utiliza arquivos binários (.bin) para armazenar as notas.
As notas são organizadas no Desktop do usuário.
Certifique-se de que as bibliotecas necessárias, como <iostream>, <fstream>, <string>, <tchar.h>, <windows.h>, e <shlobj.h>, estão disponíveis no seu ambiente de desenvolvimento.
Sinta-se à vontade para usar, modificar e contribuir para este projeto!
