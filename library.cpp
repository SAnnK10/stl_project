#include "iostream"
#include "fstream"
#include "string"
#include "cstring"
#include "sstream"
#include "map"
#include "set"
#include "algorithm"
#include "ctime"
#include "random"
#include "iomanip"
#include "vector"
#include "nlohmann\json.hpp"

using json = nlohmann::json;
using namespace std;

struct Stru
{
    string name;
    int primary;
    int secondary;
};

vector<Stru> loadDataFromJson(const string& filename) {
    vector<Stru> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return data; // Возвращаем пустой вектор в случае ошибки
    }

    json jsonData;
    try {
        file >> jsonData; // Читаем JSON из файла
    } catch (const json::parse_error& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
        return data; // Возвращаем пустой вектор в случае ошибки
    }

    for (const auto& item : jsonData) {
        Stru s;
        try {
            s.name = item["name"].get<string>();
            s.primary = item["primary"].get<int>();
            s.secondary = item["secondary"].get<int>();
            data.push_back(s);
        } catch (const json::type_error& e) {
            cerr << "Error reading data from JSON: " << e.what() << endl;
            continue; // Пропускаем элемент с ошибкой
        }
    }

    return data;
}

void displayList(const vector<Stru>& list){
    for (size_t i = 0; i < list.size(); i++) {
        const auto& s = list[i];
        cout << "[" << i + 1 << "] Name: " << s.name << ", Primary: " << s.primary << ", Secondary: " << s.secondary << endl;
    }
}

void displaySet(const set<string>& see)
{
    cout << "Set: " << endl;
    for (size_t j = 0; j < see.size(); j++)
    {   
        auto iterSet = see.begin();
        advance(iterSet, j);
        cout << "[" << j + 1 << "]" << *iterSet << endl; 
    }
}

int main()
{   

    set<string> lib;
    string str;
    string alf;
    string word;
    map<string, unsigned> voc;
    
    int r, mult;

    // Шаг №1
    ifstream in("text.txt");
    if (in.is_open())
    {
        while(getline(in, str)){
            stringstream s(str);
            while (s >> alf)
            {
                lib.insert(alf);
            }
        }
    }else{
        cerr << "Error of file openning!" << endl;
        return 1;
    }
    in.close();
    displaySet(lib);
    // Шаг №2
    ifstream in2("text.txt");
    if (in2.is_open())
    {
        while(in2 >> word)
            voc[word]++;
    }else{
        cerr << "Error of file openning!" << endl;
        return 1;
    }
    in2.close();

    // Шаг №3
    srand(time(0));
    vector<Stru> l(100);
    for(int k = 0; k < 100; k++)
    {   
        r = rand() % lib.size();
        auto setItr = lib.begin();
        advance(setItr, r);
        l[k].name = *setItr;

        mult = 1;
        for(int d = 0; d < 3; d++)
        {

            r = rand() % voc.size(); 
            auto mapItr = voc.begin(); 
            advance(mapItr, r);
            mult *= mapItr -> second;
        }
        l[k].primary = mult;

        r = rand() % voc.size();
        auto mapItr = voc.begin();
        advance(mapItr, r);
        l[k].secondary = mapItr -> second * mapItr -> second;
    }

    // Шаг №4
    sort(l.begin(), l.end(), [](const Stru& a, const Stru& b) {
        return a.secondary < b.secondary;
    });

    // Шаг №5
    int minSecondary = l[0].secondary;
    vector<Stru> minSecondaryList;
    copy_if(l.begin(), l.end(), back_inserter(minSecondaryList), [&minSecondary](const Stru& s) {
        return s.secondary == minSecondary;
    });

    // Шаг №6
    json jsonData = json::array();
    for (const auto& s : minSecondaryList) 
    {
        json item;
        item["name"] = s.name;
        item["primary"] = s.primary;
        item["secondary"] = s.secondary;
        jsonData.push_back(item);
    }

    ofstream outputFile("output.json");
    if (outputFile.is_open()) {
        outputFile << setw(4) << jsonData << endl; // setw(4) для красивого форматирования
        outputFile.close();
        cout << "JSON data written to output.json" << endl;
    } else {
        cerr << "Error opening output.json for writing" << endl;
    }

    // Шаг №7
    string JsonFilename = "output.json";

    vector<Stru> loadedData = loadDataFromJson(JsonFilename);

    random_device rd2;
    mt19937 g(rd2());
    shuffle(loadedData.begin(), loadedData.end(), g); // Перетасовываем загруженный список
    cout << "Shuffled data from JSON:" << endl;
    displayList(loadedData);

    return 0;
}