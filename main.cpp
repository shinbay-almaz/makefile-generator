#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

vector <string> getListOfObjFiles(fs::path path) {

	vector <string> objFiles;

	for (auto const& dir_entry : fs::directory_iterator{path}) {
		string extension = dir_entry.path().extension();

		if (extension == ".c") {
			objFiles.push_back(string(dir_entry.path().stem()) + ".o");
		}
	}

	return objFiles;
}

vector <string> getListOfDepsFiles(fs::path path) {
	vector <string> depsFiles;

	for (auto const& dir_entry : fs::directory_iterator{path}) {
		string extension = dir_entry.path().extension();

		if (extension == ".h") {
			depsFiles.push_back(string(dir_entry.path().filename()));
		}
	}

	return depsFiles;
}

void generateMakefile(fs::path path) {

	string filename = string(path) + "/Makefile";

	ofstream makefile(filename);

	makefile << "# Auto-Generated Makefile\n\n";

	makefile << "CC = gcc\nCFLAGS = -Wall -std=c11\n";
	
	vector <string> objFiles = getListOfObjFiles(path);
	vector <string> depsFiles = getListOfDepsFiles(path);
	
	makefile << "OBJ = ";

	for (auto objFile : objFiles) {
		makefile << objFile << " ";
	}

	makefile << "\nDEPS = ";

	for (auto depsFile : depsFiles) {
		makefile << depsFile << " ";
	}

	makefile << "\n\n";

	makefile << "\%.o : \%.c $(DEPS)\n\t$(CC) $(CFLAGS) -c -o $@ $<\n\n";

	makefile << "run: $(OBJ)\n\t$(CC) $(CFLAGS) -o run $^\n";

	makefile.close();

}

int main(int argc, char** argv) {
	
	if (argc == 1) {
		generateMakefile(fs::current_path());
	} else if (argc == 2) {
		generateMakefile(fs::path(argv[1]));
	} else {
		cout << "Usage: generate [path]\n";
	}
}
