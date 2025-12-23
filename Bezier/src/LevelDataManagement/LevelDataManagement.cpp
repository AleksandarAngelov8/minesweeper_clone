#include "LevelDataManagement.h"
#include "Scene/Game/Game.h"
#include <TextureInit/TextureInit.h>
#include "Game/Quote.h"
#include "Scene/Edit/Edit.h"

std::vector<std::string> LevelDataManagement::SplitString(const std::string& input, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(input);
	std::string token;

	while (std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}
void LevelDataManagement::SaveGameState(std::shared_ptr<Game> gamestate)
{
    std::ofstream outfile("SaveData/Save1.txt");

    outfile << "CharacterInfo," << gamestate->character->translation.x << "," << gamestate->character->translation.y << "," 
        << gamestate->character->stats->maxHealth<< "," << gamestate->character->stats->currentHealth << ","
        << gamestate->character->stats->attackDamage<< "," << gamestate->character->stats->level << ","
        << gamestate->character->stats->experience << "\n";
    //std::cout <<  "CharacterInfo," << gamestate->character->translation.x << "," << gamestate->character->translation.y << ","
    //    << gamestate->character->stats->maxHealth << "," << gamestate->character->stats->currentHealth << ","
    //    << gamestate->character->stats->attackDamage << "," << gamestate->character->stats->level << ","
    //    << gamestate->character->stats->experience << "\n";

    for (const auto& enemy : gamestate->enemies) {
        outfile << "Enemy," << enemy.first << "," << enemy.second->stats->currentHealth << "," << enemy.second->alive << "\n";
    }

    outfile.close();
    printf("Saved");
}
void LevelDataManagement::LoadGameState()
{
    std::shared_ptr<Scene> gameScene = std::make_shared<Game>();
    std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(gameScene);
    game->SetUp();
    std::string line;
    std::ifstream gameData("SaveData/Save1.txt");
    try
    {
        if (gameData.is_open())
        {
            while (std::getline(gameData, line)) {
                std::vector<std::string> gameInfo = LevelDataManagement::SplitString(line, ',');
                if (gameInfo[0] == "CharacterInfo") {
                    float xpos = std::stof(gameInfo[1]), ypos = std::stof(gameInfo[2]), maxhp = std::stof(gameInfo[3]), currhp= std::stof(gameInfo[4]), ad = std::stof(gameInfo[5]);
                    unsigned int lvl = std::stoul(gameInfo[6]); 
                    int ex = std::stoi(gameInfo[7]);
                    std::shared_ptr<Character> character = std::make_shared<Character>(
                        xpos,ypos,(int)(LevelDataManagement::cellSize), (int)(LevelDataManagement::cellSize) * 3 / 2,"isadora",glm::vec2(4,4), gameScene, maxhp, currhp, ad, lvl, ex);
                    printf("Character before load: %f", game->character->stats->currentHealth);
                    std::dynamic_pointer_cast<Game>(game)->character = character;
                    printf("\nLoaded character data: %f/%f",currhp,maxhp);
                }
                else if (gameInfo[0] == "Enemy") {
                    std::shared_ptr<Enemy> enemy = game->enemies[gameInfo[1]];
                    enemy->stats->currentHealth = std::stoi(gameInfo[2]);
                    if (std::stoi(gameInfo[3])==0) enemy->Kill();
                }
            }
        }
        else throw;
    }
    catch (const std::exception&)
    {
        std::cout << "Level data not found!";
    }
    game->InitializeEnemies();
    ButtonFunctionality::ChangeSceneFromFile(gameScene,"game");
}
void LevelDataManagement::EditLevelTextData(std::set<std::pair<unsigned int, unsigned int>> cells, unsigned int texid, std::shared_ptr<Scene> scene)
{
    std::ifstream infile("LevelData/Level1SaveFile.txt");
    if (!infile.is_open()) {
        throw;
    }
    std::string line;
    std::vector<std::vector<std::string>> data;
read:
    try{
        unsigned int i = 0, j = 0;
        data.push_back(std::vector<std::string>());
        while (std::getline(infile, line)) {
            std::vector<std::string> lineData = LevelDataManagement::SplitString(line,',');
            if (j > NUMBER_OF_CELLS_X-1) {
                data.push_back(std::vector<std::string>());
                j = 0;
                i++;
            }
            std::string rest;
            if (cells.find(std::make_pair(j, i)) != cells.end())
            {
                rest += std::to_string(texid);
            }
            else {
                rest += std::to_string(std::stoi(lineData[2]));
            }
            for (int t = 3; t < lineData.size(); t++) {
                rest += ","+lineData[t];
            }
            data[i].push_back(rest);
            j++;
        }
    }
    catch (const std::exception&) {
        CreateTextData();
        goto read;
    }
    infile.close();
    std::ofstream outfile("LevelData/Level1SaveFile.txt");
    for (unsigned int i = 0; i < NUMBER_OF_CELLS_Y; i++) {
        for (unsigned int j = 0; j < NUMBER_OF_CELLS_X; j++) {
            outfile << i << "," << j << "," << data[i][j] << "\n";
        }
    }

    outfile.close();
}
void LevelDataManagement::EditLevelTextData(std::set<std::pair<unsigned int, unsigned int>> cells, std::shared_ptr<Scene> scene, std::string textureName, float maxHP, float AD,unsigned int level, std::string name){
    std::ifstream infile("LevelData/Level1SaveFile.txt");
    if (!infile.is_open()) {
        throw;
    }
    std::string line;
    std::vector<std::vector<std::string>> data;
read:
    try {
        unsigned int i = 0, j = 0;
        data.push_back(std::vector<std::string>());
        while (std::getline(infile, line)) {
            std::vector<std::string> lineData = LevelDataManagement::SplitString(line, ',');
            if (j > NUMBER_OF_CELLS_X - 1) {
                data.push_back(std::vector<std::string>());
                j = 0;
                i++;
            }
            std::string rest = "";
            rest += std::to_string(std::stoi(lineData[2]));
            if (cells.find(std::make_pair(i, j)) != cells.end())
            {
                rest += "," + std::string("enemy");
                rest += "," + std::to_string(TextureInit::textureMapSToI[textureName]);
                rest += "," + std::to_string(static_cast<int>(maxHP));// this should be float when needed
                rest += "," + std::to_string(static_cast<int>(AD));   // this should be float when needed
                rest += "," + std::to_string(static_cast<int>(level));// this should be float when needed
                rest += "," + name;
                printf("\n%s\n",rest.c_str());
            }
            else {
                for (int t = 3; t < lineData.size(); t++) {
                    rest += "," + lineData[t];
                }
            }
            data[i].push_back(rest);
            j++;
        }
    }
    catch (const std::exception&) {
        CreateTextData();
        goto read;
    }
    infile.close();
    std::ofstream outfile("LevelData/Level1SaveFile.txt");
    for (unsigned int i = 0; i < NUMBER_OF_CELLS_Y; i++) {
        for (unsigned int j = 0; j < NUMBER_OF_CELLS_X; j++) {
            outfile << i << "," << j << "," << data[i][j] << "\n";
        }
    }

    outfile.close();
}
void LevelDataManagement::EditLevelTextData(std::set<std::pair<unsigned int, unsigned int>> cells, std::shared_ptr<Scene> scene, std::string textureName, std::vector<std::string> quotes, std::string name)
{
    std::ifstream infile("LevelData/Level1SaveFile.txt");
    if (!infile.is_open()) {
        throw;
    }
    std::string line;
    std::vector<std::vector<std::string>> data;
read:
    try {
        unsigned int i = 0, j = 0;
        data.push_back(std::vector<std::string>());
        while (std::getline(infile, line)) {
            std::vector<std::string> lineData = LevelDataManagement::SplitString(line, ',');
            if (j > NUMBER_OF_CELLS_X - 1) {
                data.push_back(std::vector<std::string>());
                j = 0;
                i++;
            }
            std::string rest = "";
            rest += std::to_string(std::stoi(lineData[2]));
            if (cells.find(std::make_pair(i, j)) != cells.end())
            {
                rest += "," + std::string("npc");
                rest += "," + std::to_string(TextureInit::textureMapSToI[textureName]) + ",";
                for (int i = 0; i < quotes.size(); i++) {
                    rest += quotes[i];
                    if (i != quotes.size() - 1) rest += ";";
                }
                rest += "," + name;
                printf("\n%s\n", rest.c_str());
            }
            else {
                for (int t = 3; t < lineData.size(); t++) {
                    rest += "," + lineData[t];
                }
            }
            data[i].push_back(rest);
            j++;
        }
    }
    catch (const std::exception&) {
        CreateTextData();
        goto read;
    }
    infile.close();
    std::ofstream outfile("LevelData/Level1SaveFile.txt");
    for (unsigned int i = 0; i < NUMBER_OF_CELLS_Y; i++) {
        for (unsigned int j = 0; j < NUMBER_OF_CELLS_X; j++) {
            outfile << i << "," << j << "," << data[i][j] << "\n";
        }
    }

    outfile.close();
}
void LevelDataManagement::EditLevelTextData(std::set<std::pair<unsigned int, unsigned int>> cells)
{
    std::ifstream infile("LevelData/Level1SaveFile.txt");
    if (!infile.is_open()) {
        throw;
    }
    std::string line;
    std::vector<std::vector<std::string>> data;
read:
    try {
        unsigned int i = 0, j = 0;
        data.push_back(std::vector<std::string>());
        while (std::getline(infile, line)) {
            std::vector<std::string> lineData = LevelDataManagement::SplitString(line, ',');
            if (j > NUMBER_OF_CELLS_X - 1) {
                data.push_back(std::vector<std::string>());
                j = 0;
                i++;
            }
            std::string rest = "";
            rest += std::to_string(std::stoi(lineData[2]));
            if (cells.find(std::make_pair(i, j)) == cells.end()){
                for (int t = 3; t < lineData.size(); t++) {
                    rest += "," + lineData[t];
                }
            }
            data[i].push_back(rest);
            j++;
        }
    }
    catch (const std::exception&) {
        CreateTextData();
        goto read;
    }
    infile.close();
    std::ofstream outfile("LevelData/Level1SaveFile.txt");
    for (unsigned int i = 0; i < NUMBER_OF_CELLS_Y; i++) {
        for (unsigned int j = 0; j < NUMBER_OF_CELLS_X; j++) {
            outfile << i << "," << j << "," << data[i][j] << "\n";
        }
    }

    outfile.close();
}
void LevelDataManagement::EditLevelBinaryData(std::set<std::pair<unsigned int, unsigned int>> cells, unsigned int texid)
{
    std::ifstream infile("LevelData/Level1SaveFile.bin", std::ios::binary);
    if (!infile.is_open()) {
        throw;
    }
    std::vector<std::vector<unsigned int>> data;
    unsigned int i = 0, j = 0;
    data.push_back(std::vector<unsigned int>());
    while (true) {
        if (j > NUMBER_OF_CELLS_X - 1) {
            data.push_back(std::vector<unsigned int>());
            j = 0;
            i++;
        }
        unsigned int _y, _x, _texId;
        infile.read(reinterpret_cast<char*>(&_y), sizeof(_y));
        infile.read(reinterpret_cast<char*>(&_x), sizeof(_x));
        infile.read(reinterpret_cast<char*>(&_texId), sizeof(_texId));
        if (infile.eof())break;
        if (cells.find(std::make_pair(j,i)) != cells.end()) 
            data[i].push_back(texid);
        else 
            data[i].push_back(_texId);
        j++;
    }
    
    infile.close();
   
    std::ofstream outfile("LevelData/Level1SaveFile.bin", std::ios::binary);
    for (unsigned int i = 0; i < NUMBER_OF_CELLS_Y; i++) {
        for (unsigned int j = 0; j < NUMBER_OF_CELLS_X; j++) {
            outfile.write(reinterpret_cast<const char*>(&i), sizeof(i));
            outfile.write(reinterpret_cast<const char*>(&j), sizeof(j));
            outfile.write(reinterpret_cast<const char*>(&data[i][j]), sizeof(data[i][j]));
        }
    }
    outfile.close();
}
void LevelDataManagement::ReadLevelTextData(std::map<unsigned int, std::map<unsigned int, std::shared_ptr<Cell>>> &cells, std::shared_ptr<Scene> scene)
{
	std::string line;
	std::ifstream cellData("LevelData/Level1SaveFile.txt");
    if (cellData.is_open())
    {
        while (std::getline(cellData, line)) {
            std::vector<std::string> cellInfoS = LevelDataManagement::SplitString(line, ',');

            //std::transform(cellInfoS.begin(), cellInfoS.end(), std::back_inserter(cellInfoI),
            //	[](const std::string& str) { return std::atoi(str.c_str()); });

            unsigned int x = std::stoi(cellInfoS[0]), y = std::stoi(cellInfoS[1]), texId = std::stoi(cellInfoS[2]);

            if (!cells.count(x)) {
                cells.insert(std::make_pair(x, std::map<unsigned int, std::shared_ptr<Cell>>()));
            }
            if (cells[x].count(y)) {
                printf("Duplication of cells at %i-%i\n", x, y);
            }
            cells[x].insert(std::make_pair(y, std::make_shared<Cell>(y, x, texId)));

            if (cellInfoS.size() <= 3) continue;
            std::string spec = cellInfoS[3];
            if (spec == "enemy") {
                //0,6,1,enemy,6,50,10,1,enemy1
                std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(
                    LevelDataManagement::zoomCellSizes[4] * x, LevelDataManagement::zoomCellSizes[4] * y
                    , LevelDataManagement::zoomCellSizes[4], LevelDataManagement::zoomCellSizes[4], 
                    TextureInit::textureMapIToS[std::stoi(cellInfoS[4])], 4, 
                    std::stoi(cellInfoS[5]), std::stof(cellInfoS[6]), 
                    std::stoi(cellInfoS[7]), scene, 1, cellInfoS[8]); if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
                    game->enemies.emplace(cellInfoS[8], enemy);
                }
                if (std::shared_ptr<Edit> edit = std::dynamic_pointer_cast<Edit>(scene)) {
                    edit->enemies.emplace(cellInfoS[8], enemy);
                }
            }
            else if (spec == "npc") {
                std::vector<std::shared_ptr<Quote>> quotes;
                std::vector<std::string> quotesS = SplitString(cellInfoS[5], ';');
                for (const auto& quoteS : quotesS) {
                    quotes.push_back(std::make_shared<Quote>(quoteS.substr(1), (quoteS[0] == '>')));
                }
                std::shared_ptr<NPC> npc = std::make_shared<NPC>(LevelDataManagement::zoomCellSizes[4] * x, LevelDataManagement::zoomCellSizes[4] * y, LevelDataManagement::zoomCellSizes[4], LevelDataManagement::zoomCellSizes[4], TextureInit::textureMapIToS[std::stoi(cellInfoS[4])], 4, scene, quotes);
                if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
                    game->npcs.emplace(cellInfoS[6], npc);
                }
                if (std::shared_ptr<Edit> edit = std::dynamic_pointer_cast<Edit>(scene)) {
                    edit->npcs.emplace(cellInfoS[6], npc);
                }
            }
        }
    }
    else throw;
}
std::string LevelDataManagement::ReadCellDataFromTextFile(std::set<std::pair<unsigned int, unsigned int>> cells, const char* file){
    int count = 0;
    std::string line;
    std::ifstream cellData("LevelData/"+ std::string(file) +".txt");
    std::string result;
    try
    {
        if (cellData.is_open())
        {
            while (std::getline(cellData, line)) {
                if (count >= cells.size()) return result;

                std::vector<std::string> cellInfoS = LevelDataManagement::SplitString(line, ',');
                if (cells.find(std::make_pair(static_cast<unsigned int>(std::stoi(cellInfoS[0])), static_cast<unsigned int>(std::stoi(cellInfoS[1])))) == cells.end())continue;
                
                unsigned int y = std::stoi(cellInfoS[0]), x = std::stoi(cellInfoS[1]), texId = std::stoi(cellInfoS[2]);

                result += "Position:" + cellInfoS[0] + "/" + cellInfoS[1] + "\tTexture of cell: " + TextureInit::textureMapIToS[texId] + "\n";

                if (cellInfoS.size() <= 3) {
                    result += "no entities";
                    continue;
                }
                if (cellInfoS[3] == "enemy") {
                    result += "enemy\n";
                    result += "Texture of enemy: " + TextureInit::textureMapIToS[std::stoi(cellInfoS[4])] + "\nMaxHP: " + cellInfoS[5] + "\nAD: " + cellInfoS[6] + "\nLevel: " + cellInfoS[7] + "\nName: " + cellInfoS[8];
                }
                else if (cellInfoS[3] == "npc") {
                    result += "npc\n";
                    result += "Texture of npc: " + TextureInit::textureMapIToS[std::stoi(cellInfoS[4])];
                    result += "\nName: " + cellInfoS[6];
                    result += "\nSayings: \n";
                    std::vector<std::string> sayings = SplitString(cellInfoS[5], ';');
                    for (std::string saying : sayings) {
                        if (saying[0] == '>') result += "<Repeated> " + saying.substr(1) +"\n";
                        else result += saying + "\n";
                    }
                    
                }
                count++;
            }
        }
        else throw;
    }
    catch (const std::exception&)
    {
        std::cout << "Level data not found!";
    }
    return result;
}
void LevelDataManagement::ReadLevelBinaryData(std::map<unsigned int, std::map<unsigned int, std::shared_ptr<Cell>>>& cells)
{
    std::ifstream cellData("LevelData/Level1SaveFile.bin", std::ios::binary);
    try
    {
        if (cellData.is_open())
        {
            while (true) {
                unsigned int y, x, texId;
                cellData.read(reinterpret_cast<char*>(&x), sizeof(x));
                cellData.read(reinterpret_cast<char*>(&y), sizeof(y));
                cellData.read(reinterpret_cast<char*>(&texId), sizeof(texId));
                if (cellData.eof()) break;
                if (!cells.count(x)) {
                    cells.insert(std::make_pair(x, std::map<unsigned int, std::shared_ptr<Cell>>()));
                }
                if (cells[x].count(y)) {
                    printf("Duplication of cells at %i-%i\n", x, y);
                }
                cells[x].insert(std::make_pair(y, std::make_shared<Cell>(y,x, texId)));
            }
        }
    }
    catch (const std::exception&)
    {
        std::cout << "Level data not found!";
    }
}
void LevelDataManagement::CreateTextData()
{
    std::ofstream outfile("LevelData/Level1SaveFile.txt");
    for (unsigned int i = 0; i < NUMBER_OF_CELLS_Y; i++) {
        for (unsigned int j = 0; j < NUMBER_OF_CELLS_X; j++) {
            outfile << i << "," << j << "," << -1 << "\n";
        }
    }

    outfile.close();
}
void LevelDataManagement::CreateBinaryData()
{
    printf("Recreating binary data:\n");
    std::ofstream outfile("LevelData/Level1SaveFile.bin", std::ios::binary);
    unsigned int tex = 2147483647;
    for (unsigned int i = 0; i < NUMBER_OF_CELLS_Y; i++) {
        for (unsigned int j = 0; j < NUMBER_OF_CELLS_X; j++) {
            outfile.write(reinterpret_cast<const char*>(&i), sizeof(i));
            outfile.write(reinterpret_cast<const char*>(&j), sizeof(j));
            outfile.write(reinterpret_cast<const char*>(&tex), sizeof(tex));
        }
    }

    outfile.close();
	static unsigned int cellSize;
}
void LevelDataManagement::ExportEnvironmentText(std::shared_ptr<Game> game, const char* name, std::set<std::pair<unsigned int, unsigned int>> path)
{
    std::ofstream outfile("LevelData/" + std::string(name) + ".txt");
    for (unsigned int i = 0; i < game->mapv->mapSize; i++) {
        for (unsigned int j = 0; j < game->mapv->mapSize; j++) {
            std::shared_ptr<Cell> cell = game->environment->GetCell(i, j);
            outfile << j << "," << i << ",";
            std::pair<uint32_t, uint32_t> current = std::make_pair(j, i);
            if (current == game->environment->start)
                outfile << TextureInit::textureMapSToI["dungeonUpstairs"];
            else if (current == game->mapv->end)
                outfile << TextureInit::textureMapSToI["dungeonDownstairs"];
            else //if (path.find(current) != path.end())
                outfile << cell->textureId;
            //else outfile << /*TextureInit::textureMapSToI["friendly"]*/-1 << "\n";
            if (cell->enemy != nullptr) {
                outfile << "," << std::string("enemy");
                outfile << "," << std::to_string(TextureInit::textureMapSToI["elysia2"]);
                outfile << "," << std::to_string(static_cast<int>(1));// this should be float when needed
                outfile << "," << std::to_string(static_cast<int>(1));   // this should be float when needed
                outfile << "," << std::to_string(static_cast<int>(1));// this should be float when needed
                outfile << "," << cell->enemy->name;
            }
            outfile << "\n";

        }
    }
    outfile.close();
}
void LevelDataManagement::LoadEnvironmentText(std::map<unsigned int, std::map<unsigned int, std::shared_ptr<Cell>>>& cells, std::shared_ptr<Scene> scene, const char* name)
{
    std::string line;
    std::ifstream cellData("LevelData/"+std::string(name) + ".txt");
    if (cellData.is_open())
    {
        while (std::getline(cellData, line)) {
            std::vector<std::string> cellInfoS = LevelDataManagement::SplitString(line, ',');

            unsigned int x = std::stoi(cellInfoS[0]), y = std::stoi(cellInfoS[1]), texId = std::stoi(cellInfoS[2]);

            if (!cells.count(x)) {
                cells.insert(std::make_pair(x, std::map<unsigned int, std::shared_ptr<Cell>>()));
            }
            if (cells[x].count(y)) {
                printf("Duplication of cells at %i-%i\n", y, x);
            }
            cells[x].insert(std::make_pair(y, std::make_shared<Cell>(y, x, texId)));

            if (cellInfoS.size() <= 3) continue;
            std::string spec = cellInfoS[3];
            if (spec == "enemy") {
                std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(
                    LevelDataManagement::zoomCellSizes[4] * x, LevelDataManagement::zoomCellSizes[4] * y, 
                    LevelDataManagement::zoomCellSizes[4], LevelDataManagement::zoomCellSizes[4], 
                    TextureInit::textureMapIToS[std::stoi(cellInfoS[4])], 4, std::stoi(cellInfoS[5]), 
                    std::stof(cellInfoS[6]), std::stoi(cellInfoS[7]), scene, 1,cellInfoS[8]);
                if (std::shared_ptr<Edit> edit = std::dynamic_pointer_cast<Edit>(scene)) {
                    edit->enemies.emplace(cellInfoS[8], enemy);
                }
            }
            else if (spec == "npc") {
                std::vector<std::shared_ptr<Quote>> quotes;
                std::vector<std::string> quotesS = SplitString(cellInfoS[5], ';');
                for (const auto& quoteS : quotesS) {
                    quotes.push_back(std::make_shared<Quote>(quoteS.substr(1), (quoteS[0] == '>')));
                }
                std::shared_ptr<NPC> npc = std::make_shared<NPC>(LevelDataManagement::zoomCellSizes[4] * x, LevelDataManagement::zoomCellSizes[4] * y, LevelDataManagement::zoomCellSizes[4], LevelDataManagement::zoomCellSizes[4], TextureInit::textureMapIToS[std::stoi(cellInfoS[4])], 4, scene, quotes);
                if (std::shared_ptr<Edit> edit = std::dynamic_pointer_cast<Edit>(scene)) {
                    edit->npcs.emplace(cellInfoS[6], npc);
                }
            }
        }
    }
    else throw;
}
float LevelDataManagement::cellSize = 96;
const std::array<unsigned int,8> LevelDataManagement::zoomCellSizes = {6,12,24,48,96,192,384,384*2};