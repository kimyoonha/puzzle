#include <bangtal.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
using namespace bangtal;

class Rect {
public:
    int centerX, centerY, width, height;

    Rect(int cx, int cy, int w, int h) : centerX(cx), centerY(cy), width(w), height(h) {}; //생성자 함수에 의해 초기화.

    bool checkin(int x, int y) { //x, y가 cx, cy에 들어가는가
        return (x > centerX - width) && (x < centerX + width) && (y > centerY - height) && (y < centerY + height);
    }
};

int main()
{
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
    std::srand((unsigned int)time(NULL));

    auto scene = Scene::create("퍼즐맞추기", "Images/version1/full.png");
    auto start = Object::create("Images/start.png", scene, 640, 360, true);
    auto change = Object::create("Images/change.png", scene, 10, 10, true);
 
    const int nums_of_pictures = 9;
    const int WIDTH = 150;
    const int HEIGHT = 100;
    int version = 1;
    bool proceeding = false;

    std::string routes_one[nums_of_pictures + 1] = {
    "",
    "Images/version1/1.png",
    "Images/version1/2.png",
    "Images/version1/3.png",
    "Images/version1/4.png",
    "Images/version1/5.png",
    "Images/version1/6.png",
    "Images/version1/7.png",
    "Images/version1/8.png",
    "Images/version1/empty.png",
    };

    std::string routes_two[nums_of_pictures + 1] = {
    "",
    "Images/version2/1.png",
    "Images/version2/2.png",
    "Images/version2/3.png",
    "Images/version2/4.png",
    "Images/version2/5.png",
    "Images/version2/6.png",
    "Images/version2/7.png",
    "Images/version2/8.png",
    "Images/version2/empty.png",
    };

    std::string routes[nums_of_pictures + 1];
    for (int i = 0; i <= nums_of_pictures; i++) {
        routes[i] = routes_one[i];
    }


    Rect rect[nums_of_pictures + 1] = {
         {0, 0, 0, 0},
         {340, 560, WIDTH, HEIGHT},
         {640, 560, WIDTH, HEIGHT},
         {940, 560, WIDTH, HEIGHT},
       {340, 360, WIDTH, HEIGHT},
       {640, 360, WIDTH, HEIGHT},
       {940, 360, WIDTH, HEIGHT},
         {340, 160, WIDTH, HEIGHT},
         {640, 160, WIDTH, HEIGHT},
         {940, 160, WIDTH, HEIGHT},
    };

    ObjectPtr image[3][3];
    int which[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) which[i][j] = -1;
    }
    for (int i = 1; i <= nums_of_pictures; ++i) {
        int x = (i - 1) / 3;
        int y = (i - 1) % 3;
        image[x][y] = Object::create(routes[i], scene);
        image[x][y]->hide();
    }
    start->setOnMouseCallback([&](ObjectPtr object, int m, int n, MouseAction action) -> bool {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) which[i][j] = -1;
        }
        for (int i = 1; i <= nums_of_pictures; ++i) {
            int num, x, y; // 들어갈 위치
            do {
                num = std::rand() % 9;
                x = num / 3;
                y = num % 3;
                if (which[x][y] == -1) break;
            } while (true);
            which[x][y] = i;
            if (i == 9) {
                which[x][y] = 0;
            }
            image[x][y]->setImage(routes[i]);
            image[x][y]->locate(scene, rect[num + 1].centerX - rect[num + 1].width, rect[num + 1].centerY - rect[num + 1].height);
            image[x][y]->show();
        }
        start->hide();
        proceeding = true;
        return true;

    });

    int count = 0;

    for (int k = 0; k < 3; k++) {
        for (int w = 0; w < 3; w++) {
            image[k][w]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
                bool checked = false;
                for (int m = 0; m < 3; m++) {
                    for (int n = 0; n < 3; n++) {
                        if (image[m][n] == object) {
                            int empty_x = -1;
                            int empty_y = -1;
                            for (int j = -1; j <= 1; j++) {
                                if (m + j >= 0 && m + j < 3) {
                                    if (which[m + j][n] == 0) {
                                        empty_x = m + j;
                                        empty_y = n;
                                        break;
                                    }
                                }
                                if (n + j >= 0 && n + j < 3) {
                                    if (which[m][n + j] == 0) {
                                        empty_x = m;
                                        empty_y = n + j;
                                        break;
                                    }
                                }
                            }
                            if (empty_x == -1 || (empty_x == m && empty_y == n)) break;
                            int empty = empty_x * 3 + empty_y + 1;
                            int i = m * 3 + n + 1;
                            image[empty_x][empty_y]->locate(scene, rect[i].centerX - rect[i].width, rect[i].centerY - rect[i].height);
                            image[m][n]->locate(scene, rect[empty].centerX - rect[empty].width, rect[empty].centerY - rect[empty].height);
                            image[empty_x][empty_y]->show();
                            image[m][n]->show();
                            ObjectPtr temp = image[empty_x][empty_y];
                            image[empty_x][empty_y] = image[m][n];
                            image[m][n] = temp;

                            which[empty_x][empty_y] = which[m][n];
                            which[m][n] = 0;
                            checked = true;
                            break;

                        }

                    }
                    if (checked) break;
                }

                count = 0;

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int num = i * 3 + j + 1;

                        if (which[i][j] == num) count++;
                    }
                }

                if (count == nums_of_pictures - 1) {
                    showMessage("성공, 다 맞췄다!!!");
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) image[i][j]->hide();
                    }
                    start->show();
                    proceeding = false;
                }



                return true;
            });
        }
    }

    change->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action) -> bool {
        if (proceeding) return true;

        if (version == 1) {
            version = 2;
            for (int i = 0; i <= nums_of_pictures; i++) routes[i] = routes_two[i];
            scene->setImage("Images/version2/full.png");
        }
        else {
            version = 1;
            for (int i = 0; i <= nums_of_pictures; i++) routes[i] = routes_one[i];
            scene->setImage("Images/version1/full.png");
        }

    });


    startGame(scene);



    return 0;
}