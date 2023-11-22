#include <iostream>
#include <fstream>
#include <string.h>
#include <tchar.h>
#include <windows.h>
#include <shlobj.h> // Para SHGetFolderPath

using namespace std;

/// <Constantes e globais>
const int LS = 50; // Tamanho limite de strings
char diretorioDeCriacao[LS];

#ifndef UNLEN // Verifica se UNLEN já foi definido
#define UNLEN 256
#endif

extern "C" { 
	WINBASEAPI BOOL WINAPI GetUserNameA(LPSTR, LPDWORD); // Obter o nome do usuário associado ao processo
}

/// <Protótipos>
void chamaMenu();
int criarNota(_TCHAR  directoryPath[LS]);
void getUserName(char diretorioDeCriacao[]);
void geraDiretorio(char username[], char diretorioDeCriacao[]);
int dir(const _TCHAR* directoryPath);
void excluirArquivo(char diretorioDeCriacao[]);

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
	_TCHAR directoryPath[LS]; //Armazena o diretório do desktop do usuário atual
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
		case 2:
			excluirArquivo(diretorioDeCriacao);
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
	getUserName(diretorioDeCriacao); // 
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

	cout << " | Notas no diretorio: " << endl;

	const basic_string<_TCHAR> targetExtension = _T(".bin"); // Definindo a extensão dos arquivos

	WIN32_FIND_DATA findFileData; // Struct para armazenar os dados do arquivo encontrado
	HANDLE hFind = FindFirstFile((basic_string<_TCHAR>(directoryPath) + _T("\\*")).c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		cerr << "Erro ao abrir o diretorio" << endl;
		return -1;
	}

	do { // Fazendo a iteração sobre os arquivos do diretório
		basic_string<_TCHAR> fileName = findFileData.cFileName; // Obtém o nome do arquivo atual
		size_t extPos = fileName.find_last_of(_T('.')); // Encontra a posição do último ponto no nome do arquivo. Usado para identificar a extensão

		if (extPos != basic_string<_TCHAR>::npos) { // Se a extensão existe
			basic_string<_TCHAR> fileExtension = fileName.substr(extPos); // armazena ela em fileExtension

			if (fileExtension == targetExtension) { // Compara a extensão do arquivo com a extensão definida acima
				wcout << findFileData.cFileName << endl;
			}
		}
	} while (FindNextFile(hFind, &findFileData) != 0); // Executa enquanto existir arquivos a serem analisados

	FindClose(hFind); // Fecha o identificador
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

// Define o caminho onde o arquivo será salvo
void geraDiretorio(char username[], char diretorioDeCriacao[]) {
	strcpy(diretorioDeCriacao, "");
	strcat(diretorioDeCriacao, "C:\\Users\\");
	strcat(diretorioDeCriacao, username);
	strcat(diretorioDeCriacao, "\\Desktop\\");
}

void excluirArquivo(char diretorioDeCriacao[]) {
	char arquivo_a_Excluir[LS];
	char username[LS];
	cout << " | Digite o nome do arquivo que deseja excluir [Sem a extensao]:";
	cin >> arquivo_a_Excluir;
	cout << " | Tem certeza?" << endl
		<< " 1 - Sim\t2 - Nao" << endl;
	int opcao;
	cin >> opcao;
	switch (opcao) {
	case 1:
		cout << " | Seguindo com a exclusao" << endl;
		break;
	case 2:
		cout << " | Exclusao cancelada" << endl;
		system("PAUSE");
		return;
	default:
		cerr << " | Opcao invalida. Cancelando." << endl;
		return;
	}
	getUserName(diretorioDeCriacao);
	strcat(diretorioDeCriacao, arquivo_a_Excluir);
	strcat(diretorioDeCriacao, ".bin");
	strcpy(arquivo_a_Excluir, diretorioDeCriacao); // arquivo_a_Excluir agora armazena o diretório completo, incluindo o nome e extensão
	cout << arquivo_a_Excluir << endl;
	// Verifica se o arquivo existe antes de tentar excluir
	if (remove(arquivo_a_Excluir) != 0) { 
		cerr << "Erro ao excluir o arquivo." << endl;
		return;
	}
	cout << "Arquivo excluido com sucesso." << endl;
}