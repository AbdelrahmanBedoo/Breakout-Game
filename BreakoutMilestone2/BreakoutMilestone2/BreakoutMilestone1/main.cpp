#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <cmath>
#include <QDebug>
#include <QGraphicsTextItem> // Include for text item
#include <QTime> // Include for delay
#include "Sphere.h"
#include "Player.h"
#include "brick.h"
#include "dispatch.h"

// Function to create a QGraphicsTextItem with specified properties
QGraphicsTextItem* type_msg(QString msg, int size, QColor clr, int x, int y)
{
    // Create a new QGraphicsTextItem with the given message
    QGraphicsTextItem* msgItem = new QGraphicsTextItem(msg);

    // Set font properties
    QFont font;
    font.setPixelSize(size);
    msgItem->setFont(font);

    // Set text color
    msgItem->setDefaultTextColor(clr);

    // Set position relative to the bounding rectangle
    msgItem->setPos(x - (msgItem->boundingRect().width() / 2), (y - (msgItem->boundingRect().height() / 2)));

    return msgItem;
}

// Function to create bricks for a level and add them to the scene
void create_level(int screen_w, int screen_h, QGraphicsScene& scene, int level, int factor, QList<Brick*>& aliveBricks, QList<Brick*>& destroyedBricks)
{
    // Calculate number of bricks and other parameters based on level
    int n_bricks = level * factor, n_rows = level, margin = 20;
    int bricks_per_row = n_bricks / n_rows;
    int brick_w = (screen_w - (2 + bricks_per_row - 1) * margin) / bricks_per_row;
    int brick_h = (screen_h - (n_rows - 1) * margin) / 2 / n_rows;
    QColor brick_clr = QColor(20, 20, 140);
    int cursor_y = margin;

    // Loop to create bricks and add them to the scene
    for (int i = 0; i < n_rows; i++) {
        int cursor_x = margin;
        for (int j = 0; j < bricks_per_row; j++) {
            Brick* new_brick = new Brick(cursor_x, cursor_y, brick_w, brick_h, brick_clr);
            aliveBricks.append(new_brick);
            scene.addItem(new_brick);
            cursor_x += margin + brick_w;
        }
        cursor_y += margin + brick_h;
    }

    // Clear list of destroyed bricks
    destroyedBricks.clear();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Set screen dimensions
    int screen_w(1000), screen_h(700);

    // Create a QGraphicsView
    QGraphicsView view;
    view.setFixedSize(screen_w, screen_h);
    QColor background = QColor(255, 255, 255);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setBackgroundBrush(background);

    // Set frame duration for the game timer
    int frame_duration = 25;

    // Create a QGraphicsScene
    QGraphicsScene scene;
    scene.setSceneRect(0, 0, screen_w, screen_h);

    // Create and initialize a dispatch object
    Dispatch dispatch;
    dispatch.setFlag(QGraphicsItem::ItemIsFocusable);
    dispatch.setFocus();
    scene.addItem(&dispatch);

    // Initialize game variables
    int score = 0, health = 3, level = 1, factor = 5, max_level = 5;
    double speed_increment = 2.5, init_player_speed(5), init_sphere_speed(7.5);
    int player_w = 120, player_h = 30, upgraded_player_w = 200;
    int upgrade_1 = 20, upgrade_2 = 40, upgrade_3 = 1;
    bool upgraded_1 = false, upgraded_2 = false, upgraded_3 = false;
    bool disabled = false;

    // Create lists to store bricks
    QList<Brick*> destroyedBricks;
    QList<Brick*> aliveBricks;

    // Create and add player and sphere objects to the scene
    Player* player = new Player(player_w, player_h, QColor(215, 214, 213), init_player_speed);
    Player* player2;
    player->setPos(screen_w / 2 - player_w / 2, screen_h - 3 * player_h);
    scene.addItem(player);
    int radius = 20;
    Sphere* sphere = new Sphere(radius, QColor(53, 50, 47), screen_w / 2 , 2 * (screen_h / 3.0), init_sphere_speed);
    scene.addItem(sphere);

    // Set the scene for the view and display
    view.setScene(&scene);
    view.show();

    // Create text items for score and health
    QGraphicsTextItem* scoreText = type_msg("Score: 0", 20, Qt::black, screen_w - 300, screen_h - 25);
    scene.addItem(scoreText);
    QGraphicsTextItem* healthText = type_msg("Health: " + QString::number(health), 20, Qt::black, screen_w - 150, screen_h - 25);
    scene.addItem(healthText);

    // Create a timer to update the game state
    QTimer timer;

    // Initialize variables for better collision detection and game state management
    int sphere_x_last_frame, sphere_y_last_frame;
    bool paused = true, count_down_running = true, display_upgrade = false;
    int pause_time = 1000, curr_count = 4;

    // Create text items for countdown, level cleared, health gained, and upgrade messages
    QGraphicsTextItem* count_down = type_msg(QString :: number (curr_count), 40, Qt::white, screen_w / 2, 2 * (screen_h / 3.0));
    QGraphicsTextItem* level_cleared = type_msg("Level " + QString :: number (level) + " cleared!", 40, Qt::black, screen_w / 2, screen_h / 2);
    QGraphicsTextItem* health_gained = type_msg("You have gained an extra health!", 20, Qt::black, screen_w / 2, screen_h / 2 + 40);
    QGraphicsTextItem* upgrade_msg = type_msg("Upgrade 1 Unlocked: Increased Size!", 20, Qt::black, screen_w * 0.25, screen_h - 25);
    scene.addItem(count_down);
    scene.addItem(level_cleared);
    scene.addItem(health_gained);
    scene.addItem(upgrade_msg);
    level_cleared->setPlainText("");
    health_gained->setPlainText("");
    upgrade_msg->setPlainText("");

    // Create initial level
    create_level(screen_w, screen_h, scene, level, factor, aliveBricks, destroyedBricks);

    // Connect timer timeout signal to game logic
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        if(paused)
        {
            // Check if countdown is running
            if (count_down_running)
            {
                // Update countdown
                if (pause_time >= 1000)
                {
                    if (--curr_count)
                    {
                        count_down->setPlainText(QString :: number (curr_count));
                    }
                    else
                    {
                        count_down->setPlainText("");
                        paused = false;
                        count_down_running = false;
                        curr_count = 4;
                    }
                    pause_time = 0;
                }
                pause_time += frame_duration;
            }
            // Check if upgrade message is being displayed
            else if (display_upgrade)
            {
                if (pause_time >= 3000)
                {
                    upgrade_msg->setPlainText("");
                    pause_time = 0;
                    paused = false;
                    display_upgrade = false;
                }
                pause_time += frame_duration;
            }
            // Handle level transition
            else
            {
                if (pause_time >= 3000)
                {
                    level_cleared->setPlainText("");
                    health_gained->setPlainText("");
                    create_level(screen_w, screen_h, scene, level, factor, aliveBricks, destroyedBricks);
                    count_down_running = true;
                    pause_time = 0;
                }
                pause_time += frame_duration;
            }
        }
        else
        {
            // Store last frame position of the sphere
            sphere_x_last_frame = sphere->x();
            sphere_y_last_frame = sphere->y();

            // Move players and sphere
            player->move(screen_w, dispatch.last_key);
            if (upgraded_3) player2->move(screen_w, dispatch.last_key);
            sphere->move();

            // Check for collisions with walls
            if (sphere->x() - radius < 0) {
                sphere->reflect_horz();  // Reflect angle on collision with side walls
            }
            else if (sphere->x() + radius > screen_w)
            {
                sphere->reflect_horz();  // Reflect angle on collision with side walls
            }
            else if (sphere->y() - radius < 0)
            {
                sphere->reflect_vert();  // Reflect angle on collision with top wall
            }
            else if (sphere->y() > screen_h)
            {
                // Ball falls
                health--;
                healthText->setPlainText("Health: " + QString::number(health));  // Update health text
                sphere->setPos(screen_w / 2 , 2 * (screen_h / 3.0)); // Reset ball position
                sphere->angle = M_PI_4;
                player->setX(3 * (screen_w / 4) -  player->w);
                player->moving = false;
                player2->setX(screen_w / 4 -  player2->w);
                player2->moving = false;
                destroyedBricks.clear(); // Clear list of destroyed bricks
                disabled = false;
            }

            // Check for collision between ball and players
            if (!disabled) {
                if (sphere->collidesWithItem(player))
                    disabled = player->disabled_after_collide(sphere, sphere_y_last_frame);
                else if (upgraded_3 && sphere->collidesWithItem(player2))
                    disabled = player2->disabled_after_collide(sphere, sphere_y_last_frame);
            }

            // Check for collision with bricks
            QList<Brick*>::Iterator it = aliveBricks.begin();
            while (it != aliveBricks.end())
            {
                if (sphere->collidesWithItem(*it))
                    break;
                it++;
            }

            if (it != aliveBricks.end())
            {
                // Remove destroyed brick from scene
                scene.removeItem(*it);
                score++;
                scoreText->setPlainText("Score: " + QString::number(score));  // Update score text
                destroyedBricks.append(*it);
                if (sphere_x_last_frame + radius >= (*it)->x_pos && sphere_x_last_frame - radius <= (*it)->x_pos + (*it)->w)
                    sphere->reflect_vert();
                else
                    sphere->reflect_horz();
                delete *it;
                aliveBricks.erase(it);
                // Check for score-based upgrades
                if (score >= upgrade_1 && !upgraded_1)
                {
                    upgraded_1 = true;
                    int old_x = player->x();
                    scene.removeItem(player);
                    delete player;
                    player = new Player(upgraded_player_w, player_h, QColor(215, 214, 213), init_player_speed);
                    player->setPos(old_x - (upgraded_player_w - player_w) / 2, screen_h - 3 * player_h);
                    scene.addItem(player);
                    paused = true;
                    display_upgrade = true;
                    pause_time = 0;
                    upgrade_msg->setPlainText("Upgrade 1 Unlocked: Increased Size!");
                }
                else if (score >= upgrade_2 && !upgraded_2)
                {
                    upgraded_2 = true;
                    paused = true;
                    display_upgrade = true;
                    pause_time = 0;
                    upgrade_msg->setPlainText("Upgrade 2 Unlocked: Machine Gun!");
                }
                else if (score >= upgrade_3 && !upgraded_3)
                {
                    upgraded_3 = true;
                    int new_x = player->x() > screen_w / 2 ? 10: screen_w - player_w - 10;
                    player2 = new Player(player_w, player_h, QColor(229, 184, 11), player->speed, Qt::Key_A, Qt::Key_D);
                    player2->setPos(new_x, screen_h - 3 * player_h);
                    scene.addItem(player2);
                    paused = true;
                    display_upgrade = true;
                    pause_time = 0;
                    upgrade_msg->setPlainText("Upgrade 3 Unlocked: Accomplice!");
                }
            }
            // Check if all bricks are destroyed
            if (!aliveBricks.size()) {
                level++;
                if(level <= max_level)
                {
                    level_cleared->setPlainText("Level " + QString :: number (level) + " cleared!");
                    health_gained->setPlainText("You have gained an extra health!");
                }

                health++;
                healthText->setPlainText("Health: " + QString::number(health));  // Update health text
                player->speed += speed_increment / 2;
                player->setX(3 * (screen_w / 4) - player->w);
                player->moving = false;
                if(upgraded_3)
                {
                    player2->speed += speed_increment / 2;
                    player2->setX(screen_w / 4 - player2->w);
                    player2->moving = false;
                }
                sphere->setPos(screen_w / 2 , 2 * (screen_h / 3.0));
                sphere->angle = M_PI_4;
                sphere->speed += speed_increment;
                paused = true;
                count_down_running = false;
            }

            // Check for game over conditions
            if (health == 0 || level > max_level) {
                timer.stop();
                QString msg = (health == 0) ? "Game Over!" : "You Won!";
                qDebug() << msg;
                QGraphicsTextItem* msgItem = type_msg(msg, 40, Qt::black, screen_w / 2, screen_h / 2);
                QGraphicsTextItem* final_score = type_msg("Final Score: " + QString::number(score), 20, Qt::black, screen_w / 2, screen_h / 2 + 40);
                scene.addItem(msgItem);
                scene.addItem(final_score);
                // Timer to close the application after 5 seconds
                QTimer::singleShot(5000, [&]() {
                    view.close();
                });
                return;
            }
        }
    });

    // Start the game timer
    timer.start(frame_duration);  // Update the game state every 25 milliseconds (40 FPS)

    return a.exec();
}
