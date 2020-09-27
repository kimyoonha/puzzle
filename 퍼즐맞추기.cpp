#include <bangtal.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <iostream>
using namespace bangtal;
class Rect {
public:
    int x, y;

    Rect(int cx, int cy) : x(cx), y(cy) {}; //생성자 함수에 의해 초기화.
};

void swap(ObjectPtr a, ObjectPtr b, ScenePtr scene, int coordinates[]) {
    int x = coordinates[0];
    int y = coordinates[1];
    int empty_x = coordinates[2];
    int empty_y = coordinates[3];
    a->locate(scene, x, y);
    b->locate(scene, empty_x, empty_y);
    a->show();
    b->show();

}

int main()
{
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
    std::srand((unsigned int)time(NULL));

    auto scene = Scene::create("퍼즐맞추기", "Images/version1/full.png");
    auto start = Object::create("Images/start.png", scene, 640, 360, true);
    auto change = Object::create("Images/change.png", scene, 10, 10, true);
    auto timer = Timer::create(0.1f);

    const int nums_of_pictures = 9;
    int version = 1;
    bool proceeding = false;
    time_t startTime, endTime;
    time_t maxTime = 1 << 30 - 1;
    int times = 1 << 30;


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
         {0, 0 },
         {190, 460},
         {490, 460},
         {790, 460},
       {190, 260},
       {490, 260},
       {790, 260},
         {190, 60},
         {490, 60},
         {790, 60},
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
            int num = i - 1, x, y;
            x = num / 3;
            y = num % 3;
            which[x][y] = i != 9 ? i : 0;

            image[x][y]->setImage(routes[i]);
            image[x][y]->locate(scene, rect[i].x, rect[i].y);
            image[x][y]->show();
        }
        times = 50;

        timer->setOnTimerCallback([&](TimerPtr ptr)->bool {
            times -= 1;
            if (times == 0) return true;

            int x = rand() % 3;
            int y = rand() % 3;

            int empty_x = -1;
            int empty_y = -1;

            for (int j = -1; j <= 1; j += 2) {
                if (x + j >= 0 && x + j <= 2) {
                    if (which[x + j][y] == 0) {
                        empty_x = x + j;
                        empty_y = y;
                        break;
                    }
                }
                if (y + j >= 0 && y + j <= 2) {
                    if (which[x][y + j] == 0) {
                        empty_x = x;
                        empty_y = y + j;
                        break;
                    }
                }
            }

            if (empty_x != -1) {

                int empty = empty_x * 3 + empty_y + 1;
                int i = x * 3 + y + 1;

                int coordinates[4] = { rect[i].x, rect[i].y, rect[empty].x, rect[empty].y };

                swap(image[empty_x][empty_y], image[x][y], scene, coordinates);

                ObjectPtr temp = image[empty_x][empty_y];
                image[empty_x][empty_y] = image[x][y];
                image[x][y] = temp;
                which[empty_x][empty_y] = which[x][y];
                which[x][y] = 0;
            }

            timer->set(0.1f);
            timer->start();

            return true;
        });

        timer->start();
        start->hide();
        proceeding = true;
        startTime = time(NULL);
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

                            int coordinates[4] = { rect[i].x, rect[i].y, rect[empty].x, rect[empty].y };

                            swap(image[empty_x][empty_y], image[m][n], scene, coordinates);

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
                    endTime = time(NULL);
                    if (maxTime > difftime(endTime, startTime)) maxTime = difftime(endTime, startTime);
                    std::string buf = std::to_string(maxTime) + "초가 최고기록입니다!";
                    showMessage(buf.c_str());
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