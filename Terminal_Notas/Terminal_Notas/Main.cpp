#include <iostream>
#include <fstream>
#include <string.h>
#include <tchar.h>
#include <windows.h>
#include <shlobj.h> // Para SHGetFolderPath

using namespace std;

/// <Constantes>
const int LS = 50; // Tamanho limite de strings

#ifndef UNLEN
#define UNLEN 256
#endif

extern "C" {
	WINBASEAPI BOOL WINAPI GetUserNameA(LPSTR, LPDWORD);
}

/// <Protótipos>
void chamaMenu();
int criarNota(_TCHAR  directoryPath[LS]);
void getUserName(char diretorioDeCriacao[]);
void geraDiretorio(char username[], char diretorioDeCriacao[]);
int dir(const _TCHAR* directoryPath);

/// <Objetos>
ofstream fout;
ifstream fin;

/// <Funções>
int main() {
	chamaMenu();

	return 0;
}

void chamaMenu() {
	int opcao;
	_TCHAR directoryPath[LS];
	do {
		system("CLS");
		cout << "\tMenu de Opcoes" << endl
			<< " 1 - Criar nova nota" << endl
			<< " 2 - Excluir nota" << endl
			<< " 3 - Editar nota" << endl
			<< " 4 - Listar todas as notas" << endl
			<< " 5 - Sair" << endl
			<< " | Selecione uma opcao: ";
		cin >> opcao;
		switch (opcao) {
		case 1:
			criarNota(directoryPath);
			break;
		case 4:
			dir(directoryPath);
			break;
		case 5:
			break;

		default:
			cerr << "Opcao invalida. Tente novamente!" << endl;
		}
		system("PAUSE");
	} while (opcao != 5);
}

int criarNota(_TCHAR directoryPath[LS]) {
	char titulo[LS];
	char diretorioDeCriacao[LS];
	getUserName(diretorioDeCriacao);
	cout << " | Qual o titulo da nota que deseja criar?" << endl;
	cin >> titulo;
	strcat(diretorioDeCriacao, titulo);
	strcat(diretorioDeCriacao, ".bin");
	fout.open(diretorioDeCriacao, ios::binary);
	int tamanhoTitulo = strlen(titulo);
	fout.write(titulo, tamanhoTitulo);
	cout << "Nota criada com sucesso!";
	fout.close();


	// Obtendo o caminho do Desktop do usuário
	if (SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, directoryPath) != S_OK) {
		cerr << "Erro ao obter o diretório do Desktop" << endl;
		return -1;
	}
}

void alteraNota() {
	char titulo[LS];
	cout << " | Digite o titulo da nota que deseja alterar:";
	cin >> titulo;
}

int dir(const _TCHAR* directoryPath) {
	const std::basic_string<_TCHAR> targetExtension = _T(".bin");

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((std::basic_string<_TCHAR>(directoryPath) + _T("\\*")).c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		std::cerr << "Erro ao abrir o diretório" << std::endl;
		return -1;
	}

	do {
		std::basic_string<_TCHAR> fileName = findFileData.cFileName;
		size_t extPos = fileName.find_last_of(_T('.'));

		if (extPos != std::basic_string<_TCHAR>::npos) {
			std::basic_string<_TCHAR> fileExtension = fileName.substr(extPos);

			if (fileExtension == targetExtension) {
				std::wcout << findFileData.cFileName << std::endl;
			}
		}
	} while (FindNextFile(hFind, &findFileData) != 0);

	FindClose(hFind);
	return 0;
}

// Altera a string, adicionando o nome do usuário
void getUserName(char diretorioDeCriacao[]) {
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;

	if (!GetUserNameA(username, &username_len)) {
		cerr << "Erro ao obter o nome do usuario" << endl;
		return;
	}
	strcpy(diretorioDeCriacao, username);
	geraDiretorio(username, diretorioDeCriacao);
}

// Define todo o caminho onde o arquivo será salvo
void geraDiretorio(char username[], char diretorioDeCriacao[]) {
	strcpy(diretorioDeCriacao, "");
	strcat(diretorioDeCriacao, "C:\\Users\\");
	strcat(diretorioDeCriacao, username);
	strcat(diretorioDeCriacao, "\\Desktop\\");
}