#include<iostream>
#include<fstream>
#include <string>
#include<vector>
#include<tuple>
#include<experimental/filesystem>
using namespace std;
namespace fs = std::experimental::filesystem;

vector<tuple<string, bool>> generate_list_input();
vector<tuple<string, bool>> generate_list_auto();
void generate_make_file(vector<tuple<string, bool>> files);
void print_data(vector<tuple<string, bool>> files);

int main(){
    vector<tuple<string, bool>> files;
    string option;
    cout << "Welcome to makefile generator" << endl;
    cout << "You can either input your file names manually," << endl;
    cout << "or you can auto generate one based on the files that are the directory!" << endl;
    while(option != "Y" && option != "N"&& option != "y" &&option != "n"){
        cout << "Input filenames manually? (Y/N)" << endl;
        cin >> option;
        if(option != "Y" && option != "N" && option != "y" && option != "n"){
            cout << "Please enter a valid option" << endl;
        }
    }
    if(option == "Y" || option == "y"){
        files = generate_list_input();
        generate_make_file(files);
    }
    else{
        files = generate_list_auto();
        generate_make_file(files);
    }
}

vector<tuple<string, bool>> generate_list_input(){
    vector<tuple<string, bool>> files;
        string f = "";
        while(f != "#"){
            cout << "Input filename without extension (# to exit)" << endl;
            cin >> f;
            
            bool h;
            string h_s;
            while(h_s != "Y" && h_s != "N" && h_s != "y" && h_s != "n"){
                if(f == "#")
                    break;
                cout << "Does this file have a corresponding header file? (Y/N)" << endl;
                cin >> h_s;
                
                if(h_s != "Y" && h_s != "N" && h_s != "y" && h_s != "n"){
                    cout << "Please enter a valid argument" << endl;
                }
            }
            if(h_s == "Y" || h_s == "y") h = true;
            else h = false;
            files.push_back(make_tuple(f, h));
        }
        files.pop_back();
    
    return files;
}

vector<tuple<string, bool>> generate_list_auto(){
    vector<tuple<string, bool>> files;

    vector<string> file_raw;
    const fs::path p = fs::current_path();
    string path_s = p.string();
    for(const auto & file: fs::directory_iterator(p))
        file_raw.push_back(file.path().string().substr(path_s.length()+1));
    
    //Populate the files list
    for(int i =0; i < file_raw.size(); i++){
        if(file_raw[i].find(".") != -1){
            string file_name = file_raw[i].substr(0, file_raw[i].find("."));
            string extension = file_raw[i].substr(file_raw[i].find("."));
            if(extension == ".cpp"){
                bool found = false;
                //Check for corresponding .h file
                for(int j = 0; j< file_raw.size(); j++){
                    if(file_raw[j].find(".") != -1){
                        string file_name2 = file_raw[j].substr(0, file_raw[j].find("."));
                        string extension2 = file_raw[j].substr(file_raw[j].find("."));
                        if(file_name2 == file_name && extension2 == ".h"){
                        found = true;
                        }
                        if(found){
                        break;
                        }
                    }
                }
                files.push_back(make_tuple(file_name, found));
            }
        }
    }
    return files;
}

void generate_make_file(vector<tuple<string, bool>> files){
    ofstream makefile("Makefile");
    //Generate the output dependencies
    makefile << "output: ";
    string dependencies;
    for(int i =0; i < files.size(); i++){
        string temp = get<0>(files[i]);
        dependencies += temp+".o "; //Add the object extension
    }
    makefile << dependencies;
    makefile << "\n\tg++ " + dependencies + " -o output\n\n";

    //Generate object dependencies
    for(int i=0; i < files.size(); i++){
        string name = get<0>(files[i]);
        makefile << name + ".o: " + name+".cpp ";
        if(get<1>(files[i])){
            makefile << name + ".h\n";
        }
        else{
            makefile << "\n";
        }

        //Generate the action
        makefile << "\tg++ -c " + name + ".cpp\n\n"; 
    }
    //Clean directory command
    makefile << "clean:\n\trm *.o output";
}

void print_data(vector<tuple<string, bool>> files){
    for(int i=0; i < files.size();i++){
        cout << get<0>(files[i]) << "," << get<1>(files[i]) << endl;
    }
}