#include "Text.h"
#include <TextureInit/TextureInit.h>

void Text::InitializeGlyphs()
{
    int j = 1;
    for (int asciiA = 'A', i = 0, asciiB = 'a', asciiC = '0'; asciiA <= 'Z' || asciiB <= 'z' || asciiC <= '9'; i++, ++asciiA) {
        if (i == 15) {
            j++;
            i=0;
        }
        
        if (asciiA > 'Z' && asciiB > 'z') {
            glyphPos[static_cast<char>(asciiC)] = std::make_pair((asciiC - 'A' + 69 + 1 - (j - 1) * 15) * 16.0 / 240, j * 16.0 / 80);
            asciiC++; 
        }
        else if (asciiA > 'Z') {
            glyphPos[static_cast<char>(asciiB)] = std::make_pair((asciiB - 'A' - 6 + 1 - (j - 1) * 15) * 16.0 / 240, j * 16.0 / 80);
            asciiB++;
        }
        else {
            glyphPos[static_cast<char>(asciiA)] = std::make_pair((asciiA - 'A' + 1 - (j - 1) * 15) * 16.0 / 240, j * 16.0 / 80);
        }
    }
    glyphPos['!'] = std::make_pair((3) * 16.0 / 240, 5 * 16.0 / 80);
    glyphPos['?'] = std::make_pair((4) * 16.0 / 240, 5 * 16.0 / 80);
    glyphPos['.'] = std::make_pair((5) * 16.0 / 240, 5 * 16.0 / 80);
    glyphPos[','] = std::make_pair((6) * 16.0 / 240, 5 * 16.0 / 80);
    glyphPos[':'] = std::make_pair((7) * 16.0 / 240, 5 * 16.0 / 80);
    glyphPos[';'] = std::make_pair((8) * 16.0 / 240, 5 * 16.0 / 80);
    glyphPos['-'] = std::make_pair((9) * 16.0 / 240, 5 * 16.0 / 80);
    glyphPos['('] = std::make_pair((10) * 16.0 / 240, 5 * 16.0 / 80);
    glyphPos[')'] = std::make_pair((11) * 16.0 / 240, 5 * 16.0 / 80);
    glyphPos['+'] = std::make_pair((12) * 16.0 / 240, 5 * 16.0 / 80);
    glyphPos['*'] = std::make_pair((13) * 16.0 / 240, 5 * 16.0 / 80);

}

Text::Text(float spawnX, float spawnY, float width, float height, const char* text, glm::vec3 color, align alignment):
    spawnX(spawnX), spawnY(spawnY), width(width), height(height), text(std::string(text)), color(color), alignment(alignment),
	scale(1.0, 1.0, 1.0), translation(glm::vec3(0.0,0.0,0.0))
{
    InitializeGlyphs();
    float texture = TextureInit::textureMapSToI["text"];
    // ^ needs to be first
    /*for (const auto& glyph : glyphPos) {
        printf("%c - %f/%f\n",glyph.first,glyph.second.first*240,glyph.second.second*80);
    }*/
    

    std::vector<float> verts;
    for (int i = 0; text[i] != '\0'; i++) {
        char glyph = text[i], prevX = ']', prevY = ']';

        if (glyph >= 'A' && glyph <= 'O') prevY = ']';
        else if (glyph >= 'P' && glyph <= 'Z' || glyph >= 'a' && glyph <= 'd') prevY = 'A';
        else if (glyph >= 'e' && glyph <= 's') prevY = 'P';
        else if (glyph >= 't' && glyph <= 'z' || glyph >= '0' && glyph <= '7') prevY = 'e';
        switch (glyph)
        {
        case '8':
        case '9':
        case '!':
        case '?':
        case '.':
        case ',':
        case ':':
        case ';':
        case '-':
        case '(':
        case ')':
        case '+':
        case '*':
            prevY = 't';
            break;
        }

        if (glyph >= 'A' && glyph <= 'O' || glyph >= 'Q' && glyph <= 'Z' || 
            glyph >= 'b' && glyph <= 'd' || glyph >= 'f' && glyph <= 's' || 
            glyph >= 'u' && glyph <= 'z' || glyph >= '1' && glyph <= '7' || 
            glyph == '9')
                prevX = glyph - 1;

        switch (glyph)
        {
        case 'P':
        case 'e':
        case 't':
        case '8':
            prevX = ']';
            break;
        case 'a':
            prevX = 'Z';
            break;
        case '0':
            prevX = 'z';
            break;
        case '!':
            prevX = '9';
            break;
        case '?':
            prevX = '!';
            break;
        case '.':
            prevX = '?';
            break;
        case ',':
            prevX = '.';
            break;
        case ':':
            prevX = ',';
            break;
        case ';':
            prevX = ':';
            break;
        case '-':
            prevX = ';';
            break;
        case '(':
            prevX = '-';
            break;
        case ')':
            prevX = '(';
            break;
        case '+':
            prevX = ')';
            break;
        case '*':
            prevX = '+';
            break;
        }

        float offset = (i*0.6) * width;
        verts.push_back(offset + 0);            verts.push_back(0);         verts.push_back(glyphPos[prevX].first);  verts.push_back(glyphPos[prevY].second);
        verts.push_back(offset + width);        verts.push_back(0);         verts.push_back(glyphPos[glyph].first);  verts.push_back(glyphPos[prevY].second);
        verts.push_back(offset + width);        verts.push_back(height);    verts.push_back(glyphPos[glyph].first);  verts.push_back(glyphPos[glyph].second);
        verts.push_back(offset + 0);            verts.push_back(height);    verts.push_back(glyphPos[prevX].first);  verts.push_back(glyphPos[glyph].second);
    }

    std::vector<unsigned int> ind;
    for (int i = 0; i < verts.size() / 16; i++) {
        ind.push_back(4 * i);
        ind.push_back(1 + 4 * i);
        ind.push_back(2 + 4 * i);
        ind.push_back(0 + 4 * i);
        ind.push_back(2 + 4 * i);
        ind.push_back(3 + 4 * i);
    }

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(verts.data(), verts.size() * sizeof(float));
    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(2);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(ind.data(), ind.size());

    shader = std::make_unique<Shader>("res/shaders/Text.shader");
    shader->Bind();
    float characterCount = static_cast<float>(std::string(text).length());
    switch (alignment) {
    case centered:
        translation = glm::vec3(spawnX - ((characterCount)*width * 0.6) / 2 - height / 8, spawnY - height / 2, 0);
        break;
    case right:
        translation = glm::vec3(spawnX - ((characterCount)*width * 0.6) - height / 8, spawnY - height / 2, 0);
        break;
    case left:
        translation = glm::vec3(spawnX, spawnY - height / 2, 0);
        break;
    }
    //PROMENI TUK ------------------------^
    shader->SetUniformVec3f("u_Color",color);
    shader->SetUniform1i("u_TexID",texture);
}
void Text::ReinitializeObjects(const char* text) {
    this->text = std::string(text);
    float texture = TextureInit::textureMapSToI["text"];
    // ^ needs to be first
    /*for (const auto& glyph : glyphPos) {
        printf("%c - %f/%f\n",glyph.first,glyph.second.first*240,glyph.second.second*80);
    }*/
    std::vector<float> verts;
    for (int i = 0; text[i] != '\0'; i++) {
        char glyph = text[i], prevX = ']', prevY = ']';

        if (glyph >= 'A' && glyph <= 'O') prevY = ']';
        else if (glyph >= 'P' && glyph <= 'Z' || glyph >= 'a' && glyph <= 'd') prevY = 'A';
        else if (glyph >= 'e' && glyph <= 's') prevY = 'P';
        else if (glyph >= 't' && glyph <= 'z' || glyph >= '0' && glyph <= '7') prevY = 'e';
        switch (glyph)
        {
        case '8':
        case '9':
        case '!':
        case '?':
        case '.':
        case ',':
        case ':':
        case ';':
        case '-':
        case '(':
        case ')':
        case '+':
        case '*':
            prevY = 't';
            break;
        }

        if (glyph >= 'A' && glyph <= 'O' || glyph >= 'Q' && glyph <= 'Z' ||
            glyph >= 'b' && glyph <= 'd' || glyph >= 'f' && glyph <= 's' ||
            glyph >= 'u' && glyph <= 'z' || glyph >= '1' && glyph <= '7' ||
            glyph == '9')
            prevX = glyph - 1;

        switch (glyph)
        {
        case 'P':
        case 'e':
        case 't':
        case '8':
            prevX = ']';
            break;
        case 'a':
            prevX = 'Z';
            break;
        case '0':
            prevX = 'z';
            break;
        case '!':
            prevX = '9';
            break;
        case '?':
            prevX = '!';
            break;
        case '.':
            prevX = '?';
            break;
        case ',':
            prevX = '.';
            break;
        case ':':
            prevX = ',';
            break;
        case ';':
            prevX = ':';
            break;
        case '-':
            prevX = ';';
            break;
        case '(':
            prevX = '-';
            break;
        case ')':
            prevX = '(';
            break;
        case '+':
            prevX = ')';
            break;
        case '*':
            prevX = '+';
            break;
        }

        float offset = (i * 0.6) * width;
        verts.push_back(offset + 0);            verts.push_back(0);         verts.push_back(glyphPos[prevX].first);  verts.push_back(glyphPos[prevY].second);
        verts.push_back(offset + width);        verts.push_back(0);         verts.push_back(glyphPos[glyph].first);  verts.push_back(glyphPos[prevY].second);
        verts.push_back(offset + width);        verts.push_back(height);    verts.push_back(glyphPos[glyph].first);  verts.push_back(glyphPos[glyph].second);
        verts.push_back(offset + 0);            verts.push_back(height);    verts.push_back(glyphPos[prevX].first);  verts.push_back(glyphPos[glyph].second);
    }

    std::vector<unsigned int> ind;
    for (int i = 0; i < verts.size() / 16; i++) {
        ind.push_back(4 * i);
        ind.push_back(1 + 4 * i);
        ind.push_back(2 + 4 * i);
        ind.push_back(0 + 4 * i);
        ind.push_back(2 + 4 * i);
        ind.push_back(3 + 4 * i);
    }

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(verts.data(), verts.size() * sizeof(float));
    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(2);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(ind.data(), ind.size());

    float characterCount = static_cast<float>(std::string(text).length());
    switch (alignment) {
    case centered:
        translation = glm::vec3(spawnX - ((characterCount)*width * 0.6) / 2 - height / 8, spawnY - height / 2, 0);
        break;
    case right:
        translation = glm::vec3(spawnX - ((characterCount)*width * 0.6) - height / 8, spawnY - height / 2, 0);
        break;
    case left:
        translation = glm::vec3(spawnX, spawnY - height / 2, 0);
        break;
    }
}
void Text::ChangeText(const char* t)
{
    if (std::string(t) == this->text)
    {
        return;
    }
    ReinitializeObjects(t);
    //std::make_shared<Text>(textObject->spawnX, textObject->spawnY, textObject->width, textObject->height,t, textObject->color, textObject->alignment);
}
void Text::Update()
{
}
void Text::Draw()
{
    Renderer renderer;

    glm::mat4 model(1.0f);
    //model = glm::translate(model, -Mesh::cameraTranslate);
    model = glm::translate(model, translation);
    //model = glm::rotate(model, glm::radians(0.0f, glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, scale);
    //model = glm::scale(model, scaleCamera);
    mvp = Mesh::Proj * Mesh::View * model;
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(*vao, *ib, *shader);
}
